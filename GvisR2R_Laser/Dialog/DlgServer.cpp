// DlgServer.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "../GvisR2R_Laser.h"
#include "../Global/GlobalDefine.h"
#include "DlgServer.h"
#include "afxdialogex.h"


// CDlgServer ��ȭ �����Դϴ�.

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


// CDlgServer �޽��� ó�����Դϴ�.

BOOL CDlgServer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	//// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	////  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	//SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	//SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

									// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//CServerTestApp* pApp = (CServerTestApp*)AfxGetApp();
	//GetDlgItem(IDC_SVR_PORT)->SetWindowText(pApp->GetHostPort());
	//GetDlgItem(IDC_IPADDRESS)->SetWindowText(pApp->GetHostAddress());

	SetTimer(0, 300, NULL);
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	StopServer();

	CDialog::OnOK();
}


void CDlgServer::OnDestroy()
{
	StopServer();

	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}



void CDlgServer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�. 
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (::IsWindow(m_editMsgList.GetSafeHwnd()))
	{
		int nLen = m_editMsgList.GetWindowTextLength();
		m_editMsgList.SetSel(0, -1);
		m_editMsgList.Clear();
	}
}
