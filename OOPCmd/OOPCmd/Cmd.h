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
	
	void			SetSeps(std::wstring seps) { m_Seps = seps;}
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
	void			ToLowerStr( std::wstring str );

private:

	static Cmd*					m_Instance;
	bool						m_IsRunning;
	DWORD						m_TokenNum;
	std::vector <std::wstring>	m_CmdTokenList;
	std::wstring				m_CmdString;
	std::wstring				m_ErrorMsg;
	std::wstring				m_Command;
	std::wstring				m_InputString;
	std::wstring				m_Seps;
};

