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
	m_pServer->WriteCommData(SocketData, INFINITE);

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


// Start for GetSysInfo()

void CEngrave::GetSysInfo(SOCKET_DATA SockData)
{
	GetOpInfo(SockData);
	GetInfo(SockData);
	GetTotRatio(SockData);
	GetStTime(SockData);
	GetRunTime(SockData);
	GetEdTime(SockData);
	GetStripRatio(SockData);
	GetDef(SockData);
	Get2DReader(SockData);
	GetEngInfo(SockData);
	GetFdInfo(SockData);
	GetAoiInfo(SockData);
	GetMkInfo(SockData);
	GetMkInfoLf(SockData);
	GetMkInfoRt(SockData);
}

void CEngrave::GetOpInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _OpName:
		pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);
		break;
	case _DualTest:
		pDoc->WorkingInfo.LastJob.bDualTest = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _SampleTest:
		pDoc->WorkingInfo.LastJob.bSampleTest = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _SampleShotNum:
		sVal.Format(_T("%d"), (int)SockData.nData1);
		pDoc->WorkingInfo.LastJob.sSampleTestShotNum = sVal;
		break;
	case _TestMode:
		pDoc->WorkingInfo.LastJob.nTestMode = (int)SockData.nData1; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
		break;
	case _RecoilerCcw:
		pDoc->WorkingInfo.LastJob.bOneMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// OneMetal : TRUE -> SetTwoMetal(FALSE);
		break;
	case _UncoilerCcw:
		pDoc->WorkingInfo.LastJob.bTwoMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// TwoMetal : TRUE -> SetTwoMetal(TRUE);
		break;
	case _AlignMethode:
		pDoc->WorkingInfo.LastJob.nAlignMethode = (int)SockData.nData1; // TWO_POINT, FOUR_POINT
		break;
	case _DoorRecoiler:
		pDoc->WorkingInfo.LastJob.bRclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorAoiUp:
		pDoc->WorkingInfo.LastJob.bAoiUpDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorAoiDn:
		pDoc->WorkingInfo.LastJob.bAoiDnDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorMk:
		pDoc->WorkingInfo.LastJob.bMkDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorEngrave:
		pDoc->WorkingInfo.LastJob.bEngvDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorUncoiler:
		pDoc->WorkingInfo.LastJob.bUclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _SaftyMk:
		pDoc->WorkingInfo.LastJob.bMkSftySen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _CleannerAoiUp:
		pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _CleannerAoiDn:
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _UltraSonicAoiDn:
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _UltraSonicEngrave:
		pDoc->WorkingInfo.LastJob.bUseEngraveCleanner = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TotReelLen:
		pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
		break;
	case _OnePnlLen:
		pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);
		break;
	case _TempPause:
		pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TempStopLen:
		pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
		break;
	case _LotCut:
		pDoc->WorkingInfo.LastJob.bLotSep = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _LotCutLen:
		pDoc->WorkingInfo.LastJob.sLotSepLen = CharToString(SockData.strData);
		break;
	case _LotCutPosLen:
		pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
		break;
	case _LmtTotYld:
		pDoc->WorkingInfo.LastJob.sLmtTotYld = CharToString(SockData.strData);
		break;
	case _LmtPatlYld:
		pDoc->WorkingInfo.LastJob.sLmtPatlYld = CharToString(SockData.strData);
		break;
	case _StripOutRatio:
		pDoc->WorkingInfo.LastJob.sStripOutRatio = CharToString(SockData.strData);
		break;
	case _CustomNeedRatio:
		pDoc->WorkingInfo.LastJob.sCustomNeedRatio = CharToString(SockData.strData);
		break;
	case _NumRangeFixDef:
		pDoc->WorkingInfo.LastJob.sNumRangeFixDef = CharToString(SockData.strData);
		break;
	case _NumContFixDef:
		pDoc->WorkingInfo.LastJob.sNumContFixDef = CharToString(SockData.strData);
		break;
	case _UltraSonicStTim:
		pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim = CharToString(SockData.strData);
		break;
	}
}

