/* -----------------------------------------------------------------------
 * Unit tests for the adventurer card.
 * Include the following lines in your makefile:
 *
 * cardTest2: cardtest2.c dominion.o rngs.o
 *      gcc -o cardTest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
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

	printf("TESTING: adventurer:\n");

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

	/*we're also going to stack the deck to follow events more easily
	checking the drawCard() funciton we know we will draw from the top*/
	G.deck[G.whoseTurn][0] = copper;
	G.deck[G.whoseTurn][1] = copper;
	G.deck[G.whoseTurn][2] = copper;
	G.deck[G.whoseTurn][3] = estate;
	G.deck[G.whoseTurn][4] = copper;

	/*with this configuration we should:
		remove adventurer from our hand
		increase played cards by 1
		reduce the deck by three cards
		add two copper to our hand
		discard an estate card*/

	//lets check our pre-condition assumptions
	assert(G.handCount[G.whoseTurn] == 5);
	assert(G.deckCount[G.whoseTurn] == 5);
	assert(G.playedCardCount == 0);
	status = cardEffect(adventurer, 0, 0, 0, &G, 4, 0);
	assertEqual("Test to see if our actions was reduced by 1", 0, G.numActions);
	assertNEqual("Test if adventurer was removed from our hand", G.hand[G.whoseTurn][4], adventurer);
	assertEqual("Test if our played cards icnreased by one", 1, G.playedCardCount);
	assertEqual("Test if our deck was reduced by three cards", 2, G.deckCount[G.whoseTurn]);
	assertEqual("Test if our hand count increased by one cards", 6, G.handCount[G.whoseTurn]);

	int copperFlag = 1;
	if(G.hand[G.whoseTurn][G.handCount[G.whoseTurn]-2] != copper){
		copperFlag = 0;
	}
	if(G.hand[G.whoseTurn][G.handCount[G.whoseTurn]-1] != copper){
		copperFlag = 0;
	}
	assertEqual("Test if the two new cards are the coppers we were expecting", 1, copperFlag);
	assertEqual("Test if we discarded an estate card", estate, G.discard[G.whoseTurn][0]);
	assert(status ==  0);

	printf("___________________________________________\n\n");
	return 0;
}

