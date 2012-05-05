#include "Window.h"
#include <Shlwapi.h>

Window::Window( )
	: _hwnd( nullptr )
	, htmlayout::event_handler( HANDLE_ALL )
{
	_classname = _wcsdup( L"dota2.replay.analyzer" );
}

bool Window::Register( )
{
	bool result = true;

	do
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof( WNDCLASSEX );

		wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS | CS_DBLCLKS;
		wcex.lpfnWndProc	= Window::WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= ( HINSTANCE ) GetModuleHandle( nullptr );
		wcex.hIcon			= nullptr;
		wcex.hCursor		= nullptr;
		wcex.hbrBackground	= nullptr;
		wcex.lpszMenuName	= nullptr;
		wcex.lpszClassName	= _classname;
		wcex.hIconSm		= nullptr;

		ATOM aWnd = RegisterClassEx( &wcex );
		DWORD dwError = GetLastError( );

		if ( aWnd != 0 )
		{
			break;
		}

		if ( dwError == ERROR_CLASS_ALREADY_EXISTS )
		{
			break;
		}

		result = false;
	}
	while ( false );

	return result;
}

LRESULT CALLBACK Window::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	BOOL handled = FALSE;
	LRESULT result = 0;
	result = HTMLayoutProcND( hWnd, msg, wParam, lParam, &handled );
	if ( handled )
	{
		return result;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

bool Window::Create( )
{
	_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		_classname,
		nullptr,
		WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0,
		0,
		0,
		0,
		GetDesktopWindow( ),
		nullptr,
		nullptr,
		nullptr
	);

	if ( ! IsWindow( _hwnd ) )
	{
		return false;
	}

	SetWindowLongPtr( _hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );

	return true;
}

void Window::Show( const wchar_t* url )
{
	if ( !Register( ) )
	{
		return;
	}

	if ( ! Create( ) )
	{
		return;
	}

	OnCreate( );

	wchar_t htmlPath[MAX_PATH] = { 0 };
	GetModuleFileName( GetModuleHandle( nullptr ), htmlPath, MAX_PATH );
	PathRemoveFileSpec( htmlPath );
	PathAppend( htmlPath, L"html" );
	PathAppend( htmlPath, L"index.html" );
	HTMLayoutLoadFile( _hwnd, htmlPath );
	
	Resize( );
	Center( );

	ShowWindow( _hwnd, SW_SHOW );
	AllowSetForegroundWindow( GetCurrentProcessId( ) );
	SetForegroundWindow( _hwnd );
	
	MSG msg;
	BOOL bRet;
	while ( IsWindow( _hwnd ) && ( bRet = GetMessage( &msg, nullptr, 0, 0 ) ) != 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	OnDestroy( );

	ShowWindow( _hwnd, SW_HIDE );
}

void Window::Resize( )
{
	UINT w = HTMLayoutGetMinWidth( _hwnd );
	UINT h = HTMLayoutGetMinHeight( _hwnd, w );
	
	SetWindowPos( _hwnd, nullptr, 0, 0, w, h, SWP_NOREDRAW | SWP_NOREPOSITION | SWP_NOMOVE | SWP_NOSENDCHANGING );
}

void Window::Center( )
{
	RECT rcDlg, rcArea, rcCenter;
	HWND hWndAux = GetDesktopWindow( );

	GetWindowRect( _hwnd, &rcDlg );

	MONITORINFO mi;
	mi.cbSize = sizeof( mi );

	GetMonitorInfo( MonitorFromWindow( _hwnd, MONITOR_DEFAULTTOPRIMARY ), &mi );
	rcArea = mi.rcWork;
	rcCenter = mi.rcWork;
	if ( ! hWndAux)
	{
		hWndAux = GetDesktopWindow( );
	}
	if( ! hWndAux || ! GetWindowRect( hWndAux, &rcCenter ) )
	{
		rcCenter = mi.rcWork;
	}

	int xLeft = ( rcCenter.left + rcCenter.right ) / 2 - ( rcDlg.right - rcDlg.left ) / 2;
	int yTop = ( rcCenter.top + rcCenter.bottom ) / 2 - ( rcDlg.bottom - rcDlg.top ) / 2;

	if (xLeft + ( rcDlg.right - rcDlg.left ) > rcArea.right )
		xLeft = rcArea.right - ( rcDlg.right - rcDlg.left );
	if (xLeft < rcArea.left)
		xLeft = rcArea.left;
	if ( yTop + ( rcDlg.bottom - rcDlg.top ) > rcArea.bottom )
		yTop = rcArea.bottom - ( rcDlg.bottom - rcDlg.top );
	if ( yTop < rcArea.top)
		yTop = rcArea.top;

	BOOL bRet = SetWindowPos( _hwnd, nullptr, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
}

void Window::OnCreate( )
{
	attach_event_handler( _hwnd, this );
	setup_callback( _hwnd );
}

void Window::OnDestroy( )
{
	detach_event_handler( _hwnd, this );
}
