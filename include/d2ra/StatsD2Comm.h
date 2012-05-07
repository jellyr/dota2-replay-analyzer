#pragma once

#include "exports.h"
#include "curl/curl.h"

class StatsD2Comm
{
private:
	CURL *curl;

public:
	StatsD2Comm( );
	~StatsD2Comm( );

public:
	void DownloadReplay( long matchId );
	void GetReplayInformation( long matchId, PD2STATS_REPLAY_INFORMATION replayInformation );

	typedef void ( *PROGRESS_CALLBACK )( void*, int );
	void SetProgressCallback( PROGRESS_CALLBACK callback, void* context );
};