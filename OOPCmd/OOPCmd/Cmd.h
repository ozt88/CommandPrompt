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
	CMD_ETC,
};


class Cmd
{
public:

	static Cmd*		GetInstance();
	static void		ReleaseInstance();

	void			Init(int argc, _TCHAR* argv[]);
	void			Run();
	void			SetCommand(std::wstring cmd) { m_Command = cmd; }

private:

	Cmd();
	~Cmd();

private:

	void			CmdProc();
	void			CmdTokenize();
	CmdStatus		ReadCmd();
	void			StartCmd( std::wstring command );
	std::wstring	CreateNextCmd( std::wstring cmd );

private:

	static Cmd*					m_Instance;
	bool						m_IsRunning;
	std::wstringstream			m_StringStream;
	std::wstring				m_InputString;
	std::wstring				m_CmdString;
	std::wstring				m_LastString;
	std::wstring				m_ErrorMsg;
	std::wstring				m_Command;
};

