#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "rlutil.h"

// --------------------------------------------------------------------------------------- CONSTANTS

#define ALUMNE "ALVARO RODRIGUEZ SANTA CRUZ"
#define ASSIGNATURA "DAM1-M3-UF1: Programacio estructurada"

#define MAX_CAPACITAT 1500 // m3
#define MAX_CABAL 400 // m3/h

#define LIMIT_COMPT1 15 // <15% => TANCADES
#define LIMIT_COMPT2 80 // [15%, 80%] i cabal creix => Només primera comporta OBERTA

#define CABAL_COMPORTA 10 // % Evacuació per comporta OBERTA

#define NUM_HORES 24 // hores

// Nivells RISC
#define BAIX LIGHTGREEN // color

#define LIMIT_ALT 60 // %
#define ALT LIGHTCYAN // color

#define LIMIT_ALARMA 90 // %
#define ALARMA LIGHTRED // color
#define NUM_BEEPS 5 // beeps
#define TIME_BEEPS 1000 // ms

// Nivells EVACUACIÓ
#define EVACUACIO 95 // %
#define EVACUACIO_ZONA 100 // %

// --------------------------------------------------------------------------------------- FI CONSTANTS

int main()
{
    float embassament, embassamentAnt, riu1, riu2, percent, riu_total, max_embassament, min_embassament, total_embassament, total_percent;
    int signe_riu, variacio_riu, compt1, compt2, hores=0, i=0;

    srand(time(NULL)); // Inicialitza el generador de nombres aleatoris

    saveDefaultColor(); // Guarda el color de la consola per defecte

    printf("BENVINGUTS AL SISTEMA DE CONTROL DE L'EMBASSAMENT DE VALLFORNERS\n");
    printf("Alumne: " ALUMNE " - Assignatura: " ASSIGNATURA "\n");

// --------------------------------------------------------------------------------------- ENTRADA DADES

    // EMBASSAMENT
    printf("Introdueix la capacitat inicial de l'embassament en m3 (0-%d):\n", MAX_CAPACITAT);
    scanf("%f", &embassament);

    while(embassament<0 || embassament>MAX_CAPACITAT){
        printf("La capacitat no pot excedir la capacitat maxima de l'embassament %dm3.\n", MAX_CAPACITAT);
        printf("Introdueix la capacitat inicial de l'embassament en m3 (0-%d):\n", MAX_CAPACITAT);
        scanf("%f", &embassament);
    }

    // RIU 1
    printf("Introdueix el cabal inicial del riu 1 en m3/h (0-%d):\n", MAX_CABAL);
    scanf("%f", &riu1);

    while(riu1<0 || riu1>MAX_CABAL){
        printf("El cabal inicial del riu 1 no pot excedir de %dm3/h.\n", MAX_CABAL);
        printf("Introdueix el cabal inicial del riu 1 en m3/h (0-%d):\n", MAX_CABAL);
        scanf("%f", &riu1);
    }

    // RIU 2
    printf("Introdueix el cabal inicial del riu 2 en m3/h (0-%d):\n", MAX_CABAL);
    scanf("%f", &riu2);

    while(riu2<0 || riu2>MAX_CABAL){
        printf("El cabal inicial del riu 2 no pot excedir de %dm3/h.\n", MAX_CABAL);
        printf("Introdueix el cabal inicial del riu 2 en m3/h (0-%d):\n", MAX_CABAL);
        scanf("%f", &riu2);
    }

// --------------------------------------------------------------------------------------- FI ENTRADA DADES

// --------------------------------------------------------------------------------------- CÀLCUL DADES

    // EMBASSAMENT
    max_embassament = embassament;
    min_embassament = embassament;
    total_embassament = embassament;
    percent = (embassament/MAX_CAPACITAT)*100;
    total_percent = percent;

    // RIUS
    riu_total = riu1+riu2;

    if(percent<LIMIT_COMPT1){
        compt1=0;
        compt2=0;
    }else{
        if(percent<=LIMIT_COMPT2){
            compt1=1;
            compt2=0;
        }else{
            compt1=1;
            compt2=1;
        }
    }

    // CAPÇALERA
    printf("\nHora\tEstat\t\tPercent\t\tRiu1\tRiu2\tTotal\tCompt1\tCompt2\n");

    // COLOR LÍNIA
    if(percent<=LIMIT_ALT){
        setColor(BAIX);
    }else{
        if(percent<=LIMIT_ALARMA){
            setColor(ALT);
        }else{
            setColor(ALARMA);
        }
    }

    // DADES LÍNIA
    printf("%d\t%.2f\t\t%.2f\t\t%.2f\t%.2f\t%.2f\t%d\t%d\n", hores++, embassament, percent, riu1, riu2, riu_total, compt1, compt2);

    while(hores<NUM_HORES && percent<EVACUACIO){

        // CÀLCUL RIU 1
        if(riu1!=0){
            signe_riu = rand()%2;
            variacio_riu = rand()%11;
            /*
                (-1)^rand()%2
                (-1)^0 = 1  => positiu
                (-1)^1 = -1 => negatiu
            */
            riu1 += pow(-1, signe_riu)*variacio_riu*riu1/100;
        }else{
            riu1 = rand()%3;
        }

        // CÀLCUL RIU 2
        if(riu2!=0){
            signe_riu = rand()%2;
            variacio_riu = rand()%11;
            riu2 += pow(-1, signe_riu)*variacio_riu*riu2/100;
        }else{
            riu2 = rand()%3;
        }

        // EMBASSAMENT
        embassamentAnt = embassament;
        embassament += riu_total-(compt1+compt2)*CABAL_COMPORTA*embassament/100;

        if(max_embassament<embassament){
            max_embassament = embassament;
        }else{
            if(min_embassament>embassament){
                min_embassament = embassament;
            }
        }

        total_embassament += embassament;
        percent = (embassament/MAX_CAPACITAT)*100;
        total_percent += percent;

        // RIUS
        riu_total = riu1+riu2;

        // Percentatge inferior a 15 o (percentatge inferior/igual a 80 i no creix)
        if(percent<LIMIT_COMPT1 || (percent<=LIMIT_COMPT2 && !(embassamentAnt < embassament))){
            compt1=0;
            compt2=0;
        }else{
            // Percentatge inferior/igual a 80 i creix
            if(percent<=LIMIT_COMPT2 && embassamentAnt < embassament){
                compt1=1;
                compt2=0;
            }else{
                compt1=1;
                compt2=1;
            }
        }

        // COLOR LÍNIA
        if(percent<=LIMIT_ALT){
            setColor(BAIX);
        }else{
            if(percent<=LIMIT_ALARMA){
                setColor(ALT);
            }else{
                setColor(ALARMA);
            }
        }

        // DADES LÍNIA
        printf("%d\t%.2f\t\t%.2f\t\t%.2f\t%.2f\t%.2f\t%d\t%d\n", hores++, embassament, percent, riu1, riu2, riu_total, compt1, compt2);
    }

// --------------------------------------------------------------------------------------- FI CÀLCUL DADES

// --------------------------------------------------------------------------------------- RESULTATS FINALS

    printf("\n\n");

    // EVACUACIÓ
    if(percent>EVACUACIO_ZONA){
        printf("EVACUACIO ZONA\n");
    }else{
        if(percent>EVACUACIO){
            printf("EVACUACIO\n");
        }
    }

    // RESULTATS
    resetColor();
    printf("RESUM\n");
    printf("El cabal maxim de l'embassament en m3 ha sigut: %.2f\n", max_embassament);
    printf("El cabal minim de l'embassament en m3 ha sigut: %.2f\n", min_embassament);
    printf("El cabal mitja de l'embassament en m3 ha sigut: %.2f\n", total_embassament/hores);
    printf("El percentatge mitja de l'embassament ha sigut: %.2f\n", total_percent/hores);

    // BEEPS: No els he col.locat en el bloc if anterior per mostrar els resultats el més aviat possible, com en una situación real d'emergència.
    if(percent>EVACUACIO_ZONA){
        for(i=0; i<NUM_BEEPS; i++){
//            printf("\a");
            Sleep(TIME_BEEPS);
        }
    }else{
        if(percent>EVACUACIO){
//            printf("\a");
        }
    }

// --------------------------------------------------------------------------------------- FI RESULTATS FINALS

    return 0;
}
