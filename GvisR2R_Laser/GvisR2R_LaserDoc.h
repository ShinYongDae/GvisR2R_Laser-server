
// GvisR2R_LaserDoc.h : CGvisR2R_LaserDoc Ŭ������ �������̽�
//


#pragma once

#include "Global/GlobalDefine.h"
#include "Global/MySpec.h"
#include "Global/Yield.h"
#include "Process/ReelMap.h"
#include "Process/DataMarking.h"
#include "Process/MyFile.h"
#include "Process/CamMaster.h"


class CGvisR2R_LaserDoc : public CDocument
{
	int GetIdxPcrBuf(int nSerial);
	int GetIdxPcrBufUp(int nSerial);
	int GetIdxPcrBufDn(int nSerial);
	void DeleteFileInFolder(CString sPathDir);
	int CheckPath(CString strPath);
	void InitPcr();

protected: // serialization������ ��������ϴ�.
	CGvisR2R_LaserDoc();
	DECLARE_DYNCREATE(CGvisR2R_LaserDoc)

// Ư���Դϴ�.
public:

	int m_nDelayShow;
	BOOL m_bBufEmpty[2]; // [0]: Up, [1]: Dn
	BOOL m_bBufEmptyF[2]; // [0]: Up, [1]: Dn

	// R2R Y Meander Adjusting....
	BOOL m_bUseRTRYShiftAdjust;
	double m_dRTRShiftVal;
	double m_dShiftAdjustRatio;

	CMyFile *m_pFile;
	CMySpec *m_pSpecLocal;
	stMkIo MkIo;
	stWorkingInfo WorkingInfo;
	stStatus Status;								// Status �Է½�ȣ
	stBtnStatus BtnStatus;
	stMenu01Status Menu01Status;

	// ���� �۾��� �����ͱ��� ===================================================================
	BOOL m_bCamChged;
	CCamMaster m_Master[2];

	CReelMap* m_pReelMap;
	CReelMap *m_pReelMapUp, *m_pReelMapDn, *m_pReelMapAllUp, *m_pReelMapAllDn;
	CDataMarking* m_pPcr[MAX_PCR][MAX_PCR_PNL];//����ȭ��ǥ�ø� ���� ������		// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	CString *pMkInfo;


	// ���� �۾��� �����ͱ���  ====================================================================
	CCamMaster m_MasterInner[2];

	CReelMap* m_pReelMapInner;
	CReelMap *m_pReelMapInnerUp, *m_pReelMapInnerDn, *m_pReelMapInnerAllUp, *m_pReelMapInnerAllDn;
	CDataMarking* m_pPcrInner[MAX_PCR][MAX_PCR_PNL];	//����ȭ��ǥ�ø� ���� ������	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn

	CReelMap* m_pReelMapIts;
	CDataMarking* m_pPcrIts[MAX_PCR_PNL];				//����ȭ��ǥ�ø� ���� ������	// ������ merging

	//=============================================================================================

	stMpeIoWrite m_pIo[TOT_M_IO];

	int m_nPrevSerial;
	int AoiDummyShot[2]; // [Up/Dn]

	CString m_strUserNameList;
	int m_nSliceIo;

	int m_nMpeIo, m_nMpeI;
	unsigned short *m_pMpeIo, *m_pMpeI;
	unsigned short *m_pMpeIoF, *m_pMpeIF;

	int m_nMpeSignal;
	unsigned short *m_pMpeSignal;

	int m_nMpeData;
	long **m_pMpeData;

	CString m_sPassword;
	CYield m_Yield[3]; // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-All

	CString m_sAlmMsg, m_sPrevAlmMsg;
	CString m_sIsAlmMsg;

	CString m_sMsgBox, m_sPrevMsgBox;
	CString m_sIsMsgBox;
	int m_nTypeMsgBox;

	stListBuf m_ListBuf[2]; // [0]:AOI-Up , [1]:AOI-Dn

	BOOL m_bNewLotShare[2]; // [0]:AOI-Up , [1]:AOI-Dn
	BOOL m_bNewLotBuf[2]; // [0]:AOI-Up , [1]:AOI-Dn
	int m_nLotLastShot, m_nBufLastShot;
	BOOL m_bDoneChgLot;

