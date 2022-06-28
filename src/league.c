#include <stdio.h>
#include <stdlib.h>

#include "league.h"
#include "cpu.h"

void printsFirstLeader()
{
    printf("Yo soy LapsoJr, del ALTO MANDO!\n");
    getchar();
    printf("Tus pokemons estarán a mi merced cuando luchemos!\n");
    getchar();
    printf("Comenzamos?\n");
    getchar();
}

void printFirstWinText()
{
    printf("Eres mejor de lo que yo pensaba!\n");
    getchar();
    printf("Adelante!\n");
    getchar();
    printf("Pero la LIGA te reserva otras sorpresas!\n");
    getchar();
}

void printsSecondLeader()
{
    printf("Yo soy JdobleM, del ALTO MANDO!\n");
    getchar();
    printf("Con un riguroso entrenamiento, la gente y los pokemons pueden fortalecerse!\n");
    getchar();
    printf("Siempre he vivido con mis pokemons y los he entrenado yo mismo!\n");
    getchar();
    printf("Es por eso que no puedo perder contra alguien como tu!\n");
    getchar();
    printf("Comencemos con esto!\n");
    getchar();
}

void printSecondWinText()
{
    printf("Yo ya he hecho mi trabajo!\n");
    getchar();
    printf("Te espera tu proximo desafio!\n");
    getchar();
}

void printsThirdLeader(Player* players)
{
    printf("Yo soy krr, del ALTO MANDO!\n");
    getchar();
    printf("He oido que Ignacio te tiene mucha admiración %s\n", players->name);
    getchar();
    printf("El era muy fuerte, pero ya ha pasado mucho tiempo desde entonces, es la sombra de lo que fue\n");
    getchar();
    printf("Ahora solo juguetea con su PokeDex, pero esta equivocado!\n");
    getchar();
    printf("%s!, yo te enseñare como lucha un gran entrenador!\n", players->name);
    getchar();
}

void printThirdWinText()
{
    printf("Tu ganas!\n");
    getchar();
    printf("Ya se por que Ignacio te aprecia tanto...\n");
    getchar();
    printf("No tengo nada mas que decir\n");
    getchar();
    printf("Vete ya!\n");
    getchar();
}

void printsFourthLeader(Player* players)
{
    printf("%s!\n", players->name);
    getchar();
    printf("Ah!, he oido hablar de ti %s!\n", players->name);
    getchar();
    printf("Dirijo al ALTO MANDO\n");
    getchar();
    printf("Puedes llamarme 4m0xicilina!\n");
    getchar();
    printf("Bueno, sabes que vas a perder, no?\n");
    getchar();
    printf("Tu desafio en la LIGA termina conmigo, %s!\n", players->name);
    getchar();
}

void printFourtWinText(Player* players)
{
    printf("%s, sigo sin poder creeme que me derrotaste\n", players->name);
    getchar();
    printf("Has ganado la LIGA!\n");
    getchar();
    printf("Bueno, casi, porque te espera otro desafio.\n");
    getchar();
    printf("Deberás luchar contra alguien muy importante para ti...\n");
    getchar();
    printf("El Profesor Ignacio, tu maestro.\n");
    getchar();
    printf("El es el autentico campeon de la LIGA\n");
    getchar();
}

void printsFinalLeader(Player* players)
{
    printf("Hola!\n");
    getchar();
    printf("Estaba deseando verte, %s\n", players->name);
    getchar();
    printf("Mi pupilo deberia ser fuerte para poder mantenerme en forma\n");
    getchar();
    printf("Gracias a la PokeDex, he buscado los pokemons mas fuertes!\n");
    getchar();
    printf("Y he creado varios equipos que derrotarian a cualquier tipo de Pokemon\n");
    getchar();
    printf("Es por eso que soy el mejor entrenador del mundo!\n");
    getchar();
    printf("Veamos de que estas hecho %s!\n", players->name);
    getchar();
}

void printFinalWinText(Player* players)
{
    printf("Que?\n");
    getchar();
    printf("Como he podido perder?\n");
    getchar();
    printf("He entrenado mis pokemons a la perfeccion...\n");
    getchar();
    printf("Pero a pesar de todo, estoy orgulloso de ti %s\n", players->name);
    getchar();
    printf("Has crecido tanto desde que te otorgue tus primeros 4 pokemons...\n");
    getchar();
    printf("Felicidades eres el campeon de la LIGA\n");
    getchar();
}

void initComputerLeague(Player* players, HashMap* effective, HashMap* uneffective, HashMap* pkmMap, HashMap* moves)
{
    int win = 0;

    while (1)
    {

        if(win == -1) 
        {
            printf("Esperaba mas de ti %s...\n", players->name);
            getchar();
            printf("Vuelve cuando estes mas fuerte\n");
            getchar();
            return;
        }
        
        switch (win)
        {
            case 0://1er combate.
            {
                printsFirstLeader();
                getchar();
                Player leader1;
                fileToPlayer(&leader1, "5968c0.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader1, effective, uneffective, &win);
                if (win == 0) return;
                else printFirstWinText();
                break;
            }    
            case 1://2do combate.
            {
                printsSecondLeader();
                Player leader2;
                fileToPlayer(&leader2, "67c9c2.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader2, effective, uneffective, &win);
                if (win == 2) printSecondWinText();
                break;
            }    
            case 2://3er combate.
            {
                printsThirdLeader(players);
                Player leader3;
                fileToPlayer(&leader3, "2fd674.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader3, effective, uneffective, &win);
                if (win == 3) printThirdWinText();
                break;
            }    
            case 3://4to combate.
            {
                printsFourthLeader(players);
                Player leader4;
                fileToPlayer(&leader4, "f6a447.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader4, effective, uneffective, &win);
                if (win == 4) printFourtWinText(players);
                break;
            }    
            case 4://5to y ultimo.
            {
                printsFinalLeader(players);
                Player leader5;
                fileToPlayer(&leader5, "33cedb.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader5, effective, uneffective, &win);
                if (win == 5)
                {
                    printFinalWinText(players);
                    return;
                }
                break;
            } 
        }
    }    
}