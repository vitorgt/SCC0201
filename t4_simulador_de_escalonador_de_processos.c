#include<stdio.h>
#include<stdlib.h>

#define MAXV 150

typedef struct _proc{
	int id, bg, qt, rk;//id, begging at, quantity, priority
}proc;

void clearproc(proc *a){
	a->id = -1;
	a->bg = -1;
	a->qt = -1;
	a->rk = -1;
}

void allocM(int ***m, int l, int c){//alocacao dinamica de matriz
	int i = 0;
	*m = (int **)malloc(sizeof(int *) * l);
	for(; i < l; i++)
		*(*m + i) = (int *)calloc(c, sizeof(int) * c);
}

int validID(int **m, int id, int lines){
	int i = 0, flag = 1, count = 0;
	while(flag){
		for(i = 0, count = 0; i < lines; i++){//prcorro a matriz ate a linha lida
			if(m[i][0] == id)//se achar algum id igual aumenta o id e nao aumanta o count
				id++;
			else
				count++;
		}
		if(count == lines)
			/*se achar algum id igual, o count e o numero de linhas vai ser diferente
			  entao preciso repercorrer a matriz para ver se o novo id eh valido
			  quando tiver percorrido toda a matriz e nao tiver alterado o id ele eh valido
			  e pode sair do while
			 */
			flag = 0;
	}
	return id;
}

void insereproc(int **m, int i, proc **r, int lim, int *qzero){//insiro processos da matriz para o vetor de processos
	int j = 0;
	for(; j < lim; j++)
		if((*r + j)->id == -1){//se a posicao estiver vazia posso colocar
			(*r + j)->id = m[i][0];
			(*r + j)->bg = m[i][1];
			(*r + j)->qt = m[i][2];
			(*r + j)->rk = m[i][3];
			break;
		}
		else
			if(
					(*r + j)->rk < m[i][3] || //se o rk da posicao atual for menor coloco o novo antes
					((*r + j)->rk == m[i][3] && (*r + j)->bg > m[i][1]) || //se o bg da atual for maior coloco o novo antes
					((*r + j)->rk == m[i][3] && (*r + j)->bg == m[i][1] && (*r + j)->id > m[i][0])//se o id da atual for maior coloco o novo antes
			  ){
				int k;
				for(k = lim - 1; k > j && k >= 1 && (
							(*r + k)->rk < m[i][3] || 
							((*r + k)->rk == m[i][3] && (*r + k)->bg > m[i][1]) || 
							((*r + k)->rk == m[i][3] && (*r + k)->bg == m[i][1] && (*r + k)->id > m[i][0])
							); k--){//percorro todo o vetor deslocando os processos para frente
					(*r + k)->id = (*r + (k-1))->id;
					(*r + k)->bg = (*r + (k-1))->bg;
					(*r + k)->qt = (*r + (k-1))->qt;
					(*r + k)->rk = (*r + (k-1))->rk;
				}
				(*r + j)->id = m[i][0];//insiro o novo processo antes do encontrado segundo as definicoes acima
				(*r + j)->bg = m[i][1];
				(*r + j)->qt = m[i][2];
				(*r + j)->rk = m[i][3];
				break;
			}
	if((*r + j)->rk == 0)//se encontrar algum rk 0 conto
		(*qzero)++;
}

void inout(int **m, char f, proc **r, int quantum, int lim, int *qzero){//verifico se houveram entradas pu saidas
	int i = 0;
	for(; i < lim; i++)
		if(m[i][1] == quantum){//se for o tempo de entrada do processo na matriz de entrada insiro o processo no vetor
			insereproc(m, i, r, lim, qzero);
		}
	for(i = 0; i < lim; i++)
		if((*r + i)->qt == 0){//se a quantidade do processo for 0 ele acabou e deve ser impresso
			printf("%d %d\n", (*r + i)->id, quantum - 1);
			((*r + i)->qt)--;
		}
}

void callocP(proc **a, int q){
	*a = (proc *)malloc(sizeof(proc) * q);//alocacao dinamica do vetor de processos
	int i = 0;
	for(; i < q; i++)
		clearproc(*a + i);//definicao incial
}

int entrounovoacima(proc **r, int last, int quantum){//verifica se entrou algum novo processo de rank superior
	int i = 0;
	if(last != -1)
		for(; (*r + i)->id != -1 && i != last; i++)//comeco no inico do vetor de processos, enquanto estiver em precessos validos e o i nao chegar no last
			if((*r + i)->rk > (*r + last)->rk && (*r + i)->bg == quantum)//verifico se tem algum acima novo e de rank maior
				return (*r + i)->rk;//retorno o rank encontrar
	return -1;
}

