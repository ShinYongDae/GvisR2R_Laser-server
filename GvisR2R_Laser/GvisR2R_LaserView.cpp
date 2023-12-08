
// GvisR2R_LaserView.cpp : CGvisR2R_LaserView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GvisR2R_Laser.h"
#endif

#include "GvisR2R_LaserDoc.h"
#include "GvisR2R_LaserView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"

#include "Process/DataFile.h"

#include "Dialog/DlgMyPassword.h"
#include "Dialog/DlgMyMsgSub00.h"
#include "Dialog/DlgMyMsgSub01.h"
#include "Dialog/DlgMyMsgSub02.h"
#include "Dialog/DlgProgress.h"

#include "Device/MDX2500DEF.h"
#include "Device/SR1000WDEF.h"

extern CMainFrame* pFrm;
extern CGvisR2R_LaserDoc* pDoc;
CGvisR2R_LaserView* pView;
extern CString PATH_WORKING_INFO;

// CGvisR2R_LaserView

IMPLEMENT_DYNCREATE(CGvisR2R_LaserView, CFormView)

BEGIN_MESSAGE_MAP(CGvisR2R_LaserView, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(WM_DLG_INFO, OnDlgInfo)
	ON_MESSAGE(WM_MYMSG_EXIT, OnMyMsgExit)
	ON_MESSAGE(WM_CLIENT_RECEIVED_MDX, wmClientReceivedMdx)
	ON_MESSAGE(WM_CLIENT_RECEIVED_SR, wmClientReceivedSr)
	ON_MESSAGE(WM_SERVER_RECEIVED, wmServerReceived)
	//ON_MESSAGE(WM_CLIENT_CLOSED, wmClientClosed)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CGvisR2R_LaserView 생성/소멸

CGvisR2R_LaserView::CGvisR2R_LaserView()
	: CFormView(IDD_GVISR2R_LASER_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	pView = this;

	int i = 0;

	for (i = 0; i < _SigInx::_EndIdx; i++)
	{
		m_bRcvSig[i] = FALSE;
	}

	m_bShift2Mk = FALSE;

	m_bBufEmpty[0] = FALSE;
	m_bBufEmpty[1] = FALSE;

//	m_nDebugStep = 0;
	m_nWatiDispMain = 0;
	m_nNewLot = 0;


	m_bStopFromThread = FALSE;
	m_bBuzzerFromThread = FALSE;

	m_nWatiDispMain = 0;

	m_bStopFromThread = FALSE;
	m_bBuzzerFromThread = FALSE;

	m_nRtnMyMsgBoxIdx = -1;
	m_bDispMyMsgBox = FALSE;
	m_bDispMain = FALSE;
	m_bProbDn[0] = m_bProbDn[1] = FALSE;

	m_nSelRmap = RMAP_UP;
	m_nSelRmapInner = RMAP_INNER_UP;

	m_bTIM_INIT_VIEW = FALSE;
	m_nStepInitView = 0;
	m_nLotEndSerial = 0;

	m_nDummy[0] = 0;
	m_nDummy[1] = 0;
	m_nAoiLastSerial[0] = 0;
	m_nAoiLastSerial[1] = 0;
	m_bChkLastProcVs = FALSE;

	m_dTempPauseLen = 0.0;

	m_bInit = FALSE;
	m_bDispMsg = FALSE;
	for (i = 0; i < 10; i++)
	{
		m_bDispMsgDoAuto[i] = FALSE;
		m_nStepDispMsg[i] = 0;
	}
	m_sFixMsg[0] = _T("");
	m_sFixMsg[1] = _T("");

	m_bWaitClrDispMsg = FALSE;
	m_bOpenShareUp = TRUE;
	m_bOpenShareDn = TRUE;

	m_bStopFeeding = FALSE;

	// 	m_pMpe = NULL;
	m_pDlgMyMsg = NULL;
	m_pDlgMsgBox = NULL;

	m_pDlgInfo = NULL;
	m_pDlgFrameHigh = NULL;
	m_pDlgMenu01 = NULL;
	m_pDlgMenu02 = NULL;
	m_pDlgMenu03 = NULL;
	m_pDlgMenu04 = NULL;
	m_pDlgMenu05 = NULL;

	m_nCntTowerWinker = 0;
	m_bTimTowerWinker = FALSE;
	m_bTimBtnWinker = FALSE;
	m_bTimBuzzerWarn = FALSE;
	m_bTowerWinker[0] = FALSE; // R
	m_bTowerWinker[1] = FALSE; // G
	m_bTowerWinker[2] = FALSE; // Y
	m_bBtnWinker[0] = FALSE; // Ready
	m_bBtnWinker[1] = FALSE; // Reset
	m_bBtnWinker[2] = FALSE; // Run
	m_bBtnWinker[3] = FALSE; // Stop
	for (i = 0; i < 4; i++)
	{
		m_nCntBtnWinker[i] = 0;
		m_nDlyWinker[i] = 5;
	}

	m_bAoiFdWriteF[0] = FALSE;
	m_bAoiFdWriteF[1] = FALSE;
	m_bAoiTest[0] = FALSE;
	m_bAoiTest[1] = FALSE;


	// H/W Device 초기화.....
	m_pMpe = NULL;
	m_pMotion = NULL;
	m_pLight = NULL;

#ifdef USE_VISION
	m_pVision[0] = NULL;
	m_pVision[1] = NULL;
#endif

	m_bTIM_DISP_STATUS = FALSE;

	m_bThread[0] = FALSE;
	m_dwThreadTick[0] = 0;
	m_bThread[1] = FALSE;
	m_dwThreadTick[1] = 0;
	m_bThread[2] = FALSE;
	m_dwThreadTick[2] = 0;

	m_bTIM_MPE_IO = FALSE;


	m_nStepAuto = 0;
	m_nPrevStepAuto = 0;
	m_nPrevMkStAuto = 0;
	m_nStepMk[0] = 0;
	m_nStepMk[1] = 0;

	m_sShare[0] = _T("");
	m_sBuf[0] = _T("");
	m_sShare[1] = _T("");
	m_sBuf[1] = _T("");

	m_sTick = _T("");
	m_sDispTime = _T("");

	m_bChkMpeIoOut = FALSE;

	m_bMkTmpStop = FALSE;
	m_bAoiLdRun = TRUE;
	m_bAoiLdRunF = FALSE;

	m_dwCycSt = 0;
	m_dwCycTim = 0;

	m_bTHREAD_DISP_DEF = FALSE;
	m_nStepTHREAD_DISP_DEF = 0;

	m_bTHREAD_MK[0] = FALSE;
	m_bTHREAD_MK[1] = FALSE;
	m_bTHREAD_MK[2] = FALSE;
	m_bTHREAD_MK[3] = FALSE;
	m_nMkPcs[0] = 0;
	m_nMkPcs[1] = 0;
	m_nMkPcs[2] = 0;
	m_nMkPcs[3] = 0;

	m_nErrCnt = 0;

	m_bAuto = FALSE;
	m_bManual = FALSE;
	m_bOneCycle = FALSE;

	m_bSwRun = FALSE; m_bSwRunF = FALSE;
	m_bSwStop = FALSE; m_bSwStopF = FALSE;
	m_bSwReset = FALSE; m_bSwResetF = FALSE;
	m_bSwReady = FALSE; m_bSwReadyF = FALSE;

	m_bSwStopNow = FALSE;

	for (int nAxis = 0; nAxis < MAX_AXIS; nAxis++)
		m_dEnc[nAxis] = 0.0;

	for (i = 0; i < 10; i++)
		m_sDispMsg[i] = _T("");

	m_bNewModel = FALSE;
	m_dTotVel = 0.0; m_dPartVel = 0.0;
	m_bTIM_CHK_TEMP_STOP = FALSE;
	m_bTIM_SAFTY_STOP = FALSE;
	m_bTIM_TCPIP_UPDATE = FALSE;
	m_bTIM_START_UPDATE = FALSE;
	m_bTIM_MENU01_UPDATE_WORK = FALSE;
	m_sMyMsg = _T("");
	m_nTypeMyMsg = IDOK;

	m_dwLotSt = 0; m_dwLotEd = 0;

	m_lFuncId = 0;

	m_bDrawGL = TRUE;
	m_bCont = FALSE;
	m_bCam = FALSE;
	m_bReview = FALSE;

	m_bChkBufIdx[0] = TRUE;
	m_nChkBufIdx[0] = 0;
	m_bChkBufIdx[1] = TRUE;
	m_nChkBufIdx[1] = 0;

	m_dwStMkDn[0] = 0;
	m_dwStMkDn[1] = 0;
	m_nVsBufLastSerial[0] = 0;
	m_nVsBufLastSerial[1] = 0;
	m_bShowModalMyPassword = FALSE;

	// 	m_bFurgeRun = FALSE;
	// 	m_nFurgeRun = 0;

	m_nRstNum = 0;
	m_bBufHomeDone = FALSE;
	m_bReadyDone = FALSE;

	//ResetPriority();
	m_Flag = 0L;
	m_AoiLdRun = 0L;

	m_bDoneDispMkInfo[0][0] = FALSE; // Cam0, Up
	m_bDoneDispMkInfo[0][1] = FALSE; // Cam0, Dn
	m_bDoneDispMkInfo[1][0] = FALSE; // Cam1, Up
	m_bDoneDispMkInfo[1][1] = FALSE; // Cam1, Dn

	m_nShareUpS = 0;
	m_nShareUpSerial[0] = 0;
	m_nShareUpSerial[1] = 0;
	m_nShareUpCnt = 0;

	m_nShareDnS = 0;
	m_nShareDnSerial[0] = 0;
	m_nShareDnSerial[1] = 0;
	m_nShareDnCnt = 0;

	m_nBufSerial[0][0] = 0; // Up-Cam0
	m_nBufSerial[0][1] = 0; // Up-Cam1
	m_nBufSerial[1][0] = 0; // Dn-Cam0
	m_nBufSerial[1][1] = 0; // Dn-Cam0

	m_bReAlign[0][0] = FALSE; // [nCam][nPos] 
	m_bReAlign[0][1] = FALSE; // [nCam][nPos] 
	m_bReAlign[1][0] = FALSE; // [nCam][nPos] 
	m_bReAlign[1][1] = FALSE; // [nCam][nPos] 

	m_bSkipAlign[0][0] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[0][1] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[1][0] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[1][1] = FALSE; // [nCam][nPos] 

	m_bFailAlign[0][0] = FALSE; // [nCam][nPos] 
	m_bFailAlign[0][1] = FALSE; // [nCam][nPos] 
	m_bFailAlign[1][0] = FALSE; // [nCam][nPos] 
	m_bFailAlign[1][1] = FALSE; // [nCam][nPos] 

	m_bDoMk[0] = TRUE;
	m_bDoMk[1] = TRUE;
	m_bDoneMk[0] = FALSE;
	m_bDoneMk[1] = FALSE;
	m_bReMark[0] = FALSE;
	m_bReMark[1] = FALSE;

	m_nMonAlmF = 0;
	m_nClrAlmF = 0;

	m_bMkSt = FALSE;
	m_bMkStSw = FALSE;
	m_nMkStAuto = 0;

	m_bEngSt = FALSE;
	m_bEngStSw = FALSE;
	m_nEngStAuto = 0;

	m_bEng2dSt = FALSE;
	m_bEng2dStSw = FALSE;
	m_nEng2dStAuto = 0;

	m_bLotEnd = FALSE;
	m_nLotEndAuto = 0;

	m_bLastProc = FALSE;
	m_bLastProcFromUp = TRUE;
	m_nLastProcAuto = 0;

	m_bLoadShare[0] = FALSE;
	m_bLoadShare[1] = FALSE;

	m_sNewLotUp = _T("");
	m_sNewLotDn = _T("");

	m_bAoiFdWrite[0] = FALSE;
	m_bAoiFdWrite[1] = FALSE;
	m_bAoiFdWriteF[0] = FALSE;
	m_bAoiFdWriteF[1] = FALSE;

	m_bCycleStop = FALSE;

	for (int a = 0; a < 2; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			m_nMkStrip[a][b] = 0;
			m_bRejectDone[a][b] = FALSE;
		}
	}

	m_sDispMain = _T("");
	m_bReMk = FALSE;

	m_bWaitPcr[0] = FALSE;
	m_bWaitPcr[1] = FALSE;

	m_bShowMyMsg = FALSE;
	m_pMyMsgForeground = NULL;
	m_bContDiffLot = FALSE;

	// 	m_nMsgShiftX = 0;
	// 	m_nMsgShiftY = 0;

	for (int nAns = 0; nAns < 10; nAns++)
		m_bAnswer[nAns] = FALSE;

	m_bChkLightErr = FALSE;

	// client for SR-1000W
	m_pSr1000w = NULL;

	// client for MD-X2500
	m_pMdx2500 = NULL;

	// server for engrave
	m_pEngrave = NULL;

	m_bDestroyedView = FALSE;
	m_bContEngraveF = FALSE;

	m_bStopF_Verify = FALSE;

	m_bLoadMstInfo = FALSE;
	m_bLoadMstInfoF = FALSE;

	m_pDlgUtil01 = NULL;
	//m_pDlgUtil02 = NULL;
	m_bEngStop = FALSE;

	m_sGetItsCode = _T("");
	m_nGetItsCodeSerial = 0;

	m_bSetSig = FALSE;
	m_bSetSigF = FALSE;
	m_bSetData = FALSE;
	m_bSetDataF = FALSE;

	m_sMonDisp = _T("");
}

CGvisR2R_LaserView::~CGvisR2R_LaserView()
{
	DestroyView();
}

void CGvisR2R_LaserView::DestroyView()
{
	if (!m_bDestroyedView)
	{
		m_bDestroyedView = TRUE;

		ThreadKill();
		Sleep(30);

		DelAllDlg();
		Sleep(100);

		Buzzer(FALSE, 0);
		Buzzer(FALSE, 1);

#ifdef USE_VISION
		if (m_pVision[1])
		{
			delete m_pVision[1];
			m_pVision[1] = NULL;
		}

		if (m_pVision[0])
		{
			delete m_pVision[0];
			m_pVision[0] = NULL;
		}
#endif

		m_bTIM_MPE_IO = FALSE;
		m_bTIM_DISP_STATUS = FALSE;
		m_bTIM_INIT_VIEW = FALSE;
		Sleep(100);


		InitIoWrite();
		SetMainMc(FALSE);

		// H/W Device 소멸.....
		HwKill();

		CloseMyMsg();

		if (m_ArrayMyMsgBox.GetSize() > 0)
		{
			m_ArrayMyMsgBox.RemoveAll();
		}
	}
}

void CGvisR2R_LaserView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CGvisR2R_LaserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CGvisR2R_LaserView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	InitMyMsg();

	pDoc->LoadWorkingInfo();
	pDoc->LoadIoInfo();
	pDoc->LoadSignalInfo();
	pDoc->LoadDataInfo();
	if (!LoadMySpec())
		LoadMySpec();
	pDoc->GetCurrentInfo();

#ifdef USE_CAM_MASTER
	CFileFind finder;
	CString sDir, sMsg;
	sDir = pDoc->WorkingInfo.System.sPathCamSpecDir;
	sDir.Delete(sDir.GetLength() - 1, 1);
	sDir.ReleaseBuffer();

	//TCHAR szFile[MAX_PATH] = { 0, };
	//_stprintf(szFile, _T("%s\\*.*"), sDir);

	//if (!finder.FindFile(sDir)) // Check 1st Spec folder is
	if(!pDoc->DirectoryExists(sDir))
	{
		sMsg.Format(_T("캠마스터에 스펙폴더가 없습니다. : \n 1.SpecFolder : %s"), sDir);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg, MB_ICONSTOP | MB_OK);
		ExitProgram();
		return;
	}
#endif

	if (!m_bTIM_INIT_VIEW)
	{
		m_nStepInitView = 0;
		m_bTIM_INIT_VIEW = TRUE;
		SetTimer(TIM_INIT_VIEW, 300, NULL);
	}

}


// CGvisR2R_LaserView 진단

#ifdef _DEBUG
void CGvisR2R_LaserView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGvisR2R_LaserView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGvisR2R_LaserDoc* CGvisR2R_LaserView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGvisR2R_LaserDoc)));
	return (CGvisR2R_LaserDoc*)m_pDocument;
}
#endif //_DEBUG


// CGvisR2R_LaserView 메시지 처리기


void CGvisR2R_LaserView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nInc = 0; int nSrl;
	CString str, sMsg, sPath;
	//BOOL bExist;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (nIDEvent == TIM_INIT_VIEW)
	{
		KillTimer(TIM_INIT_VIEW);

		switch (m_nStepInitView)
		{
		case 0:
			m_nStepInitView++;
			DispMsg(_T("프로그램을 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
	
			// H/W Device 초기화.....
			HwInit();
			break;
		case 1:
			m_nStepInitView++;
			InitIO();
			break;
		case 2:
			m_nStepInitView++;
			InitIoWrite();
			SetMainMc(TRUE);

			m_nMonAlmF = 0;
			m_nClrAlmF = 0;

			break;
		case 3:
			m_nStepInitView++;
			ThreadInit();
			break;
		case 4:
			m_nStepInitView++;
			break;
		case 5:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 1"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_02);
			break;
		case 6:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.-2"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_01);
			break;
		case 7:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 3"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			break;
		case 8:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 4"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_03);
			break;
		case 9:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 5"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_04);
			break;
		case 10:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 6"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_05);
			break;
		case 11:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 7"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_FRAME_HIGH);
			if (m_pDlgFrameHigh)
				m_pDlgFrameHigh->ChkMenu01();
			SetDualTest(pDoc->WorkingInfo.LastJob.bDualTest);
			break;
		case 12:
			Init(); // AmpReset()
			Sleep(300);
			m_nStepInitView++;
			break;
		case 13:
			m_nStepInitView++;
			DispMsg(_T("H/W를 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			InitAct();
			m_bStopFeeding = TRUE;
#ifdef USE_MPE
			if (m_pMpe)
				m_pMpe->Write(_T("MB440115"), 1); // 마킹부Feeding금지
#endif
			Sleep(300);
			break;
		case 14:
			if (m_pMotion)
			{
				DispMsg(_T("Homming"), _T("Searching Home Position..."), RGB_GREEN, 2000, TRUE);
				m_pMotion->SearchHome();
				m_nStepInitView++;
			}
			else
			{
				m_bTIM_INIT_VIEW = FALSE;
				AfxMessageBox(_T("Motion is failed."));
				PostMessage(WM_CLOSE);
			}
			break;
		case 15:
			if (m_pMotion)
			{
				if (m_pMotion->IsHomeDone())// && m_pMotion->IsHomeDone(MS_MKFD))
				{
					m_nStepInitView++;
				}

				sMsg.Format(_T("X0(%s) , Y0(%s)"), m_pMotion->IsHomeDone(MS_X0) ? _T("Done") : _T("Doing"),
					m_pMotion->IsHomeDone(MS_Y0) ? _T("Done") : _T("Doing"));
				DispMsg(sMsg, _T("Homming"), RGB_GREEN, 2000, TRUE);
				Sleep(300);
			}
			else
			{
				m_bTIM_INIT_VIEW = FALSE;
				AfxMessageBox(_T("Motion is failed."));
				PostMessage(WM_CLOSE);
			}
			break;
		case 16:
			m_nStepInitView++;
			m_bLoadMstInfo = TRUE;
			pDoc->m_bLoadMstInfo[0] = TRUE;
			pDoc->m_bLoadMstInfo[1] = TRUE;
			m_bTIM_START_UPDATE = TRUE;
			SetTimer(TIM_START_UPDATE, 50, NULL);
			break;
		case 17:
			m_nStepInitView++;
			DispMsg(_T("Completed Searching Home Pos..."), _T("Homming"), RGB_GREEN, 2000, TRUE);
			m_pMotion->ObjectMapping();
			break;
		case 18:
			TcpIpInit();
			m_nStepInitView++;
			break;
		case 19:
			if (m_bLoadMstInfo)
			{
				Sleep(100);
				break;
			}

			m_nStepInitView++;
			SetAlignPos();
			break;
		case 20:
			m_nStepInitView++;
			//m_bTIM_START_UPDATE = TRUE;
			//SetTimer(TIM_START_UPDATE, 50, NULL);
			//m_bTIM_TCPIP_UPDATE = TRUE;
			//SetTimer(TIM_TCPIP_UPDATE, 500, NULL);
			break;

		case 21:
			m_nStepInitView++;
			m_bStopFeeding = FALSE;
#ifdef USE_MPE
			if (m_pMpe)
				m_pMpe->Write(_T("MB440115"), 0); // 마킹부Feeding금지
#endif
			if(m_pDlgMenu02)
				m_pDlgMenu02->SetJogSpd(_tstoi(pDoc->WorkingInfo.LastJob.sJogSpd));
			if (m_pDlgMenu03)
				m_pDlgMenu03->InitRelation();
			//m_pMotion->SetFeedRate(MS_X0Y0, 1.0);
			//Sleep(30);
			//m_pMotion->SetFeedRate(MS_X1Y1, 1.0);
			m_pMotion->SetR2RConf();
			//m_pMotion->SetTorque(AXIS_MKTQ, _tstof(pDoc->WorkingInfo.Motion.sMkTq));
			//m_pMotion->SetTorque(AXIS_AOITQ, _tstof(pDoc->WorkingInfo.Motion.sAoiTq));
			//TowerLamp(RGB_YELLOW, FALSE, TRUE);
			TowerLamp(RGB_YELLOW, TRUE);
			//TowerLamp(RGB_RED, TRUE);

			//if(!SetCollision(-10.0))
			if (!SetCollision(-1.0*_tstof(pDoc->WorkingInfo.Motion.sCollisionLength)))
			{
				DispMsg(_T("Collision"), _T("Failed to Set Collision ..."), RGB_GREEN, 2000, TRUE);
			}

			break;
		case 22:
			m_nStepInitView++;
			//MoveMkInitPos();
			InitPLC();
			SetPlcParam();
			GetPlcParam();
			//TcpIpInit();
			m_bTIM_DISP_STATUS = TRUE;
			SetTimer(TIM_DISP_STATUS, 100, NULL);
			break;
		case 23:
			m_nStepInitView++;
			ClrDispMsg();
			if (m_pDlgMenu01)
			{
				m_pDlgMenu01->SetStripAllMk();
				m_pDlgMenu01->RefreshRmap();
				m_pDlgMenu01->ResetLastProc();
			}
			if (!MemChk())
				AfxMessageBox(_T("Memory Error - Cam Spec Data : PCR[0] or PCR[1] or Reelmap"));
			else
			{
				if (pDoc->m_pReelMap)
				{
#ifndef TEST_MODE
					ReloadRst();
					UpdateRst();
#endif
					UpdateLotTime();
				}
			}

			// Check Encoder
			if (!m_bThread[1])
				m_Thread[1].Start(GetSafeHwnd(), this, ThreadProc1);

			// DispDefImg
			if (!m_bThread[2])
				m_Thread[2].Start(GetSafeHwnd(), this, ThreadProc2);

			//MoveInitPos1();
			//Sleep(30);
			MoveInitPos0();

			SetLotLastShot();
			StartLive();

			pDoc->SetEngraveLastShot(pDoc->GetCurrentInfoEngShotNum());

			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (m_pDlgMenu01)
					m_pDlgMenu01->EnableItsMode();
			}

			m_bTIM_MPE_IO = TRUE;
			SetTimer(TIM_MPE_IO, 50, NULL);

			m_bTIM_INIT_VIEW = FALSE;
			break;
		}

		if (m_bTIM_INIT_VIEW)
			SetTimer(TIM_INIT_VIEW, 100, NULL);
	}

	// 	if(nIDEvent == TIM_MK_START)
	// 	{
	// 		KillTimer(TIM_MK_START);
	// 		Marking();
	// 		if(m_bTIM_MK_START)
	// 			SetTimer(TIM_MK_START, 50, NULL);
	// 	}

	if (nIDEvent == TIM_MPE_IO)
	{
		KillTimer(TIM_MPE_IO);

		//CntMk();
		//GetMpeIO();
		//GetMpeSignal();
		//GetMpeData();

		DoIO(); // DoAutoEng()

		ChkMyMsg();

		if (m_bTIM_MPE_IO)
			SetTimer(TIM_MPE_IO, 100, NULL);
	}

	if (nIDEvent == TIM_TOWER_WINKER)
	{
		KillTimer(TIM_TOWER_WINKER);
		DispTowerWinker();
		if (m_bTimTowerWinker)
			SetTimer(TIM_TOWER_WINKER, 100, NULL);
	}

	// 	if(nIDEvent == TIM_BTN_WINKER)
	// 	{
	// 		KillTimer(TIM_BTN_WINKER);
	// 		DispBtnWinker();
	// 		if(m_bTimBtnWinker)
	// 			SetTimer(TIM_BTN_WINKER, 100, NULL);
	// 	}

	if (nIDEvent == TIM_BUZZER_WARN)
	{
		KillTimer(TIM_BUZZER_WARN);
		m_nCntBz++;
		if (m_nCntBz > BUZZER_DELAY)
		{
			m_bTimBuzzerWarn = FALSE;
			Buzzer(FALSE);
		}
		if (m_bTimBuzzerWarn)
			SetTimer(TIM_BUZZER_WARN, 100, NULL);
	}

	if (nIDEvent == TIM_DISP_STATUS)
	{
		KillTimer(TIM_DISP_STATUS);

		DispStsBar();
		DoDispMain();
		//DispMyMsgBox();

		if (m_bStopFromThread)
		{
			m_bStopFromThread = FALSE;
			Stop();
		}
		if (m_bBuzzerFromThread)
		{
			m_bBuzzerFromThread = FALSE;
			Buzzer(TRUE, 0);
		}

		//		SetMpeIO();

		ChkEmg();
		ChkSaftySen();
		ChkDoor();
		ChkRcvSig();

		if (m_bTIM_DISP_STATUS)
			SetTimer(TIM_DISP_STATUS, 100, NULL);
	}


	if (nIDEvent == TIM_SHOW_MENU01)
	{
		KillTimer(TIM_SHOW_MENU01);
		if (m_pDlgFrameHigh)
			m_pDlgFrameHigh->ChkMenu01();
	}

	if (nIDEvent == TIM_SHOW_MENU02)
	{
		KillTimer(TIM_SHOW_MENU02);
		if (m_pDlgFrameHigh)
			m_pDlgFrameHigh->ChkMenu02();
	}

	if (nIDEvent == TIM_CHK_TEMP_STOP)
	{
		KillTimer(TIM_CHK_TEMP_STOP);
#ifdef USE_MPE
		if (!(pDoc->m_pMpeSignal[7] & (0x01 << 3)))	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
		{
			m_bTIM_CHK_TEMP_STOP = FALSE;
			m_pDlgMenu01->SetTempStop(FALSE);
		}
#endif
		if (m_bTIM_CHK_TEMP_STOP)
			SetTimer(TIM_CHK_TEMP_STOP, 500, NULL);
	}

	if (nIDEvent == TIM_SAFTY_STOP)
	{
		KillTimer(TIM_SAFTY_STOP);
		MsgBox(_T("일시정지 - 마킹부 안전센서가 감지되었습니다."));
		m_bTIM_SAFTY_STOP = FALSE;
	}

	if (nIDEvent == TIM_START_UPDATE)
	{
		KillTimer(TIM_START_UPDATE);

		if (m_bLoadMstInfo && !m_bLoadMstInfoF)
		{
			//if (m_bTIM_TCPIP_UPDATE)
			if (!pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() && !pDoc->WorkingInfo.LastJob.sLayerUp.IsEmpty())
			{
				m_bLoadMstInfoF = TRUE;
				SetTimer(TIM_TCPIP_UPDATE, 500, NULL);
			}
			else
				m_bLoadMstInfo = FALSE;
		}

		if (m_bSetSig && !m_bSetSigF)
		{
			m_bSetSigF = TRUE;

			if (m_pEngrave->m_bGetOpInfo || m_pEngrave->m_bGetInfo)
			{
				if (m_pDlgInfo)
					m_pDlgInfo->UpdateData();

				if (m_pDlgMenu01)
					m_pDlgMenu01->UpdateData();

				m_pEngrave->m_bGetOpInfo = FALSE;
				m_pEngrave->m_bGetInfo = FALSE;
			}

			//if (m_pDlgMenu03)
			//	m_pDlgMenu03->UpdateSignal();

			m_bSetSig = FALSE;
		}
		else if (!m_bSetSig && m_bSetSigF)
		{
			m_bSetSigF = FALSE;
		}

		if (m_bSetData && !m_bSetDataF)
		{
			m_bSetDataF = TRUE;

			if (m_pEngrave->m_bGetOpInfo || m_pEngrave->m_bGetInfo)
			{
				if (m_pDlgInfo)
					m_pDlgInfo->UpdateData();

				if (m_pDlgMenu01)
					m_pDlgMenu01->UpdateData();

				m_pEngrave->m_bGetOpInfo = FALSE;
				m_pEngrave->m_bGetInfo = FALSE;
			}

			if (m_pDlgMenu02)
				m_pDlgMenu02->UpdateData();

			if (m_pDlgMenu03)
				m_pDlgMenu03->UpdateData();

			if (m_pDlgMenu04)
				m_pDlgMenu04->UpdateData();

			m_bSetData = FALSE;
		}
		else if (!m_bSetData && m_bSetDataF)
		{
			m_bSetDataF = FALSE;
		}

		if (m_bTIM_START_UPDATE)
			SetTimer(TIM_START_UPDATE, 100, NULL);
	}

	if (nIDEvent == TIM_TCPIP_UPDATE)
	{
		KillTimer(TIM_TCPIP_UPDATE);
		LoadMstInfo();
		if (m_pDlgMenu01)
			m_pDlgMenu01->UpdateData();
		m_bLoadMstInfoF = FALSE;
		m_bLoadMstInfo = FALSE;
	}

	if (nIDEvent == TIM_MENU01_UPDATE_WORK)
	{
		KillTimer(TIM_MENU01_UPDATE_WORK);
		pDoc->GetMkMenu01();
		if (m_pDlgMenu01)
		{
			m_pDlgMenu01->UpdateData();
			m_pDlgMenu01->UpdateWorking();
			m_pDlgMenu01->DispTotRatio();
			m_pDlgMenu01->DispStripRatio();
			m_pDlgMenu01->DispDef();
			m_pDlgMenu01->DispMkCnt();
		}
	}

	CFormView::OnTimer(nIDEvent);
}

void CGvisR2R_LaserView::InitMyMsg()
{
	if (m_pDlgMyMsg)
		CloseMyMsg();

	m_pDlgMyMsg = new CDlgMyMsg(this);
	m_pDlgMyMsg->Create();
}

void CGvisR2R_LaserView::CloseMyMsg()
{
	if (m_pDlgMyMsg)
	{
		delete m_pDlgMyMsg;
		m_pDlgMyMsg = NULL;
	}
}

LRESULT CGvisR2R_LaserView::OnMyMsgExit(WPARAM wPara, LPARAM lPara)
{
	Buzzer(FALSE, 0);
	ClrAlarm();
	return 0L;
}

int CGvisR2R_LaserView::MsgBox(CString sMsg, int nThreadIdx, int nType, int nTimOut)
{
	int nRtnVal = -1; // Reply(-1) is None.
	if (m_pDlgMyMsg)
		nRtnVal = m_pDlgMyMsg->SyncMsgBox(sMsg, nThreadIdx, nType, nTimOut);

	return nRtnVal;
}

int CGvisR2R_LaserView::AsyncMsgBox(CString sMsg, int nThreadIdx, int nType, int nTimOut)
{
	int nRtn = -1;
	if (m_pDlgMyMsg)
		m_pDlgMyMsg->AsyncMsgBox(sMsg, nThreadIdx, nType, nTimOut);
	return nRtn;
}

int CGvisR2R_LaserView::WaitRtnVal(int nThreadIdx)
{
	int nRtn = -1;
	if (m_pDlgMyMsg)
		nRtn = m_pDlgMyMsg->WaitRtnVal(nThreadIdx);
	return nRtn;
}






void CGvisR2R_LaserView::ChkMyMsg()
{
	return;

	//HWND hwndForeground;
	CWnd *pWndForeground;

	if (m_bShowMyMsg && m_pMyMsgForeground)
	{
		pWndForeground = pFrm->GetForegroundWindow();
		if (pWndForeground != m_pMyMsgForeground)
			m_pMyMsgForeground->SetForegroundWindow();
	}
}

void CGvisR2R_LaserView::UpdateLotTime()
{
	m_dwLotSt = (DWORD)pDoc->WorkingInfo.Lot.dwStTick;

	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateLotTime();
}

void CGvisR2R_LaserView::DispStsBar(CString sMsg, int nIdx)
{
	if (m_sDispMsg[nIdx] != sMsg)
		m_sDispMsg[nIdx] = sMsg;
	sMsg.Empty();
}

void CGvisR2R_LaserView::DispStsBar()
{
	DispStsMainMsg(); // 0
	//DispStsMainMsg(6); // 6
	//DispStsMainMsg(5); // 5
	//DispThreadTick(); // 5, 6
	DispTime(); // 7
	ChkShare(); // 2, 4
	ChkBuf(); // 1, 3
}

BOOL CGvisR2R_LaserView::MemChk()
{
	if (!pDoc->m_pPcr[0] || !pDoc->m_pPcr[1])// || !pDoc->m_pReelMap)
		return FALSE;
	return TRUE;
}

void CGvisR2R_LaserView::SetMainMc(BOOL bOn)
{
	if (bOn)
	{
		if (m_pMpe)
			m_pMpe->Write(_T("MB440159"), 1); // 마킹부 Door Interlock ON
			//m_pMpe->Write(_T("MB440159"), 1); // 마킹부 MC ON
	}
	else
	{
		if (m_pMpe)
			m_pMpe->Write(_T("MB440158"), 0); // 마킹부 Door Interlock OFF
			//m_pMpe->Write(_T("MB440159"), 0); // 마킹부 MC OFF
	}
}

void CGvisR2R_LaserView::ExitProgram()
{
	long lParam = 0;
	long lData = 1;
	lParam = lParam | lData;
	lData = 0x00 << 16;
	lParam = lParam | lData;
	lData = 1 << 29;
	lParam = lParam | lData;
	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT);
}

void CGvisR2R_LaserView::Init()
{
	int nAxis;
	if (m_pMotion)
	{
		for (nAxis = 0; nAxis < m_pMotion->m_ParamCtrl.nTotAxis; nAxis++)
		{
			m_pMotion->AmpReset(nAxis);
			Sleep(30);
		}
	}
}

void CGvisR2R_LaserView::InitIO()
{
	//int i, k;
	//// 	pDoc->m_nSliceIo = pDoc->MkIo.SliceIo.nMaxSeg; // pDoc->MkIo.SliceIo.nInSeg;

	//pDoc->m_nMpeIo = pDoc->MkIo.MpeIo.nMaxSeg;
	//pDoc->m_nMpeI = pDoc->MkIo.MpeIo.nMaxSeg;

	//if (!pDoc->m_pMpeI)
	//{
	//	if (pDoc->m_nMpeI > 0)
	//	{
	//		pDoc->m_pMpeI = new unsigned short[pDoc->m_nMpeI];
	//		for (i = 0; i < pDoc->m_nMpeI; i++)
	//			pDoc->m_pMpeI[i] = 0;
	//	}
	//}

	//if (!pDoc->m_pMpeIF)
	//{
	//	if (pDoc->m_nMpeI > 0)
	//	{
	//		pDoc->m_pMpeIF = new unsigned short[pDoc->m_nMpeI];
	//		for (i = 0; i < pDoc->m_nMpeI; i++)
	//			pDoc->m_pMpeIF[i] = 0;
	//	}
	//}

	//if (!pDoc->m_pMpeIo)
	//{
	//	if (pDoc->m_nMpeIo > 0)
	//	{
	//		pDoc->m_pMpeIo = new unsigned short[pDoc->m_nMpeIo];
	//		for (i = 0; i < pDoc->m_nMpeIo; i++)
	//			pDoc->m_pMpeIo[i] = 0;
	//	}
	//}

	//if (!pDoc->m_pMpeIoF)
	//{
	//	if (pDoc->m_nMpeIo > 0)
	//	{
	//		pDoc->m_pMpeIoF = new unsigned short[pDoc->m_nMpeIo];
	//		for (i = 0; i < pDoc->m_nMpeIo; i++)
	//			pDoc->m_pMpeIoF[i] = 0;
	//	}
	//}


	//pDoc->m_nMpeSignal = pDoc->MkIo.MpeSignal.nMaxSeg;
	//if (!pDoc->m_pMpeSignal)
	//{
	//	if (pDoc->m_nMpeSignal > 0)
	//	{
	//		pDoc->m_pMpeSignal = new unsigned short[pDoc->m_nMpeSignal];
	//		for (i = 0; i < pDoc->m_nMpeSignal; i++)
	//			pDoc->m_pMpeSignal[i] = 0;
	//	}
	//}


	//pDoc->m_nMpeData = pDoc->MkIo.MpeData.nMaxSeg;
	//if (!pDoc->m_pMpeData)
	//{
	//	if (pDoc->m_nMpeData > 0)
	//	{
	//		pDoc->m_pMpeData = new long*[pDoc->m_nMpeData];
	//		for (i = 0; i < pDoc->m_nMpeData; i++)
	//		{
	//			pDoc->m_pMpeData[i] = new long[16];
	//			for (k = 0; k < 16; k++)
	//				pDoc->m_pMpeData[i][k] = 0;
	//		}
	//	}
	//}


	//// 	if(!pDoc->m_pSliceIo)
	//// 	{
	//// 		if(pDoc->m_nSliceIo>0)
	//// 		{
	//// 			pDoc->m_pSliceIo = new unsigned short[pDoc->m_nSliceIo];
	//// 			for(i=0; i<pDoc->m_nSliceIo; i++)
	//// 				pDoc->m_pSliceIo[i] = 0;
	//// 		}
	//// 	}
	//// 	if(!pDoc->m_pSliceIoF)
	//// 	{
	//// 		if(pDoc->m_nSliceIo>0)
	//// 		{
	//// 			pDoc->m_pSliceIoF = new unsigned short[pDoc->m_nSliceIo];
	//// 			for(i=0; i<pDoc->m_nSliceIo; i++)
	//// 				pDoc->m_pSliceIoF[i] = 0;
	//// 		}
	//// 	}
	//// 	if(!pDoc->m_pPrevSliceIo)
	//// 	{
	//// 		if(pDoc->m_nSliceIo>0)
	//// 		{
	//// 			pDoc->m_pPrevSliceIo = new unsigned short[pDoc->m_nSliceIo];
	//// 			for(i=0; i<pDoc->m_nSliceIo; i++)
	//// 				pDoc->m_pPrevSliceIo[i] = 0;
	//// 		}
	//// 	}
	//// 
	//// 	if(!pDoc->m_pSliceMpeIo)
	//// 	{
	//// 		if(pDoc->m_nSliceIo>0)
	//// 		{
	//// 			pDoc->m_pSliceMpeIo = new unsigned short[pDoc->m_nSliceIo];
	//// 			for(i=0; i<pDoc->m_nSliceIo; i++)
	//// 				pDoc->m_pSliceMpeIo[i] = 0;
	//// 		}
	//// 	}
	//// 	if(!pDoc->m_pSliceMpeIoF)
	//// 	{
	//// 		if(pDoc->m_nSliceIo>0)
	//// 		{
	//// 			pDoc->m_pSliceMpeIoF = new unsigned short[pDoc->m_nSliceIo];
	//// 			for(i=0; i<pDoc->m_nSliceIo; i++)
	//// 				pDoc->m_pSliceMpeIoF[i] = 0;
	//// 		}
	//// 	}
	//// 	if(!pDoc->m_pPrevSliceMpeIo)
	//// 	{
	//// 		if(pDoc->m_nSliceIo>0)
	//// 		{
	//// 			pDoc->m_pPrevSliceMpeIo = new unsigned short[pDoc->m_nSliceIo];
	//// 			for(i=0; i<pDoc->m_nSliceIo; i++)
	//// 				pDoc->m_pPrevSliceMpeIo[i] = 0;
	//// 		}
	//// 	}

	//// 	if(!pDoc->m_pPrevMpeIo)
	//// 	{
	//// 		if(pDoc->m_nMpeIo>0)
	//// 		{
	//// 			pDoc->m_pPrevMpeIo = new unsigned short[pDoc->m_nMpeIo];
	//// 			for(i=0; i<pDoc->m_nMpeIo; i++)
	//// 				pDoc->m_pPrevMpeIo[i] = 0;
	//// 		}
	//// 	}
}

BOOL CGvisR2R_LaserView::InitAct()
{
#ifdef USE_XMP
	if (!m_pMotion)
		return FALSE;
#endif

#ifdef USE_LIGHT
	if (!m_pLight)
		return FALSE;
#endif

#ifdef USE_FLUCK
	if (!m_pFluck)
		return FALSE;
#endif

	int nAxis;

	if (m_pMotion)
	{
		// 
		// // 		for(int nMsId=0; nMsId<m_pMotion->m_ParamCtrl.nTotMotion; nMsId++)
		// 		for(nAxis=0; nAxis<m_pMotion->m_ParamCtrl.nTotAxis; nAxis++)
		// 		{
		// // 			m_pMotion->AmpReset(nMsId);
		// 			m_pMotion->AmpReset(nAxis);
		// 			Sleep(30);
		// 		}

		// Motor On
		//		for(nAxis=0; nAxis<1; nAxis++)
		for (nAxis = 0; nAxis < m_pMotion->m_ParamCtrl.nTotAxis; nAxis++)
		{
			m_pMotion->ServoOnOff(nAxis, TRUE);
			Sleep(100);
		}

		double dX[2], dY[2];

		if (pDoc->m_pSpecLocal && IsPinData())
		{
			dX[0] = pDoc->m_pSpecLocal->m_dPinPosX[0];
			dY[0] = pDoc->m_pSpecLocal->m_dPinPosY[0];
			dX[1] = pDoc->m_pSpecLocal->m_dPinPosX[1];
			dY[1] = pDoc->m_pSpecLocal->m_dPinPosY[1];
		}
		else
		{
			dX[0] = _tstof(pDoc->WorkingInfo.Motion.sPinPosX[0]);
			dY[0] = _tstof(pDoc->WorkingInfo.Motion.sPinPosY[0]);
			dX[1] = _tstof(pDoc->WorkingInfo.Motion.sPinPosX[1]);
			dY[1] = _tstof(pDoc->WorkingInfo.Motion.sPinPosY[1]);
		}
		m_pMotion->SetPinPos(0, dX[0], dY[0]);
		m_pMotion->SetPinPos(1, dX[1], dY[1]);
		m_pMotion->m_dStBufPos = _tstof(pDoc->WorkingInfo.Motion.sStBufPos);


		CfPoint ptPnt0(dX[0], dY[0]);
		if (pDoc->m_Master[0].m_pPcsRgn)
			pDoc->m_Master[0].m_pPcsRgn->SetPinPos(0, ptPnt0);

		CfPoint ptPnt1(dX[1], dY[1]);
		if (pDoc->m_Master[0].m_pPcsRgn)
			pDoc->m_Master[0].m_pPcsRgn->SetPinPos(1, ptPnt1);

		if (pDoc->m_pSpecLocal)// && IsMkOffsetData())
		{
			// 			m_pDlgMenu02->SetPcsOffset();
			// 			CfPoint ptOfst(0.0, 0.0);
			pDoc->SetMkPnt(CAM_BOTH);
		}

		double dPos = _tstof(pDoc->WorkingInfo.Motion.sStBufPos);
		SetBufInitPos(dPos);
		double dVel = _tstof(pDoc->WorkingInfo.Motion.sBufHomeSpd);
		double dAcc = _tstof(pDoc->WorkingInfo.Motion.sBufHomeAcc);
		SetBufHomeParam(dVel, dAcc);
		// 		m_pMotion->SetStBufPos(dPos);

	}

	// Light On
	if (m_pDlgMenu02)
	{
		m_pDlgMenu02->SetLight(_tstoi(pDoc->WorkingInfo.Light.sVal[0]));
	}

	// Homming
	//if (m_pVoiceCoil[0])
	//	m_pVoiceCoil[0]->SearchHomeSmac(0);
	//if (m_pVoiceCoil[1])
	//	m_pVoiceCoil[1]->SearchHomeSmac(1);


	return TRUE;
}

BOOL CGvisR2R_LaserView::TcpIpInit()
{
#ifdef USE_MDX2500
	if (!m_pMdx2500)
	{
		m_pMdx2500 = new CMdx2500(pDoc->WorkingInfo.System.sIpClient[ID_MDX2500], pDoc->WorkingInfo.System.sIpServer[ID_MDX2500], pDoc->WorkingInfo.System.sPort[ID_MDX2500], this);
		//m_pMdx2500->SetHwnd(this->GetSafeHwnd());
	}
#endif

#ifdef USE_SR1000W
	if (!m_pSr1000w)
	{
		m_pSr1000w = new CSr1000w(pDoc->WorkingInfo.System.sIpClient[ID_SR1000W], pDoc->WorkingInfo.System.sIpServer[ID_SR1000W], pDoc->WorkingInfo.System.sPort[ID_SR1000W], this);
		//m_pSr1000w->SetHwnd(this->GetSafeHwnd());
	}
#endif

#ifdef USE_TCPIP
	if (!m_pEngrave)
	{
		m_pEngrave = new CEngrave(pDoc->WorkingInfo.System.sIpClient[ID_PUNCH], pDoc->WorkingInfo.System.sIpServer[ID_ENGRAVE], pDoc->WorkingInfo.System.sPort[ID_ENGRAVE], this);
		m_pEngrave->SetHwnd(this->GetSafeHwnd());
	}
#endif

	return TRUE;
}

BOOL CGvisR2R_LaserView::HwInit()
{
	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = NULL;
	}

	m_pLight = new CLight(this);
	m_pLight->Init();

	if (m_pMotion)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}
	m_pMotion = new CMotion(this);
	if (!m_pMotion->InitBoard())
	{
		//		DoMyMsgBox(_T("XMP 보드 초기화 실패, 다시 시작하세요.!!!"));
		MsgBox(_T("XMP 보드 초기화 실패, 다시 시작하세요.!!!"));
		PostMessage(WM_CLOSE);
		return FALSE;
	}

#ifdef USE_MPE
	if (!m_pMpe)
		m_pMpe = new CMpDevice(this);
	if (!m_pMpe->Init(1, 1))
	{
		//		DoMyMsgBox(_T("메카트로링크(MC0)의 통신프로그램을 실행 후, 다시 시작하세요.!!!"));
		MsgBox(_T("메카트로링크(MC0)의 통신프로그램을 실행 후, 다시 시작하세요.!!!"));
		PostMessage(WM_CLOSE);
		return FALSE;
	}
#endif

	//StartClient_MDX2500(pDoc->WorkingInfo.System.sIpClient[0], pDoc->WorkingInfo.System.sIpServer[0], pDoc->WorkingInfo.System.sPort[0]);
	//StartClient_SR1000W(pDoc->WorkingInfo.System.sIpClient[1], pDoc->WorkingInfo.System.sIpServer[1], pDoc->WorkingInfo.System.sPort[1]);
//#ifdef USE_MDX2500
//	if (!m_pMdx2500)
//	{
//		m_pMdx2500 = new CMdx2500(pDoc->WorkingInfo.System.sIpClient[ID_MDX2500], pDoc->WorkingInfo.System.sIpServer[ID_MDX2500], pDoc->WorkingInfo.System.sPort[ID_MDX2500], this);
//		//m_pMdx2500->SetHwnd(this->GetSafeHwnd());
//	}
//#endif
//
//#ifdef USE_SR1000W
//	if (!m_pSr1000w)
//	{
//		m_pSr1000w = new CSr1000w(pDoc->WorkingInfo.System.sIpClient[ID_SR1000W], pDoc->WorkingInfo.System.sIpServer[ID_SR1000W], pDoc->WorkingInfo.System.sPort[ID_SR1000W], this);
//		//m_pSr1000w->SetHwnd(this->GetSafeHwnd());
//	}
//#endif
//
//#ifdef USE_TCPIP
//	if (!m_pEngrave)
//	{
//		m_pEngrave = new CEngrave(pDoc->WorkingInfo.System.sIpClient[ID_PUNCH], pDoc->WorkingInfo.System.sIpServer[ID_ENGRAVE], pDoc->WorkingInfo.System.sPort[ID_ENGRAVE], this);
//		m_pEngrave->SetHwnd(this->GetSafeHwnd());
//	}
//#endif

	return TRUE;
}

void CGvisR2R_LaserView::HwKill()
{
	// 	if(m_pVision[1])
	// 	{
	// 		delete m_pVision[1];
	// 		m_pVision[1] = NULL;
	// 	}
	// 
	// 	if(m_pVision[0])
	// 	{
	// 		delete m_pVision[0];
	// 		m_pVision[0] = NULL;
	// 	}

	if (m_pMotion)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}

	if (m_pMpe)
	{
		delete m_pMpe;
		m_pMpe = NULL;
	}

	if (m_pLight)
	{
		m_pLight->Close();
		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pEngrave)
	{
		m_pEngrave->Close();
		delete m_pEngrave;
		m_pEngrave = NULL;
	}


	if (m_pSr1000w)
	{
		m_pSr1000w->Close();
		delete m_pSr1000w;
		m_pSr1000w = NULL;
	}

	if (m_pMdx2500)
	{
		m_pMdx2500->Close();
		delete m_pMdx2500;
		m_pMdx2500 = NULL;
	}
}

void CGvisR2R_LaserView::GetDispMsg(CString &strMsg, CString &strTitle)
{
	if (m_pDlgMsgBox)
		m_pDlgMsgBox->GetDispMsg(strMsg, strTitle);
}

void CGvisR2R_LaserView::DispMsg(CString strMsg, CString strTitle, COLORREF color, DWORD dwDispTime, BOOL bOverWrite)
{
	if (m_bDispMsg)
		return;

	if (m_bAuto)
	{
		return;
	}

	m_bDispMsg = TRUE;

	if (dwDispTime == 0)
	{
		dwDispTime = 24 * 3600 * 1000;
	}

	if (m_pDlgMsgBox != NULL)
	{
		if (bOverWrite)
		{
			if(m_pDlgMsgBox)
			m_pDlgMsgBox->SetDispMsg(strMsg, strTitle, dwDispTime, color);
		}
		if (m_pDlgMsgBox)
		m_pDlgMsgBox->ShowWindow(SW_SHOW);
		if (m_pDlgMsgBox)
		m_pDlgMsgBox->SetFocus();
		if (m_pDlgMsgBox)
		((CButton*)m_pDlgMsgBox->GetDlgItem(IDOK))->SetCheck(TRUE);
	}
	else
	{
		m_pDlgMsgBox = new CDlgMsgBox(this, strTitle, strMsg, dwDispTime, color);
		if (m_pDlgMsgBox->GetSafeHwnd() == 0)
		{
			m_pDlgMsgBox->Create();
			m_pDlgMsgBox->ShowWindow(SW_SHOW);
			m_pDlgMsgBox->SetDispMsg(strMsg, strTitle, dwDispTime, color);
			m_pDlgMsgBox->SetFocus();
			((CButton*)m_pDlgMsgBox->GetDlgItem(IDOK))->SetCheck(TRUE);
		}
	}

	m_bDispMsg = FALSE;
}

void CGvisR2R_LaserView::ClrDispMsg()
{
	OnQuitDispMsg(NULL, NULL);
}

BOOL CGvisR2R_LaserView::WaitClrDispMsg()
{
	m_bWaitClrDispMsg = TRUE;
	MSG message;

	DWORD dwMilliseconds = 0; // 100ms sec sleep
	while (WAIT_OBJECT_0 != ::WaitForSingleObject(m_evtWaitClrDispMsg, dwMilliseconds) && m_pDlgMsgBox != NULL)
	{
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	};
	Sleep(10);
	m_bWaitClrDispMsg = FALSE;
	return TRUE;
}

LONG CGvisR2R_LaserView::OnQuitDispMsg(UINT wParam, LONG lParam)
{
	if (m_pDlgMsgBox)
	{
		if (m_pDlgMsgBox->GetSafeHwnd())
			m_pDlgMsgBox->DestroyWindow();
		delete m_pDlgMsgBox;
		m_pDlgMsgBox = NULL;
	}

	return 0L;
}

void CGvisR2R_LaserView::ShowDlg(int nID)
{
	HideAllDlg();

	switch (nID)
	{
	case IDD_DLG_INFO:
		if (!m_pDlgInfo)
		{
			m_pDlgInfo = new CDlgInfo(this);
			if (m_pDlgInfo->GetSafeHwnd() == 0)
			{
				m_pDlgInfo->Create();
				m_pDlgInfo->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgInfo->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_FRAME_HIGH:
		if (!m_pDlgFrameHigh)
		{
			m_pDlgFrameHigh = new CDlgFrameHigh(this);
			if (m_pDlgFrameHigh->GetSafeHwnd() == 0)
			{
				m_pDlgFrameHigh->Create();
				m_pDlgFrameHigh->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgFrameHigh->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_01:
		if (!m_pDlgMenu01)
		{
			m_pDlgMenu01 = new CDlgMenu01(this);
			if (m_pDlgMenu01->GetSafeHwnd() == 0)
			{
				m_pDlgMenu01->Create();
				m_pDlgMenu01->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu01->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_02:
		if (!m_pDlgMenu02)
		{
			m_pDlgMenu02 = new CDlgMenu02(this);
			if (m_pDlgMenu02->GetSafeHwnd() == 0)
			{
				m_pDlgMenu02->Create();
				m_pDlgMenu02->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu02->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_03:
		if (!m_pDlgMenu03)
		{
			m_pDlgMenu03 = new CDlgMenu03(this);
			if (m_pDlgMenu03->GetSafeHwnd() == 0)
			{
				m_pDlgMenu03->Create();
				m_pDlgMenu03->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu03->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_04:
		if (!m_pDlgMenu04)
		{
			m_pDlgMenu04 = new CDlgMenu04(this);
			if (m_pDlgMenu04->GetSafeHwnd() == 0)
			{
				m_pDlgMenu04->Create();
				m_pDlgMenu04->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu04->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_05:
		if (!m_pDlgMenu05)
		{
			m_pDlgMenu05 = new CDlgMenu05(this);
			if (m_pDlgMenu05->GetSafeHwnd() == 0)
			{
				m_pDlgMenu05->Create();
				m_pDlgMenu05->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu05->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_UTIL_01:
		// 		if(!m_pDlgUtil01)
		// 		{
		// 			m_pDlgUtil01 = new CDlgUtil01(this);
		// 			if(m_pDlgUtil01->GetSafeHwnd() == 0)
		// 			{
		// 				m_pDlgUtil01->Create();
		// 				m_pDlgUtil01->ShowWindow(SW_SHOW);
		// 			}
		// 		}
		// 		else
		// 		{
		// 			m_pDlgUtil01->ShowWindow(SW_SHOW);
		// 		}
		break;
	case IDD_DLG_UTIL_02:
		//if (!m_pDlgUtil02)
		//{
		//	m_pDlgUtil02 = new CDlgUtil02(this);
		//	if (m_pDlgUtil02->GetSafeHwnd() == 0)
		//	{
		//		m_pDlgUtil02->Create();
		//		m_pDlgUtil02->ShowWindow(SW_SHOW);
		//	}
		//}
		//else
		//{
		//	m_pDlgUtil02->ShowWindow(SW_SHOW);
		//}
		break;
	}
}

void CGvisR2R_LaserView::HideAllDlg()
{
	if (m_pDlgUtil01 && m_pDlgUtil01->GetSafeHwnd())
	{
		if (m_pDlgUtil01->IsWindowVisible())
			m_pDlgUtil01->ShowWindow(SW_HIDE);
	}
	//if (m_pDlgUtil02 && m_pDlgUtil02->GetSafeHwnd())
	//{
	//	if (m_pDlgUtil02->IsWindowVisible())
	//		m_pDlgUtil02->ShowWindow(SW_HIDE);
	//}
	if (m_pDlgInfo && m_pDlgInfo->GetSafeHwnd())
	{
		if (m_pDlgInfo->IsWindowVisible())
			m_pDlgInfo->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu01 && m_pDlgMenu01->GetSafeHwnd())
	{
		if (m_pDlgMenu01->IsWindowVisible())
			m_pDlgMenu01->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu02 && m_pDlgMenu02->GetSafeHwnd())
	{
		if (m_pDlgMenu02->IsWindowVisible())
			m_pDlgMenu02->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu03 && m_pDlgMenu03->GetSafeHwnd())
	{
		if (m_pDlgMenu03->IsWindowVisible())
			m_pDlgMenu03->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu04 && m_pDlgMenu04->GetSafeHwnd())
	{
		if (m_pDlgMenu04->IsWindowVisible())
			m_pDlgMenu04->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu05 && m_pDlgMenu05->GetSafeHwnd())
	{
		if (m_pDlgMenu05->IsWindowVisible())
			m_pDlgMenu05->ShowWindow(SW_HIDE);
	}

}

void CGvisR2R_LaserView::DelAllDlg()
{

	if (m_pDlgUtil01 != NULL)
	{
		delete m_pDlgUtil01;
		m_pDlgUtil01 = NULL;
	}

	//if (m_pDlgUtil02 != NULL)
	//{
	//	delete m_pDlgUtil02;
	//	m_pDlgUtil02 = NULL;
	//}

	if (m_pDlgInfo != NULL)
	{
		delete m_pDlgInfo;
		m_pDlgInfo = NULL;
	}
	if (m_pDlgMenu05 != NULL)
	{
		delete m_pDlgMenu05;
		m_pDlgMenu05 = NULL;
	}
	if (m_pDlgMenu04 != NULL)
	{
		delete m_pDlgMenu04;
		m_pDlgMenu04 = NULL;
	}
	if (m_pDlgMenu03 != NULL)
	{
		delete m_pDlgMenu03;
		m_pDlgMenu03 = NULL;
	}
	if (m_pDlgMenu02 != NULL)
	{
		delete m_pDlgMenu02;
		m_pDlgMenu02 = NULL;
	}
	if (m_pDlgMenu01 != NULL)
	{
		delete m_pDlgMenu01;
		m_pDlgMenu01 = NULL;
	}
	if (m_pDlgFrameHigh != NULL)
	{
		delete m_pDlgFrameHigh;
		m_pDlgFrameHigh = NULL;
	}

	if (m_pDlgMsgBox != NULL)
	{
		if (m_pDlgMsgBox->GetSafeHwnd())
			m_pDlgMsgBox->DestroyWindow();
		delete m_pDlgMsgBox;
		m_pDlgMsgBox = NULL;
	}
}

LRESULT CGvisR2R_LaserView::OnDlgInfo(WPARAM wParam, LPARAM lParam)
{
	ClrDispMsg();
	CDlgInfo Dlg;
	m_pDlgInfo = &Dlg;
	Dlg.DoModal();
	m_pDlgInfo = NULL;

	if (m_pDlgMenu01)
		m_pDlgMenu01->ChkUserInfo(FALSE);

	return 0L;
}

void CGvisR2R_LaserView::DispIo()
{
	//ClrDispMsg();
	//CDlgUtil04 Dlg;
	//Dlg.DoModal();
}

void CGvisR2R_LaserView::DispDatabaseConnection()
{
	//ClrDispMsg();
	//CDlgUtil07 Dlg;
	//Dlg.DoModal();
}

void CGvisR2R_LaserView::TowerLamp(COLORREF color, BOOL bOn, BOOL bWink)
{
	if (bWink)
	{
		if (color == RGB_RED)
		{
			m_bTowerWinker[0] = bOn;
			if (bOn)
			{
				m_bTowerWinker[1] = FALSE;
				m_bTowerWinker[2] = FALSE;
			}
		}
		if (color == RGB_GREEN)
		{
			m_bTowerWinker[1] = bOn;
			if (bOn)
			{
				m_bTowerWinker[0] = FALSE;
				m_bTowerWinker[2] = FALSE;
			}
		}
		if (color == RGB_YELLOW)
		{
			m_bTowerWinker[2] = bOn;
			if (bOn)
			{
				m_bTowerWinker[1] = FALSE;
				m_bTowerWinker[0] = FALSE;
			}
		}

		if (!m_bTowerWinker[0] && !m_bTowerWinker[1] && !m_bTowerWinker[2])
			m_bTimTowerWinker = FALSE;
		else
		{
			if (!m_bTimTowerWinker)
			{
				m_bTimTowerWinker = TRUE;
				SetTimer(TIM_TOWER_WINKER, 100, NULL);
			}
		}
	}
	else
	{
		m_bTimTowerWinker = FALSE;

#ifdef USE_MPE
		if (!pDoc->m_pMpeIo)
			return;
#endif
	}
}

void CGvisR2R_LaserView::DispTowerWinker()
{
	m_nCntTowerWinker++;
	if (m_nCntTowerWinker > LAMP_DELAY)
	{
		m_nCntTowerWinker = 0;
	}

}

void CGvisR2R_LaserView::BuzzerFromThread(BOOL bOn, int nCh)
{
	m_bBuzzerFromThread = TRUE;
}

void CGvisR2R_LaserView::Buzzer(BOOL bOn, int nCh)
{
	//	return; // PLC제어
#ifdef USE_MPE
	if (!m_pMpe)
		return;

	if (!bOn)
	{
		switch (nCh)
		{
		case 0:
			//IoWrite(_T("MB44015E"), 0); // 부저1 On  (PC가 ON, OFF) - 20141020
			if (m_pMpe)
				m_pMpe->Write(_T("MB44015E"), 0);
			break;
		case 1:
			//IoWrite(_T("MB44015F"), 0); // 부저2 On  (PC가 ON, OFF) - 20141020
			if (m_pMpe)
				m_pMpe->Write(_T("MB44015F"), 0);
			break;
		}
	}
	else
	{
		switch (nCh)
		{
		case 0:
			//IoWrite(_T("MB44015E"), 1); // 부저1 On  (PC가 ON, OFF) - 20141020
			pView->m_pMpe->Write(_T("MB44015E"), 0);
			Sleep(300);
			pView->m_pMpe->Write(_T("MB44015E"), 1);
			break;
		case 1:
			//IoWrite(_T("MB44015F"), 1); // 부저2 On  (PC가 ON, OFF) - 20141020
			pView->m_pMpe->Write(_T("MB44015E"), 0);
			Sleep(300);
			pView->m_pMpe->Write(_T("MB44015F"), 1);
			break;
		}
	}
#endif
}



void CGvisR2R_LaserView::ThreadInit()
{
	if (!m_bThread[0])
		m_Thread[0].Start(GetSafeHwnd(), this, ThreadProc0);
}

void CGvisR2R_LaserView::ThreadKill()
{
	if (m_bThread[0])
	{
		m_Thread[0].Stop();
		Sleep(20);
		while (m_bThread[0])
		{
			Sleep(20);
		}
	}
	if (m_bThread[1])
	{
		m_Thread[1].Stop();
		Sleep(20);
		while (m_bThread[1])
		{
			Sleep(20);
		}
	}
	if (m_bThread[2])
	{
		m_Thread[2].Stop();
		Sleep(20);
		while (m_bThread[2])
		{
			Sleep(20);
		}
	}
}

UINT CGvisR2R_LaserView::ThreadProc0(LPVOID lpContext)
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_LaserView* pThread = reinterpret_cast<CGvisR2R_LaserView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[0] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[0].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[0] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;
			pThread->GetCurrentInfoSignal();

			/*
			if (pDoc->BtnStatus.EngAuto.IsMkSt)
			{
				pDoc->BtnStatus.EngAuto.IsMkSt = FALSE;
				pThread->m_pEngrave->IsSwEngAutoMkSt(pDoc->BtnStatus.EngAuto.MkSt);
			}

			if (pDoc->BtnStatus.EngAuto.IsRead2dSt)
			{
				pDoc->BtnStatus.EngAuto.IsRead2dSt = FALSE;
				pThread->m_pEngrave->IsSwEngAuto2dReadSt(pDoc->BtnStatus.EngAuto.Read2dSt);
			}
			
			if (pDoc->BtnStatus.Disp.IsReady)
			{
				pDoc->BtnStatus.Disp.IsReady = FALSE;
				pThread->m_pEngrave->IsSetDispReady(pDoc->BtnStatus.Disp.Ready);
			}

			if (pDoc->BtnStatus.Disp.IsRun)
			{
				pDoc->BtnStatus.Disp.IsRun = FALSE;
				pThread->m_pEngrave->IsSetDispRun(pDoc->BtnStatus.Disp.Run);
			}

			if (pDoc->BtnStatus.Disp.IsStop)
			{
				pDoc->BtnStatus.Disp.IsStop = FALSE;
				pThread->m_pEngrave->IsSetDispStop(pDoc->BtnStatus.Disp.Stop);
			}

			if (pDoc->BtnStatus.Disp.IsDualSample)
			{
				pDoc->BtnStatus.Disp.IsDualSample = FALSE;
				pThread->m_pEngrave->IsSetDispDualSample(pDoc->BtnStatus.Disp.Stop);
			}

			if (pDoc->BtnStatus.Disp.IsSingleSample)
			{
				pDoc->BtnStatus.Disp.IsSingleSample = FALSE;
				pThread->m_pEngrave->IsSetDispSingleSample(pDoc->BtnStatus.Disp.SingleSample);
			}

			if (pDoc->BtnStatus.Disp.IsDualTest)
			{
				pDoc->BtnStatus.Disp.IsDualTest = FALSE;
				pThread->m_pEngrave->IsSetDispDualTest(pDoc->BtnStatus.Disp.DualTest);
			}

			if (pDoc->BtnStatus.Disp.IsSingleTest)
			{
				pDoc->BtnStatus.Disp.IsSingleTest = FALSE;
				pThread->m_pEngrave->IsSetDispDualTest(pDoc->BtnStatus.Disp.SingleTest);
			}

			if (pDoc->BtnStatus.Disp.IsSingleTest)
			{
				pDoc->BtnStatus.Disp.IsSingleTest = FALSE;
				pThread->m_pEngrave->IsSetDispDualTest(pDoc->BtnStatus.Disp.SingleTest);
			}
			*/
			bLock = FALSE;
		}
		Sleep(100);
	}
	pThread->m_bThread[0] = FALSE;

	return 0;
}

UINT CGvisR2R_LaserView::ThreadProc1(LPVOID lpContext)
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_LaserView* pThread = reinterpret_cast<CGvisR2R_LaserView*>(lpContext);

	BOOL bLock = FALSE, bEStop = FALSE, bCollision = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[1] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[1].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[1] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;
			pThread->GetEnc();
			bLock = FALSE;
		}
		Sleep(100);
	}
	pThread->m_bThread[1] = FALSE;

	return 0;
}

UINT CGvisR2R_LaserView::ThreadProc2(LPVOID lpContext)
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_LaserView* pThread = reinterpret_cast<CGvisR2R_LaserView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[2] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[2].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[2] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;

			if (pThread->m_bTHREAD_DISP_DEF)
			{
				pThread->DispDefImg();
				pThread->UpdateLotTime();

				Sleep(0);
			}
			else
				Sleep(30);

			bLock = FALSE;
		}
	}
	pThread->m_bThread[2] = FALSE;

	return 0;
}


void CGvisR2R_LaserView::DispStsMainMsg(int nIdx)
{
	CString str;
	str = m_sDispMsg[nIdx];
	pFrm->DispStatusBar(str, nIdx);
}

//void CGvisR2R_LaserView::DispThreadTick()
//{
//	CString str;
//	//	str.Format(_T("%d"), m_dwThreadTick[1]);//, m_dwThreadTick[1], m_dwThreadTick[2]);
//	str.Format(_T("%d,%d,%d"), m_dwThreadTick[0], m_dwThreadTick[1], m_dwThreadTick[2]); // MK, Collision, Enc
//	if (m_sTick != str)
//	{
//		m_sTick = str;
//		pFrm->DispStatusBar(str, 5);
//#ifdef USE_IDS
//		double dFPS[2];
//		if (m_pVision[0])
//			m_pVision[0]->GetFramesPerSecond(&dFPS[0]);
//		if (m_pVision[1])
//			m_pVision[1]->GetFramesPerSecond(&dFPS[1]);
//		str.Format(_T("%.1f,%.1f"), dFPS[0], dFPS[1]);
//		pFrm->DispStatusBar(str, 6);
//#else
//		str.Format(_T("%d"), m_nDebugStep);
//		pFrm->DispStatusBar(str, 6);
//#endif
//	}
//}

void CGvisR2R_LaserView::SwJog(int nAxisID, int nDir, BOOL bOn)
{
	if (!pView->m_pMotion)// || pDoc->Status.bAuto)
		return;

	double fVel, fAcc, fJerk;
	double dStep;
	if (pDoc->Status.bSwJogFast)
		dStep = 0.5;
	else
		dStep = 0.1;

	if (pDoc->Status.bSwJogStep)
	{
		if (bOn)
		{
			double dPos = pView->m_dEnc[nAxisID]; // m_pMotion->GetActualPosition(nAxisID);
			if (nDir == M_CW)
				dPos += dStep;
			else if (nDir == M_CCW)
				dPos -= dStep;

			if (nAxisID == AXIS_X0)
			{
				if (m_bAuto && m_bTHREAD_MK[0] && m_bTHREAD_MK[1] && IsReview())
				{
					if (nDir == M_CW) // ▶ Jog 버튼.
						return;
				}

				if (m_pMotion->IsLimit(MS_X0, nDir))
					return;
				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, dStep, fVel, fAcc, fJerk);
				m_pMotion->Move(MS_X0, dPos, fVel, fAcc, fAcc);
			}
			else if (nAxisID == AXIS_Y0)
			{
				if (m_pMotion->IsLimit(MS_Y0, nDir))
					return;
				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_Y0, dStep, fVel, fAcc, fJerk);
				m_pMotion->Move(MS_Y0, dPos, fVel, fAcc, fAcc);
			}
			//else if (nAxisID == AXIS_X1)
			//{
			//	if (m_bAuto && m_bTHREAD_MK[0] && m_bTHREAD_MK[1] && IsReview())
			//	{
			//		if (nDir == M_CW) // ▶ Jog 버튼.
			//			return;
			//	}

			//	if (m_pMotion->IsLimit(MS_X1, nDir))
			//		return;
			//	m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, dStep, fVel, fAcc, fJerk);
			//	m_pMotion->Move(MS_X1, dPos, fVel, fAcc, fAcc);
			//}
			//else if (nAxisID == AXIS_Y1)
			//{
			//	if (m_pMotion->IsLimit(MS_Y1, nDir))
			//		return;
			//	m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_Y1, dStep, fVel, fAcc, fJerk);
			//	m_pMotion->Move(MS_Y1, dPos, fVel, fAcc, fAcc);
			//}
		}
	}
	else	// Jog Mode
	{
		if (!m_pDlgMenu02)
			return;

		if (nAxisID == AXIS_Y0)
		{
			if (nDir == M_CCW)		// Up
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_UP);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_UP);
			}
			else if (nDir == M_CW)	// Dn
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_DN);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_DN);
			}
		}
		else if (nAxisID == AXIS_X0)
		{
			if (m_bAuto && m_bTHREAD_MK[0] && m_bTHREAD_MK[1] && IsReview())
			{
				if (nDir == M_CW) // ▶ Jog 버튼.
					return;
			}

			if (nDir == M_CW)		// Rt
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_RT);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_RT);
			}
			else if (nDir == M_CCW)	// Lf
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_LF);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_LF);
			}
		}
		//else if (nAxisID == AXIS_Y1)
		//{
		//	if (nDir == M_CCW)		// Up
		//	{
		//		if (bOn)
		//			m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_UP2);
		//		else
		//			m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_UP2);
		//	}
		//	else if (nDir == M_CW)	// Dn
		//	{
		//		if (bOn)
		//			m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_DN2);
		//		else
		//			m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_DN2);
		//	}
		//}
		//else if (nAxisID == AXIS_X1)
		//{
		//	if (m_bAuto && m_bTHREAD_MK[0] && m_bTHREAD_MK[1] && IsReview())
		//	{
		//		if (nDir == M_CW) // ▶ Jog 버튼.
		//			return;
		//	}

		//	if (nDir == M_CW)		// Rt
		//	{
		//		if (bOn)
		//			m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_RT2);
		//		else
		//			m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_RT2);
		//	}
		//	else if (nDir == M_CCW)	// Lf
		//	{
		//		if (bOn)
		//			m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_LF2);
		//		else
		//			m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_LF2);
		//	}
		//}
	}
}


BOOL CGvisR2R_LaserView::ChkShareIdx(int *pBufSerial, int nBufTot, int nShareSerial)
{
	if (nBufTot < 1)
		return TRUE;
	for (int i = 0; i < nBufTot; i++)
	{
		if (pBufSerial[i] == nShareSerial)
			return FALSE;
	}
	return TRUE;
}

BOOL CGvisR2R_LaserView::ChkVsShare(int &nSerial)
{
	int nS0, nS1;
	BOOL b0 = ChkVsShareUp(nS0);
	BOOL b1 = ChkVsShareDn(nS1);

	if (!b0 || !b1)
	{
		nSerial = -1;
		return FALSE;
	}
	else if (nS0 != nS1)
	{
		nSerial = -1;
		return FALSE;
	}

	nSerial = nS0;
	return TRUE;
}

BOOL CGvisR2R_LaserView::ChkVsShareUp(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsShareUp + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::ChkVsShareDn(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsShareDn + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::ChkShare(int &nSerial)
{
	int nS0, nS1;
	BOOL b0 = ChkShareUp(nS0);
	BOOL b1 = ChkShareDn(nS1);

	if (!b0 || !b1)
	{
		nSerial = -1;
		return FALSE;
	}
	else if (nS0 != nS1)
	{
		nSerial = -1;
		return FALSE;
	}

	nSerial = nS0;
	return TRUE;
}

BOOL CGvisR2R_LaserView::ChkShareUp(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareUp + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::ChkShareDn(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareDn + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}

void CGvisR2R_LaserView::ChkShare()
{
	ChkShareUp();
	ChkShareDn();
}

void CGvisR2R_LaserView::ChkShareUp()
{
	CString str;
	int nSerial;
	if (ChkShareUp(nSerial))
	{
		str.Format(_T("US: %d"), nSerial);
		pDoc->Status.PcrShare[0].bExist = TRUE;
		pDoc->Status.PcrShare[0].nSerial = nSerial;
	}
	else
	{
		pDoc->Status.PcrShare[0].bExist = FALSE;
		pDoc->Status.PcrShare[0].nSerial = -1;
		str.Format(_T("US: "));
	}
	if (pFrm)
	{
		if (m_sShare[0] != str)
		{
			m_sShare[0] = str;
			pFrm->DispStatusBar(str, 4);
		}
	}
}

void CGvisR2R_LaserView::ChkShareDn()
{
	CString str;
	int nSerial;
	if (ChkShareDn(nSerial))
	{
		str.Format(_T("DS: %d"), nSerial);
		pDoc->Status.PcrShare[1].bExist = TRUE;
		pDoc->Status.PcrShare[1].nSerial = nSerial;
	}
	else
	{
		pDoc->Status.PcrShare[1].bExist = FALSE;
		pDoc->Status.PcrShare[1].nSerial = -1;
		str.Format(_T("DS: "));
	}
	if (pFrm)
	{
		if (m_sShare[1] != str)
		{
			m_sShare[1] = str;
			pFrm->DispStatusBar(str, 2);
		}
	}
}

BOOL CGvisR2R_LaserView::ChkBufIdx(int* pSerial, int nTot)
{
	if (nTot < 2)
		return TRUE;

	for (int i = 0; i < (nTot - 1); i++)
	{
		if (pSerial[i + 1] != pSerial[i] + 1)
			return FALSE;
	}
	return TRUE;
}


void CGvisR2R_LaserView::SwapUp(__int64 *num1, __int64 *num2) 	// 위치 바꾸는 함수
{
	__int64 temp;

	temp = *num2;
	*num2 = *num1;
	*num1 = temp;
}

BOOL CGvisR2R_LaserView::SortingInUp(CString sPath, int nIndex)
{
	struct _stat buf;
	struct tm *t;

	CString sMsg, sFileName, sSerial;
	int nPos, nSerial;
	char filename[MAX_PATH];
	StringToChar(sPath, filename);

	if (_stat(filename, &buf) != 0)
	{
		sMsg.Format(_T("일시정지 - Failed getting information."));
		//MsgBox(sMsg);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}
	else
	{
		sFileName = sPath;
		nPos = sFileName.ReverseFind('.');
		if (nPos > 0)
		{
			sSerial = sFileName.Left(nPos);
			sSerial = sSerial.Right(4);
		}

		nSerial = _tstoi(sSerial);

		t = localtime(&buf.st_mtime);
		//sFileName.Format(_T("%04d%02d%02d%02d%02d%02d%04d"), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
		//														t->tm_hour, t->tm_min, t->tm_sec, nSerial);
		//m_nBufSerialSorting[0][nIndex] = _tstoi(sFileName);

		CString sYear, sMonth, sDay, sHour, sMin, sSec;
		sYear.Format(_T("%04d"), t->tm_year + 1900);
		sMonth.Format(_T("%02d"), t->tm_mon + 1);
		sDay.Format(_T("%02d"), t->tm_mday);
		sHour.Format(_T("%02d"), t->tm_hour);
		sMin.Format(_T("%02d"), t->tm_min);
		sSec.Format(_T("%02d"), t->tm_sec);

		__int64 nYear = _tstoi(sYear);
		__int64 nMonth = _tstoi(sMonth);
		__int64 nDay = _tstoi(sDay);
		__int64 nHour = _tstoi(sHour);
		__int64 nMin = _tstoi(sMin);
		__int64 nSec = _tstoi(sSec);

		m_nBufSerialSorting[0][nIndex] = nYear * 100000000000000 + nMonth * 1000000000000 + nDay * 10000000000 +
			nHour * 100000000 + nMin * 1000000 + nSec * 10000 + nSerial;
	}

	return TRUE;
}

BOOL CGvisR2R_LaserView::SortingOutUp(int* pSerial, int nTot)
{
	int i, k;

	for (k = 0; k < nTot; k++) 			// 버블 정렬 소스 시작
	{
		for (i = 0; i < (nTot - 1) - k; i++)
		{

			if (m_nBufSerialSorting[0][i] > m_nBufSerialSorting[0][i + 1])
			{
				SwapUp(&m_nBufSerialSorting[0][i + 1], &m_nBufSerialSorting[0][i]);
			}
		}
	}									// 버블 정렬 소스 끝

	for (i = 0; i < nTot; i++)
	{
		pSerial[i] = (int)(m_nBufSerialSorting[0][i] % 10000);
	}
	return TRUE;
}

BOOL CGvisR2R_LaserView::ChkBufUp(int* pSerial, int &nTot)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufUp + _T("*.pcr"));
	if (!bExist)
	{
		pDoc->m_bBufEmpty[0] = TRUE;
		if (!pDoc->m_bBufEmptyF[0])
			pDoc->m_bBufEmptyF[0] = TRUE;		// 최초 한번 버퍼가 비어있으면(초기화를 하고 난 이후) TRUE.

		return FALSE; // pcr파일이 존재하지 않음.
	}

	int nPos, nSerial;

	CString sFileName, sSerial;
	CString sNewName;

	nTot = 0;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			sFileName = cFile.GetFileName();
			//nPos = sFileName.ReverseFind('.');
			//if (nPos > 0)
			//	sSerial = sFileName.Left(nPos);

			//nSerial = _tstoi(sSerial);
			//if (nSerial > 0)
			//{
			//	pSerial[nTot] = nSerial;
			//	nTot++;
			//}

			if (!SortingInUp(pDoc->WorkingInfo.System.sPathVrsBufUp + sFileName, nTot))
				return FALSE;

				nTot++;
			}
		}

	BOOL bRtn = SortingOutUp(pSerial, nTot);

	if (nTot == 0)
		pDoc->m_bBufEmpty[0] = TRUE;
	else
		pDoc->m_bBufEmpty[0] = FALSE;

	return (bRtn);
	//return TRUE;
}

BOOL CGvisR2R_LaserView::ChkBufDn(int* pSerial, int &nTot)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufDn + _T("*.pcr"));
	if (!bExist)
	{
		pDoc->m_bBufEmpty[1] = TRUE;
		if (!pDoc->m_bBufEmptyF[1])
			pDoc->m_bBufEmptyF[1] = TRUE;
		return FALSE; // pcr파일이 존재하지 않음.
	}

	int nPos, nSerial;

	CString sFileName, sSerial;
	nTot = 0;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			sFileName = cFile.GetFileName();
			//nPos = sFileName.ReverseFind('.');
			//if (nPos > 0)
			//	sSerial = sFileName.Left(nPos);

			//nSerial = _tstoi(sSerial);
			//if (nSerial > 0)
			//{
			//	pSerial[nTot] = nSerial;
			//	nTot++;
			//}

			if (!SortingInDn(pDoc->WorkingInfo.System.sPathVrsBufDn + sFileName, nTot))
				return FALSE;

			nTot++;
		}
	}


	BOOL bRtn = SortingOutDn(pSerial, nTot);

	if (nTot == 0)
		pDoc->m_bBufEmpty[1] = TRUE;
	else
		pDoc->m_bBufEmpty[1] = FALSE;

	return (bRtn);
	//return TRUE;
}

void CGvisR2R_LaserView::SwapDn(__int64 *num1, __int64 *num2) 	// 위치 바꾸는 함수
{
	__int64 temp;

	temp = *num2;
	*num2 = *num1;
	*num1 = temp;
}

BOOL CGvisR2R_LaserView::SortingInDn(CString sPath, int nIndex)
{
	struct _stat buf;
	struct tm *t;

	CString sMsg, sFileName, sSerial;
	int nPos, nSerial;
	char filename[MAX_PATH];
	StringToChar(sPath, filename);

	if (_stat(filename, &buf) != 0)
	{
		sMsg.Format(_T("일시정지 - Failed getting information."));
		//MsgBox(sMsg);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}
	else
	{
		sFileName = sPath;
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
		{
				sSerial = sFileName.Left(nPos);
			sSerial = sSerial.Right(4);
		}

			nSerial = _tstoi(sSerial);

		t = localtime(&buf.st_mtime);
		//sFileName.Format(_T("%04d%02d%02d%02d%02d%02d%04d"), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
		//														t->tm_hour, t->tm_min, t->tm_sec, nSerial);
		//m_nBufSerialSorting[1][nIndex] = _tstoi(sFileName);


		CString sYear, sMonth, sDay, sHour, sMin, sSec;
		sYear.Format(_T("%04d"), t->tm_year + 1900);
		sMonth.Format(_T("%02d"), t->tm_mon + 1);
		sDay.Format(_T("%02d"), t->tm_mday);
		sHour.Format(_T("%02d"), t->tm_hour);
		sMin.Format(_T("%02d"), t->tm_min);
		sSec.Format(_T("%02d"), t->tm_sec);

		__int64 nYear = _tstoi(sYear);
		__int64 nMonth = _tstoi(sMonth);
		__int64 nDay = _tstoi(sDay);
		__int64 nHour = _tstoi(sHour);
		__int64 nMin = _tstoi(sMin);
		__int64 nSec = _tstoi(sSec);

		m_nBufSerialSorting[1][nIndex] = nYear * 100000000000000 + nMonth * 1000000000000 + nDay * 10000000000 +
			nHour * 100000000 + nMin * 1000000 + nSec * 10000 + nSerial;

			}

	return TRUE;
}

BOOL CGvisR2R_LaserView::SortingOutDn(int* pSerial, int nTot)
{
	int i, k;

	for (k = 0; k < nTot; k++) 			// 버블 정렬 소스 시작
	{
		for (i = 0; i < (nTot - 1) - k; i++)
		{

			if (m_nBufSerialSorting[1][i] > m_nBufSerialSorting[1][i + 1])
			{
				SwapUp(&m_nBufSerialSorting[1][i + 1], &m_nBufSerialSorting[1][i]);
		}
	}
	}									// 버블 정렬 소스 끝

	for (i = 0; i < nTot; i++)
	{
		pSerial[i] = (int)(m_nBufSerialSorting[1][i] % 10000);
	}
	return TRUE;
}

BOOL CGvisR2R_LaserView::ChkYield() // 수율 양호 : TRUE , 수율 불량 : FALSE
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nGood = 0, nBad = 0, nTot;
	double dRatio;
	CString sMsg;

	if (bDualTest)
	{
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad);
		else
		{
			Stop();
			sMsg.Format(_T("일시정지 - Failed ChkYield()."));
			MsgBox(sMsg);
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
			return FALSE;
		}
	}
	else
	{
		if (pDoc->m_pReelMapUp)
			pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad);
		else
		{
			Stop();
			sMsg.Format(_T("일시정지 - Failed ChkYield()."));
			MsgBox(sMsg);
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
			return FALSE;
		}
	}

	nTot = (nGood + nBad);
	double dTotLmt = _tstof(pDoc->WorkingInfo.LastJob.sLmtTotYld);
	if (dTotLmt > 0.0)
	{
		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		if (dRatio < dTotLmt)
		{
			Stop();
			sMsg.Format(_T("일시정지 - 전체 수율 제한범위 : %.1f 미달 ( %.1f )"), dTotLmt, dRatio);
			MsgBox(sMsg);
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
			return FALSE;
		}
	}

	// 	int nNodeX = pDoc->m_pPcsRgn->nCol;
	// 	int nNodeY = pDoc->m_pPcsRgn->nRow;
	// 	nTot = (nNodeX*nNodeY);
	// 	int nSerial = m_pDlgMenu01->GetCurSerial();
	// 	int nIdx = pDoc->GetPcrIdx(nSerial);
	// 	int nDef = pDoc->m_pPcr[nIdx]->m_nTotRealDef;

	double dPrtLmt = _tstof(pDoc->WorkingInfo.LastJob.sLmtPatlYld);
	if (dPrtLmt > 0.0)
	{
		// 		if(nDef > 0)
		// 			dRatio = ((double)(nTot-nDef)/(double)nTot) * 100.0;
		// 		else
		// 			dRatio = 0.0;

		if (bDualTest)
		{
			if (pDoc->m_Yield[2].IsValid())
			{
				dRatio = pDoc->m_Yield[2].GetYield();

				if (dRatio < dPrtLmt)
				{
					Stop();
					sMsg.Format(_T("일시정지 - 구간 수율 제한범위 : %.1f 미달 ( %.1f )"), dPrtLmt, dRatio);
					MsgBox(sMsg);
					TowerLamp(RGB_RED, TRUE);
					Buzzer(TRUE, 0);
					return FALSE;
				}
			}
		}
		else
		{
			if (pDoc->m_Yield[0].IsValid())
			{
				dRatio = pDoc->m_Yield[0].GetYield();

				if (dRatio < dPrtLmt)
				{
					Stop();
					sMsg.Format(_T("일시정지 - 구간 수율 제한범위 : %.1f 미달 ( %.1f )"), dPrtLmt, dRatio);
					MsgBox(sMsg);
					TowerLamp(RGB_RED, TRUE);
					Buzzer(TRUE, 0);
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_LaserView::ChkSaftySen() // 감지 : TRUE , 비감지 : FALSE
{
	if (pDoc->WorkingInfo.LastJob.bMkSftySen)
	{
		if (pDoc->Status.bSensSaftyMk && !pDoc->Status.bSensSaftyMkF)
		{
			pDoc->Status.bSensSaftyMkF = TRUE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			//pView->DispStsBar(_T("정지-4"), 0);
			DispMain(_T("정 지"), RGB_RED);			
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
			m_bTIM_SAFTY_STOP = TRUE;//MsgBox(_T("일시정지 - 마킹부 안전센서가 감지되었습니다."));
			SetTimer(TIM_SAFTY_STOP, 100, NULL);
		}
		else if (!pDoc->Status.bSensSaftyMk && pDoc->Status.bSensSaftyMkF)
		{
			pDoc->Status.bSensSaftyMkF = FALSE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
	}

	return (pDoc->Status.bSensSaftyMk);
}

void CGvisR2R_LaserView::ResetMotion()
{
	for (int i = 0; i < MAX_MS; i++)
	{
		ResetMotion(i);

		if (i < MAX_AXIS)
		{
			while (!m_pMotion->IsServoOn(i))
			{
				if (i == MS_X0 || i == MS_Y0)
					pView->m_pMotion->Clear(MS_X0Y0);
				else if (i == MS_X1 || i == MS_Y1)
					pView->m_pMotion->Clear(MS_X1Y1);
				else
					pView->m_pMotion->Clear(i);
				// 				for(int k=0; k<MAX_MS; k++)
				// 					m_pMotion->Clear(k);
				Sleep(30);
				m_pMotion->ServoOnOff(i, TRUE);
				Sleep(30);
			}
		}
	}
}

void CGvisR2R_LaserView::ResetMotion(int nMsId)
{
	if (m_pDlgMenu02)
		m_pDlgMenu02->ResetMotion(nMsId);
}

unsigned long CGvisR2R_LaserView::ChkDoor() // 0: All Closed , Open Door Index : Doesn't all closed. (Bit3: F, Bit2: L, Bit1: R, Bit0; B)
{
	unsigned long ulOpenDoor = 0;

	if (pDoc->WorkingInfo.LastJob.bAoiUpDrSen)
	{
		if (pDoc->Status.bDoorAoi[DOOR_FM_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 0);
			pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FM_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 0);
			pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			//pView->DispStsBar(_T("정지-5"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 전면 중앙 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_FL_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 1);
			pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FL_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 1);
			pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			//pView->DispStsBar(_T("정지-6"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 전면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_FR_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP])
	{
			ulOpenDoor |= (0x01 << 2);
			pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FR_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 2);
			pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			//pView->DispStsBar(_T("정지-7"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 전면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_BM_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 3);
			pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BM_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 3);
			pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			//pView->DispStsBar(_T("정지-8"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 후면 중앙 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_BL_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 4);
			pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BL_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 4);
			pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			//pView->DispStsBar(_T("정지-9"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 후면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_BR_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 5);
			pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BR_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 5);
			pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			//pView->DispStsBar(_T("정지-10"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 후면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}
	}

	if (pDoc->WorkingInfo.LastJob.bAoiDnDrSen)
	{
		if (pDoc->Status.bDoorAoi[DOOR_FM_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 0);
			pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FM_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 0);
			pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			//pView->DispStsBar(_T("정지-5"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 전면 중앙 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_FL_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 1);
			pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FL_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 1);
			pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			//pView->DispStsBar(_T("정지-6"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 전면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_FR_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 2);
			pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FR_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 2);
			pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-7"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 전면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_BM_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 3);
			pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BM_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 3);
			pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-8"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 후면 중앙 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_BL_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 4);
			pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BL_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 4);
			pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-9"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 후면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorAoi[DOOR_BR_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 5);
			pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BR_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 5);
			pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-10"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 후면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}
	}

	if (pDoc->WorkingInfo.LastJob.bMkDrSen)
	{
		if (pDoc->Status.bDoorMk[DOOR_FL_MK] && !pDoc->Status.bDoorMkF[DOOR_FL_MK])
		{
			ulOpenDoor |= (0x01 << 6);
			pDoc->Status.bDoorMkF[DOOR_FL_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorMk[DOOR_FL_MK] && pDoc->Status.bDoorMkF[DOOR_FL_MK])
		{
			ulOpenDoor &= ~(0x01 << 6);
			pDoc->Status.bDoorMkF[DOOR_FL_MK] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-11"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 마킹부 전면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorMk[DOOR_FR_MK] && !pDoc->Status.bDoorMkF[DOOR_FR_MK])
		{
			ulOpenDoor |= (0x01 << 7);
			pDoc->Status.bDoorMkF[DOOR_FR_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorMk[DOOR_FR_MK] && pDoc->Status.bDoorMkF[DOOR_FR_MK])
		{
			ulOpenDoor &= ~(0x01 << 7);
			pDoc->Status.bDoorMkF[DOOR_FR_MK] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-12"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 마킹부 전면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorMk[DOOR_BL_MK] && !pDoc->Status.bDoorMkF[DOOR_BL_MK])
		{
			ulOpenDoor |= (0x01 << 8);
			pDoc->Status.bDoorMkF[DOOR_BL_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorMk[DOOR_BL_MK] && pDoc->Status.bDoorMkF[DOOR_BL_MK])
		{
			ulOpenDoor &= ~(0x01 << 8);
			pDoc->Status.bDoorMkF[DOOR_BL_MK] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-13"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 마킹부 후면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorMk[DOOR_BR_MK] && !pDoc->Status.bDoorMkF[DOOR_BR_MK])
		{
			ulOpenDoor |= (0x01 << 9);
			pDoc->Status.bDoorMkF[DOOR_BR_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorMk[DOOR_BR_MK] && pDoc->Status.bDoorMkF[DOOR_BR_MK])
		{
			ulOpenDoor &= ~(0x01 << 9);
			pDoc->Status.bDoorMkF[DOOR_BR_MK] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-14"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 마킹부 후면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}
	}

	if (pDoc->WorkingInfo.LastJob.bEngvDrSen)
	{
		if (pDoc->Status.bDoorEngv[DOOR_FL_ENGV] && !pDoc->Status.bDoorEngvF[DOOR_FL_ENGV])
		{
			ulOpenDoor |= (0x01 << 6);
			pDoc->Status.bDoorEngvF[DOOR_FL_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorEngv[DOOR_FL_ENGV] && pDoc->Status.bDoorEngvF[DOOR_FL_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 6);
			pDoc->Status.bDoorEngvF[DOOR_FL_ENGV] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-11"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 각인부 전면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorEngv[DOOR_FR_ENGV] && !pDoc->Status.bDoorEngvF[DOOR_FR_ENGV])
		{
			ulOpenDoor |= (0x01 << 7);
			pDoc->Status.bDoorEngvF[DOOR_FR_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorEngv[DOOR_FR_ENGV] && pDoc->Status.bDoorEngvF[DOOR_FR_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 7);
			pDoc->Status.bDoorEngvF[DOOR_FR_ENGV] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-12"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 각인부 전면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorEngv[DOOR_BL_ENGV] && !pDoc->Status.bDoorEngvF[DOOR_BL_ENGV])
		{
			ulOpenDoor |= (0x01 << 8);
			pDoc->Status.bDoorEngvF[DOOR_BL_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorEngv[DOOR_BL_ENGV] && pDoc->Status.bDoorEngvF[DOOR_BL_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 8);
			pDoc->Status.bDoorEngvF[DOOR_BL_ENGV] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-13"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 각인부 후면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorEngv[DOOR_BR_ENGV] && !pDoc->Status.bDoorEngvF[DOOR_BR_ENGV])
		{
			ulOpenDoor |= (0x01 << 9);
			pDoc->Status.bDoorEngvF[DOOR_BR_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorEngv[DOOR_BR_ENGV] && pDoc->Status.bDoorEngvF[DOOR_BR_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 9);
			pDoc->Status.bDoorEngvF[DOOR_BR_ENGV] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-14"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 각인부 후면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}
	}

	if (pDoc->WorkingInfo.LastJob.bUclDrSen)
	{
		if (pDoc->Status.bDoorUc[DOOR_FL_UC] && !pDoc->Status.bDoorUcF[DOOR_FL_UC])
		{
			ulOpenDoor |= (0x01 << 10);
			pDoc->Status.bDoorUcF[DOOR_FL_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorUc[DOOR_FL_UC] && pDoc->Status.bDoorUcF[DOOR_FL_UC])
		{
			ulOpenDoor &= ~(0x01 << 10);
			pDoc->Status.bDoorUcF[DOOR_FL_UC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-15"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 언코일러부 전면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorUc[DOOR_FR_UC] && !pDoc->Status.bDoorUcF[DOOR_FR_UC])
		{
			ulOpenDoor |= (0x01 << 11);
			pDoc->Status.bDoorUcF[DOOR_FR_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorUc[DOOR_FR_UC] && pDoc->Status.bDoorUcF[DOOR_FR_UC])
		{
			ulOpenDoor &= ~(0x01 << 11);
			pDoc->Status.bDoorUcF[DOOR_FR_UC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-16"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 언코일러부 측면 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorUc[DOOR_BL_UC] && !pDoc->Status.bDoorUcF[DOOR_BL_UC])
		{
			ulOpenDoor |= (0x01 << 12);
			pDoc->Status.bDoorUcF[DOOR_BL_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorUc[DOOR_BL_UC] && pDoc->Status.bDoorUcF[DOOR_BL_UC])
		{
			ulOpenDoor &= ~(0x01 << 12);
			pDoc->Status.bDoorUcF[DOOR_BL_UC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-17"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 언코일러부 후면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorUc[DOOR_BR_UC] && !pDoc->Status.bDoorUcF[DOOR_BR_UC])
		{
			ulOpenDoor |= (0x01 << 13);
			pDoc->Status.bDoorUcF[DOOR_BR_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorUc[DOOR_BR_UC] && pDoc->Status.bDoorUcF[DOOR_BR_UC])
		{
			ulOpenDoor &= ~(0x01 << 13);
			pDoc->Status.bDoorUcF[DOOR_BR_UC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-18"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 언코일러부 후면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}
	}

	if (pDoc->WorkingInfo.LastJob.bRclDrSen)
	{
		// 		if(pDoc->Status.bDoorRe[DOOR_FL_RC] && !pDoc->Status.bDoorReF[DOOR_FL_RC])
		// 		{
		// 			ulOpenDoor |= (0x01<<14);
		// 			pDoc->Status.bDoorReF[DOOR_FL_RC] = TRUE;
		// 			Stop();
		// 			MsgBox(_T("일시정지 - 리코일러부 전면 좌측 도어 Open"));
		// 			TowerLamp(RGB_RED, TRUE);
		// 			Buzzer(TRUE, 0);
		// 		}
		// 		else if(!pDoc->Status.bDoorRe[DOOR_FL_RC] && pDoc->Status.bDoorReF[DOOR_FL_RC])
		// 		{
		// 			ulOpenDoor &= ~(0x01<<14);
		// 			pDoc->Status.bDoorReF[DOOR_FL_RC] = FALSE;
		// 			Buzzer(FALSE, 0);
		// 			Sleep(300);
		// 		}

		if (pDoc->Status.bDoorRe[DOOR_FR_RC] && !pDoc->Status.bDoorReF[DOOR_FR_RC])
		{
			ulOpenDoor |= (0x01 << 15);
			pDoc->Status.bDoorReF[DOOR_FR_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorRe[DOOR_FR_RC] && pDoc->Status.bDoorReF[DOOR_FR_RC])
		{
			ulOpenDoor &= ~(0x01 << 15);
			pDoc->Status.bDoorReF[DOOR_FR_RC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-19"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 리코일러부 전면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorRe[DOOR_S_RC] && !pDoc->Status.bDoorReF[DOOR_S_RC])
		{
			ulOpenDoor |= (0x01 << 16);
			pDoc->Status.bDoorReF[DOOR_S_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorRe[DOOR_S_RC] && pDoc->Status.bDoorReF[DOOR_S_RC])
		{
			ulOpenDoor &= ~(0x01 << 16);
			pDoc->Status.bDoorReF[DOOR_S_RC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-20"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 리코일러부 측면 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorRe[DOOR_BL_RC] && !pDoc->Status.bDoorReF[DOOR_BL_RC])
		{
			ulOpenDoor |= (0x01 << 17);
			pDoc->Status.bDoorReF[DOOR_BL_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorRe[DOOR_BL_RC] && pDoc->Status.bDoorReF[DOOR_BL_RC])
		{
			ulOpenDoor &= ~(0x01 << 17);
			pDoc->Status.bDoorReF[DOOR_BL_RC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-21"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 리코일러부 후면 좌측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}

		if (pDoc->Status.bDoorRe[DOOR_BR_RC] && !pDoc->Status.bDoorReF[DOOR_BR_RC])
		{
			ulOpenDoor |= (0x01 << 18);
			pDoc->Status.bDoorReF[DOOR_BR_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorRe[DOOR_BR_RC] && pDoc->Status.bDoorReF[DOOR_BR_RC])
		{
			ulOpenDoor &= ~(0x01 << 18);
			pDoc->Status.bDoorReF[DOOR_BR_RC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Stop();
			pView->DispStsBar(_T("정지-22"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 리코일러부 후면 우측 도어 Open"));
			TowerLamp(RGB_RED, TRUE);
			Buzzer(TRUE, 0);
		}
	}

	return ulOpenDoor;
}

void CGvisR2R_LaserView::ChkEmg()
{
	if (pDoc->Status.bEmgAoi[EMG_F_AOI_UP] && !pDoc->Status.bEmgAoiF[EMG_F_AOI_UP])
	{
		//SwAoiEmg(TRUE);
		pDoc->Status.bEmgAoiF[EMG_F_AOI_UP] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-23"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 검사부 상 전면 스위치"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgAoi[EMG_F_AOI_UP] && pDoc->Status.bEmgAoiF[EMG_F_AOI_UP])
	{
		//SwAoiEmg(FALSE);
		pDoc->Status.bEmgAoiF[EMG_F_AOI_UP] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgAoi[EMG_B_AOI_UP] && !pDoc->Status.bEmgAoiF[EMG_B_AOI_UP])
	{
		//SwAoiEmg(TRUE);
		pDoc->Status.bEmgAoiF[EMG_B_AOI_UP] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-24"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 검사부 상 후면 스위치"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgAoi[EMG_B_AOI_UP] && pDoc->Status.bEmgAoiF[EMG_B_AOI_UP])
	{
		//SwAoiEmg(FALSE);
		pDoc->Status.bEmgAoiF[EMG_B_AOI_UP] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgAoi[EMG_F_AOI_DN] && !pDoc->Status.bEmgAoiF[EMG_F_AOI_DN])
	{
		//SwAoiEmg(TRUE);
		pDoc->Status.bEmgAoiF[EMG_F_AOI_DN] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-23"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 검사부 하 전면 스위치"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgAoi[EMG_F_AOI_DN] && pDoc->Status.bEmgAoiF[EMG_F_AOI_DN])
	{
		//SwAoiEmg(FALSE);
		pDoc->Status.bEmgAoiF[EMG_F_AOI_DN] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgAoi[EMG_B_AOI_DN] && !pDoc->Status.bEmgAoiF[EMG_B_AOI_DN])
	{
		//SwAoiEmg(TRUE);
		pDoc->Status.bEmgAoiF[EMG_B_AOI_DN] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-24"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 검사부 하 후면 스위치"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgAoi[EMG_B_AOI_DN] && pDoc->Status.bEmgAoiF[EMG_B_AOI_DN])
	{
		//SwAoiEmg(FALSE);
		pDoc->Status.bEmgAoiF[EMG_B_AOI_DN] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgMk[EMG_M_MK] && !pDoc->Status.bEmgMkF[EMG_M_MK])
	{
		//SwAoiEmg(TRUE);
		pDoc->Status.bEmgMkF[EMG_M_MK] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-25"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 마킹부 메인 스위치"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgMk[EMG_M_MK] && pDoc->Status.bEmgMkF[EMG_M_MK])
	{
		//SwAoiEmg(FALSE);
		pDoc->Status.bEmgMkF[EMG_M_MK] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgMk[EMG_B_MK] && !pDoc->Status.bEmgMkF[EMG_B_MK])
	{
		//SwAoiEmg(TRUE);
		pDoc->Status.bEmgMkF[EMG_B_MK] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-26"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 마킹부 스위치"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgMk[EMG_B_MK] && pDoc->Status.bEmgMkF[EMG_B_MK])
	{
		//SwAoiEmg(FALSE);
		pDoc->Status.bEmgMkF[EMG_B_MK] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgUc && !pDoc->Status.bEmgUcF)
	{
		//SwAoiEmg(TRUE);
		pDoc->Status.bEmgUcF = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-27"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 언코일러부 스위치"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgUc && pDoc->Status.bEmgUcF)
	{
		//SwAoiEmg(FALSE);
		pDoc->Status.bEmgUcF = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgRc && !pDoc->Status.bEmgRcF)
	{
		//SwAoiEmg(TRUE);
		pDoc->Status.bEmgRcF = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-28"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 리코일러부 스위치"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgRc && pDoc->Status.bEmgRcF)
	{
		//SwAoiEmg(FALSE);
		pDoc->Status.bEmgRcF = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgEngv[0] && !pDoc->Status.bEmgEngvF[0])
	{
		pDoc->Status.bEmgEngvF[0] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-29"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 각인부 모니터"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgEngv[0] && pDoc->Status.bEmgEngvF[0])
	{
		pDoc->Status.bEmgEngvF[0] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgEngv[1] && !pDoc->Status.bEmgEngvF[1])
	{
		pDoc->Status.bEmgEngvF[1] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Stop();
		pView->DispStsBar(_T("정지-30"), 0);
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 각인부 스위치"));
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
	}
	else if (!pDoc->Status.bEmgEngv[1] && pDoc->Status.bEmgEngvF[1])
	{
		pDoc->Status.bEmgEngvF[1] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}
}

int CGvisR2R_LaserView::ChkSerial() // 0: Continue, -: Previous, +: Discontinue
{
	int nSerial0 = GetBuffer0();
	int nSerial1 = GetBuffer1();
	int nLastShot = pDoc->GetLastShotMk();

	if (nSerial0 == nLastShot + 1 || nSerial1 == nLastShot + 2)
		return 0;
	if (pDoc->WorkingInfo.LastJob.bLotSep || pDoc->m_bDoneChgLot)
	{
		if (nLastShot == pDoc->m_nLotLastShot && (nSerial0 == 1 || nSerial1 == 2))
			return 0;
	}
	if (m_bLastProc && nSerial0 == m_nLotEndSerial)
		return 0;

	return (nSerial0 - nLastShot);
}

void CGvisR2R_LaserView::ChkBuf()
{
	//if (!m_bShift2Mk)
	ChkBufUp();
	//if (!m_bShift2Mk)
	ChkBufDn();
}

//void CGvisR2R_LaserView::ChkBufUp()
//{
//	CString str, sTemp;
//
//	str = _T("UB: ");
//	if (ChkBufUp(m_pBufSerial[0], m_nBufTot[0]))
//	{
//		for (int i = 0; i < m_nBufTot[0]; i++)
//		{
//			if (i == m_nBufTot[0] - 1)
//				sTemp.Format(_T("%d"), m_pBufSerial[0][i]);
//			else
//				sTemp.Format(_T("%d,"), m_pBufSerial[0][i]);
//			str += sTemp;
//		}
//	}
//	else
//	{
//		m_nBufTot[0] = 0;
//	}
//
//	if (pFrm)
//	{
//		if (m_sBuf[0] != str)
//		{
//			m_sBuf[0] = str;
//			pFrm->DispStatusBar(str, 3);
//		}
//	}
//}
//
//void CGvisR2R_LaserView::ChkBufDn()
//{
//	CString str, sTemp;
//
//	str = _T("DB: ");
//	if (ChkBufDn(m_pBufSerial[1], m_nBufTot[1]))
//	{
//		for (int i = 0; i < m_nBufTot[1]; i++)
//		{
//			if (i == m_nBufTot[1] - 1)
//				sTemp.Format(_T("%d"), m_pBufSerial[1][i]);
//			else
//				sTemp.Format(_T("%d,"), m_pBufSerial[1][i]);
//			str += sTemp;
//		}
//	}
//	else
//	{
//		m_nBufTot[1] = 0;
//	}
//
//	if (pFrm)
//	{
//		if (m_sBuf[1] != str)
//		{
//			m_sBuf[1] = str;
//			pFrm->DispStatusBar(str, 1);
//		}
//	}
//}

void CGvisR2R_LaserView::DoIO()
{
	//	ChkEmg();

	//DoEmgSens();	//20220607
	//DoSaftySens();	//20220603
	//DoDoorSens();	//20220607

	//DoModeSel();
	//DoMainSw();

	//DoInterlock();

	MonPlcAlm();
	MonMsgBox();
	MonDispMain();

	if (m_bCycleStop)
	{
		m_bCycleStop = FALSE;
		//Stop();
		//TowerLamp(RGB_YELLOW, TRUE);
		//Buzzer(TRUE);
		//MyMsgBox(pDoc->m_sAlmMsg);

		if (pDoc->m_sAlmMsg != pDoc->m_sIsAlmMsg)
		{
			pDoc->m_sIsAlmMsg = pDoc->m_sAlmMsg;
			MsgBox(pDoc->m_sAlmMsg);
			pDoc->m_sAlmMsg = _T("");
			pDoc->m_sIsAlmMsg = _T("");
			pDoc->m_sPrevAlmMsg = _T("");
		}
		else if (pDoc->m_sMsgBox != pDoc->m_sIsMsgBox)
		{
			pDoc->m_sIsMsgBox = pDoc->m_sMsgBox;
			MsgBox(pDoc->m_sMsgBox, 0, pDoc->m_nTypeMsgBox);
			pDoc->m_sMsgBox = _T("");
			pDoc->m_sIsMsgBox = _T("");
			pDoc->m_sPrevMsgBox = _T("");
		}

		pDoc->m_sAlmMsg = _T("");
		pDoc->m_sPrevAlmMsg = _T("");
	}

	//if (pDoc->Status.bManual)
	//{
	//	DoBoxSw();
	//}
	//else if (pDoc->Status.bAuto)
	//{
	//	//DoAuto();
	//	DoAutoEng();
	//}

	// 	DoSignal();
	// 	DoSens();
	// 	DoEmgSw();


	if (pDoc->Status.bAuto)
		DoAutoEng();


	if (IsRun())
	{
		if (m_pDlgMenu01)
		{
			if (m_pDlgMenu01->IsEnableBtn())
				m_pDlgMenu01->EnableBtn(FALSE);
		}
	}
	else
	{
		if (m_pDlgMenu01)
		{
			if (!m_pDlgMenu01->IsEnableBtn())
				m_pDlgMenu01->EnableBtn(TRUE);
		}
	}

}

void CGvisR2R_LaserView::DoSens()
{
	// 	DoEmgSens();		// SliceIo[4]
	// 	DoMkSens();			// SliceIo[2]
	// 	DoUcSens1();		// MpeIo[1]
	// 	DoUcSens2();		// MpeIo[2]
	// 	DoUcSens3();		// MpeIo[3]
	// 	DoRcSens1();		// MpeIo[5]
	// 	DoRcSens2();		// MpeIo[6]
	// 	DoRcSens3();		// MpeIo[7]
}

void CGvisR2R_LaserView::DoBoxSw()
{
	// 	DoRcBoxSw();		// MpeIo[4]
	// 	DoAoiBoxSw();		// SliceIo[3]
	// 	DoUcBoxSw();		// MpeIo[0]
}

void CGvisR2R_LaserView::DoEmgSw()
{
#ifdef USE_MPE
	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;

	// 	unsigned short usIn = pDoc->m_pMpeIo[4];
	// 	unsigned short *usInF = &pDoc->m_pMpeIoF[4];
	// 
	// 	if((usIn & (0x01<<0)) && !(*usInF & (0x01<<0)))
	// 	{
	// 		*usInF |= (0x01<<0);								// 리코일러 비상정지 스위치
	// 		pDoc->Status.bEmgRc = TRUE;
	// 	}
	// 	else if(!(usIn & (0x01<<0)) && (*usInF & (0x01<<0)))
	// 	{
	// 		*usInF &= ~(0x01<<0);								
	// 		pDoc->Status.bEmgRc = FALSE;
	// 	}
	// 
	// 	usIn = pDoc->m_pMpeIo[0];
	// 	usInF = &pDoc->m_pMpeIoF[0];
	// 
	// 	if((usIn & (0x01<<0)) && !(*usInF & (0x01<<0)))
	// 	{
	// 		*usInF |= (0x01<<0);								// 언코일러 비상정지 스위치
	// 		pDoc->Status.bEmgUc = TRUE;
	// 	}
	// 	else if(!(usIn & (0x01<<0)) && (*usInF & (0x01<<0)))
	// 	{
	// 		*usInF &= ~(0x01<<0);	
	// 		pDoc->Status.bEmgUc = FALSE;
	// 	}
#endif
}

void CGvisR2R_LaserView::GetEnc()
{
	if (!m_pMotion)
		return;

	m_dEnc[AXIS_X0] = m_pMotion->GetActualPosition(AXIS_X0);
	m_dEnc[AXIS_Y0] = m_pMotion->GetActualPosition(AXIS_Y0);
	//m_dEnc[AXIS_X1] = m_pMotion->GetActualPosition(AXIS_X1);
	//m_dEnc[AXIS_Y1] = m_pMotion->GetActualPosition(AXIS_Y1);
}

// void CGvisR2R_LaserView::GetIO()
// {
//	GetSliceIO();
// // 	GetMpeIO();
// }

// void CGvisR2R_LaserView::GetSliceIO()
// {
// 	if(!m_pMotion)
// 		return;
// 
// 	int nSeg;
// 	int nInSeg = pDoc->MkIo.SliceIo.nInSeg;
// 
// 	for(nSeg=0; nSeg<nInSeg; nSeg++)
// 	{
// 		pDoc->m_pSliceIo[nSeg] = m_pMotion->Read(nSeg);
// 	}
// }
void CGvisR2R_LaserView::ChkMRegOut()
{
	m_bChkMpeIoOut = TRUE;
}
/*
void CGvisR2R_LaserView::GetMpeIO()
{
if(!m_pMpe || !pDoc->m_pMpeIo)
return;

// MpeIO
int nSize, nIdx, nLoop, nSt;
int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

// MpeIO - [In]
int *pSeg0 = new int[nInSeg];
unsigned short *pData0 = new unsigned short[nInSeg];
memset(pData0, 0, sizeof(unsigned short)*nInSeg);

// Group Read...
int nGrpStep = pDoc->MkIo.MpeIo.nGrpStep;
nSt = pDoc->MkIo.MpeIo.nGrpInSt;
for(nLoop=0; nLoop<pDoc->MkIo.MpeIo.nGrpIn; nLoop++)
{
//MReg
for(nIdx=0; nIdx<nGrpStep; nIdx++)
pSeg0[nIdx] = nIdx+nSt;
nSize = m_pMpe->ReadIO(pSeg0, nGrpStep, pData0);

for(nIdx=0; nIdx<nGrpStep; nIdx++)
pDoc->m_pMpeIo[nIdx+nSt] = pData0[nIdx];

// 		if(pDoc->Status.bManual)
{
//IReg
for(nIdx=0; nIdx<nGrpStep; nIdx++)
pSeg0[nIdx] = nIdx+nSt;
nSize = m_pMpe->ReadI(pSeg0, nGrpStep, pData0);

for(nIdx=0; nIdx<nGrpStep; nIdx++)
pDoc->m_pMpeI[nIdx+nSt] = pData0[nIdx];
}

nSt += nGrpStep;
}
delete [] pData0;
delete [] pSeg0;

m_bChkMpeIoOut = FALSE;

// MpeIO - [Out]
int *pSeg2 = new int[nOutSeg];
unsigned short *pData2 = new unsigned short[nOutSeg];
memset(pData2, 0, sizeof(unsigned short)*nOutSeg);

// Group Read...
nSt = pDoc->MkIo.MpeIo.nGrpOutSt;
for(nLoop=0; nLoop<pDoc->MkIo.MpeIo.nGrpOut; nLoop++)
{
for(nIdx=0; nIdx<nGrpStep; nIdx++)
pSeg2[nIdx] = nIdx+nSt;
nSize = m_pMpe->ReadIO(pSeg2, nGrpStep, pData2);

for(nIdx=0; nIdx<nGrpStep; nIdx++)
pDoc->m_pMpeIo[nIdx+nSt] = pData2[nIdx];

nSt += nGrpStep;
}
delete [] pData2;
delete [] pSeg2;
}
*/

void CGvisR2R_LaserView::GetMpeIO()
{
	//if (!m_pMpe || !pDoc->m_pMpeIo)
	//	return;

	//m_pMpe->GetMpeIO();
}

// void CGvisR2R_LaserView::GetMpeIO()
// {
// 	if(!m_pMpe || !pDoc->m_pMpeIo)
// 		return;
// 
// 	// MpeIO
// 	int nSize, nIdx, nLoop, nSt, nBit;
// 	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
// 	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;
// 	int nGrpStep = pDoc->MkIo.MpeIo.nGrpStep;
// 	int nGrpIn = pDoc->MkIo.MpeIo.nGrpIn;
// 	int nGrpOut = pDoc->MkIo.MpeIo.nGrpOut;
// 
// 	if(nGrpIn+nGrpOut > 15)
// 	{
// 		AfxMessageBox(_T("Error - nInSeg+nOutSeg > 15"));
// 		return;
// 	} 
// 
// 	CString sAddr;
// 	char cAddr[MAX_PATH];
// 	HREGISTERDATA	hRegisterData[15] = {0};
// 	DWORD dwRC;
// 	REGISTER_INFO	RegInfo[15];
// 	WORD RegisterWData;
// 
// 	// Group Read...
// 	//nSt = pDoc->MkIo.MpeIo.nGrpInSt;
// 	nSt = 0;
// 	for(nLoop=0; nLoop<nGrpIn; nLoop++)
// 	{
// 		sAddr = pDoc->MkIo.MpeIo.pAddrIn[nSt][0];
// 		sAddr.SetAt(1,'W');
// 		strcpy(cAddr, sAddr.Left(7));
// 		dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
// 		RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
// 		RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
// 		nSt += nGrpStep;
// 	}
// 
// 	m_bChkMpeIoOut = FALSE;
// 
// 	// Group Read...
// 	//nSt = pDoc->MkIo.MpeIo.nGrpOutSt;
// 	nSt = 0;
// 	for(nLoop=0; nLoop<nGrpOut; nLoop++)
// 	{
// 		sAddr = pDoc->MkIo.MpeIo.pAddrOut[nSt][0];
// 		sAddr.SetAt(1,'W');
// 		strcpy(cAddr, sAddr.Left(7));
// 		dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop+nGrpIn]);
// 		RegInfo[nLoop+nGrpIn].hRegisterData = hRegisterData[nLoop+nGrpIn];	// Register handle
// 		RegInfo[nLoop+nGrpIn].RegisterDataNumber = nGrpStep;		// The number of register data
// 		nSt += nGrpStep;
// 	}
// 
// 	// Sets the register data for two groups of the discontinuous register information
// 	dwRC = ymcGetGroupRegisterData( (nGrpIn+nGrpOut), &RegInfo[0] );
// 	// Error check processing
// 	if( dwRC != MP_SUCCESS )
// 	{
// 		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
// 		return;
// 	}
// 
// 	nSt = pDoc->MkIo.MpeIo.nGrpInSt;
// 	for(nLoop=0; nLoop<nGrpIn; nLoop++)
// 	{
// 		for(nIdx=0; nIdx<nGrpStep; nIdx++)
// 		{
// 			RegisterWData = *((WORD*)RegInfo[nLoop].pRegisterData+nIdx);
// 			pDoc->m_pMpeIo[nIdx+nSt] = (unsigned short)RegisterWData;
// 		}
// 		nSt += nGrpStep;
// 	}
// 
// 	nSt = pDoc->MkIo.MpeIo.nGrpOutSt;
// 	for(nLoop=0; nLoop<nGrpOut; nLoop++)
// 	{
// 		for(nIdx=0; nIdx<nGrpStep; nIdx++)
// 		{
// 			RegisterWData = *((WORD*)RegInfo[nLoop].pRegisterData+nIdx);
// 			pDoc->m_pMpeIo[nIdx+nSt] = (unsigned short)RegisterWData;
// 		}
// 		nSt += nGrpStep;
// 	}
// }

void CGvisR2R_LaserView::GetMpeSignal()
{
	//if (!m_pMpe || !pDoc->m_pMpeSignal)
	//	return;
	//m_pMpe->GetMpeSignal();
}


// void CGvisR2R_LaserView::GetMpeSignal()
// {
// 	if(!m_pMpe || !pDoc->m_pMpeSignal)
// 		return;
// 
// 	// MpeIO-I
// 	int nSize, nIdx, nLoop, nSt, nBit;
// 	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
// 	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;
// 	int nGrpStep = pDoc->MkIo.MpeIo.nGrpStep;
// 	int nGrpIn = pDoc->MkIo.MpeIo.nGrpIn;
// 	int nGrpOut = pDoc->MkIo.MpeIo.nGrpOut;
// 
// 	CString sAddr;
// 	char cAddr[MAX_PATH];
// 	HREGISTERDATA	hRegisterData[15] = {0};
// 	DWORD dwRC;
// 	REGISTER_INFO	RegInfo[15];
// 	WORD RegisterWData;
// 
// 
// 	// Group Read...
// 	//nSt = pDoc->MkIo.MpeIo.nGrpInSt;
// 	nSt = 0;
// 	for(nLoop=0; nLoop<nGrpIn; nLoop++)
// 	{
// 		sAddr = pDoc->MkIo.MpeIo.pSymIn[nSt][0];
// 		sAddr.SetAt(1,'W');
// 		strcpy(cAddr, sAddr.Left(7));
// 		dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
// 		RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
// 		RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
// 		nSt += nGrpStep;
// 	}
// 
// 	sAddr = pDoc->MkIo.MpeSignal.pAddrIn[0][0];
// 	sAddr.SetAt(1,'W');
// 	strcpy(cAddr, sAddr.Left(7));
// 	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
// 	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
// 	RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
// 
// 	nLoop++;
// 
// 	sAddr = pDoc->MkIo.MpeSignal.pAddrOut[0][0];
// 	sAddr.SetAt(1,'W');
// 	strcpy(cAddr, sAddr.Left(7));
// 	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
// 	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
// 	RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
// 
// 
// 	// Sets the register data for two groups of the discontinuous register information
// 	dwRC = ymcGetGroupRegisterData( (nGrpIn+2), &RegInfo[0] );
// 	// Error check processing
// 	if( dwRC != MP_SUCCESS )
// 		AfxMessageBox(_T("ymcSetGroupRegisterData ERROR"));
// 
// 	nSt = 0;
// 	for(nLoop=0; nLoop<nGrpIn; nLoop++)
// 	{
// 		for(nIdx=0; nIdx<nGrpStep; nIdx++)
// 		{
// 			RegisterWData = *((WORD*)RegInfo[nLoop].pRegisterData+nIdx);
// 			pDoc->m_pMpeI[nIdx+nSt] = (unsigned short)RegisterWData;
// 		}
// 		nSt += nGrpStep;
// 	}
// 
// 	nSt = pDoc->MkIo.MpeSignal.nGrpInSt;
// 	for(nIdx=0; nIdx<nGrpStep; nIdx++)
// 	{
// 		RegisterWData = *((WORD*)RegInfo[nLoop].pRegisterData+nIdx);
// 		pDoc->m_pMpeSignal[nIdx+nSt] = (unsigned short)RegisterWData;
// 	}
// 
// 	nLoop++;
// 
// 	nSt = pDoc->MkIo.MpeSignal.nGrpOutSt;
// 	for(nIdx=0; nIdx<nGrpStep; nIdx++)
// 	{
// 		RegisterWData = *((WORD*)RegInfo[nLoop].pRegisterData+nIdx);
// 		pDoc->m_pMpeSignal[nIdx+nSt] = (unsigned short)RegisterWData;
// 	}
// }

/*
void CGvisR2R_LaserView::GetMpeSignal()
{
if(!m_pMpe || !pDoc->m_pMpeSignal)
return;

// MpeSignal
int nSize, nIdx, nLoop, nSt;
int nInSeg = pDoc->MkIo.MpeSignal.nInSeg;
int nOutSeg = pDoc->MkIo.MpeSignal.nOutSeg;

// MpeSignal - [In]
int *pSeg0 = new int[nInSeg];
unsigned short *pData0 = new unsigned short[nInSeg];
memset(pData0, 0, sizeof(unsigned short)*nInSeg);

// Group Read...
int nGrpStep = pDoc->MkIo.MpeSignal.nGrpStep;
nSt = pDoc->MkIo.MpeSignal.nGrpInSt;
for(nLoop=0; nLoop<pDoc->MkIo.MpeSignal.nGrpIn; nLoop++)
{
for(nIdx=0; nIdx<nGrpStep; nIdx++)
pSeg0[nIdx] = nIdx+nSt;
nSize = m_pMpe->ReadSignal(pSeg0, nGrpStep, pData0);

for(nIdx=0; nIdx<nGrpStep; nIdx++)
pDoc->m_pMpeSignal[nIdx+nSt] = pData0[nIdx];

nSt += nGrpStep;
}
delete [] pData0;
delete [] pSeg0;

m_bChkMpeIoOut = FALSE;

// MpeSignal - [Out]
int *pSeg2 = new int[nOutSeg];
unsigned short *pData2 = new unsigned short[nOutSeg];
memset(pData2, 0, sizeof(unsigned short)*nOutSeg);

// Group Read...
nSt = pDoc->MkIo.MpeSignal.nGrpOutSt;
for(nLoop=0; nLoop<pDoc->MkIo.MpeSignal.nGrpOut; nLoop++)
{
for(nIdx=0; nIdx<nGrpStep; nIdx++)
pSeg2[nIdx] = nIdx+nSt;
nSize = m_pMpe->ReadSignal(pSeg2, nGrpStep, pData2);

for(nIdx=0; nIdx<nGrpStep; nIdx++)
pDoc->m_pMpeSignal[nIdx+nSt] = pData2[nIdx];

nSt += nGrpStep;
}
delete [] pData2;
delete [] pSeg2;
}
*/

void CGvisR2R_LaserView::GetMpeData()
{
	//if (!m_pMpe || !pDoc->m_pMpeData)
	//	return;

	//// MpeData
	//int nSize, nIdx, nLoop, nSt, k;
	//int nInSeg = pDoc->MkIo.MpeData.nInSeg;
	//int nOutSeg = pDoc->MkIo.MpeData.nOutSeg;

	//// MpeData - [In]
	//int *pSeg0 = new int[nInSeg];
	//long **pData0 = new long*[nInSeg];
	//for (nIdx = 0; nIdx < nInSeg; nIdx++)
	//{
	//	pData0[nIdx] = new long[16];
	//	memset(pData0[nIdx], 0, sizeof(long) * 16);
	//}

	//// Group Read...
	//int nGrpStep = pDoc->MkIo.MpeData.nGrpStep;
	//nSt = pDoc->MkIo.MpeData.nGrpInSt;
	//for (nLoop = 0; nLoop < pDoc->MkIo.MpeData.nGrpIn; nLoop++)
	//{
	//	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	//		pSeg0[nIdx] = nIdx + nSt;
	//	nSize = m_pMpe->ReadData(pSeg0, nGrpStep, pData0);

	//	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	//	{
	//		for (k = 0; k < 16; k++)
	//			pDoc->m_pMpeData[nIdx + nSt][k] = pData0[nIdx][k];
	//	}

	//	nSt += nGrpStep;
	//}
	//for (nIdx = 0; nIdx < nInSeg; nIdx++)
	//	delete[] pData0[nIdx];
	//delete[] pData0;
	//delete[] pSeg0;

	//m_bChkMpeIoOut = FALSE;

	//// MpeData - [Out]
	//int *pSeg2 = new int[nOutSeg];
	//long **pData2 = new long*[nOutSeg];
	//for (nIdx = 0; nIdx < nOutSeg; nIdx++)
	//{
	//	pData2[nIdx] = new long[16];
	//	memset(pData2[nIdx], 0, sizeof(unsigned long) * 16);
	//}

	//// Group Read...
	//nSt = pDoc->MkIo.MpeData.nGrpOutSt;
	//for (nLoop = 0; nLoop < pDoc->MkIo.MpeData.nGrpOut; nLoop++)
	//{
	//	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	//		pSeg2[nIdx] = nIdx + nSt;
	//	nSize = m_pMpe->ReadData(pSeg2, nGrpStep, pData2);

	//	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	//	{
	//		for (k = 0; k < 16; k++)
	//			pDoc->m_pMpeData[nIdx + nSt][k] = pData2[nIdx][k];
	//	}

	//	nSt += nGrpStep;
	//}
	//for (nIdx = 0; nIdx < nOutSeg; nIdx++)
	//	delete[] pData2[nIdx];
	//delete[] pData2;
	//delete[] pSeg2;
}

int CGvisR2R_LaserView::GetTime(int nSel)
{

	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	switch (nSel)
	{
	case YEAR:
		return Tim.GetYear();
	case MONTH:
		return Tim.GetMonth();
	case DAY:
		return Tim.GetDay();
	case HOUR:
		return Tim.GetHour();
	case MINUTE:
		return Tim.GetMinute();
	case SEC:
		return Tim.GetSecond();
	}

	return 0;
}

CString CGvisR2R_LaserView::GetTime()
{
	CString strVal;
	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	int nYear = Tim.GetYear();
	int nMonth = Tim.GetMonth();
	int nDay = Tim.GetDay();
	int nHour = Tim.GetHour();
	int nMinute = Tim.GetMinute();
	int nSec = Tim.GetSecond();

	strVal.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMinute, nSec);
	return strVal;
}

CString CGvisR2R_LaserView::GetTime(stLotTime &LotTime)
{
	CString strVal;
	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	LotTime.nYear = Tim.GetYear();
	LotTime.nMonth = Tim.GetMonth();
	LotTime.nDay = Tim.GetDay();
	LotTime.nHour = Tim.GetHour();
	LotTime.nMin = Tim.GetMinute();
	LotTime.nSec = Tim.GetSecond();

	strVal.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), LotTime.nYear, LotTime.nMonth, LotTime.nDay,
		LotTime.nHour, LotTime.nMin, LotTime.nSec);
	return strVal;
}


CString CGvisR2R_LaserView::GetTime(int &nHour, int &nMinute, int &nSec)
{
	CString strVal;
	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	int nYear = Tim.GetYear();
	int nMonth = Tim.GetMonth();
	int nDay = Tim.GetDay();
	nHour = Tim.GetHour();
	nMinute = Tim.GetMinute();
	nSec = Tim.GetSecond();

	strVal.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMinute, nSec);
	return strVal;
}

void CGvisR2R_LaserView::DispTime()
{
	stLotTime LotTime;
	CString str;
	str = GetTime(LotTime);
	if (m_sDispTime != str)
	{
		m_sDispTime = str;
		pFrm->DispStatusBar(str, 7);

		pDoc->WorkingInfo.Lot.CurTime.nYear = LotTime.nYear;
		pDoc->WorkingInfo.Lot.CurTime.nMonth = LotTime.nMonth;
		pDoc->WorkingInfo.Lot.CurTime.nDay = LotTime.nDay;
		pDoc->WorkingInfo.Lot.CurTime.nHour = LotTime.nHour;
		pDoc->WorkingInfo.Lot.CurTime.nMin = LotTime.nMin;
		pDoc->WorkingInfo.Lot.CurTime.nSec = LotTime.nSec;

		if (m_pDlgMenu01)
			m_pDlgMenu01->DispRunTime();
	}
}

// System Input IO .......................................................................
void CGvisR2R_LaserView::DoSaftySens()
{
	//if (!pDoc->m_pMpeI || !pDoc->m_pMpeIF)
	//	return;

	////BOOL bOn = pDoc->m_pMpeIb[7] & (0x01 << 8) ? TRUE : FALSE;	// 마킹부 안전 센서 1
	////BOOL bOnF = pDoc->m_pMpeIF[7] & (0x01 << 8) ? TRUE : FALSE;	// 마킹부 안전 센서 1

	//unsigned short usIn = pDoc->m_pMpeI[7];
	//unsigned short *usInF = &pDoc->m_pMpeIF[7];


	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))		// 마킹부 안전 센서
	//{
	//	*usInF |= (0x01 << 8);								
	//	pDoc->Status.bSensSaftyMk = TRUE;
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))	// 마킹부 안전 센서
	//{
	//	*usInF &= ~(0x01 << 8);
	//	pDoc->Status.bSensSaftyMk = FALSE;
	//}
}

void CGvisR2R_LaserView::DoDoorSens()
{
	//unsigned short usIn;
	//unsigned short *usInF;

	//if (!pDoc->m_pMpeI || !pDoc->m_pMpeIF)
	//	return;

	//usIn = pDoc->m_pMpeI[1];
	//usInF = &pDoc->m_pMpeIF[1];

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 언코일러 전면 도어 센서
	//{
	//	*usInF |= (0x01 << 12);
	//	pDoc->Status.bDoorUc[DOOR_FL_UC] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 언코일러 측면 도어 센서
	//{
	//	*usInF &= ~(0x01 << 13);
	//	pDoc->Status.bDoorUc[DOOR_FR_UC] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))	// 언코일러 후면 도어 센서(좌)
	//{
	//	*usInF &= ~(0x01 << 14);
	//	pDoc->Status.bDoorUc[DOOR_BL_UC] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))	// 언코일러 후면 도어 센서(우)
	//{
	//	*usInF &= ~(0x01 << 15);
	//	pDoc->Status.bDoorUc[DOOR_BR_UC] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[7];
	//usInF = &pDoc->m_pMpeIF[7];

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 마킹부 도어 센서 1 
	//{
	//	*usInF |= (0x01 << 10);
	//	pDoc->Status.bDoorMk[DOOR_FL_MK] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 마킹부 도어 센서 2
	//{
	//	*usInF &= ~(0x01 << 11);
	//	pDoc->Status.bDoorMk[DOOR_FR_MK] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 마킹부 도어 센서 3
	//{
	//	*usInF &= ~(0x01 << 12);
	//	pDoc->Status.bDoorMk[DOOR_BL_MK] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 마킹부 도어 센서 4
	//{
	//	*usInF &= ~(0x01 << 13);
	//	pDoc->Status.bDoorMk[DOOR_BR_MK] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[11];
	//usInF = &pDoc->m_pMpeIF[11];

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 검사부 상 도어 센서 1 
	//{
	//	*usInF |= (0x01 << 10);
	//	pDoc->Status.bDoorMk[DOOR_FL_AOI_UP] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 검사부 상 도어 센서 2
	//{
	//	*usInF &= ~(0x01 << 11);
	//	pDoc->Status.bDoorMk[DOOR_FR_AOI_UP] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 검사부 상 도어 센서 3
	//{
	//	*usInF &= ~(0x01 << 12);
	//	pDoc->Status.bDoorMk[DOOR_BL_AOI_UP] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 검사부 상 도어 센서 4
	//{
	//	*usInF &= ~(0x01 << 13);
	//	pDoc->Status.bDoorMk[DOOR_BR_AOI_UP] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[15];
	//usInF = &pDoc->m_pMpeIF[15];

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 검사부 상 도어 센서 1 
	//{
	//	*usInF |= (0x01 << 10);
	//	pDoc->Status.bDoorAoi[DOOR_FL_AOI_UP] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 검사부 상 도어 센서 2
	//{
	//	*usInF &= ~(0x01 << 11);
	//	pDoc->Status.bDoorAoi[DOOR_FR_AOI_UP] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 검사부 상 도어 센서 3
	//{
	//	*usInF &= ~(0x01 << 12);
	//	pDoc->Status.bDoorAoi[DOOR_BL_AOI_UP] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 검사부 상 도어 센서 4
	//{
	//	*usInF &= ~(0x01 << 13);
	//	pDoc->Status.bDoorAoi[DOOR_BR_AOI_UP] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[15];
	//usInF = &pDoc->m_pMpeIF[15];

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 검사부 하 도어 센서 1 
	//{
	//	*usInF |= (0x01 << 10);
	//	pDoc->Status.bDoorAoi[DOOR_FL_AOI_DN] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 검사부 하 도어 센서 2
	//{
	//	*usInF &= ~(0x01 << 11);
	//	pDoc->Status.bDoorAoi[DOOR_FR_AOI_DN] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 검사부 하 도어 센서 3
	//{
	//	*usInF &= ~(0x01 << 12);
	//	pDoc->Status.bDoorAoi[DOOR_BL_AOI_DN] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 검사부 하 도어 센서 4
	//{
	//	*usInF &= ~(0x01 << 13);
	//	pDoc->Status.bDoorAoi[DOOR_BR_AOI_DN] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[17];
	//usInF = &pDoc->m_pMpeIF[17];

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 리코일러 전면 도어 센서
	//{
	//	*usInF |= (0x01 << 12);
	//	pDoc->Status.bDoorRe[DOOR_FL_RC] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 리코일러 측면 도어 센서
	//{
	//	*usInF &= ~(0x01 << 13);
	//	pDoc->Status.bDoorRe[DOOR_FR_RC] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))	// 리코일러 후면 도어 센서(좌)
	//{
	//	*usInF &= ~(0x01 << 14);
	//	pDoc->Status.bDoorRe[DOOR_BL_RC] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))	// 리코일러 후면 도어 센서(우)
	//{
	//	*usInF &= ~(0x01 << 15);
	//	pDoc->Status.bDoorRe[DOOR_BR_RC] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[27];
	//usInF = &pDoc->m_pMpeIF[27];

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 각인부 도어 센서 1
	//{
	//	*usInF |= (0x01 << 10);
	//	pDoc->Status.bDoorEngv[DOOR_FL_ENGV] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 각인부 도어 센서 2
	//{
	//	*usInF &= ~(0x01 << 11);
	//	pDoc->Status.bDoorEngv[DOOR_FR_ENGV] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 각인부 도어 센서 3
	//{
	//	*usInF &= ~(0x01 << 12);
	//	pDoc->Status.bDoorEngv[DOOR_BL_ENGV] = FALSE;
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 각인부 도어 센서 4
	//{
	//	*usInF &= ~(0x01 << 13);
	//	pDoc->Status.bDoorEngv[DOOR_BR_ENGV] = FALSE;
	//}
}

void CGvisR2R_LaserView::DoModeSel()
{
	//if (!pDoc->m_pMpeI)
	//	return;

	//BOOL bMode[2];
	//bMode[0] = pDoc->m_pMpeI[4] & (0x01 << 5) ? TRUE : FALSE;	// 마킹부 자동/수동/1회운전 1
	//bMode[1] = pDoc->m_pMpeI[4] & (0x01 << 6) ? TRUE : FALSE;	// 마킹부 자동/수동/1회운전 2

	//if (bMode[0] && !bMode[1])		 // 마킹부 자동/수동/1회운전 (1,2)
	//{
	//	pDoc->Status.bAuto = TRUE;
	//	pDoc->Status.bManual = FALSE;
	//	pDoc->Status.bOneCycle = FALSE;
	//}
	//else if (!bMode[0] && bMode[1])
	//{
	//	pDoc->Status.bAuto = FALSE;
	//	pDoc->Status.bManual = FALSE;
	//	pDoc->Status.bOneCycle = TRUE;
	//}
	//else if (!bMode[0] && !bMode[1])
	//{
	//	pDoc->Status.bAuto = FALSE;
	//	pDoc->Status.bManual = TRUE;
	//	pDoc->Status.bOneCycle = FALSE;
	//}
	//else
	//{
	//	pDoc->Status.bAuto = FALSE;
	//	pDoc->Status.bManual = FALSE;
	//	pDoc->Status.bOneCycle = FALSE;
	//}


	//if (pDoc->Status.bManual && !m_bManual)
	//{
	//	m_bManual = TRUE;
	//	m_bAuto = FALSE;
	//	m_bOneCycle = FALSE;

	//	if (m_pDlgMenu03)
	//	{
	//		m_pDlgMenu03->DoManual();
	//	}
	//}
	//else if (pDoc->Status.bAuto && !m_bAuto)
	//{
	//	m_bManual = FALSE;
	//	m_bAuto = TRUE;
	//	m_bOneCycle = FALSE;

	//	if (m_pDlgMenu03)
	//	{
	//		m_pDlgMenu03->DoAuto();
	//	}
	//}
	//else if (pDoc->Status.bOneCycle && !m_bOneCycle)
	//{
	//	m_bManual = FALSE;
	//	m_bAuto = FALSE;
	//	m_bOneCycle = TRUE;
	//}

}

void CGvisR2R_LaserView::DoMainSw()
{
	//// 	if(!pDoc->m_pSliceIo || !pDoc->m_pSliceIoF)
	//// 		return;
	//// 
	//// 	unsigned short usIn = pDoc->m_pSliceIo[0];
	//// 	unsigned short *usInF = &pDoc->m_pSliceIoF[0];
	//unsigned short usIn = pDoc->m_pMpeI[4];
	//unsigned short *usInF = &pDoc->m_pMpeIF[4];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	//{
	//	*usInF |= (0x01 << 0);								// 마킹부 비상정지 스위치(모니터부)
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 마킹부 운전 스위치
	//	m_bSwStopNow = FALSE;
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwRun();
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// 마킹부 정지 스위치
	//														// 		m_bSwStopNow = TRUE;
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwStop();
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// 마킹부 운전준비 스위치
	//	m_bSwStopNow = FALSE;
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwReady();
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// 마킹부 리셋 스위치
	//	m_bSwStopNow = FALSE;
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwReset();
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//}

	//// 	if((usIn & (0x01<<6)) && !(*usInF & (0x01<<6)))
	//// 	{
	//// 		*usInF |= (0x01<<6);								// 마킹부 마킹 스위치
	//// 
	//// 		BOOL bOn = pDoc->m_pSliceIo[7] & (0x01<<10) ? TRUE : FALSE;	// 마킹부 토크 클램프 스위치 램프 -> 마킹부 마킹 실린더 SOL
	//// 		if(!bOn)
	//// 		{
	//// 			if(m_pDlgMenu03)
	//// 				m_pDlgMenu03->SwMkDnSol(TRUE);
	//// 			Sleep(300);
	//// 		}
	//// 		if(m_pDlgMenu02)
	//// 		{
	//// 			m_pDlgMenu02->m_bMkDnSolOff = TRUE;
	//// 			m_pDlgMenu02->SwMarking();
	//// 		}
	//// 	}
	//// 	else if(!(usIn & (0x01<<6)) && (*usInF & (0x01<<6)))
	//// 	{
	//// 		*usInF &= ~(0x01<<6);								
	//// 	}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// 마킹부 JOG 버튼(상)
	//	//if (pDoc->Status.bSwJogLeft)
	//		SwJog(AXIS_Y0, M_CCW, TRUE);
	//	//else
	//	//	SwJog(AXIS_Y1, M_CCW, TRUE);
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//	//if (pDoc->Status.bSwJogLeft)
	//		SwJog(AXIS_Y0, M_CCW, FALSE);
	//	//else
	//	//	SwJog(AXIS_Y1, M_CCW, FALSE);
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 마킹부 JOG 버튼(하)
	//	//if (pDoc->Status.bSwJogLeft)
	//		SwJog(AXIS_Y0, M_CW, TRUE);
	//	//else
	//	//	SwJog(AXIS_Y1, M_CW, TRUE);
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//	//if (pDoc->Status.bSwJogLeft)
	//		SwJog(AXIS_Y0, M_CW, FALSE);
	//	//else
	//	//	SwJog(AXIS_Y1, M_CW, FALSE);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 마킹부 JOG 버튼(좌)
	//	//if (pDoc->Status.bSwJogLeft)
	//		SwJog(AXIS_X0, M_CCW, TRUE);
	//	//else
	//	//	SwJog(AXIS_X1, M_CCW, TRUE);
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//	//if (pDoc->Status.bSwJogLeft)
	//		SwJog(AXIS_X0, M_CCW, FALSE);
	//	//else
	//	//	SwJog(AXIS_X1, M_CCW, FALSE);
	//}

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	//{
	//	*usInF |= (0x01 << 10);								// 마킹부 JOG 버튼(우)
	//	//if (pDoc->Status.bSwJogLeft)
	//		SwJog(AXIS_X0, M_CW, TRUE);
	//	//else
	//	//	SwJog(AXIS_X1, M_CW, TRUE);
	//}
	//else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	//{
	//	*usInF &= ~(0x01 << 10);
	//	//if (pDoc->Status.bSwJogLeft)
	//		SwJog(AXIS_X0, M_CW, FALSE);
	//	//else
	//	//	SwJog(AXIS_X1, M_CW, FALSE);
	//}

	//if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	//{
	//	*usInF |= (0x01 << 11);								// 마킹부 모션 선택(LEFT)
	//	pDoc->Status.bSwJogLeft = TRUE;
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	//{
	//	*usInF &= ~(0x01 << 11);
	//	pDoc->Status.bSwJogLeft = FALSE;
	//}

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	//{
	//	*usInF |= (0x01 << 12);								// 마킹부 속도 선택
	//	pDoc->Status.bSwJogFast = TRUE;
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	//{
	//	*usInF &= ~(0x01 << 12);
	//	pDoc->Status.bSwJogFast = FALSE;
	//}

	//if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	//{
	//	*usInF |= (0x01 << 13);								// 마킹부 운전 선택(INDEX)
	//	pDoc->Status.bSwJogStep = TRUE;
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	//{
	//	*usInF &= ~(0x01 << 13);
	//	pDoc->Status.bSwJogStep = FALSE;
	//}

	//if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	//{
	//	*usInF |= (0x01 << 14);								// SPARE	
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	//{
	//	*usInF &= ~(0x01 << 14);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	//{
	//	*usInF |= (0x01 << 15);								// SPARE	
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	//{
	//	*usInF &= ~(0x01 << 15);
	//	// No Use....
	//}

	//if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
	//	return;


	//// 	usIn = pDoc->m_pMpeIo[0];
	//// 	usInF = &pDoc->m_pMpeIoF[0];
	//// 
	//// 	if((usIn & (0x01<<14)) && !(*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF |= (0x01<<14);								// 언코일러 제품 EPC원점 스위치
	//// 		m_bSwStopNow = TRUE;
	//// 		if(m_pDlgMenu03)
	//// 			m_pDlgMenu03->SwStop();
	//// 	}
	//// 	else if(!(usIn & (0x01<<14)) && (*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF &= ~(0x01<<14);								
	//// 	}
	//// 
	//// 
	//// 	usIn = pDoc->m_pMpeIo[4];
	//// 	usInF = &pDoc->m_pMpeIoF[4];
	//// 
	//// 	if((usIn & (0x01<<14)) && !(*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF |= (0x01<<14);								// 리코일러 제품 EPC원점 스위치
	//// 		m_bSwStopNow = TRUE;
	//// 		if(m_pDlgMenu03)
	//// 			m_pDlgMenu03->SwStop();
	//// 	}
	//// 	else if(!(usIn & (0x01<<14)) && (*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF &= ~(0x01<<14);								
	//// 	}
}


void CGvisR2R_LaserView::DoMkSens()
{
	//// 	if(!pDoc->m_pSliceIo || !pDoc->m_pSliceIoF)
	//// 		return;
	//// 
	//// 	unsigned short usIn = pDoc->m_pSliceIo[2];
	//// 	unsigned short *usInF = &pDoc->m_pSliceIoF[2];
	//unsigned short usIn = 0;
	//unsigned short *usInF = NULL;

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	//{
	//	*usInF |= (0x01 << 0);								// 마킹부 테이블 진공 센서
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 마킹부 테이블 압력 스위치
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// 마킹부 피딩 진공 센서
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// 마킹부 피딩 클램프 상승 센서
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// 마킹부 피딩 클램프 하강 센서
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//}

	//if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	//{
	//	*usInF |= (0x01 << 5);								// 마킹부 토크 진공 센서
	//}
	//else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	//{
	//	*usInF &= ~(0x01 << 5);
	//}

	//if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	//{
	//	*usInF |= (0x01 << 6);								// 마킹부 토크 클램프 상승 센서
	//}
	//else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	//{
	//	*usInF &= ~(0x01 << 6);
	//}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// 마킹부 토크 클램프 하강 센서
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 마킹부 댄서롤 상승 센서
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 마킹부 댄서롤 하강 센서
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//}

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	//{
	//	*usInF |= (0x01 << 10);								// 검사부 피딩 진공 센서
	//}
	//else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	//{
	//	*usInF &= ~(0x01 << 10);
	//}

	//if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	//{
	//	*usInF |= (0x01 << 11);								// 검사부 피딩 클램프 상승 센서
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	//{
	//	*usInF &= ~(0x01 << 11);
	//}

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	//{
	//	*usInF |= (0x01 << 12);								// 검사부 피딩 클램프 하강 센서
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	//{
	//	*usInF &= ~(0x01 << 12);
	//}

	//if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	//{
	//	*usInF |= (0x01 << 13);								// 검사부 토크 진공 센서
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	//{
	//	*usInF &= ~(0x01 << 13);
	//}

	//if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	//{
	//	*usInF |= (0x01 << 14);								// 검사부 토크 클램프 상승 센서
	//}
	//else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	//{
	//	*usInF &= ~(0x01 << 14);
	//}

	//if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	//{
	//	*usInF |= (0x01 << 15);								// 검사부 토크 클램프 하강 센서
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	//{
	//	*usInF &= ~(0x01 << 15);
	//}
}

void CGvisR2R_LaserView::DoAoiBoxSw()
{
	//// 	if(!pDoc->m_pSliceIo || !pDoc->m_pSliceIoF)
	//// 		return;
	//// 
	//// 	unsigned short usIn = pDoc->m_pSliceIo[3];
	//// 	unsigned short *usInF = &pDoc->m_pSliceIoF[3];
	//unsigned short usIn = 0;
	//unsigned short *usInF = NULL;

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	//{
	//	*usInF |= (0x01 << 0);								// 검사부 연동 온/오프 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwAoiRelation();
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 검사부 테이블 브로워 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwAoiTblBlw();
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// 검사부 피딩 정회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_18);
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_18);
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// 검사부 피딩 역회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_19);
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_19);
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// 검사부 피딩 진공 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwAoiFdVac();
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//}

	//if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	//{
	//	*usInF |= (0x01 << 5);								// 검사부 토크 진공 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwAoiTqVac();
	//}
	//else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	//{
	//	*usInF &= ~(0x01 << 5);
	//}

	//if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	//{
	//	*usInF |= (0x01 << 6);								// 검사부 테이블 진공 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwAoiTblVac();
	//}
	//else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	//{
	//	*usInF &= ~(0x01 << 6);
	//}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// 검사부 레이져 포인터 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwAoiLsrPt(TRUE);
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwAoiLsrPt(FALSE);
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 검사부 피딩 클램프 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwAoiFdClp();
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 검사부 토크 클램프 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwAoiTqClp();
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//}

	//// 	if((usIn & (0x01<<10)) && !(*usInF & (0x01<<10)))
	//// 	{
	//// 		*usInF |= (0x01<<10);								// 검사부 전면 도어 센서(중)
	//// 		pDoc->Status.bDoorAoi[DOOR_FM_AOI] = TRUE;
	//// 	}
	//// 	else if(!(usIn & (0x01<<10)) && (*usInF & (0x01<<10)))
	//// 	{
	//// 		*usInF &= ~(0x01<<10);								
	//// 		pDoc->Status.bDoorAoi[DOOR_FM_AOI] = FALSE;
	//// 	}
	//// 
	//// 	if((usIn & (0x01<<11)) && !(*usInF & (0x01<<11)))
	//// 	{
	//// 		*usInF |= (0x01<<11);								// 검사부 전면 도어 센서(좌)
	//// 		pDoc->Status.bDoorAoi[DOOR_FL_AOI] = TRUE;
	//// 	}
	//// 	else if(!(usIn & (0x01<<11)) && (*usInF & (0x01<<11)))
	//// 	{
	//// 		*usInF &= ~(0x01<<11);								
	//// 		pDoc->Status.bDoorAoi[DOOR_FL_AOI] = FALSE;
	//// 	}
	//// 
	//// 	if((usIn & (0x01<<12)) && !(*usInF & (0x01<<12)))
	//// 	{
	//// 		*usInF |= (0x01<<12);								// 검사부 전면 도어 센서(우)
	//// 		pDoc->Status.bDoorAoi[DOOR_FR_AOI] = TRUE;
	//// 	}
	//// 	else if(!(usIn & (0x01<<12)) && (*usInF & (0x01<<12)))
	//// 	{
	//// 		*usInF &= ~(0x01<<12);								
	//// 		pDoc->Status.bDoorAoi[DOOR_FR_AOI] = FALSE;
	//// 	}
	//// 
	//// 	if((usIn & (0x01<<13)) && !(*usInF & (0x01<<13)))
	//// 	{
	//// 		*usInF |= (0x01<<13);								// 검사부 후면 도어 센서(중)
	//// 		pDoc->Status.bDoorAoi[DOOR_BM_AOI] = TRUE;
	//// 	}
	//// 	else if(!(usIn & (0x01<<13)) && (*usInF & (0x01<<13)))
	//// 	{
	//// 		*usInF &= ~(0x01<<13);								
	//// 		pDoc->Status.bDoorAoi[DOOR_BM_AOI] = FALSE;
	//// 	}
	//// 
	//// 	if((usIn & (0x01<<14)) && !(*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF |= (0x01<<14);								// 검사부 후면 도어 센서(좌)
	//// 		pDoc->Status.bDoorAoi[DOOR_BL_AOI] = TRUE;
	//// 	}
	//// 	else if(!(usIn & (0x01<<14)) && (*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF &= ~(0x01<<14);								
	//// 		pDoc->Status.bDoorAoi[DOOR_BL_AOI] = FALSE;
	//// 	}
	//// 
	//// 	if((usIn & (0x01<<15)) && !(*usInF & (0x01<<15)))
	//// 	{
	//// 		*usInF |= (0x01<<15);								// 검사부 후면 도어 센서(우)
	//// 		pDoc->Status.bDoorAoi[DOOR_BR_AOI] = TRUE;
	//// 	}
	//// 	else if(!(usIn & (0x01<<15)) && (*usInF & (0x01<<15)))
	//// 	{
	//// 		*usInF &= ~(0x01<<15);								
	//// 		pDoc->Status.bDoorAoi[DOOR_BR_AOI] = FALSE;
	//// 	}
}

void CGvisR2R_LaserView::DoEmgSens()
{
	//// 	if(!pDoc->m_pSliceIo || !pDoc->m_pSliceIoF)
	//// 		return;

	//// 	unsigned short usIn = pDoc->m_pSliceIo[4];
	//// 	unsigned short *usInF = &pDoc->m_pSliceIoF[4];

	////unsigned short usIn = 0;
	////unsigned short *usInF = NULL;

	//unsigned short usIn, *usInF;

	//if (!pDoc->m_pMpeI || !pDoc->m_pMpeIF)
	//	return;

	//usIn = pDoc->m_pMpeI[0];
	//usInF = &pDoc->m_pMpeIF[0];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 언코일러 비상정지 스위치
	//{
	//	*usInF |= (0x01 << 0);						
	//	pDoc->Status.bEmgUc = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bEmgUc = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[4];
	//usInF = &pDoc->m_pMpeIF[4];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 마킹부 비상정지 스위치(모니터부)
	//{
	//	*usInF |= (0x01 << 0);						
	//	pDoc->Status.bEmgMk[EMG_M_MK] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bEmgMk[EMG_M_MK] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[5];
	//usInF = &pDoc->m_pMpeIF[5];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 마킹부 비상정지 스위치(스위치부)	
	//{
	//	*usInF |= (0x01 << 0);						
	//	pDoc->Status.bEmgMk[EMG_B_MK] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bEmgMk[EMG_B_MK] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[8];
	//usInF = &pDoc->m_pMpeIF[8];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 검사부 상 비상정지 스위치(후면) 
	//{
	//	*usInF |= (0x01 << 0);						
	//	pDoc->Status.bEmgAoi[EMG_B_AOI_UP] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bEmgAoi[EMG_B_AOI_UP] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[12];
	//usInF = &pDoc->m_pMpeIF[12];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 검사부 하 비상정지 스위치(후면) 
	//{
	//	*usInF |= (0x01 << 0);						
	//	pDoc->Status.bEmgAoi[EMG_B_AOI_DN] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bEmgAoi[EMG_B_AOI_DN] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[16];
	//usInF = &pDoc->m_pMpeIF[16];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 리코일러 비상정지 스위치
	//{
	//	*usInF |= (0x01 << 0);
	//	pDoc->Status.bEmgRc = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bEmgRc = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[24];
	//usInF = &pDoc->m_pMpeIF[24];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 각인부 비상정지 스위치(모니터부)
	//{
	//	*usInF |= (0x01 << 0);
	//	pDoc->Status.bEmgEngv[0] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bEmgEngv[0] = FALSE;
	//}

	//usIn = pDoc->m_pMpeI[25];
	//usInF = &pDoc->m_pMpeIF[25];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 각인부 비상정지 스위치(스위치부)
	//{
	//	*usInF |= (0x01 << 0);
	//	pDoc->Status.bEmgEngv[1] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bEmgEngv[1] = FALSE;
	//}

}

void CGvisR2R_LaserView::DoSignal()
{
	//// 	if(!pDoc->m_pSliceIo || !pDoc->m_pSliceIoF)
	//// 		return;
	//// 
	//// 	unsigned short usIn = pDoc->m_pSliceIo[5];
	//// 	unsigned short *usInF = &pDoc->m_pSliceIoF[5];
	//unsigned short usIn = 0;
	//unsigned short *usInF = NULL;

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	//{
	//	*usInF |= (0x01 << 0);								// 검사부 검사 완료
	//	pDoc->Status.bSigTestDoneAoi = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bSigTestDoneAoi = FALSE;
	//}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 검사부 테이블 진공 완료
	//	pDoc->Status.bSigTblAirAoi = TRUE;
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//	pDoc->Status.bSigTblAirAoi = FALSE;
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	//{
	//	*usInF |= (0x01 << 5);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	//{
	//	*usInF &= ~(0x01 << 5);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	//{
	//	*usInF |= (0x01 << 6);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	//{
	//	*usInF &= ~(0x01 << 6);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 검사부 시그날 타워-적색
	//	TowerLamp(RGB_RED, TRUE, FALSE);
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//	TowerLamp(RGB_RED, FALSE, FALSE);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 검사부 시그날 타워-황색
	//	TowerLamp(RGB_YELLOW, TRUE, FALSE);
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//	TowerLamp(RGB_YELLOW, FALSE, FALSE);
	//}

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	//{
	//	*usInF |= (0x01 << 10);								// 검사부 시그날 타워-녹색
	//	TowerLamp(RGB_GREEN, TRUE, FALSE);
	//}
	//else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	//{
	//	*usInF &= ~(0x01 << 10);
	//	TowerLamp(RGB_GREEN, FALSE, FALSE);
	//}

	//if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	//{
	//	*usInF |= (0x01 << 11);								// 검사부 부져 1
	//	Buzzer(TRUE, 0);
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	//{
	//	*usInF &= ~(0x01 << 11);
	//	Buzzer(FALSE, 0);
	//}

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	//{
	//	*usInF |= (0x01 << 12);								// 검사부 부져 2
	//	Buzzer(TRUE, 1);
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	//{
	//	*usInF &= ~(0x01 << 12);
	//	Buzzer(FALSE, 1);
	//}

	//if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	//{
	//	*usInF |= (0x01 << 13);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	//{
	//	*usInF &= ~(0x01 << 13);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	//{
	//	*usInF |= (0x01 << 14);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	//{
	//	*usInF &= ~(0x01 << 14);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	//{
	//	*usInF |= (0x01 << 15);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	//{
	//	*usInF &= ~(0x01 << 15);
	//	// No Use....
	//}
}

void CGvisR2R_LaserView::DoUcBoxSw()
{
	//if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
	//	return;

	//// 	unsigned short usIn = pDoc->m_pMpeIo[0];
	//// 	unsigned short *usInF = &pDoc->m_pMpeIoF[0];
	//unsigned short usIn = 0;
	//unsigned short *usInF = NULL;

	//// 	if((usIn & (0x01<<0)) && !(*usInF & (0x01<<0)))
	//// 	{
	//// 		*usInF |= (0x01<<0);								// 언코일러 비상정지 스위치
	//// 		pDoc->Status.bEmgUc = TRUE;
	//// 	}
	//// 	else if(!(usIn & (0x01<<0)) && (*usInF & (0x01<<0)))
	//// 	{
	//// 		*usInF &= ~(0x01<<0);	
	//// 		pDoc->Status.bEmgUc = FALSE;
	//// 	}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 언코일러 연동 온/오프 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwUcRelation();
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// 언코일러 댄서롤 상승/하강 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwUcDcRlUpDn();
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// 언코일러 클린롤러 상승/하강 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwUcClRlUpDn();
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// 언코일러 클린롤러누름 상승/하강 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwUcClRlPshUpDn();
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//}

	//if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	//{
	//	*usInF |= (0x01 << 5);								// 언코일러 제품 이음매(좌) 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwUcReelJoinL();
	//}
	//else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	//{
	//	*usInF &= ~(0x01 << 5);
	//}

	//if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	//{
	//	*usInF |= (0x01 << 6);								// 언코일러 제품 이음매(우) 스위치	
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwUcReelJoinR();
	//}
	//else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	//{
	//	*usInF &= ~(0x01 << 6);
	//}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// 언코일러 제품휠 지지 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwUcReelWheel();
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 언코일러 간지척 클램프 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwUcPprChuck();
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 언코일러 간지휠 정회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_32);
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_32);
	//}

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	//{
	//	*usInF |= (0x01 << 10);								// 언코일러 간지휠 역회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_39);
	//}
	//else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	//{
	//	*usInF &= ~(0x01 << 10);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_39);
	//}

	//if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	//{
	//	*usInF |= (0x01 << 11);								// 언코일러 제품척 클램프 스위치
	//	if (m_pDlgMenu03)
	//		m_pDlgMenu03->SwUcReelChuck();
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	//{
	//	*usInF &= ~(0x01 << 11);
	//}

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	//{
	//	*usInF |= (0x01 << 12);								// 언코일러 제품휠 정회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_26);
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	//{
	//	*usInF &= ~(0x01 << 12);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_26);
	//}

	//if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	//{
	//	*usInF |= (0x01 << 13);								// 언코일러 제품휠 역회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_27);
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	//{
	//	*usInF &= ~(0x01 << 13);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_27);
	//}

	//// 	if((usIn & (0x01<<14)) && !(*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF |= (0x01<<14);								// 언코일러 제품 EPC원점 스위치
	//// 		m_bSwStopNow = TRUE;
	//// 		if(m_pDlgMenu03)
	//// 			m_pDlgMenu03->SwStop();
	//// 	}
	//// 	else if(!(usIn & (0x01<<14)) && (*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF &= ~(0x01<<14);								
	//// 	}

	//if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	//{
	//	*usInF |= (0x01 << 15);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	//{
	//	*usInF &= ~(0x01 << 15);
	//	// No Use....
	//}
}

void CGvisR2R_LaserView::DoUcSens1()
{
	//if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
	//	return;

	//unsigned short usIn = 0;
	//unsigned short *usInF = NULL;
	//// 	unsigned short usIn = pDoc->m_pMpeIo[1];
	//// 	unsigned short *usInF = &pDoc->m_pMpeIoF[1];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	//{
	//	*usInF |= (0x01 << 0);								// 언코일러 간지모터조절 1	
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 언코일러 간지모터조절 2	
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// 언코일러 간지모터조절 3	
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// 언코일러 간지모터조절 4	
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// 언코일러 제품모터조절 1	
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//}

	//if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	//{
	//	*usInF |= (0x01 << 5);								// 언코일러 제품모터조절 2
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	//{
	//	*usInF &= ~(0x01 << 5);
	//}

	//if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	//{
	//	*usInF |= (0x01 << 6);								// 언코일러 제품모터조절 3	
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	//{
	//	*usInF &= ~(0x01 << 6);
	//}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// 언코일러 제품모터조절 4	
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 언코일러 제품 EPC POS 리미트 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 언코일러 제품 EPC NEG 리미트 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//}

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	//{
	//	*usInF |= (0x01 << 10);								// 언코일러 제품 EPC 원점 센서 
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	//{
	//	*usInF &= ~(0x01 << 10);
	//}

	//if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	//{
	//	*usInF |= (0x01 << 11);								// 언코일러 전면 도어 센서(좌)
	//	pDoc->Status.bDoorUc[DOOR_FL_UC] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	//{
	//	*usInF &= ~(0x01 << 11);
	//	pDoc->Status.bDoorUc[DOOR_FL_UC] = FALSE;
	//}

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	//{
	//	*usInF |= (0x01 << 12);								// 언코일러 전면 도어 센서(우)
	//														// 		pDoc->Status.bDoorUc[DOOR_FR_UC] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	//{
	//	*usInF &= ~(0x01 << 12);
	//	// 		pDoc->Status.bDoorUc[DOOR_FR_UC] = FALSE;
	//}

	//if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	//{
	//	*usInF |= (0x01 << 13);								// 언코일러 측면 도어 센서
	//	pDoc->Status.bDoorUc[DOOR_FR_UC] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	//{
	//	*usInF &= ~(0x01 << 13);
	//	pDoc->Status.bDoorUc[DOOR_FR_UC] = FALSE;
	//}

	//if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	//{
	//	*usInF |= (0x01 << 14);								// 언코일러 후면 도어 센서(좌)
	//	pDoc->Status.bDoorUc[DOOR_BL_UC] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	//{
	//	*usInF &= ~(0x01 << 14);
	//	pDoc->Status.bDoorUc[DOOR_BL_UC] = FALSE;
	//}

	//if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	//{
	//	*usInF |= (0x01 << 15);								// 언코일러 후면 도어 센서(우)
	//	pDoc->Status.bDoorUc[DOOR_BR_UC] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	//{
	//	*usInF &= ~(0x01 << 15);
	//	pDoc->Status.bDoorUc[DOOR_BR_UC] = FALSE;
	//}
}

void CGvisR2R_LaserView::DoUcSens2()
{
	//if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
	//	return;

	//unsigned short usIn = 0;
	//unsigned short *usInF = NULL;
	//// 	unsigned short usIn = pDoc->m_pMpeIo[2];
	//// 	unsigned short *usInF = &pDoc->m_pMpeIoF[2];

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	//{
	//	*usInF |= (0x01 << 0);								// 언코일러 간지텐션 POS 리미트 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 언코일러 간지텐션 상한 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// 언코일러 간지텐션 2/3 지점 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// 언코일러 간지텐션 1/3 지점 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// 언코일러 간지텐션 하한 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//}

	//if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	//{
	//	*usInF |= (0x01 << 5);								// 언코일러 간지텐션 NEG 리미트 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	//{
	//	*usInF &= ~(0x01 << 5);
	//}

	//if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	//{
	//	*usInF |= (0x01 << 6);								// 언코일러 간지척 진공 스위치
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	//{
	//	*usInF &= ~(0x01 << 6);
	//}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// 언코일러 간지버퍼 압력 스위치	
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 언코일러 클린롤러 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 언코일러 클린롤러 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//}

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	//{
	//	*usInF |= (0x01 << 10);								// 언코일러 클린롤러 누름(전면) 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	//{
	//	*usInF &= ~(0x01 << 10);
	//}

	//if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	//{
	//	*usInF |= (0x01 << 11);								// 언코일러 클린롤러 누름(전면) 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	//{
	//	*usInF &= ~(0x01 << 11);
	//}

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	//{
	//	*usInF |= (0x01 << 12);								// 언코일러 클린롤러 누름(후면) 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	//{
	//	*usInF &= ~(0x01 << 12);
	//}

	//if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	//{
	//	*usInF |= (0x01 << 13);								// 언코일러 클린롤러 누름(후면) 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	//{
	//	*usInF &= ~(0x01 << 13);
	//}

	//if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	//{
	//	*usInF |= (0x01 << 14);								// 언코일러 댄서롤 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	//{
	//	*usInF &= ~(0x01 << 14);
	//}

	//if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	//{
	//	*usInF |= (0x01 << 15);								// 언코일러 댄서롤 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	//{
	//	*usInF &= ~(0x01 << 15);
	//}
}

void CGvisR2R_LaserView::DoUcSens3()
{
	//if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
	//	return;

	//// 	unsigned short usIn = pDoc->m_pMpeIo[3];
	//// 	unsigned short *usInF = &pDoc->m_pMpeIoF[3];
	//unsigned short usIn = 0;
	//unsigned short *usInF = NULL;

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	//{
	//	*usInF |= (0x01 << 0);								// 언코일러 댄서롤 POS 리미트 센서
	//	pDoc->Status.bSensLmtBufUc[LMT_POS] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bSensLmtBufUc[LMT_POS] = FALSE;
	//}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 언코일러 댄서롤 상한 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// 언코일러 댄서롤 2/3 지점 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// 언코일러 댄서롤 1/3 지점 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// 언코일러 댄서롤 하한 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//}

	//if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	//{
	//	*usInF |= (0x01 << 5);								// 언코일러 댄서롤 NEG 리미트 센서
	//	pDoc->Status.bSensLmtBufUc[LMT_NEG] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	//{
	//	*usInF &= ~(0x01 << 5);
	//	pDoc->Status.bSensLmtBufUc[LMT_NEG] = FALSE;
	//}

	//if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	//{
	//	*usInF |= (0x01 << 6);								// 언코일러 제품척 진공 스위치
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	//{
	//	*usInF &= ~(0x01 << 6);
	//}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// 언코일러 메인 에어 
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 언코일러 제품휠 지지 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 언코일러 제품휠 지지 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//}

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	//{
	//	*usInF |= (0x01 << 10);								// 언코일러 제품 이음매(좌) 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	//{
	//	*usInF &= ~(0x01 << 10);
	//}

	//if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	//{
	//	*usInF |= (0x01 << 11);								// 언코일러 제품 이음매(좌) 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	//{
	//	*usInF &= ~(0x01 << 11);
	//}

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	//{
	//	*usInF |= (0x01 << 12);								// 언코일러 제품 이음매(우) 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	//{
	//	*usInF &= ~(0x01 << 12);
	//}

	//if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	//{
	//	*usInF |= (0x01 << 13);								// 언코일러 제품 이음매(우) 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	//{
	//	*usInF &= ~(0x01 << 13);
	//}

	//if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	//{
	//	*usInF |= (0x01 << 14);								// SPARE	
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	//{
	//	*usInF &= ~(0x01 << 14);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	//{
	//	*usInF |= (0x01 << 15);								// 언코일러 제품휠 지지 정위치 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	//{
	//	*usInF &= ~(0x01 << 15);
	//}
}

void CGvisR2R_LaserView::DoRcBoxSw()
{
	//if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
	//	return;

	//// 	unsigned short usIn = pDoc->m_pMpeIo[4];
	//// 	unsigned short *usInF = &pDoc->m_pMpeIoF[4];
	//unsigned short usIn = 0;
	//unsigned short *usInF = NULL;

	//// 	if((usIn & (0x01<<0)) && !(*usInF & (0x01<<0)))
	//// 	{
	//// 		*usInF |= (0x01<<0);								// 리코일러 비상정지 스위치
	//// 		pDoc->Status.bEmgRc = TRUE;
	//// 	}
	//// 	else if(!(usIn & (0x01<<0)) && (*usInF & (0x01<<0)))
	//// 	{
	//// 		*usInF &= ~(0x01<<0);								
	//// 		pDoc->Status.bEmgRc = FALSE;
	//// 	}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 리코일러 연동 온/오프 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwRcRelation();
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// 리코일러 댄서롤 상승/하강 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwRcDcRlUpDn();
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//	// No Use....
	//}

	//if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	//{
	//	*usInF |= (0x01 << 5);								// 리코일러 제품 이음매(좌) 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwRcReelJoinL();
	//}
	//else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	//{
	//	*usInF &= ~(0x01 << 5);
	//}

	//if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	//{
	//	*usInF |= (0x01 << 6);								// 리코일러 제품 이음매(우) 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwRcReelJoinR();
	//}
	//else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	//{
	//	*usInF &= ~(0x01 << 6);
	//}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// 리코일러 제품휠 지지 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwRcReelWheel();
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 리코일러 간지척 클램프 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwRcPprChuck();
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 리코일러 간지휠 정회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_45);
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_45);
	//}

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	//{
	//	*usInF |= (0x01 << 10);								// 리코일러 간지휠 역회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_46);
	//}
	//else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	//{
	//	*usInF &= ~(0x01 << 10);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_46);
	//}

	//if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	//{
	//	*usInF |= (0x01 << 11);								// 리코일러 제품척 클램프 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwRcReelChuck();
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	//{
	//	*usInF &= ~(0x01 << 11);
	//}

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	//{
	//	*usInF |= (0x01 << 12);								// 리코일러 제품휠 정회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_5);
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	//{
	//	*usInF &= ~(0x01 << 12);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_5);
	//}

	//if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	//{
	//	*usInF |= (0x01 << 13);								// 리코일러 제품휠 역회전 스위치
	//														// 		if(m_pDlgMenu03)
	//														// 			m_pDlgMenu03->SwMyBtnDown(IDC_CHK_6);
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	//{
	//	*usInF &= ~(0x01 << 13);
	//	// 		if(m_pDlgMenu03)
	//	// 			m_pDlgMenu03->SwMyBtnUp(IDC_CHK_6);
	//}

	//// 	if((usIn & (0x01<<14)) && !(*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF |= (0x01<<14);								// 리코일러 제품 EPC원점 스위치
	//// 		m_bSwStopNow = TRUE;
	//// 		if(m_pDlgMenu03)
	//// 			m_pDlgMenu03->SwStop();
	//// 	}
	//// 	else if(!(usIn & (0x01<<14)) && (*usInF & (0x01<<14)))
	//// 	{
	//// 		*usInF &= ~(0x01<<14);								
	//// 	}

	//if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	//{
	//	*usInF |= (0x01 << 15);								// SPARE
	//														// No Use....
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	//{
	//	*usInF &= ~(0x01 << 15);
	//	// No Use....
	//}
}

void CGvisR2R_LaserView::DoRcSens1()
{
//	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
//		return;
//
//	// 	unsigned short usIn = pDoc->m_pMpeIo[5];
//	// 	unsigned short *usInF = &pDoc->m_pMpeIoF[5];
//	unsigned short usIn = 0;
//	unsigned short *usInF = NULL;
//
//	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
//	{
//		*usInF |= (0x01 << 0);								// 리코일러 간지모터조절 1
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
//	{
//		*usInF &= ~(0x01 << 0);
//	}
//
//	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
//	{
//		*usInF |= (0x01 << 1);								// 리코일러 간지모터조절 2
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
//	{
//		*usInF &= ~(0x01 << 1);
//	}
//
//	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
//	{
//		*usInF |= (0x01 << 2);								// 리코일러 간지모터조절 3
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
//	{
//		*usInF &= ~(0x01 << 2);
//	}
//
//	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
//	{
//		*usInF |= (0x01 << 3);								// 리코일러 간지모터조절 4
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
//	{
//		*usInF &= ~(0x01 << 3);
//	}
//
//	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
//	{
//		*usInF |= (0x01 << 4);								// 리코일러 제품모터조절 1
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
//	{
//		*usInF &= ~(0x01 << 4);
//	}
//
//	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
//	{
//		*usInF |= (0x01 << 5);								// 리코일러 제품모터조절 2
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
//	{
//		*usInF &= ~(0x01 << 5);
//	}
//
//	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
//	{
//		*usInF |= (0x01 << 6);								// 리코일러 제품모터조절 3
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
//	{
//		*usInF &= ~(0x01 << 6);
//	}
//
//	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
//	{
//		*usInF |= (0x01 << 7);								// 리코일러 제품모터조절 4
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
//	{
//		*usInF &= ~(0x01 << 7);
//	}
//
//	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
//	{
//		*usInF |= (0x01 << 8);								// 리코일러 제품 EPC POS 리미트 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
//	{
//		*usInF &= ~(0x01 << 8);
//	}
//
//	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
//	{
//		*usInF |= (0x01 << 9);								// 리코일러 제품 EPC NEG 리미트 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
//	{
//		*usInF &= ~(0x01 << 9);
//	}
//
//	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
//	{
//		*usInF |= (0x01 << 10);								// 리코일러 제품 EPC 원점 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
//	{
//		*usInF &= ~(0x01 << 10);
//	}
//
//	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
//	{
//		*usInF |= (0x01 << 11);								// 리코일러 전면 도어 센서(좌)
//		pDoc->Status.bDoorRe[DOOR_FL_RC] = TRUE;
//	}
//	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
//	{
//		*usInF &= ~(0x01 << 11);
//		pDoc->Status.bDoorRe[DOOR_FL_RC] = FALSE;
//	}
//
//	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
//	{
//		*usInF |= (0x01 << 12);								// 리코일러 전면 도어 센서(우)
//		pDoc->Status.bDoorRe[DOOR_FR_RC] = TRUE;
//	}
//	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
//	{
//		*usInF &= ~(0x01 << 12);
//		pDoc->Status.bDoorRe[DOOR_FR_RC] = FALSE;
//	}
//
//	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
//	{
//		*usInF |= (0x01 << 13);								// 리코일러 측면 도어 센서
//		pDoc->Status.bDoorRe[DOOR_S_RC] = TRUE;
//	}
//	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
//	{
//		*usInF &= ~(0x01 << 13);
//		pDoc->Status.bDoorRe[DOOR_S_RC] = FALSE;
//	}
//
//	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
//	{
//		*usInF |= (0x01 << 14);								// 리코일러 후면 도어 센서(좌)
//		pDoc->Status.bDoorRe[DOOR_BL_RC] = TRUE;
//	}
//	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
//	{
//		*usInF &= ~(0x01 << 14);
//		pDoc->Status.bDoorRe[DOOR_BL_RC] = FALSE;
//	}
//
//	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
//	{
//		*usInF |= (0x01 << 15);								// 리코일러 후면 도어 센서(우)
//		pDoc->Status.bDoorRe[DOOR_BR_RC] = TRUE;
//	}
//	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
//	{
//		*usInF &= ~(0x01 << 15);
//		pDoc->Status.bDoorRe[DOOR_BR_RC] = FALSE;
//	}
//}
//
//void CGvisR2R_LaserView::DoRcSens2()
//{
//	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
//		return;
//
//	// 	unsigned short usIn = pDoc->m_pMpeIo[6];
//	// 	unsigned short *usInF = &pDoc->m_pMpeIoF[6];
//	unsigned short usIn = 0;
//	unsigned short *usInF = NULL;
//
//	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
//	{
//		*usInF |= (0x01 << 0);								// 리코일러 간지텐션 POS 리미트 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
//	{
//		*usInF &= ~(0x01 << 0);
//	}
//
//	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
//	{
//		*usInF |= (0x01 << 1);								// 리코일러 간지텐션 상한 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
//	{
//		*usInF &= ~(0x01 << 1);
//	}
//
//	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
//	{
//		*usInF |= (0x01 << 2);								// 리코일러 간지텐션 2/3 지점 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
//	{
//		*usInF &= ~(0x01 << 2);
//	}
//
//	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
//	{
//		*usInF |= (0x01 << 3);								// 리코일러 간지텐션 1/3 지점 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
//	{
//		*usInF &= ~(0x01 << 3);
//	}
//
//	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
//	{
//		*usInF |= (0x01 << 4);								// 리코일러 간지텐션 하한 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
//	{
//		*usInF &= ~(0x01 << 4);
//	}
//
//	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
//	{
//		*usInF |= (0x01 << 5);								// 리코일러 간지텐션 NEG 리미트 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
//	{
//		*usInF &= ~(0x01 << 5);
//	}
//
//	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
//	{
//		*usInF |= (0x01 << 6);								// 리코일러 간지척 진공 스위치
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
//	{
//		*usInF &= ~(0x01 << 6);
//	}
//
//	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
//	{
//		*usInF |= (0x01 << 7);								// 리코일러 간지버퍼 압력 스위치
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
//	{
//		*usInF &= ~(0x01 << 7);
//	}
//
//	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
//	{
//		*usInF |= (0x01 << 8);								// SPARE
//															// No Use....
//	}
//	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
//	{
//		*usInF &= ~(0x01 << 8);
//		// No Use....
//	}
//
//	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
//	{
//		*usInF |= (0x01 << 9);								// SPARE
//															// No Use....
//	}
//	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
//	{
//		*usInF &= ~(0x01 << 9);
//		// No Use....
//	}
//
//	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
//	{
//		*usInF |= (0x01 << 10);								// SPARE
//															// No Use....
//	}
//	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
//	{
//		*usInF &= ~(0x01 << 10);
//		// No Use....
//	}
//
//	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
//	{
//		*usInF |= (0x01 << 11);								// SPARE
//															// No Use....
//	}
//	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
//	{
//		*usInF &= ~(0x01 << 11);
//		// No Use....
//	}
//
//	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
//	{
//		*usInF |= (0x01 << 12);								// SPARE
//															// No Use....
//	}
//	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
//	{
//		*usInF &= ~(0x01 << 12);
//		// No Use....
//	}
//
//	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
//	{
//		*usInF |= (0x01 << 13);								// SPARE
//															// No Use....
//	}
//	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
//	{
//		*usInF &= ~(0x01 << 13);
//		// No Use....
//	}
//
//	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
//	{
//		*usInF |= (0x01 << 14);								// 리코일러 댄서롤 상승 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
//	{
//		*usInF &= ~(0x01 << 14);
//	}
//
//	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
//	{
//		*usInF |= (0x01 << 15);								// 리코일러 댄서롤 하강 센서
//															// Late....
//	}
//	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
//	{
//		*usInF &= ~(0x01 << 15);
//	}
}

void CGvisR2R_LaserView::DoRcSens3()
{
	//if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
	//	return;

	//// 	unsigned short usIn = pDoc->m_pMpeIo[7];
	//// 	unsigned short *usInF = &pDoc->m_pMpeIoF[7];
	//unsigned short usIn = 0;
	//unsigned short *usInF = NULL;

	//if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	//{
	//	*usInF |= (0x01 << 0);								// 리코일러 댄서롤 POS 리미트 센서
	//	pDoc->Status.bSensLmtBufRc[LMT_POS] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	//{
	//	*usInF &= ~(0x01 << 0);
	//	pDoc->Status.bSensLmtBufRc[LMT_POS] = FALSE;
	//}

	//if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	//{
	//	*usInF |= (0x01 << 1);								// 리코일러 댄서롤 상한 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	//{
	//	*usInF &= ~(0x01 << 1);
	//}

	//if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	//{
	//	*usInF |= (0x01 << 2);								// 리코일러 댄서롤 2/3 지점 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	//{
	//	*usInF &= ~(0x01 << 2);
	//}

	//if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	//{
	//	*usInF |= (0x01 << 3);								// 리코일러 댄서롤 1/3 지점 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	//{
	//	*usInF &= ~(0x01 << 3);
	//}

	//if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	//{
	//	*usInF |= (0x01 << 4);								// 리코일러 댄서롤 하한 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	//{
	//	*usInF &= ~(0x01 << 4);
	//}

	//if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	//{
	//	*usInF |= (0x01 << 5);								// 리코일러 댄서롤 NEG 리미트 센서
	//	pDoc->Status.bSensLmtBufRc[LMT_NEG] = TRUE;
	//}
	//else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	//{
	//	*usInF &= ~(0x01 << 5);
	//	pDoc->Status.bSensLmtBufRc[LMT_NEG] = FALSE;
	//}

	//if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	//{
	//	*usInF |= (0x01 << 6);								// 리코일러 제품척 진공 스위치
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	//{
	//	*usInF &= ~(0x01 << 6);
	//}

	//if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	//{
	//	*usInF |= (0x01 << 7);								// 리코일러 메인 에어 
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	//{
	//	*usInF &= ~(0x01 << 7);
	//}

	//if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	//{
	//	*usInF |= (0x01 << 8);								// 리코일러 제품휠 지지 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	//{
	//	*usInF &= ~(0x01 << 8);
	//}

	//if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	//{
	//	*usInF |= (0x01 << 9);								// 리코일러 제품휠 지지 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	//{
	//	*usInF &= ~(0x01 << 9);
	//}

	//if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	//{
	//	*usInF |= (0x01 << 10);								// 리코일러 제품 이음매(좌) 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	//{
	//	*usInF &= ~(0x01 << 10);
	//}

	//if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	//{
	//	*usInF |= (0x01 << 11);								// 리코일러 제품 이음매(좌) 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	//{
	//	*usInF &= ~(0x01 << 11);
	//}

	//if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	//{
	//	*usInF |= (0x01 << 12);								// 리코일러 제품 이음매(우) 상승 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	//{
	//	*usInF &= ~(0x01 << 12);
	//}

	//if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	//{
	//	*usInF |= (0x01 << 13);								// 리코일러 제품 이음매(우) 하강 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	//{
	//	*usInF &= ~(0x01 << 13);
	//}

	//if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	//{
	//	*usInF |= (0x01 << 14);								// 리코일러 제품 잔량 감지 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	//{
	//	*usInF &= ~(0x01 << 14);
	//}

	//if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	//{
	//	*usInF |= (0x01 << 15);								// 리코일러 제품휠 지지 정위치 센서
	//														// Late....
	//}
	//else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	//{
	//	*usInF &= ~(0x01 << 15);
	//}
}

void CGvisR2R_LaserView::SetAoiFdPitch(double dPitch)
{
	pDoc->SetAoiFdPitch(dPitch);
	// 	m_pMotion->SetLeadPitch(AXIS_AOIFD, dPitch);
	long lData = long(dPitch*1000.0);
#ifdef USE_MPE
	pView->m_pMpe->Write(_T("ML45012"), lData); // 검사부 Feeding 롤러 Lead Pitch
	pView->m_pMpe->Write(_T("ML45020"), lData); // 각인부 Feeding 롤러 Lead Pitch
#endif
}

void CGvisR2R_LaserView::SetMkFdPitch(double dPitch)
{
	pDoc->SetMkFdPitch(dPitch);
	// 	m_pMotion->SetLeadPitch(AXIS_MKFD, dPitch);
//	long lData = long(dPitch*1000.0);
//#ifdef USE_MPE
//	pView->m_pMpe->Write(_T("ML45014"), lData); // 마킹부 Feeding 롤러 Lead Pitch
//#endif
}

void CGvisR2R_LaserView::SetBufInitPos(double dPos)
{
	// 	if(m_pMotion)
	// 		m_pMotion->SetBufInitPos(dPos);
	//	long lData = long(dPos*1000.0);
	//	pView->m_pMpe->Write(_T("ML00000"), lData); // 마킹부 버퍼 초기 위치
	//	pDoc->SetBufInitPos(dPos);

}

void CGvisR2R_LaserView::SetBufHomeParam(double dVel, double dAcc)
{
	long lVel = long(dVel*1000.0);
	long lAcc = long(dAcc*1000.0);
	//	pView->m_pMpe->Write(_T("ML00000"), lVel); // 마킹부 버퍼 홈 속도
	//	pView->m_pMpe->Write(_T("ML00000"), lAcc); // 마킹부 버퍼 홈 가속도
	//	pDoc->SetBufInitPos(dVel, dAcc);
}

LRESULT CGvisR2R_LaserView::OnBufThreadDone(WPARAM wPara, LPARAM lPara)
{
	// 	if(m_pMotion)
	// 	{
	// 		m_pMotion->SetOriginPos(AXIS_AOIFD);
	// 		m_pMotion->SetOriginPos(AXIS_BUF);
	// 		m_pMotion->SetOriginPos(AXIS_RENC);
	// 	}
	// 
	// 	if(m_pVision[0])
	// 		m_pVision[0]->SetClrOverlay();
	// 
	// 	if(m_pVision[1])
	// 		m_pVision[1]->SetClrOverlay();

	return 0L;
}

//.........................................................................................

BOOL CGvisR2R_LaserView::WatiDispMain(int nDelay)
{
	if (m_nWatiDispMain % nDelay)
	{
		m_nWatiDispMain++;
		return TRUE;
	}

	m_nWatiDispMain = 0;
	m_nWatiDispMain++;
	return FALSE;
}

void CGvisR2R_LaserView::DispMain(CString sMsg, COLORREF rgb)
{
	// 	m_cs.Lock();
	// 	if(m_pDlgMenu01)
	// 	{
	// 		m_sDispMain = sMsg;
	// 		m_pDlgMenu01->DispMain(sMsg, rgb);
	// 	}
	// 	m_cs.Unlock();
	m_sMonDisp = sMsg;

	m_csDispMain.Lock();
	m_bDispMain = FALSE;
	stDispMain stData(sMsg, rgb);
	m_ArrayDispMain.Add(stData);
	m_bDispMain = TRUE;

	if (sMsg == _T("정 지"))
		m_bStopF_Verify = TRUE;

	sMsg.Empty();
	m_csDispMain.Unlock();
}

int CGvisR2R_LaserView::DoDispMain()
{
	int nRtn = -1;

	if (!m_bDispMain)
		return nRtn;

	int nCount = m_ArrayDispMain.GetSize();
	if (nCount > 0)
	{
		stDispMain stDispMsg;

		m_csDispMain.Lock();
		stDispMsg = m_ArrayDispMain.GetAt(0);
		m_ArrayDispMain.RemoveAt(0);
		m_csDispMain.Unlock();

		if (m_pDlgMenu01)
		{
			CString sMsg = stDispMsg.strMsg;
			COLORREF rgb = stDispMsg.rgb;
			m_sDispMain = sMsg;
			m_pDlgMenu01->DispMain(sMsg, rgb);
			return 0;
		}
	}

	return nRtn;
}

// CString CGvisR2R_LaserView::GetDispMain()
// {
// // 	m_cs.Lock();
// // 	CString sRtn;
// // 	sRtn = m_sDispMain;
// // 	m_cs.Unlock();
// // 	return sRtn;
// 	return m_sDispMain;
// }

BOOL CGvisR2R_LaserView::IsReady()
{
	if (m_pDlgMenu03)
		return m_pDlgMenu03->GetReady();

	return FALSE;
}

BOOL CGvisR2R_LaserView::IsAuto()
{
	if (pDoc->Status.bAuto)
		return TRUE;
	return FALSE;
}

void CGvisR2R_LaserView::Shift2Buf()
{
	//int nLastListBuf;
	if (m_nShareUpS > 0)
	{
		// 		nLastListBuf = pDoc->m_ListBuf[0].GetLast();
		// 		if(nLastListBuf > 0 && nLastListBuf < 5)
		// 		{
		// 			if(nLastListBuf > m_nShareUpS)
		// 				pDoc->m_ListBuf[0].Clear();
		// 		}
		// 
		m_bLoadShare[0] = TRUE;
		pDoc->m_ListBuf[0].Push(m_nShareUpS);

	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (m_nShareDnS > 0)
		{
			// 			nLastListBuf = pDoc->m_ListBuf[1].GetLast();
			// 			if(nLastListBuf > 0 && nLastListBuf < 5)
			// 			{
			// 				if(nLastListBuf > m_nShareDnS)
			// 					pDoc->m_ListBuf[1].Clear();
			// 			}

			m_bLoadShare[1] = TRUE;
			pDoc->m_ListBuf[1].Push(m_nShareDnS);

			//if(m_bChkLastProcVs)
			{
				//if(m_nShareDnS == GetLotEndSerial()-3)
				if (m_nShareDnS == m_nAoiLastSerial[0] - 3 && m_nAoiLastSerial[0] > 0)
				{
					if (IsVsDn())
					{
						SetDummyDn();
						Sleep(30);
						SetDummyDn();
						Sleep(30);
						SetDummyDn();
						Sleep(30);
					}
				}
			}
		}
	}

	pDoc->CopyPcrAll();
	pDoc->DelSharePcr();
}

void CGvisR2R_LaserView::Shift2Mk()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial;

	if (bDualTest)
	{
		if (m_bLastProc && m_nBufDnSerial[0] == m_nLotEndSerial)
		{
			nSerial = m_nBufDnSerial[0];
			if (nSerial > 0 && (nSerial % 2))
			{
				pDoc->UpdateYield(nSerial);
				pDoc->Shift2Mk(nSerial);	// Cam0
				if (m_pDlgFrameHigh)
					m_pDlgFrameHigh->SetMkLastShot(nSerial);
			}
		}
		else
		{
			nSerial = m_nBufDnSerial[0];
			if (!m_bCont)
			{
				if (nSerial > 0 && (nSerial % 2)) // First Shot number must be odd.
				{
					pDoc->UpdateYield(nSerial);
					pDoc->Shift2Mk(nSerial);	// Cam0
					pDoc->UpdateYield(nSerial + 1);
					pDoc->Shift2Mk(nSerial + 1);	// Cam1
					if (m_pDlgFrameHigh)
						m_pDlgFrameHigh->SetMkLastShot(nSerial + 1);
				}
				else
				{
					Stop();
				}
			}
			else
			{
				if (nSerial > 0)
				{
					pDoc->UpdateYield(nSerial);
					pDoc->Shift2Mk(nSerial);	// Cam0
					pDoc->UpdateYield(nSerial + 1);
					pDoc->Shift2Mk(nSerial + 1);	// Cam1
					if (m_pDlgFrameHigh)
						m_pDlgFrameHigh->SetMkLastShot(nSerial + 1);
				}
				else
				{
					Stop();
				}
			}
		}
	}
	else
	{
		if (m_bLastProc && m_nBufUpSerial[0] == m_nLotEndSerial)
		{
			nSerial = m_nBufUpSerial[0];
			if (!m_bCont)
			{
				if (nSerial > 0 && (nSerial % 2)) // First Shot number must be odd.
				{
					pDoc->UpdateYield(nSerial);
					pDoc->Shift2Mk(nSerial);	// Cam0
					if (m_pDlgFrameHigh)
						m_pDlgFrameHigh->SetMkLastShot(nSerial);
				}
				else
				{
					Stop();
				}
			}
			else
			{
				if (nSerial > 0)
				{
					pDoc->UpdateYield(nSerial);
					pDoc->Shift2Mk(nSerial);	// Cam0
					if (m_pDlgFrameHigh)
						m_pDlgFrameHigh->SetMkLastShot(nSerial);
				}
				else
				{
					Stop();
				}
			}
		}
		else
		{
			nSerial = m_nBufUpSerial[0];
			if (!m_bCont)
			{
				if (nSerial > 0 && (nSerial % 2)) // First Shot number must be odd.
				{
					pDoc->UpdateYield(nSerial);
					pDoc->Shift2Mk(nSerial);	// Cam0
					pDoc->UpdateYield(nSerial + 1);
					pDoc->Shift2Mk(nSerial + 1);	// Cam1
					if (m_pDlgFrameHigh)
						m_pDlgFrameHigh->SetMkLastShot(nSerial + 1);
				}
				else
				{
					Stop();
				}
			}
			else
			{
				if (nSerial > 0)
				{
					pDoc->UpdateYield(nSerial);
					pDoc->Shift2Mk(nSerial);	// Cam0
					pDoc->UpdateYield(nSerial + 1);
					pDoc->Shift2Mk(nSerial + 1);	// Cam1
					if (m_pDlgFrameHigh)
						m_pDlgFrameHigh->SetMkLastShot(nSerial + 1);
				}
				else
				{
					Stop();
				}
			}
		}
	}
}

void CGvisR2R_LaserView::SetTestSts(int nStep)
{
	// 	if(!m_pDlgMenu03)
	// 		return;
	// 
	// 	// 검사부 - TBL파기 OFF, TBL진공 ON, FD/TQ 진공 OFF, 
	// 	switch(nStep)
	// 	{
	// 	case 0:
	// 		m_pDlgMenu03->SwAoiDustBlw(FALSE);
	// 		m_pDlgMenu03->SwAoiTblBlw(FALSE);
	// 		m_pDlgMenu03->SwAoiFdVac(FALSE);
	// 		m_pDlgMenu03->SwAoiTqVac(FALSE);
	// 		break;
	// 	case 1:
	// 		m_pDlgMenu03->SwAoiTblVac(TRUE);
	// 		break;
	// 	}
}

void CGvisR2R_LaserView::SetTestSts0(BOOL bOn)
{
#ifdef USE_MPE
	if (bOn)
		pView->m_pMpe->Write(_T("MB003829"), 1);
	//IoWrite(_T("MB003829"), 1); // 검사부 상 검사 테이블 진공 SOL <-> Y4369 I/F
	else
		pView->m_pMpe->Write(_T("MB003829"), 0);
	//IoWrite(_T("MB003829"), 0); // 검사부 상 검사 테이블 진공 SOL <-> Y4369 I/F
#endif
}

void CGvisR2R_LaserView::SetTestSts1(BOOL bOn)
{
#ifdef USE_MPE
	if (bOn)
		pView->m_pMpe->Write(_T("MB003929"), 1);
	//IoWrite(_T("MB003929"), 1); // 검사부 하 검사 테이블 진공 SOL <-> Y4369 I/F
	else
		pView->m_pMpe->Write(_T("MB003929"), 0);
	//IoWrite(_T("MB003929"), 0); // 검사부 하 검사 테이블 진공 SOL <-> Y4369 I/F
#endif
}

void CGvisR2R_LaserView::SetAoiStopSts()
{
	if (!m_pDlgMenu03)
		return;

	// 검사부 - FD/TQ 진공 ON, TBL진공 OFF, TBL파기 ON,
	m_pDlgMenu03->SwAoiDustBlw(FALSE);
	m_pDlgMenu03->SwAoiTblBlw(FALSE);
	m_pDlgMenu03->SwAoiTblVac(FALSE);
	m_pDlgMenu03->SwAoiFdVac(FALSE);
	// 	m_pDlgMenu03->SwAoiTqVac(FALSE);
}

void CGvisR2R_LaserView::SetAoiFdSts()
{
	if (!m_pDlgMenu03)
		return;

	// 검사부 - FD/TQ 진공 ON, TBL진공 OFF, TBL파기 ON, 
	m_pDlgMenu03->SwAoiFdVac(TRUE);
	// 	m_pDlgMenu03->SwAoiTqVac(TRUE);
	m_pDlgMenu03->SwAoiTblVac(FALSE);
	m_pDlgMenu03->SwAoiTblBlw(TRUE);
	m_pDlgMenu03->SwAoiDustBlw(TRUE);
}

void CGvisR2R_LaserView::SetMkSts(int nStep)
{
	if (!m_pDlgMenu03)
		return;

	// 마킹부 - TBL파기 OFF, TBL진공 ON, FD/TQ 진공 OFF, 
	switch (nStep)
	{
	case 0:
		m_pDlgMenu03->SwMkTblBlw(FALSE);
		m_pDlgMenu03->SwMkFdVac(FALSE);
		m_pDlgMenu03->SwMkTqVac(FALSE);
		break;
	case 1:
		m_pDlgMenu03->SwMkTblVac(TRUE);
		break;
	}
}

void CGvisR2R_LaserView::SetMkStopSts()
{
	if (!m_pDlgMenu03)
		return;

	// 마킹부 - FD/TQ 진공 ON, TBL진공 OFF, TBL파기 ON, 
	m_pDlgMenu03->SwMkTblBlw(FALSE);
	m_pDlgMenu03->SwMkTblVac(FALSE);
	m_pDlgMenu03->SwMkFdVac(FALSE);
	// 	m_pDlgMenu03->SwMkTqVac(FALSE);
}

void CGvisR2R_LaserView::SetMkFdSts()
{
	if (!m_pDlgMenu03)
		return;

	// 마킹부 - FD/TQ 진공 ON, TBL진공 OFF, TBL파기 ON, 
	m_pDlgMenu03->SwMkFdVac(TRUE);
	// 	m_pDlgMenu03->SwMkTqVac(TRUE);
	m_pDlgMenu03->SwMkTblVac(FALSE);
	m_pDlgMenu03->SwMkTblBlw(TRUE);
}

BOOL CGvisR2R_LaserView::IsMkFdSts()
{
	if (!m_pDlgMenu03)
		return FALSE;

	BOOL bOn[4] = { 0 };
	// 마킹부 - FD/TQ 진공 ON, TBL진공 OFF, TBL파기 ON, 
	bOn[0] = m_pDlgMenu03->IsMkFdVac(); // TRUE
	bOn[1] = m_pDlgMenu03->IsMkTqVac(); // TRUE
	bOn[2] = m_pDlgMenu03->IsMkTblVac(); // FALSE
	bOn[3] = m_pDlgMenu03->IsMkTblBlw(); // TRUE

	if (bOn[0] && bOn[1] && !bOn[2] && bOn[3])
		return TRUE;

	return FALSE;
}

void CGvisR2R_LaserView::SetDelay(int mSec, int nId)
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	m_dwSetDlySt[nId] = GetTickCount();
	m_dwSetDlyEd[nId] = m_dwSetDlySt[nId] + mSec;
}

void CGvisR2R_LaserView::SetDelay0(int mSec, int nId)
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	m_dwSetDlySt0[nId] = GetTickCount();
	m_dwSetDlyEd0[nId] = m_dwSetDlySt0[nId] + mSec;
}

void CGvisR2R_LaserView::SetDelay1(int mSec, int nId)
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	m_dwSetDlySt1[nId] = GetTickCount();
	m_dwSetDlyEd1[nId] = m_dwSetDlySt1[nId] + mSec;
}

BOOL CGvisR2R_LaserView::WaitDelay(int nId) // F:Done, T:On Waiting....
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	DWORD dwCur = GetTickCount();
	if (dwCur < m_dwSetDlyEd[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::WaitDelay0(int nId) // F:Done, T:On Waiting....
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	DWORD dwCur = GetTickCount();
	if (dwCur < m_dwSetDlyEd0[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::WaitDelay1(int nId) // F:Done, T:On Waiting....
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	DWORD dwCur = GetTickCount();
	if (dwCur < m_dwSetDlyEd1[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::GetDelay(int &mSec, int nId) // F:Done, T:On Waiting....
{
	DWORD dwCur = GetTickCount();
	mSec = int(dwCur - m_dwSetDlySt[nId]);
	if (dwCur < m_dwSetDlyEd[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::GetDelay0(int &mSec, int nId) // F:Done, T:On Waiting....
{
	DWORD dwCur = GetTickCount();
	mSec = int(dwCur - m_dwSetDlySt0[nId]);
	if (dwCur < m_dwSetDlyEd0[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::GetDelay1(int &mSec, int nId) // F:Done, T:On Waiting....
{
	DWORD dwCur = GetTickCount();
	mSec = int(dwCur - m_dwSetDlySt1[nId]);
	if (dwCur < m_dwSetDlyEd1[nId])
		return TRUE;
	return FALSE;
}

void CGvisR2R_LaserView::SetAoiFd()
{
	CfPoint OfSt;
	if (GetAoiUpOffset(OfSt))
	{
		if (m_pDlgMenu02)
		{
			m_pDlgMenu02->m_dAoiUpFdOffsetX = OfSt.x;
			m_pDlgMenu02->m_dAoiUpFdOffsetY = OfSt.y;
		}
	}

	MoveAoi(-1.0*OfSt.x);
	if (m_pDlgMenu03)
		m_pDlgMenu03->ChkDoneAoi();

	if (!pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
#ifdef USE_MPE
		//IoWrite(_T("MB440151"), 1);	// 한판넬 이송상태 ON (PC가 ON, OFF)
		pView->m_pMpe->Write(_T("MB440151"), 1);
#endif
		CString sData, sPath = PATH_WORKING_INFO;
		pDoc->WorkingInfo.LastJob.bMkOnePnl = pDoc->WorkingInfo.LastJob.bAoiOnePnl = TRUE;
		sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bMkOnePnl ? 1 : 0);
		::WritePrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), sData, sPath);
		::WritePrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), sData, sPath);
	}
}

void CGvisR2R_LaserView::MoveAoi(double dOffset)
{
	//	long lData = (long)0;
	long lData = (long)(dOffset * 1000.0);
	//IoWrite(_T("MB440160"), 1);	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
	pView->m_pMpe->Write(_T("MB440160"), 1);
	//IoWrite(_T("ML45064"), lData);	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
	pView->m_pMpe->Write(_T("ML45064"), lData);
}

void CGvisR2R_LaserView::SetMkFd()
{
	CfPoint OfSt;
	GetMkOffset(OfSt);

	if (m_nShareDnCnt > 0)
	{
		if (!(m_nShareDnCnt % 2))
			MoveMk(-1.0*OfSt.x);
	}
	else
	{
		if (m_nShareUpCnt > 0)
		{
			if (!(m_nShareUpCnt % 2))
				MoveMk(-1.0*OfSt.x);
		}
	}
	if (m_pDlgMenu03)
		m_pDlgMenu03->ChkDoneMk();

	if (!pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
		//IoWrite(_T("MB440151"), 1);	// 한판넬 이송상태 ON (PC가 ON, OFF)
		pView->m_pMpe->Write(_T("MB440151"), 1);

		CString sData, sPath = PATH_WORKING_INFO;
		pDoc->WorkingInfo.LastJob.bMkOnePnl = pDoc->WorkingInfo.LastJob.bAoiOnePnl = TRUE;
		sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bMkOnePnl ? 1 : 0);
		::WritePrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), sData, sPath);
		::WritePrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), sData, sPath);
	}
}

void CGvisR2R_LaserView::MoveMk(double dOffset)
{
#ifdef USE_MPE
	long lData = (long)(dOffset * 1000.0);
	//IoWrite(_T("MB440161"), 1);		// 마킹부 피딩 CW ON (PLC가 피딩완료 후 OFF))
	pView->m_pMpe->Write(_T("MB440161"), 1);
	//IoWrite(_T("ML45066"), lData);	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
	pView->m_pMpe->Write(_T("ML45066"), lData);
#endif
}

BOOL CGvisR2R_LaserView::IsMkFd()
{
	if (m_nShareDnCnt > 0)
	{
		if (!(m_nShareDnCnt % 2))
		{
#ifdef USE_MPE
			if (pDoc->m_pMpeSignal[5] & (0x01 << 1))	// 마킹부 피딩 CW ON (PLC가 피딩완료 후 OFF)
				return TRUE;
#endif
			return FALSE;
		}
	}
	else
	{
		if (m_nShareUpCnt > 0)
		{
			if (!(m_nShareUpCnt % 2))
			{
#ifdef USE_MPE
				if (pDoc->m_pMpeSignal[5] & (0x01 << 1))	// 마킹부 피딩 CW ON (PLC가 피딩완료 후 OFF)
					return TRUE;
#endif
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_LaserView::IsAoiFd()
{
#ifdef USE_MPE
	if (pDoc->m_pMpeSignal[5] & (0x01 >> 0))	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
		return TRUE;
#endif
	return FALSE;
}

void CGvisR2R_LaserView::SetMkFd(double dDist)
{
	double fLen = pDoc->GetOnePnlLen();
	double dOffset = dDist - (fLen*2.0);
	MoveMk(dOffset);
}

void CGvisR2R_LaserView::SetCycTime()
{
	DWORD dCur = GetTickCount();
	if (m_dwCycSt > 0)
	{
		m_dwCycTim = (double)(dCur - m_dwCycSt);
		if (m_dwCycTim < 0.0)
			m_dwCycTim *= (-1.0);
	}
	else
		m_dwCycTim = 0.0;
}

int CGvisR2R_LaserView::GetCycTime()
{
	if (m_dwCycTim < 0)
		m_dwCycTim = 0;

	int nTim = int(m_dwCycTim);
	return nTim;
}

BOOL CGvisR2R_LaserView::IsMkFdDone()
{
	if (m_nShareDnCnt > 0)
	{
		if (m_nShareDnCnt % 2)
			return TRUE;
	}
#ifdef USE_MPE
	if (!(pDoc->m_pMpeSignal[5] & (0x01 << 1)))	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
		return TRUE;
#endif
	return FALSE;

	// 	if(!pView->m_pMotion)
	// 		return FALSE;
	// 
	// 	if(pView->m_pMotion->IsMotionDone(MS_MKFD))
	// 	{
	// // 		pDoc->m_pSliceIo[7] &= ~(0x01<<3);	// 마킹부 피딩 정회전 스위치 램프
	// // 		pView->m_pMpe->Write(_T("MB003853"), 0);
	// 		return TRUE;
	// 	}
	// 	return FALSE;
}

BOOL CGvisR2R_LaserView::IsAoiFdDone()
{
#ifdef USE_MPE
	if (!(pDoc->m_pMpeSignal[5] & (0x01 << 0)))	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
		return TRUE;
#endif
	return FALSE;

	// 	if(!pView->m_pMotion)
	// 		return FALSE;
	// 
	// 	if(pView->m_pMotion->IsMotionDone(MS_AOIFD))
	// 	{
	// // 		pDoc->m_pSliceIo[9] &= ~(0x01<<2);	// 검사부 피딩 정회전 스위치 램프
	// // 		pView->m_pMpe->Write(_T("MB003872"), 0);
	// 		return TRUE;
	// 	}
	// 	return FALSE;
}

double CGvisR2R_LaserView::GetAoi2InitDist()
{
	// 	double dInitD = _tstof(pDoc->WorkingInfo.LastJob.sDistAoiMk)-_tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	double dInitD = (_tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) - 1.0) * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dInitD;
}

double CGvisR2R_LaserView::GetMkInitDist()
{
	double dInitD0 = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	double dInitD1 = (_tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist) + dInitD0) - (_tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen)*2.0);
	return dInitD1;
}

double CGvisR2R_LaserView::GetMkRemain()
{
#ifdef USE_MPE
	// 	double dRemain = _tstof(pDoc->WorkingInfo.LastJob.sDistAoiMk) - m_dEnc[AXIS_RENC];
	double dCurPosMkFd = (double)pDoc->m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
	double dInitD0 = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	double dRemain = _tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist) + dInitD0 - dCurPosMkFd;
	return dRemain;
#else
	return 0.0;
#endif
}

void CGvisR2R_LaserView::UpdateWorking()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateWorking();
}

void CGvisR2R_LaserView::StopFromThread()
{
	m_bStopFromThread = TRUE;
}


void CGvisR2R_LaserView::EngStop(BOOL bOn)
{
	m_bEngStop = bOn;

	if (m_pEngrave)
	{
		if (bOn)
		{
			m_pEngrave->SwStop(TRUE);
			Sleep(100);
		}
	}
}

BOOL CGvisR2R_LaserView::IsEngStop()
{
	if (m_sDispMain != _T("운전중") && m_bEngStop)
		return TRUE;
	else if (m_sDispMain == _T("운전중") && m_bEngStop)
	{
		m_bEngStop = FALSE;
		return FALSE;
	}

	return m_bEngStop;
}

void CGvisR2R_LaserView::Stop()
{
	//StartLive();
	CString sMsg;
	if (m_pDlgMenu03)
	{
		m_pDlgMenu03->SwStop();
	}

	//if (m_pEngrave)
	//{
	//	m_pEngrave->SwStop(pView->m_bSwStop);
	//	Sleep(100);
	//}
}

BOOL CGvisR2R_LaserView::IsStop()
{
	if (m_sDispMain == _T("정 지"))
		return TRUE;
	return FALSE;

	// 	BOOL bOn=FALSE;
	// 	if(m_pDlgMenu03)
	// 		bOn = m_pDlgMenu03->IsStop();
	// 	return bOn;
}

BOOL CGvisR2R_LaserView::IsRun()
{
	//return TRUE; // AlignTest
	if (m_sDispMain == _T("운전중") || m_sDispMain == _T("초기운전") || m_sDispMain == _T("단면샘플") || m_sDispMain == _T("운전준비")
		|| m_sDispMain == _T("단면검사") || m_sDispMain == _T("양면검사") || m_sDispMain == _T("양면샘플"))
	{
		if (IsEngStop())
			return FALSE;

		return TRUE;
	}
	return FALSE;
	//	return m_bSwRun;
}

void CGvisR2R_LaserView::ShowLive(BOOL bShow)
{
	if (bShow)
	{
		if (!IsShowLive())
			SetTimer(TIM_SHOW_MENU02, 30, NULL);
	}
	else
	{
		if (IsShowLive())
			SetTimer(TIM_SHOW_MENU01, 30, NULL);
	}
}

BOOL CGvisR2R_LaserView::IsShowLive()
{
	if (m_pDlgMenu02)
	{
		if (m_pDlgMenu02->IsWindowVisible())
			return TRUE;
	}

	return FALSE;
}

void CGvisR2R_LaserView::SetLotSt()
{
	stLotTime LotTime;
	GetTime(LotTime);

	pDoc->WorkingInfo.Lot.StTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.StTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.StTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.StTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.StTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.StTime.nSec = LotTime.nSec;

	pDoc->WorkingInfo.Lot.CurTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.CurTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.CurTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.CurTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.CurTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.CurTime.nSec = LotTime.nSec;

	pDoc->WorkingInfo.Lot.EdTime.nYear = 0;
	pDoc->WorkingInfo.Lot.EdTime.nMonth = 0;
	pDoc->WorkingInfo.Lot.EdTime.nDay = 0;
	pDoc->WorkingInfo.Lot.EdTime.nHour = 0;
	pDoc->WorkingInfo.Lot.EdTime.nMin = 0;
	pDoc->WorkingInfo.Lot.EdTime.nSec = 0;

	m_dwLotSt = GetTickCount();
	pDoc->SaveLotTime(m_dwLotSt);
	DispLotTime();

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->SetLotSt();
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetLotSt();
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->SetLotSt();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->SetLotSt();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->SetLotSt();
	}
}

void CGvisR2R_LaserView::SetLotEd()
{
	stLotTime LotTime;
	GetTime(LotTime);

	pDoc->WorkingInfo.Lot.EdTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.EdTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.EdTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.EdTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.EdTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.EdTime.nSec = LotTime.nSec;

	pDoc->WorkingInfo.Lot.CurTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.CurTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.CurTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.CurTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.CurTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.CurTime.nSec = LotTime.nSec;

	m_dwLotEd = GetTickCount();

	pDoc->SaveLotTime(pDoc->WorkingInfo.Lot.dwStTick);
	DispLotTime();

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->SetLotEd();
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetLotEd();
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->SetLotEd();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->SetLotEd();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->SetLotEd();
	}

}

void CGvisR2R_LaserView::DispLotTime()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->DispLotTime();
}

BOOL CGvisR2R_LaserView::IsTest()
{
#ifdef USE_MPE
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn0 = (pDoc->m_pMpeIo[nInSeg + 10] & 0x01 << 8) ? TRUE : FALSE; //[34] 검사부 상 검사 시작 <-> Y4368 I/F
	BOOL bOn1 = (pDoc->m_pMpeIo[nInSeg + 14] & 0x01 << 8) ? TRUE : FALSE; //[38] 검사부 하 검사 시작 <-> Y4468 I/F

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (bOn0 || bOn1)
			return TRUE;
	}
	else
	{
		if (bOn0)
			return TRUE;
	}
#endif
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsTestUp()
{
#ifdef USE_MPE
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn0 = (pDoc->m_pMpeIo[nInSeg + 10] & 0x01 << 8) ? TRUE : FALSE; //[34] 검사부 상 검사 시작 <-> Y4368 I/F
	if (bOn0)
		return TRUE;
#endif
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsTestDn()
{
#ifdef USE_MPE
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return FALSE;

	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn1 = (pDoc->m_pMpeIo[nInSeg + 14] & 0x01 << 8) ? TRUE : FALSE; //[38] 검사부 하 검사 시작 <-> Y4468 I/F
	if (bOn1)
		return TRUE;
#endif
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsAoiTblVac()
{
#ifdef USE_MPE
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn0 = (pDoc->m_pMpeIo[nInSeg + 10] & 0x01 << 9) ? TRUE : FALSE; //[34] 검사부 상 검사 테이블 진공 SOL <-> Y4469 I/F
	BOOL bOn1 = (pDoc->m_pMpeIo[nInSeg + 14] & 0x01 << 9) ? TRUE : FALSE; //[38] 검사부 하 검사 테이블 진공 SOL <-> Y4469 I/F
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (bOn0 || bOn1)
			return TRUE;
	}
	else
	{
		if (bOn0)
			return TRUE;
	}
#endif
	return FALSE;
}

//void CGvisR2R_LaserView::SetTest(BOOL bOn)
//{
//	if (bOn)
//	{
//		//IoWrite(_T("MB003828"), 1); // 검사부 상 검사 시작 <-> Y4368 I/F
//		pView->m_pMpe->Write(_T("MB003828"), 1);
//		//IoWrite(_T("MB003928"), 1); // 검사부 하 검사 시작 <-> Y4468 I/F
//		pView->m_pMpe->Write(_T("MB003928"), 1);
//	}
//	else
//	{
//		//IoWrite(_T("MB003828"), 0); // 검사부 상 검사 시작 <-> Y4368 I/F
//		pView->m_pMpe->Write(_T("MB003828"), 0);
//		//IoWrite(_T("MB003928"), 0); // 검사부 하 검사 시작 <-> Y4468 I/F
//		pView->m_pMpe->Write(_T("MB003928"), 0);
//	}
//}

//void CGvisR2R_LaserView::SetTest0(BOOL bOn)
//{
//	if (bOn)
//		pView->m_pMpe->Write(_T("MB003828"), 1);
//	//IoWrite(_T("MB003828"), 1); // 검사부 상 검사 시작 <-> Y4368 I/F
//	else
//		pView->m_pMpe->Write(_T("MB003828"), 0);
//	//IoWrite(_T("MB003828"), 0); // 검사부 상 검사 시작 <-> Y4368 I/F
//}

//void CGvisR2R_LaserView::SetTest1(BOOL bOn)
//{
//	if (bOn)
//		pView->m_pMpe->Write(_T("MB003928"), 1);
//	//IoWrite(_T("MB003928"), 1); // 검사부 하 검사 시작 <-> Y4468 I/F
//	else
//		pView->m_pMpe->Write(_T("MB003928"), 0);
//	//IoWrite(_T("MB003928"), 0); // 검사부 하 검사 시작 <-> Y4468 I/F
//}

BOOL CGvisR2R_LaserView::IsTestDone()
{
#ifdef USE_MPE
	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 8)) ? TRUE : FALSE;	// 검사부 상 검사 완료 <-> X4328 I/F
	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01 << 8)) ? TRUE : FALSE;	// 검사부 하 검사 완료 <-> X4428 I/F
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (bOn0 && bOn1)
			return TRUE;
	}
	else
	{
		if (bOn0)
			return TRUE;
	}

	double dCurPosMkFd = (double)pDoc->m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
	double dTgtFd = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.Motion.sAoiFdDist);
	if (dCurPosMkFd + 10.0 < dTgtFd)//-_tstof(pDoc->WorkingInfo.Motion.sAoiFdDist))
	{
		if (bOn0)
			return TRUE;
	}
#endif
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsAoiTblVacDone()
{
#ifdef USE_MPE
	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 9)) ? TRUE : FALSE;	// 검사부 상 테이블 진공 완료 <-> X4329 I/F
	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01 << 9)) ? TRUE : FALSE;	// 검사부 하 테이블 진공 완료 <-> X4329 I/F
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (bOn0 && bOn1)
			return TRUE;
	}
	else
	{
		if (bOn0)
			return TRUE;
	}

	double dCurPosMkFd = (double)pDoc->m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
	double dTgtFd = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.Motion.sAoiFdDist);
	if (dCurPosMkFd + 10.0 < dTgtFd)//-_tstof(pDoc->WorkingInfo.Motion.sAoiFdDist))
	{
		if (bOn0)
			return TRUE;
	}
#endif
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsTestDoneUp()
{
	//BOOL bOn0 = (pDoc->m_pMpeI[10] & (0x01 << 8)) ? TRUE : FALSE;	// 검사부 상 검사 완료 <-> X4328 I/F
	//if (bOn0)
	//	return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsTestDoneDn()
{
	//BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	//if (!bDualTest)
	//	return TRUE;

	//BOOL bOn1 = (pDoc->m_pMpeI[14] & (0x01 << 8)) ? TRUE : FALSE;	// 검사부 하 검사 완료 <-> X4428 I/F
	//if (bOn1)
	//	return TRUE;
	//return FALSE;
	return TRUE;
}

int CGvisR2R_LaserView::GetSerial()
{
	int nSerial = 0;
	if (m_pDlgMenu01)
		nSerial = m_pDlgMenu01->GetSerial();
	return nSerial;
}

void CGvisR2R_LaserView::SetMkFdLen()
{
	// 	int nLast = pDoc->GetLastSerial();
	int nLast = pDoc->GetLastShotMk();
	pDoc->WorkingInfo.Motion.sMkFdTotLen.Format(_T("%.1f"), (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen));

	CString sData, sPath = PATH_WORKING_INFO;
	sData = pDoc->WorkingInfo.Motion.sMkFdTotLen;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_TOTAL_DIST"), sData, sPath);
}

double CGvisR2R_LaserView::GetMkFdLen()
{
	//	int nLast;
	//  	int nLast = pDoc->GetLastSerial();
	int nLast = pDoc->GetLastShotMk();

	// 	if(m_pDlgFrameHigh)
	// 		nLast = m_pDlgFrameHigh->m_nLastShot;
	// 	else
	// 		nLast = pDoc->m_pReelMapDn->GetLastSerial();

	double dLen = (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);

	return dLen;
}

// double CGvisR2R_LaserView::GetAoiFdLen()
// {
// 	int nLast =  pDoc->m_pReelMap->GetLastSerial();
// 	double dLen = (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
// 	return dLen;
// }

double CGvisR2R_LaserView::GetAoiUpFdLen()
{
	// 	int nLast =  pDoc->m_pReelMapUp->GetLastSerial();
	// 	if(m_nBufTot[0] > 0)
	// 		nLast = m_pBufSerial[0][m_nBufTot[0]-1];

	int nLast = pDoc->GetLastShotUp();

	double dLen = (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dLen;
}

double CGvisR2R_LaserView::GetAoiDnFdLen()
{
	// 	int nLast = pDoc->m_pReelMapDn->GetLastSerial();
	// 	if(m_nBufTot[1] > 0)
	// 		nLast = m_pBufSerial[1][m_nBufTot[1]-1];

	int nLast = pDoc->GetLastShotDn();

	double dLen = (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dLen;
}

double CGvisR2R_LaserView::GetTotVel()
{
	// 	if(!pView->IsRun() || !pView->IsAuto() || GetAutoStep() < 22) // 새로운 로트의 시작시간을 설정함.
	// 		return m_dTotVel;

	CString str, sPrev;
	int nDiff;
	int nHour, nMin, nSec;
	int nStYear, nStMonth, nStDay, nStHour, nStMin, nStSec;
	int nCurYear, nCurMonth, nCurDay, nCurHour, nCurMin, nCurSec;
	int nEdYear, nEdMonth, nEdDay, nEdHour, nEdMin, nEdSec;

	nStYear = pDoc->WorkingInfo.Lot.StTime.nYear;
	nStMonth = pDoc->WorkingInfo.Lot.StTime.nMonth;
	nStDay = pDoc->WorkingInfo.Lot.StTime.nDay;
	nStHour = pDoc->WorkingInfo.Lot.StTime.nHour;
	nStMin = pDoc->WorkingInfo.Lot.StTime.nMin;
	nStSec = pDoc->WorkingInfo.Lot.StTime.nSec;

	nCurYear = pDoc->WorkingInfo.Lot.CurTime.nYear;
	nCurMonth = pDoc->WorkingInfo.Lot.CurTime.nMonth;
	nCurDay = pDoc->WorkingInfo.Lot.CurTime.nDay;
	nCurHour = pDoc->WorkingInfo.Lot.CurTime.nHour;
	nCurMin = pDoc->WorkingInfo.Lot.CurTime.nMin;
	nCurSec = pDoc->WorkingInfo.Lot.CurTime.nSec;

	nEdYear = pDoc->WorkingInfo.Lot.EdTime.nYear;
	nEdMonth = pDoc->WorkingInfo.Lot.EdTime.nMonth;
	nEdDay = pDoc->WorkingInfo.Lot.EdTime.nDay;
	nEdHour = pDoc->WorkingInfo.Lot.EdTime.nHour;
	nEdMin = pDoc->WorkingInfo.Lot.EdTime.nMin;
	nEdSec = pDoc->WorkingInfo.Lot.EdTime.nSec;

	int nTotSec = 0;
	double dMkFdLen = GetMkFdLen();
	if (!nStYear && !nStMonth && !nStDay && !nStHour && !nStMin && !nStSec)
	{
		return 0.0;
	}
	else if (!nEdYear && !nEdMonth && !nEdDay && !nEdHour && !nEdMin && !nEdSec)
	{
		nDiff = (GetTickCount() - pView->m_dwLotSt) / 1000;
		nHour = int(nDiff / 3600);
		nMin = int((nDiff - 3600 * nHour) / 60);
		nSec = nDiff % 60;
	}
	else
	{
		nDiff = (pView->m_dwLotEd - pView->m_dwLotSt) / 1000;
		nHour = int(nDiff / 3600);
		nMin = int((nDiff - 3600 * nHour) / 60);
		nSec = nDiff % 60;
	}

	nTotSec = nHour * 3600 + nMin * 60 + nSec;
	double dVel = 0.0;
	if (nTotSec > 0)
		dVel = dMkFdLen / (double)nTotSec; // [mm/sec]
										   // 		dVel = (dMkFdLen*60.0) / ((double)nTotSec*1000.0); // [M/min]

	m_dTotVel = dVel;
	return dVel;
}

double CGvisR2R_LaserView::GetPartVel()
{
	// 	if(!pView->IsRun() || !pView->IsAuto() || GetAutoStep() < 22) // 새로운 로트의 시작시간을 설정함.
	// 		return m_dPartVel;

	double dLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen) * 2.0;
	double dSec = (double)GetCycTime() / 1000.0;
	double dVel = 0.0;
	if (dSec > 0.0)
		dVel = dLen / dSec; // [mm/sec]
							// 		dVel = (dLen*60.0) / (dSec*1000.0); // [M/min]
	m_dPartVel = dVel;
	return dVel;
}


BOOL CGvisR2R_LaserView::IsBuffer(int nNum)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	//	if(m_nBufTot[0] > 1 && m_nBufTot[1] > 1)
	if (bDualTest)
	{
		if (m_nBufTot[0] > nNum && m_nBufTot[1] > nNum) // [0]: AOI-Up , [1]: AOI-Dn
			return TRUE;
	}
	else
	{
		if (m_nBufTot[0] > nNum) // [0]: AOI-Up
			return TRUE;
	}
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsBufferUp()
{
	if (m_nBufTot[0] > 0)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsBufferDn()
{
	if (m_nBufTot[1] > 0)
		return TRUE;
	return FALSE;
}

int CGvisR2R_LaserView::GetBuffer(int *pPrevSerial)
{
	int nS0 = GetBufferUp(pPrevSerial);
	int nS1 = GetBufferDn(pPrevSerial);
	if (nS0 != nS1)
		return 0;
	return nS0;
}

int CGvisR2R_LaserView::GetBufferUp(int *pPrevSerial)
{
	if (IsBufferUp())
		return m_pBufSerial[0][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[0][0];
	return 0;
}

int CGvisR2R_LaserView::GetBufferDn(int *pPrevSerial)
{
	if (IsBufferDn())
		return m_pBufSerial[1][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[1][0];
	return 0;
}

BOOL CGvisR2R_LaserView::IsBuffer0()
{
	if (m_nBufTot[0] > 0 && m_nBufTot[1] > 0)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsBufferUp0()
{
	if (m_nBufTot[0] > 0)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsBufferDn0()
{
	if (m_nBufTot[1] > 0)
		return TRUE;
	return FALSE;
}

int CGvisR2R_LaserView::GetBuffer0(int *pPrevSerial)
{
	int nS0 = GetBufferUp0(pPrevSerial);
	int nS1 = GetBufferDn0(pPrevSerial);
	if (nS0 != nS1)
		return 0;
	return nS0;
}

int CGvisR2R_LaserView::GetBufferUp0(int *pPrevSerial)
{
	if (IsBufferUp0())
		return m_pBufSerial[0][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[0][0];
	return 0;
}

int CGvisR2R_LaserView::GetBufferDn0(int *pPrevSerial)
{
	if (IsBufferDn0())
		return m_pBufSerial[1][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[1][0];
	return 0;
}



BOOL CGvisR2R_LaserView::IsBuffer1()
{
	if (m_nBufTot[0] > 1 && m_nBufTot[1] > 1)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsBufferUp1()
{
	if (m_nBufTot[0] > 1)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsBufferDn1()
{
	if (m_nBufTot[1] > 1)
		return TRUE;
	return FALSE;
}

int CGvisR2R_LaserView::GetBuffer1(int *pPrevSerial)
{
	int nS0 = GetBufferUp1(pPrevSerial);
	int nS1 = GetBufferDn1(pPrevSerial);
	if (nS0 != nS1)
		return 0;
	return nS0;
}

int CGvisR2R_LaserView::GetBufferUp1(int *pPrevSerial)
{
	if (IsBufferUp1())
		return m_pBufSerial[0][1];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[0][1];
	return 0;
}

int CGvisR2R_LaserView::GetBufferDn1(int *pPrevSerial)
{
	if (IsBufferDn1())
		return m_pBufSerial[1][1];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[1][1];
	return 0;
}



BOOL CGvisR2R_LaserView::IsShare()
{
	// 	if(IsShareUp() || IsShareDn())
	// 		return TRUE;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		if (m_bWaitPcr[0] && m_bWaitPcr[1])
		{
			if (IsShareUp() && IsShareDn())
			{
				m_bWaitPcr[0] = FALSE;
				m_bWaitPcr[1] = FALSE;
				return TRUE;
			}
		}
		else if (m_bWaitPcr[0] && !m_bWaitPcr[1])
		{
			if (IsShareUp())
			{
				m_bWaitPcr[0] = FALSE;
				return TRUE;
			}
		}
		else if (!m_bWaitPcr[0] && m_bWaitPcr[1])
		{
			if (IsShareDn())
			{
				m_bWaitPcr[1] = FALSE;
				return TRUE;
			}
		}
		else
		{
			if (IsShareUp() || IsShareDn())
				return TRUE;
		}

		//	double dCurPosMkFd = (double)pDoc->m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
		// 	double dTgtFd = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.Motion.sAoiFdDist);
		// 	if(dCurPosMkFd < dTgtFd-_tstof(pDoc->WorkingInfo.Motion.sAoiFdDist) + 10.0)
		// 	{
		// 		if(IsShareUp())
		// 			return TRUE;
		// 	}
	}
	else
	{
		if (m_bWaitPcr[0])
		{
			if (IsShareUp())
			{
				m_bWaitPcr[0] = FALSE;
				return TRUE;
			}
		}
		else
		{
			if (IsShareUp())
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsShareUp()
{
	return pDoc->Status.PcrShare[0].bExist;
}

BOOL CGvisR2R_LaserView::IsShareDn()
{
	return pDoc->Status.PcrShare[1].bExist;
}

// BOOL CGvisR2R_LaserView::IsRst()
// {
// 	int nRstNum = GetAoiSerial();
// 	if(m_nRstNum != nRstNum)
// 	{
// 		m_nRstNum = nRstNum;
// 		return TRUE;
// 	}
// 	
// 	return FALSE;
// }

BOOL CGvisR2R_LaserView::IsVsShare()
{
	int nSerial;
	BOOL bExist = ChkVsShare(nSerial);
	return bExist;
}

// int CGvisR2R_LaserView::GetShare()
// {
// 	int nS0 = GetShareUp();
// 	int nS1 = GetShareDn();
// 	if(nS0 == nS1)
// 		return nS0;
// 
// 
// 	return -1;
// }

int CGvisR2R_LaserView::GetShareUp()
{
	return pDoc->Status.PcrShare[0].nSerial;
}
int CGvisR2R_LaserView::GetShareDn()
{
	return pDoc->Status.PcrShare[1].nSerial;
}


BOOL CGvisR2R_LaserView::ChkLastProc()
{
	BOOL bRtn = FALSE;
	if (m_pDlgMenu01)
		bRtn = (m_pDlgMenu01->m_bLastProc);
	return bRtn;
}

BOOL CGvisR2R_LaserView::ChkLastProcFromUp()
{
	BOOL bRtn = TRUE;
	if (m_pDlgMenu01)
		bRtn = (m_pDlgMenu01->m_bLastProcFromUp);
	return bRtn;
}

BOOL CGvisR2R_LaserView::ChkLotEnd(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.25"));
		return FALSE;
	}

	BOOL bEnd0 = ChkLotEndUp(nSerial);
	BOOL bEnd1 = ChkLotEndDn(nSerial);
	if (bEnd0 || bEnd1)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::ChkLotEndUp(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.26"));
		return 0;
	}

	CString sPath;
	sPath.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	// 	sPath.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsShareUp, nSerial); 
	return pDoc->ChkLotEnd(sPath);
}

BOOL CGvisR2R_LaserView::ChkLotEndDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return FALSE;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.27"));
		return 0;
	}

	CString sPath;
	sPath.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufDn, nSerial);
	// 	sPath.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsShareDn, nSerial); 
	return pDoc->ChkLotEnd(sPath);
}

//BOOL CGvisR2R_LaserView::ChkMkTmpStop()
//{
//	if (IsStop() && IsMk())
//	{
//		m_bMkTmpStop = TRUE;
//		SetMk(FALSE);	// Marking 일시정지
//	}
//	else if (IsRun() && m_bMkTmpStop)
//	{
//		m_bMkTmpStop = FALSE;
//		SetMk(TRUE);	// Marking Start
//	}
//
//	return m_bMkTmpStop;
//}

//BOOL CGvisR2R_LaserView::IsMkTmpStop()
//{
//	return m_bMkTmpStop;
//}

BOOL CGvisR2R_LaserView::SetSerial(int nSerial, BOOL bDumy)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.28"));
		return 0;
	}
//	else if (nSerial > pDoc->m_ListBuf[0].GetLast())
//	{
//		return 0;
//	}

	if (!m_pDlgMenu01)
		return FALSE;

	int nPrevSerial = m_pDlgMenu01->GetCurSerial();

	if (nPrevSerial == nSerial)
		return TRUE;

	BOOL bRtn[2] = {1};
	bRtn[0] = m_pDlgMenu01->SetSerial(nSerial, bDumy);
	//if (pDoc->GetTestMode() == MODE_OUTER)
	//{
	//	if (!m_pDlgMenu06)
	//		bRtn[1] = m_pDlgMenu06->SetSerial(nSerial, bDumy);
	//}

	return (bRtn[0] && bRtn[1]);
}

BOOL CGvisR2R_LaserView::SetSerialReelmap(int nSerial, BOOL bDumy)
{
	if (!m_pDlgMenu01)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error - SetSerialReelmap : m_pDlgMenu01 is NULL."));
		return FALSE;
	}

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//{
	//	if (!m_pDlgMenu06)
	//	{
	//		pView->ClrDispMsg();
	//		AfxMessageBox(_T("Error - SetSerialReelmap : m_pDlgMenu06 is NULL."));
	//		return FALSE;
	//	}

	//	m_pDlgMenu06->SetSerialReelmap(nSerial, bDumy);
	//}

	return m_pDlgMenu01->SetSerialReelmap(nSerial, bDumy);
}

BOOL CGvisR2R_LaserView::SetSerialMkInfo(int nSerial, BOOL bDumy)
{
	if (!m_pDlgMenu01)
		return FALSE;
	return m_pDlgMenu01->SetSerialMkInfo(nSerial, bDumy);
}

//void CGvisR2R_LaserView::InitAuto(BOOL bInit)
//{
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//
//	for (int kk = 0; kk < 10; kk++)
//	{
//		m_bDispMsgDoAuto[kk] = FALSE;
//		m_nStepDispMsg[kk] = 0;
//	}
//	m_sFixMsg[0] = _T("");
//	m_sFixMsg[1] = _T("");
//
//	m_bReadyDone = FALSE;
//	m_bChkLastProcVs = FALSE;
//	m_nDummy[0] = 0;
//	m_nDummy[1] = 0;
//	m_nAoiLastSerial[0] = 0;
//	m_nAoiLastSerial[1] = 0;
//	m_nStepAuto = 0;
//	m_nPrevStepAuto = 0;
//	m_nPrevMkStAuto = 0;
//	m_bAoiLdRun = TRUE;
//	m_bAoiLdRunF = FALSE;
//	m_bNewModel = FALSE;
//	m_nLotEndSerial = 0;
//	m_bCam = FALSE;
//	m_bReview = FALSE;
//	m_bChkBufIdx[0] = TRUE;
//	m_bChkBufIdx[0] = TRUE;
//
//	m_nErrCnt = 0;
//
//	m_nStepMk[0] = 0;
//	m_nStepMk[1] = 0;
//	m_nStepMk[2] = 0;
//	m_nStepMk[3] = 0;
//	m_bTHREAD_MK[0] = FALSE;
//	m_bTHREAD_MK[1] = FALSE;
//	m_bTHREAD_MK[2] = FALSE;
//	m_bTHREAD_MK[3] = FALSE;
//	m_nMkPcs[0] = 0;
//	m_nMkPcs[1] = 0;
//	m_nMkPcs[2] = 0;
//	m_nMkPcs[3] = 0;
//
//	m_bMkTmpStop = FALSE;
//
//	m_bWaitPcr[0] = FALSE;
//	m_bWaitPcr[1] = FALSE;
//
//
//	m_nShareUpS = 0;
//	m_nShareUpSerial[0] = 0;
//	m_nShareUpSerial[1] = 0;
//	m_nShareUpCnt = 0;
//
//	m_nShareDnS = 0;
//	m_nShareDnSerial[0] = 0;
//	m_nShareDnSerial[1] = 0;
//	m_nShareDnCnt = 0;
//
//	m_nBufUpSerial[0] = 0;
//	m_nBufUpSerial[1] = 0;
//	m_nBufUpCnt = 0;
//
//	m_nBufDnSerial[0] = 0;
//	m_nBufDnSerial[1] = 0;
//	m_nBufDnCnt = 0;
//
//	m_pDlgMenu02->m_dMkFdOffsetX[0] = 0.0;
//	m_pDlgMenu02->m_dMkFdOffsetY[0] = 0.0;
//	m_pDlgMenu02->m_dMkFdOffsetX[1] = 0.0;
//	m_pDlgMenu02->m_dMkFdOffsetY[1] = 0.0;
//	m_pDlgMenu02->m_dAoiUpFdOffsetX = 0.0;
//	m_pDlgMenu02->m_dAoiUpFdOffsetY = 0.0;
//	m_pDlgMenu02->m_dAoiDnFdOffsetX = 0.0;
//	m_pDlgMenu02->m_dAoiDnFdOffsetY = 0.0;
//
//	m_bReAlign[0][0] = FALSE;	// [nCam][nPos]
//	m_bReAlign[0][1] = FALSE;	// [nCam][nPos]
//	m_bReAlign[1][0] = FALSE;	// [nCam][nPos]
//	m_bReAlign[1][1] = FALSE;	// [nCam][nPos]
//
//	m_bSkipAlign[0][0] = FALSE;	// [nCam][nPos]
//	m_bSkipAlign[0][1] = FALSE;	// [nCam][nPos]
//	m_bSkipAlign[1][0] = FALSE;	// [nCam][nPos]
//	m_bSkipAlign[1][1] = FALSE;	// [nCam][nPos]
//
//	m_bFailAlign[0][0] = FALSE;	// [nCam][nPos]
//	m_bFailAlign[0][1] = FALSE;	// [nCam][nPos]
//	m_bFailAlign[1][0] = FALSE;	// [nCam][nPos]
//	m_bFailAlign[1][1] = FALSE;	// [nCam][nPos]
//
//	m_bDoMk[0] = TRUE;			// [nCam]
//	m_bDoMk[1] = TRUE;			// [nCam]
//	m_bDoneMk[0] = FALSE;		// [nCam]
//	m_bDoneMk[1] = FALSE;		// [nCam]
//	m_bReMark[0] = FALSE;		// [nCam]
//	m_bReMark[1] = FALSE;		// [nCam]
//
//	m_nTotMk[0] = 0;
//	m_nCurMk[0] = 0;
//	m_nTotMk[1] = 0;
//	m_nCurMk[1] = 0;
//	m_nPrevTotMk[0] = 0;
//	m_nPrevCurMk[0] = 0;
//	m_nPrevTotMk[1] = 0;
//	m_nPrevCurMk[1] = 0;
//
//
//	m_bMkSt = FALSE;
//	m_nMkStAuto = 0;
//
//	m_bLotEnd = FALSE;
//	m_nLotEndAuto = 0;
//
//	m_bLastProc = FALSE;
//	m_bLastProcFromUp = TRUE;
//	m_nLastProcAuto = 0;
//
//	pDoc->m_sAlmMsg = _T("");
//	pDoc->m_sPrevAlmMsg = _T("");
//
//	m_dwCycSt = 0;
//	m_sNewLotUp = _T("");
//	m_sNewLotDn = _T("");
//
//	m_nStop = 0;
//
//	m_nStepTHREAD_DISP_DEF = 0;
//	m_bTHREAD_DISP_DEF = FALSE;		// CopyDefImg Stop
//
//
//	for (int a = 0; a < 2; a++)
//	{
//		for (int b = 0; b < 4; b++)
//		{
//			m_nMkStrip[a][b] = 0;
//			m_bRejectDone[a][b] = FALSE;
//		}
//	}
//
//
//	m_pMpe->Write(_T("MB440100"), 0); // PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
//	m_pMpe->Write(_T("MB440110"), 0); // 마킹시작(PC가 확인하고 Reset시킴.)-20141029
//	m_pMpe->Write(_T("MB440150"), 0); // 마킹부 마킹중 ON (PC가 ON, OFF)
//	m_pMpe->Write(_T("MB440170"), 0); // 마킹완료(PLC가 확인하고 Reset시킴.)-20141029
//	//MoveInitPos1();
//	Sleep(30);
//	MoveInitPos0();
//
//	InitIoWrite();
//	OpenShareUp();
//	OpenShareDn();
//	SetTest(FALSE);
//	if (m_pDlgMenu01)
//	{
//		m_pDlgMenu01->m_bLastProc = FALSE;
//		m_pDlgMenu01->m_bLastProcFromUp = TRUE;
//		m_pDlgMenu01->ResetSerial();
//		m_pDlgMenu01->ResetLastProc();
//	}
//
//	// 	if(pDoc->m_pReelMap)
//	// 		pDoc->m_pReelMap->ClrFixPcs();
//	// 	else
//	// 		AfxMessageBox(_T("Not exist m_pReelMap."));
//	// 	if(pDoc->m_pReelMapUp)
//	// 		pDoc->m_pReelMapUp->ClrFixPcs();
//	// 	else
//	// 		AfxMessageBox(_T("Not exist m_pReelMapUp."));
//	// 
//	// 	if(bDualTest)
//	// 	{
//	// 		if(pDoc->m_pReelMapDn)
//	// 			pDoc->m_pReelMapDn->ClrFixPcs();
//	// 		else
//	// 			AfxMessageBox(_T("Not exist m_pReelMapDn."));
//	// 		if(pDoc->m_pReelMapAllUp)
//	// 			pDoc->m_pReelMapAllUp->ClrFixPcs();
//	// 		else
//	// 			AfxMessageBox(_T("Not exist m_pReelMapAllUp."));
//	// 		if(pDoc->m_pReelMapAllDn)
//	// 			pDoc->m_pReelMapAllDn->ClrFixPcs();
//	// 		else
//	// 			AfxMessageBox(_T("Not exist m_pReelMapAllDn."));
//	// 	}
//
//	if (bInit) // 이어가기가 아닌경우.
//	{
//		m_pMpe->Write(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
//
//		m_nRstNum = 0;
//		m_bCont = FALSE;
//		m_dTotVel = 0.0;
//		m_dPartVel = 0.0;
//		m_dwCycSt = 0;
//		m_dwCycTim = 0;
//
//		pDoc->m_nPrevSerial = 0;
//		pDoc->m_bNewLotShare[0] = FALSE;
//		pDoc->m_bNewLotShare[1] = FALSE;
//		pDoc->m_bNewLotBuf[0] = FALSE;
//		pDoc->m_bNewLotBuf[1] = FALSE;
//		pDoc->m_bDoneChgLot = FALSE;
//
//		m_pDlgFrameHigh->m_nMkLastShot = 0;
//		m_pDlgFrameHigh->m_nAoiLastShot[0] = 0;
//		m_pDlgFrameHigh->m_nAoiLastShot[1] = 0;
//
//		if (m_pDlgMenu01)
//			m_pDlgMenu01->ResetLotTime();
//
//		//ClrMkInfo();
//
//		ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
//		if (bDualTest)
//			ResetMkInfo(1);
//
//		ClrMkInfo();
//
//		if (m_pDlgFrameHigh)
//		{
//			m_pDlgFrameHigh->SetMkLastShot(0);
//			m_pDlgFrameHigh->SetAoiLastShot(0, 0);
//			m_pDlgFrameHigh->SetAoiLastShot(1, 0);
//		}
//
//		// 		pDoc->m_ListBuf[0].Clear();
//		// 		pDoc->m_ListBuf[1].Clear();
//	}
//	else
//	{
//		m_pMpe->Write(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
//
//										  //		DispStsBar("이어가기");
//
//		if (pDoc->m_pReelMap)
//			pDoc->m_pReelMap->ClrFixPcs();
//		if (pDoc->m_pReelMapUp)
//			pDoc->m_pReelMapUp->ClrFixPcs();
//
//		if (bDualTest)
//		{
//			if (pDoc->m_pReelMapDn)
//				pDoc->m_pReelMapDn->ClrFixPcs();
//			if (pDoc->m_pReelMapAllUp)
//				pDoc->m_pReelMapAllUp->ClrFixPcs();
//			if (pDoc->m_pReelMapAllDn)
//				pDoc->m_pReelMapAllDn->ClrFixPcs();
//		}
//
//#ifndef TEST_MODE
//		ReloadRst();
//		UpdateRst();
//#endif
//		DispLotStTime();
//		RestoreReelmap();
//	}
//
//}

void CGvisR2R_LaserView::SetListBuf()
{
	pDoc->m_ListBuf[0].Clear();
	if (ChkBufUp(m_pBufSerial[0], m_nBufTot[0]))
	{
		for (int i = 0; i < m_nBufTot[0]; i++)
			pDoc->m_ListBuf[0].Push(m_pBufSerial[0][i]);
	}

	pDoc->m_ListBuf[1].Clear();
	if (ChkBufDn(m_pBufSerial[1], m_nBufTot[1]))
	{
		for (int i = 0; i < m_nBufTot[1]; i++)
			pDoc->m_ListBuf[1].Push(m_pBufSerial[1][i]);
	}
}

void CGvisR2R_LaserView::DispLotStTime()
{
	//char szData[MAX_PATH];
	TCHAR szData[MAX_PATH];
	CString sPath = PATH_WORKING_INFO;
	// [Lot]
	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Tick"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.dwStTick = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.dwStTick = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Year"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nYear = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nYear = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Month"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nMonth = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nMonth = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Day"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nDay = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nDay = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Hour"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nHour = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nHour = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Minute"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nMin = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nMin = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Second"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nSec = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nSec = 0;

	m_dwLotSt = (DWORD)pDoc->WorkingInfo.Lot.dwStTick;
	DispLotTime();
}

void CGvisR2R_LaserView::ClrMkInfo()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	pDoc->ClrPcr();
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->Clear();
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->Clear();
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->Clear();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->Clear();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->Clear();
	}

	if (m_pDlgMenu01)
	{
		m_pDlgMenu01->ResetMkInfo();
		m_pDlgMenu01->SetPnlNum();
		m_pDlgMenu01->SetPnlDefNum();
		m_pDlgMenu01->RefreshRmap();
		m_pDlgMenu01->UpdateRst();
		m_pDlgMenu01->UpdateWorking();
	}
}

void CGvisR2R_LaserView::ModelChange(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn 
{
	if (nAoi == 0)
	{
		pDoc->SetModelInfoUp();
		pView->OpenReelmapUp(); // At Start...
		pView->SetPathAtBufUp();
		if (pView->m_pDlgMenu01)
		{
			pView->m_pDlgMenu01->UpdateData();
			if (pView->m_nSelRmap == RMAP_UP || pView->m_nSelRmap == RMAP_ALLUP)
				pView->m_pDlgMenu01->OpenReelmap(pView->m_nSelRmap);
		}

		pDoc->m_pSpecLocal->MakeDir(pDoc->Status.PcrShare[0].sModel, pDoc->Status.PcrShare[0].sLayer);

		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			OpenReelmapInner();
		}
	}
	else if (nAoi == 1)
	{
		pDoc->SetModelInfoDn();
		pView->OpenReelmapDn(); // At Start...
		pView->SetPathAtBufDn();
		if (pView->m_pDlgMenu01)
		{
			pView->m_pDlgMenu01->UpdateData();
			if (pView->m_nSelRmap == RMAP_DN || pView->m_nSelRmap == RMAP_ALLDN)
				pView->m_pDlgMenu01->OpenReelmap(pView->m_nSelRmap);
		}
		pDoc->m_pSpecLocal->MakeDir(pDoc->Status.PcrShare[1].sModel, pDoc->Status.PcrShare[1].sLayer);
	}
}

void CGvisR2R_LaserView::ResetMkInfo(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bDualTestInner;
	CString sLot, sLayerUp, sLayerDn;
	pDoc->GetCurrentInfo();

	// CamMst Info...
	pDoc->GetCamPxlRes();

	//if(!pDoc->m_pReelMap)
	//	InitReelmap();

	if (nAoi == 0 || nAoi == 2)
	{
		if (!bDualTest)
		{
			m_bDrawGL = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->ResetMkInfo();
		}

		//if (pDoc->GetItsSerialInfo(1, bDualTestInner, sLot, sLayerUp, sLayerDn, 3))
		//{
		//	//if (pDoc->GetTestMode() == MODE_OUTER)
		//	if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, sLayerUp))
		//	{
		//		if (!bDualTestInner)
		//		{
		//			if (m_pDlgMenu06)
		//				m_pDlgMenu06->ResetMkInfo();
		//		}
		//	}
		//}

		if (IsLastJob(0)) // Up
		{
			pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLayerUp);
			pDoc->m_Master[0].LoadMstInfo();
			//pDoc->m_Master[0].WriteStripPieceRegion_Text(pDoc->WorkingInfo.System.sPathOldFile, pDoc->WorkingInfo.LastJob.sLotUp);

			if (pDoc->GetItsSerialInfo(1, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
			{
				//if (pDoc->GetTestMode() == MODE_OUTER)
				if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, sLayerUp))
				{
					pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
						pDoc->WorkingInfo.LastJob.sModelUp,
						sLayerUp);
					pDoc->m_MasterInner[0].LoadMstInfo();
					//pDoc->m_MasterInner[0].WriteStripPieceRegion_Text(pDoc->WorkingInfo.System.sPathOldFile, pDoc->m_sEngLotNum);

					if (bDualTestInner)
					{
						//GetCurrentInfoEng();
						pDoc->m_MasterInner[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModelUp,
							sLayerDn);
						pDoc->m_MasterInner[1].LoadMstInfo();
					}
				}
			}
		}
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - IsLastJob(0)..."));
		}

		InitReelmapUp();

		if (pDoc->GetItsSerialInfo(1, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
		{
			if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, sLayerUp))
			{
				InitReelmapInnerUp();
				if (bDualTestInner)
					InitReelmapInnerDn();
			}
		}

		OpenReelmap();
		SetAlignPosUp();

		if (m_pDlgMenu02)
		{
			m_pDlgMenu02->ChgModelUp();

			if (bDualTest)
				m_pDlgMenu02->ChgModelDn();
		}
		
		if (m_pDlgMenu01)
		{
			m_pDlgMenu01->InitCadImgUp();

			//if (!bDualTest && pDoc->GetTestMode() != MODE_OUTER)
			//{
			//	m_pDlgMenu01->InitGL();
			//	m_bDrawGL = TRUE;
			//	m_pDlgMenu01->RefreshRmap();
			//}
		}

		//if (pDoc->GetItsSerialInfo(1, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
		//{
		//	//if (pDoc->GetTestMode() == MODE_OUTER)
		//	if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, sLayerUp))
		//	{
		//		if (m_pDlgMenu06)
		//		{
		//			m_pDlgMenu06->InitCadImgUp();
		//			if (bDualTestInner)
		//				m_pDlgMenu06->InitCadImgDn();

		//			m_pDlgMenu06->InitGL();
		//			m_pDlgMenu06->RefreshRmap();
		//		}

		//	}
		//}

		if (m_pDlgMenu01)
		{
			if (!bDualTest)
			{
				m_pDlgMenu01->InitGL();
				m_bDrawGL = TRUE;
				m_pDlgMenu01->RefreshRmap();
			}
		}
	}


	if (bDualTest)
	{
		if (nAoi == 1 || nAoi == 2)
		{
			m_bDrawGL = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->ResetMkInfo();

			//ResetReelmap();

			if (IsLastJob(1)) // Dn
			{
				pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
					pDoc->WorkingInfo.LastJob.sModelUp,
					//pDoc->WorkingInfo.LastJob.sModelDn,
					pDoc->WorkingInfo.LastJob.sLayerDn,
					pDoc->WorkingInfo.LastJob.sLayerUp);

				pDoc->m_Master[1].LoadMstInfo();
				//pDoc->m_Master[1].WriteStripPieceRegion_Text(pDoc->WorkingInfo.System.sPathOldFile, pDoc->WorkingInfo.LastJob.sLotDn);

				//if (pDoc->GetTestMode() == MODE_OUTER)
				//{
				//	//GetCurrentInfoEng();
				//	pDoc->m_MasterInner[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				//		pDoc->WorkingInfo.LastJob.sModelUp,
				//		pDoc->m_sEngLayerDn);
				//	pDoc->m_MasterInner[1].LoadMstInfo();
				//}
			}
			else
			{
				AfxMessageBox(_T("Error - IsLastJob(1)..."));
			}

			InitReelmapDn();
			SetAlignPosDn();

			if (m_pDlgMenu02)
				m_pDlgMenu02->ChgModelDn();

			if (m_pDlgMenu01)
			{
				m_pDlgMenu01->InitCadImgDn();
				m_pDlgMenu01->InitGL();
				m_bDrawGL = TRUE;
				m_pDlgMenu01->RefreshRmap();
			}

			// 		m_bDrawGL = TRUE;
		}
	}
	// 	else
	// 	{
	// 		if(m_pDlgMenu01)
	// 			m_pDlgMenu01->ResetMkInfo();
	// 	}
}

void CGvisR2R_LaserView::SetAlignPos()
{
	if (m_pMotion)
	{
		m_pMotion->m_dAlignPosX[0][0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0];
		m_pMotion->m_dAlignPosY[0][0] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0];
		//m_pMotion->m_dAlignPosX[0][1] = pDoc->m_Master[0].m_stAlignMk.X1+ pView->m_pMotion->m_dPinPosX[0];
		//m_pMotion->m_dAlignPosY[0][1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0];

		m_pMotion->m_dAlignPosX[1][0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[1];
		m_pMotion->m_dAlignPosY[1][0] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[1];
		//m_pMotion->m_dAlignPosX[1][1] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[1];
		//m_pMotion->m_dAlignPosY[1][1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1];
	}
}

void CGvisR2R_LaserView::SetAlignPosUp()
{
	if (m_pMotion)
	{
		m_pMotion->m_dAlignPosX[0][0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0];
		m_pMotion->m_dAlignPosY[0][0] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0];
		//m_pMotion->m_dAlignPosX[0][1] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0];
		//m_pMotion->m_dAlignPosY[0][1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0];
	}
}

void CGvisR2R_LaserView::SetAlignPosDn()
{
	if (m_pMotion)
	{
		m_pMotion->m_dAlignPosX[1][0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[1];
		m_pMotion->m_dAlignPosY[1][0] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[1];
		//m_pMotion->m_dAlignPosX[1][1] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[1];
		//m_pMotion->m_dAlignPosY[1][1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1];
	}
}

//BOOL CGvisR2R_LaserView::InitMk()
//{
//	m_nStepElecChk = 0;
//	int nRSer = ChkSerial();
//	int nSerial = GetBuffer0();
//
//	if (!nRSer) // 0: Continue, -: Previous, +: Discontinue
//	{
//		if (pDoc->m_bUseRTRYShiftAdjust)
//		{
//			if (nSerial > 2)
//				AdjPinPos();
//		}
//
//		return TRUE;
//	}
//	else if (nRSer < 0)
//	{
//		if (nSerial > 2)
//		{
//			ReloadRst(nSerial - 1);
//			UpdateRst();
//		}
//		return TRUE;
//	}
//
//	return FALSE;
//}

int CGvisR2R_LaserView::GetErrCode(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.29"));
		return 0;
	}

#ifndef	TEST_MODE
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nErr[2];
	nErr[0] = GetErrCodeUp(nSerial);
	if (nErr[0] != 1)
		return nErr[0];
	if (bDualTest)
	{
		nErr[1] = GetErrCodeDn(nSerial);
		if (nErr[1] != 1)
			return nErr[1];
	}
#endif

	return 1;
}

int CGvisR2R_LaserView::GetErrCodeUp(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.30"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nErrCode = pDoc->m_pPcr[0][nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}

int CGvisR2R_LaserView::GetErrCodeDn(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.31"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nErrCode = pDoc->m_pPcr[1][nIdx]->m_nErrPnl;
	}
#endif

	return nErrCode;
}


int CGvisR2R_LaserView::GetErrCode0(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.32"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

#ifndef	TEST_MODE
	int nErr[2];
	nErr[0] = GetErrCodeUp0(nSerial);
	if (nErr[0] != 1)
		return nErr[0];
	if (bDualTest)
	{
		nErr[1] = GetErrCodeDn0(nSerial);
		if (nErr[1] != 1)
			return nErr[1];
	}
#endif

	return 1;
}

int CGvisR2R_LaserView::GetErrCodeUp0(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.33"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nErrCode = pDoc->m_pPcr[0][nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}

int CGvisR2R_LaserView::GetErrCodeDn0(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.34"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nErrCode = pDoc->m_pPcr[1][nIdx]->m_nErrPnl;
	}
#endif

	return nErrCode;
}


int CGvisR2R_LaserView::GetErrCode1(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.35"));
		return 0;
	}

#ifndef	TEST_MODE
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nErr[2];
	nErr[0] = GetErrCodeUp1(nSerial);
	if (nErr[0] != 1)
		return nErr[0];

	if (bDualTest)
	{
		nErr[1] = GetErrCodeDn1(nSerial);
		if (nErr[1] != 1)
			return nErr[1];
	}
#endif

	return 1;
}

int CGvisR2R_LaserView::GetErrCodeUp1(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.36"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nErrCode = pDoc->m_pPcr[0][nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}

int CGvisR2R_LaserView::GetErrCodeDn1(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.37"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nErrCode = pDoc->m_pPcr[1][nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}


int CGvisR2R_LaserView::GetTotDefPcs(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.38"));
		return 0;
	}

	int nTotDef = 0;
#ifndef TEST_MODE
	nTotDef = GetTotDefPcsUp(nSerial) + GetTotDefPcsDn(nSerial);
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_LaserView::GetTotDefPcsUp(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.39"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_LaserView::GetTotDefPcsDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.40"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nTotDef = pDoc->m_pPcr[1][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}


int CGvisR2R_LaserView::GetTotDefPcs0(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.41"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])
		{
			if (pDoc->m_pPcr[2][nIdx])
				nTotDef = pDoc->m_pPcr[2][nIdx]->m_nTotDef;
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])
		{
			if (pDoc->m_pPcr[0][nIdx])
				nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
		}
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_LaserView::GetTotDefPcsUp0(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.42"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_LaserView::GetTotDefPcsDn0(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.43"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nTotDef = pDoc->m_pPcr[1][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}


int CGvisR2R_LaserView::GetTotDefPcs1(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.44"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])
		{
			if (pDoc->m_pPcr[2][nIdx])
				nTotDef = pDoc->m_pPcr[2][nIdx]->m_nTotDef;
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])
		{
			if (pDoc->m_pPcr[0][nIdx])
				nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
		}
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_LaserView::GetTotDefPcsUp1(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.45"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_LaserView::GetTotDefPcsDn1(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.46"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nTotDef = pDoc->m_pPcr[1][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}


CfPoint CGvisR2R_LaserView::GetMkPnt(int nMkPcs)
{
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	if (pDoc->m_Master[0].m_pPcsRgn)
		ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt(nMkPcs); // Cam0의 Mk 포인트.
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

CfPoint CGvisR2R_LaserView::GetMkPnt0(int nMkPcs)
{
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	if (pDoc->m_Master[0].m_pPcsRgn)
		ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nMkPcs); // Cam0의 Mk 포인트.
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

CfPoint CGvisR2R_LaserView::GetMkPnt1(int nMkPcs)
{
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	if (pDoc->m_Master[0].m_pPcsRgn)
		ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nMkPcs); // Cam1의 Mk 포인트.
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

// CfPoint CGvisR2R_LaserView::GetMkPnt(int nSerial, int nMkPcs)
// {
// 	if(nSerial <= 0)
// 	{
// 		AfxMessageBox(_T("Serial Error."));
// 		return 0;
// 	}
// 
// 	int nIdx = pDoc->GetPcrIdx(nSerial);
// 	int nDefPcsId;
// 	CfPoint ptPnt;
// 	ptPnt.x = -1.0;
// 	ptPnt.y = -1.0;
// 	if(pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
// 	{
// 		if(pDoc->m_pPcr[2][nIdx])
// 		{
// 			if(pDoc->m_pPcr[2][nIdx]->m_pDefPcs)
// 			{
// 				if(pDoc->m_pPcr[2][nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
// 				{
// 					nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
// 					if(pDoc->m_Master[0].m_pPcsRgn)
// 						ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt(nDefPcsId); // Cam0의 Mk 포인트.
// 				}
// 			}
// 		}
// 	}
// 	return ptPnt;
// }

CfPoint CGvisR2R_LaserView::GetMkPnt0(int nSerial, int nMkPcs)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.47"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx0(nSerial);
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	int nDefPcsId = 0;

	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				if (pDoc->m_pPcr[2][nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcr[2][nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nDefPcsId); // Cam0의 Mk 포인트.
					}
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				if (pDoc->m_pPcr[0][nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcr[0][nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nDefPcsId); // Cam0의 Mk 포인트.
					}
				}
			}
		}
	}
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

int CGvisR2R_LaserView::GetMkStripIdx0(int nDefPcsId) // 0 : Fail , 1~4 : Strip Idx
{
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nStripY = int(nNodeY / 4);
	int nStripIdx = 0;//nRow, nNum, nMode, 

#ifndef TEST_MODE
	int nRow = 0, nNum = 0, nMode = 0;
	nNum = int(nDefPcsId / nNodeY);
	nMode = nDefPcsId % nNodeY;
	if (nNum % 2) 	// 홀수.
		nRow = nNodeY - (nMode + 1);
	else		// 짝수.
		nRow = nMode;

	nStripIdx = int(nRow / nStripY) + 1;
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

int CGvisR2R_LaserView::GetMkStripIdx1(int nDefPcsId) // 0 : Fail , 1~4 : Strip Idx
{
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nStripY = int(nNodeY / 4);
	int nStripIdx = 0;//nNum, nMode, nRow, 

#ifndef TEST_MODE
	int nRow = 0, nNum = 0, nMode = 0;
	nNum = int(nDefPcsId / nNodeY);
	nMode = nDefPcsId % nNodeY;
	if (nNum % 2) 	// 홀수.
		nRow = nNodeY - (nMode + 1);
	else		// 짝수.
		nRow = nMode;

	nStripIdx = int(nRow / nStripY) + 1;
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

int CGvisR2R_LaserView::GetMkStripIdx0(int nSerial, int nMkPcs) // 0 : Fail , 1~4 : Strip Idx
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.48"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nStripY = int(nNodeY / MAX_STRIP_NUM);
	int nStripIdx = 0;

#ifndef TEST_MODE
	int nDefPcsId = 0, nNum = 0, nMode = 0, nRow = 0;

	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				if (pDoc->m_pPcr[2][nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				if (pDoc->m_pPcr[0][nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

CfPoint CGvisR2R_LaserView::GetMkPnt1(int nSerial, int nMkPcs)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.50"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	//int nDefPcsId;
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	int nDefPcsId = 0;
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				if (pDoc->m_pPcr[2][nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcr[2][nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
						{
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nDefPcsId); // Cam1의 Mk 포인트.
						}
					}
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				if (pDoc->m_pPcr[0][nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcr[0][nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
						{
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nDefPcsId); // Cam1의 Mk 포인트.
						}
					}
				}
			}
		}
	}
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

int CGvisR2R_LaserView::GetMkStripIdx1(int nSerial, int nMkPcs) // 0 : Fail , 1~4 : Strip Idx
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.51"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nStripY = int(nNodeY / MAX_STRIP_NUM);
	int nStripIdx = 0;

#ifndef TEST_MODE
	int nDefPcsId = 0, nNum = 0, nMode = 0, nRow = 0;
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				if (pDoc->m_pPcr[2][nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				if (pDoc->m_pPcr[0][nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

void CGvisR2R_LaserView::Move0(CfPoint pt, BOOL bCam)
{
	double fLen, fVel, fAcc, fJerk;
	double pPos[2];

	if (pDoc->WorkingInfo.System.bNoMk || bCam)
	{
		pPos[0] = pt.x;
		pPos[1] = pt.y;
	}
	else
	{
		pPos[0] = pt.x + _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);
		pPos[1] = pt.y + _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);
	}

	if (pPos[0] < 0.0)
		pPos[0] = 0.0;
	if (pPos[1] < 0.0)
		pPos[1] = 0.0;

	double dCurrX = pView->m_dEnc[AXIS_X0];
	double dCurrY = pView->m_dEnc[AXIS_Y0];
	fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		m_pMotion->GetSpeedProfile0(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		m_pMotion->Move0(MS_X0Y0, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
	}
}

//void CGvisR2R_LaserView::Move1(CfPoint pt, BOOL bCam)
//{
//	if (m_bProbDn[1])
//	{
//		if (m_pVoiceCoil[1])
//		{
//			m_pVoiceCoil[1]->SearchHomeSmac(1);
//			m_pVoiceCoil[1]->MoveSmacShiftPos(1);
//			m_bProbDn[1] = FALSE;
//			//if (m_pDlgMenu02->m_pDlgUtil06)
//			//	m_pDlgMenu02->m_pDlgUtil06->myBtn[6].SetCheck(FALSE);
//		}
//	}
//
//	double fLen, fVel, fAcc, fJerk;
//
//	double pPos[2];
//
//	if (pDoc->WorkingInfo.System.bNoMk || bCam)
//	{
//		pPos[0] = pt.x;
//		pPos[1] = pt.y;
//	}
//	else
//	{
//		pPos[0] = pt.x + _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX);
//		pPos[1] = pt.y + _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY);
//	}
//
//	if (pPos[0] < 0.0)
//		pPos[0] = 0.0;
//	if (pPos[1] < 0.0)
//		pPos[1] = 0.0;
//
//	double dCurrX = pView->m_dEnc[AXIS_X1];
//	double dCurrY = pView->m_dEnc[AXIS_Y1];
//	fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
//	if (fLen > 0.001)
//	{
//		m_pMotion->GetSpeedProfile1(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
//		m_pMotion->Move1(MS_X1Y1, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
//	}
//}

BOOL CGvisR2R_LaserView::IsMoveDone()
{
	if (!m_pMotion)
		return FALSE;

	if (IsMoveDone0())
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsMoveDone0()
{
	if (!m_pMotion)
		return FALSE;

	if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))
	{
		//	Sleep(50);
		return TRUE;
	}
	return FALSE;
}
/*
CString CGvisR2R_LaserView::GetRmapPath(int nRmap, stModelInfo stInfo)
{
	CString sPath;
#ifdef TEST_MODE
	sPath = PATH_REELMAP;
#else
	CString str;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		switch (nRmap)
		{
		case RMAP_UP:
			str = _T("ReelMapDataUp.txt");
			break;
		case RMAP_ALLUP:
			//#ifdef TEST_MODE
			//			str = _T("ReelMapDataAllUp.txt");
			//#else
			str = _T("ReelMapDataAll.txt");
			//#endif
			break;
		case RMAP_DN:
			str = _T("ReelMapDataDn.txt");
			break;
		case RMAP_ALLDN:
			//#ifdef TEST_MODE
			//			str = _T("ReelMapDataAllDn.txt");
			//#else
			str = _T("ReelMapDataAll.txt");
			//#endif
			break;
		}
	}
	else
		str = _T("ReelMapDataUp.txt");

	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		stInfo.sModel,
		stInfo.sLot,
		stInfo.sLayer,
		str);
#endif
	return sPath;
}

CString CGvisR2R_LaserView::GetRmapPath(int nRmap)
{
	CString sPath = _T("");
	CString Path[4], str;

	switch (nRmap)
	{
	case RMAP_UP:
		str = _T("ReelMapDataUp.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLotUp,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		break;
	case RMAP_ALLUP:
		str = _T("ReelMapDataAll.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLotUp,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		break;
	case RMAP_DN:
		str = _T("ReelMapDataDn.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				//pDoc->WorkingInfo.LastJob.sModelDn,
				//pDoc->WorkingInfo.LastJob.sLotDn,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLotUp,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				//pDoc->WorkingInfo.LastJob.sModelDn,
				//pDoc->Status.PcrShare[1].sLot,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		break;
	case RMAP_ALLDN:
		str = _T("ReelMapDataAll.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLotUp,
				//pDoc->WorkingInfo.LastJob.sModelDn,
				//pDoc->WorkingInfo.LastJob.sLotDn,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->Status.PcrShare[0].sLot,
				//pDoc->WorkingInfo.LastJob.sModelDn,
				//pDoc->Status.PcrShare[1].sLot,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		break;
	case RMAP_INNER_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		break;
	case RMAP_INNER_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		//Path[1] = pDoc->WorkingInfo.LastJob.sInnerModelDn;
		//Path[2] = pDoc->WorkingInfo.LastJob.sInnerLotDn;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		break;
	case RMAP_INNER_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		break;
	case RMAP_INNER_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		//Path[1] = pDoc->WorkingInfo.LastJob.sInnerModelDn;
		//Path[2] = pDoc->WorkingInfo.LastJob.sInnerLotDn;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		break;
	case RMAP_INOUTER_UP:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_INOUTER_DN:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			//pDoc->WorkingInfo.LastJob.sModelDn,
			//pDoc->WorkingInfo.LastJob.sLotDn,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ITS:
		pDoc->GetCurrentInfo();
		str = _T("ReelmapIts.txt");
		sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sEngItsCode,
			//pDoc->m_sItsCode,
			str);
		break;
	}

	return sPath;
}
*/
BOOL CGvisR2R_LaserView::LoadPcrUp(int nSerial, BOOL bFromShare)
{
	return TRUE;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.52"));
		return 0;
	}

	int nHeadInfo = pDoc->LoadPCR0(nSerial); // 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (nHeadInfo >= 2)
	{
		MsgBox(_T("Error-LoadPCR0()"));
		return FALSE;
	}
	return TRUE;
}

BOOL CGvisR2R_LaserView::LoadPcrDn(int nSerial, BOOL bFromShare)
{
	return TRUE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.53"));
		return 0;
	}

	int nHeadInfo = pDoc->LoadPCR1(nSerial); // 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (nHeadInfo >= 2)
	{
		MsgBox(_T("Error-LoadPCR1()"));
		return FALSE;
	}
	return TRUE;
}

BOOL CGvisR2R_LaserView::UpdateReelmap(int nSerial)
{
	return TRUE;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.54"));
		return 0;
	}

	if (!pDoc->MakeMkDir())
		return FALSE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CString str;
	CString sPathRmap[4], sPathPcr[2]; //[Up/Dn]
									   // 	int nHeadInfo = pDoc->LoadPCR(nSerial); // 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
									   // 	if(nHeadInfo<2)
									   //	{
	if (pDoc->m_pReelMap)
	{
		// 			sPathRmap.Format(_T("%s%s\\%s\\%s\\ReelMapData.txt"), pDoc->WorkingInfo.System.sPathOldFile, 
		// 															  pDoc->WorkingInfo.LastJob.sModel, 
		// 															  pDoc->WorkingInfo.LastJob.sLot, 
		// 															  pDoc->WorkingInfo.LastJob.sLayer);


		// Select Path For Lot Change....


		// 			sPathRmap = GetRmapPath(m_nSelRmap);
		// 			OpenReelmap();
		// 			if(m_pDlgMenu01)
		// 				m_pDlgMenu01->OpenReelmap(m_nSelRmap);


		stModelInfo stInfo;
		sPathPcr[0].Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
		if (bDualTest)
			sPathPcr[1].Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufDn, nSerial);

		if (!pDoc->GetPcrInfo(sPathPcr[0], stInfo)) // Up
		{
			pView->DispStsBar(_T("E(4)"), 5);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error-GetPcrInfo(4)"));
			return FALSE;
		}

		if (!pDoc->MakeMkDir(stInfo.sModel, stInfo.sLot, stInfo.sLayer))
			return FALSE;

		str = _T("ReelMapDataUp.txt"); // [0]:AOI-Up
		sPathRmap[0].Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			stInfo.sModel,
			stInfo.sLot,
			stInfo.sLayer,
			str);

		if (bDualTest)
		{
			str = _T("ReelMapDataAll.txt"); // [2]:AOI-AllUp
			sPathRmap[2].Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				stInfo.sModel,
				stInfo.sLot,
				stInfo.sLayer,
				str);

			if (!pDoc->GetPcrInfo(sPathPcr[1], stInfo)) // Dn
			{
				pView->DispStsBar(_T("E(5)"), 5);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error-GetPcrInfo(5)"));
				return FALSE;
			}

			if (!pDoc->MakeMkDir(stInfo.sModel, stInfo.sLot, stInfo.sLayer))
				return FALSE;

			str = _T("ReelMapDataDn.txt"); // [1]:AOI-Dn
			sPathRmap[1].Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				stInfo.sModel,
				stInfo.sLot,
				stInfo.sLayer,
				str);
			str = _T("ReelMapDataAll.txt"); // [3]:AOI-AllDn
			sPathRmap[3].Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				stInfo.sModel,
				stInfo.sLot,
				stInfo.sLayer,
				str);
		}

		m_nSerialRmapUpdate = nSerial;
		m_sPathRmapUpdate[0] = sPathRmap[0];
		m_sPathRmapUpdate[1] = sPathRmap[1];
		m_sPathRmapUpdate[2] = sPathRmap[2];
		m_sPathRmapUpdate[3] = sPathRmap[3];

		//pDoc->m_pReelMapUp->Write(nSerial, 0, sPathRmap[0]); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		m_bTHREAD_UPDATE_REELMAP_UP = TRUE;
		if (bDualTest)
		{
			m_bTHREAD_UPDATE_REELMAP_DN = TRUE;
			m_bTHREAD_UPDATE_REELMAP_ALLUP = TRUE;
			m_bTHREAD_UPDATE_REELMAP_ALLDN = TRUE;
			//pDoc->m_pReelMapDn->Write(nSerial, 1, sPathRmap[1]); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
			//pDoc->m_pReelMapAllUp->Write(nSerial, 2, sPathRmap[2]); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
			//pDoc->m_pReelMapAllDn->Write(nSerial, 3, sPathRmap[3]); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		}

		//if (pDoc->GetTestMode() == MODE_OUTER)
		//	m_bTHREAD_UPDATE_REELMAP_ITS = TRUE;

		Sleep(100);
		//pDoc->m_pReelMap->Write(nSerial, pView->m_nSelRmap, sPathRmap[pView->m_nSelRmap]); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn

		return TRUE;
	}
	else
		MsgBox(_T("Error-ReelMapWrite()"));

	return FALSE;
}


void CGvisR2R_LaserView::InitInfo()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateData();

	if (m_pDlgMenu05)
	{
		m_pDlgMenu05->InitModel();
		if (m_pDlgMenu05->IsWindowVisible())
			m_pDlgMenu05->AtDlgShow();
	}
}

// void CGvisR2R_LaserView::ResetReelmap()
// {
// 	if(m_pDlgMenu01)
// 		m_pDlgMenu01->ResetGL();
// 	//pDoc->ResetReelmap();
// }

void CGvisR2R_LaserView::InitReelmap()
{
	pDoc->InitReelmap();
	pDoc->SetReelmap(ROT_NONE);
	// 	pDoc->SetReelmap(ROT_CCW_90);
	pDoc->UpdateData();
}

void CGvisR2R_LaserView::InitReelmapUp()
{
	pDoc->InitReelmapUp();
	pDoc->SetReelmap(ROT_NONE);
	// 	pDoc->SetReelmap(ROT_CCW_90);
	pDoc->UpdateData();
}

void CGvisR2R_LaserView::InitReelmapDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	pDoc->InitReelmapDn();
	pDoc->SetReelmap(ROT_NONE);
	// 	pDoc->SetReelmap(ROT_CCW_90);
	pDoc->UpdateData();
}

 void CGvisR2R_LaserView::LoadMstInfo()
 {
	 BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

#ifdef TEST_MODE
	 pDoc->GetCamPxlRes();
	 if (IsLastJob(0)) // Up
	 {
		 pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
			 pDoc->WorkingInfo.LastJob.sModelUp,
			 pDoc->WorkingInfo.LastJob.sLayerUp);
		 pDoc->m_Master[0].LoadMstInfo();
	 }
	 if (IsLastJob(1)) // Dn
	 {
		 pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
			 pDoc->WorkingInfo.LastJob.sModelDn,
			 pDoc->WorkingInfo.LastJob.sLayerDn,
			 pDoc->WorkingInfo.LastJob.sLayerUp);
		 pDoc->m_Master[1].LoadMstInfo();
	 }

	 // 			pDoc->LoadMasterSpec();
	 // 			pDoc->LoadPinImg();
	 // 			pDoc->GetCamPxlRes();
	 // 			pDoc->LoadStripRgnFromCam();
	 // 			pDoc->LoadPcsRgnFromCam();
	 // 			pDoc->LoadPcsImg();
	 // 			pDoc->LoadCadImg();
#else
	pDoc->GetCamPxlRes();

	if (IsLastJob(0) && pDoc->m_bLoadMstInfo[0]) // Up
	{
		pDoc->m_bLoadMstInfo[0] = FALSE;
		pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLayerUp);
		pDoc->m_Master[0].LoadMstInfo();
		//pDoc->m_Master[0].WriteStripPieceRegion_Text(pDoc->WorkingInfo.System.sPathOldFile, pDoc->WorkingInfo.LastJob.sLotUp);
	}

	if (IsLastJob(1) && pDoc->m_bLoadMstInfo[1]) // Dn
	{
		pDoc->m_bLoadMstInfo[1] = FALSE;
		pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
			pDoc->WorkingInfo.LastJob.sModelUp,
			//pDoc->WorkingInfo.LastJob.sModelDn,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			pDoc->WorkingInfo.LastJob.sLayerUp);
		pDoc->m_Master[1].LoadMstInfo();
		//pDoc->m_Master[1].WriteStripPieceRegion_Text(pDoc->WorkingInfo.System.sPathOldFile, pDoc->WorkingInfo.LastJob.sLotDn);
	}

#endif
	 // Reelmap 정보 Loading.....
	 InitReelmap(); // Delete & New

	 if (m_pDlgMenu01)
	 {
		 m_pDlgMenu01->InitGL();
		 m_bDrawGL = TRUE;
		 m_pDlgMenu01->RefreshRmap();
		 m_pDlgMenu01->InitCadImg();
		 m_pDlgMenu01->SetPnlNum();
		 m_pDlgMenu01->SetPnlDefNum();
	 }

	 if (m_pDlgMenu02)
	 {
		 m_pDlgMenu02->ChgModelUp(); // PinImg, AlignImg를 Display함.
	 }

#ifndef TEST_MODE
	 if (m_pDlgMenu01)
		 m_pDlgMenu01->RedrawWindow();

	 DispMsg(_T("릴맵을 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
	 OpenReelmap();
#endif
	 SetPathAtBuf(); // Reelmap path를 설정함.
	 LoadPcrFromBuf();

	 //MakeResultMDS(); // For Test - 20220421
#ifdef TEST_MODE
	 //pDoc->LoadPCR(TEST_SHOT);					// for Test
	 //m_pDlgMenu01->OpenReelmap(PATH_REELMAP);	// for Test
	 //m_pDlgMenu01->DispReelmap(2);				// for Test
	 //m_pDlgMenu01->SelMap(ALL);
	 //SetSerial(TEST_SHOT);						// for Test
#else
	 //nSrl = pDoc->GetLastSerial();
	 int nSrl = pDoc->GetLastShotMk();
	 SetMkFdLen();
	 if (nSrl >= 0)
	 {
		 if (bDualTest)
			 m_pDlgMenu01->SelMap(ALL);
		 else
			 m_pDlgMenu01->SelMap(UP);
	 }
#endif

 }

BOOL CGvisR2R_LaserView::IsPinMkData()
{
	if (pDoc->IsPinMkData())
	{
		// 		if(m_pDlgMenu02)
		// 			m_pDlgMenu02->SetPcsOffset();
		return TRUE;
	}
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsPinData()
{
	return pDoc->IsPinData();
}

// BOOL CGvisR2R_LaserView::IsMkOffsetData()
// {
// 	return pDoc->IsMkOffsetData();
// }

BOOL CGvisR2R_LaserView::CopyDefImg(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.55"));
		return 0;
	}

	return pDoc->CopyDefImg(nSerial);
}

BOOL CGvisR2R_LaserView::CopyDefImg(int nSerial, CString sNewLot)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.56"));
		return 0;
	}

	return pDoc->CopyDefImg(nSerial, sNewLot);
}

BOOL CGvisR2R_LaserView::CopyDefImgUp(int nSerial, CString sNewLot)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.57"));
		return 0;
	}

	return pDoc->CopyDefImgUp(nSerial, sNewLot);
}

BOOL CGvisR2R_LaserView::CopyDefImgDn(int nSerial, CString sNewLot)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return FALSE;

	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.58"));
		return FALSE;
	}

	return pDoc->CopyDefImgDn(nSerial, sNewLot);
}

BOOL CGvisR2R_LaserView::MovePinPos()
{
	if (!m_pDlgMenu02)
		return FALSE;

	return m_pDlgMenu02->MovePinPos();
}


//BOOL CGvisR2R_LaserView::TwoPointAlign0(int nPos)
//{
//	if (!m_pDlgMenu02)
//		return FALSE;
//
//	return m_pDlgMenu02->DoAlign0(nPos);
//}


BOOL CGvisR2R_LaserView::OnePointAlign(CfPoint &ptPnt)
{
	if (!m_pDlgMenu02)
		return FALSE;

	return m_pDlgMenu02->OnePointAlign(ptPnt);
}

//BOOL CGvisR2R_LaserView::TwoPointAlign() // return FALSE; 인경우 정지.
//{
//	if (!m_pDlgMenu02)
//		return FALSE;
//
//	BOOL bRtn[2];
//
//	// TwoPointAlign(0)
//	bRtn[0] = m_pDlgMenu02->TwoPointAlign(0);
//
//	if (!bRtn[0])
//		return FALSE;
//
//	// TwoPointAlign(1)
//	bRtn[1] = m_pDlgMenu02->TwoPointAlign(1); // Align 보정.
//
//	if (bRtn[0] && bRtn[1])
//		return TRUE;
//
//	return FALSE;
//}

BOOL CGvisR2R_LaserView::IsHomeDone(int nMsId)
{
	if (!m_pMotion)
		return FALSE;

	return m_pMotion->IsHomeDone(nMsId);
}

BOOL CGvisR2R_LaserView::GetAoiUpInfo(int nSerial, int *pNewLot, BOOL bFromBuf)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.59"));
		return 0;
	}

	return pDoc->GetAoiUpInfo(nSerial, pNewLot, bFromBuf);
}

BOOL CGvisR2R_LaserView::GetAoiDnInfo(int nSerial, int *pNewLot, BOOL bFromBuf)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.60"));
		return 0;
	}

	return pDoc->GetAoiDnInfo(nSerial, pNewLot, bFromBuf);
}

BOOL CGvisR2R_LaserView::LoadMySpec()
{
	return pDoc->LoadMySpec();
}

CString CGvisR2R_LaserView::GetProcessNum()
{
	return pDoc->GetProcessNum();
}

BOOL CGvisR2R_LaserView::GetAoiUpOffset(CfPoint &OfSt)
{
	return pDoc->GetAoiUpOffset(OfSt);
}

BOOL CGvisR2R_LaserView::GetAoiDnOffset(CfPoint &OfSt)
{
	return pDoc->GetAoiDnOffset(OfSt);
}

BOOL CGvisR2R_LaserView::GetMkOffset(CfPoint &OfSt)
{
	if (m_pDlgMenu02)
	{
		OfSt.x = m_pDlgMenu02->m_dMkFdOffsetX[0][0]; // -: 제품 덜옴, +: 제품 더옴.
		OfSt.y = m_pDlgMenu02->m_dMkFdOffsetY[0][0]; // -: 제품 덜옴, +: 제품 더옴.
	}

	return TRUE;
}

BOOL CGvisR2R_LaserView::IsAoiLdRun()
{
	return TRUE;

	BOOL bRtn = FALSE;
	if (m_pDlgMenu03)
		bRtn = m_pDlgMenu03->IsAoiLdRun();

	return bRtn;
}

BOOL CGvisR2R_LaserView::IsInitPos0()
{
	if (!m_pMotion)
		return FALSE;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	double dCurrX = m_dEnc[AXIS_X0];
	double dCurrY = m_dEnc[AXIS_Y0];

	if (dCurrX < pTgtPos[0] - 2.0 || dCurrX > pTgtPos[0] + 2.0)
		return FALSE;
	if (dCurrY < pTgtPos[1] - 2.0 || dCurrY > pTgtPos[1] + 2.0)
		return FALSE;

	return TRUE;
}

void CGvisR2R_LaserView::MoveInitPos0(BOOL bWait)
{
	if (!m_pMotion)
		return;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	double dCurrX = m_dEnc[AXIS_X0];
	double dCurrY = m_dEnc[AXIS_Y0];

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		m_pMotion->GetSpeedProfile0(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		if(bWait)
			m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc, ABS, WAIT);
		else
			m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
	}
}

//BOOL CGvisR2R_LaserView::IsInitPos1()
//{
//	if (!m_pMotion)
//		return FALSE;
//
//	double pTgtPos[2];
//	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
//	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
//	double dCurrX = m_dEnc[AXIS_X1];
//	double dCurrY = m_dEnc[AXIS_Y1];
//
//	if (dCurrX < pTgtPos[0] - 2.0 || dCurrX > pTgtPos[0] + 2.0)
//		return FALSE;
//	if (dCurrY < pTgtPos[1] - 2.0 || dCurrY > pTgtPos[1] + 2.0)
//		return FALSE;
//
//	return TRUE;
//}

//BOOL CGvisR2R_LaserView::IsMkEdPos1()
//{
//	if (!m_pMotion)
//		return FALSE;
//
//	double pTgtPos[2];
//	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sMkEdPosX[1]);
//	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sMkEdPosY[1]);
//	double dCurrX = m_dEnc[AXIS_X1];
//	double dCurrY = m_dEnc[AXIS_Y1];
//
//	if (dCurrX < pTgtPos[0] - 2.0 || dCurrX > pTgtPos[0] + 2.0)
//		return FALSE;
//	if (dCurrY < pTgtPos[1] - 2.0 || dCurrY > pTgtPos[1] + 2.0)
//		return FALSE;
//
//	return TRUE;
//}

//void CGvisR2R_LaserView::MoveMkEdPos1()
//{
//	if (!m_pMotion)
//		return;
//
//	double pTgtPos[2];
//	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sMkEdPosX[1]);
//	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sMkEdPosY[1]);
//	double dCurrX = m_dEnc[AXIS_X1];
//	double dCurrY = m_dEnc[AXIS_Y1];
//
//	double fLen, fVel, fAcc, fJerk;
//	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
//	if (fLen > 0.001)
//	{
//		m_pMotion->GetSpeedProfile1(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
//		m_pMotion->Move1(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
//	}
//}


void CGvisR2R_LaserView::LotEnd()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->LotEnd();
	if (m_pDlgMenu03)
		m_pDlgMenu03->SwAoiLotEnd(TRUE);

	m_bCont = FALSE;
	SetLotEd();

	MakeResultMDS();
}

void CGvisR2R_LaserView::TimWinker(int nId, int nDly) // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
	m_bBtnWinker[nId] = TRUE;
	if (!m_bTimBtnWinker)
	{
		m_bTimBtnWinker = TRUE;
		m_nDlyWinker[nId] = nDly;
		SetTimer(TIM_BTN_WINKER, 100, NULL);
	}
	// 	DispBtnWinker();
}

void CGvisR2R_LaserView::StopTimWinker(int nId) // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
	m_bBtnWinker[nId] = FALSE;
	m_bTimBtnWinker = FALSE;
	for (int i = 0; i < 4; i++)
	{
		if (m_bBtnWinker[i])
			m_bTimBtnWinker = TRUE;
	}
	// 	DispBtnWinker();
}

void CGvisR2R_LaserView::Winker(int nId, int nDly) // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
#ifdef USE_MPE
	if (nId == MN_RUN)
	{
		if (pView->m_pMpe)
		{
			m_bBtnWinker[nId] = TRUE;
			//IoWrite(_T("MB44015D"), 1); // 자동 초기 운전상태(PC가 On/Off 시킴, PLC가 운전램프를 윙크동작, on->off시 운전램프 on, 다시 운전스위치가 눌러지면 off) - 20141017
			pView->m_pMpe->Write(_T("MB44015D"), 1); // 자동 초기 운전상태(PC가 On/Off 시킴, PLC가 운전램프를 윙크동작, on->off시 운전램프 on, 다시 운전스위치가 눌러지면 off) - 20141017
		}
	}
	// 	DispBtnWinker(nDly);
#endif
}

void CGvisR2R_LaserView::ResetWinker() // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
#ifdef USE_MPE
	//IoWrite(_T("MB44015D"), 0); // 자동 초기 운전상태(PC가 On/Off 시킴, PLC가 운전램프를 윙크동작, on->off시 운전램프 on, 다시 운전스위치가 눌러지면 off) - 20141017
	pView->m_pMpe->Write(_T("MB44015D"), 0);
#endif
	for (int i = 0; i<4; i++)
	{
		m_bBtnWinker[i] = FALSE;
		m_nCntBtnWinker[i] = FALSE;
	}
}

void CGvisR2R_LaserView::SetOrigin()
{
}

BOOL CGvisR2R_LaserView::IsSetLotEnd()
{
	if (m_nLotEndSerial > 0)
		return TRUE;
	return FALSE;
}

void CGvisR2R_LaserView::SetLotEnd(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.61"));
		return;
	}
	m_nLotEndSerial = nSerial;

	CString str;
	str.Format(_T("%d"), m_nLotEndSerial);
	DispStsBar(str, 0);
}

int CGvisR2R_LaserView::GetLotEndSerial()
{
	return m_nLotEndSerial; // 테이블상에 정지하는 Serial.
}

BOOL CGvisR2R_LaserView::StartLive()
{
//#ifdef TEST_MODE
//	return TRUE;
//#endif

	if (StartLive0())	// && StartLive1())
		return TRUE;

	return  FALSE;
}

BOOL CGvisR2R_LaserView::StartLive0()
{
//#ifdef TEST_MODE
//	return TRUE;
//#endif

	BOOL bRtn0 = FALSE;

#ifdef USE_VISION
	if (m_pVision[0])
		bRtn0 = m_pVision[0]->StartLive();
#endif
	if (bRtn0)
		return TRUE;

	return  FALSE;
}

//BOOL CGvisR2R_LaserView::StartLive1()
//{
//#ifdef TEST_MODE
//	return TRUE;
//#endif
//
//	BOOL bRtn1 = FALSE;
//
//	if (m_pVision[1])
//		bRtn1 = m_pVision[1]->StartLive();
//
//	if (bRtn1)
//		return TRUE;
//
//	return  FALSE;
//}

BOOL CGvisR2R_LaserView::StopLive()
{
#ifdef TEST_MODE
	return TRUE;
#endif

	if (StopLive0())	// && StopLive1())
		return TRUE;

	return FALSE;
}

BOOL CGvisR2R_LaserView::StopLive0()
{
#ifdef TEST_MODE
	return TRUE;
#endif

	BOOL bRtn0 = FALSE;

#ifdef USE_VISION
	if (m_pVision[0])
		bRtn0 = m_pVision[0]->StopLive();
#endif
	if (bRtn0)
		return TRUE;

	return FALSE;
}

//BOOL CGvisR2R_LaserView::StopLive1()
//{
//#ifdef TEST_MODE
//	return TRUE;
//#endif
//
//	BOOL bRtn1 = FALSE;
//
//	if (m_pVision[1])
//		bRtn1 = m_pVision[1]->StopLive();
//
//	if (bRtn1)
//		return TRUE;
//
//	return FALSE;
//}

void CGvisR2R_LaserView::UpdateRst()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateRst();
}

// BOOL CGvisR2R_LaserView::IsChkTmpStop()
// {
// 	if(pDoc->WorkingInfo.LastJob.bTempPause)
// 	{
// 		if(m_dEnc[AXIS_RENC] >= m_dTempPauseLen)
// 		{
// 			m_dTempPauseLen += m_dTempPauseLen;
// 			return TRUE;
// 		}
// 	}
// 	return FALSE;
// }

BOOL CGvisR2R_LaserView::IsChkTmpStop()
{
	if (pDoc->WorkingInfo.LastJob.bTempPause)
	{
		double dFdLen = GetMkFdLen();
		if (dFdLen >= _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen)*1000.0)
		{
			// 			m_dTempPauseLen += m_dTempPauseLen;
			pDoc->WorkingInfo.LastJob.bTempPause = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsVerify()
{
	BOOL bVerify = FALSE;
	int nSerial0 = m_nBufUpSerial[0];
	int nSerial1 = m_nBufUpSerial[1];
	int nPeriod = pDoc->WorkingInfo.LastJob.nVerifyPeriod;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		nSerial0 = m_nBufDnSerial[0];
		nSerial1 = m_nBufDnSerial[1];
	}


	if (pDoc->WorkingInfo.LastJob.bVerify)
	{
		double dFdLen = GetMkFdLen();
		double dVerifyLen = _tstof(pDoc->WorkingInfo.LastJob.sVerifyLen)*1000.0;
		// 		if(dFdLen >= dVerifyLen && dFdLen < dVerifyLen+pDoc->m_pReelMap->m_dPnlLen)
		if (dFdLen < dVerifyLen)
		{
			if (nSerial0 == 1 || nPeriod == 0 || nPeriod == 1 || nPeriod == 2 || m_bStopF_Verify)
			{
				m_bStopF_Verify = FALSE;
				bVerify = TRUE;
			}
			else
			{
				if (!(nSerial0 % nPeriod) || !(nSerial1 % nPeriod))
					bVerify = TRUE;
			}
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bVerify = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();
		}
	}

	return bVerify;
}

int CGvisR2R_LaserView::GetVsBufLastSerial()
{
	int nLastShot = pDoc->GetLastShotMk();
	if (nLastShot > 0 && m_bCont)
		return (nLastShot + 4);

	return 4;
}

int CGvisR2R_LaserView::GetVsUpBufLastSerial()
{
	int nLastShot = pDoc->GetLastShotUp();
	if (nLastShot > 0 && m_bCont)
		return (nLastShot + 4);

	return 4;
}

int CGvisR2R_LaserView::GetVsDnBufLastSerial()
{
	int nLastShot = pDoc->GetLastShotDn();
	if (nLastShot > 0 && m_bCont)
		return (nLastShot + 4);

	return 4;
}

BOOL CGvisR2R_LaserView::IsFixPcsUp(int nSerial)
{
	if (!pDoc->m_pReelMapUp)
		return FALSE;

	CString sMsg = _T(""), str = _T("");
	int nStrip, pCol[2500], pRow[2500], nTot;

	if (pDoc->m_pReelMapUp->IsFixPcs(nSerial, pCol, pRow, nTot))
	{
		int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
		int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
		int nStPcsY = nNodeY / MAX_STRIP_NUM;

		sMsg.Format(_T("상면 고정불량 발생"));
		for (int i = 0; i < nTot; i++)
		{
			nStrip = int(pRow[i] / nStPcsY);
			if (!(i % 5))
				str.Format(_T("\r\n[%d:%c-%d,%d]"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1);
			else
				str.Format(_T(" , [%d:%c-%d,%d]"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1);

			sMsg += str;
		}
		m_sFixMsg[0] = sMsg;
		// 		TowerLamp(RGB_RED, TRUE);
		// 		Buzzer(TRUE, 0);
		// 		MsgBox(sMsg);
		return TRUE;
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::IsFixPcsDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return FALSE;

	if (!pDoc->m_pReelMapUp)
		return FALSE;

	CString sMsg = _T(""), str = _T("");
	int nStrip, pCol[2500], pRow[2500], nTot;

	if (pDoc->m_pReelMapDn->IsFixPcs(nSerial, pCol, pRow, nTot))
	{
		int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
		int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
		int nStPcsY = nNodeY / 4;

		sMsg.Format(_T("하면 고정불량 발생"));
		for (int i = 0; i < nTot; i++)
		{
			nStrip = int(pRow[i] / nStPcsY);
			if (!(i % 5))
				str.Format(_T("\r\n[%d:%c-%d,%d]"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1);
			else
				str.Format(_T(" , [%d:%c-%d,%d]"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1);

			sMsg += str;
		}
		// 		TowerLamp(RGB_RED, TRUE);
		// 		Buzzer(TRUE, 0);
		// 		MsgBox(sMsg);
		m_sFixMsg[1] = sMsg;
		return TRUE;
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::IsReview()
{
	return (pDoc->WorkingInfo.LastJob.bReview);
}

BOOL CGvisR2R_LaserView::IsReview0()
{
	return (pDoc->WorkingInfo.LastJob.bReview);
}

BOOL CGvisR2R_LaserView::IsReview1()
{
	return (pDoc->WorkingInfo.LastJob.bReview);
}


BOOL CGvisR2R_LaserView::IsJogRtDn()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[4] & (0x01 << 10) ? TRUE : FALSE;	// 마킹부 JOG 버튼(우)
#endif
	return bOn;
}

BOOL CGvisR2R_LaserView::IsJogRtUp()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[4] & (0x01 << 10) ? FALSE : TRUE;	// 마킹부 JOG 버튼(우)
#endif
	return bOn;
}

BOOL CGvisR2R_LaserView::IsJogRtDn0()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[4] & (0x01 << 10) ? TRUE : FALSE;	// 마킹부 JOG 버튼(우)
#endif
	return bOn;
}

BOOL CGvisR2R_LaserView::IsJogRtUp0()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[4] & (0x01 << 10) ? FALSE : TRUE;	// 마킹부 JOG 버튼(우)
#endif
	return bOn;
}

//BOOL CGvisR2R_LaserView::IsJogRtDn1()
//{
//	BOOL bOn = pDoc->m_pMpeI[4] & (0x01 << 10) ? TRUE : FALSE;	// 마킹부 JOG 버튼(우)
//	return bOn;
//}

//BOOL CGvisR2R_LaserView::IsJogRtUp1()
//{
//	BOOL bOn = pDoc->m_pMpeI[4] & (0x01 << 10) ? FALSE : TRUE;	// 마킹부 JOG 버튼(우)
//	return bOn;
//}

void CGvisR2R_LaserView::OpenShareUp(BOOL bOpen)
{
	m_bOpenShareUp = bOpen;
}

BOOL CGvisR2R_LaserView::IsOpenShareUp()
{
	return m_bOpenShareUp;
}

void CGvisR2R_LaserView::OpenShareDn(BOOL bOpen)
{
	m_bOpenShareDn = bOpen;
}

BOOL CGvisR2R_LaserView::IsOpenShareDn()
{
	return m_bOpenShareDn;
}


void CGvisR2R_LaserView::SwAoiEmg(BOOL bOn)
{
	if (m_pDlgMenu03)
		m_pDlgMenu03->SwAoiEmg(bOn);

	if (bOn)
	{
		// 		pDoc->m_pMpeIo[8] |= (0x01<<0);		// 언코일러 비상정지 스위치 램프
		// 		pDoc->m_pMpeIo[12] |= (0x01<<0);	// 리코일러 비상정지 스위치 램프

		// 		pDoc->m_pSliceIo[9] |= (0x01<<14);	// 검사부 비상정지 스위치(스위치)
		// 		pDoc->m_pSliceIo[9] |= (0x01<<15);	// 검사부 비상정지 스위치(후면)
		// 		pDoc->m_pSliceIo[6] |= (0x01<<0);	// 마킹부 비상정지 스위치(모니터)
		// 		pDoc->m_pSliceIo[7] |= (0x01<<0);	// 마킹부 비상정지 스위치(스위치)
	}
	else
	{
		// 		pDoc->m_pMpeIo[8] &= ~(0x01<<0);	// 언코일러 비상정지 스위치 램프
		// 		pDoc->m_pMpeIo[12] &= ~(0x01<<0);	// 리코일러 비상정지 스위치 램프

		// 		pDoc->m_pSliceIo[9] &= ~(0x01<<14);	// 검사부 비상정지 스위치(스위치)
		// 		pDoc->m_pSliceIo[9] &= ~(0x01<<15);	// 검사부 비상정지 스위치(후면)
		// 		pDoc->m_pSliceIo[6] &= ~(0x01<<0);	// 마킹부 비상정지 스위치(모니터)
		// 		pDoc->m_pSliceIo[7] &= ~(0x01<<0);	// 마킹부 비상정지 스위치(스위치)
	}

}

BOOL CGvisR2R_LaserView::IsVs()
{
	if (!m_bChkLastProcVs)
	{
		// 		int nTotPnl = int(_tstof(pDoc->WorkingInfo.Motion.sFdInitDist) / _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen));;

		BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
		if (bDualTest)
		{
			if (GetAoiDnVsStatus())
				return TRUE;
			else if (GetAoiUpVsStatus())
				return TRUE;

			// 			if(m_nBufTot[1] > 2 && m_nBufTot[1] < nTotPnl-1)	// AOI 하면 버퍼 기준.
			// 				return TRUE;
			// 			else 
			// 			{
			// 				nTotPnl += _tstoi(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot);
			// 				if(m_nBufTot[0] > 2 && m_nBufTot[0] < nTotPnl-1)	// AOI 상면 버퍼 기준. 20160807
			// 					return TRUE;
			// 			}
		}
		else
		{
			if (GetAoiUpVsStatus())
				return TRUE;
			// 			nTotPnl += _tstoi(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot);
			// 			if(m_nBufTot[0] > 2 && m_nBufTot[0] < nTotPnl-1)	// AOI 상면 버퍼 기준.
			// 				return TRUE;
		}
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::IsVsUp()
{
	// 	int nTotPnl = int(_tstof(pDoc->WorkingInfo.Motion.sFdInitDist) / _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen));
	// 
	// 	nTotPnl += _tstoi(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot);
	// 	if(m_nBufTot[0] > 2 && m_nBufTot[0] < nTotPnl-1)	// AOI 상면 버퍼 기준.
	// 		return TRUE;
	// 	
	// 	return FALSE;

	return GetAoiUpVsStatus();
}

BOOL CGvisR2R_LaserView::IsVsDn()
{
	// 	int nTotPnl = int(_tstof(pDoc->WorkingInfo.Motion.sFdInitDist) / _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen));
	// 
	// 	if(m_nBufTot[1] > 2 && m_nBufTot[1] < nTotPnl-1)	// AOI 하면 버퍼 기준.
	// 		return TRUE;
	// 	
	// 	return FALSE;

	return GetAoiDnVsStatus();
}

void CGvisR2R_LaserView::SetDummyUp()
{
	m_nDummy[0]++;
	if (m_nDummy[0] == 3)
	{
		if (!MakeDummyUp(-2))
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - MakeDummyUp(-2)"));
	}
	}
	else
	{
		if (!MakeDummyUp(-1))
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - MakeDummyUp(-1)"));
	}
	}
}

void CGvisR2R_LaserView::SetDummyDn()
{
	m_nDummy[1]++;
	if (m_nDummy[1] == 3)
	{
		if (!MakeDummyDn(-2))
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - MakeDummyDn(-2)"));
	}
	}
	else
	{
		if (!MakeDummyDn(-1))
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - MakeDummyDn(-1)"));
	}
	}
}

BOOL CGvisR2R_LaserView::MakeDummyUp(int nErr) // AOI 상면 기준.
{
	int nSerial = m_nAoiLastSerial[0] + m_nDummy[0];
	FILE *fpPCR = NULL;

	CString sMsg, strRstPath, strRstPath2, sDummyRst;
	strRstPath.Format(_T("%s%s\\%s\\\\%s\\%04d.RST"), pDoc->WorkingInfo.System.sPathAoiUpVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		m_nAoiLastSerial[0]);

	CDataFile *pDataFile = new CDataFile;

	if (!pDataFile->Open(strRstPath))
	{
		sMsg.Format(_T("%s File not found."), strRstPath);
		pView->MsgBox(sMsg);
		delete pDataFile;
		return FALSE;
	}
	CString sLine, sRemain, sNewLine;
	sLine = pDataFile->GetLineString(1);
	int nPos = sLine.Find(',', 0);
	sLine.Delete(0, nPos);
	sNewLine.Format(_T("%d%s"), 0, sLine);
	pDataFile->ReplaceLine(1, sNewLine);

	strRstPath2.Format(_T("%s%s\\%s\\\\%s\\%04d.RST"), pDoc->WorkingInfo.System.sPathAoiUpVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		nSerial);
	char* pRtn = NULL;
	fpPCR = fopen(pRtn = StringToChar(strRstPath2), "w+"); if(pRtn) delete pRtn; pRtn = NULL;
	if (fpPCR == NULL)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("TROUBLE_SAVE_PIECEOUT_VRS"), MB_ICONWARNING | MB_OK);
	}
	fprintf(fpPCR, pRtn = StringToChar(pDataFile->GetAllString())); if (pRtn) delete pRtn; pRtn = NULL;
	if (fpPCR != NULL)
		fclose(fpPCR);

	delete pDataFile;


	CString sDummyPath;
	sDummyPath.Format(_T("%s%s\\%s\\\\%s\\%04d.PCR"), pDoc->WorkingInfo.System.sPathAoiUpVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		nSerial);

	fpPCR = fopen(pRtn = StringToChar(sDummyPath), "w+"); if(pRtn) delete pRtn; pRtn = NULL;
	if (fpPCR == NULL)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("TROUBLE_SAVE_PIECEOUT_VRS"), MB_ICONWARNING | MB_OK);
	}

	fprintf(fpPCR, "%04d,%s,%s,%s\n", nErr, pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		pDoc->WorkingInfo.LastJob.sLotUp);

	fprintf(fpPCR, "%04d\n", 0);

	fprintf(fpPCR, "%s", pRtn = StringToChar(_T("파일마지막"))); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fpPCR, "\n");

	if (fpPCR != NULL)
		fclose(fpPCR);



	sDummyPath.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVsShareUp, nSerial);

	fpPCR = fopen(pRtn = StringToChar(sDummyPath), "w+"); if(pRtn) delete pRtn; pRtn = NULL;
	if (fpPCR == NULL)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("TROUBLE_SAVE_PIECEOUT_VRS"), MB_ICONWARNING | MB_OK);
	}

	fprintf(fpPCR, "%04d,%s,%s,%s\n", nErr, pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		pDoc->WorkingInfo.LastJob.sLotUp);

	fprintf(fpPCR, "%04d\n", 0);

	fprintf(fpPCR, "%s", pRtn = StringToChar(_T("파일마지막"))); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fpPCR, "\n");

	if (fpPCR != NULL)
		fclose(fpPCR);

	return TRUE;
}

BOOL CGvisR2R_LaserView::MakeDummyDn(int nErr) // AOI 상면 기준.
{
	m_nAoiLastSerial[1] = m_nAoiLastSerial[0] - 3;
	int nSerial = m_nAoiLastSerial[0] + m_nDummy[1];
	FILE *fpPCR = NULL;

	CString sMsg, strRstPath, strRstPath2, sDummyRst;
	strRstPath.Format(_T("%s%s\\%s\\\\%s\\%04d.RST"), pDoc->WorkingInfo.System.sPathAoiDnVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp,
		//pDoc->WorkingInfo.LastJob.sModelDn,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		pDoc->WorkingInfo.LastJob.sLotUp,
		//pDoc->WorkingInfo.LastJob.sLotDn,
		m_nAoiLastSerial[1]);

	CDataFile *pDataFile = new CDataFile;
	char* pRtn = NULL;
	if (!pDataFile->Open(strRstPath))
	{
		sMsg.Format(_T("%s File not found."), strRstPath);
		pView->MsgBox(sMsg);
		delete pDataFile;
		return FALSE;
	}
	CString sLine, sRemain, sNewLine;
	sLine = pDataFile->GetLineString(1);
	int nPos = sLine.Find(',', 0);
	sLine.Delete(0, nPos);
	sNewLine.Format(_T("%d%s"), 0, sLine);
	pDataFile->ReplaceLine(1, sNewLine);

	strRstPath2.Format(_T("%s%s\\%s\\\\%s\\%04d.RST"), pDoc->WorkingInfo.System.sPathAoiDnVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp,
		//pDoc->WorkingInfo.LastJob.sModelDn,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		pDoc->WorkingInfo.LastJob.sLotUp,
		//pDoc->WorkingInfo.LastJob.sLotDn,
		nSerial);
	fpPCR = fopen(pRtn = StringToChar(strRstPath2), "w+"); if (pRtn) delete pRtn; pRtn = NULL;
	if (fpPCR == NULL)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("TROUBLE_SAVE_PIECEOUT_VRS"), MB_ICONWARNING | MB_OK);
	}
	fprintf(fpPCR, pRtn = StringToChar(pDataFile->GetAllString())); if (pRtn) delete pRtn; pRtn = NULL;
	if (fpPCR != NULL)
		fclose(fpPCR);

	delete pDataFile;


	CString sDummyPath;
	sDummyPath.Format(_T("%s%s\\%s\\\\%s\\%04d.PCR"), pDoc->WorkingInfo.System.sPathAoiDnVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp,
		//pDoc->WorkingInfo.LastJob.sModelDn,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		pDoc->WorkingInfo.LastJob.sLotUp,
		//pDoc->WorkingInfo.LastJob.sLotDn,
		nSerial);

	fpPCR = fopen(pRtn = StringToChar(sDummyPath), "w+"); if (pRtn) delete pRtn; pRtn = NULL;
	if (fpPCR == NULL)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("TROUBLE_SAVE_PIECEOUT_VRS"), MB_ICONWARNING | MB_OK);
	}

	fprintf(fpPCR, "%04d,%s,%s,%s\n", nErr, pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		pDoc->WorkingInfo.LastJob.sLotUp);
	//fprintf(fpPCR, "%04d,%s,%s,%s\n", nErr, pDoc->WorkingInfo.LastJob.sModelDn,
	//	pDoc->WorkingInfo.LastJob.sLayerDn,
	//	pDoc->WorkingInfo.LastJob.sLotDn);

	fprintf(fpPCR, "%04d\n", 0);

	fprintf(fpPCR, "%s", pRtn = StringToChar(_T("파일마지막"))); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fpPCR, "\n");

	if (fpPCR != NULL)
		fclose(fpPCR);



	sDummyPath.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVsShareDn, nSerial);

	fpPCR = fopen(pRtn = StringToChar(sDummyPath), "w+"); delete pRtn;
	if (fpPCR == NULL)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("TROUBLE_SAVE_PIECEOUT_VRS"), MB_ICONWARNING | MB_OK);
	}

	fprintf(fpPCR, "%04d,%s,%s,%s\n", nErr, pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		pDoc->WorkingInfo.LastJob.sLotUp);
	//fprintf(fpPCR, "%04d,%s,%s,%s\n", nErr, pDoc->WorkingInfo.LastJob.sModelDn,
	//	pDoc->WorkingInfo.LastJob.sLayerDn,
	//	pDoc->WorkingInfo.LastJob.sLotDn);

	fprintf(fpPCR, "%04d\n", 0);

	fprintf(fpPCR, "%s", pRtn = StringToChar(_T("파일마지막"))); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fpPCR, "\n");

	if (fpPCR != NULL)
		fclose(fpPCR);

	return TRUE;
}

void CGvisR2R_LaserView::SetAoiDummyShot(int nAoi, int nDummy)
{
	pDoc->AoiDummyShot[nAoi] = nDummy;
	switch (nAoi)
	{
	case 0:
		if (m_pMpe)
			m_pMpe->Write(_T("ML45068"), (long)nDummy);	// 검사부 (상) 작업종료 더미 Shot수 - 20141104
		break;
	case 1:
		if (m_pMpe)
			m_pMpe->Write(_T("ML45070"), (long)nDummy);	// 검사부 (하) 작업종료 더미 Shot수 - 20141104
		break;
	}
}

int CGvisR2R_LaserView::GetAoiUpDummyShot()
{
	int nDummy = 0;
	//char szData[200];
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpCurrInfo;
	if (0 < ::GetPrivateProfileString(_T("Dummy"), _T("Shot"), NULL, szData, sizeof(szData), sPath))
		nDummy = _tstoi(szData);
	else
		nDummy = 0;

	return nDummy;
}

int CGvisR2R_LaserView::GetAoiDnDummyShot()
{
	int nDummy = 0;
	//char szData[200];
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiDnCurrInfo;
	if (0 < ::GetPrivateProfileString(_T("Dummy"), _T("Shot"), NULL, szData, sizeof(szData), sPath))
		nDummy = _tstoi(szData);
	else
		nDummy = 0;

	return nDummy;
}

int CGvisR2R_LaserView::GetAoiUpSerial()
{
	int nSerial = 0;
	//char szData[200];
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpCurrInfo;
	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Serial"), NULL, szData, sizeof(szData), sPath))
		nSerial = _tstoi(szData);
	else
		nSerial = 0;
	if (nSerial > 1)
		nSerial--;

	return nSerial;
}

BOOL CGvisR2R_LaserView::GetAoiUpVsStatus()
{
	BOOL bVsStatus = FALSE;
	//char szData[200];
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpCurrInfo;
	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current VS Status"), NULL, szData, sizeof(szData), sPath))
		bVsStatus = _tstoi(szData) > 0 ? TRUE : FALSE;

	return bVsStatus;
}

int CGvisR2R_LaserView::GetAoiDnSerial()
{
	int nSerial = 0;
	//char szData[200];
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiDnCurrInfo;
	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Serial"), NULL, szData, sizeof(szData), sPath))
		nSerial = _tstoi(szData);
	else
		nSerial = 0;
	if (nSerial > 1)
		nSerial--;

	return nSerial;
}

BOOL CGvisR2R_LaserView::GetAoiDnVsStatus()
{
	BOOL bVsStatus = FALSE;
	//char szData[200];
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiDnCurrInfo;
	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current VS Status"), NULL, szData, sizeof(szData), sPath))
		bVsStatus = _tstoi(szData) > 0 ? TRUE : FALSE;

	return bVsStatus;
}

BOOL CGvisR2R_LaserView::IsDoneDispMkInfo()
{
	BOOL bRtn = FALSE;
	if (m_pDlgMenu01)
		bRtn = m_pDlgMenu01->IsDoneDispMkInfo();
	return bRtn;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//void CGvisR2R_LaserView::Marking()
//{
//}

void CGvisR2R_LaserView::DispDefImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CString sNewLot;
	BOOL bNewModel = FALSE;
	int nSerial = 0;
	int nNewLot = 0;
	int nBreak = 0;

	switch (m_nStepTHREAD_DISP_DEF)
	{
		// CopyDefImg Start ============================================
	case 0:
		m_nStepTHREAD_DISP_DEF++;
		m_nBufUpSerial[0] = m_nBufDnSerial[0] = _ttoi(pView->GetMkMenu01(_T("DispDefImg"), _T("SerialL")));
		m_nBufUpSerial[1] = m_nBufDnSerial[1] = _ttoi(pView->GetMkMenu01(_T("DispDefImg"), _T("SerialR")));

		//m_nDef[0] = _ttoi(pView->GetMkDispInfoUp(_T("Info"), _T("TotalDef"), pView->m_nBufUpSerial[0]));

		if (bDualTest)
		{
			nSerial = m_nBufDnSerial[0];
			sNewLot = m_sNewLotDn;
		}
		else
		{
			nSerial = m_nBufUpSerial[0];
			sNewLot = m_sNewLotUp;
		}
		break;
	case 1:
		Sleep(300);
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 2:
		if (bDualTest)
			nSerial = m_nBufDnSerial[0];
		else
			nSerial = m_nBufUpSerial[0];

		if (IsDoneDispMkInfo())	 // Check 불량이미지 Display End
		{
			if (ChkLastProc() && (nSerial > m_nLotEndSerial))
			{
				SetSerialReelmap(nSerial, TRUE);	// Reelmap(좌) Display Start
				SetSerialMkInfo(nSerial, TRUE);		// 불량이미지(좌) Display Start
			}
			else if (ChkLastProc() && nSerial < 1)
			{
				SetSerialReelmap(m_nLotEndSerial + 1, TRUE);		// Reelmap(좌) Display Start
				SetSerialMkInfo(m_nLotEndSerial + 1, TRUE);		// 불량이미지(좌) Display Start
			}
			else
			{
				SetFixPcs(nSerial);
				SetSerialReelmap(nSerial);	// Reelmap(좌) Display Start
				SetSerialMkInfo(nSerial);	// 불량이미지(좌) Display Start
			}


			//if(!m_bLastProc)
			//{
			//	if (IsFixPcsUp(nSerial))
			//	{
			//		m_bDispMsgDoAuto[2] = TRUE;
			//		m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
			//	}
			//	if (IsFixPcsDn(nSerial))
			//	{
			//		m_bDispMsgDoAuto[3] = TRUE;
			//		m_nStepDispMsg[3] = FROM_DISPDEFIMG + 3;
			//	}
			//}

			m_nStepTHREAD_DISP_DEF++;
		}

		break;

	case 3:
		if (IsDoneDispMkInfo())	 // Check 불량이미지(좌) Display End
		{
			if (bDualTest)
				nSerial = m_nBufDnSerial[1];
			else
				nSerial = m_nBufUpSerial[1];

			if (nSerial > 0)
			{
				if (ChkLastProc() && (nSerial > m_nLotEndSerial))
					SetSerialMkInfo(nSerial, TRUE);	// 불량이미지(우) Display Start
				else
					SetSerialMkInfo(nSerial);		// 불량이미지(우) Display Start
			}
			else
			{
				if (ChkLastProc())
					SetSerialMkInfo(m_nLotEndSerial + 1, TRUE);	// 불량이미지(우) Display Start
			}
			m_nStepTHREAD_DISP_DEF++;
		}
		break;
	case 4:
		if (bDualTest)
			nSerial = m_nBufDnSerial[1];
		else
			nSerial = m_nBufUpSerial[1];

		if (nSerial > 0)
		{
			m_nStepTHREAD_DISP_DEF++;

			if (ChkLastProc() && (nSerial > m_nLotEndSerial))
			{
				SetSerialReelmap(nSerial, TRUE);	// Reelmap(우) Display Start
			}
			else
			{
				SetFixPcs(nSerial);
				SetSerialReelmap(nSerial);			// Reelmap(우) Display Start
			}

			//if (bDualTest)
			//{
			//	if(!m_bLastProc)
			//	{
			//		if (IsFixPcsUp(nSerial))
			//		{
			//			m_bDispMsgDoAuto[2] = TRUE;
			//			m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
			//		}
			//		if (IsFixPcsDn(nSerial))
			//		{
			//			m_bDispMsgDoAuto[3] = TRUE;
			//			m_nStepDispMsg[3] = FROM_DISPDEFIMG + 3;
			//		}
			//	}
			//}
			//else
			//{
			//	//if(!m_bLastProc)
			//	{
			//		if (IsFixPcsUp(nSerial))
			//		{
			//			m_bDispMsgDoAuto[2] = TRUE;
			//			m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
			//		}
			//	}
			//}
		}
		else
		{
			if (ChkLastProc())
			{
				m_nStepTHREAD_DISP_DEF++;
				SetSerialReelmap(m_nLotEndSerial + 1, TRUE);	// 불량이미지(우) Display Start
			}
			else
			{
				m_nStepTHREAD_DISP_DEF++;

				//if (bDualTest)
				//{
				//	if (m_bLastProc && m_nBufDnSerial[0] == m_nLotEndSerial)
				//		m_nStepTHREAD_DISP_DEF++;
				//	else
				//	{
				//		m_nStepTHREAD_DISP_DEF++;
				//		//if (!m_bLastProc)
				//		//{
				//		//	m_bDispMsgDoAuto[0] = TRUE;
				//		//	m_nStepDispMsg[0] = FROM_DISPDEFIMG;
				//		//}
				//	}
				//}
				//else
				//{
				//	if (m_bLastProc && m_nBufUpSerial[0] == m_nLotEndSerial)
				//		m_nStepTHREAD_DISP_DEF++;
				//	else
				//	{
				//		m_nStepTHREAD_DISP_DEF++;
				//		//if (!m_bLastProc)
				//		//{
				//		//	m_bDispMsgDoAuto[1] = TRUE;
				//		//	m_nStepDispMsg[1] = FROM_DISPDEFIMG + 1;
				//		//}
				//	}
				//}
			}
		}
		break;
	case 5:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 6:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 7:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 8:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 9:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 10:
		m_nStepTHREAD_DISP_DEF++;
		break;

	case 11:
		if (IsDoneDispMkInfo() && IsRun())	 // Check 불량이미지(우) Display End
			m_nStepTHREAD_DISP_DEF++;
		break;
	case 12:
		m_bTHREAD_DISP_DEF = FALSE;
		break;
		// CopyDefImg End ============================================
	}

}

BOOL CGvisR2R_LaserView::IsSameUpDnLot()
{
	if (pDoc->Status.PcrShare[0].sLot == pDoc->Status.PcrShare[1].sLot)
		return TRUE;

	return FALSE;
}

int CGvisR2R_LaserView::GetAutoStep()
{
	return m_nStepAuto;
}

void CGvisR2R_LaserView::MoveMkInitPos()
{
	MoveMk0InitPos();
	//MoveMk1InitPos();
}

void CGvisR2R_LaserView::MoveMk0InitPos()
{
	double pTgtPos[2];
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	double dCurrX = pView->m_dEnc[AXIS_X0];
	double dCurrY = pView->m_dEnc[AXIS_Y0];

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		pView->m_pMotion->GetSpeedProfile0(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		if (!pView->m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel / 2.0, fAcc / 2.0, fAcc / 2.0))
		{
			if (!pView->m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel / 2.0, fAcc / 2.0, fAcc / 2.0))
				AfxMessageBox(_T("Move X0Y0 Error..."));
		}
	}
}


BOOL CGvisR2R_LaserView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message != WM_KEYDOWN)
		return CFormView::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			//m_bEscape = TRUE;
			return TRUE;
		case 'S':
		case 's':
			if (GetKeyState(VK_CONTROL) < 0) // Ctrl 키가 눌려진 상태
			{
				WINDOWPLACEMENT wndPlace;
				AfxGetMainWnd()->GetWindowPlacement(&wndPlace);
				wndPlace.showCmd |= SW_MAXIMIZE;
				AfxGetMainWnd()->SetWindowPlacement(&wndPlace);
			}
			break;
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}


int CGvisR2R_LaserView::MyPassword(CString strMsg, int nCtrlId)
{
	// 	if(m_pDlgMyMsg != NULL) 
	// 	{
	// 		m_pDlgMyMsg->DestroyWindow();
	// 		delete m_pDlgMyMsg;
	// 		m_pDlgMyMsg = NULL;
	// 	}

	// 	CString msg = Filtering(strMsg);

	CDlgMyPassword dlg1(this);
	dlg1.SetMsg(strMsg, nCtrlId);
	dlg1.DoModal();
	return (dlg1.m_nRtnVal);

}

BOOL CGvisR2R_LaserView::ReloadRst()
{
	return TRUE;

	double dRatio = 0.0;
	CString sVal = _T("");
	CDlgProgress dlg;
	sVal.Format(_T("On Reloading Reelmap."));
	dlg.Create(sVal);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	BOOL bRtn[5];
	if (pDoc->m_pReelMap)
		bRtn[0] = pDoc->m_pReelMap->ReloadRst();
	//dlg.SetPos(1);
	if (pDoc->m_pReelMapUp)
		bRtn[1] = pDoc->m_pReelMapUp->ReloadRst();
	//dlg.SetPos(2);
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			bRtn[2] = pDoc->m_pReelMapDn->ReloadRst();
		//dlg.SetPos(3);
		if (pDoc->m_pReelMapAllUp)
			bRtn[3] = pDoc->m_pReelMapAllUp->ReloadRst();
		//dlg.SetPos(4);
		if (pDoc->m_pReelMapAllDn)
			bRtn[4] = pDoc->m_pReelMapAllDn->ReloadRst();
		//dlg.SetPos(5);
	}

	int nRatio[5] = { 0 };
	BOOL bDone[5] = { 0 };
	int nTo = 0;
	if (bDualTest)
		nTo = 500; //[%]
	else
		nTo = 200; //[%]

	dlg.SetRange(0, nTo);

	for (int nProc = 0; nProc < nTo;)
	{
		if (pDoc->m_pReelMap)
		{
			nRatio[0] = pDoc->m_pReelMap->GetProgressReloadRst();
			bDone[0] = pDoc->m_pReelMap->IsDoneReloadRst();
		}
		else
			bDone[0] = TRUE;
		if (!bRtn[0])
			bDone[0] = TRUE;

		if (pDoc->m_pReelMapUp)
		{
			nRatio[1] = pDoc->m_pReelMapUp->GetProgressReloadRst();
			bDone[1] = pDoc->m_pReelMapUp->IsDoneReloadRst();
		}
		else
			bDone[1] = TRUE;
		if (!bRtn[1])
			bDone[1] = TRUE;

		if (bDualTest)
		{
			if (pDoc->m_pReelMapDn)
			{
				nRatio[2] = pDoc->m_pReelMapDn->GetProgressReloadRst();
				bDone[2] = pDoc->m_pReelMapDn->IsDoneReloadRst();
			}
			else
				bDone[2] = TRUE;
			if (!bRtn[2])
				bDone[2] = TRUE;

			if (pDoc->m_pReelMapAllUp)
			{
				nRatio[3] = pDoc->m_pReelMapAllUp->GetProgressReloadRst();
				bDone[3] = pDoc->m_pReelMapAllUp->IsDoneReloadRst();
			}
			else
				bDone[3] = TRUE;
			if (!bRtn[3])
				bDone[3] = TRUE;

			if (pDoc->m_pReelMapAllDn)
			{
				nRatio[4] = pDoc->m_pReelMapAllDn->GetProgressReloadRst();
				bDone[4] = pDoc->m_pReelMapAllDn->IsDoneReloadRst();
			}
			else
				bDone[4] = TRUE;
			if (!bRtn[4])
				bDone[4] = TRUE;

		}
		else
		{
			bDone[2] = TRUE;
			bDone[3] = TRUE;
			bDone[4] = TRUE;
		}

		nProc = nRatio[0] + nRatio[1] + nRatio[2] + nRatio[3] + nRatio[4];

		if (bDone[0] && bDone[1] && bDone[2] && bDone[3] && bDone[4])
			break;
		else
		{
			dlg.SetPos(nProc);
			Sleep(30);
		}
	}

	dlg.DestroyWindow();

	if (bDualTest)
	{
		for (int i = 0; i < 5; i++)
		{
			if (!bRtn[i])
				return FALSE;
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			if (!bRtn[i])
				return FALSE;
		}
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		return ReloadRstInner();
	}

	return TRUE;
}
void CGvisR2R_LaserView::ReloadRstUp()
{
return;
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->ReloadRst();
}

void CGvisR2R_LaserView::ReloadRstAllUp()
{
return;

	if (pDoc->m_pReelMapAllUp)
		pDoc->m_pReelMapAllUp->ReloadRst();
}

void CGvisR2R_LaserView::ReloadRstDn()
{
return;
	if (pDoc->m_pReelMapDn)
		pDoc->m_pReelMapDn->ReloadRst();
}

void CGvisR2R_LaserView::ReloadRstAllDn()
{
return;
	if (pDoc->m_pReelMapAllDn)
		pDoc->m_pReelMapAllDn->ReloadRst();
}

BOOL CGvisR2R_LaserView::ReloadRst(int nSerial)
{
	return TRUE;

	m_nReloadRstSerial = nSerial;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	m_bTHREAD_RELOAD_RST_UP = TRUE;
	if (bDualTest)
	{
		m_bTHREAD_RELOAD_RST_DN = TRUE;
		m_bTHREAD_RELOAD_RST_ALLUP = TRUE;
		m_bTHREAD_RELOAD_RST_ALLDN = TRUE;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		m_bTHREAD_RELOAD_RST_UP_INNER = TRUE;
		m_bTHREAD_RELOAD_RST_ITS = TRUE;
		if (pDoc->WorkingInfo.LastJob.bDualTestInner)
		{
			m_bTHREAD_RELOAD_RST_DN_INNER = TRUE;
			m_bTHREAD_RELOAD_RST_ALLUP_INNER = TRUE;
			m_bTHREAD_RELOAD_RST_ALLDN_INNER = TRUE;
		}
	}

	Sleep(100);


	return TRUE;
}

void CGvisR2R_LaserView::ReloadRstUpInner()
{
return;
	if (pDoc->m_pReelMapInnerUp)
		pDoc->m_pReelMapInnerUp->ReloadRst();
}

void CGvisR2R_LaserView::ReloadRstAllUpInner()
{
return;
	if (pDoc->m_pReelMapInnerAllUp)
		pDoc->m_pReelMapInnerAllUp->ReloadRst();
}

void CGvisR2R_LaserView::ReloadRstDnInner()
{
return;
	if (pDoc->m_pReelMapInnerDn)
		pDoc->m_pReelMapInnerDn->ReloadRst();
}

BOOL CGvisR2R_LaserView::OpenReelmapFromBuf(int nSerial)
{
	return TRUE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	stModelInfo stInfoUp, stInfoDn;
	CString sSrc;
	sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	if (!pDoc->GetPcrInfo(sSrc, stInfoUp))
	{
		pView->DispStsBar(_T("E(6)"), 5);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-GetPcrInfo(6)"));
		return FALSE;
	}
	if (bDualTest)
	{
		sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufDn, nSerial);
		if (!pDoc->GetPcrInfo(sSrc, stInfoDn))
		{
			pView->DispStsBar(_T("E(7)"), 5);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error-GetPcrInfo(7)"));
			return FALSE;
		}
	}

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->Open();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->Open();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->Open();
	}

	return TRUE;
}

void CGvisR2R_LaserView::OpenReelmap()
{
	//return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->Open();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->Open();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->Open();
	}

	//if (pDoc->m_pReelMap)
	//{
	//	if (pDoc->m_pReelMap->m_nLayer < 0)
	//		pDoc->m_pReelMap->m_nLayer = pView->m_nSelRmap;
	//	pDoc->m_pReelMap->Open();
	//}
}

void CGvisR2R_LaserView::OpenReelmapUp()
{
	//return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->Open();
	}

	if (pDoc->m_pReelMap)
	{
		//if (pDoc->m_pReelMap->m_nLayer < 0)
		//	pDoc->m_pReelMap->m_nLayer = pView->m_nSelRmap;

		if (bDualTest)
		{
			if (pDoc->m_pReelMap->m_nLayer == RMAP_UP || pDoc->m_pReelMap->m_nLayer == RMAP_ALLUP)
				pDoc->m_pReelMap->Open();
		}
		else
		{
			if (pDoc->m_pReelMap->m_nLayer == RMAP_UP)
				pDoc->m_pReelMap->Open();
		}
	}
}

void CGvisR2R_LaserView::OpenReelmapDn()
{
	return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	if (pDoc->m_pReelMapDn)
		pDoc->m_pReelMapDn->Open();
	if (pDoc->m_pReelMapAllDn)
		pDoc->m_pReelMapAllDn->Open();

	//if (pDoc->m_pReelMap)
	//{
	//	if (pDoc->m_pReelMap->m_nLayer < 0)
	//		pDoc->m_pReelMap->m_nLayer = pView->m_nSelRmap;

	//	if (pDoc->m_pReelMap->m_nLayer == RMAP_DN || pDoc->m_pReelMap->m_nLayer == RMAP_ALLDN)
	//		pDoc->m_pReelMap->Open();
	//}
}

void CGvisR2R_LaserView::EStop()
{
	if (m_pMotion)
	{
		m_pMotion->EStop(MS_X0Y0);
		//m_pMotion->EStop(MS_X1Y1);
		Sleep(30);
		ResetMotion(MS_X0Y0);
		Sleep(30);
		//ResetMotion(MS_X1Y1);
		//Sleep(30);
		// 		DispMsg(_T("X축 충돌 범위에 의한 정지입니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
		AfxMessageBox(_T("X축 충돌 범위에 의한 정지입니다."));
	}
}

//void CGvisR2R_LaserView::ResetPriority()
//{
//	m_bPriority[0] = FALSE;
//	m_bPriority[1] = FALSE;
//	m_bPriority[2] = FALSE;
//	m_bPriority[3] = FALSE;
//}

//void CGvisR2R_LaserView::SetPriority()
//{
//	int nDir[2];
//	nDir[0] = (m_dTarget[AXIS_X0] - m_dEnc[AXIS_X0]) >= 0.0 ? 1 : -1;
//	nDir[1] = (m_dTarget[AXIS_X1] - m_dEnc[AXIS_X1]) >= 0.0 ? 1 : -1;
//
//	// Cam0 : m_bPriority[0], m_bPriority[3]
//	// Cam1 : m_bPriority[1], m_bPriority[2]
//	if (nDir[1] > 0) // Cam1 ->
//	{
//		m_bPriority[0] = FALSE;
//		m_bPriority[1] = TRUE;
//		m_bPriority[2] = FALSE;
//		m_bPriority[3] = FALSE;
//	}
//	else if (nDir[0] < 0) // Cam0 <-
//	{
//		m_bPriority[0] = TRUE;
//		m_bPriority[1] = FALSE;
//		m_bPriority[2] = FALSE;
//		m_bPriority[3] = FALSE;
//	}
//	else if (nDir[1] < 0) // Cam1 <-
//	{
//		m_bPriority[0] = FALSE;
//		m_bPriority[1] = FALSE;
//		m_bPriority[2] = TRUE;
//		m_bPriority[3] = FALSE;
//	}
//	else // Cam0 ->
//	{
//		m_bPriority[0] = FALSE;
//		m_bPriority[1] = FALSE;
//		m_bPriority[2] = FALSE;
//		m_bPriority[3] = TRUE;
//	}
//}

BOOL CGvisR2R_LaserView::IsRunAxisX()
{
	if (m_pMotion->IsMotionDone(MS_X0))// && m_pMotion->IsMotionDone(MS_X1))
		return FALSE;
	return TRUE;
}

//BOOL CGvisR2R_LaserView::ChkCollision()
//{
//	double dMg = _tstof(pDoc->WorkingInfo.Motion.sCollisionLength) - _tstof(pDoc->WorkingInfo.Motion.sCollisionMargin);
//
//	if (m_bTHREAD_MK[0] || m_bTHREAD_MK[1] || m_bTHREAD_MK[2] || m_bTHREAD_MK[3])
//	{
//		if (pDoc->WorkingInfo.System.bNoMk || m_bCam)
//			dMg += (_tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX) - _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX));
//	}
//
//	if ((m_dEnc[AXIS_X0] - dMg) > m_dEnc[AXIS_X1])
//		return TRUE;
//
//	if (m_dEnc[AXIS_X0] < -1.0 || m_dEnc[AXIS_X1] < -1.0)
//		return TRUE;
//
//	return FALSE;
//}

//BOOL CGvisR2R_LaserView::ChkCollision(int nAxisId, double dTgtPosX)
//{
//	double dMg = _tstof(pDoc->WorkingInfo.Motion.sCollisionLength) - _tstof(pDoc->WorkingInfo.Motion.sCollisionMargin);
//
//
//	if (m_bTHREAD_MK[0] || m_bTHREAD_MK[1] || m_bTHREAD_MK[2] || m_bTHREAD_MK[3])
//	{
//		if (pDoc->WorkingInfo.System.bNoMk || m_bCam)
//			dMg += (_tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX) - _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX));
//	}
//
//	if (nAxisId == AXIS_X0)
//	{
//		if (dTgtPosX > m_dEnc[AXIS_X1] + dMg)
//			return TRUE;
//		if (m_bDoMk[1] && !m_bDoneMk[1])
//		{
//			if (!m_bDoneMk[0] && m_bTHREAD_MK[0])
//			{
//				if (m_dTarget[AXIS_X1] > 0.0)
//				{
//					if (dTgtPosX > m_dTarget[AXIS_X1] + dMg && m_dTarget[AXIS_X1] > 0.0)
//						return TRUE;
//					else if (m_dNextTarget[AXIS_X1] > 0.0)
//					{
//						if (dTgtPosX > m_dNextTarget[AXIS_X1] + dMg)
//							return TRUE;
//					}
//				}
//			}
//		}
//	}
//	else if (nAxisId == AXIS_X1)
//	{
//		if (dTgtPosX < m_dEnc[AXIS_X0] - dMg)
//			return TRUE;
//	}
//
//	return FALSE;
//}

//BOOL CGvisR2R_LaserView::ChkCollision(int nAxisId, double dTgtPosX, double dTgtNextPosX)
//{
//	double dMg = _tstof(pDoc->WorkingInfo.Motion.sCollisionLength) - _tstof(pDoc->WorkingInfo.Motion.sCollisionMargin);
//
//	if (m_bTHREAD_MK[0] || m_bTHREAD_MK[1] || m_bTHREAD_MK[2] || m_bTHREAD_MK[3])
//	{
//		if (pDoc->WorkingInfo.System.bNoMk || m_bCam)
//			dMg += (_tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX) - _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX));
//	}
//
//	if (nAxisId == AXIS_X0)
//	{
//		if (dTgtPosX > m_dEnc[AXIS_X1] + dMg)
//			return TRUE;
//		if (m_bDoMk[1] && !m_bDoneMk[1])
//		{
//			if (!m_bDoneMk[0] && m_bTHREAD_MK[0])
//			{
//				if (dTgtPosX > m_dTarget[AXIS_X1] + dMg && m_dTarget[AXIS_X1] > 0.0)
//					return TRUE;
//				else if (dTgtPosX > m_dNextTarget[AXIS_X1] + dMg && m_dNextTarget[AXIS_X1] > 0.0)
//					return TRUE;
//			}
//		}
//	}
//	else if (nAxisId == AXIS_X1)
//	{
//		if (dTgtPosX < m_dEnc[AXIS_X0] - dMg)
//			return TRUE;
//		if (m_bDoMk[0])
//		{
//			if (!m_bDoneMk[1] && m_bTHREAD_MK[1])
//			{
//				if (!m_pMotion->IsMotionDone(MS_X0))
//				{
//					if (dTgtPosX < m_dTarget[AXIS_X0] - dMg && m_dTarget[AXIS_X0] > 0.0)
//						return TRUE;
//					else if (dTgtPosX < m_dNextTarget[AXIS_X0] - dMg && m_dNextTarget[AXIS_X0] > 0.0)
//						return TRUE;
//				}
//			}
//		}
//	}
//
//	return FALSE;
//}

void CGvisR2R_LaserView::StopAllMk()
{
	// Mk0
	m_nStepMk[2] = MK_END;

	// Mk1
	m_nStepMk[3] = MK_END;
}

//void CGvisR2R_LaserView::DoAllMk(int nCam)
//{
//	if (nCam == CAM_LF)
//	{
//		double dCurrX = pView->m_dEnc[AXIS_X1];
//		double dCurrY = pView->m_dEnc[AXIS_Y1];
//
//		double pPos[2];
//		pPos[0] = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
//		pPos[1] = 0.0;
//
//		double fLen, fVel, fAcc, fJerk;
//		fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
//		if (fLen > 0.001)
//		{
//			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
//			if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
//			{
//				if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
//					AfxMessageBox(_T("Move X1Y1 Error..."));
//			}
//		}
//		if (!m_bTHREAD_MK[2])
//		{
//			m_nStepMk[2] = 0;
//			m_nMkPcs[2] = 0;
//			m_bTHREAD_MK[2] = TRUE;
//		}
//		else
//		{
//			m_bTHREAD_MK[2] = FALSE;
//		}
//	}
//	else if (nCam == CAM_RT)
//	{
//		double dCurrX = pView->m_dEnc[AXIS_X0];
//		double dCurrY = pView->m_dEnc[AXIS_Y0];
//
//		double pPos[2];
//		pPos[0] = 0.0;
//		pPos[1] = 0.0;
//
//		double fLen, fVel, fAcc, fJerk;
//		fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
//		if (fLen > 0.001)
//		{
//			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
//			if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
//			{
//				if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
//					AfxMessageBox(_T("Move X0Y0 Error..."));
//			}
//		}
//		if (!m_bTHREAD_MK[3])
//		{
//			m_nStepMk[3] = 0;
//			m_nMkPcs[3] = 0;
//			m_bTHREAD_MK[3] = TRUE;
//		}
//		else
//		{
//			m_bTHREAD_MK[3] = FALSE;
//		}
//	}
//	else if (nCam == CAM_BOTH)
//	{
//		if (!m_bTHREAD_MK[2])
//		{
//			m_nStepMk[2] = 0;
//			m_nMkPcs[2] = 0;
//			m_bTHREAD_MK[2] = TRUE;
//		}
//		else
//		{
//			m_bTHREAD_MK[2] = FALSE;
//		}
//
//		if (!m_bTHREAD_MK[3])
//		{
//			m_nStepMk[3] = 0;
//			m_nMkPcs[3] = 0;
//			m_bTHREAD_MK[3] = TRUE;
//		}
//		else
//		{
//			m_bTHREAD_MK[3] = FALSE;
//		}
//	}
//}


void CGvisR2R_LaserView::InitPLC()
{
#ifdef USE_MPE
	long lData;
	lData = (long)(_tstof(pDoc->WorkingInfo.Lot.sTotalReelDist) * 1000.0);
	m_pMpe->Write(_T("ML45000"), lData);	// 전체 Reel 길이 (단위 M * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Lot.sSeparateDist) * 1000.0);
	m_pMpe->Write(_T("ML45002"), lData);	// Lot 분리 길이 (단위 M * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Lot.sCuttingDist) * 1000.0);
	m_pMpe->Write(_T("ML45004"), lData);	// Lot 분리 후 절단위치 (단위 M * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Lot.sStopDist) * 1000.0);
	m_pMpe->Write(_T("ML45006"), lData);	// 일시정지 길이 (단위 M * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sAoiTq) * 1000.0);
	m_pMpe->Write(_T("ML45042"), lData);	// 검사부 Tension 모터 토크값 (단위 Kgf * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkTq) * 1000.0);
	m_pMpe->Write(_T("ML45044"), lData);	// 마킹부 Tension 모터 토크값 (단위 Kgf * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sEngraveTq) * 1000.0);
	m_pMpe->Write(_T("ML45050"), lData);	// 각인부 Tension 모터 토크값 (단위 Kgf * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen) * 1000.0);
	m_pMpe->Write(_T("ML45032"), lData);	// 한 판넬 길이 (단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdVel) * 1000.0);
	m_pMpe->Write(_T("ML45034"), lData);	// 한 판넬 Feeding 속도 (단위 mm/sec * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkJogVel) * 1000.0);
	m_pMpe->Write(_T("ML45038"), lData);	// 연속공급 속도 (단위 mm/sec * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sAoiJogAcc) * 1000.0);
	m_pMpe->Write(_T("ML45040"), lData);	// 연속공급 가속도 (단위 mm/s^2 * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdAcc) * 1000.0);
	m_pMpe->Write(_T("ML45036"), lData);	// 한 판넬 Feeding 가속도 (단위 mm/s^2 * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sAoiFdLead) * 1000.0);
	m_pMpe->Write(_T("ML45012"), lData);	// 검사부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
	m_pMpe->Write(_T("ML45020"), lData);	// 각인부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdLead) * 1000.0);
	m_pMpe->Write(_T("ML45014"), lData);	// 마킹부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sStBufPos) * 1000.0);
	pView->m_pMpe->Write(_T("ML45016"), lData);	// 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist) * 1000.0);
	pView->m_pMpe->Write(_T("ML45008"), lData);	// AOI(하)에서 마킹까지 거리 (단위 mm * 1000)
	lData = (long)(_tstoi(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * 1000);
	pView->m_pMpe->Write(_T("ML45010"), lData);	// AOI(상)에서 AOI(하) Shot수 (단위 Shot수 * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist) * 1000.0);
	pView->m_pMpe->Write(_T("ML45018"), lData);	// 각인부에서 AOI(상)까지 거리 (단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdBarcodeOffsetVel) * 1000.0);
	m_pMpe->Write(_T("ML45060"), lData);	// 2D 바코드 리더기위치까지 Feeding 속도 (단위 mm/sec * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdBarcodeOffsetAcc) * 1000.0);
	m_pMpe->Write(_T("ML45062"), lData);	// 2D 바코드 리더기위치까지 Feeding 가속도 (단위 mm/s^2 * 1000)

	lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim) * 100.0);
	m_pMpe->Write(_T("MW05940"), lData);	// AOI_Dn (단위 [초] * 100) : 1 is 10 mSec.
	m_pMpe->Write(_T("MW05942"), lData);	// AOI_Up (단위 [초] * 100) : 1 is 10 mSec.

	m_pMpe->Write(_T("MB440177"), pDoc->WorkingInfo.LastJob.bUse380mm ? 1 : 0);	// EPC실린더(제품소->OFF/제품대->ON)
#endif
}

BOOL CGvisR2R_LaserView::SetCollision(double dCollisionMargin)
{
	BOOL bRtn = FALSE;
	if (m_pMotion)
		bRtn = m_pMotion->SetCollision(dCollisionMargin);
	return bRtn;
}

//BOOL CGvisR2R_LaserView::ChkXmpCollision()
//{
//	CString sM;
//	long addressActPos1, addressActPos2, addressDifferenceStored;
//	m_pMotion->GetData(&addressActPos1, &addressActPos2, &addressDifferenceStored);
//	sM.Format(_T("%d, %d, %d"), (int)addressActPos1, (int)addressActPos2, (int)addressDifferenceStored);
//	//DispStsBar(sM);
//	return TRUE;
//}

void CGvisR2R_LaserView::MpeWrite()
{
	for (int i = TOT_M_IO - 1; i >= 0; i--)
	{
		if (pDoc->m_pIo[i].nIdx > -1)
		{
			if (m_pMpe)
				m_pMpe->Write(pDoc->m_pIo[i].sAddr, pDoc->m_pIo[i].lData);
			pDoc->m_pIo[i].nIdx = -1;
			pDoc->m_pIo[i].sAddr = _T("");
			pDoc->m_pIo[i].lData = 0;
		}
	}
}

void CGvisR2R_LaserView::IoWrite(CString sMReg, long lData)
{
	int i = 0;
	for (i = 0; i < TOT_M_IO; i++)
	{
		if (pDoc->m_pIo[i].nIdx < 0)
		{
			pDoc->m_pIo[i].nIdx = i;
			pDoc->m_pIo[i].sAddr = sMReg;
			pDoc->m_pIo[i].lData = lData;
			break;
		}
	}
	if (i >= TOT_M_IO)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Not enought TOT_M_IO Num!!!"));
	}
}

BOOL CGvisR2R_LaserView::IsRdyTest()
{
	BOOL bOn0 = IsRdyTest0();
	BOOL bOn1 = IsRdyTest1();

	if (bOn0 && bOn1)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsRdyTest0()
{
#ifdef USE_MPE
	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 상 자동 운전 <-> X432B I/F
	BOOL bOn1 = (pDoc->m_pMpeIb[10] & (0x01 << 9)) ? TRUE : FALSE;		// 검사부 상 테이블 진공 완료 <-> X4329 I/F

	if (bOn0 && bOn1)
		return TRUE;
#endif
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsRdyTest1()
{
#ifdef USE_MPE
	BOOL bOn0 = (pDoc->m_pMpeIb[14] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 하 자동 운전 <-> X442B I/F
	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01 << 9)) ? TRUE : FALSE;		// 검사부 하 테이블 진공 완료 <-> X4329 I/F
	double dCurPosMkFd = (double)pDoc->m_pMpeData[0][0];				// 마킹부 Feeding 엔코더 값(단위 mm )
	double dTgtFd = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.Motion.sAoiFdDist);
	if (dCurPosMkFd > dTgtFd - _tstof(pDoc->WorkingInfo.Motion.sAoiFdDist) / 2.0)
	{
		BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
		if (bDualTest)
		{
			if (bOn0 && bOn1)
				return TRUE;
		}
		else
		{
			if (bOn0)
				return TRUE;
		}
	}
#endif
	return FALSE;
}

//BOOL CGvisR2R_LaserView::IsMk0Done()
//{
//	BOOL bDone = FALSE;
//
//	if (pView->m_pVoiceCoil[0])
//		bDone = pView->m_pVoiceCoil[0]->IsDoneMark(0);
//
//	return bDone;
//}

//BOOL CGvisR2R_LaserView::IsMk1Done()
//{
//	BOOL bDone = FALSE;
//
//	if (pView->m_pVoiceCoil[1])
//		bDone = pView->m_pVoiceCoil[1]->IsDoneMark(1);
//
//	return bDone;
//}

void CGvisR2R_LaserView::SetPlcParam()
{
//	//long lData;
//	//IoWrite(_T("ML45006"), long(_tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen)*1000.0));	// 일시정지 길이 (단위 M * 1000)
//	pView->m_pMpe->Write(_T("ML45006"), long(_tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen)*1000.0));
//	//IoWrite(_T("ML45002"), long(_tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0));	// Lot 분리 길이 (단위 M * 1000)
//	pView->m_pMpe->Write(_T("ML45002"), long(_tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0));
//	//IoWrite(_T("ML45004"), long(_tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen)*1000.0));	// Lot 분리 후 절단위치 (단위 M * 1000)
//	pView->m_pMpe->Write(_T("ML45004"), long(_tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen)*1000.0));
//	pView->m_pMpe->Write(_T("ML45126"), (long)_tstoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum));	// 샘플검사 Shot수
//
//	if (pDoc->WorkingInfo.LastJob.bTempPause)
//	{
//		//pView->IoWrite(_T("MB440183"), 1);	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
//		pView->m_pMpe->Write(_T("MB440183"), 1);
//		pView->ChkTempStop(TRUE);
//	}
//	else
//	{
//		//pView->IoWrite(_T("MB440183"), 0);	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
//		pView->m_pMpe->Write(_T("MB440183"), 0);
//		pView->ChkTempStop(FALSE);
//	}
//
//	if (pDoc->WorkingInfo.LastJob.bLotSep)
//	{
//		pView->SetLotLastShot();
//		pView->m_pMpe->Write(_T("MB440184"), 1);
//		//pView->IoWrite(_T("MB440184"), 1);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031
//	}
//	else
//		pView->m_pMpe->Write(_T("MB440184"), 0);
//	//pView->IoWrite(_T("MB440184"), 0);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031
//
//	pView->m_pMpe->Write(_T("MB44017A"), (pDoc->WorkingInfo.LastJob.bDualTest) ? 0 : 1);		// 단면 검사 On
//	pView->m_pMpe->Write(_T("MB44017B"), (pDoc->WorkingInfo.LastJob.bSampleTest) ? 1 : 0);		// Sample 검사 On
//	pView->m_pMpe->Write(_T("MB44017D"), (pDoc->WorkingInfo.LastJob.bOneMetal) ? 1 : 0);		// One Metal On
//	pView->m_pMpe->Write(_T("MB44017C"), (pDoc->WorkingInfo.LastJob.bTwoMetal) ? 1 : 0);		// Two Metal On
//	pView->m_pMpe->Write(_T("MB44017E"), (pDoc->WorkingInfo.LastJob.bCore150Recoiler) ? 1 : 0);	// Recoiler Core 150mm On
//	pView->m_pMpe->Write(_T("MB44017F"), (pDoc->WorkingInfo.LastJob.bCore150Uncoiler) ? 1 : 0);	// Uncoiler Core 150mm On
//
//	pView->m_pMpe->Write(_T("MB44010E"), (pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0));
//	pView->m_pMpe->Write(_T("MB44010F"), (pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0));
}

void CGvisR2R_LaserView::GetPlcParam()
{
	if (!m_pMpe)
		return;
#ifdef USE_MPE
	// Main
	pDoc->BtnStatus.Main.Ready = m_pMpe->Read(_T("MB005503")) ? TRUE : FALSE;
	pDoc->BtnStatus.Main.Run = m_pMpe->Read(_T("MB005501")) ? TRUE : FALSE;
	pDoc->BtnStatus.Main.Reset = m_pMpe->Read(_T("MB005504")) ? TRUE : FALSE;
	pDoc->BtnStatus.Main.Stop = m_pMpe->Read(_T("MB005502")) ? TRUE : FALSE;
	pDoc->BtnStatus.Main.Auto = m_pMpe->Read(_T("MB005505")) ? TRUE : FALSE;	// 마킹부 자동/수동 (ON)
	pDoc->BtnStatus.Main.Manual = m_pMpe->Read(_T("MB005505")) ? FALSE : TRUE;	// 마킹부 자동/수동 (OFF)

																				// TorqueMotor
	pDoc->BtnStatus.Tq.Mk = m_pMpe->Read(_T("MB440155")) ? TRUE : FALSE;
	pDoc->BtnStatus.Tq.Aoi = m_pMpe->Read(_T("MB440156")) ? TRUE : FALSE;
	pDoc->BtnStatus.Tq.Eng = m_pMpe->Read(_T("MB440154")) ? TRUE : FALSE;

	// InductionMotor
	pDoc->BtnStatus.Induct.Rc = m_pMpe->Read(_T("MB44017D")) ? TRUE : FALSE;	//pView->SetTwoMetal(FALSE, TRUE);	// One Metal IDC_CHK_68		
	pDoc->BtnStatus.Induct.Uc = m_pMpe->Read(_T("MB44017C")) ? TRUE : FALSE;	//pView->SetTwoMetal(TRUE, TRUE);	// Two Metal IDC_CHK_69

																				// Core150mm
	pDoc->BtnStatus.Core150.Rc = m_pMpe->Read(_T("MB44017E")) ? TRUE : FALSE;	// SetCore150mmRecoiler(TRUE);	// Recoiler IDC_CHK_70	
	pDoc->BtnStatus.Core150.Uc = m_pMpe->Read(_T("MB44017F")) ? TRUE : FALSE;	// SetCore150mmUncoiler(TRUE);	// Uncoiler IDC_CHK_71	

																				// Etc
																				//pDoc->BtnStatus.Etc.EmgAoi = m_pMpe->Read(_T("")) ? TRUE : FALSE;

																				// Recoiler
	pDoc->BtnStatus.Rc.Relation = m_pMpe->Read(_T("MB005801")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.FdCw = m_pMpe->Read(_T("MB00580C")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.FdCcw = m_pMpe->Read(_T("MB00580D")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.ReelChuck = m_pMpe->Read(_T("MB00580B")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.DcRlUpDn = m_pMpe->Read(_T("MB005802")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.ReelJoinL = m_pMpe->Read(_T("MB005805")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.ReelJoinR = m_pMpe->Read(_T("MB005806")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.ReelJoinVac = m_pMpe->Read(_T("MB00580F")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.PprChuck = m_pMpe->Read(_T("MB005808")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.PprCw = m_pMpe->Read(_T("MB005809")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.PprCcw = m_pMpe->Read(_T("MB00580A")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.Rewine = m_pMpe->Read(_T("MB005803")) ? TRUE : FALSE;
	pDoc->BtnStatus.Rc.RewineReelPpr = m_pMpe->Read(_T("MB005804")) ? TRUE : FALSE;

	// Punch
	pDoc->BtnStatus.Mk.Relation = m_pMpe->Read(_T("MB005511")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.FdCw = m_pMpe->Read(_T("MB005513")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.FdCcw = m_pMpe->Read(_T("MB005514")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.FdVac = m_pMpe->Read(_T("MB005515")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.PushUp = m_pMpe->Read(_T("MB005516")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.TblBlw = m_pMpe->Read(_T("MB005512")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.TblVac = m_pMpe->Read(_T("MB005517")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.FdClp = m_pMpe->Read(_T("MB005519")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.TqClp = m_pMpe->Read(_T("MB00551A")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.MvOne = m_pMpe->Read(_T("MB440151")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.LsrPt = m_pMpe->Read(_T("MB005518")) ? TRUE : FALSE;
	pDoc->BtnStatus.Mk.DcRSol = m_pMpe->Read(_T("MB00551B")) ? TRUE : FALSE;

	// AOIDn
	pDoc->BtnStatus.AoiDn.Relation = m_pMpe->Read(_T("MB005701")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.FdCw = m_pMpe->Read(_T("MB005703")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.FdCcw = m_pMpe->Read(_T("MB005704")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.FdVac = m_pMpe->Read(_T("MB005705")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.PushUp = m_pMpe->Read(_T("MB005706")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.TblBlw = m_pMpe->Read(_T("MB005702")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.TblVac = m_pMpe->Read(_T("MB005707")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.FdClp = m_pMpe->Read(_T("MB005709")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.TqClp = m_pMpe->Read(_T("MB00570A")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.MvOne = m_pMpe->Read(_T("MB440151")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.LsrPt = m_pMpe->Read(_T("MB005708")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiDn.VelSonicBlw = m_pMpe->Read(_T("MB44014F")) ? TRUE : FALSE;

	// AOIUp
	pDoc->BtnStatus.AoiUp.Relation = m_pMpe->Read(_T("MB005601")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.FdCw = m_pMpe->Read(_T("MB005603")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.FdCcw = m_pMpe->Read(_T("MB005604")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.FdVac = m_pMpe->Read(_T("MB005605")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.PushUp = m_pMpe->Read(_T("MB005606")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.TblBlw = m_pMpe->Read(_T("MB005602")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.TblVac = m_pMpe->Read(_T("MB005607")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.FdClp = m_pMpe->Read(_T("MB005609")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.TqClp = m_pMpe->Read(_T("MB00560A")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.MvOne = m_pMpe->Read(_T("MB440151")) ? TRUE : FALSE;
	pDoc->BtnStatus.AoiUp.LsrPt = m_pMpe->Read(_T("MB005608")) ? TRUE : FALSE;

	// Engrave
	//pDoc->BtnStatus.Eng.Relation = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.FdCw = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.FdCcw = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.FdVac = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.PushUp = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.TblBlw = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.TblVac = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.FdClp = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.TqClp = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	pDoc->BtnStatus.Eng.MvOne = m_pMpe->Read(_T("MB440151")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.LsrPt = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	//pDoc->BtnStatus.Eng.DcRSol = m_pMpe->Read(_T("")) ? TRUE : FALSE;
	pDoc->BtnStatus.Eng.VelSonicBlw = m_pMpe->Read(_T("MB44014E")) ? TRUE : FALSE;

	// Uncoiler
	pDoc->BtnStatus.Uc.Relation = m_pMpe->Read(_T("MB005401")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.FdCw = m_pMpe->Read(_T("MB00540C")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.FdCcw = m_pMpe->Read(_T("MB00540D")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.ReelChuck = m_pMpe->Read(_T("MB00540B")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.DcRlUpDn = m_pMpe->Read(_T("MB005402")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.ReelJoinL = m_pMpe->Read(_T("MB005405")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.ReelJoinR = m_pMpe->Read(_T("MB005406")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.ReelJoinVac = m_pMpe->Read(_T("MB00540F")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.PprChuck = m_pMpe->Read(_T("MB005408")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.PprCw = m_pMpe->Read(_T("MB005409")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.PprCcw = m_pMpe->Read(_T("MB00540A")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.ClRlUpDn = m_pMpe->Read(_T("MB005403")) ? TRUE : FALSE;
	pDoc->BtnStatus.Uc.ClRlPshUpDn = m_pMpe->Read(_T("MB005404")) ? TRUE : FALSE;
#endif
}

void CGvisR2R_LaserView::InitIoWrite()
{
//	//IoWrite(_T("MB44015E"), 0); // 부저1 On  (PC가 ON, OFF) - 20141020
//	pView->m_pMpe->Write(_T("MB44015E"), 0);
//	//IoWrite(_T("MB44015F"), 0); // 부저2 On  (PC가 ON, OFF) - 20141020
//	pView->m_pMpe->Write(_T("MB44015F"), 0);
//
//	//IoWrite(_T("MB003828"), 0); // 검사부 상 검사 시작 <-> Y4368 I/F
//	pView->m_pMpe->Write(_T("MB003828"), 0);
//	//IoWrite(_T("MB003829"), 0); // 검사부 상 검사 테이블 진공 SOL <-> Y4369 I/F
//	pView->m_pMpe->Write(_T("MB003829"), 0);
//	//IoWrite(_T("MB00382A"), 0); // 검사부 상 Reset <-> Y436A I/F
//	pView->m_pMpe->Write(_T("MB00382A"), 0);
//	//IoWrite(_T("MB00382B"), 0); // 마킹부 Lot End <-> Y436B I/F
//	pView->m_pMpe->Write(_T("MB00382B"), 0);
//
//	//IoWrite(_T("MB003928"), 0); // 검사부 하 검사 시작 <-> Y4468 I/F
//	pView->m_pMpe->Write(_T("MB003928"), 0);
//	//IoWrite(_T("MB003929"), 0); // 검사부 하 검사 테이블 진공 SOL <-> Y4369 I/F
//	pView->m_pMpe->Write(_T("MB003929"), 0);
//	//IoWrite(_T("MB00392A"), 0); // 검사부 하 Reset <-> Y436A I/F
//	pView->m_pMpe->Write(_T("MB00392A"), 0);
//	//IoWrite(_T("MB00392B"), 0); // 마킹부 Lot End <-> Y436B I/F
//	pView->m_pMpe->Write(_T("MB00392B"), 0);
//
//	//IoWrite(_T("MB44015D"), 0); // 자동 초기 운전상태(PC가 On/Off 시킴, PLC가 운전램프를 윙크동작, on->off시 운전램프 on, 다시 운전스위치가 눌러지면 off) - 20141017
//	pView->m_pMpe->Write(_T("MB44015D"), 0);
//	//IoWrite(_T("ML45064"), 0); // 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)
//	pView->m_pMpe->Write(_T("ML45064"), 0);
//	//IoWrite(_T("ML45066"), 0); // 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)
//	pView->m_pMpe->Write(_T("ML45066"), 0);
//
//	//IoWrite(_T("MB600000"), 0); // PC가 PLC의 Alarm 발생여부를 확인
//	pView->m_pMpe->Write(_T("MB600000"), 0);
//	//IoWrite(_T("MB600008"), 0); // PC가 PLC의 Alarm 발생여부를 확인
//	pView->m_pMpe->Write(_T("MB600008"), 0);
//	//IoWrite(_T("ML60002"), 0); // 알람이 발생한 페이지 지정(PLC가 표시 할 알람의 페이지를 가리킴).
//	pView->m_pMpe->Write(_T("ML60002"), 0);
}


BOOL CGvisR2R_LaserView::MoveAlign0(int nPos)
{
	if (!m_pMotion)
		return FALSE;

	if (m_pDlgMenu02)
		m_pDlgMenu02->SetLight();

	if (m_pMotion->m_dPinPosY[0] > -5.0 && m_pMotion->m_dPinPosX[0] > -5.0)
	{
		double dCurrX = m_dEnc[AXIS_X0];
		double dCurrY = m_dEnc[AXIS_Y0];

		double pPos[2];
		if (nPos == 0)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk.X0 + m_pMotion->m_dPinPosX[0];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y0 + m_pMotion->m_dPinPosY[0];
		}
		else if (nPos == 1)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk.X1 + m_pMotion->m_dPinPosX[0];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y1 + m_pMotion->m_dPinPosY[0];
		}

		//if (ChkCollision(AXIS_X0, pPos[0]))
		//	return FALSE;

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
		if (fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
			{
				if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
				{
					pView->ClrDispMsg();
					AfxMessageBox(_T("Error - Move MoveAlign0 ..."));
					return FALSE;
				}
			}
		}

		return TRUE;
	}

	return FALSE;
}

void CGvisR2R_LaserView::SetLastProc()
{

	if (m_pDlgMenu01)
		m_pDlgMenu01->SetLastProc();
}

BOOL CGvisR2R_LaserView::IsLastProc()
{
	BOOL bRtn = FALSE;
	if (m_pDlgMenu01)
		bRtn = m_pDlgMenu01->IsLastProc();
	else
		bRtn = FALSE;

	return bRtn;
}

BOOL CGvisR2R_LaserView::IsLastJob(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
{
	switch (nAoi)
	{
	case 0: // AOI-Up
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerUp.IsEmpty())
			return FALSE;
		break;
	case 1: // AOI-Dn
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() ||
			//pDoc->WorkingInfo.LastJob.sModelDn.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerDn.IsEmpty())
			return FALSE;
		break;
	case 2: // AOI-UpDn
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerUp.IsEmpty())
			return FALSE;
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() ||
			//pDoc->WorkingInfo.LastJob.sModelDn.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerDn.IsEmpty())
			return FALSE;
		break;
	}

	return TRUE;
}

void CGvisR2R_LaserView::MonDispMain()
{
	CString sDisp = pDoc->GetMonDispMain();

	if (sDisp == _T("운전중") && m_sMonDisp != _T("운전중"))
	{		
		DispMain(_T("운전중"), RGB_GREEN);
	}

	if (sDisp == _T("운전준비") && m_sMonDisp != _T("운전준비"))
	{
		DispMain(_T("운전준비"), RGB_GREEN);
	}

	if (sDisp == _T("양면샘플") && m_sMonDisp != _T("양면샘플"))
	{
		DispMain(_T("양면샘플"), RGB_GREEN);
	}

	if (sDisp == _T("양면검사") && m_sMonDisp != _T("양면검사"))
	{
		DispMain(_T("양면검사"), RGB_GREEN);
	}

	if (sDisp == _T("단면검사") && m_sMonDisp != _T("단면검사"))
	{
		DispMain(_T("단면검사"), RGB_GREEN);
	}

	if (sDisp == _T("내층검사") && m_sMonDisp != _T("내층검사"))
	{
		DispMain(_T("내층검사"), RGB_GREEN);
	}

	if (sDisp == _T("외층검사") && m_sMonDisp != _T("외층검사"))
	{
		DispMain(_T("외층검사"), RGB_GREEN);
	}

	if (sDisp == _T("정 지") && m_sMonDisp != _T("정 지"))
	{
		pView->DispStsBar(_T("정지-44"), 0);
		DispMain(_T("정 지"), RGB_RED);
	}


	//if (pDoc->BtnStatus.Disp.Run)		// 운전중(PLC가 PC에 알려주는 설비 상태) - 20141031
	//{
	//	if (m_sDispMain != _T("운전중"))
	//	{
	//		DispMain(_T("운전중"), RGB_GREEN);
	//	}
	//}

	//if (pDoc->BtnStatus.Disp.Ready)		// 운전준비(PLC가 PC에 알려주는 설비 상태) - 20141031
	//{
	//	if (m_sDispMain != _T("운전준비"))
	//	{
	//		DispMain(_T("운전준비"), RGB_GREEN);
	//	}
	//}
	//else
	//{
	//	if (pDoc->BtnStatus.Disp.DualSample)
	//	{
	//		if (m_sDispMain != _T("양면샘플"))
	//		{
	//			DispMain(_T("양면샘플"), RGB_GREEN);
	//		}
	//	}
	//	else if (pDoc->BtnStatus.Disp.DualTest)
	//	{
	//		if (m_sDispMain != _T("양면검사"))
	//		{
	//			DispMain(_T("양면검사"), RGB_GREEN);
	//		}
	//	}
	//	else if (pDoc->BtnStatus.Disp.SingleTest)
	//	{
	//		if (m_sDispMain != _T("단면검사"))
	//		{
	//			DispMain(_T("단면검사"), RGB_GREEN);
	//		}
	//	}
	//	//else
	//	//{
	//	//	if (m_sDispMain != _T("운전준비"))
	//	//	{
	//	//		if (m_sDispMain != _T("정 지"))
	//	//		{
	//	//			pView->DispStsBar(_T("정지-44"), 0);
	//	//			DispMain(_T("정 지"), RGB_RED);
	//	//		}
	//	//	}
	//	//}

	//	if (pDoc->BtnStatus.Disp.Stop)
	//	{
	//		if (m_sDispMain != _T("정 지"))
	//		{
	//			pView->DispStsBar(_T("정지-44"), 0);
	//			DispMain(_T("정 지"), RGB_RED);
	//		}
	//	}
	//}

}

void CGvisR2R_LaserView::MonPlcAlm()
{
	//BOOL bMon, bClr;
	//long lOn = m_pMpe->Read(_T("ML60000"));

	//bMon = lOn & (0x01 << 0);
	//bClr = lOn & (0x01 << 1);
	//if (bMon)
	//	PlcAlm(bMon, 0);
	//else if (bClr)
	//	PlcAlm(0, bClr);
	//else
	//	PlcAlm(bMon, bClr);

	if (!pDoc->m_sAlmMsg.IsEmpty())
	{
		if (pDoc->m_sAlmMsg != pDoc->m_sPrevAlmMsg)
		{
			pDoc->m_sPrevAlmMsg = pDoc->m_sAlmMsg;
			CycleStop();
		}
	}
}

void CGvisR2R_LaserView::MonMsgBox()
{
	if (!pDoc->m_sMsgBox.IsEmpty())
	{
		if (pDoc->m_sMsgBox != pDoc->m_sPrevMsgBox)
		{
			pDoc->m_sPrevMsgBox = pDoc->m_sMsgBox;
			CycleStop();
		}
	}
}

void CGvisR2R_LaserView::PlcAlm(BOOL bMon, BOOL bClr)
{
	//if (bMon && !m_nMonAlmF)
	//{
	//	m_nMonAlmF = 1;
	//	//		ResetMonAlm();
	//	FindAlarm();
	//	m_pMpe->Write(_T("MB600008"), 1);
	//}
	//else if (!bMon && m_nMonAlmF)
	//{
	//	m_nMonAlmF = 0;
	//	ResetMonAlm();
	//}


	//if (bClr && !m_nClrAlmF)
	//{
	//	m_nClrAlmF = 1;
	//	ClrAlarm();
	//	m_pMpe->Write(_T("MB600009"), 1);
	//	//		ResetClear();
	//}
	//else if (!bClr && m_nClrAlmF)
	//{
	//	m_nClrAlmF = 0;
	//	ResetClear();
	//}
}

void CGvisR2R_LaserView::FindAlarm()
{
	//// 알람이 발생한 페이지
	//long lAlmPage = -1;
	//lAlmPage = m_pMpe->Read(_T("ML60002"));

	////char szData[200];
	//TCHAR szData[200];
	//CString str1, str2, str3, strM, str, strH = _T("");

	//str1.Format(_T("%d"), lAlmPage);
	//str2 = _T("Address");
	//if (0 < ::GetPrivateProfileString(str1, str2, NULL, szData, sizeof(szData), PATH_ALARM))
	//	strM = CString(szData);
	//else
	//	strM = _T("");

	//if (strM.IsEmpty())
	//	return;

	//long lAlm = m_pMpe->Read(strM); // lAlm : (32Bits << Row)
	//for (int i = 0; i < 32; i++)
	//{
	//	if (lAlm & (0x01 << i))
	//	{
	//		str3.Format(_T("%d"), i);
	//		if (0 < ::GetPrivateProfileString(str1, str3, NULL, szData, sizeof(szData), PATH_ALARM))
	//			strH = CString(szData);
	//		else
	//			strH = _T("");

	//		if (str.IsEmpty())
	//			str = strH;
	//		else
	//		{
	//			str += _T("\r\n");
	//			str += strH;
	//		}
	//	}
	//}

	//pDoc->m_sAlmMsg = strH;
}

void CGvisR2R_LaserView::ResetMonAlm()
{
	//m_pMpe->Write(_T("MB600008"), 0);
}

void CGvisR2R_LaserView::ClrAlarm()
{
	if (!pDoc->m_sAlmMsg.IsEmpty())
	{
		pDoc->m_sAlmMsg = _T("");
		pDoc->m_sIsAlmMsg = _T("");
		pDoc->m_sPrevAlmMsg = _T("");
	}
}

void CGvisR2R_LaserView::ResetClear()
{
	//m_pMpe->Write(_T("MB600009"), 0);
}

void CGvisR2R_LaserView::ChkTempStop(BOOL bChk)
{
	if (bChk)
	{
		if (!m_bTIM_CHK_TEMP_STOP)
		{
			m_bTIM_CHK_TEMP_STOP = TRUE;
			SetTimer(TIM_CHK_TEMP_STOP, 100, NULL);
		}
	}
	else
	{
		m_bTIM_CHK_TEMP_STOP = FALSE;
	}
}

void CGvisR2R_LaserView::ChgLot()
{
	pDoc->WorkingInfo.LastJob.sLotUp = pDoc->Status.PcrShare[0].sLot;
	pDoc->SetModelInfoUp();

	pDoc->WorkingInfo.LastJob.sLotDn = pDoc->Status.PcrShare[1].sLot;
	pDoc->SetModelInfoDn();

	//	pDoc->m_bDoneChgLot = TRUE;

	SetPathAtBuf();
}

void CGvisR2R_LaserView::LoadPcrFromBuf()
{
	return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString str, sTemp;

	if (ChkBufUp(m_pBufSerial[0], m_nBufTot[0]))
	{
		for (int i = 0; i < m_nBufTot[0]; i++)
		{
			LoadPcrUp(m_pBufSerial[0][i]);
			if (!bDualTest)
				UpdateReelmap(m_pBufSerial[0][i]);
		}
	}

	if (bDualTest)
	{
		if (ChkBufDn(m_pBufSerial[1], m_nBufTot[1]))
		{
			for (int i = 0; i < m_nBufTot[1]; i++)
			{
				LoadPcrDn(m_pBufSerial[1][i]);
				UpdateReelmap(m_pBufSerial[1][i]); // After inspect bottom side.
			}
		}
	}
}

void CGvisR2R_LaserView::SetPathAtBuf()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetPathAtBuf();
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->SetPathAtBuf();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->SetPathAtBuf();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->SetPathAtBuf();
	}

	//if (pDoc->m_pReelMap)
	//{
	//	if (pDoc->m_pReelMap->m_nLayer < 0)
	//		pDoc->m_pReelMap->m_nLayer = pView->m_nSelRmap;

	//	pDoc->m_pReelMap->SetPathAtBuf();
	//}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		//SetInnerPathAtBuf();
		if (pDoc->m_pReelMapIts)
			pDoc->m_pReelMapIts->SetPathAtBuf();
	}
}

void CGvisR2R_LaserView::SetPathAtBufUp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetPathAtBuf();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->SetPathAtBuf();
	}

	//if (pDoc->m_pReelMap)
	//{
	//	if (pDoc->m_pReelMap->m_nLayer < 0)
	//		pDoc->m_pReelMap->m_nLayer = pView->m_nSelRmap;

	//	if (pDoc->m_pReelMap->m_nLayer == RMAP_UP || pDoc->m_pReelMap->m_nLayer == RMAP_ALLUP)
	//		pDoc->m_pReelMap->SetPathAtBuf();
	//}

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//{
	//	SetInnerPathAtBufUp();
	//}
}

void CGvisR2R_LaserView::SetPathAtBufDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	if (pDoc->m_pReelMapDn)
		pDoc->m_pReelMapDn->SetPathAtBuf();
	if (pDoc->m_pReelMapAllDn)
		pDoc->m_pReelMapAllDn->SetPathAtBuf();

	//if (pDoc->m_pReelMap)
	//{
	//	if (pDoc->m_pReelMap->m_nLayer < 0)
	//		pDoc->m_pReelMap->m_nLayer = pView->m_nSelRmap;

	//	if (pDoc->m_pReelMap->m_nLayer == RMAP_DN || pDoc->m_pReelMap->m_nLayer == RMAP_ALLDN)
	//		pDoc->m_pReelMap->SetPathAtBuf();
	//}

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//{
	//	SetInnerPathAtBufDn();
	//}
}


void  CGvisR2R_LaserView::SetLotLastShot()
{
	pDoc->m_nLotLastShot = pDoc->m_nBufLastShot = int(_tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0 / _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen));
}

BOOL CGvisR2R_LaserView::IsMkStrip(int nStripIdx)
{
	if (!m_pDlgMenu01 || nStripIdx < 1 || nStripIdx > MAX_STRIP_NUM)
		return TRUE;

	return (m_pDlgMenu01->GetChkStrip(nStripIdx - 1));
}

void CGvisR2R_LaserView::CycleStop()
{
	m_bCycleStop = TRUE;
}

BOOL CGvisR2R_LaserView::ChkLotCutPos()
{
	if (pDoc->WorkingInfo.LastJob.bLotSep && pDoc->m_bDoneChgLot)
	{
		double dFdTotLen = GetMkFdLen();
		double dLotCutPos = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen)*1000.0;
		if (dFdTotLen >= dLotCutPos)
		{
			pDoc->WorkingInfo.LastJob.bLotSep = FALSE;
#ifdef USE_MPE
			pView->m_pMpe->Write(_T("MB440184"), 0);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031
														//pView->IoWrite(_T("MB440184"), 1);
#endif
			if (pDoc->m_pReelMap)
				pDoc->m_pReelMap->m_bUseLotSep = FALSE;

			::WritePrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), _T("0"), PATH_WORKING_INFO);

			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::ChkStShotNum()
{
	CString sMsg;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_ListBuf[0].nTot == 0)
	{
		if (m_nShareUpS > 0 && !(m_nShareUpS % 2))
		{
			sMsg.Format(_T("AOI 상면의 시리얼이 짝수로 시작하였습니다.\r\n- 시리얼 번호: %d"), m_nShareUpS);
			//MyMsgBox(sMsg);
			MsgBox(sMsg);
			return FALSE;
		}
	}

	if (bDualTest)
	{
		if (pDoc->m_ListBuf[1].nTot == 0)
		{
			if (m_nShareDnS > 0 && !(m_nShareDnS % 2))
			{
				sMsg.Format(_T("AOI 하면의 시리얼이 짝수로 시작하였습니다.\r\n- 시리얼 번호: %d"), m_nShareDnS);
				//MyMsgBox(sMsg);
				MsgBox(sMsg);
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_LaserView::ChkContShotNum()
{
	CString sMsg;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!m_pDlgFrameHigh)
		return FALSE;

	if (pDoc->m_ListBuf[0].nTot == 0)
	{
		if (m_nShareUpS > 0 && m_pDlgFrameHigh->m_nMkLastShot + 1 != m_nShareUpS)
		{
			sMsg.Format(_T("AOI 상면의 시작Shot(%d)이 마지막Shot(%d)과 불연속입니다.\r\n계속 진행하시겠습니까?"), m_nShareUpS, m_pDlgFrameHigh->m_nMkLastShot);
			//if(IDNO==DoMyMsgBox(sMsg, MB_YESNO))
			if (IDNO == MsgBox(sMsg, 0, MB_YESNO))
				return FALSE;
		}
	}

	return TRUE;
}

void CGvisR2R_LaserView::SetFixPcs(int nSerial)
{
	return;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetFixPcs(nSerial);
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->SetFixPcs(nSerial);
	}
}

void CGvisR2R_LaserView::MakeResultMDS()
{
	CString sPath, strMsg;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	pDoc->WorkingInfo.LastJob.sProcessNum = pDoc->GetProcessNum();
	pDoc->UpdateProcessNum(pDoc->WorkingInfo.LastJob.sProcessNum);

	if (m_pDlgMenu05)
	{
		m_pDlgMenu05->m_sModel = pDoc->WorkingInfo.LastJob.sModelUp;
		m_pDlgMenu05->m_sLot = pDoc->WorkingInfo.LastJob.sLotUp;
		m_pDlgMenu05->m_sLayer = pDoc->WorkingInfo.LastJob.sLayerUp;
		m_pDlgMenu05->m_sProcessNum = pDoc->WorkingInfo.LastJob.sProcessNum;
		sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp);

		if (bDualTest)
			m_pDlgMenu05->m_sRmapPath.Format(_T("%s\\ReelMapDataAll.txt"), sPath);
		else
			m_pDlgMenu05->m_sRmapPath.Format(_T("%s\\ReelMapDataUp.txt"), sPath);

		m_pDlgMenu05->MakeResult();

		MakeResult();
		MakeSapp3();
	}
	else
	{
		strMsg.Format(_T("It is trouble to open file.\r\n%s"), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK);
	}

	RemakeReelmap();
}

void CGvisR2R_LaserView::MakeResult()
{
	// TODO: Add your control notification handler code here

	// File Save......
	CString strDestPath;
	strDestPath.Format(_T("%s%s\\%s\\%s.txt"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		_T("Result"));

	CFileFind cFile;
	if (cFile.FindFile(strDestPath))
		DeleteFile(strDestPath);

	//////////////////////////////////////////////////////////
	// Directory location of Work file
	CString strData;
	strData = m_pDlgMenu05->TxtDataMDS();
	//char lpszCurDirPathFile[MAX_PATH];
	//strcpy(lpszCurDirPathFile, strDestPath);
	TCHAR lpszCurDirPathFile[MAX_PATH];
	_stprintf(lpszCurDirPathFile, _T("%s"), strDestPath);

	CFile file;
	CFileException pError;
	if (!file.Open(lpszCurDirPathFile, CFile::modeWrite, &pError))
	{
		if (!file.Open(lpszCurDirPathFile, CFile::modeCreate | CFile::modeWrite, &pError))
		{
			// 파일 오픈에 실패시 
#ifdef _DEBUG
			afxDump << _T("File could not be opened ") << pError.m_cause << _T("\n");
#endif
			return;
		}
	}
	//버퍼의 내용을 file에 복사한다.
	char* pRtn = NULL;
	file.SeekToBegin();
	file.Write(pRtn = StringToChar(strData), strData.GetLength());
	file.Close();
	if (pRtn)
		delete pRtn;
}

void CGvisR2R_LaserView::MakeSapp3()
{
	if (pDoc->WorkingInfo.LastJob.sProcessNum.IsEmpty() || pDoc->WorkingInfo.System.sPathSapp3.IsEmpty())
		return;

	FILE *fp = NULL;
	//char FileName[MAX_PATH];
	TCHAR FileName[MAX_PATH];
	CString sPath, sVal, strMsg;
	char* pRtn = NULL;

	sPath.Format(_T("%s%9s_%4s_%5s.txt"), pDoc->WorkingInfo.System.sPathSapp3,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sProcessNum,
		pDoc->WorkingInfo.System.sMcName);
	//strcpy(FileName, sPath);
	_stprintf(FileName, _T("%s"), sPath);

	fp = fopen(pRtn = TCHARToChar(FileName), "w+");
	if(pRtn) delete pRtn; 
	pRtn = NULL;

	if (fp != NULL)
	{
		fprintf( fp, "%s\n", pRtn = StringToChar(m_pDlgMenu05->Sapp3Data()) );
	}
	else
	{
		strMsg.Format(_T("It is trouble to open file.\r\n%s"), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK);
	}

	if(pRtn)
		delete pRtn;
	fclose(fp);
}

BOOL CGvisR2R_LaserView::RemakeReelmap()
{
	return TRUE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sReelmapSrc, str;

	str = _T("ReelMapDataUp.txt");
	sReelmapSrc.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		str);
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->RemakeReelmap(sReelmapSrc);

	if (bDualTest)
	{
		str = _T("ReelMapDataDn.txt");
		sReelmapSrc.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			//pDoc->WorkingInfo.LastJob.sModelDn,
			//pDoc->WorkingInfo.LastJob.sLotDn,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->RemakeReelmap(sReelmapSrc);

		str = _T("ReelMapDataAll.txt");
		sReelmapSrc.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->RemakeReelmap(sReelmapSrc);
	}

	return TRUE;
}

BOOL CGvisR2R_LaserView::IsDoneRemakeReelmap()
{
	return TRUE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bSuccess[3] = { FALSE };
	DWORD dwSt = GetTickCount();

	do
	{
		if (GetTickCount() - dwSt > 600000)
			break;
	} while (!pDoc->m_pReelMapUp->m_bThreadAliveRemakeReelmap && !pDoc->m_pReelMapDn->m_bThreadAliveRemakeReelmap && !pDoc->m_pReelMapDn->m_bThreadAliveRemakeReelmap);

	if (bDualTest)
	{
		bSuccess[0] = pDoc->m_pReelMapUp->m_bRtnThreadRemakeReelmap;
		bSuccess[1] = pDoc->m_pReelMapDn->m_bRtnThreadRemakeReelmap;
		bSuccess[2] = pDoc->m_pReelMapAllUp->m_bRtnThreadRemakeReelmap;

		if (!bSuccess[0] || !bSuccess[2] || !bSuccess[1])
		{
			MsgBox(_T("ReelMap Converting Failed."));
			return FALSE;
		}
	}
	else
	{
		if (!pDoc->m_pReelMapUp->m_bRtnThreadRemakeReelmap)
		{
			MsgBox(_T("ReelMap Converting Failed."));
			return FALSE;
		}
	}

	return TRUE;
}

//BOOL CGvisR2R_LaserView::MoveMeasPos(int nId)
//{
//	if (!m_pMotion)
//		return FALSE;
//
//	if (!m_pLight)
//		return FALSE;
//
//	if (nId == 0)
//	{
//		if (m_pDlgMenu02)
//			m_pDlgMenu02->SetLight();
//
//		double dMkOffsetX, dMkOffsetY;
//		if (pDoc->WorkingInfo.Vision[0].sMkOffsetX.IsEmpty())
//			dMkOffsetX = 0.0;
//		else
//			dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);
//
//		if (pDoc->WorkingInfo.Vision[0].sMkOffsetY.IsEmpty())
//			dMkOffsetY = 0.0;
//		else
//			dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);
//
//
//		double pPos[2];
//		if (m_bFailAlign[0][0] || m_bFailAlign[0][1])
//		{
//			pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX) + dMkOffsetX;
//			pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY) + dMkOffsetY;
//		}
//		else
//		{
//			pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX) + dMkOffsetX - m_pDlgMenu02->m_dMkFdOffsetX[0];
//			pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY) + dMkOffsetY - m_pDlgMenu02->m_dMkFdOffsetY[0];
//
//			// 			pPos[0] = pDoc->WorkingInfo.Fluck.dMeasPosX[0] + dMkOffsetX;
//			// 			pPos[1] = pDoc->WorkingInfo.Fluck.dMeasPosY[0] + dMkOffsetY;
//		}
//
//		if (pPos[1] > 0.0 && pPos[0] > 0.0)
//		{
//			double dCurrX = m_dEnc[AXIS_X0];
//			double dCurrY = m_dEnc[AXIS_Y0];
//
//			double fLen, fVel, fAcc, fJerk;
//			fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
//			if (fLen > 0.001)
//			{
//				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
//				if (!m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
//				{
//					if (!m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
//						AfxMessageBox(_T("Move XY Error..."));
//				}
//			}
//
//			return TRUE;
//		}
//	}
//	else if (nId == 1)
//	{
//		if (m_pDlgMenu02)
//			m_pDlgMenu02->SetLight2();
//
//		double dMkOffsetX, dMkOffsetY;
//		if (pDoc->WorkingInfo.Vision[1].sMkOffsetX.IsEmpty())
//			dMkOffsetX = 0.0;
//		else
//			dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX);
//
//		if (pDoc->WorkingInfo.Vision[1].sMkOffsetY.IsEmpty())
//			dMkOffsetY = 0.0;
//		else
//			dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY);
//
//
//		double pPos[2];
//		if (m_bFailAlign[1][0] || m_bFailAlign[1][1])
//		{
//			pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX) + dMkOffsetX;
//			pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY) + dMkOffsetY;
//		}
//		else
//		{
//			pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX) + dMkOffsetX - m_pDlgMenu02->m_dMkFdOffsetX[1];
//			pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY) + dMkOffsetY - m_pDlgMenu02->m_dMkFdOffsetY[1];
//
//			// 			pPos[0] = pDoc->WorkingInfo.Fluck.dMeasPosX[1] + dMkOffsetX;
//			// 			pPos[1] = pDoc->WorkingInfo.Fluck.dMeasPosY[1] + dMkOffsetY;
//		}
//
//		if (pPos[1] > 0.0 && pPos[0] > 0.0)
//		{
//			double dCurrX = m_dEnc[AXIS_X1];
//			double dCurrY = m_dEnc[AXIS_Y1];
//
//			if (ChkCollision(AXIS_X1, pPos[0]))
//			{
//				CfPoint ptPnt;
//				ptPnt.x = 0.0;
//				ptPnt.y = 0.0;//m_dEnc[AXIS_Y0];
//				Move0(ptPnt);
//			}
//
//			double fLen, fVel, fAcc, fJerk;
//			fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
//			if (fLen > 0.001)
//			{
//				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
//				if (!m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
//				{
//					if (!m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
//						AfxMessageBox(_T("Move XY Error..."));
//				}
//			}
//
//			return TRUE;
//		}
//	}
//
//	return FALSE;
//}

void CGvisR2R_LaserView::SetReject()
{
	CfPoint ptPnt;
	//int nSerial, nTot;

	if (m_bDoMk[0])
	{
		if (!m_bTHREAD_MK[2])
		{
			m_bDoneMk[0] = FALSE;
			m_nStepMk[2] = 0;
			m_nMkPcs[2] = 0;
			m_bTHREAD_MK[2] = TRUE;
		}
	}

	if (m_bDoMk[1])
	{
		if (!m_bTHREAD_MK[3])
		{
			m_bDoneMk[1] = FALSE;
			m_nStepMk[3] = 0;
			m_nMkPcs[3] = 0;
			m_bTHREAD_MK[3] = TRUE;
		}
	}
}

void CGvisR2R_LaserView::DoInterlock()
{
	//if (m_dEnc[AXIS_Y0] < 20.0 )//&& m_dEnc[AXIS_Y1] < 20.0)
	//{
	//	if (m_bStopFeeding)
	//	{
	//		m_bStopFeeding = FALSE;
	//		m_pMpe->Write(_T("MB440115"), 0); // 마킹부Feeding금지
	//	}
	//}
	//else
	//{
	//	if (!m_bStopFeeding)
	//	{
	//		m_bStopFeeding = TRUE;
	//		m_pMpe->Write(_T("MB440115"), 1); // 마킹부Feeding금지
	//	}
	//}
}

BOOL CGvisR2R_LaserView::ChkLightErr()
{
	int nSerial, nErrCode;
	BOOL bError = FALSE;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
		nSerial = m_nBufDnSerial[0];//GetBuffer0();
	else
		nSerial = m_nBufUpSerial[0];//GetBuffer0();

	if (nSerial > 0)
	{
		if ((nErrCode = GetErrCode0(nSerial)) != 1)
		{
			if (nErrCode == -1) // -1(Align Error, 노광불량)
				bError = TRUE;
		}
	}

	if (bDualTest)
		nSerial = m_nBufDnSerial[1];//GetBuffer0();
	else
		nSerial = m_nBufUpSerial[1];//GetBuffer0();

	if (nSerial > 0)
	{
		if ((nErrCode = GetErrCode1(nSerial)) != 1)
		{
			if (nErrCode == -1) // -1(Align Error, 노광불량)
				bError = TRUE;
		}
	}

	if (bError)
	{
		Stop();
		TowerLamp(RGB_RED, TRUE);
		Buzzer(TRUE, 0);
		// 		m_bSwStopNow = TRUE;
		// 		m_bSwRunF = FALSE;
		pView->DispStsBar(_T("정지-45"), 0);
		DispMain(_T("정 지"), RGB_RED);
		//MsgBox(_T("노광불량 정지 - 기판을 확인하세요.\r\n계속진행하려면 운전스위치를 누르세요.");
		//AfxMessageBox(_T("노광불량 정지 - 기판을 확인하세요.\r\n계속진행하려면 운전스위치를 누르세요."));
	}

	return bError;
}

void CGvisR2R_LaserView::CntMk()
{
#ifdef USE_MPE
	if (m_nPrevTotMk[0] != m_nTotMk[0])
	{
		m_nPrevTotMk[0] = m_nTotMk[0];
		pView->m_pMpe->Write(_T("ML45096"), (long)m_nTotMk[0]);	// 마킹부 (좌) 총 마킹수 
	}
	if (m_nPrevCurMk[0] != m_nMkPcs[0])//m_nCurMk[0])
	{
		m_nPrevCurMk[0] = m_nMkPcs[0];//m_nCurMk[0];
		pView->m_pMpe->Write(_T("ML45098"), (long)m_nMkPcs[0]);	// 마킹부 (좌) 현재 마킹한 수
	}

	if (m_nPrevTotMk[1] != m_nTotMk[1])
	{
		m_nPrevTotMk[1] = m_nTotMk[1];
		pView->m_pMpe->Write(_T("ML45100"), (long)m_nTotMk[1]);	// 마킹부 (우) 총 마킹수 
	}
	if (m_nPrevCurMk[1] != m_nMkPcs[1])//m_nCurMk[1])
	{
		m_nPrevCurMk[1] = m_nMkPcs[1];//m_nCurMk[1];
		pView->m_pMpe->Write(_T("ML45102"), (long)m_nMkPcs[1]);	// 마킹부 (우) 현재 마킹한 수
	}
#endif
}

BOOL CGvisR2R_LaserView::IsOnMarking0()
{
	if (m_nMkPcs[0] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		return TRUE;

	return FALSE;
}

BOOL CGvisR2R_LaserView::IsOnMarking1()
{
	if (m_nMkPcs[1] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		return TRUE;

	return FALSE;
}

void CGvisR2R_LaserView::SetDualTest(BOOL bOn)
{
#ifdef USE_MPE
	if (pView->m_pMpe)
		pView->m_pMpe->Write(_T("MB44017A"), bOn ? 0 : 1);		// 단면 검사 On
#endif
	if (m_pDlgFrameHigh)
		m_pDlgFrameHigh->SetDualTest(bOn);
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetDualTest(bOn);
	if (m_pDlgMenu03)
		m_pDlgMenu03->SetDualTest(bOn);
}

void CGvisR2R_LaserView::SetTwoMetal(BOOL bSel, BOOL bOn)
{
	if (bSel)
	{
		if (bOn)
		{
			pDoc->WorkingInfo.LastJob.bTwoMetal = TRUE;
			m_pMpe->Write(_T("MB44017C"), 1);
			::WritePrivateProfileString(_T("Last Job"), _T("Two Metal On"), _T("1"), PATH_WORKING_INFO);// IDC_CHK_TWO_METAL - Uncoiler\r역방향 ON : TRUE	
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bTwoMetal = FALSE;
			m_pMpe->Write(_T("MB44017C"), 0);
			::WritePrivateProfileString(_T("Last Job"), _T("Two Metal On"), _T("0"), PATH_WORKING_INFO);// IDC_CHK_TWO_METAL - Uncoiler\r정방향 ON : TRUE	
		}

//#ifdef USE_ENGRAVE
//		if (pView && pView->m_pEngrave)
//			pView->m_pEngrave->SetUncoilerCcw();	//_stSigInx::_UncoilerCcw
//#endif
	}
	else
	{
		if (bOn)
		{
			pDoc->WorkingInfo.LastJob.bOneMetal = TRUE;
			m_pMpe->Write(_T("MB44017D"), 1);
			::WritePrivateProfileString(_T("Last Job"), _T("One Metal On"), _T("1"), PATH_WORKING_INFO);// IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bOneMetal = FALSE;
			m_pMpe->Write(_T("MB44017D"), 0);
			::WritePrivateProfileString(_T("Last Job"), _T("One Metal On"), _T("0"), PATH_WORKING_INFO);// IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
		}

//#ifdef USE_ENGRAVE
//		if (pView && pView->m_pEngrave)
//			pView->m_pEngrave->SetRecoilerCcw();	//_stSigInx::_RecoilerCcw
//#endif
	}
}

void CGvisR2R_LaserView::RestoreReelmap()
{
	return;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->RestoreReelmap();
}

void CGvisR2R_LaserView::AdjPinPos()
{
	if (m_pDlgMenu02)
	{
		if (m_pDlgMenu02->m_dMkFdOffsetY[0][0] > -2.0 &&
			m_pDlgMenu02->m_dMkFdOffsetY[0][0] < 2.0)
		{
			double dOffsetY = -1.0*(m_pDlgMenu02->m_dMkFdOffsetY[0][0]);// +m_pDlgMenu02->m_dMkFdOffsetY[1][0]) / 2.0;
			dOffsetY *= pDoc->m_dShiftAdjustRatio;

			CfPoint ptPnt[2];
			ptPnt[0].x = _tstof(pDoc->WorkingInfo.Motion.sPinPosX[0]);
			ptPnt[0].y = _tstof(pDoc->WorkingInfo.Motion.sPinPosY[0]) + dOffsetY;
			//ptPnt[1].x = _tstof(pDoc->WorkingInfo.Motion.sPinPosX[1]);
			//ptPnt[1].y = _tstof(pDoc->WorkingInfo.Motion.sPinPosY[1]) + dOffsetY;

			m_pDlgMenu02->SetPinPos(0, ptPnt[0]);
			//m_pDlgMenu02->SetPinPos(1, ptPnt[1]);

			//CString sData, sPath = PATH_WORKING_INFO;
			//pDoc->WorkingInfo.Fluck.dMeasPosY[0] = _tstof(pDoc->WorkingInfo.Probing[0].sMeasurePosY) + dOffsetY;
			//sData.Format(_T("%.2f"), pDoc->WorkingInfo.Fluck.dMeasPosY[0]);
			//pDoc->WorkingInfo.Probing[0].sMeasurePosY = sData;
			//::WritePrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSY"), sData, sPath);

			//pDoc->WorkingInfo.Fluck.dMeasPosY[1] = _tstof(pDoc->WorkingInfo.Probing[1].sMeasurePosY) + dOffsetY;
			//sData.Format(_T("%.2f"), pDoc->WorkingInfo.Fluck.dMeasPosY[1]);
			//pDoc->WorkingInfo.Probing[1].sMeasurePosY = sData;
			//::WritePrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSY"), sData, sPath);
		}
	}
}



//void CGvisR2R_LaserView::StartClient_SR1000W(CString sAddrCli, CString sAddrSvr, CString sPortSvr)
//{
//	if (!m_pClient_SR1000W)
//	{
//		m_strAddrCli_SR1000W = sAddrCli;
//		m_strAddrSvr_SR1000W = sAddrSvr;
//		m_strPortSvr_SR1000W = sPortSvr;
//
//		m_pClient_SR1000W = new CTcpIpClient(this);
//		m_pClient_SR1000W->Init(sAddrCli, sAddrSvr, _tstoi(sPortSvr));
//		m_pClient_SR1000W->Start();
//		m_pClient_SR1000W->SetServer(m_nServerID_SR1000W);
//	}
//}
//
//void CGvisR2R_LaserView::StopClient_SR1000W()
//{
//	if (m_pClient_SR1000W)
//	{
//		if (!m_pClient_SR1000W->Stop()) // Called Destroy Function.
//		{
//			Sleep(30);
//			delete m_pClient_SR1000W;
//		}
//		m_pClient_SR1000W = NULL;
//	}
//}


//void CGvisR2R_LaserView::StartClient_MDX2500(CString sAddrCli, CString sAddrSvr, CString sPortSvr)
//{
//	if (!m_pClient_MDX2500)
//	{
//		m_strAddrCli_MDX2500 = sAddrCli;
//		m_strAddrSvr_MDX2500 = sAddrSvr;
//		m_strPortSvr_MDX2500 = sPortSvr;
//
//		m_pClient_MDX2500 = new CTcpIpClient(this);
//		m_pClient_MDX2500->Init(sAddrCli, sAddrSvr, _tstoi(sPortSvr));
//		m_pClient_MDX2500->Start();
//		m_pClient_MDX2500->SetServer(m_nServerID_MDX2500);
//	}
//}
//
//void CGvisR2R_LaserView::StopClient_MDX2500()
//{
//	if (m_pClient_MDX2500)
//	{
//		if (!m_pClient_MDX2500->Stop()) // Called Destroy Function.
//		{
//			Sleep(30);
//			delete m_pClient_MDX2500;
//		}
//		m_pClient_MDX2500 = NULL;
//	}
//}

void CGvisR2R_LaserView::SetEngraveSts(int nStep)
{
	if (!m_pDlgMenu03)
		return;

	// 마킹부 - TBL파기 OFF, TBL진공 ON, FD/TQ 진공 OFF, 
	switch (nStep)
	{
	case 0:
		m_pDlgMenu03->SwMkTblBlw(FALSE);
		m_pDlgMenu03->SwMkFdVac(FALSE);
		m_pDlgMenu03->SwMkTqVac(FALSE);
		break;
	case 1:
		m_pDlgMenu03->SwMkTblVac(TRUE);
		break;
	}
}

void CGvisR2R_LaserView::SetEngraveStopSts()
{
	if (!m_pDlgMenu03)
		return;

	// 마킹부 - FD/TQ 진공 ON, TBL진공 OFF, TBL파기 ON, 
	m_pDlgMenu03->SwMkTblBlw(FALSE);
	m_pDlgMenu03->SwMkTblVac(FALSE);
	m_pDlgMenu03->SwMkFdVac(FALSE);
	// 	m_pDlgMenu03->SwMkTqVac(FALSE);
}

void CGvisR2R_LaserView::SetEngraveFdSts()
{
	if (!m_pDlgMenu03)
		return;

	// 마킹부 - FD/TQ 진공 ON, TBL진공 OFF, TBL파기 ON, 
	m_pDlgMenu03->SwMkFdVac(TRUE);
	// 	m_pDlgMenu03->SwMkTqVac(TRUE);
	m_pDlgMenu03->SwMkTblVac(FALSE);
	m_pDlgMenu03->SwMkTblBlw(TRUE);
}

BOOL CGvisR2R_LaserView::IsEngraveFdSts()
{
	if (!m_pDlgMenu03)
		return FALSE;

	BOOL bOn[4] = { 0 };
	// 마킹부 - FD/TQ 진공 ON, TBL진공 OFF, TBL파기 ON, 
	bOn[0] = m_pDlgMenu03->IsMkFdVac(); // TRUE
	bOn[1] = m_pDlgMenu03->IsMkTqVac(); // TRUE
	bOn[2] = m_pDlgMenu03->IsMkTblVac(); // FALSE
	bOn[3] = m_pDlgMenu03->IsMkTblBlw(); // TRUE

	if (bOn[0] && bOn[1] && !bOn[2] && bOn[3])
		return TRUE;

	return FALSE;
}

void CGvisR2R_LaserView::SetEngraveFd()
{
	CfPoint OfSt;
	GetMkOffset(OfSt);

	if (m_nShareDnCnt > 0)
	{
		if (!(m_nShareDnCnt % 2))
			MoveMk(-1.0*OfSt.x);
	}
	else
	{
		if (m_nShareUpCnt > 0)
		{
			if (!(m_nShareUpCnt % 2))
				MoveMk(-1.0*OfSt.x);
		}
	}
	if (m_pDlgMenu03)
		m_pDlgMenu03->ChkDoneMk();

	if (!pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
#ifdef USE_MPE
		//IoWrite(_T("MB440151"), 1);	// 한판넬 이송상태 ON (PC가 ON, OFF)
		pView->m_pMpe->Write(_T("MB440151"), 1);
#endif
		CString sData, sPath = PATH_WORKING_INFO;
		pDoc->WorkingInfo.LastJob.bMkOnePnl = pDoc->WorkingInfo.LastJob.bAoiOnePnl = TRUE;
		sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bMkOnePnl ? 1 : 0);
		::WritePrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), sData, sPath);
		::WritePrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), sData, sPath);
	}
}

void CGvisR2R_LaserView::SetEngraveFd(double dDist)
{
	double fLen = pDoc->GetOnePnlLen();
	double dOffset = dDist - (fLen*2.0);
	MoveMk(dOffset);
}

void CGvisR2R_LaserView::MoveEngrave(double dOffset)
{
#ifdef USE_MPE
	long lData = (long)(dOffset * 1000.0);
	//IoWrite(_T("MB440161"), 1);		// 마킹부 피딩 CW ON (PLC가 피딩완료 후 OFF))
	pView->m_pMpe->Write(_T("MB440161"), 1);
	//IoWrite(_T("ML45066"), lData);	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
	pView->m_pMpe->Write(_T("ML45066"), lData);
#endif
}

BOOL CGvisR2R_LaserView::IsEngraveFd()
{
	if (m_nShareDnCnt > 0)
	{
		if (!(m_nShareDnCnt % 2))
		{
#ifdef USE_MPE
			if (pDoc->m_pMpeSignal[5] & (0x01 << 1))	// 마킹부 피딩 CW ON (PLC가 피딩완료 후 OFF)
				return TRUE;
#endif
			return FALSE;
		}
	}
	else
	{
		if (m_nShareUpCnt > 0)
		{
			if (!(m_nShareUpCnt % 2))
			{
#ifdef USE_MPE
				if (pDoc->m_pMpeSignal[5] & (0x01 << 1))	// 마킹부 피딩 CW ON (PLC가 피딩완료 후 OFF)
					return TRUE;
#endif
				return FALSE;
			}
		}
	}

	return TRUE;
}

double CGvisR2R_LaserView::GetEngraveFdLen()
{
	int nLast = pDoc->GetLastShotEngrave();

	double dLen = (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dLen;
}

double CGvisR2R_LaserView::GetAoiInitDist()
{
	double dInitD1 = _tstof(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist) - _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dInitD1;
}

double CGvisR2R_LaserView::GetAoiRemain()
{
#ifdef USE_MPE
	double dCurPosEngraveFd = (double)pDoc->m_pMpeData[1][0];	// ML44052	,	각인부 Feeding 엔코더 값(단위 mm)
	double dRemain = _tstof(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist) - dCurPosEngraveFd;
	return dRemain;
#else
	return 0.0;
#endif
}

LRESULT CGvisR2R_LaserView::wmClientReceivedMdx(WPARAM wParam, LPARAM lParam)
{
	int nCmd = (int)wParam;
	CString* sReceived = (CString*)lParam;
	switch (nCmd)
	{
	case MdxIsReady:
		break;
	case GetCharacterString:
		break;
	default:
		break;
	}

	return (LRESULT)1;
}

LRESULT CGvisR2R_LaserView::wmClientReceivedSr(WPARAM wParam, LPARAM lParam)
{
	int nCmd = (int)wParam;
	CString* sReceived = (CString*)lParam;
	switch (nCmd)
	{
	case SrTriggerInputOn:
		//Get2dCode(m_sGetItsCode, m_nGetItsCodeSerial);
		if (m_pDlgMenu02)
		{
			m_pDlgMenu02->Disp2dCode();
		}
		break;
	default:
		break;
	}

	return (LRESULT)1;
}

LRESULT CGvisR2R_LaserView::wmServerReceived(WPARAM wParam, LPARAM lParam)
{
	if(!m_pEngrave)
		return (LRESULT)0;

	int nAcceptId = (int)wParam;
	SOCKET_DATA sSockData;
	SOCKET_DATA *pSocketData = (SOCKET_DATA*)lParam;

	//SOCKET_DATA rSockData = m_pEngrave->GetSocketData();
	//int nCmdCode = rSockData.nCmdCode;
	//int nMsgId = rSockData.nMsgID;
	SOCKET_DATA rSockData = *pSocketData;
	int nCmdCode = rSockData.nCmdCode;
	int nMsgId = rSockData.nMsgID;
	switch (nCmdCode)
	{
	case _GetSig:
		//if(nMsgId == _Connect)
		//{
		//	sSockData.nCmdCode = _Rtn;
		//	sSockData.nMsgID = _Connect;
		//	m_pEngrave->SendCommand(nAcceptId, sSockData);
		//}
		break;
	case _GetData:
		break;
	case _SetSig:
		if (m_pEngrave && m_pEngrave->IsConnected())
			m_pEngrave->GetSysSignal(rSockData);

		pView->m_bSetSig = TRUE;
		//if (m_pEngrave->m_bGetOpInfo || m_pEngrave->m_bGetInfo)
		//{
		//	if (m_pDlgInfo)
		//		m_pDlgInfo->UpdateData();

		//	if (m_pDlgMenu01)
		//		m_pDlgMenu01->UpdateData();
		//}

		//if (m_pDlgMenu03)
		//	m_pDlgMenu03->UpdateSignal();
		break;
	case _SetData:
		if (m_pEngrave && m_pEngrave->IsConnected())
			m_pEngrave->GetSysData(rSockData);

		pView->m_bSetData = TRUE;
		//if (m_pEngrave->m_bGetOpInfo || m_pEngrave->m_bGetInfo)
		//{
		//	if (m_pDlgInfo)
		//		m_pDlgInfo->UpdateData();

		//	if (m_pDlgMenu01)
		//		m_pDlgMenu01->UpdateData();
		//}

		//if (m_pDlgMenu02)
		//	m_pDlgMenu02->UpdateData();

		//if (m_pDlgMenu03)
		//	m_pDlgMenu03->UpdateData();

		//if (m_pDlgMenu04)
		//	m_pDlgMenu04->UpdateData();
		break;
	default:
		break;
	}


	return (LRESULT)1;
}

void CGvisR2R_LaserView::SetEngraveFdPitch(double dPitch)
{
	pDoc->SetEngraveFdPitch(dPitch);
	// 	m_pMotion->SetLeadPitch(AXIS_MKFD, dPitch);
	//long lData = long(dPitch*1000.0);
	//pView->m_pMpe->Write(_T("ML45020"), lData); // 각인부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
}


BOOL CGvisR2R_LaserView::IsConnected()
{
#ifdef USE_ENGRAVE
	if (m_pEngrave)
	{
		if (m_pEngrave->IsConnected())
		{
			if (!m_bContEngraveF)
			{
				m_bContEngraveF = TRUE;
			}
			return TRUE;
		}
		else
		{
			if (m_bContEngraveF)
			{
				m_bContEngraveF = FALSE;
			}
			return FALSE;
		}
	}
#endif
	return FALSE;
}

BOOL CGvisR2R_LaserView::IsConnectedMdx()
{
	if (m_pMdx2500)
	{
		return m_pMdx2500->IsConnected();
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::IsConnectedSr()
{
	if (m_pSr1000w)
	{
		return m_pSr1000w->IsConnected();
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::IsConnectedMk()
{
	if (m_pEngrave)
	{
		return m_pEngrave->IsConnected();
	}

	return FALSE;
}

BOOL CGvisR2R_LaserView::IsDispContRun()
{
	if (m_pEngrave)
	{
		return m_pEngrave->IsDispContRun();
	}

	return FALSE;
}


void CGvisR2R_LaserView::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnClose();
}


void CGvisR2R_LaserView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CGvisR2R_LaserView::IsPinPos0()
{
	if (!m_pMotion)
		return FALSE;

	double pPos[2];
	pPos[0] = m_pMotion->m_dPinPosX[0];
	pPos[1] = m_pMotion->m_dPinPosY[0];

	double dCurrX = m_dEnc[AXIS_X0];
	double dCurrY = m_dEnc[AXIS_Y0];

	if (dCurrX < pPos[0] - 4.0 || dCurrX > pPos[0] + 4.0)
		return FALSE;
	if (dCurrY < pPos[1] - 4.0 || dCurrY > pPos[1] + 4.0)
		return FALSE;

	return TRUE;
}


void CGvisR2R_LaserView::InitAutoEng()
{
	m_bMkSt = FALSE;
	m_bMkStSw = FALSE;
	m_nMkStAuto = 0;

	m_bEngSt = FALSE;
	m_bEngStSw = FALSE;
	m_nEngStAuto = 0;

	m_bEng2dSt = FALSE;
	m_bEng2dStSw = FALSE;
	m_nEng2dStAuto = 0;

	m_nGetItsCodeSerial = 0;
	pDoc->m_nShotNum = 0;
	pDoc->m_bUploadPinImg = FALSE;
	pDoc->BtnStatus.EngAuto._Init();
	InitAutoEngSignal();

	if (m_pDlgMenu01)
		m_pDlgMenu01->ResetLotTime();
}

// DoAuto
void CGvisR2R_LaserView::DoAutoEng()
{
	if ( !IsAuto() || (MODE_INNER != pDoc->WorkingInfo.LastJob.nTestMode) )
		return;

	CString str;
	str.Format(_T("%d : %d"), m_nStepTHREAD_DISP_DEF, m_bTHREAD_DISP_DEF ? 1 : 0);
	pView->DispStsBar(str, 6);
/*
	//// CycleStop
	//DoAutoChkCycleStop();

	//// DispMsg
	//DoAutoDispMsg();
*/
	// 각인부 마킹시작 신호를 확인
	DoAtuoGetEngStSignal();

	// 각인부 2D 코드 Reading신호를 확인
	DoAtuoGet2dReadStSignal();

	// Engrave Auto Sequence
	// 각인부 Marking Start
	DoAutoMarking();

	// 각인부 2D 코드 Reading Start
	DoAuto2dReading();
}

void CGvisR2R_LaserView::DoAtuoGetEngStSignal()
{
	if (!m_bEngSt)
	{
		//if (IsRun())
		{
			if ((pDoc->BtnStatus.EngAuto.IsMkSt || m_bMkStSw) && !pDoc->BtnStatus.EngAuto.MkStF)  // AlignTest		// 마킹시작(PC가 확인하고 Reset시킴.)-20141029
			{
				m_bEngStSw = FALSE;
				pDoc->BtnStatus.EngAuto.MkStF = TRUE;

				m_bEngSt = TRUE;
				m_nEngStAuto = ENG_ST;
				
				pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt, TRUE);
				if (m_pDlgMenu02)
					m_pDlgMenu02->SetLed(0, TRUE);
			}
		}
	}

	if (!pDoc->BtnStatus.EngAuto.IsMkSt && pDoc->BtnStatus.EngAuto.MkStF)
	{
		pDoc->BtnStatus.EngAuto.MkStF = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt, FALSE);
		if (m_pDlgMenu02)
			m_pDlgMenu02->SetLed(0, FALSE);
	}
}

void CGvisR2R_LaserView::DoAtuoGet2dReadStSignal()
{
	if (!m_bEng2dSt)
	{
		//if (IsRun())
		{
			if ((pDoc->BtnStatus.EngAuto.IsRead2dSt || m_bMkStSw) && !pDoc->BtnStatus.EngAuto.Read2dStF)  // 2D(GUI) Reading 동작 Start신호(PLC On->PC Off)
			{
				m_bEng2dStSw = FALSE;
				pDoc->BtnStatus.EngAuto.Read2dStF = TRUE;

				m_bEng2dSt = TRUE;
				m_nEng2dStAuto = ENG_2D_ST;
				pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt, TRUE);
				if (m_pDlgMenu02)
					m_pDlgMenu02->SetLed(3, TRUE);
			}
		}
	}

	if (!pDoc->BtnStatus.EngAuto.IsRead2dSt && pDoc->BtnStatus.EngAuto.Read2dStF)
	{
		pDoc->BtnStatus.EngAuto.Read2dStF = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt, FALSE);
		if (m_pDlgMenu02)
			m_pDlgMenu02->SetLed(3, FALSE);
	}
}

//void CGvisR2R_LaserView::DoAtuoGetEngStSignal()
//{
//	if (!m_bEngSt)
//	{
//		if (IsRun())
//		{
//			if ((pDoc->BtnStatus.EngAuto.MkSt || m_bMkStSw) && !pDoc->BtnStatus.EngAuto.MkStF)  // AlignTest		// 마킹시작(PC가 확인하고 Reset시킴.)-20141029
//			{
//				m_bEngStSw = FALSE;
//				pDoc->BtnStatus.EngAuto.MkStF = TRUE;
//
//				//m_pMpe->Write(_T("MB440110"), 0);			// 마킹시작(PC가 확인하고 Reset시킴.)-20141029
//				//if (pDoc->m_pMpeSignal[0] & (0x01 << 1))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
//				//	m_pMpe->Write(_T("MB440101"), 0);		// 마킹부 Feeding완료
//
//				m_bEngSt = TRUE;
//				m_nEngStAuto = ENG_ST;
//				
//				if (m_pEngrave)
//				{
//					pDoc->BtnStatus.EngAuto.MkSt = FALSE;
//					m_pEngrave->SwEngAutoMkSt(FALSE);
//					Sleep(100);
//				}
//			}
//			else if (!pDoc->BtnStatus.EngAuto.MkSt && pDoc->BtnStatus.EngAuto.MkStF)
//				pDoc->BtnStatus.EngAuto.MkStF = FALSE;
//		}
//	}
//}

//void CGvisR2R_LaserView::DoAtuoGet2dReadStSignal()
//{
//	if (!m_bEng2dSt)
//	{
//		if (IsRun())
//		{
//			if ((pDoc->BtnStatus.EngAuto.Read2dSt || m_bMkStSw) && !pDoc->BtnStatus.EngAuto.Read2dStF)  // 2D(GUI) Reading 동작 Start신호(PLC On->PC Off)
//			{
//				m_bEng2dStSw = FALSE;
//				pDoc->BtnStatus.EngAuto.Read2dStF = TRUE;
//
//				//m_pMpe->Write(_T("MB440110"), 0);			// 마킹시작(PC가 확인하고 Reset시킴.)-20141029
//				//if (pDoc->m_pMpeSignal[0] & (0x01 << 1))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
//				//	m_pMpe->Write(_T("MB440101"), 0);		// 마킹부 Feeding완료
//
//				m_bEng2dSt = TRUE;
//				m_nEng2dStAuto = ENG_2D_ST;
//				
//				if (m_pEngrave)
//				{
//					pDoc->BtnStatus.EngAuto.Read2dSt = FALSE;
//					m_pEngrave->SwEngAuto2dReadSt(FALSE);
//					Sleep(100);
//				}
//			}
//			else if(!pDoc->BtnStatus.EngAuto.Read2dSt && pDoc->BtnStatus.EngAuto.Read2dStF)
//				pDoc->BtnStatus.EngAuto.Read2dStF = FALSE;
//		}
//	}
//}

void CGvisR2R_LaserView::DoAuto2dReading()
{
	if (MODE_INNER == pDoc->WorkingInfo.LastJob.nTestMode || MODE_OUTER == pDoc->WorkingInfo.LastJob.nTestMode)
	{
		Eng2dRead();
	}
}



// DoAutoMarking
void CGvisR2R_LaserView::DoAutoMarking()
{
	if (MODE_INNER == pDoc->WorkingInfo.LastJob.nTestMode)
	{
		MarkingWith1PointAlign();
	}
}

void CGvisR2R_LaserView::MarkingWith1PointAlign()
{
	Eng1PtReady();
	//Eng1PtChkSerial();
	Eng1PtInit();
	Eng1PtAlignPt0();
	//Eng1PtAlignPt1();
	//Eng1PtMoveInitPos();
	//Eng1PtElecChk();
	Eng1PtDoMarking();
	//Eng1PtLotDiff();
	//Eng1PtReject();
	//Eng1PtErrStop();
}

void CGvisR2R_LaserView::Eng1PtReady()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = pDoc->GetLastShotEngrave() + 1;

	if (m_bEngSt)
	{
		switch (m_nEngStAuto)
		{
		case ENG_ST:	// PLC MK 신호 확인	
			//if (IsRun())
			{
				m_nEngStAuto++;
			}
			break;
		case ENG_ST + 1:
			//if (m_pEngrave)
			//{
			//	m_pEngrave->SwEngAutoOnMking(TRUE);
			//	Sleep(100);
			//}
			pDoc->BtnStatus.EngAuto.IsOnMking = FALSE;
			pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, TRUE);
			if (m_pDlgMenu02)
				m_pDlgMenu02->SetLed(1, TRUE);
			m_nEngStAuto = ENG_ST + (Mk1PtIdx::Start);
			break;
		case ENG_ST + (Mk1PtIdx::Start) :	// 2
			if(pDoc->BtnStatus.EngAuto.IsOnMking)
				m_nEngStAuto++;
			break;
		case ENG_ST + (Mk1PtIdx::Start) + 1:
			m_nEngStAuto = ENG_ST + (Mk1PtIdx::InitMk);			// InitMk()
			break;
		}
	}
}

void CGvisR2R_LaserView::Eng1PtInit()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = pDoc->GetLastShotEngrave() + 1;

	if (m_bEngSt)
	{
		switch (m_nEngStAuto)
		{
		case ENG_ST + (Mk1PtIdx::InitMk) :
			m_nEngStAuto++;
			if (nSerial > 2)
			{
				AdjPinPosEng();
				// 각인부 작업완료.(PC가 On, PLC가 확인 후 Off) - ?
			}
			break;

		case ENG_ST + (Mk1PtIdx::InitMk) + 1:
			//if (IsRun())
				m_nEngStAuto = ENG_ST + (Mk1PtIdx::Move0Cam0);	// Move - Cam1 - Pt0
				//m_nEngStAuto = ENG_ST + (Mk1PtIdx::DoMk);
			break;
		}
	}
}

void CGvisR2R_LaserView::Eng1PtAlignPt0()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = pDoc->GetLastShotEngrave() + 1;

	if (m_bEngSt)
	{
		switch (m_nEngStAuto)
		{
		case ENG_ST + (Mk1PtIdx::Move0Cam0) :	// Move - Cam1 - Pt0
			MovePinPos();	//if (MoveAlign0(0))
			m_nEngStAuto++;
			break;
		case ENG_ST + (Mk1PtIdx::Move0Cam0) + 1:
			//if (IsRun())
				m_nEngStAuto++;
			break;
		case ENG_ST + (Mk1PtIdx::Move0Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nEngStAuto++;
			}
			break;
		case ENG_ST + (Mk1PtIdx::Move0Cam0) + 3:
			m_nEngStAuto++;
			break;
		case ENG_ST + (Mk1PtIdx::Align0_0) :		// 1PtAlign - Cam0 - Pt0
			m_nEngStAuto++;
			if (!m_bSkipAlign[0][0])
			{
				if (OnePointAlign0(0))
					m_bFailAlign[0][0] = FALSE;
				else
					m_bFailAlign[0][0] = TRUE;
			}
			break;
		case ENG_ST + (Mk1PtIdx::Align0_0) + 1:
			if (!IsRun())
				break;

			if (m_bFailAlign[0][0])
			{
				Buzzer(TRUE, 0);
				//if(IDNO == DoMyMsgBox(_T("카메라의 원점 정렬을 다시하시겠습니까?"), MB_YESNO))
				if (IDNO == MsgBox(_T("카메라의 원점 정렬을 다시하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					//if(IDYES == DoMyMsgBox(_T("카메라의 원점 정렬을 정말 않하시겠습니까?"), MB_YESNO))
					if (IDYES == MsgBox(_T("카메라의 원점 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][0] = FALSE;
						m_bSkipAlign[0][0] = TRUE;
						m_bSkipAlign[0][1] = TRUE;
						m_bSkipAlign[0][2] = TRUE;
						m_bSkipAlign[0][3] = TRUE;
						//if(IDNO == DoMyMsgBox(_T("판넬에 레이저 2D 마킹을 하시겠습니까?"), MB_YESNO))
						if (IDNO == MsgBox(_T("판넬에 레이저 2D 마킹을 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE;
							m_bDoneMk[0] = TRUE;

							EngStop(TRUE);
							TowerLamp(RGB_YELLOW, TRUE);
						}
						else
						{
							m_bDoMk[0] = TRUE;
							m_bDoneMk[0] = FALSE;
						}
					}
					else
					{
						// 원점 정렬을 다시 함.
						m_bReAlign[0][0] = TRUE;
						m_bSkipAlign[0][0] = FALSE;
						m_bSkipAlign[0][1] = FALSE;
						m_bSkipAlign[0][2] = FALSE;
						m_bSkipAlign[0][3] = FALSE;
						m_nEngStAuto = ENG_ST + (Mk1PtIdx::Move0Cam0); // OnePointAlign0(0) 으로 진행. - 카메라 재정렬
																	 //m_nEngStAuto = MK_ST + (Mk1PtIdx::Move0Cam0); // TwoPointAlign0(0) 으로 진행. - 카메라 재정렬
																	 //m_nEngStAuto = MK_ST + (Mk1PtIdx::Move0Cam1); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
						EngStop(TRUE);
						TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{ 
					// 원점 정렬을 다시 함.
					Buzzer(FALSE, 0);

					m_bReAlign[0][0] = TRUE;
					m_bSkipAlign[0][0] = FALSE;
					m_bSkipAlign[0][1] = FALSE;
					m_nEngStAuto = ENG_ST + (Mk1PtIdx::Move0Cam0); // OnePointAlign0(0) 으로 진행. - 카메라 재정렬
																 //m_nEngStAuto = MK_ST + (Mk1PtIdx::Move0Cam0); // TwoPointAlign0(0) 으로 진행. - 카메라 재정렬
																 //m_nEngStAuto = MK_ST + (Mk1PtIdx::Move0Cam1); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
					EngStop(TRUE);
					TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][0])
			{
				if (!m_bReAlign[0][0])
				{
					if (m_bDoMk[0])
						m_nEngStAuto++; // DoMk
					else
					{
						//MovePinPos(); // 2D 코드 위치
						m_nEngStAuto = ENG_ST + (Mk1PtIdx::DoneMk); // Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
				{
					m_nEngStAuto = ENG_ST + (Mk1PtIdx::Move0Cam0); // OnePointAlign0(0) 으로 진행. - 카메라 재정렬
																 //m_nEngStAuto = MK_ST + (Mk1PtIdx::Move0Cam0); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
				}
			}
			else
				m_nEngStAuto++; // DoMk

			break;
		case ENG_ST + (Mk1PtIdx::Align0_0) + 2:
			if (IsRun())
			{
				//MovePinPos(); // 2D 코드 위치
				//pDoc->m_nShotNum++;							// 각인할 시리얼 증가
				m_nEngStAuto = ENG_ST + (Mk1PtIdx::DoMk);
			}
			break;
		}
	}
}

void CGvisR2R_LaserView::Eng1PtDoMarking()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = pDoc->GetLastShotEngrave() + 1;

	if (m_bEngSt)
	{
		switch (m_nEngStAuto)
		{
		case ENG_ST + (Mk1PtIdx::DoMk) :				// Mk 마킹 시작
			if (!IsRun())
				break;

			if (!pDoc->WorkingInfo.System.bNoMk)
			{
				if (!SetMdxLotAndShotNum(pDoc->m_sItsCode, nSerial))
				{
					EngStop(TRUE);
					//MsgBox(_T("SetMdxLotAndShotNum - Failed."));
					//TowerLamp(RGB_RED, TRUE);
					//Buzzer(TRUE, 0);
					break;
				}
			}
			Sleep(100);
			m_nEngStAuto++;
			break;

		case ENG_ST + (Mk1PtIdx::DoMk) + 1:
			if (!IsRun())
				break;

			if (!pDoc->WorkingInfo.System.bNoMk)
			{
				if(m_nGetItsCodeSerial == 0 || (m_nGetItsCodeSerial + 1) == nSerial )
					SetMk(TRUE);	// Mk 마킹 시작
				else if ((m_nGetItsCodeSerial + 1) > nSerial)
				{
					EngStop(TRUE);
					MsgBox(_T("정지 - 2D바코드의 각인된 시리얼이 각인할 시리얼보다 큽니다."));
					TowerLamp(RGB_RED, TRUE);
					Buzzer(TRUE, 0);
					break;
				}
			}
			//SetCurrentInfoEngShotNum(pDoc->m_nShotNum);
			if (m_nGetItsCodeSerial == 0 || (m_nGetItsCodeSerial + 1) == nSerial)
				SetCurrentInfoEngShotNum(nSerial);

			Sleep(300);
			m_nEngStAuto++;
			break;
		case ENG_ST + (Mk1PtIdx::DoMk) + 2:
			if (IsMkDone())
			{
				Sleep(300);
				m_nEngStAuto = ENG_ST + (Mk1PtIdx::DoneMk);	// Mk 마킹 완료
			}
			break;
		case ENG_ST + (Mk1PtIdx::DoneMk) :
			//if (IsRun())
			{
				//m_pEngrave->SwEngAutoOnMking(FALSE);
				//Sleep(100);
				pDoc->BtnStatus.EngAuto.IsOnMking = TRUE;
				pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, FALSE);
				if (m_pDlgMenu02)
					m_pDlgMenu02->SetLed(1, FALSE);
				m_nEngStAuto++;
			}
			break;
		case ENG_ST + (Mk1PtIdx::DoneMk) + 1:
			//if (m_pEngrave)
			//{
			//	m_pEngrave->SwEngAutoMkDone(TRUE);
			//	pDoc->BtnStatus.EngAuto.MkStF = FALSE;
			//	Sleep(100);
			//}
			if (!pDoc->BtnStatus.EngAuto.IsOnMking)
			{
				pDoc->BtnStatus.EngAuto.IsMkDone = FALSE;
				pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, TRUE);
				if (m_pDlgMenu02)
					m_pDlgMenu02->SetLed(2, TRUE);
				m_nEngStAuto++;
			}
			break;
		case ENG_ST + (Mk1PtIdx::DoneMk) + 2:
			if (pDoc->BtnStatus.EngAuto.IsMkDone)
			{
				pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, FALSE);
				if (m_pDlgMenu02)
					m_pDlgMenu02->SetLed(2, FALSE);
				Sleep(300);
				m_nEngStAuto++;
			}
			break;
		case ENG_ST + (Mk1PtIdx::DoneMk) + 3:
			//if (pDoc->m_pMpeSignal[0] & (0x01 << 2))	// MB440102 - 각인부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
			if(pDoc->BtnStatus.EngAuto.IsFdDone)
			{
				if (m_nGetItsCodeSerial == 0 || (m_nGetItsCodeSerial + 1) == nSerial)
					SetLastSerialEng(nSerial); // (_ttoi(pDoc->m_sShotNum));
				//SetLastSerialEng(pDoc->m_nShotNum); // (_ttoi(pDoc->m_sShotNum));
				pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone, TRUE);
				if (m_pDlgMenu02)
					m_pDlgMenu02->SetLed(6, TRUE);
				m_nEngStAuto++;
			}
			break;
		case ENG_ST + (Mk1PtIdx::DoneMk) + 4:
			if (!pDoc->BtnStatus.EngAuto.IsFdDone)
			{
				pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone, FALSE);
				if (m_pDlgMenu02)
					m_pDlgMenu02->SetLed(6, FALSE);
				m_nEngStAuto++;
			}
			break;
		case ENG_ST + (Mk1PtIdx::DoneMk) + 5:
			m_nEngStAuto = 0;
			m_bEngSt = FALSE;
			break;
		}
	}
}

BOOL CGvisR2R_LaserView::IsMkDone()
{
	if (!pView || !pView->m_pMdx2500)
		return FALSE;

	return (!pView->m_pMdx2500->IsRunning());
}

BOOL CGvisR2R_LaserView::SetMk(BOOL bRun)	// Marking Start
{
	if (!pView || !pView->m_pMdx2500)
		return FALSE;

	return (pView->m_pMdx2500->LaserMarking());
}

BOOL CGvisR2R_LaserView::SetMdxLotAndShotNum(CString sLot, int nSerial)
{
	if (!pView || !pView->m_pMdx2500)
		return FALSE;

	CString sSerial;
	sSerial.Format(_T("%03d"), nSerial);
	return m_pMdx2500->SetMdxOrderShotNum(sLot, sSerial);
}

void CGvisR2R_LaserView::AdjPinPosEng()
{
	if (m_pDlgMenu02)
	{
		if (m_pDlgMenu02->m_dMkFdOffsetY[0][0] > -2.0 &&
			m_pDlgMenu02->m_dMkFdOffsetY[0][0] < 2.0)
		{
			double dOffsetY = -1.0*(m_pDlgMenu02->m_dMkFdOffsetY[0][0]);
			dOffsetY *= pDoc->m_dShiftAdjustRatio;

			CfPoint ptPnt[2];
			ptPnt[0].x = _tstof(pDoc->WorkingInfo.Motion.sPinPosX[0]);
			ptPnt[0].y = _tstof(pDoc->WorkingInfo.Motion.sPinPosY[0]) + dOffsetY;

			m_pDlgMenu02->SetPinPos(0, ptPnt[0]);
		}
	}
}

BOOL CGvisR2R_LaserView::OnePointAlign0(int nPos)
{
	if (!m_pDlgMenu02)
		return FALSE;
	BOOL bRtn;
	CfPoint ptPnt(0.0, 0.0);
	CfPoint _ptPnt(0.0, 0.0);
	bRtn = m_pDlgMenu02->OnePointAlign(ptPnt); // 비전으로 확인한 원점위치 (Motion의 절대좌표계).
	if(bRtn)
		SetEngOffset(ptPnt);
	else
		SetEngOffset(_ptPnt);

	return bRtn;
}


// DoAutoReading

void CGvisR2R_LaserView::Eng2dRead()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial;

	if (m_bEng2dSt)
	{
		switch (m_nEng2dStAuto)
		{
		case ENG_2D_ST:	// PLC MK 신호 확인	
			if (IsRun())
			{
				MoveInitPos0();
				m_nEng2dStAuto++;
			}
			break;
		case ENG_2D_ST + 1:
			//if (m_pEngrave)
			//{
			//	m_pEngrave->SwEngAutoOnReading2d(TRUE);
			//	Sleep(100);
			//}
			pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, TRUE);
			if (m_pDlgMenu02)
				m_pDlgMenu02->SetLed(4, TRUE);
			m_nEng2dStAuto = ENG_2D_ST + (Read2dIdx::Start);
			break;
		case ENG_2D_ST + (Read2dIdx::Start) :	// 2
			m_nEng2dStAuto++;
			break;
		case ENG_2D_ST + (Read2dIdx::Start) + 1:
			m_nEng2dStAuto = ENG_2D_ST + (Read2dIdx::DoRead);
			break;
		case ENG_2D_ST + (Read2dIdx::DoRead) :			// 2D Reading 시작
			if (!pDoc->WorkingInfo.System.bNoMk)
				Set2dRead(TRUE);							// 2D Reading 시작
			m_nEng2dStAuto++;
			break;
		case ENG_2D_ST + (Read2dIdx::DoRead) + 1:
			Sleep(300);
			m_nEng2dStAuto++;
			break;
		case ENG_2D_ST + (Read2dIdx::DoRead) + 2:
			if (Is2dReadDone())
			{
				if (!pDoc->WorkingInfo.System.bNoMk)
					Get2dCode(m_sGetItsCode, m_nGetItsCodeSerial);

				Sleep(300);
				m_nEng2dStAuto = ENG_2D_ST + (Read2dIdx::DoneRead);	// 2D Reading 완료
			}
			break;
		case ENG_2D_ST + (Read2dIdx::DoneRead) :
			if (IsRun())
			{
				m_nEng2dStAuto++;
				pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, FALSE);
				if (m_pDlgMenu02)
					m_pDlgMenu02->SetLed(4, FALSE);
				//m_pEngrave->SwEngAutoOnReading2d(FALSE);
				//Sleep(100);
				MovePinPos();
			}
			break;
		case ENG_2D_ST + (Read2dIdx::DoneRead) + 1:
			//if (m_pEngrave)
			//{
			//	pDoc->BtnStatus.EngAuto.Read2dStF = FALSE;
			//	m_pEngrave->SwEngAuto2dReadDone(TRUE);
			//	Sleep(100);
			//}
			pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, TRUE);
			if (m_pDlgMenu02)
				m_pDlgMenu02->SetLed(5, TRUE);
			m_nEng2dStAuto++;
			break;
		case ENG_2D_ST + (Read2dIdx::DoneRead) + 2:
			if (pDoc->BtnStatus.EngAuto.IsRead2dDone)
			{
				m_nEng2dStAuto = 0;
				m_bEng2dSt = FALSE;
				pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, FALSE);
				if (m_pDlgMenu02)
					m_pDlgMenu02->SetLed(5, FALSE);
				Sleep(300);
			}
			break;
		}
	}
}


BOOL  CGvisR2R_LaserView::Is2dReadDone()
{
	if (!pView || !pView->m_pSr1000w)
		return FALSE;

	return (!pView->m_pSr1000w->IsRunning());
}

BOOL CGvisR2R_LaserView::Set2dRead(BOOL bRun)	// Marking Start
{
	if (!pView || !pView->m_pSr1000w)
		return FALSE;

	return (pView->m_pSr1000w->DoRead2DCode());
}

BOOL CGvisR2R_LaserView::SetEngOffset(CfPoint &OfSt)
{
	// Write Feeding Offset data....
	if(m_pDlgFrameHigh)
		m_pDlgFrameHigh->SetEngOffset(OfSt);
	return pDoc->SetEngOffset(OfSt);
}

void CGvisR2R_LaserView::SetMyMsgYes()
{
	if (m_pDlgMyMsg)
	{
		if (m_pDlgMyMsg->m_pDlgMyMsgSub01)
		{
			((CDlgMyMsgSub01*)(m_pDlgMyMsg->m_pDlgMyMsgSub01))->ClickYes();
		}
	}
}

void CGvisR2R_LaserView::SetMyMsgNo()
{
	if (m_pDlgMyMsg)
	{
		if (m_pDlgMyMsg->m_pDlgMyMsgSub01)
		{
			((CDlgMyMsgSub01*)(m_pDlgMyMsg->m_pDlgMyMsgSub01))->ClickNo();
		}
	}
}

void CGvisR2R_LaserView::SetMyMsgOk()
{
	if (m_pDlgMyMsg)
	{
		if (m_pDlgMyMsg->m_pDlgMyMsgSub02)
		{
			((CDlgMyMsgSub02*)(m_pDlgMyMsg->m_pDlgMyMsgSub02))->ClickOk();
		}
	}
}

void CGvisR2R_LaserView::InitAutoEngSignal()
{
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone, FALSE);

	if (m_pDlgMenu02)
	{
		m_pDlgMenu02->SetLed(0, FALSE); // _SigInx::_EngAutoSeqMkSt
		m_pDlgMenu02->SetLed(1, FALSE); // _SigInx::_EngAutoSeqOnMkIng
		m_pDlgMenu02->SetLed(2, FALSE); // _SigInx::_EngAutoSeqMkDone
		m_pDlgMenu02->SetLed(3, FALSE); // _SigInx::_EngAutoSeq2dReadSt
		m_pDlgMenu02->SetLed(4, FALSE); // _SigInx::_EngAutoSeqOnReading2d
		m_pDlgMenu02->SetLed(5, FALSE); // _SigInx::_EngAutoSeq2dReadDone
		m_pDlgMenu02->SetLed(6, FALSE); // _SigInx::_EngAutoSeqFdDone
	}
}

BOOL CGvisR2R_LaserView::GetCurrentInfoSignal()
{
	pDoc->BtnStatus.EngAuto.IsInit = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoInit);
	pDoc->BtnStatus.EngAuto.IsMkSt = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt);
	pDoc->BtnStatus.EngAuto.IsOnMking = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng);
	pDoc->BtnStatus.EngAuto.IsMkDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone);
	pDoc->BtnStatus.EngAuto.IsRead2dSt = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt);
	pDoc->BtnStatus.EngAuto.IsOnRead2d = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d);
	pDoc->BtnStatus.EngAuto.IsRead2dDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone);
	pDoc->BtnStatus.EngAuto.IsFdDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone);

	return TRUE;
}

void CGvisR2R_LaserView::SetLastSerialEng(int nSerial)
{
	if (m_pDlgFrameHigh)
		m_pDlgFrameHigh->SetEngraveLastShot(nSerial);
}

int CGvisR2R_LaserView::GetLastSerialEng()
{
	int nSerial = 0;
	if (pDoc)
		nSerial = pDoc->GetLastSerialEng();

	return nSerial;
}

CString CGvisR2R_LaserView::GetCurrentInfoBufUp()
{
	return pDoc->GetCurrentInfoBufUp();
}

CString CGvisR2R_LaserView::GetCurrentInfoBufDn()
{
	return pDoc->GetCurrentInfoBufDn();
}

void CGvisR2R_LaserView::ChkBufUp()
{
	CString str = GetCurrentInfoBufUp();

	if (pFrm)
	{
		if (m_sBuf[0] != str)
		{
			m_sBuf[0] = str;
			pFrm->DispStatusBar(str, 3);
		}
	}
}

void CGvisR2R_LaserView::ChkBufDn()
{
	CString str = GetCurrentInfoBufDn();

	if (pFrm)
	{
		if (m_sBuf[1] != str)
		{
			m_sBuf[1] = str;
			pFrm->DispStatusBar(str, 1);
		}
	}
}

void CGvisR2R_LaserView::SetCurrentInfoEngShotNum(int nSerial)
{
	pDoc->SetCurrentInfoEngShotNum(nSerial);
}


BOOL CGvisR2R_LaserView::Get2dCode(CString &sLot, int &nSerial)
{
	if (!m_pSr1000w)
		return FALSE;

	CString sData;
	if (m_pSr1000w->Get2DCode(sData))
	{
		int nPos = sData.ReverseFind('-');
		if (nPos != -1)
		{
			pDoc->m_sOrderNum = sData.Left(nPos);
			pDoc->m_sShotNum = sData.Right(sData.GetLength() - nPos - 1);
			pDoc->m_nShotNum = _tstoi(pDoc->m_sShotNum);
			sLot = pDoc->m_sOrderNum;
			nSerial = pDoc->m_nShotNum;
		}
		else
		{
			pView->MsgBox(sData);
			//AfxMessageBox(sData);
		}

		return TRUE;
	}

	return FALSE;
}

void CGvisR2R_LaserView::SetTotOpRto(CString sVal)		// 전체진행율
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetTotOpRto(sVal);
}

void CGvisR2R_LaserView::SetTotVel(CString sVal)		// 전체속도
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetTotVel(sVal);
}

void CGvisR2R_LaserView::SetPartVel(CString sVal)		// 구간속도
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetPartVel(sVal);
}

void CGvisR2R_LaserView::SetMkDoneLen(CString sVal)		// 마킹부 : Distance (FdDone) [M]
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetMkDoneLen(sVal);
}

void CGvisR2R_LaserView::SetAoiDnDoneLen(CString sVal)	// 검사부(하) : Distance (FdDone) [M]
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetAoiDnDoneLen(sVal);
}

void CGvisR2R_LaserView::SetAoiUpDoneLen(CString sVal)	// 검사부(상) : Distance (FdDone) [M]
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetAoiUpDoneLen(sVal);
}

void CGvisR2R_LaserView::SetEngDoneLen(CString sVal)	// 검사부(상) : Distance (FdDone) [M]
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetEngDoneLen(sVal);
}

void CGvisR2R_LaserView::DispStatusBar(CString strMsg, int nStatusBarID)
{
	if (pFrm)
		pFrm->DispStatusBar(strMsg, nStatusBarID);
}

void CGvisR2R_LaserView::GetMkMenu01()
{
	m_bTIM_MENU01_UPDATE_WORK = TRUE;
	SetTimer(TIM_MENU01_UPDATE_WORK, 500, NULL);
}

CString CGvisR2R_LaserView::GetMkMenu01(CString sMenu, CString sItem)
{
	CString sPath = pDoc->WorkingInfo.System.sPathMkMenu01;
	TCHAR szData[512];

	if (sPath.IsEmpty())
		return _T("");

	if (0 < ::GetPrivateProfileString(sMenu, sItem, NULL, szData, sizeof(szData), sPath))
		return CString(szData);

	return _T("");
}


CString CGvisR2R_LaserView::GetMkDispInfoUp(CString sMenu, CString sItem, int nSerial)
{
	TCHAR szData[512];
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\Disp.txt"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		nSerial);

	if (sPath.IsEmpty())
		return _T("");

	if (0 < ::GetPrivateProfileString(sMenu, sItem, NULL, szData, sizeof(szData), sPath))
		return CString(szData);

	return _T("");
}

CString CGvisR2R_LaserView::GetMkDispInfoDn(CString sMenu, CString sItem, int nSerial)
{
	TCHAR szData[512];
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\Disp.txt"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		nSerial);

	if (sPath.IsEmpty())
		return _T("");

	if (0 < ::GetPrivateProfileString(sMenu, sItem, NULL, szData, sizeof(szData), sPath))
		return CString(szData);

	return _T("");
}

CString CGvisR2R_LaserView::GetTimeIts()
{
	stLotTime ItsTime;

	CString strVal;
	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	ItsTime.nYear = Tim.GetYear();
	ItsTime.nMonth = Tim.GetMonth();
	ItsTime.nDay = Tim.GetDay();
	ItsTime.nHour = Tim.GetHour();
	ItsTime.nMin = Tim.GetMinute();
	ItsTime.nSec = Tim.GetSecond();

	strVal.Format(_T("%04d%02d%02d%02d%02d%02d"),
		ItsTime.nYear, ItsTime.nMonth, ItsTime.nDay,
		ItsTime.nHour, ItsTime.nMin, ItsTime.nSec);

	return strVal;
}

BOOL CGvisR2R_LaserView::ReloadRstInner()
{
	double dRatio = 0.0;
	CString sVal = _T("");
	CDlgProgress dlg;
	sVal.Format(_T("On Reloading InnerReelmap."));
	dlg.Create(sVal);
	//dlg.SetRange(0, 500);

	//GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;


	BOOL bRtn[7];
	if (pDoc->m_pReelMapInner)
		bRtn[0] = pDoc->m_pReelMapInner->ReloadRst();
	//dlg.SetPos(1);
	if (pDoc->m_pReelMapInnerUp)
		bRtn[1] = pDoc->m_pReelMapInnerUp->ReloadRst();
	//dlg.SetPos(2);

	if (pDoc->m_pReelMapIts)
		bRtn[2] = pDoc->m_pReelMapIts->ReloadRst();

	//if (pDoc->m_pReelMapInOuterUp)
	//	bRtn[2] = pDoc->m_pReelMapInOuterUp->ReloadRst();

	//if (pDoc->m_pReelMapInOuterDn)
	//	bRtn[3] = pDoc->m_pReelMapInOuterDn->ReloadRst();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapInnerDn)
			bRtn[4] = pDoc->m_pReelMapInnerDn->ReloadRst();
		//dlg.SetPos(3);
		if (pDoc->m_pReelMapInnerAllUp)
			bRtn[5] = pDoc->m_pReelMapInnerAllUp->ReloadRst();
		//dlg.SetPos(4);
		if (pDoc->m_pReelMapInnerAllDn)
			bRtn[6] = pDoc->m_pReelMapInnerAllDn->ReloadRst();
		//dlg.SetPos(5);
	}

	int nRatio[7] = { 0 };
	BOOL bDone[7] = { 0 };
	int nTo = 0;
	if (bDualTest)
		nTo = 600; //[%]
	else
		nTo = 300; //[%]

	dlg.SetRange(0, nTo);

	for (int nProc = 0; nProc < nTo;)
	{
		if (pDoc->m_pReelMapInner)
		{
			nRatio[0] = pDoc->m_pReelMapInner->GetProgressReloadRst();
			bDone[0] = pDoc->m_pReelMapInner->IsDoneReloadRst();
		}
		else
			bDone[0] = TRUE;
		if (!bRtn[0])
			bDone[0] = TRUE;

		if (pDoc->m_pReelMapInnerUp)
		{
			nRatio[1] = pDoc->m_pReelMapInnerUp->GetProgressReloadRst();
			bDone[1] = pDoc->m_pReelMapInnerUp->IsDoneReloadRst();
		}
		else
			bDone[1] = TRUE;
		if (!bRtn[1])
			bDone[1] = TRUE;

		//if (pDoc->m_pReelMapInOuterUp)
		//{
		//	nRatio[2] = pDoc->m_pReelMapInOuterUp->GetProgressReloadRst();
		//	bDone[2] = pDoc->m_pReelMapInOuterUp->IsDoneReloadRst();
		//}
		//else
		//	bDone[2] = TRUE;
		//if (!bRtn[2])
		//	bDone[2] = TRUE;

		//if (pDoc->m_pReelMapInOuterDn)
		//{
		//	nRatio[3] = pDoc->m_pReelMapInOuterDn->GetProgressReloadRst();
		//	bDone[3] = pDoc->m_pReelMapInOuterDn->IsDoneReloadRst();
		//}
		//else
		//	bDone[3] = TRUE;
		//if (!bRtn[3])
		bDone[3] = TRUE;

		if (pDoc->m_pReelMapIts)
		{
			nRatio[2] = pDoc->m_pReelMapIts->GetProgressReloadRst();
			bDone[2] = pDoc->m_pReelMapIts->IsDoneReloadRst();
		}
		else
			bDone[2] = TRUE;
		if (!bRtn[2])
			bDone[2] = TRUE;

		if (bDualTest)
		{
			if (pDoc->m_pReelMapInnerDn)
			{
				nRatio[4] = pDoc->m_pReelMapInnerDn->GetProgressReloadRst();
				bDone[4] = pDoc->m_pReelMapInnerDn->IsDoneReloadRst();
			}
			else
				bDone[4] = TRUE;
			if (!bRtn[4])
				bDone[4] = TRUE;

			if (pDoc->m_pReelMapInnerAllUp)
			{
				nRatio[5] = pDoc->m_pReelMapInnerAllUp->GetProgressReloadRst();
				bDone[5] = pDoc->m_pReelMapInnerAllUp->IsDoneReloadRst();
			}
			else
				bDone[5] = TRUE;
			if (!bRtn[5])
				bDone[5] = TRUE;

			if (pDoc->m_pReelMapAllDn)
			{
				nRatio[6] = pDoc->m_pReelMapInnerAllDn->GetProgressReloadRst();
				bDone[6] = pDoc->m_pReelMapInnerAllDn->IsDoneReloadRst();
			}
			else
				bDone[6] = TRUE;
			if (!bRtn[6])
				bDone[6] = TRUE;

		}
		else
		{
			bDone[4] = TRUE;
			bDone[5] = TRUE;
			bDone[6] = TRUE;
		}

		nProc = nRatio[0] + nRatio[1] + nRatio[2] + nRatio[3] + nRatio[4] + nRatio[5] + nRatio[6];

		if (bDone[0] && bDone[1] && bDone[2] && bDone[3] && bDone[4] && bDone[5] && bDone[6])
			break;
		else
		{
			dlg.SetPos(nProc);
			Sleep(30);
		}
	}

	dlg.DestroyWindow();

	if (bDualTest)
	{
		for (int i = 0; i < 7; i++)
		{
			if (!bRtn[i])
				return FALSE;
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (!bRtn[i])
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CGvisR2R_LaserView::ReloadRstInner(int nSerial)
{
	//GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	BOOL bRtn[7] = { 0 };
	if (pDoc->m_pReelMapInner)
		bRtn[0] = pDoc->m_pReelMapInner->ReloadRst(nSerial);
	if (pDoc->m_pReelMapInnerUp)
		bRtn[1] = pDoc->m_pReelMapInnerUp->ReloadRst(nSerial);

	//if (pDoc->m_pReelMapInOuterUp)
	//	bRtn[2] = pDoc->m_pReelMapInOuterUp->ReloadRst(nSerial);

	//if (pDoc->m_pReelMapInOuterDn)
	//	bRtn[3] = pDoc->m_pReelMapInOuterDn->ReloadRst(nSerial);
	bRtn[3] = TRUE;

	if (pDoc->m_pReelMapIts)
		bRtn[2] = pDoc->m_pReelMapIts->ReloadRst(nSerial);

	if (bDualTest)
	{
		if (pDoc->m_pReelMapInnerDn)
			bRtn[4] = pDoc->m_pReelMapInnerDn->ReloadRst(nSerial);
		if (pDoc->m_pReelMapInnerAllUp)
			bRtn[5] = pDoc->m_pReelMapInnerAllUp->ReloadRst(nSerial);
		if (pDoc->m_pReelMapInnerAllDn)
			bRtn[6] = pDoc->m_pReelMapInnerAllDn->ReloadRst(nSerial);

		for (int i = 0; i < 7; i++)
		{
			if (!bRtn[i])
				return FALSE;
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (!bRtn[i])
				return FALSE;
		}
	}

	return TRUE;
}

void CGvisR2R_LaserView::OpenReelmapInner()
{
	//GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (pDoc->m_pReelMapInnerUp)
		pDoc->m_pReelMapInnerUp->Open();

	if (pDoc->m_pReelMapIts)
		pDoc->m_pReelMapIts->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapInnerDn)
			pDoc->m_pReelMapInnerDn->Open();
		if (pDoc->m_pReelMapInnerAllUp)
			pDoc->m_pReelMapInnerAllUp->Open();
		if (pDoc->m_pReelMapInnerAllDn)
			pDoc->m_pReelMapInnerAllDn->Open();
	}

	if (pDoc->m_pReelMapInner)
	{
		if (pDoc->m_pReelMapInner->m_nLayer < 0)
			pDoc->m_pReelMapInner->m_nLayer = pView->m_nSelRmapInner;
		pDoc->m_pReelMapInner->Open();
	}

	//if (pDoc->m_pReelMapInOuterUp)
	//	pDoc->m_pReelMapInOuterUp->Open(GetRmapPath(RMAP_INOUTER_UP));

	//bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	//if (bDualTest)
	//{
	//	if (pDoc->m_pReelMapInOuterDn)
	//		pDoc->m_pReelMapInOuterDn->Open(GetRmapPath(RMAP_INOUTER_UP));
	//}
}

void CGvisR2R_LaserView::OpenReelmapInnerUp()
{
	//GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (pDoc->m_pReelMapInnerUp)
		pDoc->m_pReelMapInnerUp->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapInnerAllUp)
			pDoc->m_pReelMapInnerAllUp->Open();
	}

	if (pDoc->m_pReelMapInner)
	{
		if (pDoc->m_pReelMapInner->m_nLayer < 0)
			pDoc->m_pReelMapInner->m_nLayer = pView->m_nSelRmapInner;

		if (bDualTest)
		{
			if (pDoc->m_pReelMapInner->m_nLayer == RMAP_UP || pDoc->m_pReelMapInner->m_nLayer == RMAP_ALLUP)
				pDoc->m_pReelMapInner->Open();
		}
		else
		{
			if (pDoc->m_pReelMapInner->m_nLayer == RMAP_UP)
				pDoc->m_pReelMapInner->Open();
		}
	}
}

void CGvisR2R_LaserView::OpenReelmapInnerDn()
{
	//GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (!bDualTest)
		return;

	if (pDoc->m_pReelMapInnerDn)
		pDoc->m_pReelMapInnerDn->Open();
	if (pDoc->m_pReelMapInnerAllDn)
		pDoc->m_pReelMapInnerAllDn->Open();

	if (pDoc->m_pReelMapInner)
	{
		if (pDoc->m_pReelMapInner->m_nLayer < 0)
			pDoc->m_pReelMapInner->m_nLayer = pView->m_nSelRmapInner;

		if (pDoc->m_pReelMapInner->m_nLayer == RMAP_DN || pDoc->m_pReelMapInner->m_nLayer == RMAP_ALLDN)
			pDoc->m_pReelMapInner->Open();
	}
}


void CGvisR2R_LaserView::UpdateRstInner()
{
	//if (m_pDlgMenu06)
	//	m_pDlgMenu06->UpdateRst();
}

void CGvisR2R_LaserView::InitReelmapInner()
{
	pDoc->InitReelmapInner();
	pDoc->SetReelmapInner(ROT_NONE);
	// 	pDoc->SetReelmap(ROT_CCW_90);
	//pDoc->UpdateData();

	//pDoc->GetCurrentInfoEng();
	////if (pDoc->GetTestMode() == MODE_OUTER)
	//if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->m_sEngLayerUp))
	//{
	//	pDoc->InitReelmapInner();
	//	pDoc->SetReelmapInner(ROT_NONE);
	//}
}

void CGvisR2R_LaserView::InitReelmapInnerUp()
{
	pDoc->InitReelmapInnerUp();
	pDoc->SetReelmapInner(ROT_NONE);
	// 	pDoc->SetReelmap(ROT_CCW_90);
	//pDoc->UpdateData();

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//pDoc->GetCurrentInfoEng();
	//if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->m_sEngLayerUp))
	//{
	//	pDoc->InitReelmapInnerUp();

	//	pDoc->GetCurrentInfoEng();
	//	if (pDoc->m_bEngDualTest)
	//		pDoc->InitReelmapInnerDn();
	//}
}

void CGvisR2R_LaserView::InitReelmapInnerDn()
{
	//pDoc->GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	//if (!bDualTest)
	//	return;

	if (bDualTest)
	{
		pDoc->InitReelmapInnerDn();
		pDoc->SetReelmapInner(ROT_NONE);
		// 	pDoc->SetReelmap(ROT_CCW_90);
		//pDoc->UpdateData();
	}
}

void CGvisR2R_LaserView::DispDefImgInner()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	CString sNewLot;
	BOOL bNewModel = FALSE;
	int nSerial = 0;
	int nNewLot = 0;
	int nBreak = 0;

	switch (m_nStepTHREAD_DISP_DEF_INNER)
	{
		// CopyDefImg Start ============================================
	case 0:
		m_nStepTHREAD_DISP_DEF_INNER++;

		//if (bDualTest)
		//{
		//	nSerial = m_nBufDnSerial[0]; // 좌측 Camera
		//	sNewLot = m_sNewLotDn;
		//}
		//else
		//{
		//	nSerial = m_nBufUpSerial[0]; // 좌측 Camera
		//	sNewLot = m_sNewLotUp;
		//}

		//if (nSerial == pView->m_nLotEndSerial)
		//	nBreak = 1;

		//if (nSerial > 0)
		//{
		//	//if (!CopyDefImg(nSerial, sNewLot)) // 좌측 Camera
		//	//{
		//	//	sNewLot.Empty();
		//	//	m_bDispMsgDoAuto[7] = TRUE;
		//	//	m_nStepDispMsg[7] = FROM_DISPDEFIMG + 7;
		//	//	break;
		//	//}

		//	if (m_bLastProc && nSerial + 1 > m_nLotEndSerial)
		//	{
		//		if (bDualTest)
		//			nSerial = m_nBufDnSerial[0]; // Test
		//		else
		//			nSerial = m_nBufUpSerial[0]; // Test
		//	}
		//	else
		//	{
		//		if (bDualTest)
		//			nSerial = m_nBufDnSerial[1]; // 우측 Camera
		//		else
		//			nSerial = m_nBufUpSerial[1]; // 우측 Camera

		//		//if (nSerial > 0)
		//		//{
		//		//	if (!CopyDefImg(nSerial, sNewLot)) // 우측 Camera
		//		//	{
		//		//		sNewLot.Empty();
		//		//		m_bDispMsgDoAuto[6] = TRUE;
		//		//		m_nStepDispMsg[6] = FROM_DISPDEFIMG + 6;
		//		//		break;
		//		//	}
		//		//}
		//	}

		//}
		//else
		//{
		//	if (!m_bLastProc)
		//	{
		//		m_bDispMsgDoAuto[5] = TRUE;
		//		m_nStepDispMsg[5] = FROM_DISPDEFIMG + 5;
		//	}
		//}
		//sNewLot.Empty();
		break;
	case 1:
		Sleep(300);
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 2:
		if (bDualTest)
			nSerial = m_nBufDnSerial[0]; // 좌측 Camera
		else
			nSerial = m_nBufUpSerial[0]; // 좌측 Camera

		if (IsDoneDispMkInfoInner())	 // Check 불량이미지 Display End
		{
			if (ChkLastProc() && (nSerial > m_nLotEndSerial))
			{
				SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(좌) Display Start
				SetSerialMkInfoInner(nSerial, TRUE);		// 불량이미지(좌) Display Start
			}
			else if (ChkLastProc() && nSerial < 1)
			{
				SetSerialReelmapInner(m_nLotEndSerial + 1, TRUE);		// Reelmap(좌) Display Start
				SetSerialMkInfoInner(m_nLotEndSerial + 1, TRUE);		// 불량이미지(좌) Display Start
			}
			else
			{
				//SetFixPcs(nSerial);
				SetSerialReelmapInner(nSerial);	// Reelmap(좌) Display Start
				SetSerialMkInfoInner(nSerial);	// 불량이미지(좌) Display Start
			}

			//if (IsFixPcsUp(nSerial))
			//{
			//	m_bDispMsgDoAuto[2] = TRUE;
			//	m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
			//}
			//if (IsFixPcsDn(nSerial))
			//{
			//	m_bDispMsgDoAuto[3] = TRUE;
			//	m_nStepDispMsg[3] = FROM_DISPDEFIMG + 3;
			//}

			m_nStepTHREAD_DISP_DEF_INNER++;
		}

		break;

	case 3:
		if (IsDoneDispMkInfoInner())	 // Check 불량이미지(좌) Display End
		{
			if (bDualTest)
				nSerial = m_nBufDnSerial[1]; // 우측 Camera
			else
				nSerial = m_nBufUpSerial[1]; // 우측 Camera

			if (nSerial > 0)
			{
				if (ChkLastProc() && (nSerial > m_nLotEndSerial))
					SetSerialMkInfoInner(nSerial, TRUE);	// 불량이미지(우) Display Start
				else
					SetSerialMkInfoInner(nSerial);		// 불량이미지(우) Display Start
			}
			else
			{
				if (ChkLastProc())
					SetSerialMkInfoInner(m_nLotEndSerial + 1, TRUE);	// 불량이미지(우) Display Start
			}
			m_nStepTHREAD_DISP_DEF_INNER++;
		}
		break;
	case 4:
		if (bDualTest)
			nSerial = m_nBufDnSerial[1]; // 우측 Camera
		else
			nSerial = m_nBufUpSerial[1]; // 우측 Camera

		if (nSerial > 0)
		{
			m_nStepTHREAD_DISP_DEF_INNER++;

			if (ChkLastProc() && (nSerial > m_nLotEndSerial))
			{
				SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(우) Display Start
			}
			else
			{
				//SetFixPcs(nSerial);
				SetSerialReelmapInner(nSerial);			// Reelmap(우) Display Start
			}

			//if (bDualTest)
			//{
			//	if (IsFixPcsUp(nSerial))
			//	{
			//		m_bDispMsgDoAuto[2] = TRUE;
			//		m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
			//	}
			//	if (IsFixPcsDn(nSerial))
			//	{
			//		m_bDispMsgDoAuto[3] = TRUE;
			//		m_nStepDispMsg[3] = FROM_DISPDEFIMG + 3;
			//	}
			//}
			//else
			//{
			//	if (IsFixPcsUp(nSerial))
			//	{
			//		m_bDispMsgDoAuto[2] = TRUE;
			//		m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
			//	}
			//}
		}
		else
		{
			if (ChkLastProc())
			{
				m_nStepTHREAD_DISP_DEF_INNER++;
				SetSerialReelmapInner(m_nLotEndSerial + 1, TRUE);	// 불량이미지(우) Display Start
			}
			else
			{
				if (bDualTest)
				{
					if (m_bLastProc && m_nBufDnSerial[0] == m_nLotEndSerial)
						m_nStepTHREAD_DISP_DEF_INNER++;
					else
					{
						m_nStepTHREAD_DISP_DEF_INNER++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[0] == m_nLotEndSerial)
						m_nStepTHREAD_DISP_DEF_INNER++;
					else
					{
						m_nStepTHREAD_DISP_DEF_INNER++;
					}
				}
			}
		}
		break;
	case 5:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 6:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 7:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 8:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 9:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 10:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;

	case 11:
		if (IsDoneDispMkInfoInner() && IsRun())	 // Check 불량이미지(우) Display End
			m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 12:
		m_bTHREAD_DISP_DEF_INNER = FALSE;
		break;
		// Disp DefImg End ============================================
	}

}

BOOL CGvisR2R_LaserView::IsDoneDispMkInfoInner()
{
	return TRUE;
	//BOOL bRtn = FALSE;
	//if (m_pDlgMenu06)
	//	bRtn = m_pDlgMenu06->IsDoneDispMkInfo();
	//return bRtn;
}

BOOL CGvisR2R_LaserView::SetSerialReelmapInner(int nSerial, BOOL bDumy)
{
	return TRUE;
	//if (!m_pDlgMenu06)
	//{
	//	pView->ClrDispMsg();
	//	AfxMessageBox(_T("Error - SetSerialReelmap : m_pDlgMenu06 is NULL."));
	//	return FALSE;
	//}

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//{
	//	if (!m_pDlgMenu06)
	//	{
	//		pView->ClrDispMsg();
	//		AfxMessageBox(_T("Error - SetSerialReelmap : m_pDlgMenu06 is NULL."));
	//		return FALSE;
	//	}

	//	m_pDlgMenu06->SetSerialReelmap(nSerial, bDumy);
	//}

	//return m_pDlgMenu06->SetSerialReelmap(nSerial, bDumy);
}

BOOL CGvisR2R_LaserView::SetSerialMkInfoInner(int nSerial, BOOL bDumy)
{
	return TRUE;
	//if (!m_pDlgMenu06)
	//	return FALSE;
	//return m_pDlgMenu06->SetSerialMkInfo(nSerial, bDumy);
}

void CGvisR2R_LaserView::ChkRcvSig()
{
	int i = 0;
	for (i = 0; i < _SigInx::_EndIdx; i++)
	{
		if (m_bRcvSig[i])
		{
			m_bRcvSig[i] = FALSE;
			switch (i)
			{
			case _SigInx::_UpdateWork:
				GetMkMenu01();
				break;
			case _SigInx::_Stop:
				EngStop(pDoc->BtnStatus.Main.Stop);
				break;
			case _SigInx::_EngAutoInit:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwReset();
				break;
			case _SigInx::_MyMsgYes:
				SetMyMsgYes();
				break;
			case _SigInx::_MyMsgNo:
				SetMyMsgNo();
				break;
			case _SigInx::_MyMsgOk:
				SetMyMsgOk();
				break;
			case _SigInx::_TestMode:
				pDoc->SetCurrentInfoTestMode(pDoc->WorkingInfo.LastJob.nTestMode);
				break;
			case _SigInx::_TempPause:
				::WritePrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), pDoc->WorkingInfo.LastJob.bTempPause ? _T("1") : _T("0"), PATH_WORKING_INFO);
#ifdef USE_MPE
				if (pView && pView->m_pMpe)
					pView->m_pMpe->Write(_T("MB440183"), pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0);
#endif
				break;
			default:
				break;
			}
		}
	}
}