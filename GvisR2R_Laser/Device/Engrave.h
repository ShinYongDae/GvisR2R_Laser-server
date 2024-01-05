#pragma once

#include "afxwin.h"
#include "TcpIpServer.h"
#include "EngraveDef.h"

#define TIM_CONNECT	1

class CEngrave : public CWnd
{
	HWND m_hParentWnd;
	CTcpIpServer* m_pServer;

	CString	m_strPortSvr, m_strAddrSvr;
	CString	m_strAddrCli;
	int m_nServerID;

	BOOL m_bWaitForResponse;
	CString m_strResponse;
	int m_nCmd;
	BOOL m_bCmdF;

	SOCKET_DATA m_SocketData;
	BOOL m_bAutoConnect;
	int m_nConnectedId;

	void StartServer(CString sAddr, CString sPort);
	void StopServer();
	BOOL CheckResponse(int nCmd, CString sResponse);
	BOOL WaitResponse();

public:
	CEngrave(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent = NULL);
	~CEngrave();

	BOOL m_bGetOpInfo, m_bGetInfo, m_bGetEngInfo;
	BOOL m_bGetSignalMain, m_bGetSignalTorqueMotor, m_bGetSignalInductionMotor, m_bGetSignalCore150mm, m_bGetSignalEtc;
	BOOL m_bGetSignalRecoiler, m_bGetSignalPunch, m_bGetSignalAOIDn, m_bGetSignalAOIUp, m_bGetSignalEngrave, m_bGetSignalUncoiler;
	BOOL m_bGetSignalEngraveAutoSequence;
	BOOL m_bGetTotRatio, m_bGetStTime, m_bGetRunTime, m_bGetEdTime, m_bGetStripRatio, m_bGetDef;
	BOOL m_bGet2DReader, m_bGetFdInfo, m_bGetAoiInfo, m_bGetMkInfo, m_bGetMkInfoLf, m_bGetMkInfoRt;

	void SetHwnd(HWND hParentWnd);
	BOOL IsRunning();
	int WriteComm(CString sMsg, DWORD dwTimeout = INFINITE);
	BOOL ReadComm(CString &sData);
	SOCKET_DATA GetSocketData();
	BOOL IsConnected();
	BOOL IsDispContRun();
	BOOL IsDispLotEnd();

	// Thread
	CEvent      m_evtThread;
	CWinThread *m_pThread;
	HANDLE		m_hThread;
	BOOL		m_bModify;
	BOOL		m_bAlive;

	void StartThread();
	void StopThread();
	void WaitUntilThreadEnd(HANDLE hThread);
	static UINT RunThread(LPVOID pParam);

	int OnServerReceived(WPARAM wParam, LPARAM lParam);
	void Close();

	// General Function
	//BOOL SendCommand(int nAcceptId, SOCKET_DATA SocketData);
	BOOL SendCommand(SOCKET_DATA SocketData, BOOL bWait = FALSE);


	// Communcation

	// GetSysSignal
	void GetSysSignal(SOCKET_DATA SockData);
	void GetSignalDisp(SOCKET_DATA SockData);
	void GetSignalMain(SOCKET_DATA SockData);
	void GetSignalTorqueMotor(SOCKET_DATA SockData);
	void GetSignalInductionMotor(SOCKET_DATA SockData);
	void GetSignalCore150mm(SOCKET_DATA SockData);
	void GetSignalEtc(SOCKET_DATA SockData);
	void GetSignalRecoiler(SOCKET_DATA SockData);
	void GetSignalPunch(SOCKET_DATA SockData);
	void GetSignalAOIDn(SOCKET_DATA SockData);
	void GetSignalAOIUp(SOCKET_DATA SockData);
	void GetSignalEngrave(SOCKET_DATA SockData);
	void GetSignalUncoiler(SOCKET_DATA SockData);
	void GetSignalEngraveAutoSequence(SOCKET_DATA SockData);
	void GetSignalMyMsg(SOCKET_DATA SockData);
	void GetSignal2dEng(SOCKET_DATA SockData);

