#include "stdafx.h"
#include "Engrave.h"

#include "EngraveDef.h"
#include "../Global/GlobalDefine.h"
#include "../GvisR2R_LaserDoc.h"
#include "../GvisR2R_LaserView.h"

extern CGvisR2R_LaserDoc* pDoc;
extern CGvisR2R_LaserView* pView;
extern CString PATH_WORKING_INFO;

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
	if(pParent)
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
	m_nConnectedId = -1;

	m_bGetOpInfo = FALSE; m_bGetInfo = FALSE; m_bGetEngInfo = FALSE;
	m_bGetSignalMain = FALSE; m_bGetSignalTorqueMotor = FALSE; m_bGetSignalInductionMotor = FALSE; m_bGetSignalCore150mm = FALSE; m_bGetSignalEtc = FALSE;
	m_bGetSignalRecoiler = FALSE; m_bGetSignalPunch = FALSE; m_bGetSignalAOIDn = FALSE; m_bGetSignalAOIUp = FALSE; m_bGetSignalEngrave = FALSE; m_bGetSignalUncoiler = FALSE;
	m_bGetSignalEngraveAutoSequence = FALSE;
	m_bGetTotRatio = FALSE; m_bGetStTime = FALSE; m_bGetRunTime = FALSE; m_bGetEdTime = FALSE; m_bGetStripRatio = FALSE; m_bGetDef = FALSE;
	m_bGet2DReader = FALSE; m_bGetEngInfo = FALSE; m_bGetFdInfo = FALSE; m_bGetAoiInfo = FALSE; m_bGetMkInfo = FALSE; m_bGetMkInfoLf = FALSE; m_bGetMkInfoRt = FALSE;
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
	SOCKET_DATA *pSocketData = (SOCKET_DATA*)lParam;
	//CString sReceived = (LPCTSTR)lParam;
	//if (sReceived == _T("Fail to connect."))
	//	return 1;

	//if (m_pServer)
	//	m_SocketData = m_pServer->GetSocketData();
	//if (m_hParentWnd)
	//	::PostMessage(m_hParentWnd, WM_SERVER_RECEIVED, (WPARAM)nAcceptID, (LPARAM)&m_SocketData); // "OrderNum-ShotNum" (9bytes'-'3bytes)
	
	if (m_hParentWnd)
		::PostMessage(m_hParentWnd, WM_SERVER_RECEIVED, (WPARAM)nAcceptID, (LPARAM)pSocketData);
	
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

//BOOL CEngrave::SendCommand(int nAcceptId, SOCKET_DATA SocketData)
//{
//	SocketData.nTxPC = _Punch;	// Client 
//	SocketData.nRxPC = _Engrave; // Server
//
//	//CSocketManager* pCurServer = GetSocketManager(nRxClientId);
//	//if (!pCurServer)
//	//{
//	//	CString strErrMsg;
//	//	strErrMsg.Format(_T("[MSG604] Message receive IPU[%d] does not connected"), nRxClientId);
//	//	AfxMessageBox(strErrMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
//	//	return SOCKET_MANAGER_ERROR;
//	//}
//	//pDoc->ClearDataRecvEndFlag(nRxClientId);
//	m_bWaitForResponse = TRUE;
//	m_pServer->WriteSocketData(nAcceptId, SocketData, INFINITE);
//	//m_pServer->WriteCommData(SocketData, INFINITE);
//	//pCurServer->DisplayTransMessage(SocketData);
//	return WaitResponse();
//}

