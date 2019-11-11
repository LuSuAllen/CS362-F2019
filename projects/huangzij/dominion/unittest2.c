#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int myAssert(int a, char* msg)
{
    if(a)
    {
        printf("assertion error | %s\n", msg);
        return 1;
    }
    printf("%s\n",msg);
    return 0;
}

int main () {

    int numPlayers, player, numOpponents, opponent1, opponent2, coins, p, deckCount, discardCount, handCount, seed, i;

    struct gameState G, G_2, G_3;
    struct gameState pre_1, pre_2, pre_3;

    int k[10] = {ambassador, council_room, tribute, gardens, mine,
                 remodel, smithy, village, baron, minion
                };

    seed = 10;
    numPlayers = 3;
    numOpponents = 2;
    player = 0;
    opponent1 = 1;
    opponent2 = 2;

    printf ("** Testing playMinion() in cardEffect() **\n");
    printf("-------------------------------------------------------------\n\n");

    //
    // check that minion either adds +2 coins or discards players hand and draws 4 cards, also changing hands of other players.
    //

    initializeGame(numPlayers, k, seed, &G);

    // give player a minion, 4 other cards are coppers
    G.handCount[player] = 5;
    G.hand[player][0] = minion;
    for(i = 1; i < 5; i++)
    {
        G.hand[player][i] = copper;
    }

    G.whoseTurn = player;

    // give opponents 5 cards and 4 cards respectively. Just placing coppers in this test (card type shouldn't matter)
    G.handCount[opponent1] = 5;
    for(i = 0; i < 5; i++)
    {
        G.hand[opponent1][i] = copper;
    }
    G.handCount[opponent2] = 4;
    for(i = 0; i < 4; i++)
    {
        G.hand[opponent2][i] = copper;
    }

    // copy game state to previous and a new state G_2
    memcpy(&pre_1, &G, sizeof(struct gameState));
    memcpy(&G_2, &G, sizeof(struct gameState));

    // separate setup for having opponent call minion and observing if player has to redraw
    initializeGame(numPlayers, k, seed, &G_3);
    G_3.handCount[opponent2] = 5;
    G_3.hand[opponent2][0] = minion;
    for(i = 1; i < 5; i++)
    {
        G_3.hand[opponent2][i] = copper;
    }

    G_3.whoseTurn = opponent2;

    // give opponents 5 cards and 4 cards respectively. Just placing coppers in this test (card type shouldn't matter)
    G_3.handCount[opponent1] = 4;
    for(i = 0; i < 4; i++)
    {
        G_3.hand[opponent1][i] = copper;
    }
    // player would need to discard because hand is 6 >= 5
    G_3.handCount[player] = 6;
    for(i = 0; i < 6; i++)
    {
        G_3.hand[player][i] = copper;
    }

    // take action for state G
    int out = cardEffect(minion, 1, 1, 0, &G, 0, NULL);

    // take action for state G_2
    int out2 = cardEffect(minion, 0, 1, 0, &G_2, 0, NULL);

    // take action for state G_3
    int out3 = cardEffect(minion, 0, 1, 0, &G_3, 0, NULL);

    // assertions for G (+2 coins happened, discard didn't happen)
    myAssert(G.coins - 2 == pre_1.coins, "number of coins increased by 2");
    myAssert(G.handCount[player] == pre_1.handCount[player], "player still has same number of cards");

    // assertions for G_2  (hand was discarded, now 4 new cards, opponent1 but not opponent2 had to discard and draw 4)
    myAssert(G_2.handCount[player] - 1 == pre_1.handCount[player], "player discarded hand and drew 4 cards");
    myAssert(G_2.handCount[opponent1] - 1 == pre_1.handCount[opponent1], "opponent1 discarded hand and drew 4 cards");
    myAssert(G_2.handCount[opponent2] == pre_1.handCount[opponent2], "opponent2 did not discard hand");

    // assertions for G_3 (player's hand was discarded, opponent 2's hand was discarded)
    myAssert(G_3.handCount[player] - 1 == pre_1.handCount[player], "player discarded hand and drew 4 cards");
    myAssert(G_3.handCount[opponent2] - 1 == pre_1.handCount[opponent2], "opponent2 discarded hand and drew 4 cards");
    myAssert(G_2.handCount[opponent1] == pre_1.handCount[opponent1], "opponent1 did not discard hand");

    printf("\n-------------------------------------------------------------\n");

    printf ("Test completed!\n");

    return 0;
}
