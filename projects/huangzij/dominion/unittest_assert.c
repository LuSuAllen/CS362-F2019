/* 	Interactive Dominion Interface
Sam Heinith CS362
1/26/2010
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "dominion_helpers.h"

void myAssert(int value, char* message) {
    if (!value) {
        printf("Failed: %s\n", message);
        assert(0);
    } else {
        printf("Passed: %s\n", message);
    }
}
