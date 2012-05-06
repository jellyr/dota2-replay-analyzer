#pragma once

#include "htmlayout.h"

#define WM_D2_SET_PROGRESS		( WM_APP + 0x100 )

#include "../d2ra/exports.h"
#pragma comment( lib, "d2ra.lib" )

class Window
	: public htmlayout::event_handler
	, public htmlayout::notification_handler<Window>
{
private:
	HWND _hwnd;
	wchar_t *_classname;
	htmlayout::dom::root_element *_root;
	ULONGLONG _startedparsing;

public:
	Window( );
	~Window( );
	void Show( const wchar_t* url );

	HWND GetHwnd( );

private:
	bool Register( );
	bool Create( );
	void Resize( );
	void Center( );

	void OnCreate( );
	void OnDestroy( );

	htmlayout::dom::root_element* Root( );

private:
	void SetProgress( int percent );

private:
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	// this NEEDS to be redone
	virtual BOOL handle_event (HELEMENT he, BEHAVIOR_EVENT_PARAMS& params );

	BOOL HandleClick( HELEMENT );


private:
	void OnClickedLoadDemo( );
	void OnFinishedParsing( );
};
