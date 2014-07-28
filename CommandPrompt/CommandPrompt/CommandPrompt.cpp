// CommandPrompt.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "CommandPrompt.h"
#include <locale.h>
#include <windows.h>



int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR ERROR_CMD[]
		= _T( "'%s'은(는) 실행할 수 있는 프로그램이 아닙니다.\n" );

	TCHAR curDir[MAX_PATH] = { 0 , };

	_tsetlocale( LC_ALL , _T( "Korean" ) );

	SortOfCommand isExit;
	while( 1 )
	{
		isExit = CmdProcessing();
		switch( isExit )
		{
			case COMMAND_EXIT:
				_fputts( _T( "명령어 처리를 종료합니다.\n" ) , stdout );
				break;
			case COMMAND_PWD:
				GetCurrentDirectory( MAX_PATH, curDir);
				_tprintf( _T( "%s\n" ) , curDir );
				break;
			default:
				break;
		}
	}
	return 0;
}

