#include<stdio.h>
#include<stdlib.h>

#define MAXV 1001

typedef struct _js{
	int obj;//objects
	int mem;//members
	int pai;//pairs
	int arr;//arrays
	int ele;//elements
	int str;//strings
	int dig;//digits
	int num;//numbers
	int tru;//trues
	int fal;//falses
	int nul;//nulls
	int pos;//position
	int siz;//size
}js;

int Jval(js *a, char *in);
int Jobj(js *a, char *in);
int Jmem(js *a, char *in);
int Jpai(js *a, char *in);
int Jarr(js *a, char *in);
int Jele(js *a, char *in);
int Jstr(js *a, char *in);
int Jdig(js *a, char *in);
int Jsig(js *a, char *in);
int Jexp(js *a, char *in);
int Jnum(js *a, char *in);
int Jtru(js *a, char *in);
int Jfal(js *a, char *in);
int Jnul(js *a, char *in);

void clearjs(js *a){//inicializa a struct
	a->obj = 0;
	a->arr = 0;
	a->pai = 0;
	a->str = 0;
	a->num = 0;
	a->tru = 0;
	a->fal = 0;
	a->nul = 0;
	a->pos = 0;
	a->mem = 0;
	a->ele = 0;
	a->dig = 0;
	a->siz = 0;
}

int Jval(js *a, char *in){//verifica se e value
	if(Jobj(a, in))//se for object
		return 1;
	if(Jarr(a, in))//se for array
		return 1;
	if(Jstr(a, in))//se for string
		return 1;
	if(Jnum(a, in))//se for number
		return 1;
	if(Jtru(a, in))//se for true
		return 1;
	if(Jfal(a, in))//se for false
		return 1;
	if(Jnul(a, in))//se for null
		return 1;
	return 0;//se nao for nenhum dos casos acima, nao e value
}

int Jobj(js *a, char *in){//verifica se e object
	int temp = a->pos, flag = 1, base = a->mem;
	//temp recebe a posicao no inicio da funcao 
	//base recebe a quantidade de members no inicio da funcao para comparar se aumentou ou nao
	if(in[(a->pos)++] == '{'){
		if(in[(a->pos)++] == '}'){//se abrir e fechar colchetes e valido
			(a->obj)++;//aumenta a quantidade de objetos
			return 1;
		}
		(a->pos)--;
		while(flag){
			flag = Jmem(a, in);//verifica a existencia de members
			if(!flag && !(a->mem - base)){//se a flag cair e nenhum member for adicionado a contagem de members
				a->pos = temp;//volta a posicao
				return 0;
			}
		}
		if(in[(a->pos)++] == '}'){//se fechar colchetes aumenta a contgem de objetos
			(a->obj)++;
			return 1;
		}
	}//senao votla a posicao e retorna que nao achou
	a->pos = temp;
	return 0;
}

int Jmem(js *a, char *in){//veirifica a existencia de members
	int temp = a->pos, flag = 1, base = a->pai;
	//temp recebe a posicao no inicio da funcao 
	//base recebe a quantidade de pairs no inicio da funcao para comparar se aumentou ou nao
	while(flag){
		Jpai(a, in);//verifica a existencia de pairs
		if(!(a->pai - base)){//se a quantidade de pairs nao aumentar 
			a->pos = temp;//volta a psicao e retorna que nao achou member
			return 0;
		}
		if(in[(a->pos)++] == ',')//se achar uma virgula sobe a flag e incrementa a posicao 
			flag = 1;
		else{//senao abaixa a flag e volta a posicao
			flag = 0;
			(a->pos)--;
		}
	}
	if((a->pai - base) && (in[(a->pos)] == '}')){//se tiver aumentado a quantidade de pairs e fechar colchetes
		(a->mem)++;//aumenta a quantidade de members
		return 1;
	}
	a->pos = temp;//se nao achar volta a posicao
	return 0;
}

