#include<stdio.h>
#include<stdlib.h>

typedef struct{//ponto tridimensional
	double x, y, z;
}dot;

typedef struct{//triangulo feito com tres pontos
	dot t[3];
}tri;

typedef struct{//autoexplicativo
	double min, max;
}minmax;

typedef struct{//caixa definida por tres maximos e minimos em cada variavel
	minmax x, y, z;
}box;

int isitin(box a, dot c){//o ponto c esta dentro da caixa a
	if(
			c.x >= a.x.min &&//se x estiver entre x minimo e x maximo da caixa
			c.x <= a.x.max &&
			c.y >= a.y.min &&//se y estiver entre y minimo e y maximo da caixa
			c.y <= a.y.max &&
			c.z >= a.z.min &&//se z estiver entre z minimo e z maximo da caixa
			c.z <= a.z.max
	  )
		return 1;
	return 0;
}

int whichoctant(box a, dot c){//qual octante da caixa a contem o ponto c	GEOMETRIA ANALITICA
	if(c.x >= (((a.x.max - a.x.min) / 2) + a.x.min)){
		if(c.y >= (((a.y.max - a.y.min) / 2) + a.y.min)){
			if(c.z >= (((a.z.max - a.z.min) / 2) + a.z.min)){
				return 8;
			}
			else{
				return 4;
			}
		}
		else{
			if(c.z >= (((a.z.max - a.z.min) / 2) + a.z.min)){
				return 6;
			}
			else{
				return 2;
			}
		}
	}
	else{
		if(c.y >= (((a.y.max - a.y.min) / 2) + a.y.min)){
			if(c.z >= (((a.z.max - a.z.min) / 2) + a.z.min)){
				return 7;
			}
			else{
				return 3;
			}
		}
		else{
			if(c.z >= (((a.z.max - a.z.min) / 2) + a.z.min)){
				return 5;
			}
			else{
				return 1;
			}
		}
	}
}

void divideoctant(box *div, int oct){//divide a caixa div no octante oct	GEOMETRIA ANALITICA
	if(oct == 8){
		div->x.min += (div->x.max - div->x.min)/2;
		div->y.min += (div->y.max - div->y.min)/2;
		div->z.min += (div->z.max - div->z.min)/2;
	}
	else if(oct == 7){
		div->x.max -= (div->x.max - div->x.min)/2;
		div->y.min += (div->y.max - div->y.min)/2;
		div->z.min += (div->z.max - div->z.min)/2;
	}
	else if(oct == 6){
		div->x.min += (div->x.max - div->x.min)/2;
		div->y.max -= (div->y.max - div->y.min)/2;
		div->z.min += (div->z.max - div->z.min)/2;
	}
	else if(oct == 5){
		div->x.max -= (div->x.max - div->x.min)/2;
		div->y.max -= (div->y.max - div->y.min)/2;
		div->z.min += (div->z.max - div->z.min)/2;
	}
	else if(oct == 4){
		div->x.min += (div->x.max - div->x.min)/2;
		div->y.min += (div->y.max - div->y.min)/2;
		div->z.max -= (div->z.max - div->z.min)/2;
	}
	else if(oct == 3){
		div->x.max -= (div->x.max - div->x.min)/2;
		div->y.min += (div->y.max - div->y.min)/2;
		div->z.max -= (div->z.max - div->z.min)/2;
	}
	else if(oct == 2){
		div->x.min += (div->x.max - div->x.min)/2;
		div->y.max -= (div->y.max - div->y.min)/2;
		div->z.max -= (div->z.max - div->z.min)/2;
	}
	else if(oct == 1){
		div->x.max -= (div->x.max - div->x.min)/2;
		div->y.max -= (div->y.max - div->y.min)/2;
		div->z.max -= (div->z.max - div->z.min)/2;
	}
}