	// GetSysData
	void GetSysData(SOCKET_DATA SockData);
	void GetOpInfo(SOCKET_DATA SockData);
	void GetInfo(SOCKET_DATA SockData);
	void GetTotRatio(SOCKET_DATA SockData);
	void GetStTime(SOCKET_DATA SockData);
	void GetRunTime(SOCKET_DATA SockData);
	void GetEdTime(SOCKET_DATA SockData);
	void GetStripRatio(SOCKET_DATA SockData);
	void GetDef(SOCKET_DATA SockData);
	void Get2DReader(SOCKET_DATA SockData);
	void GetEngInfo(SOCKET_DATA SockData);
	void GetFdInfo(SOCKET_DATA SockData);
	void GetAoiInfo(SOCKET_DATA SockData);
	void GetMkInfo(SOCKET_DATA SockData);
	void GetMkInfoLf(SOCKET_DATA SockData);
	void GetMkInfoRt(SOCKET_DATA SockData);
	void GetAlarmMsg(SOCKET_DATA SockData);
	void GetMsgBox(SOCKET_DATA SockData);

	// SetSysSignal
	BOOL SetSysSignal();
	void SetSignalMain();
	void SetSignalTorqueMotor();
	void SetSignalInductionMotor();
	void SetSignalCore150mm();
	void SetSignalEtc();
	void SetSignalRecoiler();
	void SetSignalPunch();
	void SetSignalAOIDn();
	void SetSignalAOIUp();
	void SetSignalEngrave();
	void SetSignalUncoiler();

	// SetSysData
	BOOL SetSysData();
	void SetOpInfo();
	void SetInfo();
	void SetTotRatio();
	void SetStTime();
	void SetRunTime();
	void SetEdTime();
	void SetStripRatio();
	void SetDef();
	void Set2DReader();
	void SetEngInfo();
	void SetFdInfo();
	void SetAoiInfo();
	void SetMkInfo();
	void SetMkInfoLf();
	void SetMkInfoRt();

	// Communcation - Sub Item

	// SetOpInfo()
	void SetOpName();
	void SetDualTest();
	void SetSampleTest();
	void SetSampleShotNum();
	void SetTestMode();
	void SetRecoilerCcw();
	void SetUncoilerCcw();
	void SetAlignMethode();
	void SetDoorRecoiler();
	void SetDoorAoiUp();
	void SetDoorAoiDn();
	void SetDoorMk();
	void SetDoorEngrave();
	void SetDoorUncoiler();
	void SetSaftyMk();
	void SetCleannerAoiUp();
	void SetCleannerAoiDn();
	void SetUltraSonicAoiDn();
	void SetUltraSonicEngrave();
	void SetUse380mm();
	void SetTotReelLen();
	void SetOnePnlLen();
	void SetTempPause();
	void SetTempStopLen();
	void SetLotCut();
	void SetLotCutLen();
	void SetLotCutPosLen();
	void SetLmtTotYld();
	void SetLmtPatlYld();
	void SetStripOutRatio();
	void SetCustomNeedRatio();
	void SetNumRangeFixDef();
	void SetFixDef();
	void SetNumContFixDef();
	void SetUltraSonicStTim();
	void SetEngItsCode();

	// SetInfo()
	void SetModelUpName();
	//void SetModelDnName();
	void SetLotUpName();
	void SetLotDnName();
	void SetLayerUpName();
	void SetLayerDnName();
	void SetOrderNum();
	void SetShotNum();
	void SetTotOpRto();
	void SetTotVel();
	void SetPartVel();
	void SetMkDoneLen();
	void SetAoiDnDoneLen();
	void SetAoiUpDoneLen();
	void SetLotSerial();
	void SetMkVerfyLen();

	// SetTotRatio()
	void SetDefNumUp();
	void SetDefRtoUp();
	void SetGoodNumUp();
	void SetGoodRtoUp();
	void SetTestNumUp();
	void SetDefNumDn();
	void SetDefRtoDn();
	void SetGoodNumDn();
	void SetGoodRtoDn();
	void SetTestNumDn();
	void SetDefNumTot();
	void SetDefRtoTot();
	void SetGoodNumTot();
	void SetGoodRtoTot();
	void SetTestNumTot();

	// Set2DReader()
	void Set2DEngLen();
	void Set2DAoiLen();
	void Set2DMkLen();
	void Set2DMoveVel();
	void Set2DMoveAcc();
	void Set2DOneShotLen();

