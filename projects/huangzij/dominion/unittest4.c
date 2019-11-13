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
    numPlayers = 2;
    numOpponents = 1;
    player = 0;
    opponent1 = 1;

    printf ("** Testing playTribute() in cardEffect() **\n");
    printf("-------------------------------------------------------------\n\n");


    //
    // check that next player discards the top two cards from his deck and that each differently named card properly corresponds to the benefit
    //

    // 1: next player has action card, treasure card. so player should get +2 actions, +2 coins
    // 2: next player has treasure card, victory card. so player should get +2 coins, +2 cards
    // 3: next player has victory card, victory card (different name). so player should get +4 cards
    // 4: next player has treasure card, same treasure card. so player should get +2 coins only.

    initializeGame(numPlayers, k, seed, &G);

    // give player a tribute card, 0 other cards
    G.handCount[player] = 1;
    G.hand[player][0] = tribute;

    G.whoseTurn = player;

    // create copies of game state for test. also create control game state pre_1
    memcpy(&pre_1, &G, sizeof(struct gameState));
    memcpy(&G_2, &G, sizeof(struct gameState));
    memcpy(&G_3, &G, sizeof(struct gameState));
    memcpy(&G_4, &G, sizeof(struct gameState));

    // 1: give opponent action card treasure card
    G.handCount[opponent1] = 2;
    G.hand[opponent1][0] = remodel;
    G.hand[opponent1][1] = gold;

    // 2: give opponent treasure card victory card
    G_2.handCount[opponent1] = 2;
    G_2.hand[opponent1][0] = gold;
    G_2.hand[opponent1][1] = estate;

    // 3: give opponent victory card victory card
    G_3.handCount[opponent1] = 2;
    G_3.hand[opponent1][0] = estate;
    G_3.hand[opponent1][1] = duchy;

    // 1: give opponent treasure card same treasure card
    G_4.handCount[opponent1] = 2;
    G_4.hand[opponent1][0] = copper;
    G_4.hand[opponent1][1] = copper;

    // have player play the ambassador and choose to reveal their curse card (2 of them)
    int out1 = cardEffect(tribute, 0, 0, 0, &G, 0, NULL);
    int out2 = cardEffect(tribute, 0, 0, 0, &G_2, 0, NULL);
    int out3 = cardEffect(tribute, 0, 0, 0, &G_3, 0, NULL);
    int out4 = cardEffect(tribute, 0, 0, 0, &G_4, 0, NULL);

    // assertions for G [next player has action card, treasure card. so player should get +2 actions, +2 coins]
    myAssert(G.numActions - 2 == pre_1.numActions - 1, "number of actions increased by 2");                 // have to subtract 1 from pre because tribute itself is an action
    myAssert(G.coins - 2 == pre_1.coins, "number of coins increased by 2");
    myAssert(G.handCount[opponent1] - 2 == pre_1.handCount[opponent1], "opponent deck reduced by 2 cards");

    // assertions for G_2 [next player has treasure card, victory card. so player should get +2 coins, +2 cards]
    myAssert(G_2.handCount[player] - 2 == pre_1.handCount[player] - 1, "number of actions increased by 2"); // have to subtract 1 from pre because tribute itself is a card that's played
    myAssert(G_2.coins - 2 == pre_1.coins, "number of coins increased by 2");
    myAssert(G_2.handCount[opponent1] - 2 == pre_1.handCount[opponent1], "opponent deck reduced by 2 cards");

    // assertions for G_3 [next player has victory card, victory card (different name). so player should get +4 cards]
    myAssert(G_3.handCount[player] - 4 == pre_1.handCount[player] - 1, "number of cards increased by 4");   // have to subtract 1 from pre because tribute itself is a card that's played
    myAssert(G_3.handCount[opponent1] - 2 == pre_1.handCount[opponent1], "opponent deck reduced by 2 cards");

    // assertions for G_4 [next player has treasure card, same treasure card. so player should get +2 coins only.]
    myAssert(G_4.coins - 2 == pre_1.coins, "number of coins increased by 2 only");
    myAssert(G_4.handCount[opponent1] - 2 == pre_1.handCount[opponent1], "opponent deck reduced by 2 cards");

    printf("\n-------------------------------------------------------------\n");
    printf ("Test completed!\n");

    return 0;
}
