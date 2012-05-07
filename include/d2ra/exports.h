#pragma once

#if ! defined( D2RA_API )
# if defined( D2RA_DLL )
#  define D2RA_API __declspec( dllexport )
# else
#  define D2RA_API __declspec( dllimport )
# endif
#endif

#include "export_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

// A callback for when the parsing takes place
typedef void ( *D2RA_PROGRESS_CALLBACK )( void*, int );

// A callback for when the parsing encounters an error, or is stopped
typedef void ( *D2RA_STOPPED_CALLBACK )( void* );

// Open a file name
bool D2RA_API D2Open( const wchar_t* fileName );

// Start parsing the file
void D2RA_API D2Parse( );

// Close the handle; this also stops the parsing if it's already started
void D2RA_API D2Close( );

// Get replay information
void D2RA_API D2StatsGetReplayInformation( long matchId, PD2STATS_REPLAY_INFORMATION replayInformation );

// Download a replay
void D2RA_API D2StatsDownloadReplay( long matchId );

// Set a custom callback for file parsing progress
void D2RA_API D2SetDownloadProgressCallback( D2RA_PROGRESS_CALLBACK callback, void* context );

// Set a custom callback for file parsing progress
void D2RA_API D2SetParseProgressCallback( D2RA_PROGRESS_CALLBACK callback, void* context );

// Set a custom callback for when parsing is stopped
void D2RA_API D2SetStoppedCallback( D2RA_STOPPED_CALLBACK callback, void* context );

// Get general information from the replay
void D2RA_API D2GetGeneralInformation( PD2GENERAL_INFORMATION generalInfo );

// Get information about players
void D2RA_API D2GetPlayerInformation( PD2PLAYER_INFORMATION playerInfo );

#ifdef __cplusplus
}
#endif