	// SetEngInfo()
	void SetEngLeadPitch();
	void SetEngPushOffLen();
	void SetEngTqVal();
	void SetEngAoiLen();
	void SetEngFdDiffMax();
	void SetEngFdDiffRng();
	void SetEngFdDiffNum();
	void SetEngBuffInitPos();
	void SetEngBuffCurrPos();
	void SetEngBuffUp();
	void SetEngBuffDn();
	void SetEngBuffHome();
	void SetEngBuffHomeDone();
	void SetEngBuffInitMove();
	void SetEngBuffInitMoveDone();
	void SetEngBuffInitPosSave();

	// SetFdInfo()
	void SetFdVel();
	void SetFdAcc();
	void SetOnePnlVel();
	void SetOnePnlAcc();
	void SetFdDiffMax();
	void SetFdDiffRng();
	void SetFdDiffNum();

	// SetAoiInfo()
	void SetAoiLeadPitch();
	void SetAoiPushOffLen();
	void SetAoiTqVal();
	void SetAoiBuffShotNum();
	void SetAoiMkLen();

	// SetMkInfo()
	void SetMkLeadPitch();
	void SetMkPushOffLen();
	void SetMkTqVal();
	void SetMkBuffInitPos();
	void SetMkBuffCurrPos();
	void SetMkNumLf();
	void SetMkNumRt();
	void SetMkMaxNumLf();
	void SetMkMaxNumRt();

	// SetMkInfoLf()
	void SetMkInitPosLf();
	void SetMkInitVelLf();
	void SetMkInitAccLf();
	void SetMkFnlPosLf();
	void SetMkFnlVelLf();
	void SetMkFnlAccLf();
	void SetMkFnlTqLf();
	void SetMkHgtPosX1Lf();
	void SetMkHgtPosY1Lf();
	void SetMkHgtPosX2Lf();
	void SetMkHgtPosY2Lf();
	void SetMkHgtPosX3Lf();
	void SetMkHgtPosY3Lf();
	void SetMkHgtPosX4Lf();
	void SetMkHgtPosY4Lf();
	void SetMkHgtAvgPosLf();

	// SetMkInfoRt()
	void SetMkInitPosRt();
	void SetMkInitVelRt();
	void SetMkInitAccRt();
	void SetMkFnlPosRt();
	void SetMkFnlVelRt();
	void SetMkFnlAccRt();
	void SetMkFnlTqRt();
	void SetMkHgtPosX1Rt();
	void SetMkHgtPosY1Rt();
	void SetMkHgtPosX2Rt();
	void SetMkHgtPosY2Rt();
	void SetMkHgtPosX3Rt();
	void SetMkHgtPosY3Rt();
	void SetMkHgtPosX4Rt();
	void SetMkHgtPosY4Rt();
	void SetMkHgtAvgPosRt();

	// Set Engrave Data
	void SetEngraveAoiDist();
	void SetEngraveFdPitch();
	//void IsSetEngraveAoiDist();
	//void IsSetEngraveFdPitch();

	// End for SetSysData()

	// Start Switch

	// Disp
	void SetDispReady(BOOL bOn = TRUE);
	void SetDispRun(BOOL bOn = TRUE);
	void SetDispStop(BOOL bOn = TRUE);
	void SetDispDualSample(BOOL bOn = TRUE);
	void SetDispSingleSample(BOOL bOn = TRUE);
	void SetDispDualTest(BOOL bOn = TRUE);
	void SetDispSingleTest(BOOL bOn = TRUE);
	void IsSetDispReady(BOOL bOn = TRUE);
	void IsSetDispRun(BOOL bOn = TRUE);
	void IsSetDispStop(BOOL bOn = TRUE);
	void IsSetDispDualSample(BOOL bOn = TRUE);
	void IsSetDispSingleSample(BOOL bOn = TRUE);
	void IsSetDispDualTest(BOOL bOn = TRUE);
	void IsSetDispSingleTest(BOOL bOn = TRUE);

	// Main
	void SwReady(BOOL bOn);
	void SwRun(BOOL bOn);
	void SwReset(BOOL bOn);
	void SwStop(BOOL bOn);
	void SwAuto(BOOL bOn);
	void SwManual(BOOL bOn);
	void IsSwReady(BOOL bOn);
	void IsSwRun(BOOL bOn);
	void IsSwReset(BOOL bOn);
	void IsSwStop(BOOL bOn);
	void IsSwAuto(BOOL bOn);
	void IsSwManual(BOOL bOn);

	// Torque Motor
	void SwMkTq(BOOL bOn);
	void SwAoiTq(BOOL bOn);
	void SwEngTq(BOOL bOn);
	void IsSwMkTq(BOOL bOn);
	void IsSwAoiTq(BOOL bOn);
	void IsSwEngTq(BOOL bOn);