BOOL CEngrave::SendCommand(SOCKET_DATA SocketData, BOOL bWait)
{
	if (!m_pServer)
		return FALSE;

	BOOL bRtn = TRUE;
	SocketData.nTxPC = _Engrave;	// Server
	SocketData.nRxPC = _Punch;		// Client
	m_pServer->WriteSocketData(m_nConnectedId, SocketData, INFINITE);

	if (bWait)
	{
		m_bWaitForResponse = TRUE;
		bRtn = WaitResponse();
	}

	return bRtn;
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

BOOL CEngrave::IsConnected()
{
	if (!m_pServer)
		return FALSE;

	return m_pServer->IsConnected(m_strAddrCli, m_nConnectedId);
	//return FALSE;
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

// Communcation

void CEngrave::GetSysSignal(SOCKET_DATA SockData)
{
	GetOpInfo(SockData);
	GetInfo(SockData);
	GetEngInfo(SockData);

	GetSignalDisp(SockData);
	GetSignalMain(SockData);
	GetSignalTorqueMotor(SockData);
	GetSignalInductionMotor(SockData);
	GetSignalCore150mm(SockData);
	GetSignalEtc(SockData);
	GetSignalRecoiler(SockData);
	GetSignalPunch(SockData);
	GetSignalAOIDn(SockData);
	GetSignalAOIUp(SockData);
	GetSignalEngrave(SockData);
	GetSignalUncoiler(SockData);

	GetSignalEngraveAutoSequence(SockData);
	GetSignalMyMsg(SockData);
}

void CEngrave::GetSignalDisp(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_DispReady:
			pDoc->BtnStatus.Disp.Init();
			pDoc->BtnStatus.Disp.Ready = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispRun:
			pDoc->BtnStatus.Disp.Init();
			pDoc->BtnStatus.Disp.Run = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispStop:
			pDoc->BtnStatus.Disp.Init();
			pDoc->BtnStatus.Disp.Stop = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispDualSample:
			pDoc->BtnStatus.Disp.Init();
			pDoc->BtnStatus.Disp.DualSample = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispSingleSample:
			pDoc->BtnStatus.Disp.Init();
			pDoc->BtnStatus.Disp.SingleSample = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispDualTest:
			pDoc->BtnStatus.Disp.Init();
			pDoc->BtnStatus.Disp.DualTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispSingleTest:
			pDoc->BtnStatus.Disp.Init();
			pDoc->BtnStatus.Disp.SingleTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsDispReady:
			pDoc->BtnStatus.Disp.IsReady = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispRun:
			pDoc->BtnStatus.Disp.IsRun = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispStop:
			pDoc->BtnStatus.Disp.IsStop = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispDualSample:
			pDoc->BtnStatus.Disp.IsDualSample = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispSingleSample:
			pDoc->BtnStatus.Disp.IsSingleSample = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispDualTest:
			pDoc->BtnStatus.Disp.IsDualTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispSingleTest:
			pDoc->BtnStatus.Disp.IsSingleTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalMain(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Ready:
			pDoc->BtnStatus.Main.Ready = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_Run:
			pDoc->BtnStatus.Main.Run = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.Main.Stop = (SockData.nData1 > 0) ? FALSE : pDoc->BtnStatus.Main.Stop;
			break;
		case _SigInx::_Reset:
			pDoc->BtnStatus.Main.Reset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_Stop:
			pDoc->BtnStatus.Main.Stop = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.Main.Run = (SockData.nData1 > 0) ? FALSE : pDoc->BtnStatus.Main.Run;
			pView->EngStop(pDoc->BtnStatus.Main.Stop);
			break;
		case _SigInx::_Auto:
			pDoc->Status.bAuto = pDoc->BtnStatus.Main.Auto = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->Status.bManual = pDoc->BtnStatus.Main.Manual = (SockData.nData1 > 0) ? FALSE : pDoc->BtnStatus.Main.Manual;
			break;
		case _SigInx::_Manual:
			pDoc->Status.bManual = pDoc->BtnStatus.Main.Manual = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->Status.bAuto = pDoc->BtnStatus.Main.Auto = (SockData.nData1 > 0) ? FALSE : pDoc->BtnStatus.Main.Auto;
			break;
		// Is
		case _SigInx::_IsReady:
			pDoc->BtnStatus.Main.IsReady = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsRun:
			pDoc->BtnStatus.Main.IsRun = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsReset:
			pDoc->BtnStatus.Main.IsReset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsStop:
			pDoc->BtnStatus.Main.IsStop = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsAuto:
			pDoc->Status.bAuto = pDoc->BtnStatus.Main.IsAuto = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsManual:
			pDoc->Status.bManual = pDoc->BtnStatus.Main.IsManual = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;

		}	
	}
}

void CEngrave::GetSignalTorqueMotor(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_MkTq:
			pDoc->BtnStatus.Tq.Mk = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_AoiTq:
			pDoc->BtnStatus.Tq.Aoi = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngTq:
			pDoc->BtnStatus.Tq.Eng = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsMkTq:
			pDoc->BtnStatus.Tq.IsMk = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsAoiTq:
			pDoc->BtnStatus.Tq.IsAoi = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngTq:
			pDoc->BtnStatus.Tq.IsEng = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalInductionMotor(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_CcwModRe:
			pDoc->BtnStatus.Induct.Rc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_CcwModUn:
			pDoc->BtnStatus.Induct.Uc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsCcwModRe:
			pDoc->BtnStatus.Induct.IsRc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsCcwModUn:
			pDoc->BtnStatus.Induct.IsUc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalCore150mm(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Core150Re:
			pDoc->BtnStatus.Core150.Rc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_Core150Un:
			pDoc->BtnStatus.Core150.Uc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsCore150Re:
			pDoc->BtnStatus.Core150.IsRc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsCore150Un:
			pDoc->BtnStatus.Core150.IsUc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalEtc(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_EmgAoi:
			pDoc->BtnStatus.Etc.EmgAoi = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsEmgAoi:
			pDoc->BtnStatus.Etc.IsEmgAoi = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalRecoiler(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//pDoc->BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwRe:
			pDoc->BtnStatus.Rc.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCcwRe:
			pDoc->BtnStatus.Rc.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PrdChuckRe:
			pDoc->BtnStatus.Rc.ReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DancerUpRe:
			pDoc->BtnStatus.Rc.DcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PasteUpLfRe:
			pDoc->BtnStatus.Rc.ReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PasteUpRtRe:
			pDoc->BtnStatus.Rc.ReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PasteVacRe:
			pDoc->BtnStatus.Rc.ReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PprChuckRe:
			pDoc->BtnStatus.Rc.PprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PprCwRe:
			pDoc->BtnStatus.Rc.PprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PprCcwRe:
			pDoc->BtnStatus.Rc.PprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DoRe:
			pDoc->BtnStatus.Rc.Rewine = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PrdPprRe:
			pDoc->BtnStatus.Rc.RewineReelPpr = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsRelation:
			pDoc->BtnStatus.Rc.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwRe:
			pDoc->BtnStatus.Rc.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwRe:
			pDoc->BtnStatus.Rc.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPrdChuckRe:
			pDoc->BtnStatus.Rc.IsReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDancerUpRe:
			pDoc->BtnStatus.Rc.IsDcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteUpLfRe:
			pDoc->BtnStatus.Rc.ReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteUpRtRe:
			pDoc->BtnStatus.Rc.IsReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteVacRe:
			pDoc->BtnStatus.Rc.IsReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprChuckRe:
			pDoc->BtnStatus.Rc.IsPprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprCwRe:
			pDoc->BtnStatus.Rc.IsPprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprCcwRe:
			pDoc->BtnStatus.Rc.IsPprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDoRe:
			pDoc->BtnStatus.Rc.IsRewine = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPrdPprRe:
			pDoc->BtnStatus.Rc.IsRewineReelPpr = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalPunch(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			pDoc->BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwMk:
			pDoc->BtnStatus.Mk.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCcwMk:
			pDoc->BtnStatus.Mk.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdVacMk:
			pDoc->BtnStatus.Mk.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PushUpMk:
			pDoc->BtnStatus.Mk.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblBlwMk:
			pDoc->BtnStatus.Mk.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblVacMk:
			pDoc->BtnStatus.Mk.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdClampMk:
			pDoc->BtnStatus.Mk.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TensClampMk:
			pDoc->BtnStatus.Mk.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_OnePnlMk:
			pDoc->BtnStatus.Mk.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DancerUpMk:
			pDoc->BtnStatus.Mk.DcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsRelation:
			pDoc->BtnStatus.Mk.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwMk:
			pDoc->BtnStatus.Mk.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwMk:
			pDoc->BtnStatus.Mk.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdVacMk:
			pDoc->BtnStatus.Mk.IsFdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPushUpMk:
			pDoc->BtnStatus.Mk.IsPushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblBlwMk:
			pDoc->BtnStatus.Mk.IsTblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblVacMk:
			pDoc->BtnStatus.Mk.IsTblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdClampMk:
			pDoc->BtnStatus.Mk.IsFdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTensClampMk:
			pDoc->BtnStatus.Mk.IsTqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsOnePnlMk:
			pDoc->BtnStatus.Mk.IsMvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDancerUpMk:
			pDoc->BtnStatus.Mk.IsDcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalAOIDn(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//pDoc->BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwAoiDn:
			pDoc->BtnStatus.AoiDn.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCcwAoiDn:
			pDoc->BtnStatus.AoiDn.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdVacAoiDn:
			pDoc->BtnStatus.AoiDn.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PushUpAoiDn:
			pDoc->BtnStatus.AoiDn.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblBlwAoiDn:
			pDoc->BtnStatus.AoiDn.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblVacAoiDn:
			pDoc->BtnStatus.AoiDn.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdClampAoiDn:
			pDoc->BtnStatus.AoiDn.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TensClampAoiDn:
			pDoc->BtnStatus.AoiDn.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_OnePnlAoiDn:
			pDoc->BtnStatus.AoiDn.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ClrRollAoiDn:
			pDoc->BtnStatus.AoiDn.ClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_VelClrSonicAoiDn:
			pDoc->BtnStatus.AoiDn.VelSonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TestAoiDn:
			pDoc->BtnStatus.AoiDn.Test = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ResetAoiDn:
			pDoc->BtnStatus.AoiDn.Reset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_LotEndAoiDn:
			pDoc->BtnStatus.AoiDn.LotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsRelation:
			pDoc->BtnStatus.AoiDn.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwAoiDn:
			pDoc->BtnStatus.AoiDn.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwAoiDn:
			pDoc->BtnStatus.AoiDn.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdVacAoiDn:
			pDoc->BtnStatus.AoiDn.IsFdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPushUpAoiDn:
			pDoc->BtnStatus.AoiDn.IsPushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblBlwAoiDn:
			pDoc->BtnStatus.AoiDn.IsTblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblVacAoiDn:
			pDoc->BtnStatus.AoiDn.IsTblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdClampAoiDn:
			pDoc->BtnStatus.AoiDn.IsFdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTensClampAoiDn:
			pDoc->BtnStatus.AoiDn.IsTqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsOnePnlAoiDn:
			pDoc->BtnStatus.AoiDn.IsMvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsClrRollAoiDn:
			pDoc->BtnStatus.AoiDn.IsClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsVelClrSonicAoiDn:
			pDoc->BtnStatus.AoiDn.IsVelSonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTestAoiDn:
			pDoc->BtnStatus.AoiDn.IsTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsResetAoiDn:
			pDoc->BtnStatus.AoiDn.IsReset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsLotEndAoiDn:
			pDoc->BtnStatus.AoiDn.IsLotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalAOIUp(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//pDoc->BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwAoiUp:
			pDoc->BtnStatus.AoiUp.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCcwAoiUp:
			pDoc->BtnStatus.AoiUp.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdVacAoiUp:
			pDoc->BtnStatus.AoiUp.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PushUpAoiUp:
			pDoc->BtnStatus.AoiUp.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblBlwAoiUp:
			pDoc->BtnStatus.AoiUp.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblVacAoiUp:
			pDoc->BtnStatus.AoiUp.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdClampAoiUp:
			pDoc->BtnStatus.AoiUp.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TensClampAoiUp:
			pDoc->BtnStatus.AoiUp.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_OnePnlAoiUp:
			pDoc->BtnStatus.AoiUp.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ClrRollAoiUp:
			pDoc->BtnStatus.AoiUp.ClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TestAoiUp:
			pDoc->BtnStatus.AoiUp.Test = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ResetAoiUp:
			pDoc->BtnStatus.AoiUp.Reset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_LotEndAoiUp:
			pDoc->BtnStatus.AoiUp.LotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsRelation:
			pDoc->BtnStatus.AoiUp.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwAoiUp:
			pDoc->BtnStatus.AoiUp.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwAoiUp:
			pDoc->BtnStatus.AoiUp.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdVacAoiUp:
			pDoc->BtnStatus.AoiUp.IsFdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPushUpAoiUp:
			pDoc->BtnStatus.AoiUp.IsPushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblBlwAoiUp:
			pDoc->BtnStatus.AoiUp.IsTblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblVacAoiUp:
			pDoc->BtnStatus.AoiUp.IsTblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdClampAoiUp:
			pDoc->BtnStatus.AoiUp.IsFdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTensClampAoiUp:
			pDoc->BtnStatus.AoiUp.IsTqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsOnePnlAoiUp:
			pDoc->BtnStatus.AoiUp.IsMvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsClrRollAoiUp:
			pDoc->BtnStatus.AoiUp.IsClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTestAoiUp:
			pDoc->BtnStatus.AoiUp.IsTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsResetAoiUp:
			pDoc->BtnStatus.AoiUp.IsReset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsLotEndAoiUp:
			pDoc->BtnStatus.AoiUp.IsLotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalEngrave(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//pDoc->BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwEng:
			pDoc->BtnStatus.Eng.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCcwEng:
			pDoc->BtnStatus.Eng.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdVacEng:
			pDoc->BtnStatus.Eng.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PushUpEng:
			pDoc->BtnStatus.Eng.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _TblBlwEng:
			pDoc->BtnStatus.Eng.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblVacEng:
			pDoc->BtnStatus.Eng.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdClampEng:
			pDoc->BtnStatus.Eng.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TensClampEng:
			pDoc->BtnStatus.Eng.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_OnePnlEng:
			pDoc->BtnStatus.Eng.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DancerUpEng:
			pDoc->BtnStatus.Eng.DcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_VelClrSonicEng:
			pDoc->BtnStatus.Eng.VelSonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsRelation:
			pDoc->BtnStatus.Eng.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwEng:
			pDoc->BtnStatus.Eng.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwEng:
			pDoc->BtnStatus.Eng.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdVacEng:
			pDoc->BtnStatus.Eng.IsFdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPushUpEng:
			pDoc->BtnStatus.Eng.IsPushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _IsTblBlwEng:
			pDoc->BtnStatus.Eng.IsTblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblVacEng:
			pDoc->BtnStatus.Eng.IsTblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdClampEng:
			pDoc->BtnStatus.Eng.IsFdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTensClampEng:
			pDoc->BtnStatus.Eng.IsTqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsOnePnlEng:
			pDoc->BtnStatus.Eng.IsMvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDancerUpEng:
			pDoc->BtnStatus.Eng.IsDcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsVelClrSonicEng:
			pDoc->BtnStatus.Eng.IsVelSonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalUncoiler(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//pDoc->BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//pDoc->BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwUn:
			pDoc->BtnStatus.Uc.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCcwUn:
			pDoc->BtnStatus.Uc.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PrdChuckUn:
			pDoc->BtnStatus.Uc.ReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DancerUpUn:
			pDoc->BtnStatus.Uc.DcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PasteUpLfUn:
			pDoc->BtnStatus.Uc.ReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PasteUpRtUn:
			pDoc->BtnStatus.Uc.ReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PasteVacUn:
			pDoc->BtnStatus.Uc.ReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PprChuckUn:
			pDoc->BtnStatus.Uc.PprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PprCwUn:
			pDoc->BtnStatus.Uc.PprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PprCcwUn:
			pDoc->BtnStatus.Uc.PprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ClrRollUpUn:
			pDoc->BtnStatus.Uc.ClRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ClrRollPushUn:
			pDoc->BtnStatus.Uc.ClRlPshUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsRelation:
			pDoc->BtnStatus.Uc.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwUn:
			pDoc->BtnStatus.Uc.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwUn:
			pDoc->BtnStatus.Uc.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPrdChuckUn:
			pDoc->BtnStatus.Uc.IsReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDancerUpUn:
			pDoc->BtnStatus.Uc.IsDcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteUpLfUn:
			pDoc->BtnStatus.Uc.IsReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteUpRtUn:
			pDoc->BtnStatus.Uc.IsReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteVacUn:
			pDoc->BtnStatus.Uc.IsReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprChuckUn:
			pDoc->BtnStatus.Uc.IsPprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprCwUn:
			pDoc->BtnStatus.Uc.IsPprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprCcwUn:
			pDoc->BtnStatus.Uc.IsPprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsClrRollUpUn:
			pDoc->BtnStatus.Uc.IsClRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsClrRollPushUn:
			pDoc->BtnStatus.Uc.IsClRlPshUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalEngraveAutoSequence(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_EngAutoInit:
			pDoc->BtnStatus.EngAuto.Init = (SockData.nData1 > 0) ? TRUE : FALSE;
			if (pView->m_pDlgMenu03)
				pView->m_pDlgMenu03->SwReset();
			break;
		case _SigInx::_EngAutoSeqMkSt:
			if(!pView->m_bEngSt)
				pDoc->BtnStatus.EngAuto.MkSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			//SwEngAutoMkSt(!pDoc->BtnStatus.EngAuto.MkSt);
			break;
		case _SigInx::_EngAutoSeqOnMkIng:
			pDoc->BtnStatus.EngAuto.OnMking = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngAutoSeqMkDone:
			pDoc->BtnStatus.EngAuto.MkDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngAutoSeq2dReadSt:
			if (!pView->m_bEng2dSt)
				pDoc->BtnStatus.EngAuto.Read2dSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			//SwEngAuto2dReadSt(!pDoc->BtnStatus.EngAuto.Read2dSt);
			break;
		case _SigInx::_EngAutoSeqOnReading2d:
			pDoc->BtnStatus.EngAuto.OnRead2d = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngAutoSeq2dReadDone:
			pDoc->BtnStatus.EngAuto.Read2dDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_UpdateWork:
			pView->GetMkMenu01();
			break;
		case _SigInx::_DispDefImg:
			if (!pView->m_bTHREAD_DISP_DEF)
			{
				pView->m_nStepTHREAD_DISP_DEF = 0;
				pView->m_bTHREAD_DISP_DEF = TRUE;
				//pView->UpdateLotTime();
			}
			break;
		// Is
		case _SigInx::_IsEngAutoInit:
			pDoc->BtnStatus.EngAuto.IsInit = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeqMkSt:
			pDoc->BtnStatus.EngAuto.IsMkSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeqOnMkIng:
			pDoc->BtnStatus.EngAuto.IsOnMking = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeqMkDone:
			pDoc->BtnStatus.EngAuto.IsMkDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeq2dReadSt:
			pDoc->BtnStatus.EngAuto.IsRead2dSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeqOnReading2d:
			pDoc->BtnStatus.EngAuto.IsOnRead2d = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeq2dReadDone:
			pDoc->BtnStatus.EngAuto.IsRead2dDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsUpdateWork:
			pDoc->BtnStatus.EngAuto.IsRead2dDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalMyMsg(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_MyMsgYes:
			pView->SetMyMsgYes();
			break;
		case _SigInx::_MyMsgNo:
			pView->SetMyMsgNo();
			break;
		case _SigInx::_MyMsgOk:
			pView->SetMyMsgOk();
			break;
			// Is
		case _SigInx::_IsMyMsgYes:
			pDoc->BtnStatus.Msg.IsYes = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMyMsgNo:
			pDoc->BtnStatus.Msg.IsNo = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMyMsgOk:
			pDoc->BtnStatus.Msg.IsOk = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

// Start for GetSysData()

void CEngrave::GetSysData(SOCKET_DATA SockData)
{
	GetOpInfo(SockData);
	GetInfo(SockData);
	GetTotRatio(SockData);
	GetStTime(SockData);
	GetRunTime(SockData);
	GetEdTime(SockData);
	GetUpdateWorking(SockData);
	GetStripRatio(SockData);
	GetDef(SockData);
	Get2DReader(SockData);
	GetEngInfo(SockData);
	GetFdInfo(SockData);
	GetAoiInfo(SockData);
	GetMkInfo(SockData);
	GetMkInfoLf(SockData);
	GetMkInfoRt(SockData);
	GetAlarmMsg(SockData);
	GetMsgBox(SockData);
}

void CEngrave::GetOpInfo(SOCKET_DATA SockData)
{
	long lData;

	int nCmdCode = SockData.nCmdCode; // _SetSig or _SetData
	int nMsgId = SockData.nMsgID;

	CString sVal;
	//m_bGetOpInfo = FALSE;
	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_DualTest:
			if (pDoc->WorkingInfo.LastJob.bDualTest != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bDualTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_SampleTest:
			if (pDoc->WorkingInfo.LastJob.bSampleTest != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bSampleTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_TestMode:
			//if(pDoc->GetTestMode() != (int)SockData.nData1) // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
			if(pDoc->WorkingInfo.LastJob.nTestMode != (int)SockData.nData1) // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
			{
				m_bGetOpInfo = TRUE;
				//pDoc->SetTestMode((int)SockData.nData1); // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
				//pDoc->WorkingInfo.LastJob.nTestMode = (int)SockData.nData1; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
				pDoc->SetCurrentInfoTestMode((int)SockData.nData1);
			}
			break;
		case _SigInx::_RecoilerCcw:
			if(pDoc->WorkingInfo.LastJob.bOneMetal != (SockData.nData1 > 0) ? TRUE : FALSE)	// OneMetal : TRUE -> SetTwoMetal(FALSE);
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bOneMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// OneMetal : TRUE -> SetTwoMetal(FALSE);
			}
			break;
		case _SigInx::_UncoilerCcw:
			if(pDoc->WorkingInfo.LastJob.bTwoMetal != (SockData.nData1 > 0) ? TRUE : FALSE)	// TwoMetal : TRUE -> SetTwoMetal(TRUE);
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bTwoMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// TwoMetal : TRUE -> SetTwoMetal(TRUE);
			}
			break;
		case _SigInx::_AlignMethode:
			if(pDoc->WorkingInfo.LastJob.nAlignMethode != (int)SockData.nData1) // TWO_POINT, FOUR_POINT
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.nAlignMethode = (int)SockData.nData1; // TWO_POINT, FOUR_POINT
			}
			break;
		case _SigInx::_DoorRecoiler:
			if (pDoc->WorkingInfo.LastJob.bRclDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bRclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_DoorAoiUp:
			if (pDoc->WorkingInfo.LastJob.bAoiUpDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bAoiUpDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_DoorAoiDn:
			if (pDoc->WorkingInfo.LastJob.bAoiDnDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bAoiDnDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_DoorMk:
			if (pDoc->WorkingInfo.LastJob.bMkDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bMkDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_DoorEngrave:
			if (pDoc->WorkingInfo.LastJob.bEngvDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bEngvDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_DoorUncoiler:
			if (pDoc->WorkingInfo.LastJob.bUclDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bUclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_SaftyMk:
			if (pDoc->WorkingInfo.LastJob.bMkSftySen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bMkSftySen = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_CleannerAoiUp:
			if (pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
			pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_CleannerAoiDn:
			if (pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_UltraSonicAoiDn:
			if(pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_UltraSonicEngrave:
			if (pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_TempPause:
			if (pDoc->WorkingInfo.LastJob.bTempPause != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_LotCut:
			if (pDoc->WorkingInfo.LastJob.bLotSep = (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bLotSep = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_OpName:
			if (pDoc->WorkingInfo.LastJob.sSelUserName != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);

				if (pDoc && pDoc->m_pReelMap)
					pDoc->m_pReelMap->m_sUser = pDoc->WorkingInfo.LastJob.sSelUserName;
				if (pDoc)
					::WritePrivateProfileString(_T("Last Job"), _T("Operator Name"), pDoc->WorkingInfo.LastJob.sSelUserName, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_SampleShotNum:
			sVal.Format(_T("%d"), (int)SockData.nData1);
			if (pDoc->WorkingInfo.LastJob.sSampleTestShotNum != sVal)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sSampleTestShotNum = sVal;

				::WritePrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), sVal, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_TotReelLen:
			if (pDoc->WorkingInfo.LastJob.sReelTotLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sTotalReelDist = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Reel Total Length"), pDoc->WorkingInfo.LastJob.sReelTotLen, PATH_WORKING_INFO);
				::WritePrivateProfileString(_T("Lot"), _T("LOT_TOTAL_REEL_DIST"), pDoc->WorkingInfo.Lot.sTotalReelDist, PATH_WORKING_INFO);

#ifdef USE_MPE
				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 1000.0);
				if (pView && pView->m_pMpe)
					pView->m_pMpe->Write(_T("ML45000"), lData);	// 전체 Reel 길이 (단위 M * 1000)
#endif
			}
			break;
		case _ItemInx::_OnePnlLen:
			if (pDoc->WorkingInfo.Motion.sMkFdDist != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);

				if (pDoc->m_pReelMap)
					pDoc->m_pReelMap->m_dPnlLen = _tstof(pDoc->WorkingInfo.Motion.sMkFdDist);
				::WritePrivateProfileString(_T("Last Job"), _T("One Panel Length"), pDoc->WorkingInfo.Motion.sMkFdDist, PATH_WORKING_INFO);
				::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), pDoc->WorkingInfo.Motion.sMkFdDist, PATH_WORKING_INFO);
				::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), pDoc->WorkingInfo.Motion.sMkFdDist, PATH_WORKING_INFO);
#ifdef USE_MPE
				lData = (long)(pDoc->m_pReelMap->m_dPnlLen * 1000.0);
				pView->m_pMpe->Write(_T("ML45032"), lData);	// 한 판넬 길이 (단위 mm * 1000)
#endif
			}
			break;
		case _ItemInx::_TempStopLen:
			if (pDoc->WorkingInfo.LastJob.sTempPauseLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sStopDist = CharToString(SockData.strData);

				if (pDoc->m_pReelMap)
					pDoc->m_pReelMap->m_dTempPauseLen = _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen);
				::WritePrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), pDoc->WorkingInfo.LastJob.sTempPauseLen, PATH_WORKING_INFO);

#ifdef USE_MPE
				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen) * 1000.0);
				pView->m_pMpe->Write(_T("ML45006"), lData);	// 일시정지 길이 (단위 M * 1000)
#endif
			}
			break;
		case _ItemInx::_LotCutLen:
			if (pDoc->WorkingInfo.LastJob.sLotSepLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotSepLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sSeparateDist = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotCutPosLen:
			if (pDoc->WorkingInfo.LastJob.sLotCutPosLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sCuttingDist = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Lot"), _T("LOT_CUTTING_DIST"), pDoc->WorkingInfo.LastJob.sLotCutPosLen, PATH_WORKING_INFO);
				if (pDoc->m_pReelMap)
					pDoc->m_pReelMap->m_dLotCutPosLen = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
				::WritePrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), pDoc->WorkingInfo.LastJob.sLotCutPosLen, PATH_WORKING_INFO);

#ifdef USE_MPE
				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen) * 1000.0);
				pView->m_pMpe->Write(_T("ML45004"), lData);	// Lot 분리 후 절단위치 (단위 M * 1000)
#endif
			}
			break;
		case _ItemInx::_LmtTotYld:
			if(pDoc->WorkingInfo.LastJob.sLmtTotYld != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLmtTotYld = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), pDoc->WorkingInfo.LastJob.sLmtTotYld, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_LmtPatlYld:
			if(pDoc->WorkingInfo.LastJob.sLmtPatlYld != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLmtPatlYld = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), pDoc->WorkingInfo.LastJob.sLmtPatlYld, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_StripOutRatio:
			if(pDoc->WorkingInfo.LastJob.sStripOutRatio != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sStripOutRatio = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), pDoc->WorkingInfo.LastJob.sStripOutRatio, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_CustomNeedRatio:
			if (pDoc->WorkingInfo.LastJob.sCustomNeedRatio != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sCustomNeedRatio = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), pDoc->WorkingInfo.LastJob.sCustomNeedRatio, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_NumRangeFixDef:
			if (pDoc->WorkingInfo.LastJob.sNumRangeFixDef != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sNumRangeFixDef = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), pDoc->WorkingInfo.LastJob.sNumRangeFixDef, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_NumContFixDef:
			if (pDoc->WorkingInfo.LastJob.sNumContFixDef != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sNumContFixDef = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), pDoc->WorkingInfo.LastJob.sNumContFixDef, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_UltraSonicStTim:
			if(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, PATH_WORKING_INFO);
				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim) * 100.0);
#ifdef USE_MPE
				if (pView && pView->m_pMpe)
				{
					pView->m_pMpe->Write(_T("MW05940"), (long)lData);	// AOI_Dn (단위 [초] * 100) : 1 is 10 mSec.
					pView->m_pMpe->Write(_T("MW05942"), (long)lData);	// AOI_Up (단위 [초] * 100) : 1 is 10 mSec.
				}
#endif
			}
			break;
		case _ItemInx::_EngItsCode:
			if (pDoc->WorkingInfo.LastJob.sEngItsCode != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->m_sOrderNum = pDoc->WorkingInfo.LastJob.sEngItsCode = CharToString(SockData.strData);
				pDoc->SetEngItsCode(pDoc->WorkingInfo.LastJob.sEngItsCode);
				//::WritePrivateProfileString(_T("Last Job"), _T("Engrave Order Num"), pDoc->WorkingInfo.LastJob.sEngOrderNum, PATH_WORKING_INFO);
			}
			break;
		}
	}
}

void CEngrave::GetInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	//m_bGetInfo = FALSE;
	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_TempPause:
			if(pDoc->WorkingInfo.LastJob.bTempPause != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;

				::WritePrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), pDoc->WorkingInfo.LastJob.bTempPause ? _T("1") : _T("0"), PATH_WORKING_INFO);
#ifdef USE_MPE
				if (pView && pView->m_pMpe)
					pView->m_pMpe->Write(_T("MB440183"), pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0);
#endif
			}
			break;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_OpName:
			if (pDoc->WorkingInfo.LastJob.sSelUserName != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_ModelUpName:
			if (pDoc->WorkingInfo.LastJob.sModelUp != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sModelUp = CharToString(SockData.strData);
				//pView->m_bLoadMstInfo = TRUE;
			}
			break;
		//case _ItemInx::_ModelDnName:
		//	if (pDoc->WorkingInfo.LastJob.sModelDn != CharToString(SockData.strData))
		//	{
		//		m_bGetInfo = TRUE;
		//		pDoc->WorkingInfo.LastJob.sModelDn = CharToString(SockData.strData);
		//		//pView->m_bLoadMstInfo = TRUE;
		//	}
		//	break;
		case _ItemInx::_LotUpName:
			if (pDoc->WorkingInfo.LastJob.sLotUp != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotUp = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotDnName:
			if (pDoc->WorkingInfo.LastJob.sLotDn != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotDn = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LayerUpName:
			if (pDoc->WorkingInfo.LastJob.sLayerUp != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLayerUp = CharToString(SockData.strData);
				//pView->m_bLoadMstInfo = TRUE;
			}
			break;
		case _ItemInx::_LayerDnName:
			if (pDoc->WorkingInfo.LastJob.sLayerDn != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLayerDn = CharToString(SockData.strData);
				//pView->m_bLoadMstInfo = TRUE;
			}
			break;
		case _ItemInx::_TotReelLen:
			if (pDoc->WorkingInfo.LastJob.sReelTotLen != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sTotalReelDist = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_PartVel:
			if (pDoc->WorkingInfo.LastJob.sPartialSpd != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sPartialSpd = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Partial Speed"), pDoc->WorkingInfo.LastJob.sPartialSpd, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_TempStopLen:
			if (pDoc->WorkingInfo.LastJob.sTempPauseLen != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sStopDist = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotCutLen:
			if (pDoc->WorkingInfo.LastJob.sLotSepLen != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotSepLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sSeparateDist = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotCutPosLen:
			if (pDoc->WorkingInfo.LastJob.sLotCutPosLen != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sCuttingDist = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotSerial:
			if (pDoc->WorkingInfo.LastJob.sLotSerial != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotSerial = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_MkVerfyLen:
			if (pDoc->WorkingInfo.LastJob.sVerifyLen != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sVerifyLen = CharToString(SockData.strData);
			}
			break;
		default:
			break;
		}
	}
}


void CEngrave::GetUpdateWorking(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
			;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_TotOpRto:
			pView->SetTotOpRto(CharToString(SockData.strData));
			break;
		case _ItemInx::_TotVel:
			pView->SetTotVel(CharToString(SockData.strData));
			break;
		case _ItemInx::_PartVel:
			pView->SetPartVel(CharToString(SockData.strData));
			break;
		case _ItemInx::_MkDoneLen:
			pView->SetMkDoneLen(CharToString(SockData.strData));
			break;
		case _ItemInx::_AoiDnDoneLen:
			pView->SetAoiDnDoneLen(CharToString(SockData.strData));
			break;
		case _ItemInx::_AoiUpDoneLen:
			pView->SetAoiUpDoneLen(CharToString(SockData.strData));
			break;
		}
	}
}

void CEngrave::GetTotRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
			;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_TotOpRto:
			//m_bGetInfo = TRUE;
			pView->SetTotOpRto(CharToString(SockData.strData));
			break;
		case _ItemInx::_DefNumUp:
			if (pDoc->m_nBad[0] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nBad[0] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefRtoUp:
			if (pDoc->m_dBadRatio[0] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dBadRatio[0] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_GoodNumUp:
			if (pDoc->m_nGood[0] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nGood[0] = SockData.nData1;
			}
			break;
		case _ItemInx::_GoodRtoUp:
			if (pDoc->m_dGoodRatio[0] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dGoodRatio[0] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_TestNumUp:
			if (pDoc->m_nTestNum[0] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nTestNum[0] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefNumDn:
			if (pDoc->m_nBad[1] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nBad[1] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefRtoDn:
			if (pDoc->m_dBadRatio[1] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dBadRatio[1] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_GoodNumDn:
			if (pDoc->m_nGood[1] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nGood[1] = SockData.nData1;
			}
			break;
		case _ItemInx::_GoodRtoDn:
			if (pDoc->m_dGoodRatio[1] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dGoodRatio[1] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_TestNumDn:
			if (pDoc->m_nTestNum[1] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nTestNum[1] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefNumTot:
			if (pDoc->m_nBad[2] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nBad[2] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefRtoTot:
			if (pDoc->m_dBadRatio[2] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dBadRatio[2] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_GoodNumTot:
			if (pDoc->m_nGood[2] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nGood[2] = SockData.nData1;
			}
			break;
		case _ItemInx::_GoodRtoTot:
			if (pDoc->m_dGoodRatio[2] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dGoodRatio[2] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_TestNumTot:
			if (pDoc->m_nTestNum[2] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nTestNum[2] = SockData.nData1;
			}
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetStTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_LotStTime:
			if (pDoc->m_sLotStTime != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->m_sLotStTime = CharToString(SockData.strData);
			}
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetRunTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case  _ItemInx::_LotRunTime:
			if (pDoc->m_sLotRunTime != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->m_sLotRunTime = CharToString(SockData.strData);
			}
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetEdTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_LotEdTime:
			if (pDoc->m_sLotEdTime != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->m_sLotEdTime = CharToString(SockData.strData);
			}
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetStripRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_1LnGoodRtoUp:
			if (pDoc->m_dStripRatio[0][0] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[0][0] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_2LnGoodRtoUp:
			if (pDoc->m_dStripRatio[0][1] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[0][1] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_3LnGoodRtoUp:
			if (pDoc->m_dStripRatio[0][2] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[0][2] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_4LnGoodRtoUp:
			if (pDoc->m_dStripRatio[0][3] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[0][3] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_AllLnGoodRtoUp:
			if (pDoc->m_dStripRatio[0][4] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[0][4] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_1LnGoodRtoDn:
			if (pDoc->m_dStripRatio[1][0] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[1][0] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_2LnGoodRtoDn:
			if (pDoc->m_dStripRatio[1][1] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[1][1] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_3LnGoodRtoDn:
			if (pDoc->m_dStripRatio[1][2] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[1][2] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_4LnGoodRtoDn:
			if (pDoc->m_dStripRatio[1][3] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[1][3] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_AllLnGoodRtoDn:
			if (pDoc->m_dStripRatio[1][4] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[1][4] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_1LnGoodRtoTot:
			if (pDoc->m_dStripRatio[2][0] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[2][0] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_2LnGoodRtoTot:
			if (pDoc->m_dStripRatio[2][1] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[2][1] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_3LnGoodRtoTot:
			if (pDoc->m_dStripRatio[2][2] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[2][2] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_4LnGoodRtoTot:
			if (pDoc->m_dStripRatio[2][3] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[2][3] = (double)SockData.fData1;
			}
			break;
		case _ItemInx::_AllLnGoodRtoTot:
			if (pDoc->m_dStripRatio[2][4] != SockData.fData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_dStripRatio[2][4] = (double)SockData.fData1;
			}
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetDef(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_DefNumOpen:
			if (pDoc->m_nDef[DEF_OPEN] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_OPEN] = SockData.nData1; // IDC_STC_DEF_OPEN
			}
			break;
		case _ItemInx::_DefNumShort:
			if (pDoc->m_nDef[DEF_SHORT] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_SHORT] = SockData.nData1; // IDC_STC_DEF_SHORT
			}
			break;
		case _ItemInx::_DefNumUshort:
			if (pDoc->m_nDef[DEF_USHORT] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_USHORT] = SockData.nData1; // IDC_STC_DEF_U_SHORT
			}
			break;
		case _ItemInx::_DefNumLnW:
			if (pDoc->m_nDef[DEF_SPACE] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_SPACE] = SockData.nData1; // IDC_STC_DEF_SPACE
			}
			break;
		case _ItemInx::_DefExtr:
			if (pDoc->m_nDef[DEF_EXTRA] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_EXTRA] = SockData.nData1; // IDC_STC_DEF_EXTRA
			}
			break;
		case _ItemInx::_DefNumProt:
			if (pDoc->m_nDef[DEF_PROTRUSION] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_PROTRUSION] = SockData.nData1; // IDC_STC_DEF_PROT
			}
			break;
		case _ItemInx::_DefNumPhole:
			if (pDoc->m_nDef[DEF_PINHOLE] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_PINHOLE] = SockData.nData1; // IDC_STC_DEF_P_HOLE
			}
			break;
		case _ItemInx::_DefNumPad:
			if (pDoc->m_nDef[DEF_PAD] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_PAD] = SockData.nData1; // IDC_STC_DEF_PAD
			}
			break;
		case _ItemInx::_DefNumHopen:
			if (pDoc->m_nDef[DEF_HOLE_OPEN] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_HOLE_OPEN] = SockData.nData1; // IDC_STC_DEF_H_OPEN
			}
			break;
		case _ItemInx::_DefNumHmiss:
			if (pDoc->m_nDef[DEF_HOLE_MISS] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_HOLE_MISS] = SockData.nData1; // IDC_STC_DEF_H_MISS
			}
			break;
		case _ItemInx::_DefNumHpos:
			if (pDoc->m_nDef[DEF_HOLE_POSITION] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_HOLE_POSITION] = SockData.nData1; // IDC_STC_DEF_H_POS
			}
			break;
		case _ItemInx::_DefNumHdef:
			if (pDoc->m_nDef[DEF_HOLE_DEFECT] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_HOLE_DEFECT] = SockData.nData1; // IDC_STC_DEF_H_DEF
			}
			break;
		case _ItemInx::_DefNumNick:
			if (pDoc->m_nDef[DEF_NICK] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_NICK] = SockData.nData1; // IDC_STC_DEF_NICK
			}
			break;
		case _ItemInx::_DefNumPoi:
			if (pDoc->m_nDef[DEF_POI] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_POI] = SockData.nData1; // IDC_STC_DEF_POI
			}
			break;
		case _ItemInx::_DefNumVhOpen:
			if (pDoc->m_nDef[DEF_VH_OPEN] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_VH_OPEN] = SockData.nData1; // IDC_STC_DEF_VH_OPEN
			}
			break;
		case _ItemInx::_DefNumVhMiss:
			if (pDoc->m_nDef[DEF_VH_MISS] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_VH_MISS] = SockData.nData1; // IDC_STC_DEF_VH_MISS
			}
			break;
		case _ItemInx::_DefNumVhPos:
			if (pDoc->m_nDef[DEF_VH_POSITION] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_VH_POSITION] = SockData.nData1; // IDC_STC_DEF_VH_POS
			}
			break;
		case _ItemInx::_DefNumVhd:
			if (pDoc->m_nDef[DEF_VH_DEF] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_VH_DEF] = SockData.nData1; // IDC_STC_DEF_VH_DEF
			}
			break;
		case _ItemInx::_DefNumLight:
			if (pDoc->m_nDef[DEF_LIGHT] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_LIGHT] = SockData.nData1; // IDC_STC_DEF_LIGHT
			}
			break;
		case _ItemInx::_DefNumEnick:
			if (pDoc->m_nDef[DEF_EDGE_NICK] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_EDGE_NICK] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefNumEprot:
			if (pDoc->m_nDef[DEF_EDGE_PROT] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_EDGE_PROT] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefNumEspace:
			if (pDoc->m_nDef[DEF_EDGE_SPACE] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_EDGE_SPACE] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefNumUdd1:
			if (pDoc->m_nDef[DEF_USER_DEFINE_1] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_USER_DEFINE_1] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefNumNrw:
			if (pDoc->m_nDef[DEF_NARROW] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_NARROW] = SockData.nData1;
			}
			break;
		case _ItemInx::_DefNumWide:
			if (pDoc->m_nDef[DEF_WIDE] != SockData.nData1)
			{
				m_bGetInfo = TRUE;
				pDoc->m_nDef[DEF_WIDE] = SockData.nData1;
			}
			break;
		default:
			break;
		}
	}
}

void CEngrave::Get2DReader(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_2DEngLen:
			pDoc->WorkingInfo.Motion.s2DEngLen = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DAoiLen:
			pDoc->WorkingInfo.Motion.s2DAoiLen = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DMkLen:
			pDoc->WorkingInfo.Motion.s2DMkLen = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DMoveVel:
			pDoc->WorkingInfo.Motion.s2DMoveVel = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DMoveAcc:
			pDoc->WorkingInfo.Motion.s2DMoveAcc = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DOneShotLen:
			pDoc->WorkingInfo.Motion.s2DOneShotRemainLen = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetEngInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_EngBuffJogCw:
			pDoc->WorkingInfo.Motion.bEngBuffJogCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffJogCcw:
			pDoc->WorkingInfo.Motion.bEngBuffJogCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffOrgMv:
			pDoc->WorkingInfo.Motion.bEngBuffHomming = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffOrgMvDone:
			pDoc->WorkingInfo.Motion.bEngBuffHommingDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffInitPosMv:
			pDoc->WorkingInfo.Motion.bEngBuffInitMv = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffInitPosMvDone:
			pDoc->WorkingInfo.Motion.bEngBuffInitMvDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffInitPosSave:
			pDoc->WorkingInfo.Motion.bEngBuffInitPosSave = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_EngLeadPitch:
			pDoc->WorkingInfo.Motion.sEngraveFdLead = CharToString(SockData.strData);
			pDoc->SetEngraveFdPitch(_tstof(pDoc->WorkingInfo.Motion.sEngraveFdLead));
			break;
		case _ItemInx::_EngPushOffLen:
			pDoc->WorkingInfo.Motion.sEngraveFdVacOff = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngTqVal:
			pDoc->WorkingInfo.Motion.sEngraveTq = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngAoiLen:
			pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist = pDoc->WorkingInfo.Motion.sEngAoiLen = CharToString(SockData.strData);
			pDoc->SetEngraveAoiDist(_tstoi(pDoc->WorkingInfo.Motion.sEngAoiLen));
			break;
		case _ItemInx::_EngFdDiffMax:
			pDoc->WorkingInfo.Motion.sEngFdDiffMax = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngFdDiffRng:
			pDoc->WorkingInfo.Motion.sEngFdDiffRng = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngFdDiffNum:
			pDoc->WorkingInfo.Motion.sEngFdDiffNum = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngBuffInitPos:
			pDoc->WorkingInfo.Motion.sEngBuffInitPos = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngBuffCurrPos:
			pDoc->WorkingInfo.Motion.sEngBuffCurrPos = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetFdInfo(SOCKET_DATA SockData)
{
	long lData;

	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	
	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_FdVel:
			pDoc->WorkingInfo.Motion.sMkJogVel = pDoc->WorkingInfo.Motion.sAoiJogVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), pDoc->WorkingInfo.Motion.sMkJogVel, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), pDoc->WorkingInfo.Motion.sAoiJogVel, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkJogVel) * 1000.0);
			if(pView && pView->m_pMpe)
				pView->m_pMpe->Write(_T("ML45038"), lData);	// 연속공급 속도 (단위 mm/sec * 1000)
#endif
			break;
		case _ItemInx::_FdAcc:
			pDoc->WorkingInfo.Motion.sMkJogAcc = pDoc->WorkingInfo.Motion.sAoiJogAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), pDoc->WorkingInfo.Motion.sMkJogAcc, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), pDoc->WorkingInfo.Motion.sAoiJogAcc, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkJogAcc) * 1000.0);
			if (pView && pView->m_pMpe)
				pView->m_pMpe->Write(_T("ML45040"), lData);	// 연속공급 가속도 (단위 mm/s^2 * 1000)
#endif
			break;
		case _ItemInx::_OnePnlLen:
			pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_OnePnlVel:
			pDoc->WorkingInfo.Motion.sMkFdVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), pDoc->WorkingInfo.Motion.sMkFdVel, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), pDoc->WorkingInfo.Motion.sMkFdVel, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdVel) * 1000.0);
			if (pView && pView->m_pMpe)
				pView->m_pMpe->Write(_T("ML45034"), lData);	// 한 판넬 Feeding 속도 (단위 mm/sec * 1000)
#endif
			break;
		case _ItemInx::_OnePnlAcc:
			pDoc->WorkingInfo.Motion.sMkFdAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), pDoc->WorkingInfo.Motion.sMkFdAcc, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), pDoc->WorkingInfo.Motion.sMkFdAcc, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdAcc) * 1000.0);
			if (pView && pView->m_pMpe)
				pView->m_pMpe->Write(_T("ML45036"), lData);	// 한 판넬 Feeding 가속도 (단위 mm/s^2 * 1000)
