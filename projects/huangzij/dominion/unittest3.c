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

int getHandPos(int supplyPos, struct gameState *state, int player)
{
    int i, count = 0;
    for(i = 0; i < state->handCount[player]; i++)
    {
        if(state->hand[player][i] == supplyPos)
            return i;
    }
    return -1;
}

int main () {

    int numPlayers, player, opponent1, opponent2, coins, p, seed, i;

    int handpos, bonus = 0;

    int out1, out2, out3;
    int counter1, counter2, counter3;

    struct gameState G, G_2, G_3;
    struct gameState pre_1, pre_2;

    int k[10] = {ambassador, council_room, tribute, gardens, mine,
                 remodel, smithy, village, baron, minion
                };

    seed = 10;
    numPlayers = 3;
    player = 0;
    opponent1 = 1;
    opponent2 = 2;

    printf ("** Testing playAmbassador() in cardEffect() **\n");
    printf("-------------------------------------------------------------\n\n");


    //
    // check that ambassador returns up to 2 copies of a card from your hand to the supply. Then each other player gains a copy of it.
    //

    initializeGame(numPlayers, k, seed, &G);

    G.whoseTurn = player;

    G.handCount[player] = 5;
    G.hand[player][0] = ambassador;
    for(i = 1; i < 5; i++)
    {
        G.hand[player][i] = curse;
    }


    memcpy(&pre_1, &G, sizeof(struct gameState));
    memcpy(&G_2, &G, sizeof(struct gameState));
    memcpy(&G_3, &G, sizeof(struct gameState));

    // have player play the ambassador and choose to reveal their curse card (2 of them)
    out1 = cardEffect(ambassador, 2, 2, 0, &G, handpos, &bonus);

    //printf("%d\n", out1);

    // boundary case choice2 > 2 || choice2 < 0
    out2 = cardEffect(ambassador, 1, 5, 0, &G_2, handpos, &bonus);

    // boundary case choice1 == handPos
    out3 = cardEffect(ambassador, 0, 2, 0, &G_3, 0, &bonus);

    // assertions for G (two curses in hand gone) each other player has one curse
    myAssert(G.supplyCount[curse] - 2 == pre_1.supplyCount[curse], "proper number of cards in supply");
    myAssert(G.hand[opponent1][0] == curse, "opponent drew correct card from supply, and it's a curse card");

    // assert that boundary cases were caught
    myAssert(out1 == -1, "Test passed");
    myAssert(out2 == -1, "player not allowed to discard more than 2 cards");
    myAssert(out3 == -1, "player not allowed to discard more than 2 cards");

    printf("\n-------------------------------------------------------------\n");
    printf ("Test completed!\n");

    return 0;
}
