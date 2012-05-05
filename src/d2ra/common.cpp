#include "common.h"

uint32 ReadInt( const std::string& buf, size_t& index )
{
	uint32 b;
	int count = 0;
	uint32 result = 0;

	do
	{
		if ( count == 5 )
		{
			// If we get here it means that the fifth bit had its
			// high bit set, which implies corrupt data.
			assert( 0 );
			return result;
		}
		else if ( index >= buf.size() )
		{
			assert( 0 );
			return result;
		}

		b = buf[ index++ ];
		result |= ( b & 0x7F ) << ( 7 * count );
		++count;
	} while ( b & 0x80 );

	return result;
}