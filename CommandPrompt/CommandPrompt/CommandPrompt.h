
#define STR_LEN 256
#define CMD_TOKE_NUM 10


enum SortOfCommand
{
	COMMAND_NONE,
	COMMAND_EXIT,
	COMMAND_PWD,
};

SortOfCommand CmdProcessing( void );
TCHAR* StrLower( TCHAR* pStr );