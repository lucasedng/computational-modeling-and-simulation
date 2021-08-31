#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX 1000		 //Quantidade de vertices
#define Prob_Inf 0.75	 //Probabilidade do individuo suscetivel se infectar
#define Prob_Rec 0.10	 //Probabilidade do individuo infectado se recuperar
#define Prob_InfRec 0.70 //Probabilidade do individuo recuperado se infectar
#define Inf_0 100		 //Quantidade inicial de infectados
#define Ite 200			 //Quantidade de iterações da simulação
#define TipoGrafo 3		 //Tipo de Grafo (Reg=0, Rand=1, Livre de escala=2, Mod= 3)
#define Grau 4			 //Grau medio/2
#define Morte 15

/*Para inserir uma rede modular qualquer, mude o nome do arquivo da função inicia_grafomod e coloque
TipoGrafo= 3 na constante acima.
*/

//-------------------------------------------- Structs --------------------------------------------

int Map[MAX];
typedef int TItem;

typedef struct SNo *pNo;
typedef struct SNo
{
	TItem Item;
	pNo Esq, Dir;
	int fb;
} TNo;

typedef struct Arvore
{
	pNo Raiz;
} TABB;

//-------------------------------------------- Declaração de Funções --------------------------------------------

TABB *Inicializar();

pNo CriaNo(TItem x, pNo Esq, pNo Dir);
pNo Pesquisa(pNo Raiz, TItem Item);

int Altura(pNo A);

int Insere(pNo *Raiz, TItem Item);
int Remove(pNo *Raiz, TItem Item);

void Rot(pNo *Raiz);
void RR(pNo *Raiz);
void LR(pNo *Raiz);
void RL(pNo *Raiz);
void LL(pNo *Raiz);
void UpdateFB(pNo A);

void Liberdade(TNo *A);
void PreOrdem(TNo *A);
void EmOrdem(TNo *A);
void MegaEmOrdem(TABB *ListaAdj[MAX]);

void Peso(TItem Item, int *peso, char op);

void Elementos(pNo Raiz, int *i, TItem V[MAX]);
void GeraGrafo(int **mat_graph, int Escolha);
void CarregaGrafo(TABB *ListaAdj[MAX], int **mat_graph);
void inicia_grafomod(int **mat_graph);
void inicia_grafolivre(int **mat_graph);
void inicia_graforeg(int **matriz, int grau);
void inicia_graforand(int **matriz, int grau);
void Inicializa_Infectados(TABB *Infectados);
int VerificaProb(float prob, int rand);
int EscolheProb(int max);
//void PrintaVertices(FILE *graph, TABB* Infectados, TABB* Recuperados, TABB* Mortos);
//void PrintaMatriz(FILE *graph, int **mat_graph);

int InsGraph(TABB *Adj[MAX], TItem u, TItem v);
int RemGraph(TABB *Adj[MAX], TItem u, TItem v);

void teste(pNo A)
{
	printf("\n-----: ");
	PreOrdem(A);
	printf(" :-----\n");
}

//-------------------------------------------- Main --------------------------------------------

