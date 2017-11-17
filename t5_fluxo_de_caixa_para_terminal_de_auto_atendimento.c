#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define hashR 5

typedef struct _ll{
	int t, b, d;//terminal, banco, banco de destino
	char o;//operacao
	float v;//valor
	struct _ll *n;//proxima celula
}ll;

void clearll(ll *a){
	a->t = 0;
	a->b = 0;
	a->d = 0;
	a->o = 0;
	a->v = 0;
	a->n = NULL;
}

int hashmap(int k){
	return k;
}

int main(){
	int read = 0;
	char c = 0;
	float f = 0;
	ll reader;
	while(!feof(stdin)){
		clearll(&reader);
		read = 0; c = 0; f = 0;
		if((c = getchar()) == 'E'){
			reader.o = c;
			//error
					while(c != '\n'){
						c = getchar();
					}
		}
		else{
			ungetc(c, stdin);
			scanf("%d ", &(reader.t));
			c = getchar();
			if(c >= '0' && c <= '9'){
				ungetc(c, stdin);
				scanf("%d ", &read);
				reader.b = read;
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
				reader.o = c;//ler teminal e char == AUDITORIA
				scanf(" %d\n", &reader.d);
				//hashmap
			}
		}
		printf("%d\t%3d\t%c\t%d\t%8.2f\t%p\n", reader.t, reader.b, reader.o, reader.d, reader.v, (void*)reader.n);
	}
	return 0;
}
