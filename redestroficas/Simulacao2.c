// Modelagem Computacional, Projeto 2, Simulação 2
// Celso Gabriel Vieira Robeiro Lopes (123119), Felipe Hikari Kawahama (112197), Lucas Eduardo Nogueira Gonçalves (122055).

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    float l1, l2, p1, p2, v1, v2, i1, i2, deltaG, deltaR, deltaV, deltaA, deltaL, G, R, V, A, L;
    float t, aux;
    FILE *pont_arq;
    pont_arq = fopen("Simulao2.txt", "wt");
    if (pont_arq == NULL)
    {
        printf("Problemas na CRIACAO do arquivo\n");
        return 404;
    }
    float Kappa_g = 8;
    float Kconst_g = 10000;
    float Beta_g = 0.1;
    float Psi_g = 0.1;
    float Theta_r = 0.03;
    float Theta_v = 0.04;
    float Theta_a = 0.0299999;
    float Theta_l = 0.001;
    float Theta_l_2 = 0.03;
    float gamma_r_1 = 0.8;
    float gamma_r_2 = 0.002;
    float mu_r = 0.1;
    float mu_v = 0.03;
    float mu_a = 0.8;
    float mu_l = 0.99;
    float phi_v = 0.99;
    float pi = 3.14159265359;
    float deltat = 0.001;

    l1 = 50.000000;
    l2 = 50.001000;

    G = 1000;
    R = 20;
    V = 50;
    A = 20;
    L = 15;

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

        /////// Leões dobram
        /*
        if(l1<=t&&l2>=t){
            L=L*2;
        }

*/

        /////// Temporada de Caça
        /*
        if(t == 40 || t== 80 || t==120){
            aux = t;
            mu_a = 0.99999;
        }

        if(t == aux+5){
            mu_a = 0.8;
        }
*/
        printf("%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\n", G, R, V, A, L, t);
        //Imprime os resultados no arquivo .txt
        fprintf(pont_arq, "%f\t%f\t%f\t%f\t%f\t%f\n", G, R, V, A, L, t);
    }

    fclose(pont_arq);

    return 0;
}
