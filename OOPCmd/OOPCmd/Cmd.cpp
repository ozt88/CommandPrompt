#include "stdafx.h"
#include "Cmd.h"

Cmd* Cmd::m_Instance = nullptr;

Cmd::Cmd()
	:m_InputString() , m_CmdString() , m_IsRunning( true ) ,
	m_ErrorMsg( _T( "%s is not able to run.\n" ) ) ,
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
		for( int i = 1; i < argc; ++i )
		{
			m_StringStream << argv[i];
			m_StringStream << _T( " " );
		}
		std::wstring buffer;
		m_StringStream >> m_CmdString;
		m_StringStream >> m_LastString;
		while( m_StringStream >> buffer )
		{
			m_LastString = m_LastString + _T( " " ) + buffer;
		}
		m_InputString = m_CmdString + _T( " " ) + m_LastString;
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
	std::wcout << _T( "Best Command Prompt>>" );
	getline( std::wcin , m_InputString );
	m_StringStream.str( m_InputString );
	m_StringStream >> m_CmdString;
	std::transform( m_CmdString.begin() , m_CmdString.end() , m_CmdString.begin() , tolower );
	std::wstring buffer;
	m_StringStream >> m_LastString;
	while( m_StringStream >> buffer )
	{
		m_LastString = m_LastString + _T( " " ) + buffer;
	}
}

void Cmd::CmdProc()
{
	CmdStatus cmd = ReadCommand();
	TCHAR secondCmd[MAX_PATH] = { 0 , };

	switch( cmd )
	{
		case CMD_EXIT:
			m_IsRunning = FALSE;
			break;
		case CMD_PWD:
			GetCurrentDirectory( MAX_PATH , secondCmd );
			std::wcout << secondCmd << std::endl;
			break;
		case CMD_ECHO:
			std::wcout << _T( "echo:" ) << m_LastString << std::endl;
			break;
		case CMD_START:
			StartNewCmd( CreateNextCommand( m_CmdName ) );
			break;
		case CMD_ETC:
			StartNewCmd( CreateNextCommand( m_CmdString ) );
			std::wcout << m_InputString << std::endl;
			break;
		case CMD_LIST:
			ListProcessInfo();
			break;
		case CMD_KILL:
			KillProcess();
			break;
		case CMD_DIR:
			ShowDir();
			break;
		case CMD_MKD:
			MakeDir();
			break;
		case CMD_RMD:
			m_StringStream.clear();
			m_StringStream.str( m_LastString );
			m_StringStream >> secondCmd;
			RemoveDir( secondCmd );
			break;
		case CMD_DEL:
			DeleteFile();
			break;
		case CMD_REN:
			RenameFile();
			break;
		case CMD_SORT:
			Sort();
			break;
		case CMD_TYPE:
			Type();
			break;
		case CMD_LASTCMD:
			LastCmdProc();
			return;
		case CMD_HISTORY:
			History();
			break;
		default:
			return;
	}
	m_CmdList.push_back( m_InputString );
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
	if( !m_CmdString.compare( _T( "exit" ) ) )
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
	else if( !m_CmdString.compare( _T( "dir" ) ) )
	{
		result = CMD_DIR;
	}
	else if( !m_CmdString.compare( _T( "mkdir" ) ) )
	{
		result = CMD_MKD;
	}
	else if( !m_CmdString.compare( _T( "rmdir" ) ) )
	{
		result = CMD_RMD;
	}
	else if( !m_CmdString.compare( _T( "del" ) ) )
	{
		result = CMD_DEL;
	}
	else if( !m_CmdString.compare( _T( "ren" ) ) )
	{
		result = CMD_REN;
	}
	else if( !m_CmdString.compare( _T( "sort" ) ) )
	{
		result = CMD_SORT;
	}
	else if( !m_CmdString.compare( _T( "type" ) ) )
	{
		result = CMD_TYPE;
	}
	else if( !m_CmdString.compare( _T( "history" ) ) )
	{
		result = CMD_HISTORY;
	}
	else if( m_CmdString.front() == '!' )
	{
		result = CMD_LASTCMD;
	}
	else
	{
		result = CMD_ETC;
	}

	return result;
}

void Cmd::StartNewCmd( std::wstring& command )
{
	STARTUPINFO si = { 0 , };
	PROCESS_INFORMATION pi;
	si.cb = sizeof( si );
	CreateProcess( NULL , ( TCHAR* )command.c_str() , NULL , NULL ,
				   FALSE , CREATE_NEW_CONSOLE , NULL , NULL , &si , &pi );
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}


std::wstring Cmd::CreateNextCommand( std::wstring& beginCommand )
{
	std::wstring nextCommand = beginCommand + _T( " " ) + m_LastString;
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
	PROCESSENTRY32 pe32 = GetProcessEntryFirst32( hProcessSnap );
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
	TerminateProcess( hProc , 0 );
	_tprintf_s( _T( "%s is Terminated.\n" ) , procName.c_str() );
	CloseHandle( hProcessSnap );
	return true;
}

