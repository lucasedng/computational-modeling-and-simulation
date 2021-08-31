#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define Prob_BInf 0.50 //Probabilidade do bebe suscetivel se infectar
#define Prob_CInf 0.30 //Probabilidade da criança suscetivel se infectar
#define Prob_JInf 0.20 //Probabilidade do jovem suscetivel se infectar
#define Prob_AInf 0.25 //Probabilidade do adulto suscetivel se infectar
#define Prob_IInf 0.55 //Probabilidade do idoso suscetivel se infectar

#define Prob_BRec 0.15 //Probabilidade do bebe infectado se recuperar
#define Prob_CRec 0.40 //Probabilidade da criança infectado se recuperar
#define Prob_JRec 0.50 //Probabilidade do jovem infectado se recuperar
#define Prob_ARec 0.35 //Probabilidade do adulto infectado se recuperar
#define Prob_IRec 0.20 //Probabilidade do idoso infectado se recuperar

#define Prob_BMor 0.75 //Probabilidade do bebe infectado morrer pela doença
#define Prob_CMor 0.50 //Probabilidade da criança infectado morrer pela doença
#define Prob_JMor 0.30 //Probabilidade do jovem infectado morrer pela doença
#define Prob_AMor 0.35 //Probabilidade do adulto infectado morrer pela doença
#define Prob_IMor 0.90 //Probabilidade do idoso infectado morrer pela doença

#define Prob_BInfRec 0.40 //Probabilidade do bebe recuperado se infectar
#define Prob_CInfRec 0.20 //Probabilidade da criança recuperado se infectar
#define Prob_JInfRec 0.15 //Probabilidade do jovem recuperado se infectar
#define Prob_AInfRec 0.15 //Probabilidade do adulto recuperado se infectar
#define Prob_IInfRec 0.40 //Probabilidade do idoso recuperado se infectar

#define B_0 150		  //Quantidade de bebes inicial
#define C_0 200 + B_0 //Quantidade de crianças inicial
#define J_0 200 + C_0 //Quantidade de jovens inicial
#define A_0 250 + J_0 //Quantidade de adultos inicial
#define I_0 200 + A_0 //Quantidade de idosos inicial

#define Nascimento 30 //Quantidade
#define Morte 50
#define ANO 50
#define Inf_0 500	 //Quantidade inicial de infectados
#define Ite 55 * ANO //Quantidade de iterações da simulação
#define TipoGrafo 3	 //Tipo de Grafo (Reg=0, Rand=1, Livre de escala=2, Mod= 3)
//#define Grau 4                //Grau medio/2
/*Para inserir uma rede modular qualquer, mude o nome do arquivo da função inicia_grafomod e coloque
TipoGrafo= 3 na constante acima.
*/

int MAX = 1000;

#define nMax MAX + Nascimento

#define TAM 20000
//-------------------------------------------- Structs --------------------------------------------

typedef struct SItem
{

	int Idade, Chave;
	int dias;
	float Peso;
} TItem;

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

pNo Pesquisa(pNo Raiz, TItem Item);
int Insere(pNo *Raiz, TItem Item);
int Remove(pNo *Raiz, TItem Item);
void Rot(pNo *Raiz);
void EmOrdem(TNo *A);
void Liberdade(TNo *A);
pNo CriaNo(TItem x, pNo Esq, pNo Dir);
TABB *Inicializar();
int Altura(pNo A);
void UpdateFB(pNo A);

//-------------------------------------------- Funções ----------------------------------------------
pNo Pesquisa(pNo Raiz, TItem Item)
{
	if (Raiz == NULL)
		return NULL;

	if (Raiz->Item.Chave == Item.Chave)
		return Raiz;

	if (Item.Chave < Raiz->Item.Chave)
		return Pesquisa(Raiz->Esq, Item);
	return Pesquisa(Raiz->Dir, Item);
}