int Jpai(js *a, char *in){//verifica a existencia de pairs
	int temp = a->pos;
	//temp recebe a posicao no inicio da funcao 
	if(Jstr(a, in)){//verifica a existencia de string
		if(in[(a->pos)++] == ':'){//procura pelo ':'
			if(Jval(a, in)){//verifica se ha um value
				(a->pai)++;//aumenta a quantidade de pairs
				return 1;
			}
		}
	}
	a->pos = temp;//se nao achar volta a posicao
	return 0;
}

int Jarr(js *a, char *in){//verifica a existencia de arrays
	int temp = a->pos, flag = 1, base = a->ele;
	//temp recebe a posicao no inicio da funcao 
	//base recebe a quantidade de elements no inicio da funcao para comparar se aumentou ou nao
	if(in[(a->pos)++] == '['){//verifica se abre colchetes
		if(in[(a->pos)++] == ']'){//se fechar colchetes e valido
			(a->arr)++;//aumenta a quantidade de arrays
			return 1;
		}
		(a->pos)--;
		while(flag){
			flag = Jele(a, in);//verifica se achou algum element
			if(!flag && !(a->ele - base)){//se a flag cair e nao tiver aumentado a quantidade de elements nenhuma vez
				a->pos = temp;//volta a posicao e retorna nao encontrado
				return 0;
			}
			if(in[(a->pos)++] == ']'){//se fechar colcheter e um array valido
				(a->arr)++;
				return 1;
			}
			(a->pos)--;
		}
	}
	a->pos = temp;
	return 0;
}

int Jele(js *a, char *in){//verifica a exitencia de elements
	int temp = a->pos, flag = 1, base = a->ele;
	//temp recebe a posicao no inicio da funcao 
	//base recebe a quantidade de elements no inicio da funcao para comparar se aumentou ou nao
	while(flag){
		while(in[(a->pos)] == ','){//enquanto encontrar virgulas
			(a->pos)++;//incrementa a posicao
		}
		if(in[(a->pos)] == ']'){//se a proxima posicao for um fecha colchetes e um element valido
			(a->ele)++;
			return 1;
		}
		flag = Jval(a, in);//verifica a existencia de um value
		if(flag){//se tiver econtrado um element 
			(a->ele)++;//aumenta a contagem de elements
		}
	}
	if((a->ele) - base){//se tiver acrecido algum element durante e execucao da funcao
		return 1;
	}
	a->pos = temp;//senao volta a posicao
	return 0;
}

int Jstr(js *a, char *in){
	int temp = a->pos;
	//temp recebe a posicao no inicio da funcao 
	if(in[(a->pos)++] == '"'){//se a primeira posicao for '"' incrementa e continua
		while(in[(a->pos)++] != '"'){//enquanto a posicao for diferente de '"', incrementa e 
			if(in[(a->pos) - 1] == 92){//se a posicao for um '\' e for seguido por
				if(
						in[(a->pos)] == '"' ||//se for um '"' ou
						in[(a->pos)] ==  92 ||//se for um '\' ou
						in[(a->pos)] == 'b' ||//se for um 'b' ou
						in[(a->pos)] == 'f' ||//se for um 'f' ou
						in[(a->pos)] == 'n' ||//se for um 'n' ou
						in[(a->pos)] == 'r' ||//se for um 'r' ou
						in[(a->pos)] == 't' ||//se for um 't' ou
						in[(a->pos)] == 'u' ){//se for um 'u'
					a->pos = temp;//volta a posicao e retorna que nao e uma string
					return 0;
				}
			}
		}
		if(//verifica se  apos a string tem uma ',', um ']', um ':', um '}' ou um '0' de fim de entrada validando a string
				in[(a->pos)] == ',' ||
				in[(a->pos)] == ']' ||
				in[(a->pos)] == ':' ||
				in[(a->pos)] == '}' ||
				in[(a->pos)] == 0){
			(a->str)++;//aumenta a contagem de strings
			return 1;
		}
	}
	a->pos = temp;//senao volta a posicao no inicio da funcao
	return 0;
}

