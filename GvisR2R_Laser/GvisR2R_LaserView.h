
// GvisR2R_LaserView.h : CGvisR2R_LaserView 클래스의 인터페이스
//

#pragma once

#include "Global/GlobalDefine.h"

#include "Dialog/DlgMyMsg.h"
#include "Dialog/DlgMsgBox.h"
#include "Dialog/DlgFrameHigh.h"
#include "Dialog/DlgInfo.h"
#include "Dialog/DlgMenu01.h"
#include "Dialog/DlgMenu02.h"
#include "Dialog/DlgMenu03.h"
#include "Dialog/DlgMenu04.h"
#include "Dialog/DlgMenu05.h"
#include "Dialog/DlgUtil01.h"
#include "Dialog/DlgUtil02.h"

#include "Device/Motion.h"
#include "Device/Light.h"
#include "Device/Vision.h"
#include "Device/MpDevice.h"
//#include "Device/TcpIpClient.h"
#include "Device/SR1000W.h"
#include "Device/MDX2500.h"
#include "Device/Engrave.h"

#include "Process/PtAlign.h"
#include "Process/ThreadTask.h"

#include "GvisR2R_LaserDoc.h"



#define LAMP_DELAY				5	// 5 * 100[mSec]
#define BUZZER_DELAY			2	// 5 * 100[mSec]

#define FROM_DOMARK0			10
#define FROM_DOMARK1			50
#define FROM_DISPDEFIMG			100

#define AT_LP					10
#define MK_DONE_CHECK			30	// m_nStepMk
#define MK_LIGHT_ERR			50	// m_nStepMk
#define MK_END					100	// m_nStepMk

#define MK_ST					100	// m_nMkStAuto
#define ERR_PROC				120	// m_nMkStAuto
#define REJECT_ST				200	// m_nMkStAuto
#define ERROR_ST				250	// m_nMkStAuto

#define MK_VERIFY				200	// m_nSetpAuto
#define MK_REVIEW				220	// m_nSetpAuto
#define LAST_PROC				250	// m_nSetpAuto
#define LAST_PROC_VS_ALL		300	// m_nSetpAuto
#define LAST_PROC_VS_UP			350	// m_nSetpAuto
#define LAST_PROC_VS_DN			400	// m_nSetpAuto
#define LOT_END					500	// m_nSetpAuto

#define ENG_ST					100	// m_nEngStAuto
#define ENG_2D_ST				150	// m_nEngStAuto

#define TIM_INIT_VIEW			0
#define TIM_TOWER_WINKER		10
#define TIM_BTN_WINKER			11
#define TIM_BUZZER_WARN			12
#define TIM_MB_TIME_OUT			13
#define TIM_DISP_STATUS			14
#define TIM_MPE_IO				15
// #define TIM_MK_START			16
#define TIM_SHOW_MENU01			18
#define TIM_SHOW_MENU02			19
#define TIM_CHK_TEMP_STOP		20
#define TIM_SAFTY_STOP			21
#define TIM_TCPIP_UPDATE		22
#define TIM_START_UPDATE		100

namespace Read2dIdx
{
	typedef enum Index {
		Start = 2, ChkSn = 4, InitRead = 10, Move0Cam1 = 12, Move0Cam0 = 14, Align1_0 = 17, Align0_0 = 18,
		Move1Cam1 = 21, Move1Cam0 = 23, Align1_1 = 26, Align0_1 = 27, MoveInitPt = 29, ChkElec = 32, DoRead = 35,
		Verify = 37, DoneRead = 38, LotDiff = 50
	};
}

namespace Mk1PtIdx
{
	typedef enum Index {
		Start = 2, ChkSn = 4, InitMk = 10, Move0Cam1 = 12, Move0Cam0 = 14, Align1_0 = 17, Align0_0 = 18,
		Move1Cam1 = 21, Move1Cam0 = 23, Align1_1 = 26, Align0_1 = 27, MoveInitPt = 29, ChkElec = 32, DoMk = 35,
		Verify = 37, DoneMk = 38, LotDiff = 50
	};
}

namespace Mk2PtIdx
{
	typedef enum Index {
		Start = 2, ChkSn = 4, InitMk = 10, Move0Cam1 = 12, Move0Cam0 = 14, Align1_0 = 17, Align0_0 = 18,
		Move1Cam1 = 21, Move1Cam0 = 23, Align1_1 = 26, Align0_1 = 27, MoveInitPt = 29, ChkElec = 32, DoMk = 35,
		Verify = 37, DoneMk = 38, LotDiff = 50
	};
}

namespace Mk4PtIdx
{
	typedef enum Index {
		Start = 2, ChkSn = 4, InitMk = 10, Move0Cam1 = 12, Move0Cam0 = 14, Align1_0 = 17, Align0_0 = 18,
		Move1Cam1 = 21, Move1Cam0 = 23, Align1_1 = 26, Align0_1 = 27, Move2Cam1 = 29, Move2Cam0 = 31,
		Align1_2 = 34, Align0_2 = 35, Move3Cam1 = 37, Move3Cam0 = 39, Align1_3 = 42, Align0_3 = 43,
		MoveInitPt = 45, ChkElec = 48, DoMk = 51,
		Verify = 53, DoneMk = 54, LotDiff = 60
	};
}