	// Induction Motor
	void SwRcInductionCcw(BOOL bOn);	// SetOneMetal
	void SwUcInductionCcw(BOOL bOn);	// SetTwoMetal
	void IsSwRcInductionCcw(BOOL bOn);	// SetOneMetal
	void IsSwUcInductionCcw(BOOL bOn);	// SetTwoMetal

	// Core 150mm
	void SwRcCore150mm(BOOL bOn);
	void SwUcCore150mm(BOOL bOn);
	void IsSwRcCore150mm(BOOL bOn);
	void IsSwUcCore150mm(BOOL bOn);

	// Etc
	void SwAoiEmg(BOOL bOn);
	void IsSwAoiEmg(BOOL bOn);
	//void SwBufRolSol(BOOL bOn);
	//void SwMkDnSol(BOOL bOn);

	// Recoiler
	void SwRcRelation(BOOL bOn);
	void SwRcFdCw(BOOL bOn);
	void SwRcFdCcw(BOOL bOn);
	void SwRcReelChuck(BOOL bOn);
	void SwRcDcRlUpDn(BOOL bOn);
	void SwRcReelJoinL(BOOL bOn);
	void SwRcReelJoinR(BOOL bOn);
	void SwRcReelJoinVac(BOOL bOn);
	void SwRcPaperChuck(BOOL bOn);
	void SwRcPaperCw(BOOL bOn);
	void SwRcPaperCcw(BOOL bOn);
	void SwRcRewinder(BOOL bOn);
	void SwRcRewinderReelPaper(BOOL bOn);
	void IsSwRcRelation(BOOL bOn);
	void IsSwRcFdCw(BOOL bOn);
	void IsSwRcFdCcw(BOOL bOn);
	void IsSwRcReelChuck(BOOL bOn);
	void IsSwRcDcRlUpDn(BOOL bOn);
	void IsSwRcReelJoinL(BOOL bOn);
	void IsSwRcReelJoinR(BOOL bOn);
	void IsSwRcReelJoinVac(BOOL bOn);
	void IsSwRcPaperChuck(BOOL bOn);
	void IsSwRcPaperCw(BOOL bOn);
	void IsSwRcPaperCcw(BOOL bOn);
	void IsSwRcRewinder(BOOL bOn);
	void IsSwRcRewinderReelPaper(BOOL bOn);

	// Punching
	void SwMkRelation(BOOL bOn);
	void SwMkFdCw(BOOL bOn);
	void SwMkFdCcw(BOOL bOn);
	void SwMkFdVac(BOOL bOn);	
	void SwMkPushUp(BOOL bOn);
	void SwMkTblBlw(BOOL bOn);
	void SwMkTblVac(BOOL bOn);	
	void SwMkFdClp(BOOL bOn);	
	void SwMkTqClp(BOOL bOn);
	void SwMkMvOne(BOOL bOn);
	void SwMkLsrPt(BOOL bOn);
	void SwMkDcRSol(BOOL bOn);
	void IsSwMkRelation(BOOL bOn);
	void IsSwMkFdCw(BOOL bOn);
	void IsSwMkFdCcw(BOOL bOn);
	void IsSwMkFdVac(BOOL bOn);
	void IsSwMkPushUp(BOOL bOn);
	void IsSwMkTblBlw(BOOL bOn);
	void IsSwMkTblVac(BOOL bOn);
	void IsSwMkFdClp(BOOL bOn);
	void IsSwMkTqClp(BOOL bOn);
	void IsSwMkMvOne(BOOL bOn);
	void IsSwMkLsrPt(BOOL bOn);
	void IsSwMkDcRSol(BOOL bOn);

