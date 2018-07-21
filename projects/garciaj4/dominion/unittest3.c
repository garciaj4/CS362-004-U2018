/* -----------------------------------------------------------------------
 * Unit tests for the supplyCount() function.
 * Include the following lines in your makefile:
 *
 * unitTest3: unittest3.c dominion.o rngs.o
 *      gcc -o unitTest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
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

	printf("TESTING supplyCount():\n");

	//initialize test environment
	int g = -5;
	int count = -5;

	//establish a mock games state
	int numPlayers = 2;
	int k[10] = {adventurer, council_room, feast, salvager, mine, remodel, smithy, village, baron, cutpurse};
	int seed = 1000;
	struct gameState G;

	memset(&G, -5, sizeof(struct gameState));
	g = initializeGame(numPlayers, k, seed, &G);
	assert(g == 0);

	//since this is a simple function we are going to test boundary conditions and graceful error handling
	//first what if we look for a card that was not included
	count = supplyCount(minion, &G);
	assertEqual("Test to ensure a supply of the non-included kingdom cards dne.", -1, count);
	count = -5;

	//we should start with ten of each kingdom card
	int startCount = 1;
	int idx;
	for(idx = 0; idx < 10; idx++){
		count = supplyCount(k[idx], &G);
		if(count != 10){
			startCount = 0;
		}
	}
	assertEqual("Ensure we start with the correct number of kingdom cards", 1, startCount);
	count = -5;

	//what if a supply becomes something unreasonable, how will the function handle it
	G.supplyCount[adventurer] = -50;
	count = supplyCount(adventurer, &G);
	assertEqual("Testing detection of garbage supply value", -1, count);
	G.supplyCount[adventurer] = 10;
	count = -5;

	//if I check the supply of a card out of bounds what happens
	count = supplyCount(-1, &G);
	assertEqual("Testing a negative supplyPos", -1, count);
	count = -5;

	count = supplyCount(treasure_map + 5, &G);
	assertEqual("Testing an out-of-bounds supplyPos", -1, count);
	count = -5;

	//what happens if i don't pass a valid gamestate?
	count = supplyCount(adventurer, &count);
	assertEqual("Testing an invalid game state argument", -1, count);

	printf("___________________________________________\n\n");
	return 0;
}

