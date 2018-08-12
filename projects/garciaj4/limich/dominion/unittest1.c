/* -----------------------------------------------------------------------
 * Unit tests for the buyCard() function.
 * Include the following lines in your makefile:
 *
 * unitTest1: unittest1.c dominion.o rngs.o
 *      gcc -o unitTest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
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

	printf("TESTING buyCard():\n");

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

	//give ourself enough coins to test
	G.coins = 100 * getCost(adventurer);

	//game should be in the action phase (0) as it was just initialized
	//test to make sure you can only buy during the appropriate phase
	//printf("Current Phase: %d\n", G.phase);
	status = buyCard(adventurer, &G);
	assertEqual("Ensure we can only buy if in the appropriate phase:", -1, status);
	status = -5;

	//test if we are able to buy with no buys left
	//this also indicates if buyCard decrements the buy count correctly
	assert(G.numBuys == 0);
	//printf("Current Buys: %d\n", G.numBuys);
	status = buyCard(adventurer, &G);
	assertEqual("Ensure we cannot buy with no buys left:", -1, status);
	//give ourself another buy for the next test
	G.numBuys = 1;
	status = -5;

	//test if we can buy with no supply
	G.supplyCount[adventurer] = 0;
	status = buyCard(adventurer, &G);
	assertEqual("Ensure we cannot buy if there is no supply:", -1, status);
	G.numBuys = 1;
	status = -5;

	//test if we can buy with no coins
	G.coins = getCost(adventurer) - 1;
	status = buyCard(adventurer, &G);
	assertEqual("Ensure we cannot buy if we have insufficient coins:", -1, status);
	G.numBuys = 1;
	status = -5;

	//now that we've checked buy conditions we're going to check game state
	//reinitialize the game, set coins to min needed and make a buy
	memset(&G, -5, sizeof(struct gameState));
	g = initializeGame(numPlayers, k, seed, &G);
	assert(g == 0);
	G.coins = getCost(adventurer);
	//make sure the assumptions we are using in our tests are true
	assert(G.coins >  0);
	assert(G.numBuys == 1);

	status = buyCard(adventurer, &G);
	//make sure buy was successfull
	assert(status == 0);

	//test that all appropriate fields of the gamestate have been updated correctly
	assertEqual("Test to make sure coins was decremented correctly:", 0, G.coins);

	//test to make sure numBuys is reduced
	assertEqual("Test to make sure numBuys is now 0:", 0, G.numBuys);

	//test to make sure the card is now in our hand
	int present = 0;
	int idx;
	for(idx = 0; idx < MAX_HAND; idx++){
		if(G.hand[G.whoseTurn][idx] == adventurer){
			present = 1;
		}
	}
	assertEqual("Test to make sure the card purchased hand is in the current player's hand:", 1, present);

	printf("___________________________________________\n\n");
	return 0;
}

