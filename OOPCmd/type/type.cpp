// type.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#define BUFFER_SIZE 1024

void TYPE( TCHAR* fileName )
{
	TCHAR StringBuffer[BUFFER_SIZE];
	FILE* filePtr= _tfopen( fileName , _T( "rt" ) );
	while( _fgetts( StringBuffer , BUFFER_SIZE , filePtr ) )
	{
		_fputts(StringBuffer , stdout);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if( argc < 2 )
	{
		return -1;
	}

	TYPE( argv[1] );
	return 0;
}

