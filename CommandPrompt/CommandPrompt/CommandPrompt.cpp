// CommandPrompt.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include <locale.h>
#include <windows.h>

#define STR_LEN 256
#define CMD_TOKE_NUM 100

int CmdProcessing( int tokenNum );
TCHAR* StrLower( TCHAR* pStr );
int CmdReadTokenize( void );

TCHAR ERROR_CMD[]
= _T( "'%s'은(는) 실행할 수 있는 프로그램이 아닙니다.\n" );

TCHAR curDir[MAX_PATH] = { 0 , };

TCHAR inputString[STR_LEN] = { 0 , };

TCHAR cmdString[STR_LEN] = { 0 , };

TCHAR cmdTokenList[CMD_TOKE_NUM][STR_LEN] = { 0 , };

TCHAR seps[] = _T( " ,\t\n" );

int _tmain( int argc , _TCHAR* argv[] )
{
	
	_tsetlocale( LC_ALL , _T( "Korean" ) );
	int tokenNum;

	if( argc > 1 )
	{
		tokenNum = argc - 1;
		for( int i = 0; i < tokenNum; i++ )
		{
			_stprintf_s( inputString , _T( "%s %s" ) , inputString , argv[i + 1] );
			_tcscpy_s( cmdTokenList[i] , StrLower( argv[i + 1] ) );
		}
		CmdProcessing( tokenNum );
	}

	DWORD isExit;
	while( 1 )
	{
		tokenNum = CmdReadTokenize();
		isExit = CmdProcessing( tokenNum );
		if( isExit )
		{
			_fputts( _T( "명령어 처리를 종료합니다.\n" ) , stdout );
			return 0;
		}
	}
	return 0;
}

int CmdReadTokenize( void )
{
	_fputts( _T( "Best Command Prompt >>" ) , stdout );
	_getts_s( inputString );
	_tcscpy_s( cmdString , inputString );

	TCHAR* buffer;
	TCHAR* token = _tcstok_s( cmdString , seps , &buffer );
	int tokenNum = 0;
	while( token != NULL )
	{
		_tcscpy_s( cmdTokenList[tokenNum++] , StrLower( token ) );
		token = _tcstok_s( NULL , seps , &buffer );
	}

	return tokenNum;
}

int CmdProcessing( int tokenNum )
{

	if( !_tcscmp( cmdTokenList[0] , _T( "exit" ) ) )
	{
		return TRUE;
	}
	else if( !_tcscmp( cmdTokenList[0] , _T( "pwd" ) ) )
	{
		TCHAR curDir[MAX_PATH] = { 0 , };
		GetCurrentDirectory( MAX_PATH , curDir );
		_tprintf( _T( "%s\n" ) , curDir );
		return false;
	}
	else if( !_tcscmp( cmdTokenList[0] , _T( "start" ) ) )
	{
		STARTUPINFO si1 = { 0 , };
		PROCESS_INFORMATION pi1;
		si1.cb = sizeof( si1 );

		TCHAR* buffer = nullptr;
		TCHAR command[] = _T( "CommandPrompt.exe" );
		TCHAR option[MAX_PATH] = { 0 , };
		TCHAR commandWithOption[MAX_PATH] = { 0 , };
		if( tokenNum > 1 )
		{
			_tcstok_s( inputString , seps , &buffer );
			_tcscpy_s( option , buffer );
		}
		_stprintf_s( commandWithOption , _T( "%s %s" ) , command , option );

		CreateProcess( NULL , commandWithOption , NULL , NULL ,
					   TRUE , CREATE_NEW_CONSOLE , NULL , NULL , &si1 , &pi1 );
		CloseHandle( pi1.hProcess );
		CloseHandle( pi1.hThread );

		return false;
	}
	else if( !_tcscmp( cmdTokenList[0] , _T( "echo" ) ) )
	{
		TCHAR* buffer = nullptr;
		_tcstok_s( inputString , seps , &buffer );
		_tprintf_s( _T( "%s\n" ) , buffer );
		return false;
	}
	else
	{
		STARTUPINFO si2 = { 0 , };
		PROCESS_INFORMATION pi2;
		si2.cb = sizeof( si2 );
		BOOL state = CreateProcess(
			NULL , cmdTokenList[0] , NULL , NULL ,
			TRUE , CREATE_NEW_CONSOLE , NULL , NULL , &si2 , &pi2 );
		CloseHandle( pi2.hProcess );
		CloseHandle( pi2.hThread );

		if( !state )
		{
			_tprintf_s( _T( "%s\n" ) , inputString );
		}
	}

	return false;
}


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
