
// TranceFile001.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTranceFile001App: 
// �йش����ʵ�֣������ TranceFile001.cpp
//

class CTranceFile001App : public CWinApp
{
public:
	CTranceFile001App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTranceFile001App theApp;