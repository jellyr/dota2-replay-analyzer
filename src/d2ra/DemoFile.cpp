#include "DemoFile.h"
#include "snappy/snappy.h"

NewDemoFile::NewDemoFile( )
	: _bufferpos( 0 )
{
}

NewDemoFile::~NewDemoFile( )
{
	Close( );
}

bool NewDemoFile::Open( const wchar_t* name )
{
	Close();

	FILE *fp = _wfopen( name, L"rb" );
	if ( fp )
	{
		size_t Length;
		DemoHeader demoHeader;

		fseek( fp, 0, SEEK_END );
		Length = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		fread( &demoHeader, 1, sizeof( demoHeader ), fp );
		Length -= sizeof( demoHeader );

		if( strcmp( demoHeader.filestamp, PROTODEMO_HEADER_ID ) )
		{
			return false;
		}

		_filebuffer.resize( Length );
		fread( &_filebuffer[0], 1, Length, fp );

		fclose( fp );
		fp = nullptr;

		_bufferpos = 0;
		_name = name ;
		return true;
	}
	else
	{
		return false;
	}
}

void NewDemoFile::Close( )
{
	_bufferpos = 0;
	_filebuffer.clear( );
	_name.clear( );
}

EDemoCommands NewDemoFile::ReadMessageType( int *tick, bool *compressed )
{
	int Cmd = ReadInt( _filebuffer, _bufferpos );

	if ( compressed )
	{
		*compressed = !!( Cmd & DEM_IsCompressed );
	}

	Cmd = ( Cmd & ~DEM_IsCompressed );

	int thistick = ReadInt( _filebuffer, _bufferpos );
	if ( tick )
	{
		*tick = thistick;
	}

	if ( _bufferpos >= _filebuffer.size( ) )
	{
		return DEM_Error;
	}

	return ( EDemoCommands ) Cmd;
}

bool NewDemoFile::ReadMessage(IDemoMessage *message, bool compressed, int *size /*= nullptr*/, int *uncompressedSize /*= nullptr*/ )
{
	int readsize = ReadInt( _filebuffer, _bufferpos );

	if ( size )
	{
		*size = readsize;
	}
	if ( uncompressedSize )
	{
		*uncompressedSize = 0;
	}

	if ( _bufferpos + readsize > _filebuffer.size( ) )
	{
		assert( 0 );
		return false;
	}

	if ( message )
	{
		const char *parseBuffer = &_filebuffer[ _bufferpos ];
		_bufferpos += readsize;

		if ( compressed )
		{
			if ( snappy::IsValidCompressedBuffer( parseBuffer, readsize ) )
			{
				size_t uDecompressedLen;

				if ( snappy::GetUncompressedLength( parseBuffer, readsize, &uDecompressedLen ) )
				{
					if( uncompressedSize )
					{
						*uncompressedSize = uDecompressedLen;
					}

					_snappybuffer.resize( uDecompressedLen );
					char *parseBufferUncompressed = &_snappybuffer[ 0 ];

					if ( snappy::RawUncompress( parseBuffer, readsize, parseBufferUncompressed ) )
					{
						if ( message->GetProtoMsg( ).ParseFromArray( parseBufferUncompressed, uDecompressedLen ) )
						{
							return true;
						}
					}
				}
			}

			assert( 0 );
			return false;
		}

		return message->GetProtoMsg( ).ParseFromArray( parseBuffer, readsize );
	}
	else
	{
		_bufferpos += readsize;
		return true;
	}
}

bool NewDemoFile::Finished( )
{
	return _bufferpos >= _filebuffer.size( );
}

const wchar_t* NewDemoFile::GetName( )
{
	return _name.c_str( );
}

int NewDemoFile::Percent( )
{
	double result = ( double( _bufferpos ) / double( _filebuffer.size( ) ) ) * 100;
	return int( result );
}

bool NewDemoFile::Skip( size_t bytes )
{
	if ( _bufferpos + bytes >= _filebuffer.size( ) )
	{
		return false;
	}

	_bufferpos += bytes;
	assert( ! Finished( ) );

	return true;
}