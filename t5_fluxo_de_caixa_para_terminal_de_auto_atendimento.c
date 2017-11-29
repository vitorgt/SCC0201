//11horas

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct _ll{//lista ligada
	int t, b, d;//terminal, banco, banco de destino
	char o, e;//operacao, erro
	float v;//valor
	struct _ll *n;//proxima celula
}ll;

typedef struct _s{//estutura de soma
	int id, qt, au;//numero do banco, quantidade de opecacoes, qtd de auditorias
	float mp, mn, tp, tn;//moeda positivo, moeda negativo, transferencia p, tn
}sum;

typedef struct _e{//estrutura de contagem de erro
	int S, D, C, T;//erros de Saque, Deposito, Consultas e Transferencias
}err;

void clearll(ll *a){//limpa celulas de lista ligada
	a->t = 0;
	a->b = 0;
	a->d = 0;
	a->o = 0;
	a->e = 0;
	a->v = 0;
	a->n = NULL;
}

void clearsum(sum *b){//limpa estrutura de soma
	b->id = 0;
	b->qt = 0;
	b->au = 0;
	b->mp = 0;
	b->mn = 0;
	b->tp = 0;
	b->tn = 0;
}

int hashmap(char o, int t){//funcao de hash
	return 5*t-5+((o == 'S') ? 0 : ((o == 'D') ? 1 : ((o == 'C') ? 2 : ((o == 'T') ? 3 : ((o == 'A') ? 4 : -1)))));
}

void insertHASH(ll **hash, ll *in){//recebe o vetor de enderessos (aka hash) e o enderesso da celula que sera inserida
	int p = hashmap(in->o, in->t);//mapeia para onde essa celula vai
	ll *set = NULL, *aux = NULL;
	set = (ll *)malloc(sizeof(ll));//alloco uma nova celula, pois essa in sera resetada na proxima iteracao da main
	clearll(set);//limpo a nova celula
	set->t = in->t;//copio as informacoes dos campos
	set->b = in->b;
	set->d = in->d;
	set->o = in->o;
	set->e = in->e;
	set->v = in->v;
	set->n = NULL;
	aux  = hash[p];//aux recee o enderesso inicial para a hash
	if(!aux)//se nao tiver ocupada
		hash[p] = set;//ela recebe esse enderesso
	else{
		while(aux->n)//trata colisoes por encadeamento parcorrendo ate achar a celula que tenha proximo nulo
			aux = aux->n;
		aux->n = set;//o proximo agora eh a nova celula
	}
}

void printH(ll **hash){
	int i = 0;
	for(; i < 20; i++){
		ll *aux = hash[i];
		while(aux){
			printf("|%p|\t%d\t%3d\t%c\t%3d\t%8.2f\t%d\t%9p | %02d\n", (void *)aux, aux->t, aux->b, aux->o, aux->d, aux->v, (int)aux->e, (void*)aux->n, hashmap(aux->o, aux->t));
			aux = aux->n;
		}
	}
}

void freeHash(ll **h, sum *bank){//libera as alocacoes
	int i = 0;
	ll *a = NULL, *b = NULL;
	for(i = 0; i < 20; i++){
		a = h[i];//a recebe o primeiro ponteiro da linha
		while(a){//itera
			b = a->n;//b guarda o proximo
			free(a);
			a = b;//a recebe o proximo
		}
	}
	free(h);
	free(bank);
}

void defbanks(ll **h, sum *b){
	int bb[4], i = 0, j = 0;
	ll *a = NULL;
	for(i = 0; i < 4; i++)//limpo o vetor
		bb[i] = 0;
	for(i = 0; i < 20; i++){//percorro toda a hash
		a = h[i];//a recebe o ponteiro inicial da linha
		while(a){
			for(j = 0; bb[j] && bb[j] != a->b && j < 4; j++);//enquanto bb[j] ja tiver um id associado e esse id nao for o que tem na linha vai para o proximo
			if(i%5 != 4)//ignora as autidorias pois os bancos dela carregam outras informaces e nao ids de bancos
				bb[j] = a->b;
			a = a->n;//a vai para a proxima celula
		}
	}
	for(i = 0; i < 4; i++)//bubble sort para ordenar os id de bancos
		for(j = i; j < 4; j++)
			if(bb[i] > bb[j]){
				int temp = bb[i];
				bb[i] = bb[j];
				bb[j] = temp;
			}
	for(i = 0; i < 4; i++)
		(b+i)->id = bb[i];//a estrutura de somas recebe os id ordenados
}