void CEngrave::GetInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _OpName:
		pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);
		break;
	case _ModelUpName:
		pDoc->WorkingInfo.LastJob.sModelUp = CharToString(SockData.strData);
		break;
	case _ModelDnName:
		pDoc->WorkingInfo.LastJob.sModelDn = CharToString(SockData.strData);
		break;
	case _LotUpName:
		pDoc->WorkingInfo.LastJob.sLotUp = CharToString(SockData.strData);
		break;
	case _LotDnName:
		pDoc->WorkingInfo.LastJob.sLotDn = CharToString(SockData.strData);
		break;
	case _LayerUpName:
		pDoc->WorkingInfo.LastJob.sLayerUp = CharToString(SockData.strData);
		break;
	case _LayerDnName:
		pDoc->WorkingInfo.LastJob.sLayerDn = CharToString(SockData.strData);
		break;
	case _TotReelLen:
		pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
		break;
	case _PartVel:
		pDoc->WorkingInfo.LastJob.sPartialSpd = CharToString(SockData.strData);
		break;
	case _TempPause:
		pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TempStopLen:
		pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
		break;
	case _LotCutLen:
		pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
		break;
	case _LotSerial:
		pDoc->WorkingInfo.LastJob.sLotSerial = CharToString(SockData.strData);
		break;
	case _MkVerfyLen:
		pDoc->WorkingInfo.LastJob.sVerifyLen = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetTotRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _DefNumUp:
		pDoc->m_nBad[0] = SockData.nData1;
		break;
	case _DefRtoUp:
		pDoc->m_dBadRatio[0] = (double)SockData.fData1;
		break;
	case _GoodNumUp:
		pDoc->m_nGood[0] = SockData.nData1;
		break;
	case _GoodRtoUp:
		pDoc->m_dGoodRatio[0] = (double)SockData.fData1;
		break;
	case _TestNumUp:
		pDoc->m_nTestNum[0] = SockData.nData1;
		break;
	case _DefNumDn:
		pDoc->m_nBad[1] = SockData.nData1;
		break;
	case _DefRtoDn:
		pDoc->m_dBadRatio[1] = (double)SockData.fData1;
		break;
	case _GoodNumDn:
		pDoc->m_nGood[1] = SockData.nData1;
		break;
	case _GoodRtoDn:
		pDoc->m_dGoodRatio[1] = (double)SockData.fData1;
		break;
	case _TestNumDn:
		pDoc->m_nTestNum[1] = SockData.nData1;
		break;
	case _DefNumTot:
		pDoc->m_nBad[2] = SockData.nData1;
		break;
	case _DefRtoTot:
		pDoc->m_dBadRatio[2] = (double)SockData.fData1;
		break;
	case _GoodNumTot:
		pDoc->m_nGood[2] = SockData.nData1;
		break;
	case _GoodRtoTot:
		pDoc->m_dGoodRatio[2] = (double)SockData.fData1;
		break;
	case _TestNumTot:
		pDoc->m_nTestNum[2] = SockData.nData1;
		break;
	default:
		break;
	}
}

void CEngrave::GetStTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _LotStTime:
		pDoc->m_sLotStTime = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetRunTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _LotRunTime:
		pDoc->m_sLotRunTime = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetEdTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _LotEdTime:
		pDoc->m_sLotEdTime = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetStripRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _1LnGoodRtoUp:
		pDoc->m_dStripRatio[0][0] = (double)SockData.fData1;
		break;
	case _2LnGoodRtoUp:
		pDoc->m_dStripRatio[0][1] = (double)SockData.fData1;
		break;
	case _3LnGoodRtoUp:
		pDoc->m_dStripRatio[0][2] = (double)SockData.fData1;
		break;
	case _4LnGoodRtoUp:
		pDoc->m_dStripRatio[0][3] = (double)SockData.fData1;
		break;
	case _AllLnGoodRtoUp:
		pDoc->m_dStripRatio[0][4] = (double)SockData.fData1;
		break;
	case _1LnGoodRtoDn:
		pDoc->m_dStripRatio[1][0] = (double)SockData.fData1;
		break;
	case _2LnGoodRtoDn:
		pDoc->m_dStripRatio[1][1] = (double)SockData.fData1;
		break;
	case _3LnGoodRtoDn:
		pDoc->m_dStripRatio[1][2] = (double)SockData.fData1;
		break;
	case _4LnGoodRtoDn:
		pDoc->m_dStripRatio[1][3] = (double)SockData.fData1;
		break;
	case _AllLnGoodRtoDn:
		pDoc->m_dStripRatio[1][4] = (double)SockData.fData1;
		break;
	case _1LnGoodRtoTot:
		pDoc->m_dStripRatio[2][0] = (double)SockData.fData1;
		break;
	case _2LnGoodRtoTot:
		pDoc->m_dStripRatio[2][1] = (double)SockData.fData1;
		break;
	case _3LnGoodRtoTot:
		pDoc->m_dStripRatio[2][2] = (double)SockData.fData1;
		break;
	case _4LnGoodRtoTot:
		pDoc->m_dStripRatio[2][3] = (double)SockData.fData1;
		break;
	case _AllLnGoodRtoTot:
		pDoc->m_dStripRatio[2][4] = (double)SockData.fData1;
		break;
	default:
		break;
	}
}

