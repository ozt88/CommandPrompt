// OOPCmd.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Cmd.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Cmd* cmdInstance = Cmd::GetInstance();
	cmdInstance->Init( argc , argv );
	cmdInstance->Run();
	cmdInstance->ReleaseInstance();
	return 0;
}

