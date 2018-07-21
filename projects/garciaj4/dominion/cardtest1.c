/* -----------------------------------------------------------------------
 * Unit tests for the smithy card.
 * Include the following lines in your makefile:
 *
 * cardTest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardTest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "rngs.h"

void assertEqual(char* description, int reference, int value){
	if(value == reference){
		printf("SUCCESS:%s\n", description);
	}
	else{
		printf("FAILED:%s\n", description);
		printf("Expected: %d     Received: %d\n", reference, value);
	}
}

void assertNEqual(char* description, int reference, int value){
	if(value != reference){
		printf("SUCCESS:%s\n", description);
	}
	else{
		printf("FAILED:%s\n", description);
		printf("Expected: %d     Received: %d\n", reference, value);
	}
}

int main() {

	printf("TESTING: smithy:\n");

	//initialize test environment
	int g = -5;
	int status = -5;

	//establish a mock games state
	int numPlayers = 2;
	int k[10] = {adventurer, council_room, feast, salvager, mine, remodel, smithy, village, baron, cutpurse};
	int seed = 1000;
	struct gameState G;

	memset(&G, -5, sizeof(struct gameState));
	g = initializeGame(numPlayers, k, seed, &G);
	assert(g == 0);

	//we're going to modify the hand so we can trace events
	G.hand[G.whoseTurn][0] = smithy;
	G.hand[G.whoseTurn][1] = copper;
	G.hand[G.whoseTurn][2] = silver;
	G.hand[G.whoseTurn][3] = gold;
	G.hand[G.whoseTurn][4] = adventurer;

	//the games just started but lets check our preconditions
	assert(G.handCount[G.whoseTurn] == 5);
	assert(G.deckCount[G.whoseTurn] == 5);
	assert(G.playedCardCount == 0);

	status = playSmithy(&G, 0);
	assert(status == 0);
	assertEqual("Test to see if our actions was reduced by 1", 0, G.numActions);
	assertEqual("Test to see if our deck count was reduced by 3", 2, G.deckCount[G.whoseTurn]);
	assertEqual("Test to see if our hand count was increaed by 2", 7, G.handCount[G.whoseTurn]);
	assertEqual("Test to ensure our played card count increased by 1", 1, G.playedCardCount);
	assertNEqual("Test to ensure smithy is no longer in hand", smithy, G.hand[G.whoseTurn][0]);


	printf("___________________________________________\n\n");
	return 0;
}

