#include "Window.h"
#include <Shlwapi.h>
#include <CommDlg.h>

using namespace htmlayout;

Window::Window( )
	: event_handler( HANDLE_BEHAVIOR_EVENT )
	, _hwnd( nullptr )
	, _root( nullptr )
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
	Window *w = ( Window* ) GetWindowLongPtr( hWnd, GWLP_USERDATA );

	if ( msg == WM_D2_SET_PROGRESS )
	{
		w->SetProgress( ( int ) wParam );
	}

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

htmlayout::dom::root_element* Window::Root( )
{
	if ( _root == nullptr )
	{
		_root = new htmlayout::dom::root_element( _hwnd );
	}

	return _root;
}

BOOL Window::handle_event ( HELEMENT he, BEHAVIOR_EVENT_PARAMS& params )
{
	if ( params.cmd == BUTTON_CLICK || params.cmd == HYPERLINK_CLICK )
	{
		return HandleClick( he );
	}

	return FALSE;
}

BOOL Window::HandleClick( HELEMENT he )
{
	dom::element element = he;
	const wchar_t* id = element.get_attribute( "id" );

	if ( ! id || ! id[0] )
	{
		return FALSE;
	}

#define HANDLE_ID_METHOD( x, y ) \
	if ( ! wcscmp( id, x ) ) \
	{										\
		y( );				\
		return TRUE;						\
	}

	HANDLE_ID_METHOD( L"choose-a-file", OnClickedLoadDemo );


#undef HANDLE_ID_METHOD

	return FALSE;
}

HWND Window::GetHwnd( )
{
	return _hwnd;
}

void ProgressCallback( void* context, int percent )
{
	if ( context )
	{
		Window *w = ( Window* ) context;
		PostMessage( w->GetHwnd( ), WM_D2_SET_PROGRESS, percent, 0 );
	}
}

void Window::OnClickedLoadDemo( )
{
	wchar_t selectedFile[MAX_PATH];
	ZeroMemory( selectedFile, sizeof( selectedFile ) );

	OPENFILENAME ofn;
	ZeroMemory( &ofn, sizeof( ofn ) );
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFilter = L"Dota2 replays (*.dem)\0*.dem\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = L"Open replay";
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST;

	ofn.lpstrFile = selectedFile;
	ofn.nMaxFile = MAX_PATH - 1;

	if ( ! GetOpenFileName( &ofn ) )
	{
		DWORD dw = CommDlgExtendedError();
		dw = 1;
	}

	D2Close( );

	if ( ! D2Open( selectedFile ) )
	{
		MessageBox( _hwnd, L"Cannot open that file.", L"Dota 2", MB_ICONERROR|MB_OK );
	}

	if ( MessageBox( _hwnd, L"Do you want to begin parsing the file?", L"Dota 2", MB_ICONQUESTION|MB_OKCANCEL ) == IDOK )
	{
		D2SetProgressCallback( ProgressCallback, this );
		D2Parse( );
	}

	return;
}

void Window::SetProgress( int percent )
{
	dom::element progress = Root( )->find_first( "#parsing-progress" );

	wchar_t value[5] = { 0 };
	_itow_s( percent, value, 10 );

	progress.set_attribute( "value", value );
	progress.update(REDRAW_NOW);

	if ( percent == 100 )
	{
		dom::element overlay = Root( )->find_first( "form.light-box-dialog" );
		overlay.xcall( "hide" );

		D2GENERAL_INFORMATION generalInfo;
		D2GetGeneralInformation( &generalInfo );
	}
}