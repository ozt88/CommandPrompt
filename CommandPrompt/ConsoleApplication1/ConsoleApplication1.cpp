// ConsoleApplication1.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstringstream wss;
	std::wstring str = _T("asd, asdasd \n asd");
	std::wstring buffer;
	wss.str( str );
	wss >> buffer;
	std::wcout << buffer << std::endl;
	std::wcout << str << std::endl;
	wss >> buffer;
	std::wcout << buffer << std::endl;
	std::wcout << str << std::endl;
	wss >> buffer;
	std::wcout << buffer << std::endl;
	std::wcout << str << std::endl;

	return 0;
}