void CEngrave::GetDef(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _DefNumOpen:
		pDoc->m_nDef[DEF_OPEN] = SockData.nData1; // IDC_STC_DEF_OPEN
		break;
	case _DefNumShort:
		pDoc->m_nDef[DEF_SHORT] = SockData.nData1; // IDC_STC_DEF_SHORT
		break;
	case _DefNumUshort:
		pDoc->m_nDef[DEF_USHORT] = SockData.nData1; // IDC_STC_DEF_U_SHORT
		break;
	case _DefNumLnW:
		pDoc->m_nDef[DEF_SPACE] = SockData.nData1; // IDC_STC_DEF_SPACE
		break;
	case _DefExtr:
		pDoc->m_nDef[DEF_EXTRA] = SockData.nData1; // IDC_STC_DEF_EXTRA
		break;
	case _DefNumProt:
		pDoc->m_nDef[DEF_PROTRUSION] = SockData.nData1; // IDC_STC_DEF_PROT
		break;
	case _DefNumPhole:
		pDoc->m_nDef[DEF_PINHOLE] = SockData.nData1; // IDC_STC_DEF_P_HOLE
		break;
	case _DefNumPad:
		pDoc->m_nDef[DEF_PAD] = SockData.nData1; // IDC_STC_DEF_PAD
		break;
	case _DefNumHopen:
		pDoc->m_nDef[DEF_HOLE_OPEN] = SockData.nData1; // IDC_STC_DEF_H_OPEN
		break;
	case _DefNumHmiss:
		pDoc->m_nDef[DEF_HOLE_MISS] = SockData.nData1; // IDC_STC_DEF_H_MISS
		break;
	case _DefNumHpos:
		pDoc->m_nDef[DEF_HOLE_POSITION] = SockData.nData1; // IDC_STC_DEF_H_POS
		break;
	case _DefNumHdef:
		pDoc->m_nDef[DEF_HOLE_DEFECT] = SockData.nData1; // IDC_STC_DEF_H_DEF
		break;
	case _DefNumNick:
		pDoc->m_nDef[DEF_NICK] = SockData.nData1; // IDC_STC_DEF_NICK
		break;
	case _DefNumPoi:
		pDoc->m_nDef[DEF_POI] = SockData.nData1; // IDC_STC_DEF_POI
		break;
	case _DefNumVhOpen:
		pDoc->m_nDef[DEF_VH_OPEN] = SockData.nData1; // IDC_STC_DEF_VH_OPEN
		break;
	case _DefNumVhMiss:
		pDoc->m_nDef[DEF_VH_MISS] = SockData.nData1; // IDC_STC_DEF_VH_MISS
		break;
	case _DefNumVhPos:
		pDoc->m_nDef[DEF_VH_POSITION] = SockData.nData1; // IDC_STC_DEF_VH_POS
		break;
	case _DefNumVhd:
		pDoc->m_nDef[DEF_VH_DEF] = SockData.nData1; // IDC_STC_DEF_VH_DEF
		break;
	case _DefNumLight:
		pDoc->m_nDef[DEF_LIGHT] = SockData.nData1; // IDC_STC_DEF_LIGHT
		break;
	case _DefNumEnick:
		pDoc->m_nDef[DEF_EDGE_NICK] = SockData.nData1;
		break;
	case _DefNumEprot:
		pDoc->m_nDef[DEF_EDGE_PROT] = SockData.nData1;
		break;
	case _DefNumEspace:
		pDoc->m_nDef[DEF_EDGE_SPACE] = SockData.nData1;
		break;
	case _DefNumUdd1:
		pDoc->m_nDef[DEF_USER_DEFINE_1] = SockData.nData1;
		break;
	case _DefNumNrw:
		pDoc->m_nDef[DEF_NARROW] = SockData.nData1;
		break;
	case _DefNumWide:
		pDoc->m_nDef[DEF_WIDE] = SockData.nData1;
		break;
	default:
		break;
	}
}