int Jdig(js *a, char *in){//verifica a existencia de digits
	int temp = a->pos, flag = 1, base = a->dig;
	//temp recebe a posicao no inicio da funcao 
	//base recebe a quantidade de digits no inicio da funcao para comparar se aumentou ou nao
	while(flag){
		if(in[(a->pos)] >= '0' && in[(a->pos)] <= '9'){//se for numero
			(a->pos)++;//aumenta a posicao e a quantidade de digitos
			(a->dig)++;
		}
		else
			flag = 0;
	}
	if(a->dig - base)//se a quantidade de digitos tiver aumentado desde o inicio da funcao
		return 1;//retorna que achou um number
	a->pos = temp;//senao volta a posicao
	return 0;
}

int Jsig(js *a, char *in){//verifica a existencia de signs
	if(in[(a->pos)] == '+' || in[(a->pos)] == '-'){//se for um '+' ou um '-'
		(a->pos)++;//aumenta a posicao
		return 1;//retorna que encontrou sign
	}
	return 0;
}

int Jexp(js *a, char *in){//verifica a existencia de exps
	int temp = a->pos, base = a->dig;
	//temp recebe a posicao no inicio da funcao 
	//base recebe a quantidade de digits no inicio da funcao para comparar se aumentou ou nao
	if(in[(a->pos)] == 'e' || in[(a->pos)] == 'E'){//se encontrar um 'e' ou um 'E'
		(a->pos)++;//aumenta a posicao
	}
	if(in[(a->pos)] == 'e' || in[(a->pos)] == 'E'){//se encontrar outro, invalida
		a->pos = temp;//volta a posicao
		return 0;//retorna que nao encontrou exp
	}
	Jsig(a, in);//procura sinais
	if(!Jdig(a, in)){//se nao encontrar digits
		a->pos = temp;//volta a posicao
		return 0;
	}
	if(a->dig - base)
		//se a quantidade de digits aumentar durante a execucao dessa funcao significa que o exp e valido
		return 1;
	a->pos = temp;
	return 0;
}

int Jnum(js *a, char *in){
	int temp = a->pos, base = a->dig;
	//temp recebe a posicao no inicio da funcao 
	//base recebe a quantidade de digits no inicio da funcao para comparar se aumentou ou nao
	Jsig(a, in);//procura por sinais
	if(Jdig(a, in)){//se encontrar digits
		if(in[(a->pos)] == '.'){//se encontrar '.'
			(a->pos)++;
			base = a->dig;//reinicia a base para procurar por numeros apos o '.'
			if(Jdig(a, in)){//se encontrar digits
				if((a->dig - base) && (in[(a->pos)] == ',' || in[(a->pos)] == ']' || in[(a->pos)] == '}' || in[(a->pos)] == 0)){
					//se a quantidade de digits tiver aumentado e for seguido de ',', ']', '}', '0'
					(a->num)++;//aumenta a quantidade de numbers
					return 1;
				}
				if(Jexp(a, in)){//se encontrar um exp apos um numero real
					(a->num)++;//aumenta a quantidade de numbers
					return 1;
				}
			}
		}
		if(Jexp(a, in)){//se encontrar um exp apos um inteiro
			(a->num)++;//aumenta a quantidade de numbers
			return 1;
		}
	}
	if((a->dig - base) && (in[(a->pos)] == ',' || in[(a->pos)] == ']' || in[(a->pos)] == '}' || in[(a->pos)] == 0)){
		//se a quantidade de digits tiver aumentado e for seguido de ',', ']', '}', '0'
		(a->num)++;//aumenta a quantidade de numbers
		return 1;
	}
	a->pos = temp;//senao volta a posicao e retorna que nao e numero
	return 0;
}

