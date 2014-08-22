#include "stdafx.h"
#include "Cmd.h"

Cmd* Cmd::m_Instance = nullptr;

Cmd::Cmd()
	:m_InputString() , m_CmdString() , m_IsRunning( true ),
	m_ErrorMsg( _T( "%s is not able to run.\n" )) ,
	m_Command( _T( "OOPCmd.exe" ) ) 
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
		CmdTokenize();
		CmdProc();
	}
}

void Cmd::CmdProc()
{
	CmdStatus cmd = ReadCmd();
	std::wstring buffer;
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
			std::wcout << m_LastString << std::endl;
			return;
		case CMD_START:
			StartCmd(CreateNextCmd(m_Command));
			return;
		case CMD_ETC:
			StartCmd(CreateNextCmd(m_CmdString));
			std::wcout << m_InputString << std::endl;
			return;
		default:
			return;
	}
}


void Cmd::CmdTokenize()
{
	m_StringStream.str( _T( "" ) );
	m_StringStream.clear();
	m_CmdString.clear();
	m_InputString.clear();
	std::wcin.clear();

	std::wcout << _T( "Best Command Prompt >>" );
	getline(std::wcin, m_InputString);
	m_StringStream.str( m_InputString );
	m_StringStream >> m_CmdString;
	std::transform( m_CmdString.begin() , m_CmdString.end() , m_CmdString.begin() , tolower );
	std::wstring option;
	while( m_StringStream >> option )
	{
		m_LastString = m_LastString + _T( " " ) + option;
	}
}


CmdStatus Cmd::ReadCmd()
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
	else
	{
		result = CMD_ETC;
	}

	return result;
}

void Cmd::StartCmd(std::wstring command)
{
	STARTUPINFO si = { 0 , };
	PROCESS_INFORMATION pi;
	si.cb = sizeof( si );
	CreateProcess( NULL , (TCHAR*)command.c_str() , NULL , NULL ,
				   TRUE , CREATE_NEW_CONSOLE , NULL , NULL , &si , &pi );
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}

std::wstring Cmd::CreateNextCmd( std::wstring cmd )
{
	std::wstring commandWithOption = cmd + _T(" ") + m_LastString;
	return commandWithOption;
}



