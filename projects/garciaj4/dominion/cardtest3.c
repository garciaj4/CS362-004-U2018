/* -----------------------------------------------------------------------
 * Unit tests for the council_room card.
 * Include the following lines in your makefile:
 *
 * cardTest3: cardtest3.c dominion.o rngs.o
 *      gcc -o cardTest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
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

	printf("TESTING: Council Room:\n");

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
	G.hand[G.whoseTurn][4] = council_room;

	/*with this configuration we should see:
		remove council_room from our hand
		played cards icnreases by one
		increase hand size by three
		increase buys by 1
		check if other players hand size has increase by one and deck size has decreased by one*/

	//lets check our pre-condition assumptions
	assert(G.handCount[G.whoseTurn] == 5);
	assert(G.playedCardCount == 0);
	assert(G.numBuys == 1);
	assert(G.handCount[(G.whoseTurn + 1)%2] == 0);
	assert(G.deckCount[(G.whoseTurn + 1)%2] == 10);

	status = cardEffect(council_room, 0, 0, 0, &G, 4, 0);
	assert(status == 0);

	assertEqual("Test to see if our actions was reduced by 1", 0, G.numActions);
	assertNEqual("Test council_room is no longer in our hand", G.hand[G.whoseTurn][4], council_room);
	assertEqual("Test our played cards increases by 1", G.playedCardCount, 1);
	assertEqual("Test if our hand size increases by 3", 8, G.handCount[G.whoseTurn]);
	assertEqual("Test if our buys increased by 1", 2, G.numBuys);
	assertEqual("Test if the other player's hand count increase by one", 1, G.handCount[G.whoseTurn + 1]);
	assertEqual("Test if the other player's deck count decreased by one", 9, G.deckCount[G.whoseTurn + 1]);


	printf("___________________________________________\n\n");
	return 0;
}

