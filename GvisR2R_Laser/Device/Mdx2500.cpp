#include "stdafx.h"
#include "Mdx2500.h"

#include "Mdx2500Def.h"
#include "../Global/GlobalDefine.h"
#include "../GvisR2R_LaserDoc.h"
#include "../GvisR2R_LaserView.h"

extern CGvisR2R_LaserDoc* pDoc;
extern CGvisR2R_LaserView* pView;

CMdx2500::CMdx2500(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent /*=NULL*/)
{
	m_hParentWnd = pParent->GetSafeHwnd();
	//m_strPortSvr = _T("2000");
	//m_strAddrSvr = _T("");
	//m_strAddrCli = _T("");
	m_pClient = NULL;
	m_nServerID = ID_MDX2500;
	m_bWaitForResponse = FALSE;
	m_strResponse = _T("");
	m_bGuideLaserRect = FALSE;
	m_nCmd = MdxNone;
	m_bCmdF = FALSE;

	RECT rt = { 0,0,0,0 };
	Create(NULL, _T("None"), WS_CHILD, rt, FromHandle(m_hParentWnd), (UINT)this);

	StartClient(sAddrCli, sAddrSvr, sPortSvr);
	Sleep(10);

	m_pThread = NULL;
	//StartThread();
}


CMdx2500::~CMdx2500()
{
	if (m_pClient)
	{
		StopClient();
	}
}

BEGIN_MESSAGE_MAP(CMdx2500, CWnd)
	//{{AFX_MSG_MAP(CMdx2500)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_CLIENT_RECEIVED, wmClientReceived)
	ON_MESSAGE(WM_CLIENT_CLOSED, wmClientClosed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CMdx2500::SetHwnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

void CMdx2500::StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr)
{
	if (!m_pClient)
	{
		m_strAddrCli = sAddrCli;
		m_strAddrSvr = sAddrSvr;
		m_strPortSvr = sPortSvr;

		m_pClient = new CTcpIpClient(this);
		m_pClient->Init(sAddrCli, sAddrSvr, _tstoi(sPortSvr));
		m_pClient->Start();
		m_pClient->SetServer(m_nServerID);
	}
}

void CMdx2500::StopClient()
{
	if (m_pClient)
	{
		m_pClient->Stop();
		//if (!m_pClient->Stop()) // Called Destroy Function.
		{
			Sleep(30);
			delete m_pClient;
		}
		m_pClient = NULL;
	}
}

LRESULT CMdx2500::wmClientClosed(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	switch (nServerID)
	{
	case ID_MDX2500:
		if (m_pClient)
			m_pClient = NULL;
		break;
	}

	return (LRESULT)1;
}

LRESULT CMdx2500::wmClientReceived(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)OnClientReceived(wParam, lParam);
}

int CMdx2500::OnClientReceived(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;
	CString sReceived = (LPCTSTR)lParam;

	if (sReceived == _T("Fail to connect."))
		return 1;

	switch (nServerID)
	{
	case ID_MDX2500:
		if (sReceived.GetLength() > 0)
		{
			if(CheckResponse(m_nCmd, sReceived))
			{
				;
			}
			else
			{
				pView->MsgBox(_T("Error-CheckResponse"));
				//AfxMessageBox(_T("Error-CheckResponse"));
			}			
		}
		break;
	}

	return 1;
}