int Jtru(js *a, char *in){
	int temp = a->pos;
	//temp recebe a posicao no inicio da funcao 
	if(in[(a->pos)] == 't' || in[(a->pos)] == 'T'){//se for 't' ou 'T'
		(a->pos)++;
		if(in[(a->pos)] == 'r' || in[(a->pos)] == 'R'){//se for 'r' ou 'R'
			(a->pos)++;
			if(in[(a->pos)] == 'u' || in[(a->pos)] == 'U'){//se for 'u' ou 'U'
				(a->pos)++;
				if(in[(a->pos)] == 'e' || in[(a->pos)] == 'E'){//se for 'e' ou 'E'
					(a->pos)++;
					if(in[(a->pos)] == ',' || in[(a->pos)] == ']' || in[(a->pos)] == '}' || in[(a->pos)] == 0){
						//para validar o 'true' verifica se a proxima posicao e valida
						(a->tru)++;//incrementa trues
						return 1;//retorna que encontrou true
					}
				}
			}
		}
	}
	a->pos = temp;
	return 0;
}

int Jfal(js *a, char *in){
	int temp = a->pos;
	//temp recebe a posicao no inicio da funcao 
	if(in[(a->pos)] == 'f' || in[(a->pos)] == 'F'){//analogo ao true
		(a->pos)++;
		if(in[(a->pos)] == 'a' || in[(a->pos)] == 'A'){
			(a->pos)++;
			if(in[(a->pos)] == 'l' || in[(a->pos)] == 'L'){
				(a->pos)++;
				if(in[(a->pos)] == 's' || in[(a->pos)] == 'S'){
					(a->pos)++;
					if(in[(a->pos)] == 'e' || in[(a->pos)] == 'E'){
						(a->pos)++;
						if(in[(a->pos)] == ',' || in[(a->pos)] == ']' || in[(a->pos)] == '}' || in[(a->pos)] == 0){
							//para validar o 'false' verifica se a proxima posicao e valida
							(a->fal)++;
							return 1;
						}
					}
				}
			}
		}
	}
	a->pos = temp;
	return 0;
}

int Jnul(js *a, char *in){
	int temp = a->pos;
	//temp recebe a posicao no inicio da funcao 
	if(in[(a->pos)] == 'n' || in[(a->pos)] == 'N'){//analogo ao true
		(a->pos)++;
		if(in[(a->pos)] == 'u' || in[(a->pos)++] == 'U'){
			(a->pos)++;
			if(in[(a->pos)] == 'l' || in[(a->pos)++] == 'L'){
				(a->pos)++;
				if(in[(a->pos)] == 'l' || in[(a->pos)++] == 'L'){
					(a->pos)++;
					if(in[(a->pos)] == ',' || in[(a->pos)] == ']' || in[(a->pos)] == '}' || in[(a->pos)] == 0){
						//para validar o 'null' verifica se a proxima posicao e valida
						(a->nul)++;
						return 1;
					}
				}
			}
		}
	}
	a->pos = temp;
	return 0;
}

int main(){
	int flagEOF = 1, control = 0;//declaracoes
	char c, in[MAXV];
	js a;
	clearjs(&a);//limpa a struct de contadores
	while((c = getchar()) && flagEOF){//enquanto c recebe chars e nao for o fim da entrada
		if(c == '\n' || c == EOF){//se for um '\n' ou um 'EOF'
			control++;
			if(!Jval(&a, in)){//verifica se nao ha um value
				printf("Error line %d\n", control);//se nao tiver um value valido printa erro
				break;//e encerra o programa
			}
			a.pos = 0;//zera as posicoes
			a.siz = 0;//e o tamanho
			if(c == EOF)//se c for 'EOF'
				flagEOF = 0;//abaixa a flag de EOF para encerrar o while
		}
		else if(c > 32 && c < 127)
			//tabela ASCII, se for um caracter jugado valido, ele entra para o vetor de entrada
			in[a.siz++] = c;
	}
	printf("Number of Objects: %d\n", a.obj);//prints
	printf("Number of Arrays: %d\n", a.arr);
	printf("Number of Pairs: %d\n", a.pai);
	printf("Number of Strings: %d\n", a.str);
	printf("Number of Numbers: %d\n", a.num);
	printf("Number of Trues: %d\n", a.tru);
	printf("Number of Falses: %d\n", a.fal);
	printf("Number of Nulls: %d\n", a.nul);
	return 0;
}
