#include <stdio.h>
#include <stdlib.h>

#include "league.h"
#include "cpu.h"

void initComputerLeague(Player* players, HashMap* effective, HashMap* uneffective, HashMap* pkmMap, HashMap* moves)
{
    int combats = 0;
    int win = 0;

    do
    {
        switch (combats)
        {
            case 0:
            {
                Player leader1;
                fileToPlayer(&leader1, "5968c0.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader1, effective, uneffective, &win);
                break;
            }    
            case 1:
            {
                Player leader2;
                fileToPlayer(&leader2, "67c9c2.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader2, effective, uneffective, &win);
                printf("HOla");
                break;
            }    
            case 2:
            {
                Player leader3;
                fileToPlayer(&leader3, "2fd674.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader3, effective, uneffective, &win);
                break;
            }    
            case 3:
            {
                Player leader4;
                fileToPlayer(&leader4, "f6a447.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader4, effective, uneffective, &win);
                break;
            }    
            case 4:
            {
                Player leader5;
                fileToPlayer(&leader5, "33cedb.pkdb", pkmMap, moves);
                initCpuCombat(players, &leader5, effective, uneffective, &win);
                break;
            }    
        }
    } while (win != -1);

    if (win == 5)
    {
        printf("Felicidades, eres el lider\n");
    }

    if(win == -1) 
    {
        printf("Has perdido!\n");
    }
        
}