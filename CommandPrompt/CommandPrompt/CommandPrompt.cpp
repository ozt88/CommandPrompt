// CommandPrompt.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"
#include "CommandPrompt.h"
#include <locale.h>
#include <windows.h>



int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR ERROR_CMD[]
		= _T( "'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�.\n" );

	TCHAR curDir[MAX_PATH] = { 0 , };

	_tsetlocale( LC_ALL , _T( "Korean" ) );

	SortOfCommand command;
	while( 1 )
	{
		command = CmdProcessing();
		switch( command )
		{
			case COMMAND_EXIT:
				_fputts( _T( "��ɾ� ó���� �����մϴ�.\n" ) , stdout );
				return 0;
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

