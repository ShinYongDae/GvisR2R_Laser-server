#if !defined(AFX_DLGMENU02_H__20579F9A_60DF_4376_99B4_C63C1CACF17E__INCLUDED_)
#define AFX_DLGMENU02_H__20579F9A_60DF_4376_99B4_C63C1CACF17E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMenu02.h : header file
//

#include "MyBtn.h"
#include "MyLabel.h"
#include "MyStatic.h"
#include "MyGL.h"
#include "../Global/MyData.h"

#define MAX_MENU02_LABEL			7

#define MAX_MENU02_LBL				(14)
#define MAX_MENU02_STC				(23+14+2+7+7)
#define MAX_MENU02_STC_DATA			(10+10+5+1+2+3)
#define MAX_MENU02_BTN				(22+6+5+6+1+4+1+1)

#define MAX_MENU02_STC_DUO			15
#define MAX_MENU02_STC_DATA_DUO		17
#define MAX_MENU02_BTN_DUO			29

#define TIM_LIGHT_UP				200
#define TIM_LIGHT_DN				201
#define TIM_BUF_ENC					202
#define TIM_MARKING_OFF				203
#define TIM_SHOW_MK_TEST			206

#define TIM_2D_READING				207
#define TIM_TEST_2D_READING			208
#define TIM_MDX_RESPONSE			209
#define TIM_MDX_READY				210
#define TIM_MDX_READY_CHECK			211

#define TIM_MPE_OFFSET_INITPOS_MOVE			220

#define WAIT_MARKING				50		// [mSec]

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu02 dialog

class CDlgMenu02 : public CDialog
{
	CMyStatic myStcPcsImg;//, myStcPinImg;

	CMyBtn myBtn[MAX_MENU02_BTN];
	CMyLabel myLblTitle[MAX_MENU02_LBL];
	CMyStatic myStcTitle[MAX_MENU02_STC];
	CMyStatic myStcData[MAX_MENU02_STC_DATA];

	CMyStatic myStcData2[MAX_MENU02_STC_DATA_DUO];
	CMyLabel myLabel[MAX_MENU02_LABEL];

	CMyGL *m_pPcsGL;//, *m_pPinGL;
	BOOL m_bLoadImg, m_bLockTimer;

	BOOL m_bTIM_LIGHT_UP, m_bTIM_LIGHT_DN, m_bTIM_BUF_ENC;
	BOOL m_bTIM_2D_READING, m_bTIM_TEST_2D_READING;
	BOOL m_bTIM_MDX_READY, m_bTIM_MDX_READY_CHECK, m_bTIM_MDX_RESPONSE;
	BOOL m_bTIM_MPE_OFFSET_INITPOS_MOVE;
	double m_dStOffsetX, m_dStOffsetY;
	double m_dCurPosX[2], m_dCurPosY[2];
	int m_nSelectCam0Pos, m_nSelectCam1Pos;
	int m_nSpd;
	unsigned long m_lChk;
	int m_nMoveAlign[2];

	ULONGLONG m_stTime;

	void LoadImg();
	void DelImg();

	void InitStatic();
	void InitStcTitle();
	void InitStcData();
	void InitBtn();
	void InitLabel();
//	void InitGL();
	void InitSlider();
	void DispBufEnc();
	void DispAxisPos();
	void DispCenterMark();
	void MarkingOff();
//	CfPoint GetPcsCenterOffset(CPoint pt);
	void MsClr(int nMsId);
	void ShowDlg(int nID);
	
// 	void InitResMeas();
// 	void SetResMeas(int nStep);

	void Disp();
	void Input_myStcData(int nIdx, int nCtlId); // , CPoint ptSt, int nDir);
	BOOL DispTest2dCode();
	BOOL ChkMdxResponse();
	BOOL IsMdxReady();
	BOOL ChkMdxReady();

// Construction
public:
	CDlgMenu02(CWnd* pParent = NULL);   // standard constructor
	~CDlgMenu02();

	CRect* m_pRect;
	int m_nJogSpd;
	int m_nBtnAlignCam0Pos, m_nBtnAlignCam1Pos;
	double m_dMkFdOffsetX[2][4], m_dMkFdOffsetY[2][4];// 2Cam / 4Point Align
	double m_dAoiUpFdOffsetX, m_dAoiUpFdOffsetY;
	double m_dAoiDnFdOffsetX, m_dAoiDnFdOffsetY;
// 	BOOL m_bMkDnSolOff;

	double m_dOneShotRemainLen;
	void DispOneShotRemainLen();



	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

