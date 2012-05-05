#include "htmlayout.h"
#include "htmlayout_behavior.hpp"

namespace htmlayout
{
	struct drag_window : public behavior
	{
		drag_window( )
			: behavior( HANDLE_MOUSE, "drag-window" )
		{
		}

		virtual BOOL handle_mouse( HELEMENT he, MOUSE_PARAMS& params )
		{
			if ( params.cmd == MOUSE_DOWN )
			{
				return TRUE;
			}
			else if ( params.cmd == MOUSE_UP )
			{
				return TRUE;
			}
			else if ( params.cmd == MOUSE_MOVE )
			{
				return TRUE;
			}

			return FALSE;
		}
	};

	drag_window drag_window_instance;
};