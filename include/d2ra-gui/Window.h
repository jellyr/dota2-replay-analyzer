#pragma once

#include "htmlayout.h"

class Window
	: public htmlayout::event_handler
	, public htmlayout::notification_handler<Window>
{
private:
	HWND _hwnd;
	wchar_t *_classname;

public:
	Window( );
	void Show( const wchar_t* url );

private:
	bool Register( );
	bool Create( );
	void Resize( );
	void Center( );

	void OnCreate( );
	void OnDestroy( );
private:
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};