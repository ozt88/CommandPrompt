// OOPCmd.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