BOOL CMdx2500::CheckResponse(int nCmd, CString sResponse)
{
	int nPos;
	CString sParsing;

	switch(nCmd)
	{
	case MdxIsReady:
		if(nPos = sResponse.Find(_T("OK")) > 0)
		{
			//sParsing = sResponse.Right(nPos + 3);
			//m_strResponse = sParsing;
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
			if(m_hParentWnd)
				::PostMessage(m_hParentWnd, WM_CLIENT_RECEIVED_MDX, (WPARAM)MdxIsReady, (LPARAM)&m_strResponse);
		}
		else
		{
			//pView->MsgBox(_T("Error-Mdx response"));
			//AfxMessageBox(_T("Error-Mdx response"));
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
		}
		return TRUE;
	case GuideLaser:
		if (nPos = sResponse.Find(_T("OK")) > 0)
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
		}
		else
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
			pView->MsgBox(_T("Error-Mdx response"));
			//AfxMessageBox(_T("Error-Mdx response"));
		}
		return TRUE;
	case StopMarking:
		if(nPos = sResponse.Find(_T("OK")) > 0)
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
		}
		else
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
			//AfxMessageBox(_T("Error-Mdx response"));
			pView->MsgBox(_T("Error-Mdx response"));
		}
		return TRUE;
	case StartMarking:
		if (nPos = sResponse.Find(_T("OK")) > 0)
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
		}
		else
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
			//AfxMessageBox(_T("Error-Mdx response"));
			pView->MsgBox(_T("Error-Mdx response"));
		}
		return TRUE;
	case SetCharacterString:
		if (nPos = sResponse.Find(_T("OK")) > 0)
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
		}
		else
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
			//AfxMessageBox(_T("Error-Mdx response"));
			pView->MsgBox(_T("Error-Mdx response"));
		}
		return TRUE;
	case GetCharacterString:
		if (nPos = sResponse.Find(_T("OK")) > 0)
		{
			sParsing = sResponse.Right(nPos+3);
			m_strResponse = sParsing;
			m_bWaitForResponse = FALSE;
			if (m_hParentWnd)
				::PostMessage(m_hParentWnd, WM_CLIENT_RECEIVED_MDX, (WPARAM)GetCharacterString, (LPARAM)&m_strResponse);
		}
		else
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
			//AfxMessageBox(_T("Error-Mdx response"));
			pView->MsgBox(_T("Error-Mdx response"));
		}
		return TRUE;
	case SetJobPosition:
		if (nPos = sResponse.Find(_T("OK")) > 0)
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
		}
		else
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
			//AfxMessageBox(_T("Error-Mdx response"));
			pView->MsgBox(_T("Error-Mdx response"));
		}
		return TRUE;
	}
	
	return FALSE;
}


BOOL CMdx2500::GetMdxResponse(CString &sData)
{
	if (!m_bWaitForResponse)
	{
		sData = m_strResponse;
		return TRUE;
	}

	return FALSE;
}


void CMdx2500::ResetWaitForResponse()
{
	m_bWaitForResponse = FALSE;
}

BOOL CMdx2500::IsRunning()
{
	return (m_bWaitForResponse);
}