#endif
			break;
		case _ItemInx::_FdDiffMax:
			pDoc->WorkingInfo.Motion.sLmtFdErr = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), pDoc->WorkingInfo.Motion.sLmtFdErr, PATH_WORKING_INFO);
			break;
		case _ItemInx::_FdDiffRng:
			pDoc->WorkingInfo.Motion.sLmtFdAdjOffSet = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), pDoc->WorkingInfo.Motion.sLmtFdAdjOffSet, PATH_WORKING_INFO);
			break;
		case _ItemInx::_FdDiffNum:
			pDoc->WorkingInfo.Motion.sLmtFdOvrNum = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), pDoc->WorkingInfo.Motion.sLmtFdOvrNum, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetAoiInfo(SOCKET_DATA SockData)
{
	long lData;

	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_AoiLeadPitch:
			pDoc->WorkingInfo.Motion.sAoiFdLead = CharToString(SockData.strData);

			pDoc->SetAoiFdPitch(_tstof(pDoc->WorkingInfo.Motion.sAoiFdLead));
			break;
		case _ItemInx::_AoiPushOffLen:
			pDoc->WorkingInfo.Motion.sAoiFdVacOff = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), pDoc->WorkingInfo.Motion.sAoiFdVacOff, PATH_WORKING_INFO);
			break;
		case _ItemInx::_AoiTqVal:
			pDoc->WorkingInfo.Motion.sAoiTq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), pDoc->WorkingInfo.Motion.sAoiTq, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sAoiTq) * 1000.0);
			if (pView && pView->m_pMpe)
				pView->m_pMpe->Write(_T("ML45042"), lData);	// 검사부 Tension 모터 토크값 (단위 Kgf * 1000)
