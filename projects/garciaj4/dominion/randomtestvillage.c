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
#include <math.h>

#define NUM_RUNS 100

void assertEqual(char* description, int reference, int value){
	if(value == reference){
		//printf("SUCCESS:%s\n", description);
	}
	else{
		printf("FAILED:%s\n", description);
		printf("Expected: %d     Received: %d\n", reference, value);
	}
}

void assertNEqual(char* description, int reference, int value){
	if(value != reference){
		//printf("SUCCESS:%s\n", description);
	}
	else{
		printf("FAILED:%s\n", description);
		printf("Expected: %d     Received: %d\n", reference, value);
	}
}

void assertLess(char* description, int lhs, int rhs){
	if(lhs<rhs){
		//printf("SUCCESS:%s\n", description);
	}
	else{
		printf("FAILED:%s", description);
		printf(" %d >= %d\n", lhs, rhs);
	}
}

int testVillage(int p, struct gameState* game){

	//save the game so we can check the state after smithy has been played
	struct gameState savedGame;
	memcpy(&savedGame, game, sizeof(struct gameState));

	//int status = cardEffect(village, 0, 0, 0, game, 0, 0);
	//assert(status == 0);

	//assertNEqual("Test if village was removed from the hand", village, G.hand[G.whoseTurn][4]);
	//assertEqual("Test if our hand size remained the same", 5, G.handCount[G.whoseTurn]);
	//assertEqual("Test if our actions increased by one", 2, G.numActions);
	//assertEqual("Test if our deck size decreased by one", 4, G.deckCount[G.whoseTurn]);
	//assertEqual("Test if our played cards increased by one", 1, G.playedCardCount);

	return 0;
}

int main() {

	printf("TESTING: village:\n");

	PlantSeeds(1);
	int idx, jdx, p;

	struct gameState G;

	for (idx = 0; idx < NUM_RUNS; idx++) {
		//completely randomize game state
		//taken from testDrawCard.c
		for (jdx = 0; jdx < sizeof(struct gameState); jdx++) {
	  		((char*)&G)[jdx] = floor(Random() * 256);
		}
		//I increased the integers by one so that we get a more uniform distribution
		//this will cause a problem if Random() return '1', but the chances
		//of this occuring are technically 0.

		//get random player from gamestate
		p = floor(Random() * 3) + 1;

		//make sure to conform to logical game state prerequisites
		G.deckCount[p] = floor(Random() * MAX_DECK);
		G.discardCount[p] = floor(Random() * MAX_DECK);
		G.handCount[p] = floor(Random() * MAX_HAND);

		//make sure we have village
		G.hand[p][0] = village;

		//put limits on game conditions that would bound mem blocks
		G.whoseTurn = p;
		G.playedCardCount = floor(Random() * MAX_HAND);
		G.handCount[p] = 2;
		//test the card
		testVillage(p, &G);
	}

	printf("\n");

	return 0;
}