int Remove(pNo *Raiz, TItem Item)
{
	int a = 1;
	pNo aux;

	if ((*Raiz) == NULL)
		return 0;

	if ((*Raiz)->Item.Chave == Item.Chave)
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

	if (Item.Chave < (*Raiz)->Item.Chave)
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

	if ((*Raiz)->Item.Chave == Item.Chave)
		return 0; //Se o elemento já existir, retorna sem exito

	//Casos de processamento

	if (Item.Chave < (*Raiz)->Item.Chave) //Se o elemento é menor que a raiz
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
		printf("%d ", A->Item.Chave);
		PreOrdem(A->Esq);
		PreOrdem(A->Dir);
	}
}

void EmOrdem(TNo *A)
{
	if (A != NULL)
	{
		EmOrdem(A->Esq);
		printf("%d ", A->Item.Chave);
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

void teste(pNo A)
{
	printf("\n-----: ");
	PreOrdem(A);
	printf(" :-----\n");
}

void MegaEmOrdem(TABB **ListaAdj) // Vetor
{
	int i;
	for (i = 0; i < MAX; i++)
	{
		EmOrdem(ListaAdj[i]->Raiz);
		printf("\n");
	}
}

int InsGraph(TABB **Adj, TItem u, TItem v) // Vetor
{
	if (Insere(&(Adj[u.Chave]->Raiz), v))
		return Insere(&(Adj[v.Chave]->Raiz), u);
	return 0;
}

int RemGraph(TABB **Adj, TItem u, TItem v) // Vetor
{
	if (Remove(&(Adj[u.Chave]->Raiz), v))
		return Remove(&(Adj[v.Chave]->Raiz), u);
	return 0;
}

void Elementos(pNo Raiz, int *i, TItem *V) // Vetor
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

void Inicializa_Infectados(TABB **ListaAdj, TABB *Infectados)
{
	int cont = 0, c;
	TItem no;
	pNo pont;
	while (cont < Inf_0)
	{
		no.Chave = EscolheProb(MAX);
		int c = 0;
		pont = NULL;
		while (pont == NULL)
			pont = Pesquisa(ListaAdj[c++]->Raiz, no);

		no = pont->Item;
		if (Insere(&(Infectados->Raiz), no))
			cont++;
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

void inicia_grafomod(float **mat_graph)
{
	int v_entrada, v_saida, i, j;
	float peso;
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
		fscanf(fpointer, "%f", &peso);
		mat_graph[v_entrada - 1][v_saida - 1] = peso;
	}

	fclose(fpointer);
}

void Embaralhar(int *v)
{
	int i, pos, aux;
	for (i = 0; i < MAX; i++)
	{
		pos = EscolheProb(MAX);
		aux = v[i];
		v[i] = v[pos];
		v[pos] = aux;
	}
}

int EscolheRandIntervalo(int min, int max)
{
	return (rand() % (max - min) + min);
}

int EscolheIdade(int classe)
{
	switch (classe)
	{
	case 0:
		return EscolheRandIntervalo(0, 3);

	case 1:
		return EscolheRandIntervalo(4, 15);

	case 2:
		return EscolheRandIntervalo(16, 29);

	case 3:
		return EscolheRandIntervalo(30, 59);

	case 4:
		return EscolheRandIntervalo(60, 80);
	}
}

TItem InicializaNo(int chave, int idade, float peso)
{
	TItem no;
	no.Chave = chave;
	no.Idade = idade;
	no.Peso = peso;
	no.dias = 0;
	return no;
}

void CarregaGrafo(TABB **ListaAdj, float **mat_graph, TABB *Adultos, TABB *Idosos) // Vetor
{
	TItem x, y;
	int i, j;
	int *idade = (int *)malloc(sizeof(int) * MAX);

	//inicialização

	for (i = 0; i < B_0; i++)
		idade[i] = EscolheIdade(0);

	for (i = B_0; i < C_0; i++)
		idade[i] = EscolheIdade(1);

	for (i = C_0; i < J_0; i++)
		idade[i] = EscolheIdade(2);

	for (i = J_0; i < A_0; i++)
		idade[i] = EscolheIdade(3);

	for (i = A_0; i < I_0; i++)
		idade[i] = EscolheIdade(4);

	Embaralhar(idade);

	//----------------------

	for (i = 0; i < MAX; i++)
	{
		for (j = 0; j < MAX; j++)
		{
			if (mat_graph[i][j] != 0)
			{
				x = InicializaNo(i, idade[i], mat_graph[i][j]);
				y = InicializaNo(j, idade[j], mat_graph[i][j]);
				InsGraph(ListaAdj, x, y);

				if (idade[i] >= 60)
					Insere(&(Idosos->Raiz), x);
				else if (idade[i] >= 30)
					Insere(&(Adultos->Raiz), x);

				if (idade[j] >= 60)
					Insere(&(Idosos->Raiz), y);
				else if (idade[j] >= 30)
					Insere(&(Adultos->Raiz), y);
			}
		}
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
	int i;
	TItem no;
	pNo pont_inf, pont_mor;

	fprintf(graph, "*Vertices %d\n", MAX);
	for (i = 0; i < MAX; i++)
	{
		no.Chave = i;
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

void OrdenarPorPeso(TItem *V, int n) // Vetor
{
	TABB *Ordena = Inicializar();
	TItem aux;
	int i, j;

	for (i = 0; i < n - 1; i++)
		for (j = i + 1; j < n; j++)
			if (V[j].Peso > V[i].Peso)
			{
				aux = V[j];
				V[j] = V[i];
				V[i] = aux;
			}
}

int DefineClasse(TItem Item)
{
	if (Item.Idade <= 3)
		return 0;
	else if (Item.Idade <= 15)
		return 1;
	else if (Item.Idade <= 29)
		return 2;
	else if (Item.Idade <= 59)
		return 3;
	else if (Item.Idade <= 100)
		return 4;
	else
		return 404;
}

float DefineProbRec(int dias, int classe)
{
	float aux;
	switch (classe)
	{
	case 0:
		aux = (Prob_BRec - (dias / 50));
		break;
	case 1:
		aux = (Prob_CRec - (dias / 50));
		break;
	case 2:
		aux = (Prob_JRec - (dias / 50));
		break;
	case 3:
		aux = (Prob_ARec - (dias / 50));
		break;
	case 4:
		aux = (Prob_IRec - (dias / 50));
		break;
	}
	if (aux > 0)
		return aux;
	else
		return 0;
}

float DefineProbMor(int dias, int classe)
{
	float aux;
	switch (classe)
	{
	case 0:
		aux = (Prob_BMor + (dias / 50));
		break;
	case 1:
		aux = (Prob_CMor + (dias / 50));
		break;
	case 2:
		aux = (Prob_JMor + (dias / 50));
		break;
	case 3:
		aux = (Prob_AMor + (dias / 50));
		break;
	case 4:
		aux = (Prob_IMor + (dias / 50));
		break;
	}
	if (aux < 1)
		return aux;
	else
		return 1;
}

float DefineProbInf(int classe)
{
	switch (classe)
	{
	case 0:
		return Prob_BInf;
	case 1:
		return Prob_CInf;
	case 2:
		return Prob_JInf;
	case 3:
		return Prob_AInf;
	case 4:
		return Prob_IInf;
	}
}

float DefineProbInfRec(int classe)
{
	switch (classe)
	{
	case 0:
		return Prob_BInfRec;
	case 1:
		return Prob_CInfRec;
	case 2:
		return Prob_JInfRec;
	case 3:
		return Prob_AInfRec;
	case 4:
		return Prob_IInfRec;
	}
}

void atualizaItem(TABB *Arvore, TItem novo)
{
	pNo aux = Pesquisa(Arvore->Raiz, novo);
	if (aux != NULL)
		aux->Item = novo;
}

void EscolhePais(TItem *vet_adu, int tam, TItem *y, TItem *z)
{
	int rand1, rand2;

	rand1 = EscolheProb(tam);

	rand2 = EscolheProb(tam);

	while (rand1 == rand2)
	{
		rand2 = EscolheProb(tam);
	}

	y->Chave = vet_adu[rand1].Chave;
	z->Chave = vet_adu[rand2].Chave;
}

void ProxAno(pNo Raiz) // Vetor
{
	if (Raiz != NULL)
	{
		ProxAno(Raiz->Esq);

		Raiz->Item.Idade += 1;

		ProxAno(Raiz->Dir);
	}
}

void ReposicionarIdade(TABB *Velho, TABB *Novo, int idade, int alfa)
{
	int tam = 0, i;
	TItem *V = (TItem *)malloc(sizeof(TItem) * MAX);

	Elementos(Velho->Raiz, &tam, V);

	if (idade < 0)
	{
		for (i = 0; i < tam; i++)
		{
			if (V[i].Idade <= idade)
			{
				if (alfa)
					Remove(&(Velho->Raiz), V[i]);
				Insere(&(Novo->Raiz), V[i]);
			}
		}
	}
	else
	{
		for (i = 0; i < tam; i++)
		{
			if (V[i].Idade >= idade)
			{
				if (alfa)
					Remove(&(Velho->Raiz), V[i]);
				Insere(&(Novo->Raiz), V[i]);
			}
		}
	}

	free(V);
}

//-------------------------------------------- Main --------------------------------------------

int main()
{
	int n, j, i, k, r, tam_inf = 0, tam_adu, tam_rec = 0, tam_mor = 0, tam_poss = 0, tam_ido = 0;
	float **mat_graph;

	TItem *vet_inf = (TItem *)malloc(sizeof(TItem) * TAM);
	TItem *vet_poss = (TItem *)malloc(sizeof(TItem) * TAM);
	TItem *vet_adu = (TItem *)malloc(sizeof(TItem) * TAM);
	TItem *vet_rec = (TItem *)malloc(sizeof(TItem) * TAM);
	TItem *vet_mor = (TItem *)malloc(sizeof(TItem) * TAM);
	TItem *vet_ido = (TItem *)malloc(sizeof(TItem) * TAM);
	TItem *vetaux = (TItem *)malloc(sizeof(TItem) * TAM);
	TItem *vetauxx = (TItem *)malloc(sizeof(TItem) * TAM);

	TItem no;
	pNo pont_inf, pont_rec, pont_mor;

	// Inicializações

	FILE *pont_arq;
	pont_arq = fopen("ProjetoFinal.txt", "wt");

	fprintf(pont_arq, "t \t I \t R \t M \t N \t Id\n");

	TABB *Infectados = Inicializar();
	TABB *Recuperados = Inicializar();
	TABB *Mortos = Inicializar();

	//Add conforme o necessário
	TABB *Adultos = Inicializar();
	TABB *Idosos = Inicializar();

	TABB **ListaAdj = (TABB **)malloc(sizeof(TABB *) * TAM);

	mat_graph = (float **)malloc(sizeof(float *) * MAX);

	for (n = 0; n < MAX; n++)
	{
		ListaAdj[n] = Inicializar();
		mat_graph[n] = (float *)malloc(sizeof(float) * MAX);
	}

	//Gera grafo
	inicia_grafomod(mat_graph);

	//Carrega grafo na lista de adjacência
	CarregaGrafo(ListaAdj, mat_graph, Adultos, Idosos);

	//Printa a lista de adjacência
	//MegaEmOrdem(ListaAdj);

	//Inicializa lista de infectados
	Inicializa_Infectados(ListaAdj, Infectados);

	//PrintaVertices(graph_i, Infectados);

	//PrintaMatriz(graph_i, mat_graph);

	for (i = 0; i < Ite; i++)
	{
		//Reseta a quantidade de infectados para 0
		tam_inf = 0;
		//Reseta a quantidade de recuperados para 0
		tam_rec = 0;
		//Reseta a quantidade de mortos para 0
		tam_mor = 0;

		tam_ido = 0;

		//Função pega a lista de Infectados, retorna a quantidade de elementos dela e todos os nós infectados no vetor vet_inf
		Elementos(Infectados->Raiz, &tam_inf, vet_inf);
		//Função pega a lista de recuperados, retorna a quantidade de elementos dela e todos os nós recuperados no vetor vet_rec
		Elementos(Recuperados->Raiz, &tam_rec, vet_rec);
		//Função pega a lista de mortos, retorna a quantidade de elementos dela e todos os nós mortos no vetor vet_rec
		Elementos(Mortos->Raiz, &tam_mor, vet_mor);
		Elementos(Idosos->Raiz, &tam_ido, vet_ido);

		k = 0; // Identificador se alguem morreu

		// Mortos ou Recuperados
		for (j = 0; j < tam_inf; j++)
		{
			no = vet_inf[j];
			r = EscolheProb(100);

			//Verifico se ele se recuperou
			if (VerificaProb(DefineProbRec(no.dias, DefineClasse(no)), r))
			{
				no.dias = 0;
				Remove(&(Infectados->Raiz), no);
				Insere(&(Recuperados->Raiz), no);
			}
			//Não se recuperou, verifico se morreu
			else if ((VerificaProb(DefineProbMor(no.dias, DefineClasse(no)), r)))
			{
				k = 1; // Altera estado

				int c = 0;

				// Desfaz as ligações do elemento no grafo, e onde mais ele possa existir

				Elementos(ListaAdj[no.Chave]->Raiz, &c, vetauxx);

				for (c = c - 1; c >= 0; c--)
					RemGraph(ListaAdj, no, vetauxx[c]);

				Remove(&(Adultos->Raiz), no);
				Remove(&(Idosos->Raiz), no);
				// ----------------------------------------

				no.Idade = -1; // Marca para ser removido depois

				atualizaItem(Infectados, no);

				goto End; // Pula atualização
			}
			else
				no.dias++;

			//Atualizador de listas

			//Atualiza itens das listas

			int c = 0;
			Elementos(ListaAdj[no.Chave]->Raiz, &c, vetaux);

			for (c = c - 1; c >= 0; c--)
				atualizaItem(ListaAdj[vetaux[c].Chave], no);

			//atualizar as outras listas
			atualizaItem(Infectados, no);
			atualizaItem(Recuperados, no);
			atualizaItem(Idosos, no);
			atualizaItem(Adultos, no);

		End:
			continue;
		}

		if (k) // Atualiza infectados, caso alguém morreu
		{
			ReposicionarIdade(Infectados, Mortos, -1, 1);
			tam_inf = 0;
			Elementos(Infectados->Raiz, &tam_inf, vet_inf);
		}

		//Verifico se infectou seus vizinhos, que certamente estão vivos
		for (j = 0; j < tam_inf; j++)
		{
			tam_poss = 0;

			Elementos(ListaAdj[vet_inf[j].Chave]->Raiz, &tam_poss, vet_poss); // Vizinhança do Infectado

			for (k = 0; k < tam_poss; k++)
			{
				no = vet_poss[k];

				if (Pesquisa(Infectados->Raiz, no) == NULL) // Verifica se já é um Infectado
				{
					r = EscolheProb(100);
					if (VerificaProb(no.Peso, r)) //Verifica se entrou em contato
					{
						r = EscolheProb(100);
						if (Pesquisa(Recuperados->Raiz, no) == NULL) //NÃO Recuperado se infectou
						{
							if (VerificaProb(DefineProbInf(DefineClasse(no)), r))
								Insere(&(Infectados->Raiz), no);
						}
						else //Recuperado se infectou
						{
							if (VerificaProb(DefineProbInfRec(DefineClasse(no)), r))
							{
								Insere(&(Infectados->Raiz), no);
								Remove(&(Recuperados->Raiz), no);
							}
						}
					}
				}
			}
		}

		//Printa a iteração, quantidade de infectados/recuperados e os nós
		//printf("\nIteracao: %d ", i+1);
		//printf("\nNumero de Infectados: %d, Lista de Infectados: ", tam_inf); EmOrdem(Infectados->Raiz);
		//printf("\nNumero de Recuperados: %d, Lista de Recuperados: ", tam_rec); EmOrdem(Recuperados->Raiz);
		fprintf(pont_arq, "%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, tam_inf, tam_rec, tam_mor, MAX - tam_mor, tam_ido);
		printf("%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, tam_inf, tam_rec, tam_mor, MAX - tam_mor, tam_ido);
		if (i % 55 == 0)
		{
			//Atualizar idade de todo mundo
			for (j = 0; j < MAX; j++)
				ProxAno(ListaAdj[j]->Raiz);

			ProxAno(Infectados->Raiz);
			ProxAno(Recuperados->Raiz);
			ProxAno(Idosos->Raiz);
			ProxAno(Adultos->Raiz);

			//Se idade >=60 (Inserir na lista de Idosos)
			ReposicionarIdade(Adultos, Idosos, 60, 1);

			//If else idade >=30 (Inserir na lista de Adultos)
			for (j = 0; j < MAX; j++)
				ReposicionarIdade(ListaAdj[j], Adultos, 30, 0);

			//Nascimentos -------------------------------------------
			int c;

			// multiplicar MAX por 19 !

			tam_adu = 0;
			Elementos(Adultos->Raiz, &tam_adu, vet_adu);

			for (c = MAX; c < nMax; c++)
			{
				ListaAdj[c] = Inicializar();
				// fazer ligações

				TItem x, y, z;
				x.Chave = c;
				x.Idade = 0;
				x.dias = 0;

				EscolhePais(vet_adu, tam_adu, &y, &z);

				x.Peso = (EscolheRandIntervalo(80, 100)) / 100;

				InsGraph(ListaAdj, x, y);

				InsGraph(ListaAdj, x, z);

				int ligacoes = 0;

				while (ligacoes < 8)
				{
					no.Chave = EscolheProb(MAX);
					pNo pont;
					if (Pesquisa(Mortos->Raiz, no) == NULL)
					{
						j = 0;
						pont = NULL;
						while (pont == NULL && j < MAX)
						{
							pont = Pesquisa(ListaAdj[j]->Raiz, no);
							j++;
						}

						if (j < MAX)
						{
							x.Peso = EscolheProb(100) / 100;
							no = pont->Item;
							no.Peso = x.Peso;
							InsGraph(ListaAdj, x, no);
						}

						ligacoes++;
					}
				}
			}

			MAX = nMax;

			//Mortes Naturais
			tam_ido = 0;
			Elementos(Idosos->Raiz, &tam_ido, vet_ido);
			int escolhidos = 0;

			while (escolhidos < 8)
			{
				no = vet_ido[EscolheProb(tam_ido)];
				if (Pesquisa(Mortos->Raiz, no) == NULL)
				{
					int c = 0;

					Elementos(ListaAdj[no.Chave]->Raiz, &c, vetauxx);

					for (c = c - 1; c >= 0; c--)
						RemGraph(ListaAdj, no, vetauxx[c]);

					Remove(&(Idosos->Raiz), no);
					Insere(&(Mortos->Raiz), no);
					escolhidos++;
				}
			}
		}

		// Colocar dentro do fim de ano
	}

	int var = nMax;
	//Fim de programa
	//Libera todos os espaços alocados
	{
		free(vet_inf);
		free(vet_poss);
		free(vet_adu);
		free(vet_rec);
		free(vet_mor);
		free(vetauxx);
		free(vetauxx);

		for (n = 0; n < MAX; n++)
		{
			Liberdade(ListaAdj[n]->Raiz);
			free(ListaAdj[n]);
		}

		Liberdade(Infectados->Raiz);
		Liberdade(Recuperados->Raiz);
	}

	system("pause"); //system("pause");
	return 0;
}
