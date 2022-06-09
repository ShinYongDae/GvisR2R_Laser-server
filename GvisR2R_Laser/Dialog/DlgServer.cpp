// DlgServer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../GvisR2R_Laser.h"
#include "../Global/GlobalDefine.h"
#include "DlgServer.h"
#include "afxdialogex.h"


// CDlgServer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgServer, CDialog)

CDlgServer::CDlgServer(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_SERVER, pParent)
{
	m_strPort = _T("2000");
	m_pServer = NULL;
}

CDlgServer::~CDlgServer()
{
	if (m_pServer)
	{
		delete m_pServer;
		m_pServer = NULL;
		Sleep(30);
	}
}

void CDlgServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_editMsgList);
}


BEGIN_MESSAGE_MAP(CDlgServer, CDialog)
	ON_BN_CLICKED(IDC_BTN_START, &CDlgServer::OnBnClickedBtnStart)
	ON_MESSAGE(WM_SERVER_RECEIVED, wmAcceptReceived)
	ON_BN_CLICKED(IDOK, &CDlgServer::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_STOP, &CDlgServer::OnBnClickedBtnStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SEND, &CDlgServer::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgServer::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CDlgServer 메시지 처리기입니다.

BOOL CDlgServer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	BOOL bNameValid;
	//	CString strAboutMenu;
	//	bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	ASSERT(bNameValid);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	//// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	////  프레임워크가 이 작업을 자동으로 수행합니다.
	//SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	//SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

									// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//CServerTestApp* pApp = (CServerTestApp*)AfxGetApp();
	//GetDlgItem(IDC_SVR_PORT)->SetWindowText(pApp->GetHostPort());
	//GetDlgItem(IDC_IPADDRESS)->SetWindowText(pApp->GetHostAddress());

	SetTimer(0, 300, NULL);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDlgServer::StartServer(CString sAddr, CString sPort)
{
	if (!m_pServer)
	{
		m_pServer = new CTcpIpServer(this);
		m_pServer->Init(sAddr, _tstoi(sPort));
		m_pServer->Start();
	}
}

void CDlgServer::StopServer()
{
	if (m_pServer)
	{
		if (!m_pServer->Stop()) // Called Destroy Function.
		{
			delete m_pServer;
		}
		m_pServer = NULL;
		Sleep(30);
	}
}

void CDlgServer::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CServerTestApp* pApp = (CServerTestApp*)AfxGetApp();

	CString sPort, sAddr;
	GetDlgItem(IDC_SVR_PORT)->GetWindowText(sPort);
	GetDlgItem(IDC_IPADDRESS)->GetWindowText(sAddr);
	//pApp->SetHostAddress(sAddr);
	//pApp->SetHostPort(sPort);

	StartServer(sAddr, sPort);
}

void CDlgServer::OnBnClickedBtnStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	StopServer();
}

LRESULT CDlgServer::wmAcceptReceived(WPARAM wParam, LPARAM lParam)
{
	CString sReceived = (LPCTSTR)lParam;

	CString sDisp;
	GetDlgItem(IDC_MESSAGE_LIST)->GetWindowText(sDisp);
	sDisp += sReceived + _T("\r\n");
	GetDlgItem(IDC_MESSAGE_LIST)->SetWindowText(sDisp);

	return (LRESULT)1;
}

void CDlgServer::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	StopServer();

	CDialog::OnOK();
}


void CDlgServer::OnDestroy()
{
	StopServer();

	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



void CDlgServer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 0:
		KillTimer(0);
		DispClientStatus();
		SetTimer(0, 300, NULL);
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgServer::DispClientStatus()
{
	if (m_pServer)
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(m_pServer->IsConnected(0));
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(m_pServer->IsConnected(1));
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(m_pServer->IsConnected(2));
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(m_pServer->IsConnected(3));
		((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(m_pServer->IsConnected(4));
		((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(m_pServer->IsConnected(5));
		((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(m_pServer->IsConnected(6));
		((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(m_pServer->IsConnected(7));
	}
}


void CDlgServer::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다. 
	CString sClientID, sMsg;
	GetDlgItem(IDC_CLIENT_ID)->GetWindowText(sClientID);
	GetDlgItem(IDC_TXT_MESSAGE)->GetWindowText(sMsg);

	if (sClientID.IsEmpty() || sMsg.IsEmpty())
		return;

	if (m_pServer)
	{
		int nClientID = _tstoi(sClientID);
		if (nClientID < 1 || nClientID > 8)
			return;

		m_pServer->WriteComm(nClientID - 1, sMsg);
	}
}


void CDlgServer::OnBnClickedBtnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (::IsWindow(m_editMsgList.GetSafeHwnd()))
	{
		int nLen = m_editMsgList.GetWindowTextLength();
		m_editMsgList.SetSel(0, -1);
		m_editMsgList.Clear();
	}
}
