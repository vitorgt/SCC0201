#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct _dot{
	//definição de um "ponto" com x e y como double
	double x, y;
}dot;

typedef struct _iot{
	//definição de um "ponto" com x e y como inteiro
	int x, y;
}iot;

void allocMatrix(int ***matrix, int row, int col){
	//alocação dinamica de matriz
	int i = 0;
	*matrix = (int **)calloc(sizeof(int *), row);
	for(i = 0; i < row; i++){
		(*matrix)[i] = (int *)calloc(sizeof(int), col);
	}
}

void deallocMatrix(int **matrix, int row){
	//desalocação dinamica de matriz
	//SEM MEMORY LEAK!
	int i = 0;
	for(i = 0; i < row; i++){
		free(matrix[i]);
	}
	free(matrix);
}

void copyandcleanup(int **antiga, int **nova, int row, int col){
	//copia a matriz de adjacencia evitando zeros no meio dela e copiando apenas os positivos pois defini os negativos como caminhos fechados
	int i, j, k;
	for(i = 0; i < row; i++){
		for(j = 0, k = 0; k < col; k++){
			if(antiga[i][k] > 0){
				nova[i][j] = antiga[i][k];
				j++;
			}
		}
	}
}

void fecha(int **lis, int **output, int tes, int row, int col){
	//fecha todos os caminhos que vão para o tesouro, copia a lista de adjacencia para uma outra, reabre os caminhos e define a unica saida como a camara do tesouro
	int i, j;
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			if(lis[i][j] == tes || i == tes){
				//se o caminho ligar à camara do tesouro ou se for a linha do tesouro: negativa essa passagem
				lis[i][j] = -lis[i][j];
			}
		}
	}
	copyandcleanup(lis, output, row, col);
	//copia a lista de adjacencia sem os caminhos partindo ou chegando na camara do tesouro para outra lista
	//agora volta a lista anterior ao normal
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			if(lis[i][j] < 0){
				lis[i][j] = -lis[i][j];
			}
			if(!i){
				//zero todas as saidas possiveis
				lis[i][j] = 0;
			}
		}
	}
	lis[0][0] = tes;
	//defino como a unica saida possivel a camara do tesouro
}

void update(int **a, iot *b, int col, int qacoes){
	//atualiza a lista de ajdacencia depois de passar pelo tesourio abrindo e fechando caminhos
	int i, j;
	for(i = 0; i < qacoes; i++){
		for(j = 0; j < col; j++){
			if(abs(a[abs(b[i].x)][j]) == abs(b[i].y)){
				a[abs(b[i].x)][j] = b[i].y;
				break;
			}
		}
		for(j = 0; j < col; j++){
			if(abs(a[abs(b[i].y)][j]) == abs(b[i].x)){
				a[abs(b[i].y)][j] = b[i].x;
				break;
			}
		}
	}
}

int distan(dot a, dot b){
	//calculo da distancia geodesica
	return ((int)round(sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2))));
}

void dsf(int **lis, int **output, int *qop, int ent, dot *camaras){
/*	while(aindatemsolucoes){
		int i, j = 2, atual = ent, antiga = -1;
		while(lis[atual][]){
			output[*qop][0]++;
			//minha coluna 0 é a quantidade de camaras passadas para essa linha de solucao
			if(antiga != -1)
				output[*qop][1] -= distan(camaras[antiga], camaras[atual]);
			//a coluna 1 é a distancia percorrida por esta linha de solucao
			output[*qop][j] = atual;
			//a partir da segunda coluna começa as camaras percorridas
			if(ehsaida){
				(*qop)++;
				//vou para a linha de baixo e continuo percorrendo caminhos
			}
			if(becosemsaida){
				//zero toda a linha e subtraio a quantidade de solucoes possiveis
				for(i = 0; output[*qop][i] != 0; i++){
					output[*qop][i] = 0;
				}
				(*qop)--;
			}
			antiga = atual;
			atual = lis[atual][];
		}
		(*qop)++;
	}
*/}

