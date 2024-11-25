
// GvisR2R_Laser.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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

// 디버그창에 메모리 릭위치 출력 정의  
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// CGvisR2R_LaserApp

BEGIN_MESSAGE_MAP(CGvisR2R_LaserApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CGvisR2R_LaserApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CGvisR2R_LaserApp 생성

CGvisR2R_LaserApp::CGvisR2R_LaserApp()
{
	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("GvisR2R_Laser.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

CGvisR2R_LaserApp::~CGvisR2R_LaserApp()
{
	Sleep(500);
	wsaEndup();
	Sleep(100);
}

// 유일한 CGvisR2R_LaserApp 개체입니다.

CGvisR2R_LaserApp theApp;


// CGvisR2R_LaserApp 초기화

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

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGvisR2R_LaserDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CGvisR2R_LaserView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CGvisR2R_LaserApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CGvisR2R_LaserApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


int CGvisR2R_LaserApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CGvisR2R_LaserApp 메시지 처리기


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