#endif
			break;
		case _ItemInx::_AoiBuffShotNum:
			pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstoi(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * 1000);
			if (pView && pView->m_pMpe)
				pView->m_pMpe->Write(_T("ML45010"), lData);	// AOI(상)에서 AOI(하) Shot수 (단위 Shot수 * 1000)
#endif
			break;
		case _ItemInx::_AoiMkLen:
			pDoc->WorkingInfo.Motion.sFdMkAoiInitDist = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), pDoc->WorkingInfo.Motion.sFdMkAoiInitDist, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist) * 1000.0);
			if (pView && pView->m_pMpe)
				pView->m_pMpe->Write(_T("ML45008"), lData);	// AOI(하)에서 마킹까지 거리 (단위 mm * 1000)
#endif
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetMkInfo(SOCKET_DATA SockData)
{
	long lData;
	CString sData;

	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_MkLeadPitch:
			pDoc->WorkingInfo.Motion.sMkFdLead = CharToString(SockData.strData);

			pDoc->SetMkFdPitch(_tstof(pDoc->WorkingInfo.Motion.sMkFdLead));
			break;
		case _ItemInx::_MkPushOffLen:
			pDoc->WorkingInfo.Motion.sMkFdVacOff = CharToString(SockData.strData);
	
			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), pDoc->WorkingInfo.Motion.sMkFdVacOff, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkTqVal:
			pDoc->WorkingInfo.Motion.sMkTq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), pDoc->WorkingInfo.Motion.sMkTq, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkTq) * 1000.0);
			if (pView && pView->m_pMpe)
				pView->m_pMpe->Write(_T("ML45044"), lData);	// 마킹부 Tension 모터 토크값 (단위 Kgf * 1000)
#endif
			break;
		case _ItemInx::_MkBuffInitPos:
			pDoc->WorkingInfo.Motion.sStBufPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), pDoc->WorkingInfo.Motion.sStBufPos, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sStBufPos) * 1000.0);
			if (pView && pView->m_pMpe)
				pView->m_pMpe->Write(_T("ML45016"), lData);	// 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
#endif
			break;
		case _ItemInx::_MkBuffCurrPos:
			pDoc->m_dMkBuffCurrPos = (double)SockData.fData1;
			break;
		case _ItemInx::_MkNumLf:
			pDoc->WorkingInfo.Marking[0].nMkCnt = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[0].nMkCnt);
			::WritePrivateProfileString(_T("Marking0"), _T("Marking Count"), sData, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkNumRt:
			pDoc->WorkingInfo.Marking[1].nMkCnt = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[1].nMkCnt);
			::WritePrivateProfileString(_T("Marking1"), _T("Marking Count"), sData, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkMaxNumLf:
			pDoc->WorkingInfo.Marking[0].nMkLimit = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[0].nMkLimit);
			::WritePrivateProfileString(_T("Marking0"), _T("Marking Limit"), sData, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkMaxNumRt:
			pDoc->WorkingInfo.Marking[1].nMkLimit = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[1].nMkLimit);
			::WritePrivateProfileString(_T("Marking1"), _T("Marking Limit"), sData, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetMkInfoLf(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_MkInitPosLf:
			pDoc->WorkingInfo.Marking[0].sWaitPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_POS"), pDoc->WorkingInfo.Marking[0].sWaitPos, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
			break;
		case _ItemInx::_MkInitVelLf:
			pDoc->WorkingInfo.Marking[0].sWaitVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_VEL"), pDoc->WorkingInfo.Marking[0].sWaitVel, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
			break;
		case _ItemInx::_MkInitAccLf:
			pDoc->WorkingInfo.Marking[0].sWaitAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_ACC"), pDoc->WorkingInfo.Marking[0].sWaitAcc, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
			break;
		case _ItemInx::_MkFnlPosLf:
			pDoc->WorkingInfo.Marking[0].sMarkingPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_POS"), pDoc->WorkingInfo.Marking[0].sMarkingPos, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkFnlVelLf:
			pDoc->WorkingInfo.Marking[0].sMarkingVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_VEL"), pDoc->WorkingInfo.Marking[0].sMarkingVel, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkFnlAccLf:
			pDoc->WorkingInfo.Marking[0].sMarkingAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_ACC"), pDoc->WorkingInfo.Marking[0].sMarkingAcc, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkFnlTqLf:
			pDoc->WorkingInfo.Marking[0].sMarkingToq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ"), pDoc->WorkingInfo.Marking[0].sMarkingToq, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkHgtPosX1Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX1"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY1Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY1"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX2Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX2"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY2Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY2"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX3Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX3"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY3Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY3"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX4Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX4"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY4Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY4"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtAvgPosLf:
			pDoc->WorkingInfo.Marking[0].sAverDist = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), pDoc->WorkingInfo.Marking[0].sAverDist, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetMkInfoRt(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_MkInitPosRt:
			pDoc->WorkingInfo.Marking[1].sWaitPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_POS"), pDoc->WorkingInfo.Marking[1].sWaitPos, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
			break;
		case _ItemInx::_MkInitVelRt:
			pDoc->WorkingInfo.Marking[1].sWaitVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_VEL"), pDoc->WorkingInfo.Marking[1].sWaitVel, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
			break;
		case _ItemInx::_MkInitAccRt:
			pDoc->WorkingInfo.Marking[1].sWaitAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_ACC"), pDoc->WorkingInfo.Marking[1].sWaitAcc, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
			break;
		case _ItemInx::_MkFnlPosRt:
			pDoc->WorkingInfo.Marking[1].sMarkingPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_POS"), pDoc->WorkingInfo.Marking[1].sMarkingPos, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkFnlVelRt:
			pDoc->WorkingInfo.Marking[1].sMarkingVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_VEL"), pDoc->WorkingInfo.Marking[1].sMarkingVel, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkFnlAccRt:
			pDoc->WorkingInfo.Marking[1].sMarkingAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_ACC"), pDoc->WorkingInfo.Marking[1].sMarkingAcc, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkFnlTqRt:
			pDoc->WorkingInfo.Marking[1].sMarkingToq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ"), pDoc->WorkingInfo.Marking[1].sMarkingToq, PATH_WORKING_INFO);
			//pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkHgtPosX1Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX1"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY1Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY1"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX2Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX2"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY2Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY2"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX3Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX3"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY3Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY3"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX4Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX4"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY4Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY4"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtAvgPosRt:
			pDoc->WorkingInfo.Marking[1].sAverDist = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), pDoc->WorkingInfo.Marking[1].sAverDist, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetAlarmMsg(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _stAlarmInx::_Alarm:
			pDoc->m_sAlmMsg = CharToString(SockData.strData);
			IsSetAlarm(pDoc->m_sAlmMsg);
			break;
		case _stAlarmInx::_IsAlarm:
			;
			break;
		}
	}
}

void CEngrave::GetMsgBox(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _stMsgBoxInx::_MsgBox:
			pDoc->m_sMsgBox = CharToString(SockData.strData);
			pDoc->m_nTypeMsgBox = SockData.nData1;
			IsSetMsgBox(pDoc->m_sMsgBox);
			break;
		case _stMsgBoxInx::_IsMsgBox:
			pDoc->m_sIsMsgBox = CharToString(SockData.strData);
			break;
		}
	}
}
// End for GetSysData()

// Start for SetSysSignal()

BOOL CEngrave::SetSysSignal()
{
	if (!IsRunning())
	{
		SetSignalMain();
		SetSignalTorqueMotor();
		SetSignalInductionMotor();
		SetSignalCore150mm();
		//SetSignalEtc();
		SetSignalRecoiler();
		SetSignalPunch();
		SetSignalAOIDn();
		SetSignalAOIUp();
		SetSignalEngrave();
		SetSignalUncoiler();

		return TRUE;
	}

	return FALSE;
}

void CEngrave::SetSignalMain()
{
	if (pDoc->BtnStatus.Main.PrevReady != pDoc->BtnStatus.Main.Ready)
	{
		pDoc->BtnStatus.Main.PrevReady = pDoc->BtnStatus.Main.Ready;
		SwReady(pDoc->BtnStatus.Main.Ready);		// 마킹부 운전준비 스위치 "MB005503" IDC_CHK_34
	}
	if (pDoc->BtnStatus.Main.PrevRun != pDoc->BtnStatus.Main.Run)
	{
		pDoc->BtnStatus.Main.PrevRun = pDoc->BtnStatus.Main.Run;
		SwRun(pDoc->BtnStatus.Main.Run);			// 마킹부 운전 스위치 "MB005501" IDC_CHK_0
	}
	if (pDoc->BtnStatus.Main.PrevReset != pDoc->BtnStatus.Main.Reset)
	{
		pDoc->BtnStatus.Main.PrevReset = pDoc->BtnStatus.Main.Reset;
		SwReset(pDoc->BtnStatus.Main.Reset);		// 마킹부 리셋 스위치 "MB005504" IDC_CHK_1
	}
	if (pDoc->BtnStatus.Main.PrevStop != pDoc->BtnStatus.Main.Stop)
	{
		pDoc->BtnStatus.Main.PrevStop = pDoc->BtnStatus.Main.Stop;
		SwStop(pDoc->BtnStatus.Main.Stop);			// 마킹부 정지 스위치 "MB005502" IDC_CHK_33
	}
	if (pDoc->BtnStatus.Main.PrevAuto != pDoc->BtnStatus.Main.Auto)
	{
		pDoc->BtnStatus.Main.PrevAuto = pDoc->BtnStatus.Main.Auto;
		SwAuto(pDoc->BtnStatus.Main.Auto);			// 마킹부 자동 스위치 MB005505,	X4205	,	마킹부 자동/수동 (ON)
	}
	if (pDoc->BtnStatus.Main.PrevManual != pDoc->BtnStatus.Main.Manual)
	{
		pDoc->BtnStatus.Main.PrevManual = pDoc->BtnStatus.Main.Manual;
		SwManual(pDoc->BtnStatus.Main.Manual);		// 마킹부 수동 스위치 MB005505,	X4205	,	마킹부 자동/수동 (OFF)
	}
}

void CEngrave::SetSignalTorqueMotor()
{
	if (pDoc->BtnStatus.Tq.PrevMk != pDoc->BtnStatus.Tq.Mk)
	{
		pDoc->BtnStatus.Tq.PrevMk = pDoc->BtnStatus.Tq.Mk;
		SwMkTq(pDoc->BtnStatus.Tq.Mk); 				// 마킹부 텐션 ON (PC가 ON/OFF시킴) "MB440155" IDC_CHK_2
													// pDoc->WorkingInfo.Motion.bMkTq
	}
	if (pDoc->BtnStatus.Tq.PrevAoi != pDoc->BtnStatus.Tq.Aoi)
	{
		pDoc->BtnStatus.Tq.PrevAoi = pDoc->BtnStatus.Tq.Aoi;
		SwAoiTq(pDoc->BtnStatus.Tq.Aoi); 			// 검사부 텐션 ON (PC가 ON/OFF시킴) "MB440156" IDC_CHK_3
													// pDoc->WorkingInfo.Motion.bAoiTq
	}
	if (pDoc->BtnStatus.Tq.PrevEng != pDoc->BtnStatus.Tq.Eng)
	{
		pDoc->BtnStatus.Tq.PrevEng = pDoc->BtnStatus.Tq.Eng;
		SwEngTq(pDoc->BtnStatus.Tq.Eng); 			// 각인부 텐션 ON (PC가 ON/OFF시킴) "MB440154" IDC_CHK_84
													// pDoc->WorkingInfo.Motion.bEngraveTq
	}
}

