#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _ll{
	int t, b, d;//terminal, banco, banco de destino
	char o, e;//operacao, erro
	float v;//valor
	struct _ll *n;//proxima celula
}ll;

void clearll(ll *a){
	a->t = 0;
	a->b = 0;
	a->d = 0;
	a->o = 0;
	a->e = 0;
	a->v = 0;
	a->n = NULL;
}

int hashmap(char o, int t){
	return 4*t-4+((o == 'S') ? 0 : ((o == 'D') ? 1 : ((o == 'C') ? 2 : ((o == 'T') ? 3 : -1))));
}

int main(){
	int i = 0;
	char c = 0, buffer[100] = {0};
	float f = 0;
	ll reader;
	while(!feof(stdin)){
		clearll(&reader);
		read = 0; c = 0; f = 0;
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
			reader.o = c;//ler teminal e char == AUDITORIA
			scanf(" %d\n", &reader.d);
			//hashmap
		}
		for(i = 0; (c = getchar()); i++){
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
		if(!reader.e)
			for(; i >= 0; i--)
				ungetc(buffer[i], stdin);
		printf("%d\t%3d\t%c\t%d\t%8.2f\t%d\t%p | %d\n", reader.t, reader.b, reader.o, reader.d, reader.v, (int)reader.e, (void*)reader.n, hashmap(reader.o, reader.t));
	}
	return 0;
}