	// AOIDn
	void SwAoiDnRelation(BOOL bOn);
	void SwAoiDnFdCw(BOOL bOn);
	void SwAoiDnFdCcw(BOOL bOn);
	void SwAoiDnFdVac(BOOL bOn);
	void SwAoiDnPushUp(BOOL bOn);
	void SwAoiDnTblBlw(BOOL bOn);
	void SwAoiDnTblVac(BOOL bOn);
	void SwAoiDnFdClp(BOOL bOn);
	void SwAoiDnTqClp(BOOL bOn);
	void SwAoiDnMvOne(BOOL bOn);
	void SwAoiDnLsrPt(BOOL bOn);
	void SwAoiDnClrRoll(BOOL bOn);
	void SwAoiDnVelSonicBlw(BOOL bOn);
	void SwAoiDnTest(BOOL bOn);
	void SwAoiDnReset(BOOL bOn);
	void SwAoiDnLotEnd(BOOL bOn);
	void IsSwAoiDnRelation(BOOL bOn);
	void IsSwAoiDnFdCw(BOOL bOn);
	void IsSwAoiDnFdCcw(BOOL bOn);
	void IsSwAoiDnFdVac(BOOL bOn);
	void IsSwAoiDnPushUp(BOOL bOn);
	void IsSwAoiDnTblBlw(BOOL bOn);
	void IsSwAoiDnTblVac(BOOL bOn);
	void IsSwAoiDnFdClp(BOOL bOn);
	void IsSwAoiDnTqClp(BOOL bOn);
	void IsSwAoiDnMvOne(BOOL bOn);
	void IsSwAoiDnLsrPt(BOOL bOn);
	void IsSwAoiDnClrRoll(BOOL bOn);
	void IsSwAoiDnVelSonicBlw(BOOL bOn);
	void IsSwAoiDnTest(BOOL bOn);
	void IsSwAoiDnReset(BOOL bOn);
	void IsSwAoiDnLotEnd(BOOL bOn);

	// AOIUp
	void SwAoiUpRelation(BOOL bOn);
	void SwAoiUpFdCw(BOOL bOn);
	void SwAoiUpFdCcw(BOOL bOn);
	void SwAoiUpFdVac(BOOL bOn);
	void SwAoiUpPushUp(BOOL bOn);
	void SwAoiUpTblBlw(BOOL bOn);
	void SwAoiUpTblVac(BOOL bOn);
	void SwAoiUpFdClp(BOOL bOn);
	void SwAoiUpTqClp(BOOL bOn);
	void SwAoiUpMvOne(BOOL bOn);
	void SwAoiUpLsrPt(BOOL bOn);
	void SwAoiUpClrRoll(BOOL bOn);
	//void SwAoiUpTqVac(BOOL bOn);
	void SwAoiUpTest(BOOL bOn);
	void SwAoiUpReset(BOOL bOn);
	void SwAoiUpLotEnd(BOOL bOn);
	void IsSwAoiUpRelation(BOOL bOn);
	void IsSwAoiUpFdCw(BOOL bOn);
	void IsSwAoiUpFdCcw(BOOL bOn);
	void IsSwAoiUpFdVac(BOOL bOn);
	void IsSwAoiUpPushUp(BOOL bOn);
	void IsSwAoiUpTblBlw(BOOL bOn);
	void IsSwAoiUpTblVac(BOOL bOn);
	void IsSwAoiUpFdClp(BOOL bOn);
	void IsSwAoiUpTqClp(BOOL bOn);
	void IsSwAoiUpMvOne(BOOL bOn);
	void IsSwAoiUpLsrPt(BOOL bOn);
	void IsSwAoiUpClrRoll(BOOL bOn);
	//void IsSwAoiUpTqVac(BOOL bOn);
	void IsSwAoiUpTest(BOOL bOn);
	void IsSwAoiUpReset(BOOL bOn);
	void IsSwAoiUpLotEnd(BOOL bOn);

	// Engrave
	void SwEngRelation(BOOL bOn);
	void SwEngFdCw(BOOL bOn);
	void SwEngFdCcw(BOOL bOn);
	void SwEngFdVac(BOOL bOn);
	void SwEngPushUp(BOOL bOn);
	void SwEngTblBlw(BOOL bOn);
	void SwEngTblVac(BOOL bOn);
	void SwEngFdClp(BOOL bOn);
	void SwEngTqClp(BOOL bOn);
	void SwEngMvOne(BOOL bOn);
	void SwEngLsrPt(BOOL bOn);
	void SwEngDcRSol(BOOL bOn);
	void SwEngVelSonicBlw(BOOL bOn);
	void IsSwEngRelation(BOOL bOn);
	void IsSwEngFdCw(BOOL bOn);
	void IsSwEngFdCcw(BOOL bOn);
	void IsSwEngFdVac(BOOL bOn);
	void IsSwEngPushUp(BOOL bOn);
	void IsSwEngTblBlw(BOOL bOn);
	void IsSwEngTblVac(BOOL bOn);
	void IsSwEngFdClp(BOOL bOn);
	void IsSwEngTqClp(BOOL bOn);
	void IsSwEngMvOne(BOOL bOn);
	void IsSwEngLsrPt(BOOL bOn);
	void IsSwEngDcRSol(BOOL bOn);
	void IsSwEngVelSonicBlw(BOOL bOn);

