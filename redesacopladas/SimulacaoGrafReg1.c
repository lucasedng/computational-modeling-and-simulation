#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define Theta 0.5
#define Alpha 6.0
#define Epsilon 0.02
#define Beta 0.1
#define w 0.75
#define Iativo 0.2
#define Inativo -0.02
#define TMAX 9000
#define DT 0.01

typedef struct neuronio
{
    float x, y, I, S;
} Neuronio;

void inicia_graforeg(int mat_graph[500][500])
{
    int i, j;
    for (i = 0; i < 500; i++)
        for (j = 0; j < 500; j++)
            mat_graph[i][j] = 0;

    mat_graph[0][1] = 1;
    mat_graph[0][2] = 1;
    mat_graph[0][498] = 1;
    mat_graph[0][499] = 1;

    mat_graph[1][0] = 1;
    mat_graph[1][2] = 1;
    mat_graph[1][3] = 1;
    mat_graph[1][499] = 1;

    mat_graph[498][0] = 1;
    mat_graph[498][499] = 1;
    mat_graph[498][497] = 1;
    mat_graph[498][496] = 1;

    mat_graph[499][0] = 1;
    mat_graph[499][1] = 1;
    mat_graph[499][498] = 1;
    mat_graph[499][497] = 1;

    mat_graph[499][1] = 1;
    mat_graph[499][497] = 1;
    mat_graph[499][0] = 1;
    mat_graph[499][498] = 1;

    for (i = 2; i < 498; i++)
    {
        mat_graph[i][i - 1] = 1;
        mat_graph[i][i + 1] = 1;
        mat_graph[i][i - 2] = 1;
        mat_graph[i][i + 2] = 1;
    }
}

void inicializa_x(Neuronio *neuro)
{
    int max_x = 4, i;
    float rx;
    srand((unsigned)time(NULL));
    for (i = 0; i < 500; i++)
    {
        rx = max_x * ((float)rand() / (float)RAND_MAX - 0.5);
        neuro[i].x = rx;
    }
}

void inicializa_y(Neuronio *neuro)
{
    int max_y = 4, i;
    float ry;
    srand((unsigned)time(NULL));
    for (i = 0; i < 500; i++)
    {
        ry = (float)rand() / ((float)RAND_MAX / max_y);
        neuro[i].y = ry;
    }
}

void inicializa_i(Neuronio *neuro)
{
    int i;
    for (i = 0; i < 500; i++)
        neuro[i].I = Iativo;
}

int main()
{
    Neuronio neuro[500];
    int mat_graph[500][500], i, j, cont_ite = 0;
    float t, DeltaX, DeltaY;
    FILE *pont_arq;
    pont_arq = fopen("GrafoReg(1).txt", "wt");

    if (pont_arq == NULL)
    {
        printf("Problemas na CRIACAO do arquivo\n");
        return 404;
    }

    inicia_graforeg(mat_graph);

    //Simulação 1, todos começam ativos
    inicializa_i(neuro);
    inicializa_x(neuro);
    inicializa_y(neuro);

    //Inicializando o cabeçalho do txt
    fprintf(pont_arq, "t,");
    for (i = 0; i < 150; i++)
    {
        fprintf(pont_arq, "x%d,", 2 * i);
    }
    fprintf(pont_arq, "\n");

    //Começando a simulação
    for (t = 0; t < TMAX; t = t + DT)
    {
        //Calcula S de cada neuro
        for (i = 0; i < 500; i++)
        {
            neuro[i].S = 0; //Inicializando S do neuronio i
            for (j = 0; j < 500; j++)
            {
                //Se j está na vizinhança de i, checa se o x de j é maior que Theta
                if ((mat_graph[i][j] == 1) && (neuro[j].x >= Theta))
                {
                    neuro[i].S = neuro[i].S + w;
                }
            }
        }
        for (i = 0; i < 500; i++)
        {
            DeltaX = DT * (((3 * neuro[i].x) - (pow(neuro[i].x, 3)) + 2 - neuro[i].y + neuro[i].S + neuro[i].I));
            DeltaY = DT * (Epsilon * (Alpha * (1 + tanh(neuro[i].x / Beta)) - neuro[i].y));
            neuro[i].x = neuro[i].x + DeltaX;
            neuro[i].y = neuro[i].y + DeltaY;
        }
        if (cont_ite == 100)
        {
            fprintf(pont_arq, "%f,", t);
            for (i = 0; i < 150; i++)
            {
                fprintf(pont_arq, "%f,", neuro[2 * i].x);
            }
            fprintf(pont_arq, "\n");
            cont_ite = 0;
        }
        else
            cont_ite++;
    }
}