typedef struct _DispMain
{
	CString strMsg;
	COLORREF rgb;

	_DispMain()
	{
		Init();
	}
	_DispMain(CString sMsg, COLORREF Rgb)
	{
		strMsg = sMsg;
		rgb = Rgb;
	}

	void Init()
	{
		strMsg = _T("");
		rgb = RGB_WHITE;
	}

}stDispMain;
typedef CArray<stDispMain, stDispMain> CArDispMain;




class CGvisR2R_LaserView : public CFormView
{
	BOOL m_bEngStop;

	BOOL m_bDestroyedView;
	BOOL m_bBufEmpty[2];

	CDlgMyMsg* m_pDlgMyMsg;
	CCriticalSection m_csMyMsgBox;
	CCriticalSection m_csDispMain;
	BOOL m_bAnswer[10];
	int m_nDummy[2], m_nAoiLastSerial[2]; //[0]: Up, [1]: Dn
	BOOL m_bChkLastProcVs;
	int m_nChkBufIdx[2];

	double m_dTempPauseLen;
	DWORD m_dwCycSt, m_dwCycTim;

	BOOL m_bRtnMyMsgBox[4]; 	// [0] mk0, [1] mk1, [2] reject0, [3] reject1
	int m_nRtnMyMsgBox[4]; 	// [0] mk0, [1] mk1, [2] reject0, [3] reject1
	int m_nRtnMyMsgBoxIdx;

	int m_nPrevStepAuto, m_nPrevMkStAuto;
	int m_nStepMk[4], m_nMkPcs[4]; 	// [0] Auto-Left, [1] Auto-Right, [2] Manual-Left, [3] Manual-Right 
	int m_nMkStrip[2][4]; // [nCam][nStrip]
	int m_nErrCnt;
	int m_nStepInitView;

	BOOL m_bDispMsg, m_bWaitClrDispMsg;
	BOOL m_bDispMsgDoAuto[10];
	int m_nStepDispMsg[10];
	CString m_sFixMsg[2]; //[0]:up , [1]:dn

	int m_nCntTowerWinker, m_nCntBtnWinker[4], m_nDlyWinker[4], m_nCntBz;
	BOOL m_bTimTowerWinker, m_bTimBtnWinker, m_bTimBuzzerWarn;
	BOOL m_bTowerWinker[3], m_bBtnWinker[4]; // [R/G/Y] , [Ready/Reset/Run/Stop]
	BOOL m_bTIM_DISP_STATUS, m_bTIM_MPE_IO;

	CString m_sPrevMyMsg;
	//stMyMsgBox stMyMsgData;
	CString m_sTick, m_sDispTime;
	DWORD m_dwSetDlySt[10], m_dwSetDlyEd[10];
	DWORD m_dwSetDlySt0[10], m_dwSetDlyEd0[10];
	DWORD m_dwSetDlySt1[10], m_dwSetDlyEd1[10];

	CString m_sShare[2], m_sBuf[2]; // [0]: AOI-Up , [1]: AOI-Dn
	int m_pBufSerial[2][100], m_nBufTot[2]; // [0]: AOI-Up , [1]: AOI-Dn
	__int64 m_nBufSerialSorting[2][100]; // [0]: AOI-Up , [1]: AOI-Dn
	int m_nBufSerial[2][2]; // [0]: AOI-Up , [1]: AOI-Dn // [0]: Cam0, [1]:Cam1

	CString m_sDispMsg[10];
	double m_dTotVel, m_dPartVel;
	BOOL m_bTIM_CHK_TEMP_STOP;
	BOOL m_bTIM_SAFTY_STOP;
	BOOL m_bTIM_TCPIP_UPDATE;
	BOOL m_bTIM_START_UPDATE;
	CString m_sMyMsg; int m_nTypeMyMsg;
	int m_nVsBufLastSerial[2];
	BOOL m_bOpenShareUp, m_bOpenShareDn;

	BOOL m_bStopFeeding;
	BOOL m_bChkLightErr;

	int m_nTotMk[2], m_nCurMk[2]; // [0]: 좌 MK, [1]: 우 MK
	int m_nPrevTotMk[2], m_nPrevCurMk[2]; // [0]: 좌 MK, [1]: 우 MK

	BOOL m_bContEngraveF;


	void InitMyMsg();
	void CloseMyMsg();

	void DispStsBar();
	void UpdateLotTime();

	void SetMainMc(BOOL bOn);
	void ExitProgram();
	void HideAllDlg();
	void DelAllDlg();
	BOOL HwInit();
	BOOL TcpIpInit();
	void InitPLC();
	void HwKill();
	void ThreadInit();
	void ThreadKill();
	//void GetSliceIO();
	void GetMpeIO();
	void GetMpeSignal();
	void GetMpeData();
	//void DispThreadTick();
	void DispTime();
	void Init();
	void InitIO();
	BOOL InitAct();
	void SwJog(int nAxisID, int nDir, BOOL bOn = TRUE);
	int GetVsBufLastSerial();
	int GetVsUpBufLastSerial();
	int GetVsDnBufLastSerial();