int main()
{
	int n, no, j, i, k, r, **mat_graph, tam_inf = 0, tam_rec = 0, tam_mor = 0, tam_poss = 0, vet_inf[MAX], vet_poss[MAX], vet_rec[MAX], vet_mor[MAX], boo;
	pNo pont_inf, pont_rec, pont_mor;

	// Inicializações
	TABB *Infectados = Inicializar();
	TABB *Recuperados = Inicializar();
	TABB *Mortos = Inicializar();
	TABB *ListaAdj[MAX];

	FILE *pont_arq;
	pont_arq = fopen("Simulacao4.txt", "wt");

	if (pont_arq == NULL)
	{
		printf("Problemas na CRIACAO do arquivo\n");
		return 404;
	}

	FILE *graph_i, *graph_f;
	graph_i = fopen("Grafo1Inicial.txt", "wt");

	if (graph_i == NULL)
	{
		printf("Problemas na CRIACAO do arquivo\n");
		return 404;
	}

	graph_f = fopen("Grafo1Final.txt", "wt");
	if (graph_i == NULL)
	{
		printf("Problemas na CRIACAO do arquivo\n");
		return 404;
	}

	fprintf(pont_arq, "t\tI\tR\tM\n");

	mat_graph = (int **)malloc(sizeof(int *) * MAX);

	for (n = 0; n < MAX; n++)
	{
		ListaAdj[n] = Inicializar();
		mat_graph[n] = (int *)malloc(sizeof(int) * MAX);
		Map[n] = 0;
	}

	//Gera grafo
	GeraGrafo(mat_graph, TipoGrafo);

	//Carrega grafo na lista de adjacência
	CarregaGrafo(ListaAdj, mat_graph);

	//Printa a lista de adjacência
	MegaEmOrdem(ListaAdj);

	//Inicializa lista de infectados
	Inicializa_Infectados(Infectados);

	PrintaVertices(graph_i, Infectados);

	PrintaMatriz(graph_i, mat_graph);

	for (i = 0; i < Ite; i++)
	{
		//Reseta a quantidade de infectados para 0
		tam_inf = 0;
		//Reseta a quantidade de recuperados para 0
		tam_rec = 0;
		//Reseta a quantidade de mortos para 0
		tam_mor = 0;

		//Função pega a lista de Infectados, retorna a quantidade de elementos dela e todos os nós infectados no vetor vet_inf
		Elementos(Infectados->Raiz, &tam_inf, vet_inf);
		//Função pega a lista de recuperados, retorna a quantidade de elementos dela e todos os nós recuperados no vetor vet_rec
		Elementos(Recuperados->Raiz, &tam_rec, vet_rec);
		//Função pega a lista de mortos, retorna a quantidade de elementos dela e todos os nós mortos no vetor vet_rec
		Elementos(Mortos->Raiz, &tam_mor, vet_mor);

		//Printa a iteração, quantidade de infectados/recuperados e os nós
		//printf("\nIteracao: %d ", i+1);
		//printf("\nNumero de Infectados: %d, Lista de Infectados: ", tam_inf); EmOrdem(Infectados->Raiz);
		//printf("\nNumero de Recuperados: %d, Lista de Recuperados: ", tam_rec); EmOrdem(Recuperados->Raiz);
		fprintf(pont_arq, "%d\t%d\t%d\t%d\n", i + 1, tam_inf, tam_rec, tam_mor);

		for (j = 0; j < tam_inf; j++)
		{
			//Escolhe um nó infectado
			no = vet_inf[j];
			r = EscolheProb(100);

			//Se o teste VerificaProb retornar 1, então o nó infectado se recupera
			if (VerificaProb(Prob_Rec, r))
			{
				//Remove o no da lista de infectados.
				Remove(&(Infectados->Raiz), no);
				//Insere o no na lista de infectados (c/imunidade).
				Insere(&(Recuperados->Raiz), no);
				Map[no] = 0;
			}
		}

		for (j = 0; j < tam_inf; j++)
		{
			//Reseta a quantidade de nós na vizinhança do nó infectado
			tam_poss = 0;

			//Função pega a lista de adjacência do nó infectado j, obtém a quantidade de nós ligados a ele e todos os nós da sua vizinhança no vetor vet_poss
			Elementos(ListaAdj[vet_inf[j]]->Raiz, &tam_poss, vet_poss);

			//Verifica todos os nós ligados ao nó infectado j
			for (k = 0; k < tam_poss; k++)
			{
				no = vet_poss[k];
				pont_inf = Pesquisa(Infectados->Raiz, no);
				pont_rec = Pesquisa(Recuperados->Raiz, no);
				pont_mor = Pesquisa(Mortos->Raiz, no);
				//Se o nó nao estiver na lista de infectados nem na de mortos, fazemos o teste para verificar se ele se infectou
				if ((pont_inf == NULL) && (pont_mor == NULL))
				{
					r = EscolheProb(100);

					//Se o nó não estiver na lista de recuperados, ele tem uma prob Prob_Inf de se infectar
					if (pont_rec == NULL)
					{
						boo = VerificaProb(Prob_Inf, r);
					}
					//Se o nó estiver na lista de recuperados, ele tem uma prob Prob_InfRec de se infectar novamente
					else
					{
						boo = VerificaProb(Prob_InfRec, r);
					}

					//Se o teste retornar 1, o nó se infectou
					if (boo == 1)
					{
						//Insere nó na lista de infectados
						Insere(&(Infectados->Raiz), no);
						//Remove nó da lista de recuperados
						Remove(&(Recuperados->Raiz), no);
					}
				}
			}
		}
		for (j = 0; j < tam_inf; j++)
		{
			no = vet_inf[j];
			//Incrementamos o vetor map na posição do nó infectado em mais um
			Map[no]++;
			//Se chegar a 5, então o individuo morre
			if (Map[no] >= Morte)
			{
				//Inserimos o nó na lista de mortos
				Insere(&(Mortos->Raiz), no);
				//Removemos ele da lista de infectados
				Remove(&(Infectados->Raiz), no);
			}
		}
	}
	PrintaVertices(graph_f, Infectados, Recuperados, Mortos);

	PrintaMatriz(graph_f, mat_graph);

	//Fim de programa
	//Libera todos os espaços alocados
	for (n = 0; n < MAX; n++)
	{
		Liberdade(ListaAdj[n]->Raiz);
		free(ListaAdj[n]);
	}
	Liberdade(Infectados->Raiz);
	Liberdade(Recuperados->Raiz);
	system("pause"); //system("pause");
	return 0;
}

