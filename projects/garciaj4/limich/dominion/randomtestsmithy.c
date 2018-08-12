 /* -----------------------------------------------------------------------
 * Random tests for the smithy card.
 * Include the following lines in your makefile:
 *
 * cardTest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardTest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 *
 *Directly modeled after testDrawCard.c
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

int testSmithy(int p, struct gameState* game){

	//save the game so we can check the state after smithy has been played
	struct gameState savedGame;
	memcpy(&savedGame, game, sizeof(struct gameState));

	int status = cardEffect(smithy, 0, 0, 0, game, 0, 0);
	assert(status == 0);
	assertEqual("Test to see if our actions were reduced by 1", savedGame.numActions-1, game->numActions);
	assertEqual("Test to see if our deck count was reduced by 3", savedGame.deckCount[savedGame.whoseTurn]-3, game->deckCount[game->whoseTurn]);
	assertEqual("Test to see if our hand count was increaed by 2", savedGame.handCount[savedGame.whoseTurn]+2, game->handCount[game->whoseTurn]);
	assertEqual("Test to ensure our played card count increased by 1", savedGame.playedCardCount+1, game->playedCardCount);
	assertNEqual("Test to ensure smithy is no longer in hand", smithy, game->hand[game->whoseTurn][0]);

	return 0;
}

int main() {

	printf("TESTING: smithy:\n");

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
		p = floor(Random() * 5);

		//make sure to conform to logical game state prerequisites
		G.deckCount[p] = floor(Random() * MAX_DECK + 1);
		G.discardCount[p] = floor(Random() * MAX_DECK + 1);
		G.handCount[p] = floor(Random() * MAX_HAND + 1);

		//make sure we have smithy
		G.hand[p][0] = smithy;

		//put limits on game conditions that would bound mem blocks
		G.whoseTurn = p;
		G.playedCardCount = floor(Random() * MAX_HAND);

		//test the card
		testSmithy(p, &G);
	}

	printf("\n");

	return 0;
}