void CEngrave::SetSignalInductionMotor()
{
	if (pDoc->BtnStatus.Induct.PrevRc != pDoc->BtnStatus.Induct.Rc)
	{
		pDoc->BtnStatus.Induct.PrevRc = pDoc->BtnStatus.Induct.Rc;
		SwRcInductionCcw(pDoc->BtnStatus.Induct.Rc);	// One Metal IDC_CHK_68		
														//pView->SetTwoMetal(FALSE, TRUE);
	}
	if (pDoc->BtnStatus.Induct.PrevUc != pDoc->BtnStatus.Induct.Uc)
	{
		pDoc->BtnStatus.Induct.PrevUc = pDoc->BtnStatus.Induct.Uc;
		SwUcInductionCcw(pDoc->BtnStatus.Induct.Uc);	// Two Metal IDC_CHK_69
														//pView->SetTwoMetal(TRUE, TRUE);
	}
}

void CEngrave::SetSignalCore150mm()
{
	if (pDoc->BtnStatus.Core150.PrevRc != pDoc->BtnStatus.Core150.Rc)
	{
		pDoc->BtnStatus.Core150.PrevRc = pDoc->BtnStatus.Core150.Rc;
		SwRcCore150mm(pDoc->BtnStatus.Core150.Rc);	// Recoiler IDC_CHK_70		
													//SetCore150mmRecoiler(TRUE);
	}
	if (pDoc->BtnStatus.Core150.PrevUc != pDoc->BtnStatus.Core150.Uc)
	{
		pDoc->BtnStatus.Core150.PrevUc = pDoc->BtnStatus.Core150.Uc;
		SwUcCore150mm(pDoc->BtnStatus.Core150.Uc);	// Uncoiler IDC_CHK_71		
													//SetCore150mmUncoiler(TRUE);
	}
}

void CEngrave::SetSignalEtc()
{
	//SwAoiEmg(pDoc->BtnStatus.Etc.EmgAoi);
}

void CEngrave::SetSignalRecoiler()
{
	if (pDoc->BtnStatus.Rc.PrevRelation != pDoc->BtnStatus.Rc.Relation)
	{
		pDoc->BtnStatus.Rc.PrevRelation = pDoc->BtnStatus.Rc.Relation;
		SwRcRelation(pDoc->BtnStatus.Rc.Relation);						// 리코일러 연동 온/오프 스위치 "MB005801" IDC_CHK_4
	}
	if (pDoc->BtnStatus.Rc.PrevFdCw != pDoc->BtnStatus.Rc.FdCw)
	{
		pDoc->BtnStatus.Rc.PrevFdCw = pDoc->BtnStatus.Rc.FdCw;
		SwRcFdCw(pDoc->BtnStatus.Rc.FdCw);								// 리코일러 제품휠 정회전 스위치 "MB00580C" IDC_CHK_5
	}
	if (pDoc->BtnStatus.Rc.PrevFdCcw != pDoc->BtnStatus.Rc.FdCcw)
	{
		pDoc->BtnStatus.Rc.PrevFdCcw = pDoc->BtnStatus.Rc.FdCcw;
		SwRcFdCcw(pDoc->BtnStatus.Rc.FdCcw);							// 리코일러 제품휠 역회전 스위치 "MB00580D" IDC_CHK_6
	}
	if (pDoc->BtnStatus.Rc.PrevReelChuck != pDoc->BtnStatus.Rc.ReelChuck)
	{
		pDoc->BtnStatus.Rc.PrevReelChuck = pDoc->BtnStatus.Rc.ReelChuck;
		SwRcReelChuck(pDoc->BtnStatus.Rc.ReelChuck);					// 리코일러 제품척 클램프 스위치 "MB00580B" IDC_CHK_41
	}
	if (pDoc->BtnStatus.Rc.PrevDcRlUpDn != pDoc->BtnStatus.Rc.DcRlUpDn)
	{
		pDoc->BtnStatus.Rc.PrevDcRlUpDn = pDoc->BtnStatus.Rc.DcRlUpDn;
		SwRcDcRlUpDn(pDoc->BtnStatus.Rc.DcRlUpDn);						// 리코일러 댄서롤 상승/하강 스위치 "MB005802" IDC_CHK_42
	}
	if (pDoc->BtnStatus.Rc.PrevReelJoinL != pDoc->BtnStatus.Rc.ReelJoinL)
	{
		pDoc->BtnStatus.Rc.PrevReelJoinL = pDoc->BtnStatus.Rc.ReelJoinL;
		SwRcReelJoinL(pDoc->BtnStatus.Rc.ReelJoinL);					// 리코일러 제품 이음매(상/좌) 스위치 "MB005805" IDC_CHK_43
	}
	if (pDoc->BtnStatus.Rc.PrevReelJoinR != pDoc->BtnStatus.Rc.ReelJoinR)
	{
		pDoc->BtnStatus.Rc.PrevReelJoinR = pDoc->BtnStatus.Rc.ReelJoinR;
		SwRcReelJoinR(pDoc->BtnStatus.Rc.ReelJoinR);					// 리코일러 제품 이음매(하/우) 스위치 "MB005806" IDC_CHK_7
	}
	if (pDoc->BtnStatus.Rc.PrevReelJoinVac != pDoc->BtnStatus.Rc.ReelJoinVac)
	{
		pDoc->BtnStatus.Rc.PrevReelJoinVac = pDoc->BtnStatus.Rc.ReelJoinVac;
		SwRcReelJoinVac(pDoc->BtnStatus.Rc.ReelJoinVac);				// 리코일러 제품 이음매 진공 스위치 "MB00580F" IDC_CHK_8
	}
	if (pDoc->BtnStatus.Rc.PrevPprChuck != pDoc->BtnStatus.Rc.PprChuck)
	{
		pDoc->BtnStatus.Rc.PrevPprChuck = pDoc->BtnStatus.Rc.PprChuck;
		SwRcPaperChuck(pDoc->BtnStatus.Rc.PprChuck);					// 리코일러 간지척 클램프 스위치 "MB005808" IDC_CHK_44
	}
	if (pDoc->BtnStatus.Rc.PrevPprCw != pDoc->BtnStatus.Rc.PprCw)
	{
		pDoc->BtnStatus.Rc.PrevPprCw = pDoc->BtnStatus.Rc.PprCw;
		SwRcPaperCw(pDoc->BtnStatus.Rc.PprCw);							// 리코일러 간지휠 정회전 스위치 "MB005809" IDC_CHK_45
	}
	if (pDoc->BtnStatus.Rc.PrevPprCcw != pDoc->BtnStatus.Rc.PprCcw)
	{
		pDoc->BtnStatus.Rc.PrevPprCcw = pDoc->BtnStatus.Rc.PprCcw;
		SwRcPaperCcw(pDoc->BtnStatus.Rc.PprCcw);						// 리코일러 간지휠 역회전 스위치 "MB00580A" IDC_CHK_46
	}
	if (pDoc->BtnStatus.Rc.PrevRewine != pDoc->BtnStatus.Rc.Rewine)
	{
		pDoc->BtnStatus.Rc.PrevRewine = pDoc->BtnStatus.Rc.Rewine;
		SwRcRewinder(pDoc->BtnStatus.Rc.Rewine);						// 리코일러 Rewinder 동작 스위치 "MB005803" IDC_CHK_66
	}
	if (pDoc->BtnStatus.Rc.PrevRewineReelPpr != pDoc->BtnStatus.Rc.RewineReelPpr)
	{
		pDoc->BtnStatus.Rc.PrevRewineReelPpr = pDoc->BtnStatus.Rc.RewineReelPpr;
		SwRcRewinderReelPaper(pDoc->BtnStatus.Rc.RewineReelPpr);		// 리코일러 Rewinder 제품 & 간지 스위치 "MB005804" IDC_CHK_67
	}
}

void CEngrave::SetSignalPunch()
{
	if (pDoc->BtnStatus.Mk.PrevRelation != pDoc->BtnStatus.Mk.Relation)
	{
		pDoc->BtnStatus.Mk.PrevRelation = pDoc->BtnStatus.Mk.Relation;
		SwMkRelation(pDoc->BtnStatus.Mk.Relation);			// 마킹부 연동 온/오프 스위치 "MB005511" IDC_CHK_9
	}
	if (pDoc->BtnStatus.Mk.PrevFdCw != pDoc->BtnStatus.Mk.FdCw)
	{
		pDoc->BtnStatus.Mk.PrevFdCw = pDoc->BtnStatus.Mk.FdCw;
		SwMkFdCw(pDoc->BtnStatus.Mk.FdCw);					// 마킹부 피딩 정회전 스위치 "MB005513" IDC_CHK_10
	}
	if (pDoc->BtnStatus.Mk.PrevFdCcw != pDoc->BtnStatus.Mk.FdCcw)
	{
		pDoc->BtnStatus.Mk.PrevFdCcw = pDoc->BtnStatus.Mk.FdCcw;
		SwMkFdCcw(pDoc->BtnStatus.Mk.FdCcw);				// 마킹부 피딩 역회전 스위치 "MB005514" IDC_CHK_11
	}
	if (pDoc->BtnStatus.Mk.PrevFdVac != pDoc->BtnStatus.Mk.FdVac)
	{
		pDoc->BtnStatus.Mk.PrevFdVac = pDoc->BtnStatus.Mk.FdVac;
		SwMkFdVac(pDoc->BtnStatus.Mk.FdVac);				// 마킹부 피딩 진공 스위치 "MB005515" IDC_CHK_12
	}
	if (pDoc->BtnStatus.Mk.PrevPushUp != pDoc->BtnStatus.Mk.PushUp)
	{
		pDoc->BtnStatus.Mk.PrevPushUp = pDoc->BtnStatus.Mk.PushUp;
		SwMkPushUp(pDoc->BtnStatus.Mk.PushUp);				// 마킹부 제품푸쉬 스위치 "MB005516" // (토크 진공 스위치) - X IDC_CHK_13
	}
	if (pDoc->BtnStatus.Mk.PrevTblBlw != pDoc->BtnStatus.Mk.TblBlw)
	{
		pDoc->BtnStatus.Mk.PrevTblBlw = pDoc->BtnStatus.Mk.TblBlw;
		SwMkTblBlw(pDoc->BtnStatus.Mk.TblBlw);				// 마킹부 테이블 브로워 스위치 "MB005512" IDC_CHK_14
	}
	if (pDoc->BtnStatus.Mk.PrevTblVac != pDoc->BtnStatus.Mk.TblVac)
	{
		pDoc->BtnStatus.Mk.PrevTblVac = pDoc->BtnStatus.Mk.TblVac;
		SwMkTblVac(pDoc->BtnStatus.Mk.TblVac);				// 마킹부 테이블 진공 스위치 "MB005517" IDC_CHK_15
	}
	if (pDoc->BtnStatus.Mk.PrevFdClp != pDoc->BtnStatus.Mk.FdClp)
	{
		pDoc->BtnStatus.Mk.PrevFdClp = pDoc->BtnStatus.Mk.FdClp;
		SwMkFdClp(pDoc->BtnStatus.Mk.FdClp);				// 마킹부 피딩 클램프 스위치 "MB005519" IDC_CHK_51
	}
	if (pDoc->BtnStatus.Mk.PrevTqClp != pDoc->BtnStatus.Mk.TqClp)
	{
		pDoc->BtnStatus.Mk.PrevTqClp = pDoc->BtnStatus.Mk.TqClp;
		SwMkTqClp(pDoc->BtnStatus.Mk.TqClp);				// 마킹부 텐션 클램프 스위치 "MB00551A" IDC_CHK_52
	}
	if (pDoc->BtnStatus.Mk.PrevMvOne != pDoc->BtnStatus.Mk.MvOne)
	{
		pDoc->BtnStatus.Mk.PrevMvOne = pDoc->BtnStatus.Mk.MvOne;
		SwMkMvOne(pDoc->BtnStatus.Mk.MvOne);				// 마킹부 한판넬 이송 스위치 "MB440151" IDC_CHK_16
	}
	if (pDoc->BtnStatus.Mk.PrevLsrPt != pDoc->BtnStatus.Mk.LsrPt)
	{
		pDoc->BtnStatus.Mk.PrevLsrPt = pDoc->BtnStatus.Mk.LsrPt;
		SwMkLsrPt(pDoc->BtnStatus.Mk.LsrPt);				// 마킹부 레이져마크 스위치 "MB005518" IDC_CHK_49
	}
	if (pDoc->BtnStatus.Mk.PrevDcRSol != pDoc->BtnStatus.Mk.DcRSol)
	{
		pDoc->BtnStatus.Mk.PrevDcRSol = pDoc->BtnStatus.Mk.DcRSol;
		SwMkDcRSol(pDoc->BtnStatus.Mk.DcRSol);				// 마킹부 댄서롤 상승/하강 스위치 "MB00551B", "X421B" IDC_CHK_48
	}
}

void CEngrave::SetSignalAOIDn()
{
	if (pDoc->BtnStatus.AoiDn.PrevRelation != pDoc->BtnStatus.AoiDn.Relation)
	{
		pDoc->BtnStatus.AoiDn.PrevRelation = pDoc->BtnStatus.AoiDn.Relation;
		SwAoiDnRelation(pDoc->BtnStatus.AoiDn.Relation);			// 검사부 하 연동 온/오프 스위치 "MB005701" IDC_CHK_55
	}
	if (pDoc->BtnStatus.AoiDn.PrevFdCw != pDoc->BtnStatus.AoiDn.FdCw)
	{
		pDoc->BtnStatus.AoiDn.PrevFdCw = pDoc->BtnStatus.AoiDn.FdCw;
		SwAoiDnFdCw(pDoc->BtnStatus.AoiDn.FdCw);					// 검사부 하 피딩 정회전 스위치 "MB005703" IDC_CHK_56
	}
	if (pDoc->BtnStatus.AoiDn.PrevFdCcw != pDoc->BtnStatus.AoiDn.FdCcw)
	{
		pDoc->BtnStatus.AoiDn.PrevFdCcw = pDoc->BtnStatus.AoiDn.FdCcw;
		SwAoiDnFdCcw(pDoc->BtnStatus.AoiDn.FdCcw);					// 검사부 하 피딩 역회전 스위치 "MB005704" IDC_CHK_57
	}
	if (pDoc->BtnStatus.AoiDn.PrevFdVac != pDoc->BtnStatus.AoiDn.FdVac)
	{
		pDoc->BtnStatus.AoiDn.PrevFdVac = pDoc->BtnStatus.AoiDn.FdVac;
		SwAoiDnFdVac(pDoc->BtnStatus.AoiDn.FdVac);					// 검사부 하 피딩 진공 스위치 "MB005705" IDC_CHK_58
	}
	if (pDoc->BtnStatus.AoiDn.PrevPushUp != pDoc->BtnStatus.AoiDn.PushUp)
	{
		pDoc->BtnStatus.AoiDn.PrevPushUp = pDoc->BtnStatus.AoiDn.PushUp;
		SwAoiDnPushUp(pDoc->BtnStatus.AoiDn.PushUp);				// 검사부 하 제품푸쉬 스위치 "MB005706" IDC_CHK_59 // (토크 진공 스위치) - X
	}
	if (pDoc->BtnStatus.AoiDn.PrevTblBlw != pDoc->BtnStatus.AoiDn.TblBlw)
	{
		pDoc->BtnStatus.AoiDn.PrevTblBlw = pDoc->BtnStatus.AoiDn.TblBlw;
		SwAoiDnTblBlw(pDoc->BtnStatus.AoiDn.TblBlw);				// 검사부 하 테이블 브로워 스위치 "MB005702" IDC_CHK_60
	}
	if (pDoc->BtnStatus.AoiDn.PrevTblVac != pDoc->BtnStatus.AoiDn.TblVac)
	{
		pDoc->BtnStatus.AoiDn.PrevTblVac = pDoc->BtnStatus.AoiDn.TblVac;
		SwAoiDnTblVac(pDoc->BtnStatus.AoiDn.TblVac);				// 검사부 하 테이블 진공 스위치 "MB005707" IDC_CHK_61
	}
	if (pDoc->BtnStatus.AoiDn.PrevFdClp != pDoc->BtnStatus.AoiDn.FdClp)
	{
		pDoc->BtnStatus.AoiDn.PrevFdClp = pDoc->BtnStatus.AoiDn.FdClp;
		SwAoiDnFdClp(pDoc->BtnStatus.AoiDn.FdClp);					// 검사부 하 피딩 클램프 스위치 "MB005709" IDC_CHK_64
	}
	if (pDoc->BtnStatus.AoiDn.PrevTqClp != pDoc->BtnStatus.AoiDn.TqClp)
	{
		pDoc->BtnStatus.AoiDn.PrevTqClp = pDoc->BtnStatus.AoiDn.TqClp;
		SwAoiDnTqClp(pDoc->BtnStatus.AoiDn.TqClp);					// 검사부 하 텐션 클램프 스위치 "MB00570A" IDC_CHK_65
	}
	if (pDoc->BtnStatus.AoiDn.PrevMvOne != pDoc->BtnStatus.AoiDn.MvOne)
	{
		pDoc->BtnStatus.AoiDn.PrevMvOne = pDoc->BtnStatus.AoiDn.MvOne;
		SwAoiDnMvOne(pDoc->BtnStatus.AoiDn.MvOne);					// 검사부 하 한판넬 이송 스위치 "MB440151" IDC_CHK_62
	}
	if (pDoc->BtnStatus.AoiDn.PrevLsrPt != pDoc->BtnStatus.AoiDn.LsrPt)
	{
		pDoc->BtnStatus.AoiDn.PrevLsrPt = pDoc->BtnStatus.AoiDn.LsrPt;
		SwAoiDnLsrPt(pDoc->BtnStatus.AoiDn.LsrPt);					// 검사부 하 레이져마크 스위치 "MB005708" IDC_CHK_63
	}
	if (pDoc->BtnStatus.AoiDn.PrevVelSonicBlw != pDoc->BtnStatus.AoiDn.VelSonicBlw)
	{
		pDoc->BtnStatus.AoiDn.PrevVelSonicBlw = pDoc->BtnStatus.AoiDn.VelSonicBlw;
		SwAoiDnVelSonicBlw(pDoc->BtnStatus.AoiDn.VelSonicBlw);		// 검사부 하 초음파 세정기 속도 스위치 "MB44014F"  IDC_CHK_88 // pDoc->WorkingInfo.LastJob.bAoiDnCleanner
	}
}