	char m_cBigDefCode[MAX_DEF];
	char m_cSmallDefCode[MAX_DEF];

	// Sapp3 code
	int m_nSapp3Code[10];

	int m_nOrderNum, m_nShotNum, m_nTestOrderNum, m_nTestShotNum;
	CString m_sOrderNum, m_sShotNum;
	CString m_sTestOrderNum, m_sTestShotNum;

	// Communcation
	int m_nBad[3], m_nGood[3];					// [0]: Up, [1]: Dn, [2]: Total
	double m_dBadRatio[3], m_dGoodRatio[3];		// [0]: Up, [1]: Dn, [2]: Total
	int m_nTestNum[3];							// [0]: Up, [1]: Dn, [2]: Total
	CString m_sLotStTime, m_sLotEdTime, m_sLotRunTime;
	double m_dStripRatio[3][5];					// [3]: Up/Dn/ALL , [5]: Strip 1~4 , ALL
	double m_dTotRatio[3][5];					// [3]: Up/Dn/ALL , [5]: Strip 1~4 , ALL
	int m_nDef[MAX_DEF];						// [3]: Up/Dn/ALL
	double m_dMkBuffCurrPos;

	BOOL m_bUploadPinImg;

// �۾��Դϴ�.
public:
	CString GetProcessNum();
	BOOL LoadWorkingInfo();
	void SaveWorkingInfo();

	BOOL InitReelmap();
	BOOL InitReelmapUp();
	BOOL InitReelmapDn();

	void SetReelmap(int nDir = ROT_NONE);
	CString GetCamPxlRes();
	int LoadPCR(int nSerial, BOOL bFromShare = FALSE);		// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPCR0(int nSerial, BOOL bFromShare = FALSE);		// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPCR1(int nSerial, BOOL bFromShare = FALSE);		// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPCRUp(int nSerial, BOOL bFromShare = FALSE);	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPCRDn(int nSerial, BOOL bFromShare = FALSE);	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPCRAllUp(int nSerial, BOOL bFromShare = FALSE);	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPCRAllDn(int nSerial, BOOL bFromShare = FALSE);	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)

	int GetPcrIdx(int nSerial, BOOL bNewLot = FALSE);
	int GetPcrIdx0(int nSerial, BOOL bNewLot = FALSE); // Up - ����ȭ�� ǥ�ø� ���� Display buffer�� Shot �ε���
	int GetPcrIdx1(int nSerial, BOOL bNewLot = FALSE); // Dn - ����ȭ�� ǥ�ø� ���� Display buffer�� Shot �ε���

