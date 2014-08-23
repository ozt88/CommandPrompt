// ListProcessInfo.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hProcessSnap =
		CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS , 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		std::wcout << _T( "CreateToolhelp32Snapshot Error!" ) << std::endl;
		return -1;
	}

	//프로세스 정보를 얻기위한 구조체 변수
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	if( !Process32First( hProcessSnap , &pe32 ) )
	{
		std::wcout << _T( "Process32First Error!" ) << std::endl;
		CloseHandle( hProcessSnap );
		return -1;
	}

	do { 
		_tprintf_s( _T( "%-25s\t%5d\n" ) , pe32.szExeFile , pe32.th32ProcessID );
	} while( Process32Next(hProcessSnap, &pe32));

	CloseHandle( hProcessSnap );
	return 0;
}