void CEngrave::SetSignalAOIUp()
{
	if (pDoc->BtnStatus.AoiUp.PrevRelation != pDoc->BtnStatus.AoiUp.Relation)
	{
		pDoc->BtnStatus.AoiUp.PrevRelation = pDoc->BtnStatus.AoiUp.Relation;
		SwAoiUpRelation(pDoc->BtnStatus.AoiUp.Relation);	// 검사부 상 연동 온/오프 스위치 "MB005601" IDC_CHK_17
	}
	if (pDoc->BtnStatus.AoiUp.PrevFdCw != pDoc->BtnStatus.AoiUp.FdCw)
	{
		pDoc->BtnStatus.AoiUp.PrevFdCw = pDoc->BtnStatus.AoiUp.FdCw;
		SwAoiUpFdCw(pDoc->BtnStatus.AoiUp.FdCw);			// 검사부 상 피딩 정회전 스위치 "MB005603" IDC_CHK_18
	}
	if (pDoc->BtnStatus.AoiUp.PrevFdCcw != pDoc->BtnStatus.AoiUp.FdCcw)
	{
		pDoc->BtnStatus.AoiUp.PrevFdCcw = pDoc->BtnStatus.AoiUp.FdCcw;
		SwAoiUpFdCcw(pDoc->BtnStatus.AoiUp.FdCcw);			// 검사부 상 피딩 역회전 스위치 "MB005604" IDC_CHK_19
	}
	if (pDoc->BtnStatus.AoiUp.PrevFdVac != pDoc->BtnStatus.AoiUp.FdVac)
	{
		pDoc->BtnStatus.AoiUp.PrevFdVac = pDoc->BtnStatus.AoiUp.FdVac;
		SwAoiUpFdVac(pDoc->BtnStatus.AoiUp.FdVac);			// 검사부 상 피딩 진공 스위치 "MB005605" IDC_CHK_20
	}
	if (pDoc->BtnStatus.AoiUp.PrevPushUp != pDoc->BtnStatus.AoiUp.PushUp)
	{
		pDoc->BtnStatus.AoiUp.PrevPushUp = pDoc->BtnStatus.AoiUp.PushUp;
		SwAoiUpPushUp(pDoc->BtnStatus.AoiUp.PushUp);		// 검사부 상 제품푸쉬 스위치 "MB005606" IDC_CHK_21 // (토크 진공 스위치) - X
	}
	if (pDoc->BtnStatus.AoiUp.PrevTblBlw != pDoc->BtnStatus.AoiUp.TblBlw)
	{
		pDoc->BtnStatus.AoiUp.PrevTblBlw = pDoc->BtnStatus.AoiUp.TblBlw;
		SwAoiUpTblBlw(pDoc->BtnStatus.AoiUp.TblBlw);		// 검사부 상 테이블 브로워 스위치 "MB005602" IDC_CHK_22
	}
	if (pDoc->BtnStatus.AoiUp.PrevTblVac != pDoc->BtnStatus.AoiUp.TblVac)
	{
		pDoc->BtnStatus.AoiUp.PrevTblVac = pDoc->BtnStatus.AoiUp.TblVac;
		SwAoiUpTblVac(pDoc->BtnStatus.AoiUp.TblVac);		// 검사부 상 테이블 진공 스위치 "MB005607" IDC_CHK_23
	}
	if (pDoc->BtnStatus.AoiUp.PrevFdClp != pDoc->BtnStatus.AoiUp.FdClp)
	{
		pDoc->BtnStatus.AoiUp.PrevFdClp = pDoc->BtnStatus.AoiUp.FdClp;
		SwAoiUpFdClp(pDoc->BtnStatus.AoiUp.FdClp);			// 검사부 상 피딩 클램프 스위치 "MB005609" IDC_CHK_53
	}
	if (pDoc->BtnStatus.AoiUp.PrevTqClp != pDoc->BtnStatus.AoiUp.TqClp)
	{
		pDoc->BtnStatus.AoiUp.PrevTqClp = pDoc->BtnStatus.AoiUp.TqClp;
		SwAoiUpTqClp(pDoc->BtnStatus.AoiUp.TqClp);			// 검사부 상 텐션 클램프 스위치 "MB00560A" IDC_CHK_54
	}
	if (pDoc->BtnStatus.AoiUp.PrevMvOne != pDoc->BtnStatus.AoiUp.MvOne)
	{
		pDoc->BtnStatus.AoiUp.PrevMvOne = pDoc->BtnStatus.AoiUp.MvOne;
		SwAoiUpMvOne(pDoc->BtnStatus.AoiUp.MvOne);			// 검사부 상 한판넬 이송 스위치  "MB440151" IDC_CHK_24
	}
	if (pDoc->BtnStatus.AoiUp.PrevLsrPt != pDoc->BtnStatus.AoiUp.LsrPt)
	{
		pDoc->BtnStatus.AoiUp.PrevLsrPt = pDoc->BtnStatus.AoiUp.LsrPt;
		SwAoiUpLsrPt(pDoc->BtnStatus.AoiUp.LsrPt);			// 검사부 상 레이져마크 스위치 "MB005608" IDC_CHK_50
	}
}

void CEngrave::SetSignalEngrave()
{
	if (pDoc->BtnStatus.Eng.PrevRelation != pDoc->BtnStatus.Eng.Relation)
	{
		pDoc->BtnStatus.Eng.PrevRelation = pDoc->BtnStatus.Eng.Relation;
		SwEngRelation(pDoc->BtnStatus.Eng.Relation);		// 각인부 연동 온/오프 스위치 IDC_CHK_72
	}
	if (pDoc->BtnStatus.Eng.PrevFdCw != pDoc->BtnStatus.Eng.FdCw)
	{
		pDoc->BtnStatus.Eng.PrevFdCw = pDoc->BtnStatus.Eng.FdCw;
		SwEngFdCw(pDoc->BtnStatus.Eng.FdCw);				// 각인부 피딩 정회전 스위치 IDC_CHK_73
	}
	if (pDoc->BtnStatus.Eng.PrevFdCcw != pDoc->BtnStatus.Eng.FdCcw)
	{
		pDoc->BtnStatus.Eng.PrevFdCcw = pDoc->BtnStatus.Eng.FdCcw;
		SwEngFdCcw(pDoc->BtnStatus.Eng.FdCcw);				// 각인부 피딩 역회전 스위치 IDC_CHK_74
	}
	if (pDoc->BtnStatus.Eng.PrevFdVac != pDoc->BtnStatus.Eng.FdVac)
	{
		pDoc->BtnStatus.Eng.PrevFdVac = pDoc->BtnStatus.Eng.FdVac;
		SwEngFdVac(pDoc->BtnStatus.Eng.FdVac);				// 각인부 피딩 진공 스위치 IDC_CHK_75
	}
	if (pDoc->BtnStatus.Eng.PrevPushUp != pDoc->BtnStatus.Eng.PushUp)
	{
		pDoc->BtnStatus.Eng.PrevPushUp = pDoc->BtnStatus.Eng.PushUp;
		SwEngPushUp(pDoc->BtnStatus.Eng.PushUp);			// 각인부 제품푸쉬 스위치 IDC_CHK_76 // (토크 진공 스위치) - X
	}
	if (pDoc->BtnStatus.Eng.PrevTblBlw != pDoc->BtnStatus.Eng.TblBlw)
	{
		pDoc->BtnStatus.Eng.PrevTblBlw = pDoc->BtnStatus.Eng.TblBlw;
		SwEngTblBlw(pDoc->BtnStatus.Eng.TblBlw);			// 각인부 테이블 브로워 스위치 IDC_CHK_77
	}
	if (pDoc->BtnStatus.Eng.PrevTblVac != pDoc->BtnStatus.Eng.TblVac)
	{
		pDoc->BtnStatus.Eng.PrevTblVac = pDoc->BtnStatus.Eng.TblVac;
		SwEngTblVac(pDoc->BtnStatus.Eng.TblVac);			// 각인부 테이블 진공 스위치 IDC_CHK_78
	}
	if (pDoc->BtnStatus.Eng.PrevFdClp != pDoc->BtnStatus.Eng.FdClp)
	{
		pDoc->BtnStatus.Eng.PrevFdClp = pDoc->BtnStatus.Eng.FdClp;
		SwEngFdClp(pDoc->BtnStatus.Eng.FdClp);				// 각인부 피딩 클램프 스위치 IDC_CHK_82
	}
	if (pDoc->BtnStatus.Eng.PrevTqClp != pDoc->BtnStatus.Eng.TqClp)
	{
		pDoc->BtnStatus.Eng.PrevTqClp = pDoc->BtnStatus.Eng.TqClp;
		SwEngTqClp(pDoc->BtnStatus.Eng.TqClp);				// 각인부 텐션 클램프 스위치 IDC_CHK_83
	}
	if (pDoc->BtnStatus.Eng.PrevMvOne != pDoc->BtnStatus.Eng.MvOne)
	{
		pDoc->BtnStatus.Eng.PrevMvOne = pDoc->BtnStatus.Eng.MvOne;
		SwEngMvOne(pDoc->BtnStatus.Eng.MvOne);				// 각인부 한판넬 이송 스위치  "MB440151" IDC_CHK_79
	}
	if (pDoc->BtnStatus.Eng.PrevLsrPt != pDoc->BtnStatus.Eng.LsrPt)
	{
		pDoc->BtnStatus.Eng.PrevLsrPt = pDoc->BtnStatus.Eng.LsrPt;
		SwEngLsrPt(pDoc->BtnStatus.Eng.LsrPt);				// 각인부 레이져마크 스위치 "" IDC_CHK_81
	}
	if (pDoc->BtnStatus.Eng.PrevVelSonicBlw != pDoc->BtnStatus.Eng.VelSonicBlw)
	{
		pDoc->BtnStatus.Eng.PrevVelSonicBlw = pDoc->BtnStatus.Eng.VelSonicBlw;
		SwEngVelSonicBlw(pDoc->BtnStatus.Eng.VelSonicBlw);		// 각인부 초음파 세정기 스위치 "MB44014E" IDC_CHK_87 pDoc->WorkingInfo.LastJob.bEngraveCleanner
	}
	if (pDoc->BtnStatus.Eng.PrevDcRSol != pDoc->BtnStatus.Eng.DcRSol)
	{
		pDoc->BtnStatus.Eng.PrevDcRSol = pDoc->BtnStatus.Eng.DcRSol;
		SwEngDcRSol(pDoc->BtnStatus.Eng.DcRSol);			// 각인부 댄서롤 상승/하강 스위치 IDC_CHK_80
	}
}

void CEngrave::SetSignalUncoiler()
{
	if (pDoc->BtnStatus.Uc.PrevRelation != pDoc->BtnStatus.Uc.Relation)
	{
		pDoc->BtnStatus.Uc.PrevRelation = pDoc->BtnStatus.Uc.Relation;
		SwUcRelation(pDoc->BtnStatus.Uc.Relation);			// 언코일러 연동 온/오프 스위치 "MB005401" IDC_CHK_25
	}
	if (pDoc->BtnStatus.Uc.PrevFdCw != pDoc->BtnStatus.Uc.FdCw)
	{
		pDoc->BtnStatus.Uc.PrevFdCw = pDoc->BtnStatus.Uc.FdCw;
		SwUcFdCw(pDoc->BtnStatus.Uc.FdCw);					// 언코일러 제품휠 정회전 스위치 "MB00540C" IDC_CHK_26
	}
	if (pDoc->BtnStatus.Uc.PrevFdCcw != pDoc->BtnStatus.Uc.FdCcw)
	{
		pDoc->BtnStatus.Uc.PrevFdCcw = pDoc->BtnStatus.Uc.FdCcw;
		SwUcFdCcw(pDoc->BtnStatus.Uc.FdCcw);				// 언코일러 제품휠 역회전 스위치 "MB00540D" IDC_CHK_27
	}
	if (pDoc->BtnStatus.Uc.PrevReelChuck != pDoc->BtnStatus.Uc.ReelChuck)
	{
		pDoc->BtnStatus.Uc.PrevReelChuck = pDoc->BtnStatus.Uc.ReelChuck;
		SwUcReelChuck(pDoc->BtnStatus.Uc.ReelChuck);		// 언코일러 제품척 클램프 스위치 "MB00540B" IDC_CHK_35
	}
	if (pDoc->BtnStatus.Uc.PrevDcRlUpDn != pDoc->BtnStatus.Uc.DcRlUpDn)
	{
		pDoc->BtnStatus.Uc.PrevDcRlUpDn = pDoc->BtnStatus.Uc.DcRlUpDn;
		SwUcDcRlUpDn(pDoc->BtnStatus.Uc.DcRlUpDn);			// 언코일러 댄서롤 상승/하강 스위치 "MB005402" IDC_CHK_28
	}
	if (pDoc->BtnStatus.Uc.PrevReelJoinL != pDoc->BtnStatus.Uc.ReelJoinL)
	{
		pDoc->BtnStatus.Uc.PrevReelJoinL = pDoc->BtnStatus.Uc.ReelJoinL;
		SwUcReelJoinL(pDoc->BtnStatus.Uc.ReelJoinL);		// 언코일러 제품 이음매(상/좌) 스위치 "MB005405" IDC_CHK_30
	}
	if (pDoc->BtnStatus.Uc.PrevReelJoinR != pDoc->BtnStatus.Uc.ReelJoinR)
	{
		pDoc->BtnStatus.Uc.PrevReelJoinR = pDoc->BtnStatus.Uc.ReelJoinR;
		SwUcReelJoinR(pDoc->BtnStatus.Uc.ReelJoinR);		// 언코일러 제품 이음매(하/우) 스위치 "MB005406" IDC_CHK_37
	}
	if (pDoc->BtnStatus.Uc.PrevReelJoinVac != pDoc->BtnStatus.Uc.ReelJoinVac)
	{
		pDoc->BtnStatus.Uc.PrevReelJoinVac = pDoc->BtnStatus.Uc.ReelJoinVac;
		SwUcReelJoinVac(pDoc->BtnStatus.Uc.ReelJoinVac);	// 언코일러 제품 이음매 진공 스위치 "MB00540F" IDC_CHK_38
	}
	if (pDoc->BtnStatus.Uc.PrevPprChuck != pDoc->BtnStatus.Uc.PprChuck)
	{
		pDoc->BtnStatus.Uc.PrevPprChuck = pDoc->BtnStatus.Uc.PprChuck;
		SwUcPaperChuck(pDoc->BtnStatus.Uc.PprChuck);		// 언코일러 간지척 클램프 스위치 "MB005408" IDC_CHK_31
	}
	if (pDoc->BtnStatus.Uc.PrevPprCw != pDoc->BtnStatus.Uc.PprCw)
	{
		pDoc->BtnStatus.Uc.PrevPprCw = pDoc->BtnStatus.Uc.PprCw;
		SwUcPaperCw(pDoc->BtnStatus.Uc.PprCw);				// 언코일러 간지휠 정회전 스위치 "MB005409" IDC_CHK_32
	}
	if (pDoc->BtnStatus.Uc.PrevPprCcw != pDoc->BtnStatus.Uc.PprCcw)
	{
		pDoc->BtnStatus.Uc.PrevPprCcw = pDoc->BtnStatus.Uc.PprCcw;
		SwUcPaperCcw(pDoc->BtnStatus.Uc.PprCcw);			// 언코일러 간지휠 역회전 스위치 "MB00540A" IDC_CHK_39
	}
	if (pDoc->BtnStatus.Uc.PrevClRlUpDn != pDoc->BtnStatus.Uc.ClRlUpDn)
	{
		pDoc->BtnStatus.Uc.PrevClRlUpDn = pDoc->BtnStatus.Uc.ClRlUpDn;
		SwUcClRlUpDn(pDoc->BtnStatus.Uc.ClRlUpDn);			// 언코일러 클린롤러 상승/하강 스위치 "MB005403" IDC_CHK_29
	}
	if (pDoc->BtnStatus.Uc.PrevClRlPshUpDn != pDoc->BtnStatus.Uc.ClRlPshUpDn)
	{
		pDoc->BtnStatus.Uc.PrevClRlPshUpDn = pDoc->BtnStatus.Uc.ClRlPshUpDn;
		SwUcClRlPshUpDn(pDoc->BtnStatus.Uc.ClRlPshUpDn);	// 언코일러 클린롤러누름 상승/하강 스위치 "MB005404" IDC_CHK_36
	}
}

