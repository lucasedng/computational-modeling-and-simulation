// Modelagem Computacional, Projeto 2, Simulação 1
// Celso Gabriel Vieira Robeiro Lopes (123119), Felipe Hikari Kawahama (112197), Lucas Eduardo Nogueira Gonçalves (122055).

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    float g1, g2, a1, a2, l1, l2, deltaG, deltaR, deltaV, deltaA, deltaL, G, R, V, A, L;
    float t;
    FILE *pont_arq;
    pont_arq = fopen("Sim2.txt", "wt");
    if (pont_arq == NULL)
    {
        printf("Problemas na CRIACAO do arquivo\n");
        return 404;
    }

    float Kappa_g = 8;
    float Kconst_g = 1000;
    float Beta_g = 0.1;
    float Psi_g = 0.1;
    float Theta_r = 0.06;
    float Theta_v = 0.06;
    float Theta_a = 0.0299999;
    float Theta_l = 0.001;
    float Theta_l_2 = 0.03;
    float gamma_r_1 = 0.65;
    float gamma_r_2 = 0.001;
    float mu_r = 0.09;
    float mu_v = 0.03;
    float mu_a = 0.8;
    float mu_l = 0.99;
    float phi_v = 0.87;
    float pi = 3.14159265359;
    float deltat = 0.001;

    g1 = 24.000000;
    g2 = 24.001000;
    a1 = 25.000000;
    a2 = 25.001000;
    l1 = 15.000000;
    l2 = 15.001000;

    G = 130;
    R = 30;
    V = 5;
    A = 10;
    L = 4;

    for (t = 0.0; t < 160; t = t + deltat)
    {
        deltaG = G * deltat * ((Kappa_g * (1 - G / Kconst_g)) - ((Beta_g * V) + (Psi_g * R)));
        deltaR = R * deltat * ((Theta_r * G) - ((gamma_r_1 * A) + (gamma_r_2 * L) + mu_r));
        deltaV = V * deltat * ((Theta_v * G) - ((phi_v * L) + mu_v));
        deltaA = A * deltat * ((Theta_a * R) - mu_a);
        deltaL = L * deltat * ((Theta_l * R) + (Theta_l_2 * V) - mu_l);

        G = G + deltaG;
        R = R + deltaR;
        V = V + deltaV;
        A = A + deltaA;
        L = L + deltaL;

        //Quando o tempo for maior que 35 e menor que 60, a população de vacas pega uma doença e começam a morrer.
        /*
        if(t>35 && t<60){
            mu_v=4;
        }

        if(t> 60){
            mu_v = 0.03;
        }*/

        //// Quando o tempo for maior que 35 e menor que 45, a população dos ratos aumenta
        /*
    if(t>35 && t<45){
        Theta_r=0.9999999;
    }
    if(t>45)
        Theta_r= 0.06;
*/

        //// Variação da Grama
        /*
        if(t>=g1&&t<=g2){
            G=G*0.5;
            g1+=24;
            g2+=24;
        }
*/

        printf("%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\n", G, R, V, A, L, t);
        //Imprime os resultados no arquivo .txt
        fprintf(pont_arq, "%f\t%f\t%f\t%f\t%f\t%f\n", G, R, V, A, L, t);
    }

    fclose(pont_arq);
    return 0;
}