int isthereanyvertex(tri *mesh, box deli, int qtri){//tem algum vertice da malha mesh dentro da caixa deli
	int i, j;
	for(i = 0; i < qtri; i++){
		for(j = 0; j < 3; j++){
			if(isitin(deli, mesh[i].t[j])){//o ponto 'mesh[i].t[j]' esta dentro da caixa deli
				return 1;
			}
		}
	}
	return 0;
}

int didithit(tri *mesh, box *deli, dot coli, int levl, int qtri){
	if(levl){//se o nivel de profundidade ainda for positivo
		divideoctant(deli, whichoctant(*deli, coli));//divide o octante deli no octante q contem o ponto colisor
		return didithit(mesh, deli, coli, --levl, qtri);//rechama a funcao um nivel abaixo
	}
	else{
		if(isthereanyvertex(mesh, *deli, qtri) && isitin(*deli, coli)){//se tiver algum ponto da malha e o colisor dentro do octante TRUE
			return 1;
		}
		return 0;
	}
}

int main(){
	int buff, levl, qtri, i, j;//declaracoes
	dot coli;
	tri *mesh;
	box deli;

	scanf("%d\n%d\n%d\n%lf %lf %lf\n", &buff, &levl, &qtri, &coli.x, &coli.y, &coli.z);//escaneamento
	qtri *= 2;//duplicacao da quantidade de triangulos para posterior subdivisao
	mesh = (tri*)malloc(sizeof(tri)*qtri);

	for(i = 0; i < qtri; i++){
		for(j = 0; j < 3; j++){
			scanf("%lf %lf %lf\n", &mesh[i].t[j].x, &mesh[i].t[j].y, &mesh[i].t[j].z);

			if(!i && !j){//se i e j forem 0
				deli.x.min = mesh[i].t[j].x;
				deli.x.max = mesh[i].t[j].x;
				deli.y.min = mesh[i].t[j].y;
				deli.y.max = mesh[i].t[j].y;
				deli.z.min = mesh[i].t[j].z;
				deli.z.max = mesh[i].t[j].z;
			}
			if(mesh[i].t[j].x < deli.x.min){//calculo do minimo e maximo da malha de entrada para a definicao da caixa delimitadora
				deli.x.min = mesh[i].t[j].x;
			}
			if(mesh[i].t[j].x >= deli.x.max){
				deli.x.max = mesh[i].t[j].x;
			}
			if(mesh[i].t[j].y < deli.y.min){
				deli.y.min = mesh[i].t[j].y;
			}
			if(mesh[i].t[j].y >= deli.y.max){
				deli.y.max = mesh[i].t[j].y;
			}
			if(mesh[i].t[j].z < deli.z.min){
				deli.z.min = mesh[i].t[j].z;
			}
			if(mesh[i].t[j].z >= deli.z.max){
				deli.z.max = mesh[i].t[j].z;
			}

		}
		i++;
		/* vou para a proxima posicao do vetor de triangulos e subdivido o triangulo anteriormente lido
			repare que o novo triangulo subdividido nao precisa entrar no calculo do maximo e minimo
			por que ele sempre sera o ponto medio das arestas do anterior
		*/
		for(j = 0; j < 3; j++){//subdivisao da malha		GEOMETRIA ANALITICA
			mesh[i].t[j].x = (mesh[i-1].t[j].x + mesh[i-1].t[(j+1)%3].x)/2;//possiveis j: 01, 12, 20
			mesh[i].t[j].y = (mesh[i-1].t[j].y + mesh[i-1].t[(j+1)%3].y)/2;
			mesh[i].t[j].z = (mesh[i-1].t[j].z + mesh[i-1].t[(j+1)%3].z)/2;
		}
	}

	if(!isitin(deli, coli)){//se o ponto colisor dado nao estiver dentro da caixa delimitadora dada encerra o programa
		printf("0\n");
		return 0;
	}

	printf("%d\n", didithit(mesh, &deli, coli, levl, qtri));

	free(mesh);//liberacao da alocacao dinamica
	return 0;
}