	void MarkingWith2PointAlign();
	void Mk2PtReady();
	void Mk2PtChkSerial();
	void Mk2PtInit();
	void Mk2PtAlignPt0();
	void Mk2PtAlignPt1();
	void Mk2PtMoveInitPos();
	void Mk2PtElecChk();
	void Mk2PtDoMarking();
	void Mk2PtLotDiff();
	void Mk2PtReject();
	void Mk2PtErrStop();

	void MarkingWith4PointAlign();
	void Mk4PtReady();
	void Mk4PtChkSerial();
	void Mk4PtInit();
	void Mk4PtAlignPt0();
	void Mk4PtAlignPt1();
	void Mk4PtAlignPt2();
	void Mk4PtAlignPt3();
	void Mk4PtMoveInitPos();
	void Mk4PtElecChk();
	void Mk4PtDoMarking();
	void Mk4PtLotDiff();
	void Mk4PtReject();
	void Mk4PtErrStop();

	void DoBoxSw();
	void DoEmgSw();
	void DoSens();
	void DoInterlock();

	void DoSaftySens();
	void DoDoorSens();

	void DoModeSel();		// SliceIo[0]
	void DoMainSw();		// SliceIo[0]
	void DoMkSens();		// SliceIo[2]
	void DoAoiBoxSw();		// SliceIo[3]
	void DoEmgSens();		// SliceIo[4]
	void DoSignal();		// SliceIo[5]

	void DoUcBoxSw();		// MpeIo[0]
	void DoUcSens1();		// MpeIo[1]
	void DoUcSens2();		// MpeIo[2]
	void DoUcSens3();		// MpeIo[3]
	void DoRcBoxSw();		// MpeIo[4]
	void DoRcSens1();		// MpeIo[5]
	void DoRcSens2();		// MpeIo[6]
	void DoRcSens3();		// MpeIo[7]

	BOOL SetCollision(double dCollisionMargin);
	void DispStsMainMsg(int nIdx = 0);
	void SetPlcParam();


	BOOL SortingInUp(CString sPath, int nIndex);
	BOOL SortingOutUp(int* pSerial, int nTot);
	void SwapUp(__int64 *num1, __int64 *num2);
	BOOL SortingInDn(CString sPath, int nIndex);
	BOOL SortingOutDn(int* pSerial, int nTot);
	void SwapDn(__int64 *num1, __int64 *num2);

	void MoveInitPos0(BOOL bWait=TRUE);

	void DoAutoEng();
	void DoAutoMarking();
	void DoAtuoGetEngStSignal();
	void DoAtuoGet2dReadStSignal();
	void DoAuto2dReading();
	//void DoAutoDispMsg();
	//void DoAutoChkCycleStop();
	//void DoAutoSetFdOffset();
	//void DoAutoSetFdOffsetLastProc();
	//void DoAutoSetLastProcAtPlc();
	//BOOL DoAutoGetLotEndSignal();

protected: // serialization에서만 만들어집니다.
	CGvisR2R_LaserView();
	DECLARE_DYNCREATE(CGvisR2R_LaserView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_GVISR2R_LASER_FORM };
#endif

// 특성입니다.
public:
	CGvisR2R_LaserDoc* GetDocument() const;

	BOOL m_bStopFromThread, m_bBuzzerFromThread;

	CMpDevice* m_pMpe;
	CPtAlign m_Align[2];	// [0] : LeftCam , [1] : RightCam
#ifdef USE_VISION
	CVision* m_pVision[2];	// [0] : LeftCam , [1] : RightCam
#endif
	//CPtAlign m_Align;
	//CVision* m_pVision;
	CMotion* m_pMotion;
	CLight* m_pLight;
	CDlgMsgBox* m_pDlgMsgBox;
	CEvent      m_evtWaitClrDispMsg;

	CDlgInfo *m_pDlgInfo;
	CDlgFrameHigh *m_pDlgFrameHigh;
	CDlgMenu01 *m_pDlgMenu01;
	CDlgMenu02 *m_pDlgMenu02;
	CDlgMenu03 *m_pDlgMenu03;
	CDlgMenu04 *m_pDlgMenu04;
	CDlgMenu05 *m_pDlgMenu05;
	CDlgUtil01 *m_pDlgUtil01;
	//CDlgUtil02 *m_pDlgUtil02;


	int m_nLotEndSerial;

	BOOL m_bTIM_INIT_VIEW;
	BOOL m_bCam, m_bReview;

	DWORD m_dwThreadTick[3];
	BOOL m_bThread[3];
	CThreadTask m_Thread[3];

	double m_dEnc[MAX_AXIS], m_dTarget[MAX_AXIS];
	double m_dNextTarget[MAX_AXIS];
	int m_nSelRmap;
	int m_nStepAuto;

	int m_nStop;
	BOOL m_bReMk;
	BOOL m_bChkMpeIoOut;

