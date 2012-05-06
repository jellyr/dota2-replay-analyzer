#include "exports.h"
#include <iostream>
#include <Windows.h>

void ProgressCallback( void*, int percent )
{
	std::cout << percent << "% done" << std::endl;
}

int main( )
{
	D2Open( L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\dota 2 beta\\dota\\12288739.dem" );
	D2SetParseProgressCallback( ProgressCallback, nullptr );
	D2Parse( );
	
	Sleep( 20000 );
	
	D2Close( );
}