//-------------------------------------------- Funções ----------------------------------------------
pNo Pesquisa(pNo Raiz, TItem Item)
{
	if (Raiz == NULL)
		return NULL;

	if (Raiz->Item == Item)
		return Raiz;

	if (Item < Raiz->Item)
		return Pesquisa(Raiz->Esq, Item);
	return Pesquisa(Raiz->Dir, Item);
}

int Remove(pNo *Raiz, TItem Item)
{
	int a = 1;
	pNo aux;
	if ((*Raiz) == NULL)
		return 0;
	if ((*Raiz)->Item == Item)
	{
		if ((*Raiz)->Esq == NULL && (*Raiz)->Dir == NULL) //Folha
		{
			free((*Raiz));
			(*Raiz) = NULL;
		}
		else if ((*Raiz)->Esq != NULL && (*Raiz)->Dir != NULL) //Pior caso
		{
			for (aux = (*Raiz)->Dir; aux->Esq != NULL; aux = aux->Esq)
				;
			//Encontra menor valor na subarvore da direita

			(*Raiz)->Item = aux->Item; //Substitui valor

			if (!Remove(&(*Raiz)->Dir, aux->Item))
				return 0;
			Rot(Raiz);
		}
		else //Apenas Um Filho
		{
			if ((*Raiz)->Esq == NULL)
				aux = (*Raiz)->Dir; //Esquerda Livre
			else
				aux = (*Raiz)->Esq; //Direita Livre

			free((*Raiz));
			(*Raiz) = aux;
		}

		return 1;
	}

	if (Item < (*Raiz)->Item)
		a = Remove(&(*Raiz)->Esq, Item);
	else
		a = Remove(&(*Raiz)->Dir, Item);

	if (!a)
		return 0;

	Rot(Raiz);

	return 1;
}

void RR(pNo *Raiz)
{
	pNo A = (*Raiz), B = (*Raiz)->Esq; //Definições

	A->Esq = B->Dir;
	B->Dir = A;

	(*Raiz) = B;

	UpdateFB(A);
	UpdateFB(B); //Refaz os balanceamentos
}

void RL(pNo *Raiz)
{
	pNo A = (*Raiz), B = (*Raiz)->Esq, C = (*Raiz)->Esq->Dir; //Definições

	A->Esq = C->Dir;
	B->Dir = C->Esq;
	C->Dir = A;
	C->Esq = B;

	(*Raiz) = C; //Atualiza Raiz

	UpdateFB(A);
	UpdateFB(B);
	UpdateFB(C); //Refaz Balanceamento
}

void LL(pNo *Raiz)
{
	pNo A = (*Raiz), B = (*Raiz)->Dir; //Definições

	A->Dir = B->Esq;
	B->Esq = A;

	(*Raiz) = B;

	UpdateFB(A);
	UpdateFB(B); //Refaz os balanceamentos
}

void LR(pNo *Raiz)
{
	pNo A = (*Raiz), B = (*Raiz)->Dir, C = (*Raiz)->Dir->Esq; //Definições

	A->Dir = C->Esq;
	B->Esq = C->Dir;
	C->Esq = A;
	C->Dir = B;

	(*Raiz) = C; //Atualiza Raiz

	UpdateFB(A);
	UpdateFB(B);
	UpdateFB(C); //Refaz Balanceamento
}

void Rot(pNo *Raiz)
{
	if (*Raiz != NULL)
	{
		UpdateFB((*Raiz));

		if (!(-2 < (*Raiz)->fb && (*Raiz)->fb < 2))
		{
			if ((*Raiz)->fb == 2) //Arvore maior a Esquerda
			{
				if ((*Raiz)->Esq->fb != -1)
					RR(Raiz);
				else
					RL(Raiz);
			}
			else //Arvore maior a Direita
			{
				if ((*Raiz)->Dir->fb != 1)
					LL(Raiz);
				else
					LR(Raiz);
			}

			UpdateFB((*Raiz));
		}
	}
}