PROCESSENTRY32 Cmd::GetProcessEntryFirst32( HANDLE& hSnap )
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


bool Cmd::ShowDir()
{
	TCHAR dirPath[MAX_PATH];
	GetCurrentDirectory( MAX_PATH , dirPath );
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	_tcsncat_s( dirPath , _T( "\\*" ) , 3 );
	hFind = FindFirstFile( dirPath , &FindFileData );
	if( hFind == INVALID_HANDLE_VALUE )
	{
		_tprintf_s( _T( "Invalid File Handle\n" ) );
		return FALSE;
	}
	else
	{
		_tprintf_s( _T( "\n%s \n\n" ) , dirPath );
		ShowFile( FindFileData );
		while( FindNextFile( hFind , &FindFileData ) )
		{
			ShowFile( FindFileData );
		}
		_tprintf_s( _T( "\n" ) );
	}
	FindClose( hFind );
	return TRUE;
}

void Cmd::ShowFile( WIN32_FIND_DATA findFileData )
{
	TCHAR fileDirInfo[MAX_STR_LEN] = { 0 , };
	TCHAR fileTimeInfo[MAX_STR_LEN] = { 0 , };
	SYSTEMTIME stUTC , stLocal;
	if( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	{
		_tcscpy_s( fileDirInfo , _T( "<DIR>" ) );
	}
	else
	{
		_tcscpy_s( fileDirInfo , _T( "" ) );
	}

	FileTimeToSystemTime( &findFileData.ftLastWriteTime , &stUTC );
	SystemTimeToTzSpecificLocalTime( NULL , &stUTC , &stLocal );

	_stprintf_s( fileTimeInfo , _T( "%02d/%02d/%d\t%02d:%02d" ) ,
				 stLocal.wMonth , stLocal.wDay ,
				 stLocal.wYear , stLocal.wHour ,
				 stLocal.wMinute );
	_tprintf_s( _T( "%s\t%5s\t%4ukb\t\t%s\n" ) , fileTimeInfo , fileDirInfo , findFileData.nFileSizeLow / 1000 , findFileData.cFileName );

}

void Cmd::MakeDir()
{
	TCHAR dirName[MAX_STR_LEN];
	m_StringStream.clear();
	m_StringStream.str( m_LastString );
	m_StringStream >> dirName;
	CreateDirectory( dirName , NULL );
}

bool Cmd::RemoveDir( TCHAR* deletePath )
{
	TCHAR dirName[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	_stprintf_s( dirName , _T( "%s\\*" ) , deletePath );
	hFind = FindFirstFile( dirName , &FindFileData );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		RemoveDirIter( FindFileData , deletePath );
		while( FindNextFile( hFind , &FindFileData ) )
		{
			RemoveDirIter( FindFileData , deletePath );
		}
	}
	RemoveDirectory( deletePath );
	FindClose( hFind );
	return true;
}

void Cmd::RemoveDirIter( WIN32_FIND_DATA findFileData , TCHAR* srcName )
{
	if( !_tcscmp( findFileData.cFileName , _T( "." ) ) || !_tcscmp( findFileData.cFileName , _T( ".." ) ) )
	{
		return;
	}

	TCHAR path[MAX_STR_LEN] = { 0 , };
	_stprintf_s( path , _T( "%s\\%s" ) , srcName , findFileData.cFileName );

	if( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	{
		RemoveDir( path );
	}
	else
	{
		_tremove( path );
	}
}

void Cmd::DeleteFile()
{
	TCHAR fileName[MAX_STR_LEN];
	m_StringStream.clear();
	m_StringStream.str( m_LastString );
	m_StringStream >> fileName;
	if( !_tremove( fileName ) )
	{
		_tprintf_s( _T( "%s is deleted\n" ) , fileName );
	}
	else
	{
		_tprintf_s( _T( "%s is not found\n" ) , fileName );
	}
}

void Cmd::RenameFile()
{
	TCHAR oldName[MAX_STR_LEN];
	TCHAR newName[MAX_STR_LEN];
	m_StringStream.clear();
	m_StringStream.str( m_LastString );
	m_StringStream >> oldName;
	m_StringStream >> newName;
	if( !_trename( oldName , newName ) )
	{
		_tprintf_s( _T( "%s is renamed by %s\n" ) , oldName , newName );
	}
	else
	{
		_tprintf_s( _T( "%s is not found\n" ) , oldName );
	}
}

void Cmd::Sort()
{
	STARTUPINFO si = { 0 , };
	PROCESS_INFORMATION pi;
	si.cb = sizeof( si );

	TCHAR nextCmd[MAX_STR_LEN];
	m_StringStream.clear();
	m_StringStream.str( m_LastString );
	m_StringStream >> nextCmd;

	if( !_tcscmp( nextCmd , _T( ">" ) ) )
	{
		SECURITY_ATTRIBUTES fileSec = { sizeof( SECURITY_ATTRIBUTES ) , NULL , TRUE };
		m_StringStream >> nextCmd;
		HANDLE hFile = CreateFile( nextCmd , GENERIC_WRITE , FILE_SHARE_READ ,
								   &fileSec , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
		si.hStdOutput = hFile;
		si.hStdInput = GetStdHandle( STD_INPUT_HANDLE );
		si.hStdError = GetStdHandle( STD_ERROR_HANDLE );
		si.dwFlags |= STARTF_USESTDHANDLES;

		m_IsRunning = CreateProcess( NULL , ( TCHAR* )m_CmdString.c_str() , NULL , NULL ,
									 TRUE , 0 , NULL , NULL , &si , &pi );
		WaitForSingleObject( pi.hProcess , INFINITE );
		CloseHandle( hFile );
	}
	else
	{
		m_IsRunning = CreateProcess( NULL , ( TCHAR* )m_CmdString.c_str() , NULL , NULL ,
									 FALSE , 0 , NULL , NULL , &si , &pi );
		WaitForSingleObject( pi.hProcess , INFINITE );
	}
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}

void Cmd::Type()
{
	TCHAR secondCmd[MAX_STR_LEN];
	TCHAR thirdCmd[MAX_STR_LEN];
	TCHAR forthCmd[MAX_STR_LEN];

	m_StringStream.clear();
	m_StringStream.str( m_LastString );
	m_StringStream >> secondCmd;
	m_StringStream >> thirdCmd;
	m_StringStream >> forthCmd;

	if( !_tcscmp( thirdCmd , _T( "|" ) ) )
	{
		//For Type

		HANDLE hReadPipe , hWritePipe;
		SECURITY_ATTRIBUTES pipeSA = { sizeof( SECURITY_ATTRIBUTES ) , NULL , TRUE };
		CreatePipe( &hReadPipe , &hWritePipe , &pipeSA , 0 );

		STARTUPINFO siType = { 0 , };
		PROCESS_INFORMATION piType;
		siType.cb = sizeof( siType );

		siType.hStdOutput = hWritePipe;
		siType.hStdInput = GetStdHandle( STD_INPUT_HANDLE );
		siType.hStdError = GetStdHandle( STD_ERROR_HANDLE );
		siType.dwFlags |= STARTF_USESTDHANDLES;
		m_CmdString = m_CmdString + _T(" ") + secondCmd;
		m_IsRunning = CreateProcess( NULL , (TCHAR*)m_CmdString.c_str() , NULL , NULL ,
									 TRUE , 0 , NULL , NULL , &siType , &piType );
		CloseHandle( piType.hThread );
		CloseHandle( hWritePipe );

		//For Sort

		STARTUPINFO siSort = { 0 , }; 
		PROCESS_INFORMATION piSort;
		siSort.cb = sizeof( siSort );

		siSort.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
		siSort.hStdInput = hReadPipe;
		siSort.hStdError = GetStdHandle( STD_ERROR_HANDLE );
		siSort.dwFlags |= STARTF_USESTDHANDLES;

		m_IsRunning = CreateProcess( NULL , forthCmd , NULL , NULL ,
									 TRUE , 0 , NULL , NULL , &siSort , &piSort );

		CloseHandle( piSort.hThread );
		CloseHandle( hReadPipe );

		WaitForSingleObject( piType.hProcess , INFINITE );
		WaitForSingleObject( piSort.hProcess , INFINITE );

		CloseHandle( piType.hProcess );
		CloseHandle( piSort.hProcess );
	}
	else
	{
		STARTUPINFO si = { 0 , };
		PROCESS_INFORMATION pi;
		si.cb = sizeof( si );

		m_CmdString = m_CmdString + _T( " " ) + secondCmd;
		m_IsRunning = CreateProcess( NULL , ( TCHAR* )m_CmdString.c_str() , NULL , NULL ,
									 FALSE , 0 , NULL , NULL , &si , &pi );
		WaitForSingleObject( pi.hProcess , INFINITE );
		_tprintf( _T( "\n" ) );
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
}

void Cmd::LastCmdProc()
{
	std::wstring nextCmd;

	if( m_CmdString.length() == 2 && m_CmdString.at(1) == '!')
	{
		nextCmd = m_CmdList.back();
	}
	else
	{
		m_CmdString.erase( m_CmdString.begin() );
		for( auto cmdIter : m_CmdList )
		{
			if( cmdIter.find( m_CmdString ) != std::wstring::npos )
			{
				nextCmd = cmdIter;
				break;
			}
		}
	}

	m_CmdString.clear();
	m_LastString.clear();
	m_StringStream.clear();

	m_InputString = nextCmd;
	m_StringStream.str( m_InputString );
	m_StringStream >> m_CmdString;
	std::transform( m_CmdString.begin() , m_CmdString.end() , m_CmdString.begin() , tolower );
	std::wstring buffer;
	m_StringStream >> m_LastString;

	while( m_StringStream >> buffer )
	{
		m_LastString = m_LastString + _T( " " ) + buffer;
	}
	CmdProc();
}

void Cmd::History()
{
	_fputts( _T( "==LAST COMMAND==\n" ) , stdout );
	for( auto lastCmd : m_CmdList )
	{
		std::wcout <<lastCmd << std::endl;
	}
}
