/* -----------------------------------------------------------------------
 * Unit tests for the playCard() function.
 * Include the following lines in your makefile:
 *
 * unitTest2: unittest2.c dominion.o rngs.o
 *      gcc -o unitTest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
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

int main() {

	printf("TESTING playcard():\n");

	//initialize test environment
	int g = -5;
	int status = -5;

	//establish a mock games state
	int numPlayers = 2;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	int seed = 1000;
	struct gameState G;
	memset(&G, -5, sizeof(struct gameState));
	g = initializeGame(numPlayers, k, seed, &G);
	assert(g == 0);
	G.hand[G.whoseTurn][0] = estate;
	G.hand[G.whoseTurn][1] = smithy;
	G.hand[G.whoseTurn][2] = council_room;
	G.hand[G.whoseTurn][3] = village;
	G.hand[G.whoseTurn][4] = baron;

	//test function conditions
	//set to buy phase
	G.phase = 1;
	//test to ensure we cannot perform an action
	status = playCard(4, 0, 0, 0, &G);
	assertEqual("Test to ensure we cannot perform an action during the buy phase.", -1, status);
	//reset game state
	memset(&G, -5, sizeof(struct gameState));
	g = initializeGame(numPlayers, k, seed, &G);
	assert(g == 0);
	G.hand[G.whoseTurn][0] = estate;
	G.hand[G.whoseTurn][1] = smithy;
	G.hand[G.whoseTurn][2] = council_room;
	G.hand[G.whoseTurn][3] = village;
	G.hand[G.whoseTurn][4] = baron;

	//reduce available actions to zero
	G.numActions = 0;
	//test to ensure we cannot perform an action
	status = playCard(4, 0, 0, 0, &G);
	assertEqual("Test to ensure we cannot perform an action if we have none left.", -1, status);
	//reset game state
	memset(&G, -5, sizeof(struct gameState));
	g = initializeGame(numPlayers, k, seed, &G);
	assert(g == 0);
	G.hand[G.whoseTurn][0] = estate;
	G.hand[G.whoseTurn][1] = smithy;
	G.hand[G.whoseTurn][2] = council_room;
	G.hand[G.whoseTurn][3] = village;
	G.hand[G.whoseTurn][4] = baron;

	//test to ensure we cannot play a non-action card as an action
	status = playCard(0, 0, 0, 0, &G);
	assertEqual("Test to ensure we cannot play a non-action card.", -1, status);
	//reset game state
	memset(&G, -5, sizeof(struct gameState));
	g = initializeGame(numPlayers, k, seed, &G);
	assert(g == 0);
	G.hand[G.whoseTurn][0] = estate;
	G.hand[G.whoseTurn][1] = smithy;
	G.hand[G.whoseTurn][2] = council_room;
	G.hand[G.whoseTurn][3] = village;
	G.hand[G.whoseTurn][4] = baron;
	G.coins = 0;

	//now that we've checked function conditions we're going to check game state

	//now we're going to successfully play baron passing in the trade choice as true
	//lets check all of our preconditions
	//printf("G.numBuys: %d\n", G.numBuys);
	assert(G.numBuys == 1);
	//printf("G.hand[G.whoseTurn][0]: %d\n", G.hand[G.whoseTurn][0]);
	assert(G.hand[G.whoseTurn][0] == estate);
	//printf("G.discardCount[G.whoseTurn]: %d\n", G.discardCount[G.whoseTurn]);
	assert(G.discardCount[G.whoseTurn] == 0);
	assert(G.coins == 0);

	
	status = playCard(4, 1, 0, 0, &G);
	assert(status == 0);

	/*we should see:
		our buys increase by one
		the estate dissapear from our hand
		the estate appear in the discard
		our coins increase by four
	*/
	int goodGameState = 1;
	if(G.numBuys != 2){
		goodGameState = -1;
	}
	if(G.hand[G.whoseTurn][0] == estate){
		goodGameState = -2;
	}
	if(G.discardCount[G.whoseTurn] != 1 || G.discard[G.whoseTurn][0] != estate){
		goodGameState = -3;
	}
	//printf("G.coins: %d\n", G.coins);
	if(G.coins != 4){
		goodGameState = -4;
	}
	assertEqual("Testing for appropriate gamestate after playing Baron/Discard == True:", 1, goodGameState);

	printf("___________________________________________\n\n");
	return 0;
}

