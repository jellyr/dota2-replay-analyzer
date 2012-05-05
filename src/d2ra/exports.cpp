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