	BOOL LoadIoInfo();
	BOOL LoadSignalInfo();
	BOOL LoadDataInfo();
	BOOL LoadMySpec();
	void UpdateData();
	int GetLastSerial();
	int GetLotSerial();
	void SetMkPnt(int nCam);	// Pin��ġ�� ���� ����.
	BOOL GetAoiDnInfo(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	BOOL GetAoiUpInfo(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	BOOL GetAoiInfoUp(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	BOOL GetAoiInfoDn(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	void SetOnePnlLen(double dLen);
	double GetOnePnlLen();
	void SetOnePnlVel(double dVel);
	void SetFdJogVel(double dVel);
	void SetFdJogAcc(double dVel);
	double GetOnePnlVel();
	void SetOnePnlAcc(double dAcc);
	double GetOnePnlAcc();
	void SetAoiFdPitch(double dPitch);
	double GetAoiFdPitch();
	void SetMkFdPitch(double dPitch);
	double GetMkFdPitch();
	void SetFdErrLmt(double dLmt);
	double GetFdErrLmt();
	void SetFdErrRng(double dRng);
	double GetFdErrRng();
	void SetFdErrNum(int nNum);
	int GetFdErrNum();
	void SetBufInitPos(double dPos);
	void SetBufInitPos(double dVel, double dAcc);
	double GetBuffInitPos();
	void SetAoiMkDist(double dPos);
	double GetAoiMkDist();
	void SetAoiAoiDist(int nShot);
	int GetAoiAoiDist();
	void SaveLotTime(DWORD dwStTick);

	void SetWaitPos1(double dPos);
	double GetWaitPos1();
	void SetWaitVel1(double dVel);
	double GetWaitVel1();
	void SetWaitAcc1(double dAcc);
	double GetWaitAcc1();

	void SetMarkingPos1(double dPos);
	double GetMarkingPos1();
	void SetMarkingVel1(double dVel);
	double GetMarkingVel1();
	void SetMarkingAcc1(double dAcc);
	double GetMarkingAcc1();
	void SetMarkingToq1(double dToq);
	void SetMarkingToq1Offset(double dToqOffset);
	double GetMarkingToq1();

	void SetPosX1_1(double dPosX);
	double GetPosX1_1();
	void SetPosY1_1(double dPosY);
	double GetPosY1_1();

	void SetPosX1_2(double dPosX);
	double GetPosX1_2();
	void SetPosY1_2(double dPosY);
	double GetPosY1_2();

	void SetPosX1_3(double dPosX);
	double GetPosY1_3();
	void SetPosY1_3(double dPosY);
	double GetPosX1_3();

	void SetPosX1_4(double dPosX);
	double GetPosX1_4();
	void SetPosY1_4(double dPosY);
	double GetPosY1_4();

	void SetAverDist1(double dDist);
	double GetAverDist1();

	void SetWaitPos2(double dPos);
	double GetWaitPos2();
	void SetWaitVel2(double dVel);
	double GetWaitVel2();
	void SetWaitAcc2(double dAcc);
	double GetWaitAcc2();

	void SetMarkingPos2(double dPos);
	double GetMarkingPos2();
	void SetMarkingVel2(double dVel);
	double GetMarkingVel2();
	void SetMarkingAcc2(double dAcc);
	double GetMarkingAcc2();
	void SetMarkingToq2(double dToq);
	void SetMarkingToq2Offset(double dToqOffset);
	double GetMarkingToq2();

	void SetPosX2_1(double dPosX);
	double GetPosX2_1();
	void SetPosY2_1(double dPosY);
	double GetPosY2_1();

	void SetPosX2_2(double dPosX);
	double GetPosX2_2();
	void SetPosY2_2(double dPosY);
	double GetPosY2_2();

	void SetPosX2_3(double dPosX);
	double GetPosY2_3();
	void SetPosY2_3(double dPosY);
	double GetPosX2_3();

	void SetPosX2_4(double dPosX);
	double GetPosX2_4();
	void SetPosY2_4(double dPosY);
	double GetPosY2_4();

	void SetAverDist2(double dDist);
	double GetAverDist2();

	void SetTotalReelDist(double dDist);
	double GetTotalReelDist();
	void SetSeparateDist(double dDist);
	double GetSeparateDist();
	void SetCuttingDist(double dDist);
	double GetCuttingDist();
	void SetStopDist(double dDist);
	double GetStopDist();

	void SetAOIToq(double dToq);
	double GetAOIToq();
	void SetMarkingToq(double dToq);
	double GetMarkingToq();
	void SetEngraveToq(double dToq);
	double GetEngraveToq();

	int CopyPcrAll();
	int CopyPcrUp();
	int CopyPcrDn();
	void DelPcrAll();
	void DelPcrUp();
	void DelPcrDn();
	void DelSharePcr();
	void DelSharePcrUp();
	void DelSharePcrDn();
	BOOL MakeMkDir(stModelInfo stInfo);
	BOOL MakeMkDir(CString sModel, CString sLot, CString sLayer);
	BOOL MakeMkDir();
	BOOL MakeMkDirUp();
	BOOL MakeMkDirDn();
	BOOL Shift2Mk(int nSerial);
	void SetLastSerial(int nSerial);								// ���� �ؽ�Ʈ ������ ���������� ������Ʈ��.
	void UpdateYield(int nSerial);
	void SetCompletedSerial(int nSerial);
	BOOL ChkLotEnd(CString sPath);
	BOOL IsPinMkData();
	BOOL IsPinData();

	BOOL CopyDefImg(int nSerial);
	BOOL CopyDefImg(int nSerial, CString sNewLot);
	BOOL CopyDefImgUp(int nSerial, CString sNewLot = _T(""));
	BOOL CopyDefImgDn(int nSerial, CString sNewLot = _T(""));
	double GetFdJogVel();
	double GetFdJogAcc();
	void SetModelInfoUp();
	void SetModelInfoDn();
	void SetModelInfoProcessNum();
	BOOL GetAoiUpOffset(CfPoint &OfSt);
	BOOL GetAoiDnOffset(CfPoint &OfSt);
	void ClrPcr();

	CString GetMin(int nDlgId, int nCtrlId);
	CString GetMax(int nDlgId, int nCtrlId);

	int GetLastShotMk();	// m_pDlgFrameHigh���� ��ų� ������, sPathOldFile������ ReelMapDataDn.txt���� _T("Info"), _T("Marked Shot") ã��.
	int GetLastShotUp();	// pView->m_pDlgFrameHigh->m_nAoiLastShot[0]
	int GetLastShotDn();	// pView->m_pDlgFrameHigh->m_nAoiLastShot[1]

	int Mirroring(int nPcsId);

	BOOL GetPcrInfo(CString sPath, stModelInfo &stInfo);

	int GetMkCntL();
	void AddMkCntL();
	void SaveMkCntL();
	void SetMkCntL(int nNum);
	int GetMkLimitL();
	void SetMkLimitL(int nNum = 31536000); // 1[year] = 31536000[sec]
	int GetMkCntR();
	void AddMkCntR();
	void SaveMkCntR();
	void SetMkCntR(int nNum);
	int GetMkLimitR();
	void SetMkLimitR(int nNum = 31536000); // 1[year] = 31536000[sec]

	void UpdateProcessNum(CString sProcessNum);


	void SetProbWaitPos(int nProb, double dPos);
	double GetProbWaitPos(int nProb);
	void SetProbWaitVel(int nProb, double dVel);
	double GetProbWaitVel(int nProb);
	void SetProbWaitAcc(int nProb, double dAcc);
	double GetProbWaitAcc(int nProb);
	void SetProbingPos(int nProb, double dPos);
	double GetProbingPos(int nProb);
	void SetProbingVel(int nProb, double dVel);
	double GetProbingVel(int nProb);
	void SetProbingAcc(int nProb, double dAcc);
	double GetProbingAcc(int nProb);
	void SetProbingToq(int nProb, double dToq);
	double GetProbingToq(int nProb);
	void SetProbPosX(int nProb, double dPosX);
	void SetProbPosY(int nProb, double dPosY);
	double GetProbPosX(int nProb);
	double GetProbPosY(int nProb);

	void SetProbingThreshold(double dThreshold);
	double GetProbingThreshold();

	void SetStripRejectMkNum(int nNum);
	int GetStripRejectMkNum();

	int GetLastShotEngrave();
	void SetEngraveLastShot(int nSerial);
	void SetEngraveReaderDist(double dLen);
	double GetEngraveReaderDist();
	void SetEngraveAoiDist(double dLen);
	double GetEngraveAoiDist();
	void SetAoiReaderDist(double dLen);
	double GetAoiReaderDist();
	void SetMkReaderDist(double dLen);
	double GetMkReaderDist();
	void Set2DReaderPosMoveVel(double dVel);
	void Set2DReaderPosMoveAcc(double dAcc);
	double Get2DReaderPosMoveVel();
	double Get2DReaderPosMoveAcc();

	void SetEngraveFdPitch(double dPitch);
	double GetEngraveFdPitch();
	void SetEngraveFdErrLmt(double dLmt);
	double GetEngraveFdErrLmt();
	void SetEngraveFdErrRng(double dRng);
	double GetEngraveFdErrRng();
	void SetEngraveFdErrNum(int nNum);
	int GetEngraveFdErrNum();
	void SetEngraveBufInitPos(double dPos);
	double GetEngraveBuffInitPos();

	BOOL DirectoryExists(LPCTSTR szPath);
	void UpdateRstOnRmap();
	void SetTestMode(int nMode);
	BOOL GetEngOffset(CfPoint &OfSt);

	CString m_strSharedDir;
	void CheckCurrentInfo();
	void WriteFdOffset(double dOffsetX, double dOffsetY);
	void SetEngItsCode(CString sItsCode);
	BOOL SetEngOffset(CfPoint &OfSt);

	CString GetCurrentInfoBufUp();
	CString GetCurrentInfoBufDn();

	void GetCurrentInfo();
	void SetCurrentInfo();
	int GetCurrentInfoTestMode();
	void SetCurrentInfoTestMode(int nMode);
	int GetCurrentInfoEngShotNum();
	void SetCurrentInfoEngShotNum(int nSerial);
	BOOL GetCurrentInfoSignal(int nIdxSig);
	void SetCurrentInfoSignal(int nIdxSig, BOOL bOn);

	CString GetMonDispMain();
	void SetMonDispMain(CString sDisp);
	void GetMkMenu01();
	void SetMkMenu01(CString sMenu, CString sItem, CString sData);
	void GetMkMenu03();
	void GetMkMenu03Main();
	void SetMkMenu03(CString sMenu, CString sItem, BOOL bOn);
	void GetMkInfo();
	void SetMkInfo(CString sMenu, CString sItem, BOOL bOn);
	void SetMkInfo(CString sMenu, CString sItem, CString sData);

	int GetLastSerialEng();
	int GetTestMode();


	// PCS �ε����� ��������� �ε����� ��ȯ��.
	int MirrorLR(int nPcsId); // �¿� �̷���
	int Rotate180(int nPcsId);// 180�� ȸ�� = �¿� �̷��� & ���� �̷���
	int MirrorUD(int nPcsId); // ���� �̷���

	// For ITS
	BOOL m_bEngDualTest;
	CString m_sItsCode;
	CString m_sLotNum, m_sProcessNum;
	CString m_sModelUp, m_sLayerUp;
	CString m_sLayerDn; //m_sModelDn, 
	int m_nWritedItsSerial;

	BOOL MakeLayerMappingHeader();
	BOOL MakeLayerMappingSerial(int nIdx, int nItsSerial);

	BOOL GetItsSerialInfo(int nItsSerial, BOOL &bDualTest, CString &sLot, CString &sLayerUp, CString &sLayerDn, int nOption = 0);		// ���������� ITS �ø����� ����
	BOOL SetItsSerialInfo(int nItsSerial);																							// ���������� ITS �ø����� ����
																																	//BOOL WriteReelmapIts(int nItsSerial);																							// ������ ��¡�� ���� ����Ÿ
	int GetLastItsSerial();																											// ������ ��¡�� ���� ����Ÿ�� Last �ø���
	CString GetItsFolderPath();
	CString GetItsReelmapPath();
	BOOL GetInnerFolderPath(int nItsSerial, CString  &sUp, CString &sDn);

	char* StrToChar(CString str);
	void StrToChar(CString str, char* pCh);


	// For MODE_OUTER ============================================
	int LoadPCRAllUpInner(int nSerial, BOOL bFromShare = FALSE);	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPCRAllDnInner(int nSerial, BOOL bFromShare = FALSE);	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPCRUpInner(int nSerial, BOOL bFromShare = FALSE);	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPCRDnInner(int nSerial, BOOL bFromShare = FALSE);	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)

																// ITS
	int LoadPCRIts(int nSerial, BOOL bFromShare = FALSE);	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	void LoadPCRIts11(int nSerial); // 11 -> ���� : ���, ���� : ���
	void LoadPCRIts10(int nSerial); // 10 -> ���� : ���, ���� : �ܸ�
	void LoadPCRIts01(int nSerial); // 11 -> ���� : �ܸ�, ���� : ���
	void LoadPCRIts00(int nSerial); // 10 -> ���� : �ܸ�, ���� : �ܸ�

	BOOL InitReelmapInner();
	BOOL InitReelmapInnerUp();
	BOOL InitReelmapInnerDn();
	void SetReelmapInner(int nDir = ROT_NONE);
	CString GetItsPath(int nSerial, int nLayer);	// RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
	int GetItsDefCode(int nDefCode);				// return to [Sapp3Code]


// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CGvisR2R_LaserDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
