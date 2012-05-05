#pragma once

#include <string>
#include <assert.h>
#include "demo.pb.h"
#include "netmessages.pb.h"

typedef __int32			 	int32;
typedef unsigned __int32	uint32;
typedef unsigned __int64	uint64;
typedef unsigned long		CRC32_t;

#define PROTODEMO_HEADER_ID				"PBUFDEM"
#define DEMOFILE_FULLPACKETS_VERSION	2

class DemoHeader
{
public:
	char filestamp[ 8 ];
	int32 fileinfo_offset;
};

#define MAX_PLAYER_NAME_LENGTH	32
#define MAX_CUSTOM_FILES		4	// max 4 files
#define SIGNED_GUID_LEN			32	// Hashed CD Key (32 hex alphabetic chars + 0 terminator )

class PlayerInfo
{
public:
	// network xuid
	uint64			xuid;
	// scoreboard information
	char			name[MAX_PLAYER_NAME_LENGTH];
	// local server user ID, unique while server is running
	int				userID;
	// global unique player identifer
	char			guid[SIGNED_GUID_LEN + 1];
	// friends identification number
	uint32			friendsID;
	// friends name
	char			friendsName[MAX_PLAYER_NAME_LENGTH];
	// true, if player is a bot controlled by game.dll
	bool			fakeplayer;
	// true if player is the HLTV proxy
	bool			ishltv;
#if defined( REPLAY_ENABLED )
	// true if player is the Replay proxy
	bool			isreplay;
#endif
	// custom files CRC for this player
	CRC32_t			customFiles[MAX_CUSTOM_FILES];
	// this counter increases each time the server downloaded a new file
	unsigned char	filesDownloaded;
};


class IDemoMessage
{
public:
	virtual	~IDemoMessage( )
	{}

	virtual EDemoCommands	GetType( ) const = 0;
	virtual size_t			GetSize( ) const = 0;
	virtual ::google::protobuf::Message& GetProtoMsg( ) = 0;
};

template<EDemoCommands msgType, typename PROTOBUF_OBJECT_TYPE>
class CDemoMessagePB : public IDemoMessage, public PROTOBUF_OBJECT_TYPE
{
public:
	CDemoMessagePB( )
	{}
	virtual ~CDemoMessagePB( )
	{}

	virtual EDemoCommands GetType( ) const 				{ return msgType; }
	virtual size_t GetSize( ) const  					{ return sizeof( *this ); }
	virtual ::google::protobuf::Message& GetProtoMsg( ) { return *this; }
};

typedef CDemoMessagePB<DEM_FileHeader, CDemoFileHeader>						CDemoFileHeader_t;
typedef CDemoMessagePB<DEM_FileInfo, CDemoFileInfo>							CDemoFileInfo_t;
typedef CDemoMessagePB<DEM_Stop, CDemoStop>									CDemoStop_t;
typedef CDemoMessagePB<DEM_SyncTick, CDemoSyncTick>							CDemoSyncTick_t;
typedef CDemoMessagePB<DEM_SendTables, CDemoSendTables>						CDemoSendTables_t;
typedef CDemoMessagePB<DEM_ClassInfo, CDemoClassInfo>						CDemoClassInfo_t;
typedef CDemoMessagePB<DEM_StringTables, CDemoStringTables>					CDemoStringTables_t;
typedef CDemoMessagePB<DEM_ConsoleCmd, CDemoConsoleCmd>						CDemoConsoleCmd_t;
typedef CDemoMessagePB<DEM_CustomData, CDemoCustomData>						CDemoCustomData_t;
typedef CDemoMessagePB<DEM_CustomDataCallbacks, CDemoCustomDataCallbacks>	CDemoCustomDataCallbacks_t;
typedef CDemoMessagePB<DEM_UserCmd, CDemoUserCmd>							CDemoUserCmd_t;
typedef CDemoMessagePB<DEM_FullPacket, CDemoFullPacket>						CDemoFullPacket_t;
typedef CDemoMessagePB<DEM_Packet, CDemoPacket>								CDemoPacket_t;

#define CDemoFileHeader_t_type												"CDemoMessagePB<1, CDemoFileHeader>"

uint32 ReadInt( const std::string& buf, size_t& index );