// End SetSysSignal()

// On Running Auto

BOOL CEngrave::UpdateWorking()
{
	SetTotOpRto();		// 전체진행율
						// 로트진행율
	SetTotVel();		// 전체속도
	SetPartVel();		// 구간속도
	SetMkDoneLen();		// 마킹부 : Distance (FdDone) [M]
	SetAoiDnDoneLen();	// 검사부(하) : Distance (FdDone) [M]
	SetAoiUpDoneLen();	// 검사부(상) : Distance (FdDone) [M]
						// 각인부 : Distance (FdDone) [M]

	return TRUE;
}

BOOL CEngrave::UpdateRst()
{
	UpdateTotRatio();
	UpdateStripRatio();

	return TRUE;
}

BOOL CEngrave::UpdateTotRatio()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	// 상면
	SetDefNumUp();			// IDC_STC_DEFECT_NUM
	SetDefRtoUp();			// IDC_STC_DEFECT_RATIO
	SetGoodNumUp();			// IDC_STC_GOOD_NUM
	SetGoodRtoUp();			// IDC_STC_GOOD_RATIO
	SetTestNumUp();			// IDC_STC_TOTAL_NUM

	if (bDualTest)
	{
		// 하면
		SetDefNumDn();		// IDC_STC_DEFECT_NUM_DN
		SetDefRtoDn();		// IDC_STC_DEFECT_RATIO_DN
		SetGoodNumDn();		// IDC_STC_GOOD_NUM_DN
		SetGoodRtoDn();		// IDC_STC_GOOD_RATIO_DN
		SetTestNumDn();		// IDC_STC_TOTAL_NUM_DN

							// 전체
		SetDefNumTot();		// IDC_STC_DEFECT_NUM_ALL
		SetDefRtoTot();		// IDC_STC_DEFECT_RATIO_ALL
		SetGoodNumTot();	// IDC_STC_GOOD_NUM_ALL
		SetGoodRtoTot();	// IDC_STC_GOOD_RATIO_ALL
		SetTestNumTot();	// IDC_STC_TOTAL_NUM_ALL
	}

	return TRUE;
}

BOOL CEngrave::UpdateStripRatio()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	return TRUE;
}

BOOL CEngrave::UpdateDef()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	return TRUE;
}

// Start for SetSysData()

BOOL CEngrave::SetSysData()
{
	if (!IsRunning())
	{
		SetOpInfo();
		SetInfo();
		SetTotRatio();
		SetStTime();
		SetRunTime();
		SetEdTime();
		SetStripRatio();
		SetDef();
		Set2DReader();
		SetEngInfo();
		SetFdInfo();
		SetAoiInfo();
		SetMkInfo();
		SetMkInfoLf();
		SetMkInfoRt();

		return TRUE;
	}

	return FALSE;
}

void CEngrave::SetOpInfo()
{
	SetOpName();
	SetDualTest();
	SetSampleTest();
	SetSampleShotNum();
	SetTestMode();
	SetRecoilerCcw();
	SetUncoilerCcw();
	SetAlignMethode();
	SetDoorRecoiler();
	SetDoorAoiUp();
	SetDoorAoiDn();
	SetDoorMk();
	SetDoorEngrave();
	SetDoorUncoiler();
	SetSaftyMk();
	SetCleannerAoiUp();
	SetCleannerAoiDn();
	SetUltraSonicAoiDn();
	SetUltraSonicEngrave();
	SetTotReelLen();
	SetOnePnlLen();
	SetTempPause();
	SetTempStopLen();
	SetLotCut();
	SetLotCutLen();
	SetLotCutPosLen();
	SetLmtTotYld();
	SetLmtPatlYld();
	SetStripOutRatio();
	SetCustomNeedRatio();
	SetNumRangeFixDef();
	SetFixDef();
	SetNumContFixDef();
	SetUltraSonicStTim();
	SetEngItsCode();
}

void CEngrave::SetInfo()
{
	SetModelUpName();
	//SetModelDnName();
	SetLotUpName();
	SetLotDnName();
	SetLayerUpName();
	SetLayerDnName();
	//SetOrderNum();
	SetShotNum();
	SetTotOpRto();
	SetTotVel();
	SetPartVel();
	SetMkDoneLen();
	SetAoiDnDoneLen();
	SetAoiUpDoneLen();
	SetLotSerial();
	SetMkVerfyLen();
}

void CEngrave::SetTotRatio()
{
	SetDefNumUp();
	SetDefRtoUp();
	SetGoodNumUp();
	SetGoodRtoUp();
	SetTestNumUp();
	SetDefNumDn();
	SetDefRtoDn();
	SetGoodNumDn();
	SetGoodRtoDn();
	SetTestNumDn();
	SetDefNumTot();
	SetDefRtoTot();
	SetGoodNumTot();
	SetGoodRtoTot();
	SetTestNumTot();
}


void CEngrave::SetStTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if(pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetStTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _stItemInx::_LotStTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetRunTime()
{
	return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetRunTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _stItemInx::_LotRunTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetEdTime()
{
	return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetEdTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _stItemInx::_LotEdTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetStripRatio()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nGood = 0, nBad = 0, nTot = 0, nStTot = 0, nSum = 0, nVal[2][4];
	int nMer[MAX_STRIP];
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 4; k++)
			nVal[i][k] = 0;
	}

	// < 스트립 별 수율 >
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;
	nStTot = nTot / 4;

	// 상면
	nSum = 0;
	nVal[0][0] = pDoc->m_pReelMapUp->GetDefStrip(0);
	nSum += nVal[0][0];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][0]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_1LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][1] = pDoc->m_pReelMapUp->GetDefStrip(1);
	nSum += nVal[0][1];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][1]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_2LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][2] = pDoc->m_pReelMapUp->GetDefStrip(2);
	nSum += nVal[0][2];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][2]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_3LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][3] = pDoc->m_pReelMapUp->GetDefStrip(3);
	nSum += nVal[0][3];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][3]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_4LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	if (nTot > 0)
		dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_AllLnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);


	if (bDualTest)
	{
		// 하면
		nSum = 0;
		nVal[1][0] = pDoc->m_pReelMapDn->GetDefStrip(0);
		nSum += nVal[1][0];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][0]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_1LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][1] = pDoc->m_pReelMapDn->GetDefStrip(1);
		nSum += nVal[1][1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_2LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][2] = pDoc->m_pReelMapDn->GetDefStrip(2);
		nSum += nVal[1][2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_3LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][3] = pDoc->m_pReelMapDn->GetDefStrip(3);
		nSum += nVal[1][3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_4LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_AllLnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		// 상면 + 하면
		nSum = 0;
		nMer[0] = pDoc->m_pReelMapAllUp->GetDefStrip(0);
		nSum += nMer[0];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[0]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_1LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[1] = pDoc->m_pReelMapAllUp->GetDefStrip(1);
		nSum += nMer[1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_2LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[2] = pDoc->m_pReelMapAllUp->GetDefStrip(2);
		nSum += nMer[2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_3LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[3] = pDoc->m_pReelMapAllUp->GetDefStrip(3);
		nSum += nMer[3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_4LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_AllLnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDef()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nNum = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CReelMap* pReelMap=NULL;

	if (bDualTest)
	{
		switch (pView->m_nSelRmap)
		{
		case RMAP_UP:
			pReelMap = pDoc->m_pReelMapUp;
			break;
		case RMAP_DN:
			pReelMap = pDoc->m_pReelMapDn;
			break;
		case RMAP_ALLUP:
			pReelMap = pDoc->m_pReelMapAllUp;
			break;
		case RMAP_ALLDN:
			pReelMap = pDoc->m_pReelMapAllDn;
			break;
		}
	}
	else
		pReelMap = pDoc->m_pReelMapUp;

	if(pReelMap)
	{
		nNum = pReelMap->GetDefNum(DEF_OPEN); // IDC_STC_DEF_OPEN
		SocketData.nMsgID = _stItemInx::_DefNumOpen;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_SHORT); // IDC_STC_DEF_SHORT
		SocketData.nMsgID = _stItemInx::_DefNumShort;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_USHORT); // IDC_STC_DEF_U_SHORT
		SocketData.nMsgID = _stItemInx::_DefNumUshort;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_SPACE); // IDC_STC_DEF_SPACE
		SocketData.nMsgID = _stItemInx::_DefNumLnW;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EXTRA); // IDC_STC_DEF_EXTRA
		SocketData.nMsgID = _stItemInx::_DefExtr;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_PROTRUSION); // IDC_STC_DEF_PROT
		SocketData.nMsgID = _stItemInx::_DefNumProt;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_PINHOLE); // IDC_STC_DEF_P_HOLE
		SocketData.nMsgID = _stItemInx::_DefNumPhole;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_PAD); // IDC_STC_DEF_PAD
		SocketData.nMsgID = _stItemInx::_DefNumPad;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_OPEN); // IDC_STC_DEF_H_OPEN
		SocketData.nMsgID = _stItemInx::_DefNumHopen;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_MISS); // IDC_STC_DEF_H_MISS
		SocketData.nMsgID = _stItemInx::_DefNumHmiss;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_POSITION); // IDC_STC_DEF_H_POS
		SocketData.nMsgID = _stItemInx::_DefNumHpos;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_DEFECT); // IDC_STC_DEF_H_DEF
		SocketData.nMsgID = _stItemInx::_DefNumHdef;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_NICK); // IDC_STC_DEF_NICK
		SocketData.nMsgID = _stItemInx::_DefNumNick;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_POI); // IDC_STC_DEF_POI
		SocketData.nMsgID = _stItemInx::_DefNumPoi;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_OPEN); // IDC_STC_DEF_VH_OPEN
		SocketData.nMsgID = _stItemInx::_DefNumVhOpen;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_MISS); // IDC_STC_DEF_VH_MISS
		SocketData.nMsgID = _stItemInx::_DefNumVhMiss;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_POSITION); // IDC_STC_DEF_VH_POS
		SocketData.nMsgID = _stItemInx::_DefNumVhPos;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_DEF); // IDC_STC_DEF_VH_DEF
		SocketData.nMsgID = _stItemInx::_DefNumVhd;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_LIGHT); // IDC_STC_DEF_LIGHT
		SocketData.nMsgID = _stItemInx::_DefNumLight;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EDGE_NICK);
		SocketData.nMsgID = _stItemInx::_DefNumEnick;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EDGE_PROT);
		SocketData.nMsgID = _stItemInx::_DefNumEprot;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EDGE_SPACE);
		SocketData.nMsgID = _stItemInx::_DefNumEspace;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_USER_DEFINE_1);
		SocketData.nMsgID = _stItemInx::_DefNumUdd1;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_NARROW);
		SocketData.nMsgID = _stItemInx::_DefNumNrw;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_WIDE);
		SocketData.nMsgID = _stItemInx::_DefNumWide;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);
	}
}

void CEngrave::Set2DReader()
{
	Set2DEngLen();
	Set2DAoiLen();
	Set2DMkLen();
	Set2DMoveVel();
	Set2DMoveAcc();
	Set2DOneShotLen();
}

void CEngrave::SetEngInfo()
{
	SetEngLeadPitch();
	SetEngPushOffLen();
	SetEngTqVal();
	SetEngAoiLen();
	SetEngFdDiffMax();
	SetEngFdDiffRng();
	SetEngFdDiffNum();
	SetEngBuffInitPos();
	SetEngBuffCurrPos();
}

void CEngrave::SetFdInfo()
{
	SetFdVel();
	SetFdAcc();
	SetOnePnlVel();
	SetOnePnlAcc();
	SetFdDiffMax();
	SetFdDiffRng();
	SetFdDiffNum();
}

void CEngrave::SetAoiInfo()
{
	SetAoiLeadPitch();
	SetAoiPushOffLen();
	SetAoiTqVal();
	SetAoiBuffShotNum();
	SetAoiMkLen();
}

void CEngrave::SetMkInfo()
{
	SetMkLeadPitch();
	SetMkPushOffLen();
	SetMkTqVal();
	SetMkBuffInitPos();
	SetMkBuffCurrPos();
	SetMkNumLf();
	SetMkNumRt();
	SetMkMaxNumLf();
	SetMkMaxNumRt();
}

void CEngrave::SetMkInfoLf()
{
	SetMkInitPosLf();
	SetMkInitVelLf();
	SetMkInitAccLf();
	SetMkFnlPosLf();
	SetMkFnlVelLf();
	SetMkFnlAccLf();
	SetMkFnlTqLf();
	SetMkHgtPosX1Lf();
	SetMkHgtPosY1Lf();
	SetMkHgtPosX2Lf();
	SetMkHgtPosY2Lf();
	SetMkHgtPosX3Lf();
	SetMkHgtPosY3Lf();
	SetMkHgtPosX4Lf();
	SetMkHgtPosY4Lf();
	SetMkHgtAvgPosLf();
}

void CEngrave::SetMkInfoRt()
{
	SetMkInitPosRt();
	SetMkInitVelRt();
	SetMkInitAccRt();
	SetMkFnlPosRt();
	SetMkFnlVelRt();
	SetMkFnlAccRt();
	SetMkFnlTqRt();
	SetMkHgtPosX1Rt();
	SetMkHgtPosY1Rt();
	SetMkHgtPosX2Rt();
	SetMkHgtPosY2Rt();
	SetMkHgtPosX3Rt();
	SetMkHgtPosY3Rt();
	SetMkHgtPosX4Rt();
	SetMkHgtPosY4Rt();
	SetMkHgtAvgPosRt();
}