	// Uncoiler
	void SwUcRelation(BOOL bOn);
	void SwUcFdCw(BOOL bOn);
	void SwUcFdCcw(BOOL bOn);
	void SwUcReelChuck(BOOL bOn);
	void SwUcDcRlUpDn(BOOL bOn);
	void SwUcReelJoinL(BOOL bOn);
	void SwUcReelJoinR(BOOL bOn);
	void SwUcReelJoinVac(BOOL bOn);
	void SwUcPaperChuck(BOOL bOn);
	void SwUcPaperCw(BOOL bOn);
	void SwUcPaperCcw(BOOL bOn);
	void SwUcClRlUpDn(BOOL bOn);
	void SwUcClRlPshUpDn(BOOL bOn);
	void IsSwUcRelation(BOOL bOn);
	void IsSwUcFdCw(BOOL bOn);
	void IsSwUcFdCcw(BOOL bOn);
	void IsSwUcReelChuck(BOOL bOn);
	void IsSwUcDcRlUpDn(BOOL bOn);
	void IsSwUcReelJoinL(BOOL bOn);
	void IsSwUcReelJoinR(BOOL bOn);
	void IsSwUcReelJoinVac(BOOL bOn);
	void IsSwUcPaperChuck(BOOL bOn);
	void IsSwUcPaperCw(BOOL bOn);
	void IsSwUcPaperCcw(BOOL bOn);
	void IsSwUcClRlUpDn(BOOL bOn);
	void IsSwUcClRlPshUpDn(BOOL bOn);

	// DlgMyMsg
	void SetMyMsgYes();
	void SetMyMsgNo();
	void SetMyMsgOk();
	void IsSetMyMsgYes();
	void IsSetMyMsgNo();
	void IsSetMyMsgOk();

	// Engrave Auto Sequence
	void SwEngAutoInit(BOOL bOn); // 각인부 초기화(Reset)
	void SwEngAutoMkSt(BOOL bOn);
	void SwEngAutoOnMking(BOOL bOn);
	void SwEngAutoMkDone(BOOL bOn);
	void SwEngAuto2dReadSt(BOOL bOn);
	void SwEngAutoOnReading2d(BOOL bOn);
	void SwEngAuto2dReadDone(BOOL bOn);
	void IsSwEngAutoInit(BOOL bOn); // 각인부 초기화(Reset)
	void IsSwEngAutoMkSt(BOOL bOn);
	void IsSwEngAutoOnMking(BOOL bOn);
	void IsSwEngAutoMkDone(BOOL bOn);
	void IsSwEngAuto2dReadSt(BOOL bOn);
	void IsSwEngAutoOnReading2d(BOOL bOn);
	void IsSwEngAuto2dReadDone(BOOL bOn);

	// End Switch

	// Alarm
	void SetAlarm(CString sMsg);
	void IsSetAlarm(CString sMsg);
	void SetMsgBox(CString sMsg);
	void IsSetMsgBox(CString sMsg);

	// On Running Auto
	BOOL UpdateWorking();
	BOOL UpdateRst();
	BOOL UpdateTotRatio();
	BOOL UpdateStripRatio();
	BOOL UpdateDef();
	void GetUpdateWorking(SOCKET_DATA SockData);

	void SwMenu01UpdateWorking(BOOL bOn);
	void IsSwMenu01UpdateWorking(BOOL bOn);

	// SetteingEng
	void Set2DOffsetInitPos();
	void Set2DOffsetInitPosMove(BOOL bOn);

	void SetBuzzer(BOOL bOn, int nCh = 0);
	void IsSetBuzzer();
	void SetTowerLamp(COLORREF color, BOOL bOn, BOOL bWink = FALSE);
	void IsSetTowerLamp();
	void SetErrorRead2dCode(int nParam);
	void IsSetErrorRead2dCode(int nParam);
	void SetDispContRun(BOOL bOn);
	void IsSetDispContRun();

protected:
	afx_msg LRESULT wmAcceptReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT wmServerClosed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateConnection(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

