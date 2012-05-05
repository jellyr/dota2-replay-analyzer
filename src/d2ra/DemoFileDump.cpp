#include "DemoFileDump.h"
#include <iostream>
#include <typeinfo>

NewDemoFileDump::NewDemoFileDump( )
	: _frame( 0 )
	, _callback( nullptr )
	, _context( nullptr )
	, _previouspercent( -1 )
	, _parsingthread( nullptr )
{
	ZeroMemory( &_generalInformation, sizeof( _generalInformation ) );
	ZeroMemory( &_playerInformation, sizeof( _playerInformation ) );
}

NewDemoFileDump::~NewDemoFileDump( )
{
}

bool NewDemoFileDump::Open( const wchar_t *filename )
{
	return _demofile.Open( filename );
}

void NewDemoFileDump::Close( )
{
	_demofile.Close( );
}

void NewDemoFileDump::GetGeneralInformation( PD2GENERAL_INFORMATION generalInformation )
{
	memcpy_s( generalInformation, sizeof( *generalInformation ), &_generalInformation, sizeof( _generalInformation ) );
}

void NewDemoFileDump::GetPlayerInformation( PD2PLAYER_INFORMATION playerInformation )
{
	memcpy_s( playerInformation, sizeof( *playerInformation ), &_playerInformation, sizeof( _playerInformation ) );
}

DWORD WINAPI NewDemoFileDump::ParsingThreadProc( LPVOID param )
{
	if ( param )
	{
		NewDemoFileDump* instance = ( NewDemoFileDump* ) param;
		instance->DumpThread( );
	}

	return 0;
}

void NewDemoFileDump::DoDump( )
{
	// so, we want this: each thread to have a message queue:
	// - thread 1 needs to be notified when the progress changes
	// - thread 2 needs to be notified to stop/cancel

	// create a message loop for thread 1
	MSG msg;
	PeekMessage( &msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE );

	// create an event, then create the thread; wait for the event
	// so that we know the 
	_parsingthread = CreateThread( nullptr, 0, &NewDemoFileDump::ParsingThreadProc, this, 0, nullptr );
	assert( _parsingthread );
}

void NewDemoFileDump::DumpThread( )
{
	bool bStopReading = false;

	int tick = 0;
	int size = 0;
	bool compressed = false;
	int uncompressedSize = 0;

	for ( _frame = 0; !bStopReading; _frame++ )
	{
		tick = 0;
		size = 0;
		compressed = false;
		uncompressedSize = 0;

		if ( _demofile.Finished( ) )
			break;

		EDemoCommands DemoCommand = _demofile.ReadMessageType( &tick, &compressed );

		switch( DemoCommand )
		{
			case DEM_FileHeader:
				HandleMessage<CDemoFileHeader_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_FileInfo:
				HandleMessage<CDemoFileInfo_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_Stop:
				HandleMessage<CDemoStop_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_SyncTick:
				HandleMessage<CDemoSyncTick_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_ConsoleCmd:
				HandleMessage<CDemoConsoleCmd_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_SendTables:
				HandleMessage<CDemoSendTables_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_ClassInfo:
				HandleMessage<CDemoClassInfo_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_StringTables:
				HandleMessage<CDemoStringTables_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_UserCmd:
				HandleMessage<CDemoUserCmd_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_CustomDataCallbacks:
				HandleMessage<CDemoCustomDataCallbacks_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_CustomData:
				HandleMessage<CDemoCustomData_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_Packet:
			case DEM_SignonPacket:
				HandleMessage<CDemoPacket_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_FullPacket:
				HandleMessage<CDemoFullPacket_t>( compressed, tick, size, uncompressedSize ); break;
			
			case DEM_Error:
				bStopReading = true;
				break;
			
			default:
				std::cout << DemoCommand << std::endl;
				break;
		}

		if ( _callback )
		{
			int progress = _demofile.Percent( );
			if ( _previouspercent != progress )
			{
				_callback( _context, progress );
				_previouspercent = progress;
			}
		}
	}
}

template <class DEMCLASS>
void NewDemoFileDump::HandleMessage( bool compressed, int tick, int& size, int& uncompressedSize )
{
	DEMCLASS Msg;
	_demofile.ReadMessage( &Msg, compressed, &size, &uncompressedSize );

	//Msg.GetProtoMsg( ).PrintDebugString( );
}

static const char* STRING_TABLE_GAME_MODE[] =
{
	"",
	"All Pick",
	"All Random",
	"Random Draft",
	"Single Draft"
};

static const char* STRING_TABLE_GAME_WINNER[] =
{
	"",
	"",
	"Radiant",
	"Dire"
};

template <>
void NewDemoFileDump::HandleMessage<CDemoFileHeader_t>( bool compressed, int tick, int& size, int& uncompressedSize )
{
	CDemoFileHeader_t Msg;
	_demofile.ReadMessage( &Msg, compressed, &size, &uncompressedSize );

	strcpy_s( _generalInformation.serverName, Msg.server_name( ).c_str( ) );
	_generalInformation.networkProtocol = Msg.network_protocol( );
	_generalInformation.allowClientsideEntities = Msg.allow_clientside_entities( );
}

template <>
void NewDemoFileDump::HandleMessage<CDemoFileInfo_t>( bool compressed, int tick, int& size, int& uncompressedSize )
{
	CDemoFileInfo_t Msg;
	_demofile.ReadMessage( &Msg, compressed, &size, &uncompressedSize );

	_generalInformation.matchId = Msg.game_info( ).dota( ).match_id( );
	_generalInformation.winner = Msg.game_info( ).dota( ).game_winner( );
	_generalInformation.mode = Msg.game_info( ).dota( ).game_mode( );
	_generalInformation.duration = Msg.playback_time( );

	strcpy_s( _generalInformation.winnerString, STRING_TABLE_GAME_WINNER[_generalInformation.winner] );
	strcpy_s( _generalInformation.modeString, STRING_TABLE_GAME_MODE[_generalInformation.mode] );

	std::string str = Msg.GetProtoMsg( ).DebugString( );
	OutputDebugStringA( str.c_str( ) );
	
	for ( int i = 0; i < _countof( _playerInformation.players ); i++ )
	{
		strcpy_s( _playerInformation.players[i].name, Msg.game_info( ).dota( ).player_info( i ).player_name( ).c_str( ) );
		strcpy_s( _playerInformation.players[i].hero, Msg.game_info( ).dota( ).player_info( i ).hero_name( ).c_str( ) + 14 );
	}
}

void NewDemoFileDump::SetProgressCallback( PROGRESS_CALLBACK callback, void* context )
{
	_callback = callback;
	_context = context;
}