#include <Windows.h>
#include "Window.h"

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	Window w;
	w.Show( L"http://www.google.ro/" );
}