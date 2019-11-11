#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int myAssert(int a, char* msg)
{
    if(!a)
    {
        printf("assertion error | %s\n", msg);
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

int main () {

    int numPlayers, player, coins, p, seed;

    int handPos, bonus;

    int counter1, counter2, counter3;

    struct gameState G, G_2, G_3;
    struct gameState pre_1, pre_2;

    int k[10] = {ambassador, council_room, tribute, gardens, mine,
                 remodel, smithy, village, baron, minion
                };

    seed = 10;
    numPlayers = 2;
    player = 0;

    printf ("** Testing playBaron() in cardEffect() **\n");
    printf("-------------------------------------------------------------\n\n");

    //
    // check that baron will let you discard estate card for +4 if both baron and estate card are in hand.
    //

    initializeGame(numPlayers, k, seed, &G);

    player = whoseTurn(&G);

    // put a baron and estate in player's hand, save three gamestates (one with estate, and two without estate), both hands have some other filler cards
    // add filler cards
    gainCard(ambassador, &G, 2, player);
    gainCard(tribute, &G, 2, player);
    gainCard(gold, &G, 2, player);
    // add baron
    gainCard(baron, &G, 2, player);

    memcpy(&pre_1, &G, sizeof(struct gameState));
    memcpy(&G_2, &G, sizeof(struct gameState));
    memcpy(&G_3, &G, sizeof(struct gameState));

    // other gamestate player has estate with baron
    gainCard(estate, &G_2, 2, player);
    memcpy(&pre_2, &G_2, sizeof(struct gameState));

    // play the baron card on game G without estate in hand and try to gain estate
    int out1 = cardEffect(baron, 0, 0, 0, &G, handPos, &bonus);
    counter1 = countCardsInHand(estate, &G, player) - countCardsInHand(estate, &pre_1, player);

    // play baron card with an estate in hand
    int out2 = cardEffect(baron, 1, 0, 0, &G_2, handPos, &bonus);
    counter2 = countCardsInHand(estate, &G_2, player) - countCardsInHand(estate, &pre_2, player);

    // play the baron card on game G_3 without estate in hand and try to discard state
    int out3 = cardEffect(baron, 1, 0, 0, &G_3, handPos, &bonus);
    counter3 = countCardsInHand(estate, &G_3, player) - countCardsInHand(estate, &pre_1, player);

    // verify that an estate is now in the hand, number of buys increased by 1.
    myAssert(G.supplyCount[estate] - 1 == pre_1.supplyCount[estate], "supply of estates decreased");
    myAssert(counter1 == 1, "player gained estate");
    myAssert(G.discard[player][G.discardCount[player]] == estate, "player gained estate");
    myAssert(G.numBuys - 1 == pre_1.numBuys, "number of buys increased by 1");

    // verify that coins increased by 4, number of buys increased by 1. discard deck has an estate in it now
    myAssert(G_2.discard[player][G_2.discardCount[player]] == estate, "player discarded an estate");
    myAssert(counter2 == 0, "player did not gain estate");
    myAssert(G_2.coins - 4 == pre_2.coins, "number of coins increased by 4");
    myAssert(G_2.numBuys - 1 == pre_2.numBuys, "number of buys increased by 1");

    // verify that the player just gains an estate on a failed attempt to discard one
    myAssert(G_3.supplyCount[estate] - 1 == pre_1.supplyCount[estate], "supply of estates decreased");
    myAssert(counter3 == 1, "player did not discard estate, instead gained one");
    myAssert(G_3.discard[player][G_3.discardCount[player]] == estate, "estate gained in players deck when they try to baron without having estate");
    myAssert(G_3.numBuys - 1 == pre_1.numBuys, "number of buys increased by 1");

    printf("\n-------------------------------------------------------------\n");
    printf ("Test completed!\n");

    return 0;
}
