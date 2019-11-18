#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include<time.h>

int myAssert(int a, char* msg)
{
    if(!a)
    {
        printf("FAILED: | %s\n", msg);
        return 1;
    }
    printf("%s\n",msg);
    return 0;
}

int countCardsInHand(int supplyPos, struct gameState *state, int player)
{
    int i, count = 0;
    for(i = 0; i < state->handCount[player]; i++)
    {
        if(state->hand[player][i] == supplyPos)
            count++;
    }
    return count;
}

int main()
{
	srand(time(NULL));

    int i, j, total_iters;
    int q = 0;
    int numPlayers, player, coins, p, seed;
    struct gameState pre, G;
    int out1;

    int handpos, bonus = 0;
    int choice1, choice2, choice3 = 0;
    int hasEstate = 0;
    int supplyEstateCount = 0;

    int cardToAdd = 0;

	   int k[10] = {ambassador, council_room, tribute, gardens, mine,
                 remodel, smithy, village, baron, minion};

    seed = 10;
    player=0;

    total_iters = 100;
	  for(i = 0; i < total_iters; i++)
	    {
        // number of players is random
		    // numPlayers = (rand() % 3) + 2; // range of players is 2 to 5
        choice1 = (rand() % 2); // takes on 0 or 1 (discard or gain estate)
        hasEstate = (rand() % 2); // takes on 0 or 1 (has estate in hand or not)
        supplyEstateCount = (rand() % 4); // takes on 0 or 1 (estate in supply or not)

		// initialize a game state and player cards
		initializeGame(2, k, seed, &G);

        player = whoseTurn(&G);

        G.handCount[player] = 0;

        // some cards before estate
        for(j = 0; j < (rand() % 2); j++)
        {
            G.hand[player][q] = rand() % 26;
            q++;
        }
        if(hasEstate)
        {
            G.hand[player][j] = estate;
        }

        G.handCount[player] = j;

        G.supplyCount[estate] = supplyEstateCount;

        printf ("** Testing playBaron() in cardEffect() ** \tIteration: %d/%d\n", i+1, total_iters);
        printf("%d\t%d\t%d\t%d\n", numPlayers, choice1, hasEstate, supplyEstateCount);

		memcpy(&pre, &G, sizeof(struct gameState));

		out1 = cardEffect(baron, choice1, choice2, choice3, &G, handpos, &bonus);

        // Checks are based on whether or not player wanted to discard estate or gain estate
        if (hasEstate == 1 && choice1 == 1) // has an estate and chooses to discard
        {
            myAssert(G.discard[player][G.discardCount[player]] == estate, "player discarded an estate");
            myAssert(countCardsInHand(estate, &G, player) == 0, "estate out of player's hand");
            myAssert(G.coins - 4 == pre.coins, "number of coins increased by 4");
            myAssert(G.numBuys - 1 == pre.numBuys, "number of buys increased by 1");
        }
        else if (hasEstate == 0 && choice1 == 1) // has no estate but chooses to discard
        {
            myAssert(countCardsInHand(estate, &G, player) == 2, "estate added to player's hand when they try to discard w/o estate");
            myAssert(G.supplyCount[estate] - 1 == pre.supplyCount[estate], "supply of estates decreased");
            myAssert(G.numBuys - 1 == pre.numBuys, "number of buys increased by 1");
        }
        else // has estate or not, but chooses to gain
        {
            myAssert(countCardsInHand(estate, &G, player) == 2, "estate added to player's hand");
            myAssert(G.supplyCount[estate] - 1 == pre.supplyCount[estate], "supply of estates decreased");
            myAssert(G.numBuys - 1 == pre.numBuys, "number of buys increased by 1");
        }
	}
	printf ("Test completed!\n");
  return 0;
}