	BOOL m_bProbDn[2]; // 좌/우 .

					   // Auto Sequence
	BOOL m_bAuto, m_bManual, m_bOneCycle;
	BOOL m_bMkTmpStop, m_bAoiLdRun, m_bAoiLdRunF;
	BOOL m_bTHREAD_MK[4];	// [0] Auto-Left, [1] Auto-Right, [2] Manual-Left, [3] Manual-Right
	BOOL m_bTHREAD_DISP_DEF;
	int	m_nStepTHREAD_DISP_DEF;

	BOOL m_bSwRun, m_bSwRunF;
	BOOL m_bSwStop, m_bSwStopF;
	BOOL m_bSwReset, m_bSwResetF;
	BOOL m_bSwReady, m_bSwReadyF;

	BOOL m_bNewModel;
	DWORD m_dwLotSt, m_dwLotEd;
	long m_lFuncId;
	BOOL m_bDrawGL;
	BOOL m_bCont;
	DWORD m_dwStMkDn[2];
	BOOL m_bInit;
	BOOL m_bSwStopNow;
	BOOL m_bShowModalMyPassword;

	int m_nRstNum;

	BOOL m_bChkBufIdx[2];
	BOOL m_bBufHomeDone, m_bReadyDone;
	BOOL m_bEngBufHomeDone;

	unsigned long m_Flag;
	unsigned long m_AoiLdRun;
	BOOL m_bDoneDispMkInfo[2][2]; // [nCam][Up/Dn]

	int m_nShareUpS;
	int m_nShareUpSerial[2]; // [nCam]
	int m_nShareDnS;
	int m_nShareDnSerial[2]; // [nCam]
	int m_nShareUpCnt;
	int m_nShareDnCnt;

	int m_nBufUpSerial[2]; // [nCam]
	int m_nBufDnSerial[2]; // [nCam]
	int m_nBufUpCnt;
	int m_nBufDnCnt;

	//BOOL m_bFailAlign[2][2]; // [nCam][nPos] 
	//BOOL m_bReAlign[2][2]; // [nCam][nPos] 
	//BOOL m_bSkipAlign[2][2]; // [nCam][nPos] 
	BOOL m_bFailAlign[2][4]; // [nCam][nPos] 
	BOOL m_bReAlign[2][4]; // [nCam][nPos] 
	BOOL m_bSkipAlign[2][4]; // [nCam][nPos] 

	BOOL m_bDoMk[2], m_bDoneMk[2]; // [nCam]
	BOOL m_bReMark[2]; // [nCam]

	int m_nMonAlmF, m_nClrAlmF;
	BOOL m_bLotEnd, m_bLastProc, m_bLastProcFromUp;
	BOOL m_bMkSt, m_bMkStSw;
	BOOL m_bEngSt, m_bEngStSw;
	BOOL m_bEng2dSt, m_bEng2dStSw;
	int m_nMkStAuto, m_nEngStAuto, m_nEng2dStAuto;
	int m_nLotEndAuto, m_nLastProcAuto;
	BOOL m_bLoadShare[2]; // [Up/Dn]
	CString m_sNewLotUp, m_sNewLotDn;

	BOOL m_bAoiFdWrite[2], m_bAoiFdWriteF[2]; // [Up/Dn]
	BOOL m_bAoiTest[2], m_bAoiTestF[2], m_bWaitPcr[2]; // [Up/Dn]

	BOOL m_bCycleStop, m_bContDiffLot;
	CString m_sDispMain;
	BOOL m_bStopF_Verify;

	BOOL m_bShowMyMsg;
	CWnd *m_pMyMsgForeground;

	BOOL m_bRejectDone[2][4]; // Shot[2], Strip[4]

	CString m_sDispSts[2];

	BOOL m_bDispMyMsgBox;
	CArMyMsgBox  m_ArrayMyMsgBox;
	BOOL m_bDispMain;
	CArDispMain  m_ArrayDispMain;

	int m_nWatiDispMain;

	CSr1000w* m_pSr1000w;
	CMdx2500* m_pMdx2500;
	CEngrave* m_pEngrave;

	int m_nNewLot;
	CString m_sMonDisp;

// 작업입니다.
public:
	afx_msg LRESULT OnDlgInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMsgExit(WPARAM wPara, LPARAM lPara);

	int MsgBox(CString sMsg, int nThreadIdx = 0, int nType = MB_OK, int nTimOut = DEFAULT_TIME_OUT);		// SyncMsgBox
	int AsyncMsgBox(CString sMsg, int nThreadIdx = 1, int nType = MB_OK, int nTimOut = DEFAULT_TIME_OUT); // AsyncMsgBox
	int WaitRtnVal(int nThreadIdx = 1);
	void Buzzer(BOOL bOn, int nCh = 0);
	void StopFromThread();
	void BuzzerFromThread(BOOL bOn, int nCh = 0);


	void GetPlcParam();
	BOOL WatiDispMain(int nDelay);

	void RestoreReelmap();
	CString GetProcessNum();

