
// GvisR2R_Laser.h : GvisR2R_Laser ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "SingleInstance.h"

// CGvisR2R_LaserApp:
// �� Ŭ������ ������ ���ؼ��� GvisR2R_Laser.cpp�� �����Ͻʽÿ�.
//

class CGvisR2R_LaserApp : public CWinApp
{
	CString m_strHostAddress, m_strHostPort;

	void wsaStartup();
	void wsaEndup();

public:
	CGvisR2R_LaserApp();
	~CGvisR2R_LaserApp();

	CSingleInstance m_singleInstance;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();

	CString GetHostAddress();
	CString GetHostPort();
	void SetHostAddress(CString sAddr);
	void SetHostPort(CString sPort);


	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CGvisR2R_LaserApp theApp;
