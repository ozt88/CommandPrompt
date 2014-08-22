#include "stdafx.h"
#include "Cmd.h"

Cmd* Cmd::m_Instance = nullptr;

Cmd::Cmd()
	:m_CmdString() , m_CmdTokenList() , m_ErrorMsg() ,
	 m_InputString( { 0 , } ) , m_Seps( { 0 , } ) , m_Command( { 0 , } ),
	 m_TokenNum( 0 ) , m_IsRunning( true ) 
{
	m_ErrorMsg = _T( "%s is not able to run.\n" );
}


Cmd::~Cmd()
{
	m_CmdTokenList.clear();
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
	SetSeps( _T( " ,\t\n" ) );
	SetCommand( _T( "OOPCmd.exe" ) );
	m_CmdTokenList.resize( CMD_TOKEN_NUM );

	if( argc > 1 )
	{
		m_TokenNum = argc - 1;
		for( DWORD i = 0; i < m_TokenNum; ++i )
		{
			_stprintf_s( m_InputString , _T( "%s %s" ) ,
						 m_InputString , argv[i + 1] );
			m_CmdTokenList[i] = argv[i + 1];
			ToLowerStr( m_CmdTokenList[i] );
		}
	}
	CmdProc();
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
	TCHAR* buffer = nullptr;
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
			_tcstok_s( m_InputString , m_Seps , &buffer );
			std::wcout << buffer << std::endl;
			return;
		case CMD_START:
			StartCmd(CreateNextCmd(m_Command));
			return;

		default:
			StartCmd(CreateNextCmd(m_CmdTokenList[0].c_str()));
			return;
	}
}


void Cmd::CmdTokenize()
{
	m_CmdTokenList.clear();
	m_CmdTokenList.resize(CMD_TOKEN_NUM);
	m_InputString.clear();
	m_TokenNum = 0;

	std::wcout << _T( "Best Command Prompt >>" );
	std::wcin >> m_InputString;
	m_CmdString = m_InputString;

	TCHAR* token , *buffer;
	token = _tcstok_s( (TCHAR*)m_CmdString.c_str() , m_Seps.c_str() , &buffer );
	while( token != NULL )
	{
		m_CmdTokenList[m_TokenNum++] = token;
		ToLowerStr( m_CmdTokenList[m_TokenNum++] );
		token = _tcstok_s( NULL , m_Seps.c_str() , &buffer );
	}
}


CmdStatus Cmd::ReadCmd()
{
	CmdStatus result = CMD_NONE;
	if( !_tcscmp(m_CmdTokenList[0].c_str(), _T( "exit" )) )
	{
		result = CMD_EXIT;
	}
	else if( !_tcscmp( m_CmdTokenList[0].c_str(), _T( "pwd" )) )
	{
		result = CMD_PWD;
	}
	else if( !_tcscmp( m_CmdTokenList[0].c_str() , _T( "start" ) ) )
	{
		result = CMD_START;
	}
	else if( !_tcscmp( m_CmdTokenList[0].c_str() , _T( "echo" ) ) )
	{
		result = CMD_ECHO;
	}
	else
	{
		result = CMD_ETC;
	}

	return result;
}


void Cmd::ToLowerStr( std::wstring str)
{
	for( size_t i = 0; i < str.size(); ++i )
	{
		if( _istupper( str[i] ) )
		{
			str[i] = _totlower( str[i] );
		}
	}
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

std::wstring Cmd::CreateNextCmd( const TCHAR* cmd )
{
	TCHAR* buffer = nullptr;
	TCHAR option[MAX_PATH] = { 0 , };
	std::wstring commandWithOption;
	if( m_TokenNum > 1 )
	{
		m_InputString.tokenize
		_tcscpy_s( option , buffer );
	}
	commandWithOption = cmd + _T(" ") + option;

	return commandWithOption;
}