	CString GetRmapPath(int nRmap, stModelInfo stInfo);
	CString GetRmapPath(int nRmap);
	void DispMain(CString sMsg, COLORREF rgb = RGB(0, 255, 0));
	int DoDispMain();
	void GetDispMsg(CString &strMsg, CString &strTitle);
	void DispMsg(CString strMsg, CString strTitle = _T(""), COLORREF color = RGB(255, 0, 0), DWORD dwDispTime = 0, BOOL bOverWrite = TRUE);
	void ClrDispMsg();
	BOOL WaitClrDispMsg();
	LONG OnQuitDispMsg(UINT wParam, LONG lParam);
	void ShowDlg(int nID);
	void DispIo();
	void DispDatabaseConnection();

	void TowerLamp(COLORREF color, BOOL bOn, BOOL bWink = FALSE);
	void DispTowerWinker();
	int MyPassword(CString strMsg, int nCtrlId = 0);

	void GetEnc();
	void SetAoiFdPitch(double dPitch);
	void SetMkFdPitch(double dPitch);

	void DoIO();
	void DispDefImg();

	//void DoMark0();
	//void DoMark1();
	//void DoMark0All();
	//void DoMark1All();

	BOOL ChkVsShare(int &nSerial);
	BOOL ChkVsShareUp(int &nSerial);
	BOOL ChkVsShareDn(int &nSerial);
	void ChkShare();
	void ChkShareUp();
	void ChkShareDn();
	BOOL ChkShare(int &nSerial);
	BOOL ChkShareUp(int &nSerial);
	BOOL ChkShareDn(int &nSerial);
	BOOL ChkShareIdx(int *pBufSerial, int nBufTot, int nShareSerial);
	void ChkBuf();
	void ChkBufUp();
	void ChkBufDn();
	void ChkEmg();
	BOOL ChkBufUp(int* pSerial, int &nTot);
	BOOL ChkBufDn(int* pSerial, int &nTot);
	BOOL ChkBufIdx(int* pSerial, int nTot = 0);
	void ChkMRegOut();


	CString GetTime();
	CString GetTime(stLotTime &LotTime);
	CString GetTime(int &nHour, int &nMinute, int &nSec);
	int GetTime(int nSel);
	void SetBufInitPos(double dPos);
	void SetBufHomeParam(double dVel, double dAcc);
	void DispLotStTime();
	void SetListBuf();

	static UINT ThreadProc0(LPVOID lpContext); // Safety check thread procedure
	static UINT ThreadProc1(LPVOID lpContext); // Safety check thread procedure
	static UINT ThreadProc2(LPVOID lpContext); // Safety check thread procedure

	// Auto Sequence
	//void SetTest(BOOL bOn);
	//void SetTest0(BOOL bOn);
	//void SetTest1(BOOL bOn);
	//BOOL SetMk(BOOL bRun = TRUE);
	//void SetReMk(BOOL bMk0 = FALSE, BOOL bMk1 = FALSE);
	//BOOL InitMk();
	//void InitAuto(BOOL bInit = TRUE);
	//void Mk1();
	//BOOL IsMk();
	//BOOL IsReMk();
	//BOOL IsMkDone();
	//void Marking();
	//void DoReject0();

	BOOL IsReady();				// not used
	void Shift2Buf();
	void Shift2Mk();
	void SetTestSts(int nStep);
	void SetMkSts(int nStep);
	void SetAoiFdSts();
	void SetAoiStopSts();
	void SetMkFdSts();
	void SetMkStopSts();
	BOOL IsMkFdSts();		// not used
	void SetAoiFd();			// not used
	void SetMkFd();				// not used
	BOOL IsMkFd();				// not used
	BOOL IsAoiFd();				// not used
	void SetMkFd(double dDist);
	void SetDelay(int mSec, int nId = 0);
	BOOL WaitDelay(int nId = 0);				// F:Done, T:On Waiting....
	void SetDelay0(int mSec, int nId = 0);
	BOOL WaitDelay0(int nId = 0);				// F:Done, T:On Waiting....
	BOOL GetDelay(int &mSec, int nId = 0);	// F:Done, T:On Waiting....
	BOOL GetDelay0(int &mSec, int nId = 0);	// F:Done, T:On Waiting....
	BOOL IsMkFdDone();
	BOOL IsAoiFdDone();
	double GetAoi2InitDist();
	double GetMkInitDist();
	void UpdateWorking();
	double GetMkRemain();
	void Stop();
	void ShowLive(BOOL bShow = TRUE);
	void SetLotSt();
	void SetLotEd();
	void DispLotTime();

	BOOL GetDelay1(int &mSec, int nId = 0);	// F:Done, T:On Waiting....
	void SetDelay1(int mSec, int nId = 0);
	BOOL WaitDelay1(int nId = 0);				// F:Done, T:On Waiting....

	BOOL IsAoiTblVac();
	BOOL IsAoiTblVacDone();
	BOOL IsTest();
	BOOL IsTestUp();
	BOOL IsTestDn();
	BOOL IsTestDone();
	BOOL IsTestDoneUp();
	BOOL IsTestDoneDn();
	BOOL IsStop();
	BOOL IsRun();

