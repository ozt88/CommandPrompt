#include "stdafx.h"
#include <windows.h>
#include "CommandPrompt.h"

TCHAR* StrLower( TCHAR* pStr )
{
	TCHAR* ret = pStr;

	while( *pStr )
	{
		if( _istupper( *pStr ) )
		{
			*pStr = _totlower( *pStr );
		}
		pStr++;
	}

	return ret;
}
