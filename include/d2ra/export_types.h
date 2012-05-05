#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_SERVERNAME_LENGTH	200
	
// General information about the replay
typedef struct
{
	long matchId;
	char serverName[MAX_SERVERNAME_LENGTH];
	int networkProtocol;
	bool allowClientsideEntities;
} D2GENERAL_INFORMATION, *PD2GENERAL_INFORMATION;


#ifdef __cplusplus
}
#endif