void CEngrave::Get2DReader(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _2DEngLen:
		pDoc->WorkingInfo.Motion.s2DEngLen = CharToString(SockData.strData);
		break;
	case _2DAoiLen:
		pDoc->WorkingInfo.Motion.s2DAoiLen = CharToString(SockData.strData);
		break;
	case _2DMkLen:
		pDoc->WorkingInfo.Motion.s2DMkLen = CharToString(SockData.strData);
		break;
	case _2DMoveVel:
		pDoc->WorkingInfo.Motion.s2DMoveVel = CharToString(SockData.strData);
		break;
	case _2DMoveAcc:
		pDoc->WorkingInfo.Motion.s2DMoveAcc = CharToString(SockData.strData);
		break;
	case _2DOneShotLen:
		pDoc->WorkingInfo.Motion.s2DOneShotRemainLen = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetEngInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _EngLeadPitch:
		pDoc->WorkingInfo.Motion.sEngraveFdLead = CharToString(SockData.strData);
		break;
	case _EngPushOffLen:
		pDoc->WorkingInfo.Motion.sEngraveFdVacOff = CharToString(SockData.strData);
		break;
	case _EngTqVal:
		pDoc->WorkingInfo.Motion.sEngraveTq = CharToString(SockData.strData);
		break;
	case _EngAoiLen:
		pDoc->WorkingInfo.Motion.sEngAoiLen = CharToString(SockData.strData);
		break;
	case _EngFdDiffMax:
		pDoc->WorkingInfo.Motion.sEngFdDiffMax = CharToString(SockData.strData);
		break;
	case _EngFdDiffRng:
		pDoc->WorkingInfo.Motion.sEngFdDiffRng = CharToString(SockData.strData);
		break;
	case _EngFdDiffNum:
		pDoc->WorkingInfo.Motion.sEngFdDiffNum = CharToString(SockData.strData);
		break;
	case _EngBuffInitPos:
		pDoc->WorkingInfo.Motion.sEngBuffInitPos = CharToString(SockData.strData);
		break;
	case _EngBuffCurrPos:
		pDoc->WorkingInfo.Motion.sEngBuffCurrPos = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetFdInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _FdVel:
		pDoc->WorkingInfo.Motion.sMkFdVel = CharToString(SockData.strData);
		break;
	case _FdAcc:
		pDoc->WorkingInfo.Motion.sMkFdAcc = CharToString(SockData.strData);
		break;
	case _OnePnlLen:
		pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);
		break;
	case _OnePnlVel:
		pDoc->WorkingInfo.Motion.sMkFdVel = CharToString(SockData.strData);
		break;
	case _OnePnlAcc:
		pDoc->WorkingInfo.Motion.sMkFdAcc = CharToString(SockData.strData);
		break;
	case _FdDiffMax:
		pDoc->WorkingInfo.Motion.sLmtFdErr = CharToString(SockData.strData);
		break;
	case _FdDiffRng:
		pDoc->WorkingInfo.Motion.sLmtFdAdjOffSet = CharToString(SockData.strData);
		break;
	case _FdDiffNum:
		pDoc->WorkingInfo.Motion.sLmtFdOvrNum = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetAoiInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _AoiLeadPitch:
		pDoc->WorkingInfo.Motion.sAoiFdLead = CharToString(SockData.strData);
		break;
	case _AoiPushOffLen:
		pDoc->WorkingInfo.Motion.sAoiFdVacOff = CharToString(SockData.strData);
		break;
	case _AoiTqVal:
		pDoc->WorkingInfo.Motion.sAoiTq = CharToString(SockData.strData);
		break;
	case _AoiBuffShotNum:
		pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot = CharToString(SockData.strData);
		break;
	case _AoiMkLen:
		pDoc->WorkingInfo.Motion.sFdMkAoiInitDist = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetMkInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _MkLeadPitch:
		pDoc->WorkingInfo.Motion.sMkFdLead = CharToString(SockData.strData);
		break;
	case _MkPushOffLen:
		pDoc->WorkingInfo.Motion.sMkTq = CharToString(SockData.strData);
		break;
	case _MkBuffInitPos:
		pDoc->WorkingInfo.Motion.sStBufPos = CharToString(SockData.strData);
		break;
	case _MkBuffCurrPos:
		pDoc->m_dMkBuffCurrPos = (double)SockData.fData1;
		break;
	case _MkNumLf:
		pDoc->WorkingInfo.Marking[0].nMkCnt = SockData.nData1;
		break;
	case _MkNumRt:
		pDoc->WorkingInfo.Marking[1].nMkCnt = SockData.nData1;
		break;
	case _MkMaxNumLf:
		pDoc->WorkingInfo.Marking[0].nMkLimit = SockData.nData1;
		break;
	case _MkMaxNumRt:
		pDoc->WorkingInfo.Marking[1].nMkLimit = SockData.nData1;
		break;
	default:
		break;
	}
}

