#include "stdafx.h"
#include "Cmd.h"

Cmd* Cmd::m_Instance = nullptr;

Cmd::Cmd()
	:m_InputString() , m_CmdString() , m_IsRunning( true ),
	m_ErrorMsg( _T( "%s is not able to run.\n" )) ,
	m_CmdName( _T( "OOPCmd.exe" ) ) 
{
}


Cmd::~Cmd()
{
}


Cmd* Cmd::GetInstance()
{
	if( m_Instance == nullptr )
	{
		m_Instance = new Cmd();
	}
	return m_Instance;
}

void Cmd::ReleaseInstance()
{
	if( m_Instance != nullptr )
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
}

void Cmd::Init( int argc , _TCHAR* argv[] )
{
	_tsetlocale( LC_ALL , _T( "Korean" ) );
	SetCommand( _T( "OOPCmd.exe" ) );
	if( argc > 1 )
	{
		m_CmdString = argv[1];
		for( int i = 1; i < argc - 1; ++i )
		{
			m_LastString = m_LastString + _T( " " ) + argv[i + 1];
		}
		CmdProc();
	}
}

void Cmd::Run()
{
	while( m_IsRunning )
	{
		GetCommand();
		CmdProc();
	}
}

void Cmd::GetCommand()
{
	ClearStream();
	std::wcout << _T( "Best Command Prompt >>" );
	getline( std::wcin , m_InputString );
	m_StringStream.str( m_InputString );
	m_StringStream >> m_CmdString;
	std::transform( m_CmdString.begin() , m_CmdString.end() , m_CmdString.begin() , tolower );
	std::wstring option;
	m_StringStream >> m_LastString;
	while( m_StringStream >> option )
	{
		m_LastString = m_LastString + _T( " " ) + option;
	}
}

void Cmd::CmdProc()
{
	CmdStatus cmd = ReadCommand();
	TCHAR curDir[MAX_PATH] = { 0 , };

	switch( cmd )
	{
		case CMD_EXIT:
			m_IsRunning = FALSE;
			return;
		case CMD_PWD:
			GetCurrentDirectory( MAX_PATH , curDir);
			std::wcout << curDir << std::endl;
			return;
		case CMD_ECHO:
			std::wcout << _T("echo:") <<m_LastString << std::endl;
			return;
		case CMD_START:
			StartNewCmd(CreateNextCommand(m_CmdName));
			return;
		case CMD_ETC:
			StartNewCmd(CreateNextCommand(m_CmdString));
			std::wcout << m_InputString << std::endl;
			return;
		case CMD_LIST:
			ListProcessInfo();
			return;
		case CMD_KILL:
			KillProcess();
			return;
		default:
			return;
	}
}



void Cmd::ClearStream()
{
	m_StringStream.str( _T( "" ) );
	m_StringStream.clear();
	m_CmdString.clear();
	m_LastString.clear();
	m_InputString.clear();
	std::wcin.clear();
}

CmdStatus Cmd::ReadCommand()
{
	CmdStatus result = CMD_NONE;
	if( !m_CmdString.compare( _T( "exit" )) )
	{
		result = CMD_EXIT;
	}
	else if( !m_CmdString.compare( _T( "pwd" ) ) )
	{
		result = CMD_PWD;
	}
	else if( !m_CmdString.compare( _T( "start" ) ) )
	{
		result = CMD_START;
	}
	else if( !m_CmdString.compare( _T( "echo" ) ) )
	{
		result = CMD_ECHO;
	}
	else if( !m_CmdString.compare( _T( "list" ) ) )
	{
		result = CMD_LIST;
	}
	else if( !m_CmdString.compare( _T( "kill" ) ) )
	{
		result = CMD_KILL;
	}
	else
	{
		result = CMD_ETC;
	}

	return result;
}

void Cmd::StartNewCmd(std::wstring& command)
{
	STARTUPINFO si = { 0 , };
	PROCESS_INFORMATION pi;
	si.cb = sizeof( si );
	CreateProcess( NULL , (TCHAR*)command.c_str() , NULL , NULL ,
				   TRUE , CREATE_NEW_CONSOLE , NULL , NULL , &si , &pi );
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}


std::wstring Cmd::CreateNextCommand( std::wstring& beginCommand )
{
	std::wstring nextCommand = beginCommand + _T(" ") + m_LastString;
	return nextCommand;
}


bool Cmd::ListProcessInfo()
{
	HANDLE hProcessSnap = GetProcessSnapShot();
	PROCESSENTRY32 pe32 = GetProcessEntryFirst32( hProcessSnap );

	do
	{
		_tprintf_s( _T( "%-25s\t%5d\n" ) , pe32.szExeFile , pe32.th32ProcessID );
	} while( Process32Next( hProcessSnap , &pe32 ) );
	std::wcout << std::endl;
	CloseHandle( hProcessSnap );
	return true;
}

bool Cmd::KillProcess()
{
	HANDLE hProcessSnap = GetProcessSnapShot();
	PROCESSENTRY32 pe32 = GetProcessEntryFirst32(hProcessSnap); 
	std::wstring procName;
	m_StringStream.clear();
	m_StringStream.str( m_LastString );
	m_StringStream >> procName;
	DWORD ProcID = GetProcessID( procName , hProcessSnap , pe32 );
	if( ProcID == NULL )
	{
		_tprintf_s( _T( "%s is not Exist.\n" ) , procName.c_str() );
		CloseHandle( hProcessSnap );
		return false;
	}
	HANDLE hProc = OpenProcess( PROCESS_ALL_ACCESS , FALSE , ProcID );
	TerminateProcess( hProc , 0);
	_tprintf_s( _T( "%s is Terminated.\n" ) , procName.c_str() );
	CloseHandle( hProcessSnap );
	return true;
}

PROCESSENTRY32 Cmd::GetProcessEntryFirst32(HANDLE& hSnap)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	if( !Process32First( hSnap , &pe32 ) )
	{
		std::wcout << _T( "Process32First Error!" ) << std::endl;
		CloseHandle( hSnap );
	}

	return pe32;
}

HANDLE Cmd::GetProcessSnapShot()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS , 0 );
	_ASSERT( hProcessSnap != INVALID_HANDLE_VALUE );

	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		std::wcout << _T( "CreateToolhelp32Snapshot Error!" ) << std::endl;
		return NULL;
	}
	return hProcessSnap;
}

DWORD Cmd::GetProcessID( std::wstring& procName , HANDLE& hSnap , PROCESSENTRY32& pe32 )
{
	do
	{
		if( !procName.compare( pe32.szExeFile ) )
		{
			return pe32.th32ProcessID;
		}
	} while( Process32Next( hSnap , &pe32 ) );


	return NULL;
}



