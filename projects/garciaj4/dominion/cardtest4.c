/* -----------------------------------------------------------------------
 * Unit tests for the village card.
 * Include the following lines in your makefile:
 *
 * cardTest4: cardtest4.c dominion.o rngs.o
 *      gcc -o cardTest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
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

	printf("TESTING: Village:\n");

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
	G.hand[G.whoseTurn][4] = village;

	/*with this configuration we should see:
		our actions increase by one
		village removed from our hand
		hand size remain the same
		deck size decrease by one
		played cards increase by one*/

	//lets check our pre-condition assumptions
	assert(G.numActions == 1);
	assert(G.handCount[G.whoseTurn] == 5);
	assert(G.playedCardCount == 0);
	assert(G.deckCount[G.whoseTurn] == 5);

	status = playVillage(&G, 4);
	assert(status == 0);

	assertNEqual("Test if village was removed from the hand", village, G.hand[G.whoseTurn][4]);
	assertEqual("Test if our hand size remained the same", 5, G.handCount[G.whoseTurn]);
	assertEqual("Test if our actions increased by one", 2, G.numActions);
	assertEqual("Test if our deck size decreased by one", 4, G.deckCount[G.whoseTurn]);
	assertEqual("Test if our played cards increased by one", 1, G.playedCardCount);

	printf("___________________________________________\n\n");
	return 0;
}