void CEngrave::GetMkInfoLf(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _MkInitPosLf:
		pDoc->WorkingInfo.Marking[0].sWaitPos = CharToString(SockData.strData);
		break;
	case _MkInitVelLf:
		pDoc->WorkingInfo.Marking[0].sWaitVel = CharToString(SockData.strData);
		break;
	case _MkInitAccLf:
		pDoc->WorkingInfo.Marking[0].sWaitAcc = CharToString(SockData.strData);
		break;
	case _MkFnlPosLf:
		pDoc->WorkingInfo.Marking[0].sMarkingPos = CharToString(SockData.strData);
		break;
	case _MkFnlVelLf:
		pDoc->WorkingInfo.Marking[0].sMarkingVel = CharToString(SockData.strData);
		break;
	case _MkFnlAccLf:
		pDoc->WorkingInfo.Marking[0].sMarkingAcc = CharToString(SockData.strData);
		break;
	case _MkFnlTqLf:
		pDoc->WorkingInfo.Marking[0].sMarkingToq = CharToString(SockData.strData);
		break;
	case _MkHgtPosX1Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosX[0] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY1Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosY[0] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX2Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosX[1] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY2Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosY[1] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX3Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosX[2] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY3Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosY[2] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX4Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosX[3] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY4Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosY[3] = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetMkInfoRt(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _MkInitPosRt:
		pDoc->WorkingInfo.Marking[1].sWaitPos = CharToString(SockData.strData);
		break;
	case _MkInitVelRt:
		pDoc->WorkingInfo.Marking[1].sWaitVel = CharToString(SockData.strData);
		break;
	case _MkInitAccRt:
		pDoc->WorkingInfo.Marking[1].sWaitAcc = CharToString(SockData.strData);
		break;
	case _MkFnlPosRt:
		pDoc->WorkingInfo.Marking[1].sMarkingPos = CharToString(SockData.strData);
		break;
	case _MkFnlVelRt:
		pDoc->WorkingInfo.Marking[1].sMarkingVel = CharToString(SockData.strData);
		break;
	case _MkFnlAccRt:
		pDoc->WorkingInfo.Marking[1].sMarkingAcc = CharToString(SockData.strData);
		break;
	case _MkFnlTqRt:
		pDoc->WorkingInfo.Marking[1].sMarkingToq = CharToString(SockData.strData);
		break;
	case _MkHgtPosX1Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosX[0] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY1Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosY[0] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX2Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosX[1] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY2Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosY[1] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX3Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosX[2] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY3Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosY[2] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX4Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosX[3] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY4Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosY[3] = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

// End for GetSysInfo()

// Start for SetSysInfo()

BOOL CEngrave::SetSysInfo()
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
	SetNumContFixDef();
	SetUltraSonicStTim();

	/*	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString sVal;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _OpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sSelUserName, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _DualTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDualTest ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _SampleTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bSampleTest ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _SampleShotNum;
	SocketData.nData1 = _ttoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	SendCommand(SocketData);

	SocketData.nMsgID = _TestMode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nTestMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
	SendCommand(SocketData);

	SocketData.nMsgID = _RecoilerCcw;						// OneMetal : TRUE -> SetTwoMetal(FALSE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bOneMetal ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _UncoilerCcw;						// TwoMetal : TRUE -> SetTwoMetal(TRUE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTwoMetal ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _AlignMethode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nAlignMethode; // TWO_POINT, FOUR_POINT
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorRecoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorUncoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _SaftyMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkSftySen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _CleannerAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _CleannerAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _UltraSonicAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _UltraSonicEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseEngraveCleanner ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _TotReelLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sReelTotLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _OnePnlLen;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlLen());
	SendCommand(SocketData);

	SocketData.nMsgID = _TempPause;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _TempStopLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sTempPauseLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotCut;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bLotSep ? 1 : 0;	// pDoc->m_pReelMap->m_bUseLotSep
	SendCommand(SocketData);

	SocketData.nMsgID = _LotCutLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSepLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotCutPosLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotCutPosLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LmtTotYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtTotYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LmtPatlYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtPatlYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _StripOutRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sStripOutRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _CustomNeedRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sCustomNeedRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _NumRangeFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumRangeFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _NumContFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumContFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _UltraSonicStTim;
	StringToChar(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
}

void CEngrave::SetInfo()
{
	SetModelUpName();
	SetModelDnName();
	SetLotUpName();
	SetLotDnName();
	SetLayerUpName();
	SetLayerDnName();
	SetOrderNum();
	SetShotNum();
	SetTotOpRto();
	SetTotVel();
	SetPartVel();
	SetMkDoneLen();
	SetAoiDnDoneLen();
	SetAoiUpDoneLen();
	SetLotSerial();
	SetMkVerfyLen();

	/*SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString sVal;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _OpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sSelUserName, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _ModelUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _ModelDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LayerUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LayerDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	// _OrderNum
	//_ShotNum

	SocketData.nMsgID = _TotReelLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sReelTotLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	// _TotOpRto
	// _LotOpRto
	// _TotVel

	SocketData.nMsgID = _PartVel;
	StringToChar(pDoc->WorkingInfo.LastJob.sPartialSpd, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _TempPause;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _TempStopLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sTempPauseLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotCutLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSepLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	// _MkDoneLen
	// _AoiDnDoneLen
	// _AoiUpDoneLen

	SocketData.nMsgID = _LotSerial;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSerial, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkVerfyLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sVerifyLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
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

	/*	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nGood = 0, nBad = 0, nTot = 0, nVal;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	// < 전체 수율 >

	// 상면
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;

	SocketData.nMsgID = _DefNumUp;
	SocketData.nData1 = nBad;
	SendCommand(SocketData);

	if (nTot > 0)
	dRatio = ((double)nBad / (double)nTot) * 100.0;
	else
	dRatio = 0.0;

	SocketData.nMsgID = _DefRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	SocketData.nMsgID = _GoodNumUp;
	SocketData.nData1 = nGood;
	SendCommand(SocketData);

	if (nTot > 0)
	dRatio = ((double)nGood / (double)nTot) * 100.0;
	else
	dRatio = 0.0;

	SocketData.nMsgID = _GoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	SocketData.nMsgID = _TestNumUp;
	SocketData.nData1 = nTot;
	SendCommand(SocketData);

	if (bDualTest)
	{
	// 하면
	pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;

	SocketData.nMsgID = _DefNumDn;
	SocketData.nData1 = nBad;
	SendCommand(SocketData);

	if (nTot > 0)
	dRatio = ((double)nBad / (double)nTot) * 100.0;
	else
	dRatio = 0.0;

	SocketData.nMsgID = _DefRtoDn;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	SocketData.nMsgID = _GoodNumDn;
	SocketData.nData1 = nGood;
	SendCommand(SocketData);

	if (nTot > 0)
	dRatio = ((double)nGood / (double)nTot) * 100.0;
	else
	dRatio = 0.0;

	SocketData.nMsgID = _GoodRtoDn;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	SocketData.nMsgID = _TestNumDn;
	SocketData.nData1 = nTot;
	SendCommand(SocketData);

	// 전체
	pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;

	SocketData.nMsgID = _DefNumTot;
	SocketData.nData1 = nTot;
	SendCommand(SocketData);

	if (nTot > 0)
	dRatio = ((double)nBad / (double)nTot) * 100.0;
	else
	dRatio = 0.0;

	SocketData.nMsgID = _DefRtoTot;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	SocketData.nMsgID = _GoodNumTot;
	SocketData.nData1 = nGood;
	SendCommand(SocketData);

	if (nTot > 0)
	dRatio = ((double)nGood / (double)nTot) * 100.0;
	else
	dRatio = 0.0;

	SocketData.nMsgID = _GoodRtoTot;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	SocketData.nMsgID = _TestNumTot;
	SocketData.nData1 = nTot;
	SendCommand(SocketData);
	}*/
}

void CEngrave::SetStTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetStTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _LotStTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetRunTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetRunTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _LotRunTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetEdTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetEdTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _LotEdTime;
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
	int nMer[4];
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

	SocketData.nMsgID = _1LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][1] = pDoc->m_pReelMapUp->GetDefStrip(1);
	nSum += nVal[0][1];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][1]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _2LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][2] = pDoc->m_pReelMapUp->GetDefStrip(2);
	nSum += nVal[0][2];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][2]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _3LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][3] = pDoc->m_pReelMapUp->GetDefStrip(3);
	nSum += nVal[0][3];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][3]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _4LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	if (nTot > 0)
		dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _AllLnGoodRtoUp;
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

		SocketData.nMsgID = _1LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][1] = pDoc->m_pReelMapDn->GetDefStrip(1);
		nSum += nVal[1][1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _2LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][2] = pDoc->m_pReelMapDn->GetDefStrip(2);
		nSum += nVal[1][2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _3LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][3] = pDoc->m_pReelMapDn->GetDefStrip(3);
		nSum += nVal[1][3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _4LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _AllLnGoodRtoDn;
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

		SocketData.nMsgID = _1LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[1] = pDoc->m_pReelMapAllUp->GetDefStrip(1);
		nSum += nMer[1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _2LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[2] = pDoc->m_pReelMapAllUp->GetDefStrip(2);
		nSum += nMer[2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _3LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[3] = pDoc->m_pReelMapAllUp->GetDefStrip(3);
		nSum += nMer[3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _4LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _AllLnGoodRtoTot;
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

	CReelMap* pReelMap;

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

	nNum = pReelMap->GetDefNum(DEF_OPEN); // IDC_STC_DEF_OPEN
	SocketData.nMsgID = _DefNumOpen;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_SHORT); // IDC_STC_DEF_SHORT
	SocketData.nMsgID = _DefNumShort;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_USHORT); // IDC_STC_DEF_U_SHORT
	SocketData.nMsgID = _DefNumUshort;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_SPACE); // IDC_STC_DEF_SPACE
	SocketData.nMsgID = _DefNumLnW;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_EXTRA); // IDC_STC_DEF_EXTRA
	SocketData.nMsgID = _DefExtr;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_PROTRUSION); // IDC_STC_DEF_PROT
	SocketData.nMsgID = _DefNumProt;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_PINHOLE); // IDC_STC_DEF_P_HOLE
	SocketData.nMsgID = _DefNumPhole;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_PAD); // IDC_STC_DEF_PAD
	SocketData.nMsgID = _DefNumPad;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_HOLE_OPEN); // IDC_STC_DEF_H_OPEN
	SocketData.nMsgID = _DefNumHopen;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_HOLE_MISS); // IDC_STC_DEF_H_MISS
	SocketData.nMsgID = _DefNumHmiss;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_HOLE_POSITION); // IDC_STC_DEF_H_POS
	SocketData.nMsgID = _DefNumHpos;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_HOLE_DEFECT); // IDC_STC_DEF_H_DEF
	SocketData.nMsgID = _DefNumHdef;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_NICK); // IDC_STC_DEF_NICK
	SocketData.nMsgID = _DefNumNick;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_POI); // IDC_STC_DEF_POI
	SocketData.nMsgID = _DefNumPoi;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_VH_OPEN); // IDC_STC_DEF_VH_OPEN
	SocketData.nMsgID = _DefNumVhOpen;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_VH_MISS); // IDC_STC_DEF_VH_MISS
	SocketData.nMsgID = _DefNumVhMiss;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_VH_POSITION); // IDC_STC_DEF_VH_POS
	SocketData.nMsgID = _DefNumVhPos;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_VH_DEF); // IDC_STC_DEF_VH_DEF
	SocketData.nMsgID = _DefNumVhd;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_LIGHT); // IDC_STC_DEF_LIGHT
	SocketData.nMsgID = _DefNumLight;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_EDGE_NICK);
	SocketData.nMsgID = _DefNumEnick;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_EDGE_PROT);
	SocketData.nMsgID = _DefNumEprot;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_EDGE_SPACE);
	SocketData.nMsgID = _DefNumEspace;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_USER_DEFINE_1);
	SocketData.nMsgID = _DefNumUdd1;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_NARROW);
	SocketData.nMsgID = _DefNumNrw;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_WIDE);
	SocketData.nMsgID = _DefNumWide;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

}

