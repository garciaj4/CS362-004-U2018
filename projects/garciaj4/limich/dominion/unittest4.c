/* -----------------------------------------------------------------------
 * Unit tests for the updateCoins() function.
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

	printf("TESTING updateCoins():\n");

	/*Update coins is called in 4 instances, so we're going to test game states around those
	four instances and then also look at boundary conditions on the function itself.*/

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

	//Test to make sure the first player starts with 0 coins
	assertEqual("Test to make sure the very first player starts with 0 coins", 0, G.coins);
	G.coins = 0;

	//make sure the hand of the current player is summed correctly in a few instances

	G.hand[G.whoseTurn][0] = copper;
	G.hand[G.whoseTurn][1] = silver;
	G.hand[G.whoseTurn][2] = gold;
	G.hand[G.whoseTurn][3] = copper;
	G.hand[G.whoseTurn][4] = silver;
	status = updateCoins(G.whoseTurn, &G, 0);
	assert(status == 0);
	assertEqual("Test treausre of each type in a full treasure hand amounting to 9", 9, G.coins);
	status = -5;
	G.coins = -5;

	status = updateCoins(G.whoseTurn, &G, 5);
	assert(status == 0);
	assertEqual("Test treausre of each type in a full treasure hand amounting to 9 with a bonus of 5", 14, G.coins);
	status = -5;
	G.coins = -5;

	status = updateCoins(G.whoseTurn, &G, -5);
	assert(status == 0);
	assertEqual("Test treausre of each type in a full treasure hand amounting to 9 with a bonus of -5", 4, G.coins);
	status = -5;
	G.coins = -5;

	G.hand[G.whoseTurn][0] = steward;
	G.hand[G.whoseTurn][1] = adventurer;
	G.hand[G.whoseTurn][2] = adventurer;
	G.hand[G.whoseTurn][3] = adventurer;
	G.hand[G.whoseTurn][4] = adventurer;
	status = updateCoins(G.whoseTurn, &G, 0);
	assert(status == 0);
	assertEqual("Test an empty treasure hand", 0, G.coins);
	status = -5;
	G.coins = -5;

	status = updateCoins(G.whoseTurn, &G, 10);
	assert(status == 0);
	assertEqual("Test an empty treasure hand with a 10 bonus", 10, G.coins);
	status = -5;
	G.coins = -5;

	status = updateCoins(G.whoseTurn, &G, -10);
	assert(status == 0);
	assertEqual("Test an empty treasure hand with a -10 bonus", -10, G.coins);
	status = -5;
	G.coins = -5;

	//lets play a card and check coins then add our hand and check coins
	status = playCard(0, 2, 0, 0, &G);
	assert(status == 0);
	assertEqual("Test to ensure playing Steward with appropriate choices gives us appropriate treausre", 2, G.coins);
	status = 0;
	G.coins = -5;

	/*We cannot test the endTurn appearance of the updateCoins as the hand of the new player
	will change from game to game.*/

	printf("___________________________________________\n\n");
	return 0;
}

