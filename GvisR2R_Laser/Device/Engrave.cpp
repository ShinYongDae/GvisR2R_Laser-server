#include "stdafx.h"
#include "Engrave.h"

#include "EngraveDef.h"
#include "../Global/GlobalDefine.h"
#include "../GvisR2R_LaserDoc.h"
#include "../GvisR2R_LaserView.h"

extern CGvisR2R_LaserDoc* pDoc;
extern CGvisR2R_LaserView* pView;

BEGIN_MESSAGE_MAP(CEngrave, CWnd)
	//{{AFX_MSG_MAP(CEngrave)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_UPDATE_CONNECTION, OnUpdateConnection)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SERVER_RECEIVED, wmAcceptReceived)
	ON_MESSAGE(WM_SERVER_CLOSED, wmServerClosed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CEngrave::CEngrave(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent /*=NULL*/)
{
	m_hParentWnd = pParent->GetSafeHwnd();

	m_pServer = NULL;
	m_nServerID = ID_ENGRAVE;
	m_bWaitForResponse = FALSE;
	m_strResponse = _T("");
	m_nCmd = _None_;
	m_bCmdF = FALSE;
	m_bAutoConnect = TRUE;

	RECT rt = { 0,0,0,0 };
	Create(NULL, _T("None"), WS_CHILD, rt, FromHandle(m_hParentWnd), (UINT)this);

	m_strAddrCli = sAddrCli;
	StartServer(sAddrSvr, sPortSvr);
	Sleep(10);

	m_pThread = NULL;
}


CEngrave::~CEngrave()
{
}


///////////////////////////////////////////////////////////////////////////////
// OnUpdateConnection
// This message is sent by server manager to indicate connection status
LRESULT CEngrave::OnUpdateConnection(WPARAM wParam, LPARAM lParam)
{
	UINT uEvent = (UINT)wParam;
	CEngrave* pClient = reinterpret_cast<CEngrave*>(lParam);

	if (pClient != NULL)
	{
		// Server socket is now connected, we need to pick a new one
		if (uEvent == EVT_CONSUCCESS)
		{
			//pGlobalDoc->m_bConnect = TRUE;
		}
		else if (uEvent == EVT_CONFAILURE || uEvent == EVT_CONDROP)
		{
			// Disconnect socket
			// 자동접속시도
			//StopServer();
			//if (uEvent == EVT_CONDROP && m_bAutoConnect)
			//	SetTimer(TIM_CONNECT, 9000, NULL);
		}
	}

	return 1L;
}


void CEngrave::SetHwnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

void CEngrave::StartServer(CString sAddr, CString sPort)
{
	if (!m_pServer)
	{
		m_strAddrSvr = sAddr;
		m_strPortSvr = sPort;

		m_pServer = new CTcpIpServer(this);
		m_pServer->Init(sAddr, _tstoi(sPort));
		m_pServer->Start();
		m_pServer->SetServer(m_nServerID);
	}
}

void CEngrave::StopServer()
{
	if (m_pServer)
	{
		//if (!m_pServer->Stop()) // Called Destroy Function.
		//{
		//	delete m_pServer;
		//}
		m_pServer->Stop();
		Sleep(10);
		m_pServer->StopThread();
		Sleep(10);
		delete m_pServer;
		m_pServer = NULL;
		Sleep(10);
	}
}

LRESULT CEngrave::wmServerClosed(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	switch (nServerID)
	{
	case ID_ENGRAVE:
		if (m_pServer)
			m_pServer = NULL;
		break;
	}

	return (LRESULT)1;
}


SOCKET_DATA CEngrave::GetSocketData()
{
	return m_SocketData;
}

LRESULT CEngrave::wmAcceptReceived(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	//if (nServerID == ID_ENGRAVE)
	//	m_SocketData = m_pServer->GetSocketData();

	return (LRESULT)OnServerReceived(wParam, lParam);
}

int CEngrave::OnServerReceived(WPARAM wParam, LPARAM lParam)
{
	int nAcceptID = (int)wParam;
	//CString sReceived = (LPCTSTR)lParam;
	//if (sReceived == _T("Fail to connect."))
	//	return 1;
	if (m_pServer)
		m_SocketData = m_pServer->GetSocketData();
	if (m_hParentWnd)
		::PostMessage(m_hParentWnd, WM_SERVER_RECEIVED, (WPARAM)nAcceptID, (LPARAM)&m_SocketData); // "OrderNum-ShotNum" (9bytes'-'3bytes)
	m_bWaitForResponse = FALSE;
	//if (sReceived.GetLength() > 0)
/*
	switch (nAcceptID)
	{
	case ID_ENGRAVE:
		if(m_pServer)
			m_SocketData = m_pServer->GetSocketData();
		if (m_hParentWnd)
			::PostMessage(m_hParentWnd, WM_SERVER_RECEIVED, (WPARAM)ID_ENGRAVE, (LPARAM)&m_SocketData); // "OrderNum-ShotNum" (9bytes'-'3bytes)
		m_bWaitForResponse = FALSE;
		//if (sReceived.GetLength() > 0)
		//{
		//	if (CheckResponse(m_nCmd, sReceived))
		//	{
		//		;
		//	}
		//	else
		//	{
		//		pView->MsgBox(_T("Error-CheckResponse"));
		//		//AfxMessageBox(_T("Error-CheckResponse"));
		//	}
		//}
		break;
	}
*/
	return 1;
}

BOOL CEngrave::CheckResponse(int nCmd, CString sResponse)
{
	int nPos;
	CString sParsing;

	switch (nCmd)
	{
	//case SrTriggerInputOn:
	//	if (nPos = sResponse.Find(_T("ERROR")) > 0)
	//	{
	//		pView->MsgBox(_T("Error-Mdx response"));
	//		//AfxMessageBox(_T("Error-Mdx response"));
	//		m_strResponse = sResponse;
	//		m_bWaitForResponse = FALSE;
	//	}
	//	else
	//	{
	//		m_strResponse = sResponse;
	//		m_bWaitForResponse = FALSE;
	//		if (m_hParentWnd)
	//			::PostMessage(m_hParentWnd, WM_CLIENT_RECEIVED_SR, (WPARAM)SrTriggerInputOn, (LPARAM)&m_strResponse); // "OrderNum-ShotNum" (9bytes'-'3bytes)
	//	}
	//	return TRUE;
		;
	}

	return FALSE;
}


BOOL CEngrave::IsRunning()
{
	return (m_bWaitForResponse);
}

BOOL CEngrave::ReadComm(CString &sData)
{
	if (!m_bWaitForResponse)
	{
		sData = m_strResponse;
		return TRUE;
	}

	return FALSE;
}

int CEngrave::WriteComm(CString sMsg, DWORD dwTimeout)
{
	//return m_pServer->WriteComm(m_strAddrCli, sMsg);
	return 0;
}


// Thread
void CEngrave::StartThread() // Worker Thread 구동관련 Step8
{
	if (m_pThread == NULL)
	{
		m_bModify = TRUE;
		m_evtThread.ResetEvent();
		m_pThread = AfxBeginThread(RunThread, this);
		if (m_pThread)
			m_hThread = m_pThread->m_hThread;
	}
}

void CEngrave::StopThread() // Worker Thread 구동관련 Step9
{
	if (m_pThread != NULL)
	{
		m_evtThread.SetEvent();
		WaitUntilThreadEnd(m_hThread);
	}
	m_pThread = NULL;
	m_bModify = FALSE;
}

void CEngrave::WaitUntilThreadEnd(HANDLE hThread) // Worker Thread 구동관련 Step6
{
	TRACE("WaitUntilThreadEnd(0x%08x:RunThread) Entering\n", hThread);
	MSG message;
	const DWORD dwTimeOut = 500000;
	DWORD dwStartTick = GetTickCount();
	DWORD dwExitCode;
	while (GetExitCodeThread(hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE && m_bAlive) {
		// Time Out Check
		if (GetTickCount() >= (dwStartTick + dwTimeOut))
		{
			//pView->MsgBox("WaitUntilThreadEnd() Time Out!!!");
			AfxMessageBox(_T("WaitUntilThreadEnd() Time Out!!!", NULL, MB_OK | MB_ICONSTOP));
			return;
		}
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
	TRACE("WaitUntilThreadEnd(0x%08x:DispMsgThread) Exit\n", hThread);
}

UINT CEngrave::RunThread(LPVOID pParam)
{
	CEngrave* pMk = (CEngrave*)pParam;
	DWORD dwMilliseconds = 10;
	pMk->m_bAlive = TRUE;

	MSG message;
	CString sMsg;

	while (WAIT_OBJECT_0 != ::WaitForSingleObject(pMk->m_evtThread, dwMilliseconds))
	{
		Sleep(100);
	}

	pMk->m_bAlive = FALSE;
	return 0;
}

void CEngrave::Close()
{
	StopThread();
	Sleep(100);

	if (m_pServer)
	{
		m_bAutoConnect = FALSE;
		StopServer();
	}
}


// General Function

BOOL CEngrave::SendCommand(int nAcceptId, SOCKET_DATA SocketData)
{
	SocketData.nTxPC = _Punch;	// Client 
	SocketData.nRxPC = _Engrave; // Server

	//CSocketManager* pCurServer = GetSocketManager(nRxClientId);
	//if (!pCurServer)
	//{
	//	CString strErrMsg;
	//	strErrMsg.Format(_T("[MSG604] Message receive IPU[%d] does not connected"), nRxClientId);
	//	AfxMessageBox(strErrMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
	//	return SOCKET_MANAGER_ERROR;
	//}
	//pDoc->ClearDataRecvEndFlag(nRxClientId);
	m_bWaitForResponse = TRUE;
	m_pServer->WriteSocketData(nAcceptId, SocketData, INFINITE);
	//m_pServer->WriteCommData(SocketData, INFINITE);
	//pCurServer->DisplayTransMessage(SocketData);
	return WaitResponse();
}

BOOL CEngrave::WaitResponse()
{
	MSG message;
	DWORD dwStartTick = GetTickCount();
/*
	while (IsRunning())
	{
		if (GetTickCount() - dwStartTick < 10000)
		{
			AfxMessageBox(_T("WaitResponse() Time Out!!!"));
			return FALSE;
		}

		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(10);
	}
*/
	return TRUE;
}

void CEngrave::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TIM_CONNECT:
		KillTimer(TIM_CONNECT);
		SetTimer(TIM_CONNECT + 1, 1000, NULL);
		break;
	case TIM_CONNECT + 1:
		KillTimer(TIM_CONNECT + 1);
		StartServer(m_strAddrSvr, m_strPortSvr);
		SetTimer(TIM_CONNECT + 2, 1000, NULL);
		break;
	case TIM_CONNECT + 2:
		KillTimer(TIM_CONNECT + 2);
		//if(!m_pServer->IsConnected(m_strAddrCli))
		//	SetTimer(TIM_CONNECT + 1, 1000, NULL);
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

BOOL CEngrave::IsConnected()
{
	return m_pServer->IsConnected(m_strAddrCli);
	//return FALSE;
}
