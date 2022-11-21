
// GvisR2R_Laser.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "GvisR2R_Laser.h"
#include "MainFrm.h"

#include "GvisR2R_LaserDoc.h"
#include "GvisR2R_LaserView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGvisR2R_LaserApp

BEGIN_MESSAGE_MAP(CGvisR2R_LaserApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CGvisR2R_LaserApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CGvisR2R_LaserApp ����

CGvisR2R_LaserApp::CGvisR2R_LaserApp()
{
	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("GvisR2R_Laser.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

CGvisR2R_LaserApp::~CGvisR2R_LaserApp()
{
	Sleep(500);
	wsaEndup();
	Sleep(100);
}

// ������ CGvisR2R_LaserApp ��ü�Դϴ�.

CGvisR2R_LaserApp theApp;


// CGvisR2R_LaserApp �ʱ�ȭ

BOOL CGvisR2R_LaserApp::InitInstance()
{
	if (!m_singleInstance.Create(IDR_MAINFRAME))
	{
		AfxMessageBox(_T("GvisR2R is already running!!"));
		return FALSE;
	}

	wsaStartup();

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	// ���� ���α׷��� ���� ���ø��� ����մϴ�.  ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGvisR2R_LaserDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CGvisR2R_LaserView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CGvisR2R_LaserApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CGvisR2R_LaserApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


int CGvisR2R_LaserApp::ExitInstance()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CGvisR2R_LaserApp �޽��� ó����


void CGvisR2R_LaserApp::wsaStartup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	CString IP;
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD(2, 0);

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				IP = inet_ntoa(*(struct in_addr*)* hostinfo->h_addr_list);
			}
		}
	}

	m_strHostAddress = IP;
}

void CGvisR2R_LaserApp::wsaEndup()
{
	Sleep(300);
	WSACleanup();
}

CString CGvisR2R_LaserApp::GetHostAddress()
{
	return m_strHostAddress;
}

void CGvisR2R_LaserApp::SetHostAddress(CString sAddr)
{
	m_strHostAddress = sAddr;
}

CString CGvisR2R_LaserApp::GetHostPort()
{
	return m_strHostPort;
}

void CGvisR2R_LaserApp::SetHostPort(CString sPort)
{
	m_strHostPort = sPort;
}



