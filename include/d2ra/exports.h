#pragma once

#if ! defined( D2RA_API )
# if defined( D2RA_DLL )
#  define D2RA_API __declspec( dllexport )
# else
#  define D2RA_API __declspec( dllimport )
# endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

// A callback for when the parsing takes place
typedef void ( *D2RA_PROGRESS_CALLBACK )( void*, int );

// Open a file name
bool D2RA_API D2Open( const wchar_t* fileName );

// Start parsing the file
void D2RA_API D2Parse( );

// Close the handle; this also stops the parsing if it's already started
void D2RA_API D2Close( );

// Set a custom callback for file parsing progress
void D2RA_API D2SetProgressCallback( D2RA_PROGRESS_CALLBACK callback, void* context );

#ifdef __cplusplus
}
#endif