// SetOpInfo()
void CEngrave::SetOpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_OpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sSelUserName, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetDualTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DualTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDualTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_SampleTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bSampleTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_SampleShotNum;
	SocketData.nData1 = _ttoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	SendCommand(SocketData);
}

void CEngrave::SetTestMode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _stSigInx::_TestMode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nTestMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
	SendCommand(SocketData);
}

void CEngrave::SetRecoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_RecoilerCcw;						// OneMetal : TRUE -> SetTwoMetal(FALSE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bOneMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUncoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UncoilerCcw;						// TwoMetal : TRUE -> SetTwoMetal(TRUE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTwoMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetAlignMethode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _stSigInx::_AlignMethode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nAlignMethode; // TWO_POINT, FOUR_POINT
	SendCommand(SocketData);
}

void CEngrave::SetDoorRecoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorRecoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorUncoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorUncoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSaftyMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_SaftyMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkSftySen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CleannerAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CleannerAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UltraSonicAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UltraSonicEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetTotReelLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_TotReelLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sReelTotLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}


void CEngrave::SetOnePnlLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_OnePnlLen;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlLen());
	SendCommand(SocketData);
}

void CEngrave::SetTempPause()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TempPause;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetTempStopLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_TempStopLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sTempPauseLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotCut()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LotCut;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bLotSep ? 1 : 0;	// pDoc->m_pReelMap->m_bUseLotSep
	SendCommand(SocketData);
}

void CEngrave::SetLotCutLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotCutLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSepLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotCutPosLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotCutPosLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotCutPosLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLmtTotYld()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LmtTotYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtTotYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLmtPatlYld()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LmtPatlYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtPatlYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetStripOutRatio()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_StripOutRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sStripOutRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetCustomNeedRatio()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_CustomNeedRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sCustomNeedRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetNumRangeFixDef()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_NumRangeFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumRangeFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetFixDef()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FixDef;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bContFixDef ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetNumContFixDef()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_NumContFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumContFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicStTim()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_UltraSonicStTim;
	StringToChar(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngItsCode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngItsCode;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngItsCode, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetInfo()
void CEngrave::SetModelUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_ModelUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// void CEngrave::SetModelDnName()
// {
// 	if (!pDoc)
// 		return;
// 
// 	SOCKET_DATA SocketData;
// 	SocketData.nCmdCode = _SetData;
// 	char cData[BUFFER_DATA_SIZE];
// 
// 	SocketData.nMsgID = _stItemInx::_ModelDnName;
// 	StringToChar(pDoc->WorkingInfo.LastJob.sModelDn, cData);
// 	sprintf(SocketData.strData, "%s", cData);
// 	SendCommand(SocketData);
// }

void CEngrave::SetLotUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotDnName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLayerUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LayerUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLayerDnName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LayerDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOrderNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_OrderNum;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngraveOrderNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_ShotNum;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngraveLastShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetTotOpRto()
{
	if (!pDoc || !pView || !pView->m_pDlgFrameHigh)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_TotOpRto;
	double dFdTotLen = (double)pView->m_pDlgFrameHigh->m_nMkLastShot * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	sVal.Format(_T("%d"), (int)(dFdTotLen / _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 100.0));
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetTotVel()
{
	if (!pView)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_TotVel;
	sVal.Format(_T("%.1f"), pView->GetTotVel());
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetPartVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_PartVel;
	StringToChar(pDoc->WorkingInfo.LastJob.sPartialSpd, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkDoneLen()
{
	if (!pDoc || !pView || !pView->m_pDlgFrameHigh)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_MkDoneLen;
	double dFdTotLen = (double)pView->m_pDlgFrameHigh->m_nMkLastShot * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	sVal.Format(_T("%.2f"), dFdTotLen / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiDnDoneLen()
{
	if (!pView)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_AoiDnDoneLen;
	sVal.Format(_T("%.2f"), pView->GetAoiDnFdLen() / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiUpDoneLen()
{
	if (!pView)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_AoiUpDoneLen;
	sVal.Format(_T("%.2f"), pView->GetAoiUpFdLen() / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotSerial()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotSerial;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSerial, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkVerfyLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkVerfyLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sVerifyLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetTotRatio()
void CEngrave::SetDefNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;


	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면
	nTot = nGood + nBad;

	SocketData.nMsgID = _stItemInx::_DefNumUp;
	SocketData.nData1 = nBad;
	SendCommand(SocketData);
}

void CEngrave::SetDefRtoUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	if (nTot > 0)
		dRatio = ((double)nBad / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_DefRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);
}

void CEngrave::SetGoodNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	SocketData.nMsgID = _stItemInx::_GoodNumUp;
	SocketData.nData1 = nGood;
	SendCommand(SocketData);
}

void CEngrave::SetGoodRtoUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	if (nTot > 0)
		dRatio = ((double)nGood / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_GoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);
}

void CEngrave::SetTestNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면
	nTot = nGood + nBad;

	SocketData.nMsgID = _stItemInx::_TestNumUp;
	SocketData.nData1 = nTot;
	SendCommand(SocketData);
}

void CEngrave::SetDefNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_DefNumDn;
		SocketData.nData1 = nBad;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefRtoDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_DefRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면

		SocketData.nMsgID = _stItemInx::_GoodNumDn;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodRtoDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_GoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetTestNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_TestNumDn;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_DefNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefRtoTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_DefRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체

		SocketData.nMsgID = _stItemInx::_GoodNumTot;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodRtoTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_GoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetTestNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_TestNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

// Set2DReader()
void CEngrave::Set2DEngLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DEngLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DEngLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DAoiLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMkLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMkLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMoveVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DMoveVel;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMoveAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DMoveAcc;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DOneShotLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DOneShotLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DOneShotRemainLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetEngInfo()
void CEngrave::SetEngLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngAoiLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngAoiLen;
	pDoc->WorkingInfo.Motion.sEngAoiLen = pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist;
	StringToChar(pDoc->WorkingInfo.Motion.sEngAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffMax()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngFdDiffMax;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffMax, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffRng()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngFdDiffRng;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffRng, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngFdDiffNum;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffInitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffCurrPos()
{
	if (!pDoc && !pDoc->m_pMpeData)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngBuffCurrPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffCurrPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffJogCw;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffJogCw ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffJogCcw;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffJogCcw ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffHome()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffOrgMv;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffHomming ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffHomeDone()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffOrgMvDone;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffHommingDone ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitMove()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffInitPosMv;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffInitMv ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitMoveDone()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffInitPosMvDone;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffInitMvDone ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitPosSave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffInitPosSave;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffInitPosSave ? 1 : 0;
	SendCommand(SocketData);
}


// SetFdInfo()
void CEngrave::SetFdVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_FdVel;
	//StringToChar(pDoc->WorkingInfo.Motion.sMkFdVel, cData);
	StringToChar(pDoc->WorkingInfo.Motion.sMkJogVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetFdAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_FdAcc;
	//StringToChar(pDoc->WorkingInfo.Motion.sMkFdAcc, cData);
	StringToChar(pDoc->WorkingInfo.Motion.sMkJogAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_OnePnlVel;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlVel());
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_OnePnlAcc;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlAcc());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffMax()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_FdDiffMax;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrLmt());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffRng()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_FdDiffRng;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrRng());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_FdDiffNum;
	sprintf(SocketData.strData, "%d", pDoc->GetFdErrNum());
	SendCommand(SocketData);
}

// SetAoiInfo()
void CEngrave::SetAoiLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiBuffShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiBuffShotNum;
	StringToChar(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiMkLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetMkInfo()
void CEngrave::SetMkLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sMkTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkBuffInitPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sStBufPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkBuffCurrPos()
{
	//if (!pDoc)
	//	return;

	//SOCKET_DATA SocketData;
	//SocketData.nCmdCode = _SetData;

	//SocketData.nMsgID = _MkBuffCurrPos;
	//double dBufEnc = (double)pDoc->m_pMpeData[0][1] / 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
	//															//sprintf(SocketData.strData, "%.1f", dBufEnc);
	//SocketData.fData1 = (float)dBufEnc;
	//SendCommand(SocketData);
}

void CEngrave::SetMkNumLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkNumLf;
	SocketData.nData1 = pDoc->GetMkCntL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntL());
	SendCommand(SocketData);
}

void CEngrave::SetMkNumRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkNumRt;
	SocketData.nData1 = pDoc->GetMkCntR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntR());
	SendCommand(SocketData);
}

void CEngrave::SetMkMaxNumLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkMaxNumLf;
	SocketData.nData1 = pDoc->GetMkLimitL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitL());
	SendCommand(SocketData);
}

void CEngrave::SetMkMaxNumRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkMaxNumRt;
	SocketData.nData1 = pDoc->GetMkLimitR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitR());
	SendCommand(SocketData);
}

// SetMkInfoLf()
void CEngrave::SetMkInitPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitVelLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitAccLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlVelLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlAccLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlTqLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlTqLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX1Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtAvgPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtAvgPosLf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetAverDist1());
	SendCommand(SocketData);
}

// SetMkInfoRt()
void CEngrave::SetMkInitPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitVelRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _ItemInx::_MkInitVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitAccRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlVelRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlAccRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlTqRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlTqRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX1Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtAvgPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtAvgPosRt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetAverDist2());
	SendCommand(SocketData);
}

// End for SetSysData()


// Start Switch

// Main
void CEngrave::SwReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Ready;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsReady;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Run;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRun;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Reset;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsReset;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Stop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsStop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAuto(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Auto;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAuto(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsAuto;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwManual(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Manual;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwManual(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsManual;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Torque Motor
void CEngrave::SwMkTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MkTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMkTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_AoiTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsAoiTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsEngTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Induction Motor
void CEngrave::SwRcInductionCcw(BOOL bOn)	// SetOneMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CcwModRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcInductionCcw(BOOL bOn)	// SetOneMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCcwModRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcInductionCcw(BOOL bOn)	// SetTwoMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CcwModUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcInductionCcw(BOOL bOn)	// SetTwoMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCcwModUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Core 150mm
void CEngrave::SwRcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Core150Re;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCore150Re;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Core150Un;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCore150Un;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Recoiler
void CEngrave::SwRcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PrdChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPrdChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpLfRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpLfRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpRtRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpRtRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteVacRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteVacRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcRewinder(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcRewinder(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDoRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcRewinderReelPaper(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PrdPprRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcRewinderReelPaper(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPrdPprRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Punching
void CEngrave::SwMkRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTblBlw(BOOL bOn)	
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// AOIDn
void CEngrave::SwAoiDnRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiDnClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_VelClrSonicAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsVelClrSonicAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiDnTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TestAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTestAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ResetAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsResetAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LotEndAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLotEndAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}



// AOIUp
void CEngrave::SwAoiUpRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiUpTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TestAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTestAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ResetAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsResetAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LotEndAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLotEndAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Engrave
void CEngrave::SwEngRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_VelClrSonicEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsVelClrSonicEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Uncoiler
void CEngrave::SwUcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PrdChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPrdChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpLfUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpLfUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpRtUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpRtUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteVacUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteVacUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcClRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcClRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcClRlPshUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollPushUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcClRlPshUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollPushUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Etc
void CEngrave::SwAoiEmg(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EmgAoi;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiEmg(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsEmgAoi;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// EngraveAutoSequence

void CEngrave::SwEngAutoInit(BOOL bOn) // 각인부 초기화(Reset)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.Init = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoInit;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoInit(BOOL bOn) // 각인부 초기화(Reset)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.Init = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoInit;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoMkSt(BOOL bOn) // 각인부 마킹시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.MkSt = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqMkSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoMkSt(BOOL bOn) // 각인부 마킹시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.MkSt = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqMkSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoOnMking(BOOL bOn) // 각인부 마킹중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.OnMking = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqOnMkIng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoOnMking(BOOL bOn) // 각인부 마킹중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.OnMking = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqOnMkIng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoMkDone(BOOL bOn) // 각인부 마킹완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.MkDone = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqMkDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoMkDone(BOOL bOn) // 각인부 마킹완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.MkDone = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqMkDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAuto2dReadSt(BOOL bOn) // 각인부 2D Read 시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Read2dSt = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeq2dReadSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAuto2dReadSt(BOOL bOn) // 각인부 2D Read 시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Read2dSt = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeq2dReadSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoOnReading2d(BOOL bOn) // 각인부 Read중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.OnRead2d = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqOnReading2d;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoOnReading2d(BOOL bOn) // 각인부 Read중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.OnRead2d = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqOnReading2d;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAuto2dReadDone(BOOL bOn) // 각인부 2D Read 완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.Read2dDone = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeq2dReadDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAuto2dReadDone(BOOL bOn) // 각인부 2D Read 완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.EngAuto.Read2dDone = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeq2dReadDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Set Engrave Data
void CEngrave::SetEngraveAoiDist()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngraveFdPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}


// DlgMyMsg
void CEngrave::SetMyMsgYes()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.Msg.Yes = TRUE;
	SocketData.nMsgID = _SigInx::_MyMsgYes;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMyMsgYes()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.Msg.IsYes = TRUE;
	SocketData.nMsgID = _SigInx::_IsMyMsgYes;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::SetMyMsgNo()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.Msg.No = TRUE;
	SocketData.nMsgID = _SigInx::_MyMsgNo;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMyMsgNo()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.Msg.IsNo = TRUE;
	SocketData.nMsgID = _SigInx::_IsMyMsgNo;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::SetMyMsgOk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Msg.No = TRUE;
	SocketData.nMsgID = _SigInx::_MyMsgOk;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMyMsgOk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.Msg.IsNo = TRUE;
	SocketData.nMsgID = _SigInx::_IsMyMsgOk;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}


// Disp
void CEngrave::SetDispReady(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.Ready = TRUE;
	SocketData.nMsgID = _SigInx::_DispReady;
	SocketData.nData1 = pDoc->BtnStatus.Disp.Ready > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispReady;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispRun(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.Run = TRUE;
	SocketData.nMsgID = _SigInx::_DispRun;
	SocketData.nData1 = pDoc->BtnStatus.Disp.Run > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispRun;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispStop(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.Stop = TRUE;
	SocketData.nMsgID = _SigInx::_DispStop;
	SocketData.nData1 = pDoc->BtnStatus.Disp.Stop > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispStop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispDualSample(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.DualSample = TRUE;
	SocketData.nMsgID = _SigInx::_DispDualSample;
	SocketData.nData1 = pDoc->BtnStatus.Disp.DualSample > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispDualSample(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispDualSample;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispSingleSample(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.SingleSample = TRUE;
	SocketData.nMsgID = _SigInx::_DispSingleSample;
	SocketData.nData1 = pDoc->BtnStatus.Disp.SingleSample > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispSingleSample(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispSingleSample;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispDualTest(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.DualTest = TRUE;
	SocketData.nMsgID = _SigInx::_DispDualTest;
	SocketData.nData1 = pDoc->BtnStatus.Disp.DualTest > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispDualTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispDualTest;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispSingleTest(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.SingleTest = TRUE;
	SocketData.nMsgID = _SigInx::_DispSingleTest;
	SocketData.nData1 = pDoc->BtnStatus.Disp.SingleTest > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispSingleTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispSingleTest;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// End Switch


// Alarm

void CEngrave::SetAlarm(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stAlarmInx::_Alarm;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::IsSetAlarm(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stAlarmInx::_IsAlarm;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMsgBox(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stMsgBoxInx::_MsgBox;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::IsSetMsgBox(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stMsgBoxInx::_IsMsgBox;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}


void CEngrave::SwMenu01UpdateWorking(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UpdateWork;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMenu01UpdateWorking(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsUpdateWork;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}