void CEngrave::Set2DReader()
{
	Set2DEngLen();
	Set2DAoiLen();
	Set2DMkLen();
	Set2DMoveVel();
	Set2DMoveAcc();
	Set2DOneShotLen();

	/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _2DEngLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DEngLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMkLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DMoveVel;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DMoveAcc;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DOneShotLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DOneShotRemainLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
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

	/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _EngLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.sEngAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngFdDiffMax;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffMax, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngFdDiffRng;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffRng, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngFdDiffNum;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffInitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngBuffCurrPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffCurrPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
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

	/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _FdVel;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _FdAcc;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _OnePnlLen;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlLen());
	SendCommand(SocketData);

	SocketData.nMsgID = _OnePnlVel;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlVel());
	SendCommand(SocketData);

	SocketData.nMsgID = _OnePnlAcc;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlAcc());
	SendCommand(SocketData);

	SocketData.nMsgID = _FdDiffMax;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrLmt());
	SendCommand(SocketData);

	SocketData.nMsgID = _FdDiffRng;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrRng());
	SendCommand(SocketData);

	SocketData.nMsgID = _FdDiffNum;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrNum());
	SendCommand(SocketData);*/
}

void CEngrave::SetAoiInfo()
{
	SetAoiLeadPitch();
	SetAoiPushOffLen();
	SetAoiTqVal();
	SetAoiBuffShotNum();
	SetAoiMkLen();

	/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _AoiLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _AoiPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _AoiTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _AoiBuffShotNum;
	StringToChar(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _AoiMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
}

void CEngrave::SetMkInfo()
{
	SetMkLeadPitch();
	SetMkPushOffLen();
	SetMkBuffInitPos();
	SetMkBuffCurrPos();
	SetMkNumLf();
	SetMkNumRt();
	SetMkMaxNumLf();
	SetMkMaxNumRt();

	/*	char cData[BUFFER_DATA_SIZE];
	CString str;
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sMkTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sStBufPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkBuffCurrPos;
	double dBufEnc = (double)pDoc->m_pMpeData[0][1] / 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
	//sprintf(SocketData.strData, "%.1f", dBufEnc);
	SocketData.fData1 = (float)dBufEnc;
	SendCommand(SocketData);

	SocketData.nMsgID = _MkNumLf;
	SocketData.nData1 = pDoc->GetMkCntL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntL());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkNumRt;
	SocketData.nData1 = pDoc->GetMkCntR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntR());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkMaxNumLf;
	SocketData.nData1 =  pDoc->GetMkLimitL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitL());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkMaxNumRt;
	SocketData.nData1 = pDoc->GetMkLimitR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitR());
	SendCommand(SocketData);*/
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

	/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkInitPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkInitVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkInitAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlTqLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_1());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_1());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_2());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_2());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_3());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_3());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_4());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_4());
	SendCommand(SocketData);*/
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

	/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkInitPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkInitVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkInitAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlTqRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_1());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_1());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_2());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_2());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_3());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_3());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_4());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_4());
	SendCommand(SocketData);*/
}

