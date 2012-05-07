#include "StatsD2Comm.h"
#include <sstream>

StatsD2Comm::StatsD2Comm( )
{
	curl = curl_easy_init( );

	curl_easy_setopt( curl, CURLOPT_VERBOSE, 1 );
}

StatsD2Comm::~StatsD2Comm( )
{
	curl_easy_cleanup( curl );
}

void StatsD2Comm::DownloadReplay( long matchId )
{
}

void StatsD2Comm::SetProgressCallback( PROGRESS_CALLBACK callback, void* context )
{
}

int my_curl_debug_callback (CURL *c, curl_infotype type, char * str, size_t size, void *)
{
	return 0;
}

size_t my_curl_write_data( void *ptr, size_t size, size_t nmemb, std::string *stream )
{
	size_t result = 0;
	if ( stream )
	{
		stream->append( ( const char* ) ptr, size * nmemb );
		result = size * nmemb;
	}
	return result;
}

void StatsD2Comm::GetReplayInformation( long matchId, PD2STATS_REPLAY_INFORMATION replayInformation )
{
	std::stringstream url;
	url << "http://stats.dota2.be/api/match.php?id=" << matchId << "\0";

	std::string buffer;

	curl_easy_setopt( curl, CURLOPT_URL, url.str( ).c_str( ) );
	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION,  my_curl_write_data );
	curl_easy_setopt( curl, CURLOPT_WRITEDATA, &buffer );

	curl_easy_perform( curl );

	OutputDebugStringA( buffer.c_str( ) );
}