int main(){
	//declarações
	int qcamaas, qcainhos, buffer, i, j, camaratesouro, tesouro, qacoes, inicial, qop = 0;
	int **crur = NULL, **ent_tes = NULL, **tes_sai = NULL, **ent_sai = NULL, **shs = NULL;

	scanf("%d\n", &qcamaas);

	//aloções dinamocas
	dot *camaras = (dot *)malloc(sizeof(dot) * qcamaas);
	allocMatrix(&crur, qcamaas+1, qcamaas);
	allocMatrix(&ent_tes, qcamaas+1, qcamaas);
	allocMatrix(&tes_sai, qcamaas+1, qcamaas);
	allocMatrix(&ent_sai, qcamaas+1, qcamaas);
	allocMatrix(&shs, qcamaas+1, qcamaas+3);

	for(i = 0; i < qcamaas; i++){
		//escanear as posicoes das camaras
		scanf("%lf %lf\n", &camaras[i].x, &camaras[i].y);
	}

	scanf("%d\n", &buffer);
	//escaneia a quantidade de camaras e portais
	for(i = 0, j = 0; i < buffer; i++){
		int a1, a2;
		scanf("%d %d\n", &a1, &a2);
		if(a2){
			crur[0][j] = a1;
			//coloco as saidas na linha zero da minha lista de adjacencias
			j++;
		}
	}

	scanf("%d\n", &qcainhos);
	//escaneia a quantidade de caminhos
	for(i = 0; i < qcainhos; i++){
		int a1, a2;
		scanf("%d %d\n", &a1, &a2);
		if(a1 < 0 || a2 < 0){
			//se algum dos dois for negativo, faço os dois ficarem negativos: -|a|
			a1 = -abs(a1);
			a2 = -abs(a2);
		}
		for(j = 0; j < qcainhos; j++){
			if(crur[abs(a1)][j] == 0){
				//percorro a linha do primeiro valor escaneado até achar algum zero entao coloco o segundo valor escaneado nela
				//depois faço o contrario no proximo for
				crur[abs(a1)][j] = a2;
				break;
			}
		}
		for(j = 0; j < qcainhos; j++){
			if((crur[abs(a2)][j] == 0) && (a1 != a2)){
				crur[abs(a2)][j] = a1;
				break;
			}
		}
	}

	scanf("%d %d\n%d", &camaratesouro, &tesouro, &qacoes);
	//^autoexplicativo
	iot *acoes = (iot *)malloc(sizeof(iot) * qacoes);
	//alocação dinamica de um vetor de pontos x e y inteiros com a quantidade de acoes
	for(i = 0; i < qacoes; i++){
		scanf("%d %d\n", &acoes[i].x, &acoes[i].y);
		if(acoes[i].x < 0 || acoes[i].y < 0){
			//se alguma das duas forem negativas, negativo as duas
			acoes[i].x = -abs(acoes[i].x);
			acoes[i].y = -abs(acoes[i].y);
		}
	}

	scanf("%d", &inicial);

	copyandcleanup(crur, ent_tes, qcamaas+1, qcamaas);
	//copia a lista de adjacencias crur que ainda tem caminhos negativos para a ent_sai tirando esses negativos
	fecha(ent_tes, ent_sai, camaratesouro, qcamaas+1, qcamaas);
	//explicação na função
	update(crur, acoes, qcamaas, qacoes);
	//atualiza a lista crur para criar a lista de adjacencia depois de se passar pelo tesouro
	copyandcleanup(crur, tes_sai, qcamaas+1, qcamaas);
	//copia a lista de adjacencias crur agora atualizada que ainda tem caminhos negativos para a tes_sai tirando esses negativos
	deallocMatrix(crur, qcamaas+1);
	//desaloca a matriz crur

	dsf(ent_sai, shs, &qop, inicial, camaras);
	//percorre todos os caminhos entre a entrada e as saidas evitando a camara do tesouro e comçando na entrada normal
	dsf(ent_tes, shs, &qop, inicial, camaras);
	//percorre todos os caminhos entre a entrada e o tesouro "esquecendo-se" das saidas comuns e comçando na entrada normal
	dsf(tes_sai, shs, &qop, camaratesouro, camaras);
	//percorre todos os caminhos entre o tesouro e as saidas, onde agora a camara do tesouro é a "camara inical"

	for(i = 0; i < qcamaas+1; i++){
		printf("\x1B[0mt_s_%02d:\t\t", i);
		for(j = 0; j < qcamaas && tes_sai[i][j]; j++){
			if(!i)
				printf("\x1B[32m%2d\t", tes_sai[i][j]);
			else
				printf("%2d\t", tes_sai[i][j]);
		}
		printf("\n");
	}
	for(i = 0; i < qcamaas+1; i++){
		printf("\x1B[0me_t_%02d:\t\t", i);
		for(j = 0; j < qcamaas && ent_tes[i][j]; j++){
			if(!i)
				printf("\x1B[32m%2d\t", ent_tes[i][j]);
			else
				printf("%2d\t", ent_tes[i][j]);
		}
		printf("\n");
	}
	for(i = 0; i < qcamaas+1; i++){
		printf("\x1B[0me_s_%02d:\t\t", i);
		for(j = 0; j < qcamaas && ent_sai[i][j]; j++){
			if(!i)
				printf("\x1B[32m%2d\t", ent_sai[i][j]);
			else
				printf("%2d\t", ent_sai[i][j]);
		}
		printf("\n");
	}

	printf("%d %d\n", camaratesouro, tesouro);


	//liberação das alocações dinamicas
	free(acoes);
	free(camaras);
	deallocMatrix(ent_tes, qcamaas+1);
	deallocMatrix(tes_sai, qcamaas+1);
	deallocMatrix(ent_sai, qcamaas+1);
	deallocMatrix(shs, qcamaas+1);
	return 0;
}
