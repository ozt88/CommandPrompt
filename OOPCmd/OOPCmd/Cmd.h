#pragma once
#define CMD_TOKEN_NUM 100
#define MAX_STR_LEN 256

enum CmdStatus
{
	CMD_NONE,
	CMD_EXIT,
	CMD_PWD,
	CMD_ECHO,
	CMD_START,
	CMD_LIST,
	CMD_KILL,
	CMD_ETC,
};


class Cmd
{
public:

	static Cmd*		GetInstance();
	static void		ReleaseInstance();

	void			Init(int argc, _TCHAR* argv[]);
	void			Run();
	void			SetCommand(std::wstring name) { m_CmdName = name; }

private:

	Cmd();
	~Cmd();

private:

	void			CmdProc();
	void			GetCommand();
	CmdStatus		ReadCommand();

	void			ClearStream();
	void			StartNewCmd( std::wstring& command );
	std::wstring	CreateNextCommand( std::wstring& beginCommand );

	bool			ListProcessInfo();
	bool			KillProcess();

	HANDLE			GetProcessSnapShot();
	PROCESSENTRY32	GetProcessEntryFirst32( HANDLE& hSnap );
	DWORD			GetProcessID( std::wstring& procName , HANDLE& hSnap ,PROCESSENTRY32& pe32);

private:

	static Cmd*					m_Instance;
	bool						m_IsRunning;
	std::wstringstream			m_StringStream;
	std::wstring				m_InputString;
	std::wstring				m_CmdString;
	std::wstring				m_LastString;
	std::wstring				m_ErrorMsg;
	std::wstring				m_CmdName;
};