void accsum(ll **h, sum *b, int i, err *e){//soma acumulada
	int max  = i + 5, j = 0;//o maximo recebe cinco linhas a mais que o i inicial passado
	if(i == 20){//se i for 20, fara a soma acumulada de toda a hash
		i = 0;
		max = 20;
	}
	for(; i < max; i++){
		if(i%5 != 4){//se nao for auditoria
			ll *a = h[i];//a recebe o ponteiro inicial da linha
			while(a){//enquanto a nao for nulo
				if(!(a->e)){//se nao for erro
					for(j = 0; (b+j)->id != a->b && j < 4; j++);//acha o id do banco
					if(i%5 == 0)//se for Saque
						(b+j)->mn += a->v;
					else if(i%5 == 1)//se for Deposito
						(b+j)->mp += a->v;
					else if(i%5 == 3){//se for transferencia
						if(a->b != a->n->b){//se forem bancos diferentes (a celula atual e a proxima)
							(b+j)->tn += a->v;//o banco j recebe o valor do atual
							a = a->n;//a recebe a proxima celula
							for(j = 0; (b+j)->id && (b+j)->id != a->b; j++);//procuro o j do banco dessa nova celula
							(b+j)->tp += a->v;//o banco do novo j recebe o valor da celula atual
						}
						else//se fortransferencia interna (para o mesmo banco)
							a = a->n;//a recebe a proxima celula
					}
					++(b->qt);//quantidade de operacoes++
				}
				else{//se for erro
					if(a->o == 'S')//se for Saque
						(e->S)++;//erros de Saque++
					else if(a->o == 'D')//analogo
						(e->D)++;//analogo
					else if(a->o == 'C')//analogo
						(e->C)++;//analogo
					else if(a->o == 'T')//analogo
						(e->T)++;//analogo
				}
				a = a->n;//a recebe a proxima celula
			}
		}
		else//de for auditoria
			(b->au)++;//quantidade de auditoria++
	}
	e->T = e->T/2;//quantidade de erros de transferencias acabam sendo constadas duas vezes, entao divide por 2
}

void audit(ll **h){//auditora a hash
	int i = 0;
	ll *a = h[4], *b = NULL;//a recebe o enderesso inicial que estiver na linha 4 da hash
	if(a){//se tiver auditorias
		printf("===AUDITORIA===\n");
		while(a){//enquanto a != NULL
			if(a->v == 'S'){//se for saque
				printf("===SAQUE TERMINAL %d===\nMostrando primeiros %d resultados\n", a->b, a->d);
				b = h[hashmap((char)a->v, a->b)];//b recebe a linha do hashmap dessa operacao
				if(b)//se tiver operacoes 
					for(i = 0; b && i < a->d; i++){//enquanto b for diferente de nulo e i < quantidade pedida para auditoria
						if(!(b->e))//se essa operacao nao for erro
							printf("%d- Banco %d %.2f\n", i+1, b->b, fabs(b->v));//mostra
						else//se for erro
							i--;
						b = b->n;//b vai para a proxima celula
					}
				else//se nao tiver nenhuma operacao a ser mostrada
					printf("Sem resultados\n");
			}
			else if(a->v == 'D'){//se for deposito
				printf("===DEPOSITO TERMINAL %d===\nMostrando primeiros %d resultados\n", a->b, a->d);
				b = h[hashmap((char)a->v, a->b)];//b recebe a linha do hashmap dessa operacao
				if(b)//se tiver operacoes 
					for(i = 0; b && i < a->d; i++){//enquanto b for diferente de nulo e i < quantidade pedida para auditoria
						if(!(b->e))//se essa operacao nao for erro
							printf("%d- Banco %d %.2f\n", i+1, b->b, fabs(b->v));//mostra
						else//se for erro
							i--;
						b = b->n;//b vai para a proxima celula
					}
				else//se nao tiver nenhuma operacao a ser mostrada
					printf("Sem resultados\n");
			}
			else if(a->v == 'T'){//se for transferencia
				printf("===TRANSFERENCIA TERMINAL %d===\nMostrando primeiros %d resultados\n", a->b, a->d);
				b = h[hashmap((char)a->v, a->b)];//b recebe a linha do hashmap dessa operacao
				if(b)//se tiver operacoes 
					for(i = 0; b && i < a->d; i++){//enquanto b for diferente de nulo e i < quantidade pedida para auditoria
						if(!(b->e))//se essa operacao nao for erro
							printf("%d- Banco origem %d Banco destino %d %.2f\n", i+1, b->b, b->d, fabs(b->v));//mostra
						else//se for erro
							i--;
						b = b->n;//b vai duas celulas para frente
						b = b->n;
					}
				else//se nao tiver nenhuma operacao a ser mostrada
					printf("Sem resultados\n");
			}
			a = a->n;//a vai para a proxima auditoria
		}
	}
}

