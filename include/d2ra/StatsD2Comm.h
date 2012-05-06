#pragma once

#include "exports.h"

class StatsD2Comm
{
private:

public:
	StatsD2Comm( );

public:
	void DownloadReplay( long matchId );

	typedef void ( *PROGRESS_CALLBACK )( void*, int );
	void SetProgressCallback( PROGRESS_CALLBACK callback, void* context );
};