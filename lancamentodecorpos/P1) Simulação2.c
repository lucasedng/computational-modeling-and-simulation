/*
    Celso Gabriel Vieira Robeiro Lopes (123119),
    Felipe Hikari Kawahama (112197),
    Lucas Eduardo Nogueira Gonçalves (122055)
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Condições iniciais

#define vo 20            //Vel. Inicial (m/s)
#define soy 50           //Alt. Inicial  (m)
#define g 9, 8           //Gravidade (m/s²)
#define m 100            //Massa do corpo (kg)
#define k 0.027          //Coef. de Atrito (-)
#define deltat 0.01      //Passo da integração (s)
#define pi 3.14159265359 //Valor de pi (-)
#define theta pi / 3     //Ângulo de lançamento (rad)

int main()
{
    //Criando um arquivo .txt para guardar os dados
    FILE *pont_arq;
    pont_arq = fopen("Simulação2.txt", "wt");
    if (pont_arq == NULL)
    {
        printf("Problemas na CRIACAO do arquivo\n");
        return 404;
    }

    //Declarando as variáveis com as mesmas unidades dos parâmetros
    float vx, vy, sy, sx, ax, ay, t, K, v, U;
    /*
    sx= Deslocamento em x
    sy= Deslocamento em y
    vx= Velocidade em x
    vy= Velocidade em y
    v= Velocidade em módulo
    ax= Aceleração em x
    ay= Aceleração em y
    t= Tempo
    K= Energia cinética
    U= Energia Potencial
    */
    vx = (vo * cos(theta)); //Inicializando vx, decompondo a velocidade inicial v0
    vy = (vo * sin(theta)); //Inicializando vy, decompondo a velocidade inicial v0
    sx = 0;                 //Inicializando posição inicial em x
    sy = soy;               //Inicializando posição inicial em y

    //Enquanto o corpo de massa m não chegar ao solo, irei calcular as variáveis
    while (sy > 0)
    {
        ax = -(k * vx) / m;                               //Vem da decomposição de forças em x
        ay = -g + (k * vy) / m;                           //Vem da decomposição de forças em y
        vx = vx + (ax * deltat);                          //Método de Euler para achar vx
        vy = vy + (ay * deltat);                          //Método de Euler para achar vy
        v = sqrt((vx * vx) + (vy * vy));                  //Módulo da Velocidade
        sx = sx + (vx * deltat) + (ax * deltat * deltat); //Método de Euler para achar sx
        sy = sy + (vy * deltat) + (ay * deltat * deltat); //Método de Euler para achar sy
        K = (m * v * v) / 2;                              //Fórmula da Energia Cinética
        U = (m * g * sy);                                 //Fórmula da Energia Potencial
        t = t + deltat;                                   //Incremento o tempo
        //Printa os resultados no prompt
        printf("\n vx=%f\n vy=%f\n v=%f\n sx=%f\n sy=%f\n t=%f\n K=%f\n U=%f\n", vx, vy, v, sx, sy, t, K, U);
        //Printa os resultados no arquivo .txt
        fprintf(pont_arq, "\n vx=%f\n vy=%f\n v=%f\n sx=%f\n sy=%f\n t=%f\n K=%f\n U=%f\n", vx, vy, v, sx, sy, t, K, U);
    }

    //Fecha arquivo, finaliza simulação
    fclose(pont_arq);
    return 0;
}
