#include "exports.h"
#include "DemoFileDump.h"
#include "StatsD2Comm.h"

NewDemoFileDump fileDump;
StatsD2Comm statsDota2Comm;

bool D2Open( const wchar_t* fileName )
{
	return fileDump.Open( fileName );
}

void D2Parse( )
{
	fileDump.DoDump( );
}

void D2Close( )
{
	fileDump.Close( );
}

void D2SetParseProgressCallback( D2RA_PROGRESS_CALLBACK callback, void* context )
{
	fileDump.SetProgressCallback( callback, context );
}

void D2SetStoppedCallback( D2RA_STOPPED_CALLBACK callback, void* context )
{
}

void D2RA_API D2GetGeneralInformation( PD2GENERAL_INFORMATION generalInfo )
{
	fileDump.GetGeneralInformation( generalInfo );
}

void D2RA_API D2GetPlayerInformation( PD2PLAYER_INFORMATION playerInfo )
{
	fileDump.GetPlayerInformation( playerInfo );
}

void D2SetDownloadProgressCallback( D2RA_PROGRESS_CALLBACK callback, void* context )
{
	statsDota2Comm.SetProgressCallback( callback, context );
}

void D2StatsDownloadReplay( long matchId )
{
	statsDota2Comm.DownloadReplay( matchId );
}

void D2StatsGetReplayInformation( long matchId, PD2STATS_REPLAY_INFORMATION replayInformation )
{
	return statsDota2Comm.GetReplayInformation( matchId, replayInformation );
}