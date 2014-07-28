#include "stdafx.h"
#include "CommandPrompt.h"
#include <windows.h>



SortOfCommand CmdProcessing( void )
{

	TCHAR ERROR_CMD[]
		= _T( "'%s'은(는) 실행할 수 있는 프로그램이 아닙니다.\n" );
	TCHAR inputString[STR_LEN] = { 0 , };
	TCHAR cmdString[STR_LEN] = { 0 , };

	TCHAR cmdTokenList[CMD_TOKE_NUM][STR_LEN] = { 0 , };
	TCHAR seps[] = _T( ",\t\n" );

	_fputts( _T( "Best Command Prompt >>" ) , stdout );
	_getts_s( inputString );
	
	_tcscpy_s( cmdString , inputString );
	TCHAR* buffer;
	TCHAR* token = _tcstok_s( cmdString , seps , &buffer);
	int tokenNum = 0;
	while( token != NULL )
	{
		_tcscpy_s( cmdTokenList[tokenNum++] , StrLower( token ) );
		token = _tcstok_s( NULL , seps , &buffer);
	}

	if( !_tcscmp( cmdTokenList[0] , _T( "exit" ) ) )
	{
		return COMMAND_EXIT;
	}
	else if( !_tcscmp( cmdTokenList[0] , _T( "pwd" ) ) )
	{
		return COMMAND_PWD;
	}
	else
	{
		STARTUPINFO si = { 0 , };
		PROCESS_INFORMATION pi;
		si.cb = sizeof( si );
		BOOL state = CreateProcess(
			NULL , cmdTokenList[0] , NULL , NULL ,
			TRUE , CREATE_NEW_CONSOLE , NULL , NULL , &si , &pi );

		if( !state )
		{
			_tprintf_s(_T("%s\n"), inputString);
		}
	}

	return COMMAND_NONE;
}