// SetOpInfo()
void CEngrave::SetOpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _OpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sSelUserName, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetDualTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DualTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDualTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _SampleTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bSampleTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _SampleShotNum;
	SocketData.nData1 = _ttoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	SendCommand(SocketData);
}

void CEngrave::SetTestMode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _TestMode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nTestMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
	SendCommand(SocketData);
}

void CEngrave::SetRecoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _RecoilerCcw;						// OneMetal : TRUE -> SetTwoMetal(FALSE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bOneMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUncoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _UncoilerCcw;						// TwoMetal : TRUE -> SetTwoMetal(TRUE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTwoMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetAlignMethode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _AlignMethode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nAlignMethode; // TWO_POINT, FOUR_POINT
	SendCommand(SocketData);
}

void CEngrave::SetDoorRecoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorRecoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorUncoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorUncoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSaftyMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _SaftyMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkSftySen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _CleannerAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _CleannerAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _UltraSonicAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _UltraSonicEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseEngraveCleanner ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetTotReelLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _TotReelLen;
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

	SocketData.nMsgID = _OnePnlLen;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlLen());
	SendCommand(SocketData);
}

void CEngrave::SetTempPause()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _TempPause;
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

	SocketData.nMsgID = _TempStopLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sTempPauseLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotCut()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _LotCut;
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

	SocketData.nMsgID = _LotCutLen;
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

	SocketData.nMsgID = _LotCutPosLen;
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

	SocketData.nMsgID = _LmtTotYld;
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

	SocketData.nMsgID = _LmtPatlYld;
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

	SocketData.nMsgID = _StripOutRatio;
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

	SocketData.nMsgID = _CustomNeedRatio;
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

	SocketData.nMsgID = _NumRangeFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumRangeFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetNumContFixDef()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _NumContFixDef;
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

	SocketData.nMsgID = _UltraSonicStTim;
	StringToChar(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, cData);
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

	SocketData.nMsgID = _ModelUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetModelDnName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _ModelDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LotUpName;
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

	SocketData.nMsgID = _LotDnName;
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

	SocketData.nMsgID = _LayerUpName;
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

	SocketData.nMsgID = _LayerDnName;
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

	SocketData.nMsgID = _OrderNum;
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

	SocketData.nMsgID = _ShotNum;
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

	SocketData.nMsgID = _TotOpRto;
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

	SocketData.nMsgID = _TotVel;
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

	SocketData.nMsgID = _PartVel;
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

	SocketData.nMsgID = _MkDoneLen;
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

	SocketData.nMsgID = _AoiDnDoneLen;
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

	SocketData.nMsgID = _AoiUpDoneLen;
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

	SocketData.nMsgID = _LotSerial;
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

	SocketData.nMsgID = _MkVerfyLen;
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

	SocketData.nMsgID = _DefNumUp;
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

	SocketData.nMsgID = _DefRtoUp;
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

	SocketData.nMsgID = _GoodNumUp;
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

	SocketData.nMsgID = _GoodRtoUp;
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

	SocketData.nMsgID = _TestNumUp;
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

		SocketData.nMsgID = _DefNumDn;
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

		SocketData.nMsgID = _DefRtoDn;
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

		SocketData.nMsgID = _GoodNumDn;
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

		SocketData.nMsgID = _GoodRtoDn;
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

		SocketData.nMsgID = _TestNumDn;
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

		SocketData.nMsgID = _DefNumTot;
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

		SocketData.nMsgID = _DefRtoTot;
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

		SocketData.nMsgID = _GoodNumTot;
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

		SocketData.nMsgID = _GoodRtoTot;
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

		SocketData.nMsgID = _TestNumTot;
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

	SocketData.nMsgID = _2DEngLen;
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

	SocketData.nMsgID = _2DAoiLen;
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

	SocketData.nMsgID = _2DMkLen;
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

	SocketData.nMsgID = _2DMoveVel;
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

	SocketData.nMsgID = _2DMoveAcc;
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

	SocketData.nMsgID = _2DOneShotLen;
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

	SocketData.nMsgID = _EngLeadPitch;
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

	SocketData.nMsgID = _EngPushOffLen;
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

	SocketData.nMsgID = _EngTqVal;
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

	SocketData.nMsgID = _EngAoiLen;
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

	SocketData.nMsgID = _EngFdDiffMax;
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

	SocketData.nMsgID = _EngFdDiffRng;
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

	SocketData.nMsgID = _EngFdDiffNum;
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

	SocketData.nMsgID = _EngBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffInitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffCurrPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngBuffCurrPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffCurrPos, cData);
	sprintf(SocketData.strData, "%s", cData);
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

	SocketData.nMsgID = _FdVel;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVel, cData);
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

	SocketData.nMsgID = _FdAcc;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _OnePnlVel;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlVel());
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _OnePnlAcc;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlAcc());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffMax()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _FdDiffMax;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrLmt());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffRng()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _FdDiffRng;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrRng());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _FdDiffNum;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrNum());
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

	SocketData.nMsgID = _AoiLeadPitch;
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

	SocketData.nMsgID = _AoiPushOffLen;
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

	SocketData.nMsgID = _AoiTqVal;
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

	SocketData.nMsgID = _AoiBuffShotNum;
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

	SocketData.nMsgID = _AoiMkLen;
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

	SocketData.nMsgID = _MkLeadPitch;
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

	SocketData.nMsgID = _MkPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVacOff, cData);
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

	SocketData.nMsgID = _MkBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sStBufPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkBuffCurrPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkBuffCurrPos;
	double dBufEnc = (double)pDoc->m_pMpeData[0][1] / 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
																//sprintf(SocketData.strData, "%.1f", dBufEnc);
	SocketData.fData1 = (float)dBufEnc;
	SendCommand(SocketData);
}