int main(){
	int i = 0, k = 0, quantum = 1, qposs = 0, lastexe = -1, qualrank = 0, aindanao = 0;//definicoes
	int **me, qzero = 0, lastexezero = -1, primeiravezaqui = 1, entrouaqui = 0, regqualrank = 0;
	char f = 0;
	allocM(&me, MAXV, 4);//alocacao da matriz de leitura das entradas
	f = getchar();
	proc *rank = NULL;
	if(f != 'f'){//se o char lido nao for um 'f'
		fseek(stdin, 0, SEEK_SET);//volta ao inicio da entrada
		for(i = 0; scanf("%d %d %d %d\n", &me[i][0], &me[i][1], &me[i][2], &me[i][3]) == 4; i++){//enquanto o scanf retornar 4 
			me[i][0] = validID(me, me[i][0], i);//o ID recebe um ID valido (o mesmo se ja for valido)
			qposs += me[i][2];//a quantidade de tempo limite recebe a soma das quantidades de tempo nessessarias de cada processo
		}
		i++;//incremento o i por seguranca
		callocP(&rank, i);//aloco o vetor de processos
		for(; quantum <= qposs + 1; quantum++){
			inout(me, f, &rank, quantum, i, &qzero);//verifico se entrou ou saiu algum processo
			if(qzero){//se tiver entrado algum processo de rank 0
				if(lastexezero != -1 && rank[k+1].rk == 0)//se ja tiver executado algum processo 0 e tiver mais algum incrementa k
					k++;
				else
					for(k = 0; rank[k].rk != 0; k++){//se for a primeira vez que executa algum rank 0 comeca no inicio do vetor ate achar o primeiro processo de rank 0
					}
				rank[k].qt--;//executa o processo nesse quantum
				lastexezero = k;
				if(rank[k].qt == 0){//se a quantidade for zero
					rank[k].rk--;//defino o rank dele como -1
					qzero--;//diminuo a quantidade de processos de prioridade zero
				}
				if(!qzero){//se acabar todos os processos zero 
					lastexezero = -1;
					continue;
				}
			}
			qualrank = entrounovoacima(&rank, lastexe, quantum);//mais informacoes na funcao
			if(!qzero && qualrank != -1 && primeiravezaqui){//se tiver entrado algum processo acima e ainda nao tiver entrado nesse if
				for(k = 1; rank[lastexe + k].rk == rank[lastexe].rk; k++)//percorre o for procurando processos do mesmo rank antes de ir para o rank superior que tiver entrado um novo processo de rank superior
					aindanao++;
				primeiravezaqui = 0;
			}
			if(qualrank > regqualrank)//se nesse meio tempo entrar um rank acima ainda do anterior o registrador de qual rank recebe o novo qualrank
				regqualrank = qualrank;
			if(!qzero && aindanao){//se tiver processos antes de ir para o rank acima executa eles
				lastexe++;
				rank[lastexe].qt--;
				aindanao--;
				entrouaqui = 1;//sobe essa flag para saber se acabou os procesos 
			}
			else if(!qzero && (qualrank != -1 || regqualrank != 0) && !aindanao){//se tiver acabado os processos do nivel e tiver entrado algum acima entra nesse if
				if(!entrouaqui && rank[lastexe].rk == 1){//se nao tiver executado processos, entrou algum nivel acima e tiver terminado todo o vetor de entrada
					k = 0;
					if(rank[k].qt == -1)//se o primeiro processo ja tiver acabado
						for(k  = 0; rank[k].qt == -1; k++){//procura pelo proximo valido
						}
					rank[k].qt--;//executa o primeiro processo de todos ainda valido
					lastexe = k + 1;	
				}
				else{//se tiver processos novo de rank superior e nao mais processos no nivel atual, vai para o novo rank
					for(k = 0; rank[k].rk != -1; k++)
						if(rank[k].rk == regqualrank)
							break;
					rank[k].qt--;//e executa
					lastexe = k + 1;
				}
				regqualrank = 0;//zera a variavel
				primeiravezaqui = 1;
			}
			else if(!qzero){//caso base
				if(lastexe == -1 || lastexe == i - 1)//se for a primeira execucao absoluta ou o fim da lista 
					lastexe = 0;
				for(k = 0; rank[lastexe].qt == -1 && k < 5; k++){//verifica cinco vezes se os processos validos acabaram
					lastexe++;
					if(lastexe == i - 1)
						lastexe = 0;
				}
				rank[lastexe].qt--;//executa o processo
				lastexe++;
			}
		}
	}
	else{//se o primeiro char lido for 'f'
		for(i = 0; scanf("%d %d %d\n", &me[i][0], &me[i][1], &me[i][2]) == 3; i++){//enquanto o scanf retornar 3
			me[i][0] = validID(me, me[i][0], i);//o ID de entrada recebe um ID valido
			qposs += me[i][2];//a quantidade de qauntum limite eh a soma das quantidades de quantum necessarias
		}
		i++;
		callocP(&rank, i);//inicia o vetor de processos
		for(; quantum <= qposs + 1; quantum++){
			inout(me, f, &rank, quantum, i, &qzero);//verifica a entrada e saida de processos
			if((lastexe = -1))//se for a primera execucao absoluta 
				lastexe = 0;
			for(k = 0; rank[lastexe].qt == -1 && k < 5; k++){//se o processo nao for mais valido encontra o proximo valido
				lastexe++;
				if(lastexe == i - 1)
					lastexe = 0;
			}
			rank[lastexe].qt--;//e executa
		}
	}
	for(i = 0; i < MAXV; i++)//libracao das alocacoes
		free(me[i]);
	free(me);
	free(rank);
	return 0;
}
