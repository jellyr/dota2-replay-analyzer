#pragma once

#include "common.h"

class NewDemoFile
{
private:
	std::wstring		_name;
	std::string		_filebuffer;
	std::string		_snappybuffer;
	size_t			_bufferpos;

public:
	NewDemoFile( );
	~NewDemoFile( );

	bool Open( const wchar_t *name );
	void Close( );
	bool Finished( );

	bool Skip( size_t bytes );
	int Percent( );

public:
	const wchar_t* GetName( );

public:
	EDemoCommands ReadMessageType( int *tick, bool *compressed );
	bool ReadMessage(IDemoMessage *message, bool compressed, int *size = nullptr, int *uncompressedSize = nullptr );
};
