
// ClientTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "../GvisR2R_Laser.h"
#include "../Global/GlobalDefine.h"
#include "DlgClient.h"
//#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDlgClient ��ȭ ����



CDlgClient::CDlgClient(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_CLIENT, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strPortSvr = _T("2000");
	m_strAddrSvr = _T("");
	m_strAddrCli = _T("");
	m_pClient = NULL;
	m_nServerID = ID_NONE;
}

CDlgClient::~CDlgClient()
{
	if (m_pClient)
	{
		delete m_pClient;
		m_pClient = NULL;
	}
}

void CDlgClient::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_editMsgList);
}

BEGIN_MESSAGE_MAP(CDlgClient, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CDlgClient::OnBnClickedBtnStart)
	ON_MESSAGE(WM_CLIENT_RECEIVED, wmClientReceived)
	ON_MESSAGE(WM_CLIENT_CLOSED, wmClientClosed)
	ON_BN_CLICKED(IDOK, &CDlgClient::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgClient::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDlgClient::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_SEND, &CDlgClient::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_CHECK_SR1000W, &CDlgClient::OnBnClickedCheckSr1000w)
	ON_BN_CLICKED(IDC_CHECK_MDX2500, &CDlgClient::OnBnClickedCheckMdx2500)
END_MESSAGE_MAP()


// CDlgClient �޽��� ó����

BOOL CDlgClient::OnInitDialog()
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	//SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	//SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//CClientTestApp* pApp = (CClientTestApp*)AfxGetApp();
/*	GetDlgItem(IDC_SVR_PORT)->SetWindowText(m_strPortSvr = pApp->GetHostPort());
	GetDlgItem(IDC_IPADDRESS_CLI)->SetWindowText(pApp->GetHostAddress());
	GetDlgItem(IDC_IPADDRESS_SVR)->SetWindowText(m_strAddrSvr);*/

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

//void CDlgClient::OnSysCommand(UINT nID, LPARAM lParam)
//{
//	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
//	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
//	}
//	else
//	{
//		CDialog::OnSysCommand(nID, lParam);
//	}
//}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDlgClient::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CDlgClient::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDlgClient::StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr)
{
	if (!m_pClient)
	{
		m_pClient = new CTcpIpClient(this);
		m_pClient->Init(sAddrCli, sAddrSvr, _tstoi(sPortSvr));
		m_pClient->Start();
		m_pClient->SetServer(m_nServerID);
	}
}

void CDlgClient::StopClient()
{
	if (m_pClient)
	{
		if (!m_pClient->Stop()) // Called Destroy Function.
		{
			delete m_pClient;
		}
		m_pClient = NULL;
		Sleep(30);
	}
}

void CDlgClient::OnBnClickedBtnStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CClientTestApp* pApp = (CClientTestApp*)AfxGetApp();

	CString sPortCli, sPortSvr, sAddrCli, sAddrSvr;
	GetDlgItem(IDC_IPADDRESS_CLI)->GetWindowText(sAddrCli);
	GetDlgItem(IDC_SVR_PORT)->GetWindowText(sPortSvr);
	GetDlgItem(IDC_IPADDRESS_SVR)->GetWindowText(sAddrSvr);
	//pApp->SetHostAddress(sAddrCli);
	//pApp->SetHostPort(sPortCli);
	m_strPortSvr = sPortSvr;
	m_strAddrSvr = sAddrSvr;	
	m_strAddrCli = sAddrCli;

	StartClient(sAddrCli, sAddrSvr, sPortSvr);

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
}


void CDlgClient::OnBnClickedBtnStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg = _T("Is it sure to stop connection?");
	if (AfxMessageBox(sMsg, MB_ICONQUESTION | MB_OKCANCEL, MB_DEFBUTTON2) == IDOK)
	{
		StopClient();
		GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);		
	}
}

LRESULT CDlgClient::wmClientReceived(WPARAM wParam, LPARAM lParam)
{
	CString sReceived = (LPCTSTR)lParam;

	CString sDisp;
	GetDlgItem(IDC_MESSAGE_LIST)->GetWindowText(sDisp);
	sDisp += sReceived + _T("\r\n");
	GetDlgItem(IDC_MESSAGE_LIST)->SetWindowText(sDisp);

	return (LRESULT)1;
}

LRESULT CDlgClient::wmClientClosed(WPARAM wParam, LPARAM lParam)
{
	if (m_pClient)
		m_pClient = NULL;

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

	return (LRESULT)1;
}


void CDlgClient::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	StopClient();

	CDialog::OnOK();
}


void CDlgClient::OnDestroy()
{
	StopClient();

	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CDlgClient::OnBnClickedBtnClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (::IsWindow(m_editMsgList.GetSafeHwnd()))
	{
		int nLen = m_editMsgList.GetWindowTextLength();
		m_editMsgList.SetSel(0, -1);
		m_editMsgList.Clear();
	}
}


void CDlgClient::OnBnClickedBtnSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sMsg;
	GetDlgItem(IDC_TXT_MESSAGE)->GetWindowText(sMsg);
	if (!sMsg.IsEmpty())
	{
		if (m_pClient)
		{
			m_pClient->WriteComm(sMsg);
		}
	}
}


void CDlgClient::OnBnClickedCheckSr1000w()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BOOL bSr1000w = ((CButton*)GetDlgItem(IDC_CHECK_SR1000W))->GetCheck();
	BOOL bMdx2500 = ((CButton*)GetDlgItem(IDC_CHECK_MDX2500))->GetCheck();
	if (bSr1000w)
	{
		((CButton*)GetDlgItem(IDC_CHECK_MDX2500))->SetCheck(FALSE);

		GetDlgItem(IDC_SVR_PORT)->SetWindowText(_T("9004"));
		GetDlgItem(IDC_IPADDRESS_CLI)->SetWindowText(_T("192.168.100.99"));
		GetDlgItem(IDC_IPADDRESS_SVR)->SetWindowText(_T("192.168.100.100"));

		m_nServerID = ID_SR1000W;
	}

	if (!bSr1000w && !bMdx2500)
		m_nServerID = ID_NONE;
}


void CDlgClient::OnBnClickedCheckMdx2500()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BOOL bSr1000w = ((CButton*)GetDlgItem(IDC_CHECK_SR1000W))->GetCheck();
	BOOL bMdx2500 = ((CButton*)GetDlgItem(IDC_CHECK_MDX2500))->GetCheck();
	if (bMdx2500)
	{
		((CButton*)GetDlgItem(IDC_CHECK_SR1000W))->SetCheck(FALSE);

		GetDlgItem(IDC_SVR_PORT)->SetWindowText(_T("50002"));
		GetDlgItem(IDC_IPADDRESS_CLI)->SetWindowText(_T("192.168.100.199"));
		GetDlgItem(IDC_IPADDRESS_SVR)->SetWindowText(_T("192.168.100.200"));

		m_nServerID = ID_MDX2500;
	}

	if (!bSr1000w && !bMdx2500)
		m_nServerID = ID_NONE;
}