	afx_msg LRESULT OnDrawPcsImg(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnDrawPinImg(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyStaticRedraw(WPARAM wPara, LPARAM lPara);

	int GetJogSpd();
	void SetJogSpd();
	//void SetJogSpd2();
	void SetJogSpd(int nSpd);
	//void SetJogSpd2(int nSpd);
	void SetLight(int nVal=-1);
	void ResetLight();
	BOOL MovePos(int nPos);
	//BOOL MovePos2(int nPos);
	BOOL MovePinPos();
	//BOOL MoveAlign0(int nPos);
	BOOL Grab(int nPos, BOOL bMove=FALSE);
	BOOL OnePointAlign(CfPoint &ptPnt);
	BOOL GetPmRst0(double &dX, double &dY, double &dAgl, double &dScr);
	BOOL GetPmRst1(double &dX, double &dY, double &dAgl, double &dScr);
	//BOOL TwoPointAlign(int nPos);
	//BOOL TwoPointAlign0(int nPos);

	afx_msg LRESULT OnMyBtnDown(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyBtnUp(WPARAM wPara, LPARAM lPara);

	void SwMyBtnDown(int nCtrlID);
	void SwMyBtnUp(int nCtrlID);
	void SwMarking();
	//void SwMarking2();

//	void SetMkPos(CfPoint ptOfst);
	void SetMkCurPos(int nCam);
	void SetMkDlyOff(int nCam, int mSec=WAIT_MARKING);
	void SetPinPos(int nCam, CfPoint ptPnt);
//	void DispLocalSpec();
// 	void SetPcsOffset();
	void ChgModel();
	void ChgModelUp();
	void ChgModelDn();

// 	void PinSave();
	void ResetMotion(int nMsId);

	BOOL ShowKeypad(int nCtlID, CPoint ptSt=(0, 0), int nDir=TO_NONE);

	BOOL DoAlign0(int nPos, BOOL bDraw=FALSE);
//	BOOL DoAlign1(int nPos, BOOL bDraw=FALSE);

	void ResetMkTestBtn();
	void InitCadImg();

	// Engrave
	BOOL Disp2dCode();
	void WaitResponse();

	void UpdateData();
	void SetLed(int nIdx, BOOL bOn=TRUE);

// Dialog Data
	//{{AFX_DATA(CDlgMenu02)
	enum { IDD = IDD_DLG_MENU_02 };
	CSliderCtrl	m_LightSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMenu02)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMenu02)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnChkLtOn();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	afx_msg void OnChkJogVf();
	afx_msg void OnChkJogF();
	afx_msg void OnChkJogN();
	afx_msg void OnChkJogS();
	afx_msg void OnChkJogVs();
	afx_msg void OnBtnPinMove();
	afx_msg void OnBtnPinSave();
	afx_msg void OnBtnHomeMove();
	afx_msg void OnChkResPosSt();
	afx_msg void OnChkMkOffsetSt();
	afx_msg void OnChkMkOffsetEd();
	afx_msg void OnBtnAlignMove();
	afx_msg void OnBtnAlign();
	afx_msg void OnBtnGrab();
	afx_msg void OnBtnHomeSave();
	afx_msg void OnStcAlignStdScr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnLaserAlign();
	afx_msg void OnStnClickedStc5();
	afx_msg void OnStnClickedStc007();
	afx_msg void OnStnClickedStc010();
	afx_msg void OnStnClickedStc12();
	afx_msg void OnStnClickedStc40();
	afx_msg void OnStnClickedStc17();
	afx_msg void OnStnClickedStc19();
	afx_msg void OnStnClickedStc36();
	afx_msg void OnStnClickedStc45();
	afx_msg void OnStnClickedStc180();
	afx_msg void OnStnClickedStc184();
	afx_msg void OnStnClickedStc218();
	afx_msg void OnStnClickedStc225();
	afx_msg void OnBnClickedBtnBuffInitSave1();
	afx_msg void OnBnClickedBtnBuffInitMove1();
	afx_msg void OnStnClickedStc38();
	afx_msg void OnStnClickedStc191();
	afx_msg void OnBnClickedBtn2dReading();
	afx_msg void OnBnClickedBtnLaserMarking();
	afx_msg void OnBnClickedBtnLaserAdjustZero();
	afx_msg void OnBnClickedBtnLaserAdjustUp();
	afx_msg void OnBnClickedBtnLaserAdjustRt();
	afx_msg void OnBnClickedBtnLaserAdjustDn();
	afx_msg void OnBnClickedBtnLaserAdjustLf();
	afx_msg void OnBnClickedBtnLaserAdjustCw2();
	afx_msg void OnBnClickedBtnLaserAdjustCcw2();
	afx_msg void OnBnClickedBtnShotRemainCw();
	afx_msg void OnBnClickedBtnShotRemainCcw();
	afx_msg void OnBnClickedBtnOffsetEngraveCw();
	afx_msg void OnBnClickedBtnOffsetEngraveCcw();
	afx_msg void OnBnClickedBtnOffsetAoiCw();
	afx_msg void OnBnClickedBtnOffsetAoiCcw();
	afx_msg void OnBnClickedBtnOffsetMkCw();
	afx_msg void OnBnClickedBtnOffsetMkCcw();
	afx_msg void OnStnClickedStc49();
	afx_msg void OnStnClickedStc52();
	afx_msg void OnStnClickedStc55();
	afx_msg void OnBnClickedBtnMoveInitOffset();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMENU02_H__20579F9A_60DF_4376_99B4_C63C1CACF17E__INCLUDED_)
