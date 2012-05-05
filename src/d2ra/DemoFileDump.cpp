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

//template <>
//void NewDemoFileDump::HandleMessage<CDemoPacket_t>( bool compressed, int tick, int& size, int& uncompressedSize )
//{
//	CDemoPacket_t Msg;
//	_demofile.ReadMessage( &Msg, compressed, &size, &uncompressedSize );
//
////	size_t index = 0;
////
////	while ( index < Msg.data( ).size( ) )
////	{
////		int Cmd = ReadInt( Msg.data( ), index );
////		uint32 Size = ReadInt( Msg.data( ), index );
////
////		std::string strName;
////		if( NET_Messages_IsValid( Cmd ) )
////		{
////			strName = NET_Messages_Name( ( NET_Messages ) Cmd );
////		}
////		else if( SVC_Messages_IsValid( Cmd ) )
////		{
////			strName = SVC_Messages_Name( ( SVC_Messages ) Cmd );
////		}
////
////		if ( index + Size > Msg.data( ).size( ) )
////		{
////			// ???
////		}
////
//////		switch( Cmd )
//////		{
//////#define HANDLE_NetMsg( _x )		case net_ ## _x: PrintNetMessage< CNETMsg_ ## _x, net_ ## _x >( *this, &buf[ index ], Size ); break
//////#define HANDLE_SvcMsg( _x )		case svc_ ## _x: PrintNetMessage< CSVCMsg_ ## _x, svc_ ## _x >( *this, &buf[ index ], Size ); break
//////
//////		default:
//////			printf( "WARNING. DumpUserMessage(): Unknown netmessage %d.\n", Cmd );
//////			break;
//////
//////		HANDLE_NetMsg( NOP );            	// 0
//////		HANDLE_NetMsg( Disconnect );        // 1
//////		HANDLE_NetMsg( File );              // 2
//////		HANDLE_NetMsg( SplitScreenUser );   // 3
//////		HANDLE_NetMsg( Tick );              // 4
//////		HANDLE_NetMsg( StringCmd );         // 5
//////		HANDLE_NetMsg( SetConVar );         // 6
//////		HANDLE_NetMsg( SignonState );       // 7
//////		HANDLE_SvcMsg( ServerInfo );        // 8
//////		HANDLE_SvcMsg( SendTable );         // 9
//////		HANDLE_SvcMsg( ClassInfo );         // 10
//////		HANDLE_SvcMsg( SetPause );          // 11
//////		HANDLE_SvcMsg( CreateStringTable ); // 12
//////		HANDLE_SvcMsg( UpdateStringTable ); // 13
//////		HANDLE_SvcMsg( VoiceInit );         // 14
//////		HANDLE_SvcMsg( VoiceData );         // 15
//////		HANDLE_SvcMsg( Print );             // 16
//////		HANDLE_SvcMsg( Sounds );            // 17
//////		HANDLE_SvcMsg( SetView );           // 18
//////		HANDLE_SvcMsg( FixAngle );          // 19
//////		HANDLE_SvcMsg( CrosshairAngle );    // 20
//////		HANDLE_SvcMsg( BSPDecal );          // 21
//////		HANDLE_SvcMsg( SplitScreen );       // 22
//////		HANDLE_SvcMsg( UserMessage );       // 23
//////		//$ HANDLE_SvcMsg( EntityMessage ); // 24
//////		HANDLE_SvcMsg( GameEvent );         // 25
//////		HANDLE_SvcMsg( PacketEntities );    // 26
//////		HANDLE_SvcMsg( TempEntities );      // 27
//////		HANDLE_SvcMsg( Prefetch );          // 28
//////		HANDLE_SvcMsg( Menu );              // 29
//////		HANDLE_SvcMsg( GameEventList );     // 30
//////		HANDLE_SvcMsg( GetCvarValue );      // 31
//////
//////#undef HANDLE_SvcMsg
//////#undef HANDLE_NetMsg
//////		}
////
////		index += Size;
////	}
//}
//

//template <>
//void NewDemoFileDump::HandleMessage<CDemoFullPacket_t>( bool compressed, int tick, int& size, int& uncompressedSize )
//{
//	CDemoFullPacket_t Msg;
//	_demofile.ReadMessage( &Msg, compressed, &size, &uncompressedSize );
//
//	Msg.GetProtoMsg( ).PrintDebugString( );
//}

void NewDemoFileDump::SetProgressCallback( PROGRESS_CALLBACK callback, void* context )
{
	_callback = callback;
	_context = context;
}