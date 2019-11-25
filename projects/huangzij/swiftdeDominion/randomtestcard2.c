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
    int w, q = 0;
    int numPlayers, player, coins, p, seed;
    struct gameState pre, G;
    int out1;

    int handpos, bonus = 0;
    int choice1, choice2, choice3 = 0;
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
		numPlayers = (rand() % 3) + 2; // range of players is 2 to 5
        choice1 = (rand() % 2); // takes on 0 or 1 (+2 or discard hand)

		// initialize a game state and player cards
		initializeGame(numPlayers, k, seed, &G);

        // for each player, chance of discarding some cards (and going below 5 cards)
        for(j = 0; j < numPlayers; j++)
        {
            w = rand() % 2;
            for(q = 0; q < w; q++)
            {
                discardCard(handpos, j, &G,  0);
            }
        }

        player = whoseTurn(&G);

        printf ("** Testing playMinion() in cardEffect() ** \tIteration: %d/%d\n", i+1, total_iters);

		memcpy(&pre, &G, sizeof(struct gameState));

		out1 = cardEffect(minion, choice1, choice2, choice3, &G, handpos, &bonus);

        // Checks are based on whether or not player wanted to discard hand
        if(choice1 == 0) // player wants to get +2
        {
            myAssert(G.coins - 2 == pre.coins, "number of coins increased by 2");
            myAssert(G.handCount[player] == pre.handCount[player], "player still has same number of cards");
        }
        else
        {
            for(j = 0; j < numPlayers; j++)
            {
                if(pre.handCount[j] >= 5)
                {
                    myAssert(G.handCount[j] == 4, "player discarded and got 4 cards");
                }
                else
                {
                    myAssert(G.handCount[j] == pre.handCount[j], "player did not discard any cards");
                }
            }
        }

	}
	printf ("Test completed!\n");
  return 0;
}
