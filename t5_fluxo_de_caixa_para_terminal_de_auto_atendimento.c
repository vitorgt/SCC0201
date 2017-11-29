//114+113+51+138+80

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _ll{
	int t, b, d;//terminal, banco, banco de destino
	char o, e;//operacao, erro
	float v;//valor
	struct _ll *n;//proxima celula
}ll;

typedef struct _s{
	int id, qt;
	float mp, mn, tp, tn;
}sum;

typedef struct _e{
	int S, D, C, T;
}err;

void accsum(ll **, sum *, int, err *);

void clearll(ll *a){
	a->t = 0;
	a->b = 0;
	a->d = 0;
	a->o = 0;
	a->e = 0;
	a->v = 0;
	a->n = NULL;
}

void clearsum(sum *b){
	b->id = 0;
	b->qt = 0;
	b->mp = 0;
	b->mn = 0;
	b->tp = 0;
	b->tn = 0;
}

int hashmap(char o, int t){
	return 5*t-5+((o == 'S') ? 0 : ((o == 'D') ? 1 : ((o == 'C') ? 2 : ((o == 'T') ? 3 : ((o == 'A') ? 4 : -1)))));
}

void insertHASH(ll **hash, ll *in){
	int p = hashmap(in->o, in->t);
	ll *set = NULL, *aux = NULL;
	set = (ll *)malloc(sizeof(ll));
	clearll(set);
	set->t = in->t;
	set->b = in->b;
	set->d = in->d;
	set->o = in->o;
	set->e = in->e;
	set->v = in->v;
	set->n = NULL;
	aux  = hash[p];
	if(!aux){
		hash[p] = set;
	}
	else{
		while(aux->n)
			aux = aux->n;
		aux->n = set;
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

void freeHash(ll **h, sum *bank){
	int i = 0;
	ll *a = NULL, *b = NULL;
	for(i = 0; i < 20; i++){
		a = h[i];
		while(a){
			b = a->n;
			free(a);
			a = b;
		}
	}
	free(h);
	free(bank);
}

int main(){
	int i = 0, j = 0, k = 0;
	char c = 0, buffer[100] = {0};
	float f = 0;
	ll reader, **hash = NULL;
	sum *banks = NULL;
	err e; e.S = 0; e.D = 0; e.C = 0; e.T = 0;
	hash = (ll **)malloc(sizeof(ll *)*20);
	banks = (sum *)malloc(sizeof(sum)*4);
	for(i = 0; i < 20; i++) hash[i] = NULL;
	while(!feof(stdin)){
		clearll(&reader);
		c = 0; f = 0;
		for(i = 0; i < 100; i++) buffer[i] = 0;
		scanf("%d ", &(reader.t));
		c = getchar();
		if(c >= '0' && c <= '9'){
			ungetc(c, stdin);
			scanf("%d ", &(reader.b));
			reader.o = getchar();
			if(reader.o == 'C'){
				while(c != '\n'){
					c = getchar();
				}
			}
			else{
				scanf("%f", &f);
				if((c = getchar()) == ' '){
					reader.d = (int)f;
					scanf("%f\n", &reader.v);
				}
				else{
					if(reader.o == 'T')
						reader.d = reader.b;
					while(c == '\r'){
						c = getchar();
					}
					reader.v = f;
				}
			}
		}
		else if(c == 'S' || c == 'D' || c == 'C' || c == 'T'){
			reader.o = 'A';//ler teminal e char == AUDITORIA
			reader.v = c;
			scanf(" %d\n", &reader.d);
		}
		for(i = 0; (c = getchar()) && i < 100; i++){
			buffer[i] = c;
			if(c == 'E'){
				reader.e = 1;
				while(1){
					c = getchar();
					if(c == 0 || c == '\n' || c == EOF)
						break;
				}
			}
			if(c == 0 || c == '\n' || c == EOF)
				break;
		}
		if(!reader.e) for(; i >= 0; i--) ungetc(buffer[i], stdin);
		if(reader.o == 'S') reader.v = -reader.v;
		if(reader.o == 'T'){
			reader.v = -reader.v;
			insertHASH(hash, &reader);
			reader.v = -reader.v;
			int temp = reader.b;
			reader.b = reader.d;
			reader.d = temp;
			insertHASH(hash, &reader);
		}
		else
			insertHASH(hash, &reader);
	}
	//printH(hash);
	for(i = 0; i < 5; i++){
		for(k = 0; k < 4; k++) clearsum(banks+k);
		e.S = 0; e.D = 0; e.C = 0; e.T = 0;
		if(i != 4)
			printf("===TERMINAL %d===\n", i+1);
		else
			printf("===TOTAL===\n");
		accsum(hash, banks, i * 5, &e);
		for(j = 0; j < 4; j++){
			if(banks[j].mp || banks[j].mn || banks[j].tp || banks[j].tn)
				printf("Banco %d: Moeda +%.2f %.2f Tranferencia +%.2f %.2f\n", banks[j].id, banks[j].mp, banks[j].mn, banks[j].tp, banks[j].tn);
		}
		printf("Lucro obtido: %d.00\n", banks[0].qt*3);
		if(e.S)
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
	freeHash(hash, banks);
	return 0;
}

void accsum(ll **h, sum *b, int i, err *e){
	int max  = i + 5, j = 0;
	if(i == 20){
		i = 0;
		max = 20;
	}
	for(; i < max; i++){
		if(i%5 != 4){
			ll *a = h[i];
			while(a){
				if(!(a->e)){
					for(j = 0; (b+j)->id && (b+j)->id != a->b; j++);
					(b+j)->id = a->b;
					if(i%5 == 0)
						(b+j)->mn += a->v;
					else if(i%5 == 1)
						(b+j)->mp += a->v;
					else if(i%5 == 3){
						if(a->b != a->n->b){
							(b+j)->tn += a->v;
							a = a->n;
							for(j = 0; (b+j)->id && (b+j)->id != a->b; j++);
							(b+j)->id = a->b;
							(b+j)->tp += a->v;
						}
						else
							a = a->n;
					}
					++(b->qt);
				}
				else{
					if(a->o == 'S')
						(e->S)++;
					else if(a->o == 'D')
						(e->D)++;
					else if(a->o == 'C')
						(e->C)++;
					else if(a->o == 'T')
						(e->T)++;
				}
				a = a->n;
			}
		}
	}
	e->T = e->T/2;
}
