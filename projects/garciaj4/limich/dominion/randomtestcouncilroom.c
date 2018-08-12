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

int testCouncilRoom(int p, struct gameState* game){

	//save the game so we can check the state after council room has been played
	struct gameState savedGame;
	memcpy(&savedGame, game, sizeof(struct gameState));

	int status = cardEffect(council_room, 0, 0, 0, game, 0, 0);
	assert(status == 0);
	assertEqual("Test to see if our actions was reduced by 1", savedGame.numActions-1, game->numActions);
	assertNEqual("Test council_room is no longer in our hand", game->hand[game->whoseTurn][0], council_room);
	assertEqual("Test our played cards increases by 1", savedGame.playedCardCount+1, game->playedCardCount);
	assertEqual("Test if our hand size increases by 3", savedGame.handCount[savedGame.whoseTurn]+3, game->handCount[game->whoseTurn]);
	assertEqual("Test if our buys increased by 1", savedGame.numBuys+1, game->numBuys);

	return 0;
}

int main() {

	printf("TESTING: council_room:\n");

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
		G.numPlayers = 4;
		p = floor(Random() * 3) + 1;

		//make sure to conform to logical game state prerequisites
		int idx;
		for(idx = 0; idx < G.numPlayers; idx++){
			G.deckCount[idx] = floor(Random() * MAX_DECK + 1);
			G.discardCount[idx] = floor(Random() * MAX_DECK + 1);
			G.handCount[idx] = floor(Random() * MAX_HAND + 1);
		}

		//make sure we have council room
		G.hand[p][0] = council_room;

		//put limits on game conditions that would bound mem blocks
		G.whoseTurn = p;
		G.playedCardCount = floor(Random() * MAX_HAND);

		//test the card
		testCouncilRoom(p, &G);
	}

	printf("\n");

	return 0;
}

