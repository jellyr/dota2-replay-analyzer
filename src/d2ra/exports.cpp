#include "exports.h"
#include "DemoFileDump.h"

NewDemoFileDump fileDump;

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

void D2SetProgressCallback( D2RA_PROGRESS_CALLBACK callback, void* context )
{
	fileDump.SetProgressCallback( callback, context );
}

void D2SetStoppedCallback( D2RA_STOPPED_CALLBACK callback, void* context )
{
	// TODO
}

void D2RA_API D2GetGeneralInformation( PD2GENERAL_INFORMATION generalInfo )
{
	fileDump.GetGeneralInformation( generalInfo );
}

void D2RA_API D2GetPlayerInformation( PD2PLAYER_INFORMATION playerInfo )
{
	fileDump.GetPlayerInformation( playerInfo );
}