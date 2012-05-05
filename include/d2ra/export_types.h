#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_SERVERNAME_LENGTH	200
#define MAX_GAME_WINNER_LENGTH	40
#define MAX_GAME_MODE_LENGTH	40

// TODO: Make sure these are ok
#define GAME_WINNER_RADIANT		2
#define GAME_WINNER_DIRE		3

// TODO: Fix these
#define GAME_MODE_ALL_PICK		1
#define GAME_MODE_ALL_RANDOM	2
#define GAME_MODE_RANDOM_DRAFT	3
#define GAME_MODE_SINGLE_DRAFT	4

// General information about the replay
typedef struct
{
	long matchId;								// match id
	char serverName[MAX_SERVERNAME_LENGTH];		// server name (also gives hostname)
	int networkProtocol;						// protocol version
	bool allowClientsideEntities;				// allow client entities (not sure what it means)
	int winner;									// winner; see GAME_WINNER_*
	char winnerString[MAX_GAME_WINNER_LENGTH];	// winner given as string
	int mode;									// game mode; see GAME_MODE_*
	char modeString[MAX_GAME_MODE_LENGTH];		// game mode given as string
	float duration;								// how long it takes, in seconds
} D2GENERAL_INFORMATION, *PD2GENERAL_INFORMATION;


#ifdef __cplusplus
}
#endif
