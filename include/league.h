#ifndef league_h
#define league_h
#include "hashmap.h"
#include "player.h"

void printsFirstLeader();

void printFirstWinText();

void printsSecondLeader();

void printSecondWinText();

void printsThirdLeader(Player* players);

void printThirdWinText();

void printsFourthLeader(Player* players);

void printFourtWinText(Player* players);

void printsFinalLeader(Player* players);

void printFinalWinText(Player* players);

void initComputerLeague(Player* players, HashMap* effective, HashMap* uneffective, HashMap* pkmMap, HashMap* moves);

#endif