int Insere(pNo *Raiz, TItem Item)
{
	int a;

	//Casos de parada

	if (*Raiz == NULL) //Se o elemento encontrar seu lugar, insere
	{
		*Raiz = CriaNo(Item, NULL, NULL);
		return 1;
	}

	if ((*Raiz)->Item == Item)
		return 0; //Se o elemento já existir, retorna sem exito

	//Casos de processamento

	if (Item < (*Raiz)->Item) //Se o elemento é menor que a raiz
		a = Insere(&(*Raiz)->Esq, Item);
	else //Se o elemento é maior que a raiz
		a = Insere(&(*Raiz)->Dir, Item);

	if (!a)
		return 0;
	//se a inserção falhou, retorna 0

	Rot(Raiz);
	//Verifica se há necessidade de rotacionar, se houver, rotaciona

	return 1; //Retorna inserção balanceada
}

void PreOrdem(TNo *A)
{
	if (A != NULL)
	{
		printf("%d ", A->Item);
		PreOrdem(A->Esq);
		PreOrdem(A->Dir);
	}
}

void MegaEmOrdem(TABB *ListaAdj[MAX])
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		EmOrdem(ListaAdj[i]->Raiz);
		printf("\n");
	}
}

void EmOrdem(TNo *A)
{
	if (A != NULL)
	{
		EmOrdem(A->Esq);
		printf("%d ", A->Item);
		EmOrdem(A->Dir);
	}
}

void Liberdade(TNo *A) //Libera todo o espaço alocado
{
	if (A != NULL)
	{
		Liberdade(A->Esq);
		Liberdade(A->Dir);
		free(A);
	}
}

pNo CriaNo(TItem x, pNo Esq, pNo Dir)
{
	pNo aux = (TNo *)malloc(sizeof(TNo)); //Cria Nó
	aux->Dir = Dir;
	aux->Esq = Esq;
	aux->Item = x;
	aux->fb = 0;
	return aux;
}

TABB *Inicializar()
{
	TABB *A = (TABB *)malloc(sizeof(TABB));
	A->Raiz = NULL;
	return A;
}

int Altura(pNo A) //Calcula altura apartir do nó A
{
	if (A == NULL)
		return -1;

	int a = Altura(A->Esq), b = Altura(A->Dir);

	if (a > b)
		return a + 1; //retorna a maior altura
	return b + 1;
}

void UpdateFB(pNo A)
{
	if (A != NULL)
		A->fb = Altura(A->Esq) - Altura(A->Dir);
}

void Peso(TItem Item, int *peso, char op)
{
	if (op == 'w')
		Map[Item] = *peso;
	else if (op == 'r')
		*peso = Map[Item];
}

int InsGraph(TABB *Adj[MAX], TItem u, TItem v)
{
	if (Insere(&(Adj[u]->Raiz), v))
		return Insere(&(Adj[v]->Raiz), u);
	return 0;
}

int RemGraph(TABB *Adj[MAX], TItem u, TItem v)
{
	if (Remove(&(Adj[u]->Raiz), v))
		return Remove(&(Adj[v]->Raiz), u);
	return 0;
}

void Elementos(pNo Raiz, int *i, TItem V[MAX])
{
	if (Raiz != NULL)
	{
		Elementos(Raiz->Esq, i, V);

		V[(*i)] = Raiz->Item;
		(*i) += 1;

		Elementos(Raiz->Dir, i, V);
	}
}

int EscolheProb(int max)
{
	//Escolhe um numero aleatorio entre 0 e max
	return rand() % max;
}

int VerificaProb(float prob, int rand)
{
	//Numero aleatorio gerado é menor que a probabilidade definida => Retorna 1
	if (rand <= floor(100 * prob))
		return 1;
	else
		return 0;
}

void Inicializa_Infectados(TABB *Infectados)
{
	int i, r, cont = 0;
	pNo pont;
	while (cont < Inf_0)
	{
		if (Insere(&(Infectados->Raiz), EscolheProb(MAX)))
			cont++;
	}
}

