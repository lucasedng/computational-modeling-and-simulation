/*
    Celso Gabriel Vieira Robeiro Lopes (123119),
    Felipe Hikari Kawahama (112197),
    Lucas Eduardo Nogueira Gon�alves (122055)
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Condi��es iniciais

#define vo 20            //Vel. Inicial (m/s)
#define soy 50           //Alt. Inicial  (m)
#define g 9, 8           //Gravidade (m/s�)
#define m 100            //Massa do corpo (kg)
#define k 0.027          //Coef. de Atrito (-)
#define deltat 0.01      //Passo da integra��o (s)
#define pi 3.14159265359 //Valor de pi (-)
#define theta pi / 3     //�ngulo de lan�amento (rad)

int main()
{
    //Criando um arquivo .txt para guardar os dados
    FILE *pont_arq;
    pont_arq = fopen("Simula��o2.txt", "wt");
    if (pont_arq == NULL)
    {
        printf("Problemas na CRIACAO do arquivo\n");
        return 404;
    }

    //Declarando as vari�veis com as mesmas unidades dos par�metros
    float vx, vy, sy, sx, ax, ay, t, K, v, U;
    /*
    sx= Deslocamento em x
    sy= Deslocamento em y
    vx= Velocidade em x
    vy= Velocidade em y
    v= Velocidade em m�dulo
    ax= Acelera��o em x
    ay= Acelera��o em y
    t= Tempo
    K= Energia cin�tica
    U= Energia Potencial
    */
    vx = (vo * cos(theta)); //Inicializando vx, decompondo a velocidade inicial v0
    vy = (vo * sin(theta)); //Inicializando vy, decompondo a velocidade inicial v0
    sx = 0;                 //Inicializando posi��o inicial em x
    sy = soy;               //Inicializando posi��o inicial em y

    //Enquanto o corpo de massa m n�o chegar ao solo, irei calcular as vari�veis
    while (sy > 0)
    {
        ax = -(k * vx) / m;                               //Vem da decomposi��o de for�as em x
        ay = -g + (k * vy) / m;                           //Vem da decomposi��o de for�as em y
        vx = vx + (ax * deltat);                          //M�todo de Euler para achar vx
        vy = vy + (ay * deltat);                          //M�todo de Euler para achar vy
        v = sqrt((vx * vx) + (vy * vy));                  //M�dulo da Velocidade
        sx = sx + (vx * deltat) + (ax * deltat * deltat); //M�todo de Euler para achar sx
        sy = sy + (vy * deltat) + (ay * deltat * deltat); //M�todo de Euler para achar sy
        K = (m * v * v) / 2;                              //F�rmula da Energia Cin�tica
        U = (m * g * sy);                                 //F�rmula da Energia Potencial
        t = t + deltat;                                   //Incremento o tempo
        //Printa os resultados no prompt
        printf("\n vx=%f\n vy=%f\n v=%f\n sx=%f\n sy=%f\n t=%f\n K=%f\n U=%f\n", vx, vy, v, sx, sy, t, K, U);
        //Printa os resultados no arquivo .txt
        fprintf(pont_arq, "\n vx=%f\n vy=%f\n v=%f\n sx=%f\n sy=%f\n t=%f\n K=%f\n U=%f\n", vx, vy, v, sx, sy, t, K, U);
    }

    //Fecha arquivo, finaliza simula��o
    fclose(pont_arq);
    return 0;
}