void CEngrave::SetMkNumLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkNumLf;
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

	SocketData.nMsgID = _MkNumRt;
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

	SocketData.nMsgID = _MkMaxNumLf;
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

	SocketData.nMsgID = _MkMaxNumRt;
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

	SocketData.nMsgID = _MkInitPosLf;
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

	SocketData.nMsgID = _MkInitVelLf;
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

	SocketData.nMsgID = _MkInitAccLf;
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

	SocketData.nMsgID = _MkFnlPosLf;
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

	SocketData.nMsgID = _MkFnlVelLf;
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

	SocketData.nMsgID = _MkFnlAccLf;
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

	SocketData.nMsgID = _MkFnlTqLf;
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

	SocketData.nMsgID = _MkHgtPosX1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_4());
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

	SocketData.nMsgID = _MkInitPosRt;
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

	SocketData.nMsgID = _MkInitVelRt;
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

	SocketData.nMsgID = _MkInitAccRt;
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

	SocketData.nMsgID = _MkFnlPosRt;
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

	SocketData.nMsgID = _MkFnlVelRt;
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

	SocketData.nMsgID = _MkFnlAccRt;
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

	SocketData.nMsgID = _MkFnlTqRt;
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

	SocketData.nMsgID = _MkHgtPosX1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_4());
	SendCommand(SocketData);
}

// End for SetSysInfo()
