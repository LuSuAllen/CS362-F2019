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
    int tributeRevealedCards[2] = {-1, -1};

	  int k[10] = {ambassador, council_room, tribute, gardens, mine,
                 remodel, smithy, village, baron, minion};

    seed = 10;
    player=0;

    total_iters = 1000;
	  for(i = 0; i < total_iters; i++)
    {
        // number of players is random
		    numPlayers = (rand() % 3) + 2; // range of players is 2 to 5

		    // initialize a game state and player cards
        initializeGame(numPlayers, k, seed, &G);

        // give each player a random number of random cards
        for(j = 0; j < numPlayers; j++)
        {
            w = rand() % 5;
            for(q = 0; q < w; q++)
            {
                G.hand[j][q] = rand() % 26;
            }
            G.handCount[j] = w;
        }

        // give each player random deckcount
        for(j = 0; j < numPlayers; j++)
        {
            w = rand() % 5;
            G.deckCount[j] = w;
        }

        player = whoseTurn(&G);

        printf ("** Testing playTribute() in cardEffect() ** \tIteration: %d/%d\n", i+1, total_iters);

		memcpy(&pre, &G, sizeof(struct gameState));

		out1 = cardEffect(tribute, choice1, choice2, choice3, &G, handpos, &bonus);

        // Checks are based on the top two cards of player to left [player+1]
        tributeRevealedCards[0] = pre.hand[player+1][4];
        tributeRevealedCards[1] = pre.hand[player+1][3];

        if(tributeRevealedCards[0] <= 3) // it is a victory card
        {
            if(tributeRevealedCards[0] == tributeRevealedCards[1]) // identical victory card
            {
                myAssert(G.handCount[player] - 2 == pre.handCount[player] - 1, "number of cards increased by 2");
            }
            else if(tributeRevealedCards[1] <=3) // victory card
            {
                myAssert(G.handCount[player] - 4 == pre.handCount[player] - 1, "number of cards increased by 4");
            }
            else if(tributeRevealedCards[1] <=6) // action victory card
            {
                myAssert(G.handCount[player] - 2 == pre.handCount[player] - 1, "number of cards increased by 2");
                myAssert(G.numActions - 2 == pre.numActions - 1, "number of actions increased by 2");
            }
            else // treaure card
            {
                myAssert(G.handCount[player] - 2 == pre.handCount[player] - 1, "number of cards increased by 2");
                myAssert(G.coins - 2 == pre.coins, "number of coins increased by 2");
            }
        }
        else if(tributeRevealedCards[0] <= 6) // it's a treasure card
        {
            if(tributeRevealedCards[0] == tributeRevealedCards[1])
            {
                myAssert(G.coins - 2 == pre.coins, "number of coins increased by 2");
            }
            else if(tributeRevealedCards[1] <=3)
            {
                myAssert(G.handCount[player] - 2 == pre.handCount[player] - 1, "number of cards increased by 2");
                myAssert(G.coins - 2 == pre.coins, "number of coins increased by 2");
            }
            else if(tributeRevealedCards[1] <=6)
            {
                myAssert(G.coins - 4 == pre.coins, "number of coins increased by 4");
            }
            else
            {
                myAssert(G.handCount[player] - 2 == pre.handCount[player] - 1, "number of actions increased by 2");
                myAssert(G.coins - 2 == pre.coins, "number of coins increased by 2");
            }
        }
        else // it's an action card
        {
            if(tributeRevealedCards[0] == tributeRevealedCards[1])
            {
                myAssert(G.handCount[player] - 2 == pre.handCount[player] - 1, "number of actions increased by 2");
            }
            else if(tributeRevealedCards[1] <=3)
            {
                myAssert(G.handCount[player] - 2 == pre.handCount[player] - 1, "number of actions increased by 2");
                myAssert(G.handCount[player] - 2 == pre.handCount[player] - 1, "number of cards increased by 2");
            }
            else if(tributeRevealedCards[1] <=6)
            {
                myAssert(G.handCount[player] - 2 == pre.handCount[player] - 1, "number of actions increased by 2");
                myAssert(G.coins - 2 == pre.coins, "number of coins increased by 2");
            }
            else
            {
                myAssert(G.handCount[player] - 4 == pre.handCount[player] - 1, "number of actions increased by 4");
            }
        }

	}
	printf ("Test completed!\n");
  return 0;
}
