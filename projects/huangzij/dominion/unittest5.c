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

    int numPlayers, player, numOpponents, opponent1, coins, p, deckCount, discardCount, handCount, seed, i;

    struct gameState G, G_2, G_3, G_4;
    struct gameState pre_1;

    int k[10] = {ambassador, council_room, tribute, gardens, mine,
                 remodel, smithy, village, baron, minion
                };

    seed = 10;
    numPlayers = 1;
    numOpponents = 0;
    player = 0;

    printf ("** Testing playMine() in cardEffect() **\n");
    printf("-------------------------------------------------------------\n\n");


    //
    // check that mine replaced treasure with another treasute vosting up to 3 more than it.
    //

    // 1: test copper to silver
    // 2: test silver to gold
    // 3: test copper to gold (should fail)
    // 4: boundary case for gold

    initializeGame(numPlayers, k, seed, &G);

    // 1: give player a mine card, copper card
    G.handCount[player] = 2;
    G.hand[player][0] = mine;

    G.whoseTurn = player;

    // create copies of game state for test. also create control game state pre_1
    memcpy(&pre_1, &G, sizeof(struct gameState));
    memcpy(&G_2, &G, sizeof(struct gameState));
    memcpy(&G_3, &G, sizeof(struct gameState));
    memcpy(&G_4, &G, sizeof(struct gameState));

    // 1: give player copper
    G.hand[player][1] = copper;

    // 2: give player silver
    G_2.hand[player][1] = silver;

    // 3: give player copper
    G_3.hand[player][1] = copper;

    // 4: give player gold
    G_4.hand[player][1] = gold;

    // have player play the ambassador and choose to reveal their curse card (2 of them)
    int out1 = cardEffect(mine, 1, silver, 0, &G, 0, NULL);
    int out2 = cardEffect(mine, 1, gold, 0, &G_2, 0, NULL);
    int out3 = cardEffect(mine, 1, gold, 0, &G_3, 0, NULL);
    int out4 = cardEffect(mine, 1, gold, 0, &G_4, 0, NULL);

    // assertions for G [next player has action card, treasure card. so player should get +2 actions, +2 coins]
    myAssert(G.hand[player][1] == silver, "copper changed to silver");

    // assertions for G_2 [next player has treasure card, victory card. so player should get +2 coins, +2 cards]
    myAssert(G_2.hand[player][1] == silver, "silver changed to gold");

    // assertions for G_3 [next player has victory card, victory card (different name). so player should get +4 cards]
    myAssert(out3 == -1, "copper should not turn to gold");

    // assertions for G_4
    myAssert(G_4.hand[player][1] == gold, "gold changed to gold (yup this is normal and allowed)");

    printf("\n-------------------------------------------------------------\n");
    printf ("Test completed!\n");

    return 0;
}
