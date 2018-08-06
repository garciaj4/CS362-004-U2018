/* -----------------------------------------------------------------------
 * Random tests for the adventurer card.
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

int testAdventurer(int p, struct gameState* game){

	//save the game so we can check the state after smithy has been played
	struct gameState savedGame;
	memcpy(&savedGame, game, sizeof(struct gameState));

	int status = cardEffect(adventurer, 0, 0, 0, game, 0, 0);
	assert(status == 0);
	assertEqual("Test to see if our actions was reduced by 1", savedGame.numActions-1, game->numActions);
	assertNEqual("Test if adventurer was removed from our hand", game->hand[game->whoseTurn][0], adventurer);
	assertEqual("Test if our played cards icnreased by one", savedGame.playedCardCount+1, game->playedCardCount);
	assertLess("Test if our deck was reduced", game->deckCount[game->whoseTurn], savedGame.deckCount[savedGame.whoseTurn]);
	assertEqual("Test if our hand count increased by one cards", savedGame.handCount[savedGame.whoseTurn]+1, game->handCount[game->whoseTurn]);

	return 0;
}

int main() {

	printf("TESTING: adventurer:\n");

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
		G.deckCount[p] = floor(Random() * MAX_DECK + 1);
		G.discardCount[p] = floor(Random() * MAX_DECK + 1);
		G.handCount[p] = floor(Random() * MAX_HAND + 1);

		//make sure we have adventurer
		G.hand[p][0] = adventurer;

		//make sure there are a few treasures in the deck
		for(jdx = MAX_HAND - 5; jdx < MAX_HAND; jdx++){
			G.deck[p][(int)floor(Random()*27)] = floor(Random()*3)+4;
		}

		//put limits on game conditions that would bound mem blocks
		G.whoseTurn = p;
		G.playedCardCount = floor(Random() * MAX_HAND);

		//we need to make sure our player's discard does not overflow
		//since i added 5 treasure we can discard up to our deck count -5
		G.discardCount[p] = floor(Random() * MAX_HAND) - (G.deckCount[p]+5);
		//test the card
		testAdventurer(p, &G);
	}

	printf("\n");

	return 0;
}