#pragma once

#include "common.h"
#include "DemoFile.h"
#include "export_types.h"

#include <Windows.h>

class NewDemoFileDump
{
public:
	NewDemoFileDump( );
	~NewDemoFileDump( );

public:
	bool Open( const wchar_t *filename );
	void DoDump( );
	void Close( );

	typedef void ( *PROGRESS_CALLBACK )( void*, int );
	void SetProgressCallback( PROGRESS_CALLBACK callback, void* context );

private:
	static DWORD WINAPI ParsingThreadProc( LPVOID param );
	void DumpThread( );

	template <class DEMCLASS>
	void HandleMessage( bool compressed, int tick, int& size, int& uncompressedSize );

	template <class DEMCLASS>
	void SkipMessage( bool compressed, int tick, int& size, int& uncompressedSize );

	template <>
	void HandleMessage<CDemoFileHeader_t>( bool compressed, int tick, int& size, int& uncompressedSize );

	template <>
	void HandleMessage<CDemoFileInfo_t>( bool compressed, int tick, int& size, int& uncompressedSize );

private:
	NewDemoFile _demofile;
	int _frame;

	PROGRESS_CALLBACK _callback;
	void* _context;
	int _previouspercent;

	HANDLE _parsingthread;

private:
	D2GENERAL_INFORMATION _generalInformation;
	D2PLAYER_INFORMATION _playerInformation;

public:
	void GetGeneralInformation( PD2GENERAL_INFORMATION generalInformation );
	void GetPlayerInformation( PD2PLAYER_INFORMATION playerInformation );
};