	int GetSerial();
	void SetMkFdLen();
	double GetMkFdLen();
	double GetTotVel();
	double GetPartVel();
	void SetCycTime();
	int GetCycTime(); // [mSec]

	BOOL IsShare();
	BOOL IsShareUp();
	BOOL IsShareDn();
	// 	BOOL IsRst();
	BOOL IsVsShare();
	// 	int GetShare();
	int GetShareUp();
	int GetShareDn();

	BOOL IsBuffer(int nNum = 0);
	BOOL IsBufferUp();
	BOOL IsBufferDn();
	int GetBuffer(int *pPrevSerial = NULL);
	int GetBufferUp(int *pPrevSerial = NULL);
	int GetBufferDn(int *pPrevSerial = NULL);

	BOOL IsBuffer0();
	BOOL IsBufferUp0();
	BOOL IsBufferDn0();
	int GetBuffer0(int *pPrevSerial = NULL);
	int GetBufferUp0(int *pPrevSerial = NULL);
	int GetBufferDn0(int *pPrevSerial = NULL);

	BOOL IsBuffer1();
	BOOL IsBufferUp1();
	BOOL IsBufferDn1();
	int GetBuffer1(int *pPrevSerial = NULL);
	int GetBufferUp1(int *pPrevSerial = NULL);
	int GetBufferDn1(int *pPrevSerial = NULL);

	//BOOL ChkMkTmpStop();
	//BOOL IsMkTmpStop();
	BOOL IsAuto();
	BOOL SetSerial(int nSerial, BOOL bDumy = FALSE);

	int GetErrCode(int nSerial);
	int GetErrCodeUp(int nSerial);
	int GetErrCodeDn(int nSerial);

	int GetErrCode0(int nSerial);
	int GetErrCodeUp0(int nSerial);
	int GetErrCodeDn0(int nSerial);

	int GetErrCode1(int nSerial);
	int GetErrCodeUp1(int nSerial);
	int GetErrCodeDn1(int nSerial);

	int GetTotDefPcs(int nSerial);
	int GetTotDefPcsUp(int nSerial);
	int GetTotDefPcsDn(int nSerial);

	int GetTotDefPcs0(int nSerial);
	int GetTotDefPcsUp0(int nSerial);
	int GetTotDefPcsDn0(int nSerial);

	int GetTotDefPcs1(int nSerial);
	int GetTotDefPcsUp1(int nSerial);
	int GetTotDefPcsDn1(int nSerial);

	CfPoint GetMkPnt(int nMkPcs);
	CfPoint GetMkPnt0(int nMkPcs);
	CfPoint GetMkPnt1(int nMkPcs);
	CfPoint GetMkPnt0(int nSerial, int nMkPcs);
	CfPoint GetMkPnt1(int nSerial, int nMkPcs);

	//void Move(CfPoint pt, BOOL bCam=FALSE);
	void Move0(CfPoint pt, BOOL bCam = FALSE);
	BOOL IsMoveDone();
	BOOL IsMoveDone0();
	//void Ink(BOOL bOn = TRUE);
	BOOL UpdateReelmap(int nSerial);

	void InitInfo();
	void InitReelmap();
	void InitReelmapUp();
	void InitReelmapDn();
	BOOL IsPinMkData();
	BOOL IsPinData();
	BOOL CopyDefImg(int nSerial);
	BOOL CopyDefImg(int nSerial, CString sNewLot);
	BOOL CopyDefImgUp(int nSerial, CString sNewLot = _T(""));
	BOOL CopyDefImgDn(int nSerial, CString sNewLot = _T(""));

	BOOL MovePinPos();
	//BOOL MoveAlign(int nPos);
	BOOL OnePointAlign(CfPoint &ptPnt);
	//BOOL TwoPointAlign();
	//BOOL IsNoMk();
	BOOL StartLive();
	BOOL StopLive();
	//void MoveInitPos();
	//BOOL IsInitPos();