BOOL CMdx2500::GuideLaserRect(BOOL bOn)
{
	if (m_bWaitForResponse)
	{
		//AfxMessageBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		pView->MsgBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		return FALSE;
	}

	CString sMsg;
	m_bGuideLaserRect = bOn;

	if (m_pClient)
	{
		if (bOn)
		{
			sMsg = _T(MDX_GuideLaserRect);	// _T("WX,GuideLaser=5");
			if (m_pClient->WriteComm(sMsg) > 0)
			{
				m_nCmd = GuideLaser;
				m_strResponse = _T("");
				m_bWaitForResponse = TRUE;
				return TRUE;
			}
		}
		else
		{
			sMsg = _T(MDX_StopMarking);	// _T("WX,StopMarking");
			if (m_pClient->WriteComm(sMsg) > 0)
			{
				m_nCmd = StopMarking;
				m_strResponse = _T("");
				m_bWaitForResponse = TRUE;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CMdx2500::IsLaserReady()
{
	if (m_bWaitForResponse)
	{
		//AfxMessageBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		pView->MsgBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		return FALSE;
	}

	CString sMsg;

	if (m_pClient)
	{
		sMsg = _T(MDX_IsReady);	// _T("RX,Ready");
		if (m_pClient->WriteComm(sMsg) > 0)
		{
			m_nCmd = MdxIsReady;
			m_strResponse = _T("");
			m_bWaitForResponse = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMdx2500::LaserMarking(BOOL bOn)
{
	if (m_bWaitForResponse)
	{
		//AfxMessageBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		pView->MsgBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		return FALSE;
	}

	CString sMsg;

	if (m_pClient)
	{
		if (bOn)
		{
			sMsg = _T(MDX_StartMarking);	// _T("WX,StartMarking");
			if (m_pClient->WriteComm(sMsg) > 0)
			{
				m_nCmd = StartMarking;
				m_strResponse = _T("");
				m_bWaitForResponse = TRUE;
				return TRUE;
			}
		}
		else
		{
			sMsg = _T(MDX_StopMarking);	// _T("WX,StopMarking");
			if (m_pClient->WriteComm(sMsg) > 0)
			{
				m_nCmd = StopMarking;
				m_strResponse = _T("");
				m_bWaitForResponse = TRUE;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CMdx2500::SetMdxOrderShotNum(CString sOrder, CString sShot, int nSet)
{
	if (m_bWaitForResponse)
	{
		//AfxMessageBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		pView->MsgBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		return FALSE;
	}

	if ((sOrder.GetLength() != 9) && (nSet & SET_ITS))
	{
		//AfxMessageBox(_T("Error - ITS CODE는 9자여야 합니다."));
		pView->MsgBox(_T("Error - ITS CODE는 9자여야 합니다."));
		return FALSE;
	}
	else if ((sShot.GetLength() != 3) && (nSet & SET_SHOT))
	{
		//AfxMessageBox(_T("Error - Shot넘버는 3자여야 합니다."));
		pView->MsgBox(_T("Error - Shot넘버는 3자여야 합니다."));
		return FALSE;
	}
	else
	{
		//pDoc->m_sShotNum = sData;

		CString sMsg, sCharacterString;

		if (m_pClient)
		{
			sCharacterString.Format(_T("%s-%s"), sOrder, sShot);
			sMsg.Format(_T(MDX_SetCharacterString), 0, 0, sCharacterString);	// _T("WX,JOB=%04d,BLK=%03d,CharacterString=%s");
			if (m_pClient->WriteComm(sMsg) > 0)
			{
				m_nCmd = SetCharacterString;
				m_strResponse = _T("");
				m_bWaitForResponse = TRUE;

				//CString sPath = PATH_WORKING_INFO;
				//pDoc->WorkingInfo.LastJob.sEngraveLastShot = pDoc->m_sShotNum;
				//::WritePrivateProfileString(_T("Last Job"), _T("Engrave Order Num"), sOrder, sPath);
				//::WritePrivateProfileString(_T("Last Job"), _T("Engrave Last Shot"), sShot, sPath);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CMdx2500::GetMdxSettingData(CString& sOrderNum, CString& sShotNum)
{
	if (m_bWaitForResponse)
	{
		//AfxMessageBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		pView->MsgBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		return FALSE;
	}

	CString sMsg;

	if (m_pClient)
	{
		sMsg.Format(_T(MDX_GetCharacterString));	// _T("RX,JOB=%04d,BLK=%03d,CharacterString");
		if (m_pClient->WriteComm(sMsg) > 0)
		{
			m_nCmd = GetCharacterString;
			m_strResponse = _T("");
			m_bWaitForResponse = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMdx2500::SetLaserPosZero()
{
	if (m_bWaitForResponse)
	{
		//AfxMessageBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		pView->MsgBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		return FALSE;
	}

	CString sMsg, sJobPos[5]; // X_org,Y_org,X_offset,Y_offset,Theta_offset [8bytes]

	if (m_pClient)
	{
		// _T("WX,JOB=%04d,JobPosition=%s,%s,%s,%s,%s");
		sJobPos[0].Format(_T("%04.03f"), 0.0);	// X_org
		sJobPos[1].Format(_T("%04.03f"), 0.0);	// Y_org
		sJobPos[2].Format(_T("%04.03f"), 0.0);	// X_offset
		sJobPos[3].Format(_T("%04.03f"), 0.0);	// Y_offset
		sJobPos[4].Format(_T("%04.03f"), 0.0);	// Theta_offset
		sMsg.Format(_T(MDX_SetJobPosition), 0, sJobPos[0], sJobPos[1], sJobPos[2], sJobPos[3], sJobPos[4]);
		if (m_pClient->WriteComm(sMsg) > 0)
		{
			m_nCmd = SetJobPosition;
			m_strResponse = _T("");
			m_bWaitForResponse = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMdx2500::SetLaserPos(double* pData)
{
	if (m_bWaitForResponse)
	{
		//AfxMessageBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		pView->MsgBox(_T("Error - m_bWaitForResponse[ID_MDX2500] is TRUE."));
		return FALSE;
	}

	CString sMsg, sJobPos[5]; // X_org,Y_org,X_offset,Y_offset,Theta_offset [8bytes]

	if (m_pClient)
	{
		// _T("WX,JOB=%04d,JobPosition=%s,%s,%s,%s,%s");
		sJobPos[0].Format(_T("%04.03f"), pData[0]);				// X_org
		sJobPos[1].Format(_T("%04.03f"), pData[1]);				// Y_org
		sJobPos[2].Format(_T("%04.03f"), pData[2]);				// X_offset
		sJobPos[3].Format(_T("%04.03f"), pData[3]);				// Y_offset
		sJobPos[4].Format(_T("%04.03f"), pData[4]);				// Theta_offset
		sMsg.Format(_T(MDX_SetJobPosition), 0, sJobPos[0], sJobPos[1], sJobPos[2], sJobPos[3], sJobPos[4]);
		if (m_pClient->WriteComm(sMsg) > 0)
		{
			m_nCmd = SetJobPosition;
			m_strResponse = _T("");
			m_bWaitForResponse = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

int CMdx2500::WriteComm(CString sMsg, DWORD dwTimeout)
{
	return m_pClient->WriteComm(sMsg);
}

// Thread
void CMdx2500::StartThread() // Worker Thread 구동관련 Step8
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

void CMdx2500::StopThread() // Worker Thread 구동관련 Step9
{
	if (m_pThread != NULL)
	{
		m_evtThread.SetEvent();
		WaitUntilThreadEnd(m_hThread);
	}
	m_pThread = NULL;
	m_bModify = FALSE;
}

void CMdx2500::WaitUntilThreadEnd(HANDLE hThread) // Worker Thread 구동관련 Step6
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

UINT CMdx2500::RunThread(LPVOID pParam)
{
	CMdx2500* pDlg = (CMdx2500*)pParam;
	DWORD dwMilliseconds = 10;
	CString strTemp;

	static int dHour, dMin, dSec, dSubSec;

	DWORD dwCurTick;
	DWORD dwTimeOut;
	pDlg->m_bAlive = TRUE;
	MSG message;
	while (WAIT_OBJECT_0 != ::WaitForSingleObject(pDlg->m_evtThread, dwMilliseconds))
	{
		Sleep(10);


	}
	pDlg->m_bAlive = FALSE;
	return 0;
}

void CMdx2500::Close()
{
	StopThread();
	Sleep(10);
}

BOOL CMdx2500::IsConnected()
{
	return m_pClient->IsConnected();
}

BOOL CMdx2500::StartLaserMarking()
{
	if (WaitUntilLaserReady())
	{
		// Start Marking Request												: (1) 2D 코드를 인쇄하기
		if (LaserMarking(TRUE)) // TRUE: Marking Start , FALSE: Marking Stop
		{
			if (WaitForResponse(60000, 100))
			{
				// Marking Complete 2											: (2) 인쇄가 완료된 것을 확인
				if (WaitUntilLaserReady())
				{

				}

				// 2DC Read Request												: (3) 2D 코드를 판독

				// 2DC Read Complete											: (4) 2D 코드의 판독이 완료된 것을 확인

				// 2DC Read Grade Status										: (5) 2D 코드의 판정결과를 읽기
			}
		}
	}

	return FALSE;
}

BOOL CMdx2500::WaitUntilLaserReady()
{
	MSG message;
	DWORD dwTimeOut = 10000;
	DWORD dwStartTick = GetTickCount();
	DWORD dwTick;
	while (1)
	{
		dwTick = GetTickCount() - dwStartTick;
		if (dwTick > dwTimeOut)
		{
			pView->MsgBox(_T("Error - WaitUntilLaserReady[ID_MDX2500] is time over 10seconds."));
			return FALSE;
		}

		if (IsLaserReady())
		{
			if (WaitForResponse())
			{
				if (IsOK())
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CMdx2500::IsOK()
{
	int nPos;
	if (nPos = m_strResponse.Find(_T("OK")) > 0)
		return TRUE;

	return FALSE;
}

BOOL CMdx2500::WaitForResponse(DWORD dwTimeOut, DWORD dwTimeLate)
{
	MSG message;
	DWORD dwStartTick = GetTickCount();
	DWORD dwTick;
	while (IsRunning()) // m_bWaitForResponse = TRUE;
	{
		dwTick = GetTickCount() - dwStartTick;
		if (dwTick > dwTimeOut)
		{
			pView->MsgBox(_T("Error - WaitForResponse[ID_MDX2500] is time over."));
			return FALSE;
		}
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(30);
	}
	Sleep(dwTimeLate);

	return TRUE;
}