int main(){
	int i = 0, j = 0, k = 0;
	char c = 0, buffer[100] = {0};
	float f = 0;

	ll reader, **hash = NULL;//defino a celula que fara a leitura e vetor de enderessos (hash)
	sum *banks = NULL;//estrutura de somas
	err e; e.S = 0; e.D = 0; e.C = 0; e.T = 0;//estrutura de contagem de erros

	hash = (ll **)malloc(sizeof(ll *)*20);
	banks = (sum *)malloc(sizeof(sum)*4);

	for(i = 0; i < 20; i++) hash[i] = NULL;//define todas as primeiras posicoes da hash NULL
	while(!feof(stdin)){
		clearll(&reader);//limpa a celula de leitura
		c = 0; f = 0;
		for(i = 0; i < 100; i++) buffer[i] = 0;//limpa o buffer
		scanf("%d ", &(reader.t));//le o terminal
		c = getchar();
		if(c >= '0' && c <= '9'){//se for nuemro
			ungetc(c, stdin);
			scanf("%d ", &(reader.b));//le para o campo banco da celula de leitura
			reader.o = getchar();//operador da celula de leitura recebe a operacao
			if(reader.o == 'C')//se for C
				while(c != '\n')//termina de ler os caracteres ocultos da linha
					c = getchar();
			else{//se for S, D ou T
				scanf("%f", &f);//escaneia com float pois pode ser um inteiro (banco de destino) ou um float (valor)
				if((c = getchar()) == ' '){//se depois disso vier um espaco
					reader.d = (int)f;//foi lido um banco de destino entao o bando de destino da celula de leitura recebe o inteiro de f
					scanf("%f\n", &reader.v);//o valor da celula de leitura recebe a proxima leitura
				}
				else{//se nao for um ' '
					if(reader.o == 'T')//se a operacao for uma  transferencia
						reader.d = reader.b;//copio o banco de origem para o banco de destino ja que nao foi especificado um banco de destino eh uma transferencia interna
					while(c == '\r'){//consome caracteres ocultos
						c = getchar();
					}
					reader.v = f;//o valor da celula de leitura recebe f
				}
			}
		}
		else if(c == 'S' || c == 'D' || c == 'C' || c == 'T'){//ler teminal e char == AUDITORIA
			reader.o = 'A';//operador da celula de leitura recebe 'A'
			reader.v = c;//valor da celula de leitura recebe o char da operacao
			reader.b = reader.t;//o banco da celula de leitura recebe o terminal
			reader.t = 1;//o terminal recebe 1
			scanf(" %d\n", &reader.d);//escaneia a quantidade de operacoes pedidas para o campo do banco de destino da celula de leitura
		}
		for(i = 0; (c = getchar()) && i < 100; i++){//percorro os proximos 100 chars procurando por 'ERRO'
			buffer[i] = c;//armazena em um baffuer
			if(c == 'E'){//se encontrar erro
				reader.e = 1;
				while(1){
					c = getchar();//consome todos os caracteres ate encontrar um destes tres
					if(c == 0 || c == '\n' || c == EOF)
						break;
				}
			}
			if(c == 0 || c == '\n' || c == EOF)//se nao encontrar erro e terminar a linha
				break;
		}
		if(!reader.e) for(; i >= 0; i--) ungetc(buffer[i], stdin);//devolve os caracteres lidos para o stdin
		if(reader.o == 'S') reader.v = -reader.v;//se a operacao for saque inverte o sinal do valor
		if(reader.o == 'T'){//se for transferencia
			reader.v = -reader.v;//inverte o sinal do valor
			insertHASH(hash, &reader);//insere a celula lida na hahs
			reader.v = -reader.v;//inverte de novo o sinal do valor
			int temp = reader.b;//troca os bancos de destino e origem
			reader.b = reader.d;
			reader.d = temp;
			insertHASH(hash, &reader);//insere essa nova celula modificada na hahs
		}
		else
			insertHASH(hash, &reader);//insere a celula na hash
	}
	//printH(hash);
	for(i = 0; i < 5; i++){
		for(k = 0; k < 4; k++) clearsum(banks+k);//limpa a soma acumulada
		e.S = 0; e.D = 0; e.C = 0; e.T = 0;//limpa a estrutura de contagem de erros
		defbanks(hash, banks);//define os bancos que existem e ordena eles
		if(i != 4)
			printf("===TERMINAL %d===\n", i+1);
		else
			printf("===TOTAL===\n");
		accsum(hash, banks, i * 5, &e);//faz a soma acumulada da hash para cada terminal
		for(j = 0; j < 4; j++){
			if(banks[j].mp || banks[j].mn || banks[j].tp || banks[j].tn)//se tiver algum valor mostra senao nao
				printf("Banco %d: Moeda +%.2f -%.2f Transferencia +%.2f -%.2f\n", banks[j].id, banks[j].mp, fabs(banks[j].mn), banks[j].tp, fabs(banks[j].tn));
		}
		printf("Lucro obtido: %d.00\n", banks[0].qt*3);
		if(e.S)//se tiverem erros
			printf("Erros de saque: %d\n", e.S);
		if(e.D)
			printf("Erros de deposito: %d\n", e.D);
		if(e.C)
			printf("Erros de consulta: %d\n", e.C);
		if(e.T)
			printf("Erros de transferencia: %d\n", e.T);
		if(e.S || e.D || e.C || e.T)
			printf("Total de erros: %d\n", e.S + e.D + e.C + e.T);
	}
	if(banks[0].au){//se tiverem auditorias
		audit(hash);
	}
	freeHash(hash, banks);//libera as alocacoes dinamicas
	return 0;
}
