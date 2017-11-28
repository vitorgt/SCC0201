//114+113

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _ll{
	int t, b, d;//terminal, banco, banco de destino
	char o, e;//operacao, erro
	float v;//valor
	struct _ll *n;//proxima celula
}ll;

typedef struct _b{
	int id;
	float mp, mn, tp, tn;
}bank;

void clearll(ll *a){
	a->t = 0;
	a->b = 0;
	a->d = 0;
	a->o = 0;
	a->e = 0;
	a->v = 0;
	a->n = NULL;
}

void clearbank(bank *b){
	b->id = 0;
	b->mp = 0;
	b->mn = 0;
	b->tp = 0;
	b->tn = 0;
}

int hashmap(char o, int t){
	return 5*t-4+((o == 'S') ? 0 : ((o == 'D') ? 1 : ((o == 'C') ? 2 : ((o == 'T') ? 3 : ((o == 'A') ? 4 : -1)))));
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
	for(; i < 21; i++){
		ll *aux = hash[i];
		while(aux){
			printf("|%p|\t%d\t%3d\t%c\t%3d\t%8.2f\t%d\t%9p | %02d\n", (void *)aux, aux->t, aux->b, aux->o, aux->d, aux->v, (int)aux->e, (void*)aux->n, hashmap(aux->o, aux->t));
			aux = aux->n;
		}
	}
}

int main(){
	int i = 0;
	char c = 0, buffer[100] = {0};
	float f = 0;
	ll reader, **hash = NULL;
	hash = (ll **)malloc(sizeof(ll *)*21);
	bank *sum = NULL;
	sum = (bank *)malloc(sizeof(bank)*4);
	for(i = 0; i < 21; i++) hash[i] = NULL;
	for(i = 0; i < 4; i++) clearbank(sum+i);
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
		//printf("%d\t%3d\t%c\t%3d\t%8.2f\t%d\t%9p | %02d\n", reader.t, reader.b, reader.o, reader.d, reader.v, (int)reader.e, (void*)reader.n, hashmap(reader.o, reader.t));
	}
	printH(hash);
	for(i = 1; i < 5; i++){
		printf("===TERMINAL %d===\n", i);
		accsum(hash, sum, i);
		printf("Banco %d: Moeda +%.2f -%.2f Tranferencia +%.2f -%.2f\n", sum[i].id, sum[i].mp, sum[i].mn, sum[i].tp, sum[i].tn);
	}
	free(hash);
	return 0;
}


void accsum(ll **hash, bank *sum, int i){
	int max = 5 * i + 5;
	for(i = 5*i; i <= max; i++){
		ll *aux = hash[i];
		while(aux){
			sum->mp += aux->
			aux = aux->n;
		}
	}
}