void inicia_graforand(int **matriz, int grau)
{
	int i, j;
	float prob, r, k = grau, N = MAX;

	prob = k / (2 * N);
	for (i = 0; i < MAX; i++)
	{
		for (j = 0; j < MAX; j++)
		{
			matriz[i][j] = 0;
		}
	}

	for (i = 0; i < MAX; i++)
	{
		for (j = 0; j < MAX; j++)
		{
			r = (float)rand() / ((float)RAND_MAX / 1);
			if ((r <= prob) && (i != j))
			{
				matriz[i][j] = 1;
				matriz[j][i] = 1;
			}
		}
	}
}

void inicia_graforeg(int **matriz, int grau)
{
	int i, j, aux = grau;
	for (i = 0; i < MAX; i++)
		for (j = 0; j < MAX; j++)
			matriz[i][j] = 0;
	i = 0;
	//j=499;
	while (aux > 0)
	{
		matriz[0][aux] = 1;
		matriz[0][MAX - aux] = 1;
		matriz[aux][0] = 1;
		matriz[MAX - aux][0] = 1;
		aux--;
	}

	for (i = 1; i < MAX; i++)
	{
		for (j = i; j < MAX; j++)
		{
			matriz[i][j] = matriz[i - 1][j - 1];
			matriz[j][i] = matriz[j - 1][i - 1];
		}
	}
}

void inicia_grafolivre(int **mat_graph)
{
	int *vet, size = 2, i, j, no;
	vet = (int *)malloc(2 * sizeof(int));
	srand((unsigned)time(NULL));

	vet[0] = 0;
	vet[1] = 1;
	for (i = 0; i < MAX; i++)
		for (j = 0; j < MAX; j++)
			mat_graph[i][j] = 0;

	mat_graph[0][1] = 1;
	mat_graph[1][0] = 1;

	for (i = 2; i < 500; i++)
	{
		no = vet[rand() % size];
		vet = (int *)realloc(vet, 2 * i * sizeof(int));
		vet[2 * i - 1] = no;
		vet[2 * i - 2] = i;
		mat_graph[no][i] = 1;
		mat_graph[i][no] = 1;
		size = size + 2;
	}
}

void inicia_grafomod(int **mat_graph)
{
	int v_entrada, v_saida, i, j;
	FILE *fpointer;
	fpointer = fopen("network.txt", "r");

	for (i = 0; i < MAX; i++)
	{
		for (j = 0; j < MAX; j++)
			mat_graph[i][j] = 0;
	}

	while (!feof(fpointer))
	{
		fscanf(fpointer, "%d", &v_entrada);
		fscanf(fpointer, "%d", &v_saida);
		mat_graph[v_entrada - 1][v_saida - 1] = 1;
	}

	fclose(fpointer);
}

void CarregaGrafo(TABB *ListaAdj[MAX], int **mat_graph)
{
	int i, j;
	for (i = 0; i < MAX; i++)
		for (j = 0; j < MAX; j++)
			if (mat_graph[i][j] == 1)
				InsGraph(ListaAdj, i, j);
}

void GeraGrafo(int **mat_graph, int Escolha)
{
	switch (Escolha)
	{
	case 0:
		inicia_graforeg(mat_graph, Grau);
		break;
	case 1:
		inicia_graforand(mat_graph, Grau);
		break;
	case 2:
		inicia_grafolivre(mat_graph);
		break;
	case 3:
		inicia_grafomod(mat_graph);
		break;
	default:
		printf("Tipo de grafo inválido\n");
	}
}

void PrintaMatriz(FILE *graph, int **mat_graph)
{
	int i, j;
	fprintf(graph, "*Matrix\n");
	for (i = 0; i < MAX; i++)
	{
		for (j = 0; j < MAX; j++)
		{
			fprintf(graph, "%d ", mat_graph[i][j]);
		}
		fprintf(graph, "\n");
	}
}

void PrintaVertices(FILE *graph, TABB *Infectados, TABB *Recuperados, TABB *Mortos)
{
	int i, no;
	pNo pont_inf, pont_rec, pont_mor;

	fprintf(graph, "*Vertices %d\n", MAX);
	for (i = 0; i < MAX; i++)
	{
		no = i;
		pont_mor = Pesquisa(Mortos->Raiz, no);
		pont_inf = Pesquisa(Infectados->Raiz, no);
		if (pont_mor != NULL)
			fprintf(graph, "%d \"M\" ic Black bc Black", i + 1);
		else if (pont_inf == NULL)
			fprintf(graph, "%d \"S\ ic Green bc Green", i + 1);
		else
			fprintf(graph, "%d \"I\" ic Brown bc Brown", i + 1);

		fprintf(graph, "\n");
	}
}