	void MoveMkInitPos();
	void ResetMkInfo(int nAoi = 0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
	void ClrMkInfo();

	BOOL IsHomeDone(int nMsId);
	BOOL GetAoiUpInfo(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	BOOL GetAoiDnInfo(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	BOOL ChkLotEnd(int nSerial);
	BOOL ChkLotEndUp(int nSerial);
	BOOL ChkLotEndDn(int nSerial);
	BOOL LoadMySpec();
	BOOL MemChk();
	BOOL GetAoiUpOffset(CfPoint &OfSt);
	BOOL GetAoiDnOffset(CfPoint &OfSt);
	BOOL GetMkOffset(CfPoint &OfSt);
	BOOL IsAoiLdRun();


	void LotEnd();
	void Winker(int nId, int nDly = 20); // 0:Ready, 1:Reset, 2:Run, 3:Stop
	void ResetWinker(); // 0:Ready, 1:Reset, 2:Run, 3:Stop
	void SetOrigin();
	void SetLotEnd(int nSerial);
	int GetLotEndSerial();
	void ModelChange(int nAoi = 0); // 0 : AOI-Up , 1 : AOI-Dn
	void UpdateRst();
	int GetAutoStep();
	void TimWinker(int nId, int nDly = 5);
	void StopTimWinker(int nId);
	BOOL IsShowLive();
	BOOL IsChkTmpStop();
	BOOL ChkLastProc();
	double GetAoiUpFdLen();
	double GetAoiDnFdLen();
	BOOL IsVerify();
	//BOOL IsFixPcs();
	//BOOL IsFixPcsUp();
	//BOOL IsFixPcsDn();
	BOOL IsFixPcsUp(int nSerial);
	BOOL IsFixPcsDn(int nSerial);
	BOOL IsReview();
	BOOL IsReview0();
	BOOL IsReview1();
	BOOL IsJogRtDn();
	BOOL IsJogRtUp();
	void OpenShareUp(BOOL bOpen = TRUE);
	void OpenShareDn(BOOL bOpen = TRUE);
	BOOL IsOpenShareUp();
	BOOL IsOpenShareDn();
	void ResetMotion();
	void ResetMotion(int nMsId);
	unsigned long ChkDoor(); // 0: All Closed , Open Door Index : Doesn't all closed. (Bit3: F, Bit2: L, Bit1: R, Bit0; B)
	BOOL ChkSaftySen();
	BOOL ChkYield();
	void SwAoiEmg(BOOL bOn);
	BOOL IsVs();
	BOOL IsVsUp();
	BOOL IsVsDn();
	void SetDummyUp();
	void SetDummyDn();
	BOOL MakeDummyUp(int nErr);
	BOOL MakeDummyDn(int nErr);
	int GetAoiUpSerial();
	int GetAoiDnSerial();
	BOOL GetAoiUpVsStatus();
	BOOL GetAoiDnVsStatus();
	BOOL IsDoneDispMkInfo();
	BOOL IsSetLotEnd();

	afx_msg LRESULT OnBufThreadDone(WPARAM wPara, LPARAM lPara);
	int ChkSerial(); // 0: Continue, -: Previous, +:Discontinue
	BOOL ReloadRst();
	void OpenReelmap();
	void OpenReelmapUp();
	void OpenReelmapDn();
	BOOL IsRunAxisX();
	//BOOL ChkCollision();
	//BOOL ChkCollision(int nAxisId, double dTgtPosX);
	//BOOL ChkCollision(int nAxisId, double dTgtPosX, double dTgtNextPos);
	//void DoAllMk(int nCam);
	void StopAllMk();
	void EStop();
	//void SetPriority();
	//void ResetPriority();
	void SetAlignPos();
	void SetAlignPosUp();
	void SetAlignPosDn();
	void MpeWrite();
	void IoWrite(CString sMReg, long lData);
	BOOL IsRdyTest();


	BOOL LoadPcrUp(int nSerial, BOOL bFromShare = FALSE);
	BOOL LoadPcrDn(int nSerial, BOOL bFromShare = FALSE);
	void MoveAoi(double dOffset);
	void MoveMk(double dOffset);
	void InitIoWrite();

	void SetLastProc();
	BOOL IsLastProc();
	BOOL IsLastJob(int nAoi); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn

	void MonPlcAlm();
	void MonDispMain();
	void PlcAlm(BOOL bMon, BOOL bClr);
	void FindAlarm();
	void ResetMonAlm();
	void ClrAlarm();
	void ResetClear();

	void ChkTempStop(BOOL bChk);
	void ChgLot();

	void SetLotLastShot();

	BOOL IsMkStrip(int nStripIdx);
	void CycleStop();
	BOOL ChkLotCutPos();
	BOOL OpenReelmapFromBuf(int nSerial);
	int GetAoiUpDummyShot();
	int GetAoiDnDummyShot();
	void SetAoiDummyShot(int nAoi, int nDummy);

	void SetPathAtBuf();
	void SetPathAtBufUp();
	void SetPathAtBufDn();
	void LoadPcrFromBuf();

	BOOL SetSerialReelmap(int nSerial, BOOL bDumy = FALSE);
	BOOL SetSerialMkInfo(int nSerial, BOOL bDumy = FALSE);
	BOOL ChkLastProcFromUp();

	void CntMk();
	void ChkMyMsg();
	BOOL ReloadRst(int nSerial);
	BOOL IsSameUpDnLot();
	BOOL ChkStShotNum();
	BOOL ChkContShotNum();
	void SetFixPcs(int nSerial);

	BOOL RemakeReelmap();
	BOOL IsDoneRemakeReelmap();
	void MakeSapp3();
	void MakeResult();
	void MakeResultMDS();

	//BOOL MoveMeasPos(int nId);
	void SetReject();

	BOOL ChkLightErr();
	BOOL IsOnMarking0();
	BOOL IsOnMarking1();

	void SetDualTest(BOOL bOn = TRUE);
	void SetTwoMetal(BOOL bSel, BOOL bOn = TRUE);
	void DispStsBar(CString sMsg, int nIdx = 0);
	void AdjPinPos();

	int GetMkStripIdx0(int nDefPcsId); // 0 : Fail , 1~4 : Strip Idx
	int GetMkStripIdx1(int nDefPcsId); // 0 : Fail , 1~4 : Strip Idx
	int GetMkStripIdx0(int nSerial, int nMkPcs); // 0 : Fail , 1~4 : Strip Idx
	int GetMkStripIdx1(int nSerial, int nMkPcs); // 0 : Fail , 1~4 : Strip Idx

	void MoveMk0InitPos();
	BOOL MoveAlign0(int nPos);
	//BOOL TwoPointAlign0(int nPos);
	//BOOL IsNoMk0();
	BOOL IsInitPos0();
	BOOL StartLive0();
	BOOL StopLive0();
	BOOL IsRdyTest0();
	BOOL IsJogRtDn0();
	BOOL IsJogRtUp0();
	void SetTestSts0(BOOL bOn);	// not used
	//BOOL IsMk0Done();

	void SetTestSts1(BOOL bOn);	// not used
	BOOL IsRdyTest1();

	//BOOL IsNoMk1();
	//BOOL TwoPointAlign1(int nPos);
	//void MoveInitPos1();
	//void MoveMkEdPos1();
	//BOOL IsInitPos1();
	//BOOL IsMkEdPos1();
	//BOOL StartLive1();
	//BOOL StopLive1();
	//BOOL IsJogRtDn1();
	//BOOL IsJogRtUp1();
	//void MoveMk1InitPos();
	//BOOL IsMk1Done();

	//void SetMkFdSts();
	//void SetMkStopSts();
	//BOOL IsMkFdSts();
	//void SetAoiFd();
	//void SetMkFd();
	//BOOL IsMkFd();
	//BOOL IsAoiFd();
	//void SetMkFd(double dDist);

	BOOL IsEngraveFdSts();
	BOOL IsEngraveFd();
	void SetEngraveFdSts();
	void SetEngraveStopSts();
	void SetEngraveSts(int nStep);
	void SetEngraveFd();
	void SetEngraveFd(double dDist);
	void MoveEngrave(double dOffset);

	double GetEngraveFdLen();
	double GetAoiInitDist();
	double GetAoiRemain();
	void SetEngraveFdPitch(double dPitch);
	BOOL IsConnected();
	void DestroyView();

	BOOL IsConnectedMdx();
	BOOL IsConnectedSr();
	BOOL IsConnectedMk();

	BOOL IsPinPos0();

	void InitAutoEng();
	void InitAutoEngSignal();
	void MarkingWith1PointAlign();

	void Eng1PtReady();
	void Eng1PtInit();
	void Eng1PtAlignPt0();
	void Eng1PtDoMarking();

	void Eng2dRead();

	void AdjPinPosEng();
	BOOL OnePointAlign0(int nPos);
	BOOL SetMk(BOOL bRun = TRUE);
	BOOL SetMdxLotAndShotNum(CString sLot, int nSerial);
	BOOL IsMkDone();

	BOOL Set2dRead(BOOL bRun = TRUE);
	BOOL Is2dReadDone();
	BOOL m_bSetSig, m_bSetSigF, m_bSetData, m_bSetDataF;
	BOOL m_bLoadMstInfo, m_bLoadMstInfoF;
	void LoadMstInfo();

	//void CompletedMk(int nCam); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None

	void SetMyMsgYes();
	void SetMyMsgNo();
	void SetMyMsgOk();

	BOOL SetEngOffset(CfPoint &OfSt);
	void EngStop(BOOL bOn);
	BOOL IsEngStop();
	BOOL GetCurrentInfoSignal();
	void SetLastSerialEng(int nSerial);
	CString GetCurrentInfoBufUp();
	CString GetCurrentInfoBufDn();
	void SetCurrentInfoEngShotNum(int nSerial);

	CString m_sGet2dCodeLot;
	int m_nGet2dCodeSerial;
	BOOL Get2dCode(CString &sLot, int &nSerial);

	void SetTotOpRto(CString sVal);		// 전체진행율
	void SetTotVel(CString sVal);		// 전체속도
	void SetPartVel(CString sVal);		// 구간속도
	void SetMkDoneLen(CString sVal);	// 마킹부 : Distance (FdDone) [M]
	void SetAoiDnDoneLen(CString sVal);	// 검사부(하) : Distance (FdDone) [M]
	void SetAoiUpDoneLen(CString sVal);	// 검사부(상) : Distance (FdDone) [M]
	void SetEngDoneLen(CString sVal);	// 각인부 : Distance (FdDone) [M]

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

	afx_msg LRESULT wmClientReceivedMdx(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT wmClientReceivedSr(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT wmServerReceived(WPARAM wParam, LPARAM lParam);

// 구현입니다.
public:
	virtual ~CGvisR2R_LaserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // GvisR2R_LaserView.cpp의 디버그 버전
inline CGvisR2R_LaserDoc* CGvisR2R_LaserView::GetDocument() const
   { return reinterpret_cast<CGvisR2R_LaserDoc*>(m_pDocument); }
#endif

