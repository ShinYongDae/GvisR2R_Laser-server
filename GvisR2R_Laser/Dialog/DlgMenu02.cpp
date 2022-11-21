// DlgMenu02.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Laser.h"
#include "DlgMenu02.h"
#include "DlgKeyNum.h"

#include "../Device/MDX2500DEF.h""

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../MainFrm.h"
#include "../GvisR2R_LaserDoc.h"
#include "../GvisR2R_LaserView.h"

extern CMainFrame* pFrm;
extern CGvisR2R_LaserDoc* pDoc;
extern CGvisR2R_LaserView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu02 dialog


CDlgMenu02::CDlgMenu02(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMenu02::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMenu02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	int nCam, nPoint;

	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_pPcsGL = NULL;
//	m_pPinGL = NULL;

	m_bTIM_LIGHT_UP = FALSE;
	m_bTIM_LIGHT_DN = FALSE;
	m_bTIM_BUF_ENC = FALSE;

	m_bTIM_2D_READING = FALSE;
	m_bTIM_TEST_2D_READING = FALSE;
	m_bTIM_MDX_RESPONSE = FALSE;
	m_bTIM_MDX_READY = FALSE;
	m_bTIM_MDX_READY_CHECK = FALSE;

	m_dStOffsetX = 0.0; 
	m_dStOffsetY = 0.0;

	for (nCam = 0; nCam < 2; nCam++)
	{
		for (nPoint = 0; nPoint < 4; nPoint++)
		{
			m_dMkFdOffsetX[nCam][nPoint] = 0.0;
			m_dMkFdOffsetY[nCam][nPoint] = 0.0;
		}
	}

	m_dAoiUpFdOffsetX = 0.0;
	m_dAoiUpFdOffsetY = 0.0;
	m_dAoiDnFdOffsetX = 0.0;
	m_dAoiDnFdOffsetY = 0.0;

	m_nBtnAlignCam0Pos = 0;
	m_nBtnAlignCam1Pos = 0;
	m_nSpd = 1;
// 	m_bMkDnSolOff = FALSE;

	m_nSelectCam0Pos = 0;
	m_nSelectCam1Pos = 0;

	m_lChk = 0L;

	m_bLockTimer = FALSE;

	m_dOneShotRemainLen = 0.0;
}

CDlgMenu02::~CDlgMenu02()
{
	m_bTIM_LIGHT_UP = FALSE;
	m_bTIM_LIGHT_DN = FALSE;
	m_bTIM_BUF_ENC = FALSE;

	m_bTIM_2D_READING = FALSE;
	m_bTIM_TEST_2D_READING = FALSE;
	m_bTIM_MDX_RESPONSE = FALSE;
	m_bTIM_MDX_READY = FALSE;
	m_bTIM_MDX_READY_CHECK = FALSE;

// 	if(m_pVision)
// 	{
// 		delete m_pVision;
// 		m_pVision = NULL;
// 	}


	DelImg();

	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}

	if(m_pPcsGL)
	{
		delete m_pPcsGL;
		m_pPcsGL = NULL;
	}

// 	if(m_pPinGL)
// 	{
// 		delete m_pPinGL;
// 		m_pPinGL = NULL;
// 	}
}

BOOL CDlgMenu02::Create()
{
	return CDialog::Create(CDlgMenu02::IDD);
}

void CDlgMenu02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMenu02)
	DDX_Control(pDX, IDC_SLD_LT, m_LightSlider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMenu02, CDialog)
	//{{AFX_MSG_MAP(CDlgMenu02)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHK_LT_ON, OnChkLtOn)
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_JOG_VF, OnChkJogVf)
	ON_BN_CLICKED(IDC_CHK_JOG_F, OnChkJogF)
	ON_BN_CLICKED(IDC_CHK_JOG_N, OnChkJogN)
	ON_BN_CLICKED(IDC_CHK_JOG_S, OnChkJogS)
	ON_BN_CLICKED(IDC_CHK_JOG_VS, OnChkJogVs)
	ON_BN_CLICKED(IDC_BTN_PIN_MOVE, OnBtnPinMove)
	ON_BN_CLICKED(IDC_BTN_PIN_SAVE, OnBtnPinSave)
	ON_BN_CLICKED(IDC_BTN_HOME_MOVE, OnBtnHomeMove)
	ON_BN_CLICKED(IDC_CHK_RES_POS_ST, OnChkResPosSt)
	ON_BN_CLICKED(IDC_CHK_MK_OFFSET_ST, OnChkMkOffsetSt)
	ON_BN_CLICKED(IDC_CHK_MK_OFFSET_ED, OnChkMkOffsetEd)
	ON_BN_CLICKED(IDC_BTN_ALIGN_MOVE, OnBtnAlignMove)
	ON_BN_CLICKED(IDC_BTN_ALIGN, OnBtnAlign)
	ON_BN_CLICKED(IDC_BTN_GRAB, OnBtnGrab)
	ON_BN_CLICKED(IDC_BTN_START_SAVE, OnBtnHomeSave)
	ON_BN_CLICKED(IDC_STC_ALIGN_STD_SCR, OnStcAlignStdScr)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DRAW_PCSIMG, OnDrawPcsImg)
	ON_MESSAGE(WM_DRAW_PINIMG, OnDrawPinImg)
	ON_MESSAGE(WM_MYSTATIC_REDRAW, OnMyStaticRedraw)
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_LASER_ALIGN, &CDlgMenu02::OnBnClickedBtnLaserAlign)
	ON_STN_CLICKED(IDC_STC_5, &CDlgMenu02::OnStnClickedStc5)
	ON_STN_CLICKED(IDC_STC_007, &CDlgMenu02::OnStnClickedStc007)
	ON_STN_CLICKED(IDC_STC_010, &CDlgMenu02::OnStnClickedStc010)
	ON_STN_CLICKED(IDC_STC_12, &CDlgMenu02::OnStnClickedStc12)
	ON_STN_CLICKED(IDC_STC_40, &CDlgMenu02::OnStnClickedStc40)
	ON_STN_CLICKED(IDC_STC_17, &CDlgMenu02::OnStnClickedStc17)
	ON_STN_CLICKED(IDC_STC_19, &CDlgMenu02::OnStnClickedStc19)
	ON_STN_CLICKED(IDC_STC_36, &CDlgMenu02::OnStnClickedStc36)
	ON_STN_CLICKED(IDC_STC_45, &CDlgMenu02::OnStnClickedStc45)
	ON_STN_CLICKED(IDC_STC_180, &CDlgMenu02::OnStnClickedStc180)
	ON_STN_CLICKED(IDC_STC_184, &CDlgMenu02::OnStnClickedStc184)
	ON_STN_CLICKED(IDC_STC_218, &CDlgMenu02::OnStnClickedStc218)
	ON_STN_CLICKED(IDC_STC_225, &CDlgMenu02::OnStnClickedStc225)
	ON_BN_CLICKED(IDC_BTN_BUFF_INIT_SAVE1, &CDlgMenu02::OnBnClickedBtnBuffInitSave1)
	ON_BN_CLICKED(IDC_BTN_BUFF_INIT_MOVE1, &CDlgMenu02::OnBnClickedBtnBuffInitMove1)
	ON_STN_CLICKED(IDC_STC_38, &CDlgMenu02::OnStnClickedStc38)
	ON_STN_CLICKED(IDC_STC_191, &CDlgMenu02::OnStnClickedStc191)
	ON_BN_CLICKED(IDC_BTN_2D_READING, &CDlgMenu02::OnBnClickedBtn2dReading)
	ON_BN_CLICKED(IDC_BTN_LASER_MARKING, &CDlgMenu02::OnBnClickedBtnLaserMarking)
	ON_BN_CLICKED(IDC_BTN_LASER_ADJUST_ZERO, &CDlgMenu02::OnBnClickedBtnLaserAdjustZero)
	ON_BN_CLICKED(IDC_BTN_LASER_ADJUST_UP, &CDlgMenu02::OnBnClickedBtnLaserAdjustUp)
	ON_BN_CLICKED(IDC_BTN_LASER_ADJUST_RT, &CDlgMenu02::OnBnClickedBtnLaserAdjustRt)
	ON_BN_CLICKED(IDC_BTN_LASER_ADJUST_DN, &CDlgMenu02::OnBnClickedBtnLaserAdjustDn)
	ON_BN_CLICKED(IDC_BTN_LASER_ADJUST_LF, &CDlgMenu02::OnBnClickedBtnLaserAdjustLf)
	ON_BN_CLICKED(IDC_BTN_LASER_ADJUST_CW2, &CDlgMenu02::OnBnClickedBtnLaserAdjustCw2)
	ON_BN_CLICKED(IDC_BTN_LASER_ADJUST_CCW2, &CDlgMenu02::OnBnClickedBtnLaserAdjustCcw2)
	ON_BN_CLICKED(IDC_BTN_SHOT_REMAIN_CW, &CDlgMenu02::OnBnClickedBtnShotRemainCw)
	ON_BN_CLICKED(IDC_BTN_SHOT_REMAIN_CCW, &CDlgMenu02::OnBnClickedBtnShotRemainCcw)
	ON_BN_CLICKED(IDC_BTN_OFFSET_ENGRAVE_CW, &CDlgMenu02::OnBnClickedBtnOffsetEngraveCw)
	ON_BN_CLICKED(IDC_BTN_OFFSET_ENGRAVE_CCW, &CDlgMenu02::OnBnClickedBtnOffsetEngraveCcw)
	ON_BN_CLICKED(IDC_BTN_OFFSET_AOI_CW, &CDlgMenu02::OnBnClickedBtnOffsetAoiCw)
	ON_BN_CLICKED(IDC_BTN_OFFSET_AOI_CCW, &CDlgMenu02::OnBnClickedBtnOffsetAoiCcw)
	ON_BN_CLICKED(IDC_BTN_OFFSET_MK_CW, &CDlgMenu02::OnBnClickedBtnOffsetMkCw)
	ON_BN_CLICKED(IDC_BTN_OFFSET_MK_CCW, &CDlgMenu02::OnBnClickedBtnOffsetMkCcw)
	ON_STN_CLICKED(IDC_STC_49, &CDlgMenu02::OnStnClickedStc49)
	ON_STN_CLICKED(IDC_STC_52, &CDlgMenu02::OnStnClickedStc52)
	ON_STN_CLICKED(IDC_STC_55, &CDlgMenu02::OnStnClickedStc55)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu02 message handlers

void CDlgMenu02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		m_pRect = new CRect;
		
		this->GetClientRect(m_pRect);
		m_pRect->top = 75;
		m_pRect->bottom += 75;
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if(bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMenu02::AtDlgShow()
{
	LoadImg();

	CString str;
//	str.Format(_T("%.3f"), _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX));
	myStcData[3].SetText(pDoc->WorkingInfo.Vision[0].sMkOffsetX);
//	str.Format(_T("%.3f"), _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY));
	myStcData[4].SetText(pDoc->WorkingInfo.Vision[0].sMkOffsetY);
//	str.Format(_T("%.1f"), pDoc->WorkingInfo.Vision[0].dStdScr);
	myStcData[9].SetText(pDoc->WorkingInfo.Vision[0].sStdScr);

	SetJogSpd();

// 	if(!pView->IsPinMkData())			// 핀위치와 마킹위치 저장 확인.
// 		AfxMessageBox(_T("마킹 Pin위치 값이 설정되지 않았습니다."));

	m_nMoveAlign[0] = 0;
	m_nMoveAlign[1] = 0;

	Disp();
}

void CDlgMenu02::AtDlgHide()
{
	DelImg();
}

void CDlgMenu02::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for(i=0; i<MAX_MENU02_BTN; i++)
	{
		if(i==1)
		{
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==2)
		{
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==6)
		{
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==7)
		{
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==8)
		{
			myBtn[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==9)
		{
			myBtn[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 28)
		{
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 29)
		{
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu02, BTN_IMG_DN);
		}
	}
}

void CDlgMenu02::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;
	for(i=0; i<MAX_MENU02_BTN; i++)
	{
		myBtn[i].DelImgList();
	}
}

BOOL CDlgMenu02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HWND hW[2];
	CRect rt[2];
	void *pMilSys=NULL;

#ifdef USE_VISION
	if(pView->m_pVision[1])
	{
		delete pView->m_pVision[1];
		pView->m_pVision[1] = NULL;
	}
	if(pView->m_pVision[0])
	{
		delete pView->m_pVision[0];
		pView->m_pVision[0] = NULL;
	}

	HWND hCtrlV0[4] = {0};
	hCtrlV0[0] = GetDlgItem(IDC_STC_VISION)->GetSafeHwnd();
	MIL_ID MilSys = M_NULL;
	pView->m_pVision[0] = new CVision(0, MilSys, hCtrlV0, this);
	MilSys = pView->m_pVision[0]->GetSystemID();

#ifndef TEST_MODE
	HWND hCtrlV1[4] = { 0 };
	hCtrlV1[0] = GetDlgItem(IDC_STC_VISION2)->GetSafeHwnd();
	pView->m_pVision[1] = new CVision(1, MilSys, hCtrlV1, this);
	GetDlgItem(IDC_STC_VISION2)->ShowWindow(SW_HIDE);
#endif

	if(pView->m_pVision[0])
	{
		pView->m_pVision[0]->ClearOverlay();
		pView->m_pVision[0]->DrawCenterMark();
	}
#endif
	InitStatic();
	InitBtn();
	InitSlider();

	InitCadImg();


	myStcData[1].SetText(pDoc->WorkingInfo.Vision[0].sResX);
	myStcData[2].SetText(pDoc->WorkingInfo.Vision[0].sResY);


	m_bTIM_BUF_ENC = TRUE;
 	SetTimer(TIM_BUF_ENC, 100, NULL);

	GetDlgItem(IDC_GRP_LT4)->ShowWindow(SW_HIDE);
 	GetDlgItem(IDC_STC_MK_OFFSET)->ShowWindow(SW_HIDE);
 	GetDlgItem(IDC_STC_CAM_MK_X)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_CAM_MK_Y)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_CAM_MK_OFFSET_X)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_CAM_MK_OFFSET_Y)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MK_POS1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MK_POS2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_MK_OFFSET_ST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_MK_OFFSET_ED)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMenu02::InitCadImg()
{
	HWND hWin;
	CRect rect;

#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
		hWin = GetDlgItem(IDC_STC_PIN_IMG)->GetSafeHwnd();
		GetDlgItem(IDC_STC_PIN_IMG)->GetWindowRect(&rect);
		pView->m_pVision[0]->SelDispPin(hWin, rect, 0);
		pView->m_pVision[0]->ShowDispPin(0);

		//pView->m_pVision[0]->SelDispAlign(hWin, rect, 0);
		//pView->m_pVision[0]->ShowDispAlign();

		pView->m_pVision[0]->SetDispAxisPos();
	}
#endif
}

void CDlgMenu02::InitSlider()
{
	m_LightSlider.SetRange(0,100);
	m_LightSlider.SetLineSize(1);
	m_LightSlider.SetPageSize(10);
	m_LightSlider.SetPos(100);
	m_LightSlider.SetTicFreq(10);
}

void CDlgMenu02::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_LT_ON, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LT_ON);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_BTN_LT_UP, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_UP);
	myBtn[1].SetBoarder(FALSE);

	myBtn[2].SubclassDlgItem(IDC_BTN_LT_DN, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_DN);
	myBtn[2].SetBoarder(FALSE);

	myBtn[3].SubclassDlgItem(IDC_BTN_PIN_MOVE, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_MOVE);

	myBtn[4].SubclassDlgItem(IDC_BTN_PIN_SAVE, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_SAVE);

	myBtn[5].SubclassDlgItem(IDC_BTN_HOME_MOVE, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_HOME_MOVE);

	myBtn[6].SubclassDlgItem(IDC_BTN_JOG_UP, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_UP);
	myBtn[6].SetBoarder(FALSE);

	myBtn[7].SubclassDlgItem(IDC_BTN_JOG_DN, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_DN);
	myBtn[7].SetBoarder(FALSE);

	myBtn[8].SubclassDlgItem(IDC_BTN_JOG_LF, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_LF);
	myBtn[8].SetBoarder(FALSE);

	myBtn[9].SubclassDlgItem(IDC_BTN_JOG_RT, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_RT);
	myBtn[9].SetBoarder(FALSE);

	myBtn[10].SubclassDlgItem(IDC_CHK_JOG_VF, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VF);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_CHK_JOG_F, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_F);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);

	myBtn[12].SubclassDlgItem(IDC_CHK_JOG_N, this);
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_N);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_JOG_S, this);
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_S);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_JOG_VS, this);
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VS);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	myBtn[15].SubclassDlgItem(IDC_BTN_ALIGN, this);
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN);

	myBtn[16].SubclassDlgItem(IDC_CHK_RES_POS_ST, this);
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_RES_POS_ST);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);

	myBtn[17].SubclassDlgItem(IDC_BTN_START_SAVE, this);
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_BTN_START_SAVE);

	myBtn[18].SubclassDlgItem(IDC_CHK_MK_OFFSET_ST, this);
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ST);
	myBtn[18].SetBtnType(BTN_TYPE_CHECK);

	myBtn[19].SubclassDlgItem(IDC_CHK_MK_OFFSET_ED, this);
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ED);
	myBtn[19].SetBtnType(BTN_TYPE_CHECK);

	myBtn[20].SubclassDlgItem(IDC_BTN_ALIGN_MOVE, this);
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN_MOVE);

	myBtn[21].SubclassDlgItem(IDC_BTN_GRAB, this);
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GRAB);

	myBtn[22].SubclassDlgItem(IDC_BTN_OFFSET_ENGRAVE_CW, this);
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_BTN_OFFSET_ENGRAVE_CW);

	myBtn[23].SubclassDlgItem(IDC_BTN_LASER_ADJUST_LF, this);
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LASER_ADJUST_LF);

	myBtn[24].SubclassDlgItem(IDC_BTN_LASER_ALIGN, this);
	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LASER_ALIGN);
	myBtn[24].SetBtnType(BTN_TYPE_CHECK);

	myBtn[25].SubclassDlgItem(IDC_BTN_LASER_MARKING, this);
	myBtn[25].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LASER_MARKING);
	myBtn[25].SetBtnType(BTN_TYPE_CHECK);

	myBtn[26].SubclassDlgItem(IDC_BTN_2D_READING, this);
	myBtn[26].SetHwnd(this->GetSafeHwnd(), IDC_BTN_2D_READING);

	//myBtn[27].SubclassDlgItem(IDC_BTN_FEED_SHOT_OFFSET, this);
	//myBtn[27].SetHwnd(this->GetSafeHwnd(), IDC_BTN_FEED_SHOT_OFFSET);
	myBtn[27].SubclassDlgItem(IDC_BTN_SHOT_REMAIN_CW, this);
	myBtn[27].SetHwnd(this->GetSafeHwnd(), IDC_BTN_SHOT_REMAIN_CW);

	myBtn[28].SubclassDlgItem(IDC_BTN_BUFF_UP1, this);
	myBtn[28].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_UP1);
	myBtn[28].SetBoarder(FALSE);

	myBtn[29].SubclassDlgItem(IDC_BTN_BUFF_DN1, this);
	myBtn[29].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_DN1);
	myBtn[29].SetBoarder(FALSE);

	myBtn[30].SubclassDlgItem(IDC_BTN_BUFF_HOME1, this);
	myBtn[30].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_HOME1);

	myBtn[31].SubclassDlgItem(IDC_BTN_BUFF_INIT_MOVE1, this);
	myBtn[31].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_INIT_MOVE1);

	myBtn[32].SubclassDlgItem(IDC_BTN_BUFF_INIT_SAVE1, this);
	myBtn[32].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_INIT_SAVE1);

	myBtn[33].SubclassDlgItem(IDC_BTN_OFFSET_ENGRAVE_CCW, this);
	myBtn[33].SetHwnd(this->GetSafeHwnd(), IDC_BTN_OFFSET_ENGRAVE_CCW);

	myBtn[34].SubclassDlgItem(IDC_BTN_OFFSET_AOI_CW, this);
	myBtn[34].SetHwnd(this->GetSafeHwnd(), IDC_BTN_OFFSET_AOI_CW);

	myBtn[35].SubclassDlgItem(IDC_BTN_OFFSET_AOI_CCW, this);
	myBtn[35].SetHwnd(this->GetSafeHwnd(), IDC_BTN_OFFSET_AOI_CCW);

	myBtn[36].SubclassDlgItem(IDC_BTN_OFFSET_MK_CW, this);
	myBtn[36].SetHwnd(this->GetSafeHwnd(), IDC_BTN_OFFSET_MK_CW);

	myBtn[37].SubclassDlgItem(IDC_BTN_OFFSET_MK_CCW, this);
	myBtn[37].SetHwnd(this->GetSafeHwnd(), IDC_BTN_OFFSET_MK_CCW);

	myBtn[38].SubclassDlgItem(IDC_BTN_LASER_ADJUST_RT, this);
	myBtn[38].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LASER_ADJUST_RT);

	myBtn[39].SubclassDlgItem(IDC_BTN_LASER_ADJUST_ZERO, this);
	myBtn[39].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LASER_ADJUST_ZERO);

	myBtn[40].SubclassDlgItem(IDC_BTN_LASER_ADJUST_UP, this);
	myBtn[40].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LASER_ADJUST_UP);

	myBtn[41].SubclassDlgItem(IDC_BTN_LASER_ADJUST_DN, this);
	myBtn[41].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LASER_ADJUST_DN);

	myBtn[42].SubclassDlgItem(IDC_BTN_LASER_ADJUST_CW2, this);
	myBtn[42].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LASER_ADJUST_CW2);

	myBtn[43].SubclassDlgItem(IDC_BTN_LASER_ADJUST_CCW2, this);
	myBtn[43].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LASER_ADJUST_CCW2);

	myBtn[44].SubclassDlgItem(IDC_BTN_SHOT_REMAIN_CCW, this);
	myBtn[44].SetHwnd(this->GetSafeHwnd(), IDC_BTN_SHOT_REMAIN_CCW);

	int i;
	for(i=0; i<MAX_MENU02_BTN; i++)
	{
		switch(i)
		{
		case 23:	// IDC_BTN_LASER_ADJUST_LF
		case 38:	// IDC_BTN_LASER_ADJUST_RT
		case 39:	// IDC_BTN_LASER_ADJUST_ZERO
		case 40:	// IDC_BTN_LASER_ADJUST_UP
		case 41:	// IDC_BTN_LASER_ADJUST_DN
		case 42:	// IDC_BTN_LASER_ADJUST_CW2
		case 43:	// IDC_BTN_LASER_ADJUST_CCW2
			myBtn[i].SetFont(_T("굴림체"), 14, TRUE);
			myBtn[i].SetTextColor(RGB_RED);
			break;
		default:
			myBtn[i].SetFont(_T("굴림체"), 14, TRUE);
			myBtn[i].SetTextColor(RGB_BLACK);
			//myBtn[i].SetBoarder(FALSE);
			break;
		}
	}

	myBtn[18].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
	myBtn[19].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED
}


void CDlgMenu02::InitStatic()
{
// 	myStcPcsImg.SubclassDlgItem(IDC_STC_PCS_IMG, this);
// 	myStcPcsImg.SetHwnd(this->GetSafeHwnd(), IDC_STC_PCS_IMG);

// 	myStcPinImg.SubclassDlgItem(IDC_STC_PIN_IMG, this);
// 	myStcPinImg.SetHwnd(this->GetSafeHwnd(), IDC_STC_PIN_IMG);

	InitStcTitle();
	InitStcData();
}


void CDlgMenu02::InitStcTitle()
{
	myLblTitle[0].SubclassDlgItem(IDC_STC_RES, this);
	myLblTitle[1].SubclassDlgItem(IDC_STC_RES_X, this);
	myLblTitle[2].SubclassDlgItem(IDC_STC_RES_Y, this);
	myLblTitle[3].SubclassDlgItem(IDC_STC_MK_OFFSET, this);
	myLblTitle[4].SubclassDlgItem(IDC_STC_CAM_MK_X, this);
	myLblTitle[5].SubclassDlgItem(IDC_STC_CAM_MK_Y, this);
	myLblTitle[6].SubclassDlgItem(IDC_STC_MK_POS1, this);
	myLblTitle[7].SubclassDlgItem(IDC_STC_MK_POS2, this);
	myLblTitle[8].SubclassDlgItem(IDC_STC_ALN, this);
	myLblTitle[9].SubclassDlgItem(IDC_STC_ALN_X, this);
	myLblTitle[10].SubclassDlgItem(IDC_STC_ALN_Y, this);
	myLblTitle[11].SubclassDlgItem(IDC_STC_ALN_ANGL, this);
	myLblTitle[12].SubclassDlgItem(IDC_STC_ALN_SCR, this);
	myLblTitle[13].SubclassDlgItem(IDC_STC_ALN_SCR2, this);

	myStcTitle[0].SubclassDlgItem(IDC_STC_000, this);
	myStcTitle[1].SubclassDlgItem(IDC_STC_2, this);
	myStcTitle[2].SubclassDlgItem(IDC_STC_4, this);
	myStcTitle[3].SubclassDlgItem(IDC_STC_10, this);
	myStcTitle[4].SubclassDlgItem(IDC_STC_005, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_006, this);
	myStcTitle[6].SubclassDlgItem(IDC_STC_008, this);
	myStcTitle[7].SubclassDlgItem(IDC_STC_11, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_011, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_1, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_24, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_13, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_37, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_41, this);

	myStcTitle[14].SubclassDlgItem(IDC_STC_14, this);
	myStcTitle[15].SubclassDlgItem(IDC_STC_15, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_16, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_18, this);

	myStcTitle[18].SubclassDlgItem(IDC_STC_22, this);
	myStcTitle[19].SubclassDlgItem(IDC_STC_30, this);
	myStcTitle[20].SubclassDlgItem(IDC_STC_31, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_33, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_35, this);

	myStcTitle[23].SubclassDlgItem(IDC_STC_42, this);
	myStcTitle[24].SubclassDlgItem(IDC_STC_43, this);
	myStcTitle[25].SubclassDlgItem(IDC_STC_47, this);
	myStcTitle[26].SubclassDlgItem(IDC_STC_44, this);
	myStcTitle[27].SubclassDlgItem(IDC_STC_174, this);
	myStcTitle[28].SubclassDlgItem(IDC_STC_183, this);
	myStcTitle[29].SubclassDlgItem(IDC_STC_46, this);
	myStcTitle[30].SubclassDlgItem(IDC_STC_182, this);
	myStcTitle[31].SubclassDlgItem(IDC_STC_185, this);

	myStcTitle[32].SubclassDlgItem(IDC_STC_217, this);
	myStcTitle[33].SubclassDlgItem(IDC_STC_227, this);
	myStcTitle[34].SubclassDlgItem(IDC_STC_224, this);
	myStcTitle[35].SubclassDlgItem(IDC_STC_223, this);
	myStcTitle[36].SubclassDlgItem(IDC_STC_226, this);

	myStcTitle[37].SubclassDlgItem(IDC_STC_BUF_ROL1, this);
	myStcTitle[38].SubclassDlgItem(IDC_STC_BUF_HI1, this);

	myStcTitle[39].SubclassDlgItem(IDC_STC_186, this);
	myStcTitle[40].SubclassDlgItem(IDC_STC_187, this);
	myStcTitle[41].SubclassDlgItem(IDC_STC_189, this);
	myStcTitle[42].SubclassDlgItem(IDC_STC_188, this);
	myStcTitle[43].SubclassDlgItem(IDC_STC_192, this);
	myStcTitle[44].SubclassDlgItem(IDC_STC_190, this);
	myStcTitle[45].SubclassDlgItem(IDC_STC_193, this);

	myStcTitle[46].SubclassDlgItem(IDC_STC_48, this);
	myStcTitle[47].SubclassDlgItem(IDC_STC_50, this);
	myStcTitle[48].SubclassDlgItem(IDC_STC_53, this);
	myStcTitle[49].SubclassDlgItem(IDC_STC_56, this);
	myStcTitle[50].SubclassDlgItem(IDC_STC_51, this);
	myStcTitle[51].SubclassDlgItem(IDC_STC_54, this);
	myStcTitle[52].SubclassDlgItem(IDC_STC_57, this);

	for (int i = 0; i < MAX_MENU02_LBL; i++)
	{
		myLblTitle[i].SetFontName(_T("Arial"));
		myLblTitle[i].SetFontSize(14);
		myLblTitle[i].SetFontBold(TRUE);
		myLblTitle[i].SetTextColor(RGB_NAVY);
		myLblTitle[i].SetBkColor(RGB_DLG_FRM);
	}

	for(int i=0; i<MAX_MENU02_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(14);

		if(i==0 || i==14 || i==18 || i==23 || i==39)																				//대제목
		{
			myStcTitle[i].SetFontBold(TRUE);
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
		}
		else if (i == 1 || i==15 || i==19 || i==24 || i==40)																		// 데이타 칼럼
		{
			myStcTitle[i].SetFontBold(TRUE);
			myStcTitle[i].SetTextColor(RGB_WHITE);
			myStcTitle[i].SetBkColor(RGB_DARKGREEN);
		}
		else if (i == 2 || i==3 || i==5 || i==7 || i==9 || i==10 || i==12 || i==16 || i==17 || i==20 || i==21 || i==22				// 중,소 제목
					|| i == 25 || i == 26 || i == 27 || i == 28 || i == 32 || i == 33 || i == 34 || i == 41 || i == 43
					|| i == 46 || i == 47 || i == 48 || i == 49)					
		{
			myStcTitle[i].SetFontBold(TRUE);
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTGRAY);
		}
		else if (i == 4 || i==6 || i==8 || i==11 || i==13 || i==29 || i==30 || i==31 || i==35 || i==36 || i==42 || i==44			// [단위]
					|| i == 45 || i == 50 || i == 51 || i == 52)
		{
			myStcTitle[i].SetFontBold(FALSE);
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_WHITE);
		}
		else
		{
			myStcTitle[i].SetFontBold(TRUE);
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_DLG_FRM);
		}
	}
}


void CDlgMenu02::InitStcData()
{
	myStcData[0].SubclassDlgItem(IDC_STC_LT_VAL, this);
	myStcData[1].SubclassDlgItem(IDC_STC_RESOLUTION_X, this);
	myStcData[2].SubclassDlgItem(IDC_STC_RESOLUTION_Y, this);
	myStcData[3].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_X, this);
	myStcData[4].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_Y, this);
	myStcData[5].SubclassDlgItem(IDC_STC_ALIGN_X, this);
	myStcData[6].SubclassDlgItem(IDC_STC_ALIGN_Y, this);
	myStcData[7].SubclassDlgItem(IDC_STC_ALIGN_AGL, this);
	myStcData[8].SubclassDlgItem(IDC_STC_ALIGN_SCR, this);
	myStcData[9].SubclassDlgItem(IDC_STC_ALIGN_STD_SCR, this);

	myStcData[10].SubclassDlgItem(IDC_STC_5, this);
	myStcData[11].SubclassDlgItem(IDC_STC_007, this);
	myStcData[12].SubclassDlgItem(IDC_STC_010, this);
	myStcData[13].SubclassDlgItem(IDC_STC_12, this);
	myStcData[14].SubclassDlgItem(IDC_STC_40, this);

	myStcData[15].SubclassDlgItem(IDC_STC_17, this);
	myStcData[16].SubclassDlgItem(IDC_STC_19, this);

	myStcData[17].SubclassDlgItem(IDC_STC_32, this);
	myStcData[18].SubclassDlgItem(IDC_STC_34, this);
	myStcData[19].SubclassDlgItem(IDC_STC_36, this);

	myStcData[20].SubclassDlgItem(IDC_STC_45, this);
	myStcData[21].SubclassDlgItem(IDC_STC_180, this);
	myStcData[22].SubclassDlgItem(IDC_STC_184, this);
	myStcData[23].SubclassDlgItem(IDC_STC_218, this);
	myStcData[24].SubclassDlgItem(IDC_STC_225, this);

	myStcData[25].SubclassDlgItem(IDC_STC_BUF_POS1, this);

	myStcData[26].SubclassDlgItem(IDC_STC_38, this);
	myStcData[27].SubclassDlgItem(IDC_STC_191, this);

	myStcData[28].SubclassDlgItem(IDC_STC_49, this);
	myStcData[29].SubclassDlgItem(IDC_STC_52, this);
	myStcData[30].SubclassDlgItem(IDC_STC_55, this);

	for(int i=0; i<MAX_MENU02_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		if (i == 0)
			myStcData[i].SetFontSize(22);
		else if (i < 10)
			myStcData[i].SetFontSize(14);
		else
			myStcData[i].SetFontSize(20);

		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}

//	myStcData[6].EnableWindow(FALSE); // IDC_STC_CAM_MK_OFFSET_Y
}


LRESULT CDlgMenu02::OnDrawPcsImg(WPARAM wPara, LPARAM lPara)
{
// 	if(m_pPcsGL)
// 	{
// 		m_pPcsGL->Draw();
// 	}
	return 0L;
}

LRESULT CDlgMenu02::OnDrawPinImg(WPARAM wPara, LPARAM lPara)
{
// 	if(m_pPinGL)
// 	{
// 		m_pPinGL->Draw();
// 	}
	return 0L;
}

LRESULT CDlgMenu02::OnMyStaticRedraw(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	
	switch(nCtrlID)
	{
// 	case IDC_STC_PCS_IMG:
// 		this->PostMessage(WM_DRAW_PCSIMG, 0, 0);
// 		break;
	case IDC_STC_PIN_IMG:
		this->PostMessage(WM_DRAW_PINIMG, 0, 0);
		break;
	}

	return 0L;
}


void CDlgMenu02::SetLight(int nVal)
{
	if(pView->m_pLight)
	{
		pView->m_pLight->Set(_tstoi(pDoc->WorkingInfo.Light.sCh[0]), nVal);

		if(nVal)
		{
			if(!myBtn[0].GetCheck())
				myBtn[0].SetCheck(TRUE);
		}
		else
		{
			if(myBtn[0].GetCheck())
				myBtn[0].SetCheck(FALSE);
		}

		nVal = pView->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));

		int nPos;
		nPos = int(100.0*(1.0 - (nVal-0.5)/255.0));
		m_LightSlider.SetPos(nPos);
		CString str;
		str.Format(_T("%d"), nVal);
		myStcData[0].SetText(str);

		pDoc->WorkingInfo.Light.sVal[0] = str;
		::WritePrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);		
	}
}


void CDlgMenu02::ResetLight()
{
	if(pView->m_pLight)
	{
		if(myBtn[0].GetCheck())
			myBtn[0].SetCheck(FALSE);

		pView->m_pLight->Reset(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
	}
}


int CDlgMenu02::GetJogSpd()
{
	return m_nSpd;
}

void CDlgMenu02::SetJogSpd()
{
	SetJogSpd(m_nSpd);
}

void CDlgMenu02::SetJogSpd(int nSpd)
{
	double dSpdX, dAccX, dSpdY, dAccY;

	m_nSpd = nSpd;

	myBtn[10].SetCheck(FALSE);	//	IDC_CHK_JOG_VF
	myBtn[11].SetCheck(FALSE);	//	IDC_CHK_JOG_F
	myBtn[12].SetCheck(FALSE);	//	IDC_CHK_JOG_N
	myBtn[13].SetCheck(FALSE);	//	IDC_CHK_JOG_S
	myBtn[14].SetCheck(FALSE);	//	IDC_CHK_JOG_VS

	switch(nSpd)
	{
	case 0: // VS
		myBtn[14].SetCheck(TRUE);	//	IDC_CHK_JOG_VS
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogLowSpd / 150.0;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc / 150.0;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogLowSpd / 150.0;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc / 150.0;
		}
		break;
	case 1: // S
		myBtn[13].SetCheck(TRUE);	//	IDC_CHK_JOG_S
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogLowSpd / 10.0;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc / 10.0;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogLowSpd / 10.0;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc / 10.0;
		}
		break;
	case 2: // N
		myBtn[12].SetCheck(TRUE);	//	IDC_CHK_JOG_N
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogLowSpd;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogLowSpd;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc;
		}
		break;
	case 3: // F
		myBtn[11].SetCheck(TRUE);	//	IDC_CHK_JOG_F
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogMidSpd;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogMidSpd;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc;
		}
		break;
	case 4: // VF
		myBtn[10].SetCheck(TRUE);	//	IDC_CHK_JOG_VF
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogFastSpd;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogFastSpd;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc;
		}
		break;
	}

	if(pView->m_pMotion)
	{
		pView->m_pMotion->SetVMove(MS_Y0, dSpdY, dAccY);
//		if(!pView->ChkCollision())
			pView->m_pMotion->SetVMove(MS_X0, dSpdX, dAccX);
		m_nJogSpd = nSpd;
	}
}

void CDlgMenu02::OnChkLtOn() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[0].GetCheck();
	if(bOn)
	{
		myBtn[1].EnableWindow(TRUE);
		GetDlgItem(IDC_SLD_LT)->EnableWindow(TRUE);
		myBtn[2].EnableWindow(TRUE);

		SetLight();
	}
	else
	{
		ResetLight();
		
		myBtn[1].EnableWindow(FALSE);
		GetDlgItem(IDC_SLD_LT)->EnableWindow(FALSE);
		myBtn[2].EnableWindow(FALSE);
	}
}

void CDlgMenu02::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	int LightValue;
	if(pScrollBar == (CScrollBar*)&m_LightSlider)
	{
		nPos = m_LightSlider.GetPos();
		str = myStcData[0].GetText();
		LightValue = (int)floor(((double)(100-nPos)/100.0)*255.0+0.5);
		pView->m_pLight->Set(_tstoi(pDoc->WorkingInfo.Light.sCh[0]), LightValue);
		str.Format(_T("%d"),LightValue);
		myStcData[0].SetText(str);

		pDoc->WorkingInfo.Light.sVal[0] = str;
		::WritePrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);		
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CDlgMenu02::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	SwMyBtnDown(nCtrlID);

	return 0L;
}

void CDlgMenu02::SwMyBtnDown(int nCtrlID)
{
	switch(nCtrlID)
	{
	case IDC_BTN_LT_UP:
		if(!m_bTIM_LIGHT_UP)
		{
			m_bTIM_LIGHT_UP = TRUE;
			SetTimer(TIM_LIGHT_UP, 100, NULL);
		}
		break;
	case IDC_BTN_LT_DN:
		if(!m_bTIM_LIGHT_DN)
		{
			m_bTIM_LIGHT_DN = TRUE;
			SetTimer(TIM_LIGHT_DN, 100, NULL);
		}
		break;
	case IDC_BTN_JOG_UP:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_Y0, M_CCW);
		break;
	case IDC_BTN_JOG_DN:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_Y0, M_CW);
		break;
	case IDC_BTN_JOG_RT:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_X0, M_CW);
		break;
	case IDC_BTN_JOG_LF:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_X0, M_CCW);
		break;
	case IDC_BTN_BUFF_UP1:		// 마킹부 피딩 정회전 스위치
		if (pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005513"), 1);
		break;
	case IDC_BTN_BUFF_DN1:		// 마킹부 피딩 역회전 스위치
		if (pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005514"), 1);
		break;
	}
}

LRESULT CDlgMenu02::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	SwMyBtnUp(nCtrlID);
	return 0L;
}

void CDlgMenu02::SwMyBtnUp(int nCtrlID)
{
	switch(nCtrlID)
	{
	case IDC_BTN_LT_UP:
		m_bTIM_LIGHT_UP = FALSE;
		break;
	case IDC_BTN_LT_DN:
		m_bTIM_LIGHT_DN = FALSE;
		break;
	case IDC_BTN_JOG_UP:
	case IDC_BTN_JOG_DN:
		if(pView->m_pMotion)
		{
			pView->m_pMotion->EStop(MS_Y0);
// 			MsClr(MS_Y);
			ResetMotion(MS_Y0);
		}
		break;
	case IDC_BTN_JOG_RT:
	case IDC_BTN_JOG_LF:
		if(pView->m_pMotion)
		{
			pView->m_pMotion->EStop(MS_X0);
// 			MsClr(MS_X);
			ResetMotion(MS_X0);
		}
		break;
	}
}
	
void CDlgMenu02::ResetMotion(int nMsId)
{
	if(!pView->m_pMotion)
		return;

	long lRtn = pView->m_pMotion->GetState(nMsId);  // -1 : MPIStateERROR, 0 : MPIStateIDLE, 1 : MPIStateSTOPPING, 2 : MPIStateMOVING
	if(lRtn==2)
	{
		if(nMsId==MS_X0 || nMsId==MS_Y0)
			pView->m_pMotion->Abort(MS_X0Y0);
		else if(nMsId==MS_X1 || nMsId==MS_Y1)
			pView->m_pMotion->Abort(MS_X1Y1);
		else
			pView->m_pMotion->Abort(nMsId);
		Sleep(30);
	}

	if(nMsId==MS_X0 || nMsId==MS_Y0)
		pView->m_pMotion->Clear(MS_X0Y0);
	else if(nMsId==MS_X1 || nMsId==MS_Y1)
		pView->m_pMotion->Clear(MS_X1Y1);
	else
		pView->m_pMotion->Clear(nMsId);

	Sleep(30);

	if(!pView->m_pMotion->IsEnable(nMsId))
	{
		if(nMsId==MS_X0Y0 || nMsId==MS_X0 || nMsId==MS_Y0)
		{
			pView->m_pMotion->ServoOnOff(AXIS_X0, TRUE);
			Sleep(30);
			pView->m_pMotion->ServoOnOff(AXIS_Y0, TRUE);
		}
		//else if(nMsId==MS_X1Y1 || nMsId==MS_X1 || nMsId==MS_Y1)
		//{
		//	pView->m_pMotion->ServoOnOff(AXIS_X1, TRUE);
		//	Sleep(30);
		//	pView->m_pMotion->ServoOnOff(AXIS_Y1, TRUE);
		//}
		else
			pView->m_pMotion->ServoOnOff(nMsId, TRUE);
	
		Sleep(30);
	}
}

void CDlgMenu02::MsClr(int nMsId)
{
	if(pView->m_pMotion)
	{
		Sleep(30);

		long lRtn = pView->m_pMotion->GetState(nMsId);  // -1 : MPIStateERROR, 0 : MPIStateIDLE, 1 : MPIStateSTOPPING, 2 : MPIStateMOVING
		if(lRtn==2)//lRtn<0 || 
		{
			if(nMsId==MS_X0 || nMsId==MS_Y0)
				pView->m_pMotion->Abort(MS_X0Y0);
			else if(nMsId==MS_X1 || nMsId==MS_Y1)
				pView->m_pMotion->Abort(MS_X1Y1);
			else
				pView->m_pMotion->Abort(nMsId);
			Sleep(30);
		}

		if(nMsId==MS_X0 || nMsId==MS_Y0)
			pView->m_pMotion->Clear(MS_X0Y0);
		else if(nMsId==MS_X1 || nMsId==MS_Y1)
			pView->m_pMotion->Clear(MS_X1Y1);
		else
			pView->m_pMotion->Clear(nMsId);
		Sleep(30);

		if(lRtn==2)//lRtn<0 || 
		{
			if(nMsId==MS_X0Y0 || nMsId==MS_X0 || nMsId==MS_Y0)
			{
				pView->m_pMotion->ServoOnOff(AXIS_X0, TRUE);
				Sleep(30);
				pView->m_pMotion->ServoOnOff(AXIS_Y0, TRUE);
			}
			//else if(nMsId==MS_X1Y1 || nMsId==MS_X1 || nMsId==MS_Y1)
			//{
			//	pView->m_pMotion->ServoOnOff(AXIS_X1, TRUE);
			//	Sleep(30);
			//	pView->m_pMotion->ServoOnOff(AXIS_Y1, TRUE);
			//}
			else
				pView->m_pMotion->ServoOnOff(nMsId, TRUE);
		
			Sleep(30);
		}
	}
}

void CDlgMenu02::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);

	int nVal;
	if(!m_bLockTimer)
	{
		m_bLockTimer = TRUE;
		switch(nIDEvent)
		{
		case TIM_LIGHT_UP:
	//		KillTimer(TIM_LIGHT_UP);
			nVal = pView->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
			nVal += 5;
			if(nVal > 255)
				nVal = 255;
			SetLight(nVal);
			if(m_bTIM_LIGHT_UP)
				SetTimer(TIM_LIGHT_UP, 100, NULL);
			break;
		case TIM_LIGHT_DN:
	//		KillTimer(TIM_LIGHT_DN);
			nVal = pView->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
			nVal -= 5;
			if(nVal < 0)
				nVal = 0;
			SetLight(nVal);
			if(m_bTIM_LIGHT_DN)
				SetTimer(TIM_LIGHT_DN, 100, NULL);
			break;
		case TIM_BUF_ENC:
	//		KillTimer(TIM_BUF_ENC);
			DispAxisPos();
	// 		DispBufEnc();
			if(m_bTIM_BUF_ENC)
				SetTimer(TIM_BUF_ENC, 100, NULL);
			break;
		case TIM_MARKING_OFF:
	//		KillTimer(TIM_MARKING_OFF);
			MarkingOff();
			break;
	//	case TIM_2D_READING:
	////		KillTimer(TIM_2D_READING);
	//		if(!Disp2dCode())
	//		{
	//			SetTimer(TIM_2D_READING, 100, NULL);
	//		}
	//		else
	//			m_bTIM_2D_READING = FALSE;
	//		break;
	//	case TIM_TEST_2D_READING:
	////		KillTimer(TIM_2D_READING);
	//		if(!DispTest2dCode())
	//		{
	//			SetTimer(TIM_TEST_2D_READING, 100, NULL);
	//		}
	//		else
	//			m_bTIM_TEST_2D_READING = FALSE;
	//		break;
		case TIM_MDX_RESPONSE:
			KillTimer(TIM_MDX_RESPONSE);
			if(m_bTIM_MDX_RESPONSE)
			{ 
				if(!ChkMdxResponse())
				{
					SetTimer(TIM_MDX_RESPONSE, 100, NULL);
				}
				else
				{
					if(myBtn[25].GetCheck())
						myBtn[25].SetCheck(FALSE);	// IDC_BTN_LASER_MARKING

					m_bTIM_MDX_RESPONSE = FALSE;
				}
			}
			break;
		case TIM_MDX_READY:
			KillTimer(TIM_MDX_READY);
			if (m_bTIM_MDX_READY)
			{
				if (IsMdxReady())
				{
					m_bTIM_MDX_READY_CHECK = TRUE;
					SetTimer(TIM_MDX_READY_CHECK, 100, NULL);
					m_stTime = GetTickCount64();
				}
				else
				{
					if (myBtn[24].GetCheck())
						myBtn[24].SetCheck(FALSE);	// IDC_BTN_LASER_ALIGN

					m_bTIM_MDX_READY = FALSE;
				}
			}
			break;
		case TIM_MDX_READY_CHECK:
			KillTimer(TIM_MDX_READY_CHECK);
			if (m_bTIM_MDX_READY_CHECK)
			{
				if (!ChkMdxReady())
				{
					m_bTIM_MDX_READY_CHECK = TRUE;
					SetTimer(TIM_MDX_READY_CHECK, 100, NULL);
				}
				else
				{
					if (myBtn[24].GetCheck())
						myBtn[24].SetCheck(FALSE);	// IDC_BTN_LASER_ALIGN

					m_bTIM_MDX_READY_CHECK = FALSE;
				}
			}
			break;
		}

		m_bLockTimer = FALSE;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgMenu02::OnChkJogVf() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(4);
}

void CDlgMenu02::OnChkJogF() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(3);
}

void CDlgMenu02::OnChkJogN() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(2);
}

void CDlgMenu02::OnChkJogS() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(1);
}

void CDlgMenu02::OnChkJogVs() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(0);
}

void CDlgMenu02::OnBtnPinMove() 
{
	// TODO: Add your control notification handler code here
	MovePinPos();
}


BOOL CDlgMenu02::MovePos(int nPos) 
{
	if(!pView->m_pMotion)
		return FALSE;

	//if(pView->ChkCollision(AXIS_X0, _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[nPos])))
	//{
	//	CfPoint ptPnt;
	//	ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
	//	ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
	//	pView->Move1(ptPnt, pView->m_bCam);
	//}

	if(_tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[nPos]) > 0.0 && _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosY[nPos]) > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X0];
		double dCurrY = pView->m_dEnc[AXIS_X0];

		double pPos[2];
		pPos[0] = _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[nPos]);
		pPos[1] = _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosY[nPos]);

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
			{
				if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}

//BOOL CDlgMenu02::MovePos2(int nPos) 
//{
//	if(!pView->m_pMotion)
//		return FALSE;
//
//	if(pView->ChkCollision(AXIS_X1, _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[nPos])))
//	{
//		CfPoint ptPnt;
//		ptPnt.x = 0.0;
//		ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y0];
//		pView->Move0(ptPnt, pView->m_bCam);
//	}
//
//	if(_tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[nPos]) > 0.0 && _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosY[nPos]) > 0.0)
//	{
//		double dCurrX = pView->m_dEnc[AXIS_X1];
//		double dCurrY = pView->m_dEnc[AXIS_X1];
//
//		double pPos[2];
//		pPos[0] = _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[nPos]);
//		pPos[1] = _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosY[nPos]);
//
//		double fLen, fVel, fAcc, fJerk;
//		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
//		if(fLen > 0.001)
//		{
//			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
//			if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
//			{
//				if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
//					AfxMessageBox(_T("Move XY Error..."));
//			}
//		}
//
//		return TRUE;
//	}
//
//	return FALSE;
//}


BOOL CDlgMenu02::MovePinPos() 
{
	if(!pView->m_pMotion)
		return FALSE;

	SetLight();

	if(pView->m_pMotion->m_dPinPosY[0] > 0.0 && pView->m_pMotion->m_dPinPosX[0] > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X0];
		double dCurrY = pView->m_dEnc[AXIS_Y0];

		double pPos[2];
		pPos[0] = pView->m_pMotion->m_dPinPosX[0];
		pPos[1] = pView->m_pMotion->m_dPinPosY[0];

		//if(pView->ChkCollision(AXIS_X0, pPos[0]))
		//{
		//	CfPoint ptPnt;
		//	ptPnt.x =  _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
		//	ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
		//	pView->Move1(ptPnt);
		//}

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
			{
				if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}


//BOOL CDlgMenu02::MoveAlign0(int nPos) 
//{
//	if(!pView->m_pMotion)
//		return FALSE;
//
//	SetLight();
//
//	if(pView->m_pMotion->m_dPinPosY[0] > 0.0 && pView->m_pMotion->m_dPinPosX[0] > 0.0)
//	{
//		double dCurrX = pView->m_dEnc[AXIS_X0];
//		double dCurrY = pView->m_dEnc[AXIS_Y0];
//
//		double pPos[2];
//		if(nPos==0)
//		{
//			pPos[0] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0];
//			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0];
//		}
//		else if(nPos==1)
//		{
//			pPos[0] = pDoc->m_Master[0].m_stAlignMk.X2 + pView->m_pMotion->m_dPinPosX[0];
//			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y2 + pView->m_pMotion->m_dPinPosY[0];
//		}
//
//		//if(pView->ChkCollision(AXIS_X0, pPos[0]))
//		//{
//		//	CfPoint ptPnt;
//		//	ptPnt.x =  _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
//		//	ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
//		//	pView->Move1(ptPnt);
//		//}
//
//		double fLen, fVel, fAcc, fJerk;
//		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
//		if(fLen > 0.001)
//		{
//			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
//			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
//			{
//				if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
//					pView->MsgBox(_T("Move XY Error..."));
//					//AfxMessageBox(_T("Move XY Error..."));
//			}
//		}
//
//		return TRUE;
//	}
//
//	return FALSE;
//}

void CDlgMenu02::SetPinPos(int nCam, CfPoint ptPnt) 
{
	if(pView->m_pMotion)
		pView->m_pMotion->SetPinPos(nCam, ptPnt.x, ptPnt.y);
	if(pDoc->m_Master[0].m_pPcsRgn)
		pDoc->m_Master[0].m_pPcsRgn->SetPinPos(nCam, ptPnt);
	if(pDoc->m_pSpecLocal)
		pDoc->m_pSpecLocal->SavePinPos(nCam, ptPnt);

	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), ptPnt.x);
	pDoc->WorkingInfo.Motion.sPinPosX[nCam] = sData;
	
	sData.Format(_T("%.3f"), ptPnt.y);
	pDoc->WorkingInfo.Motion.sPinPosY[nCam] = sData;

	if(nCam==0)
	{
		::WritePrivateProfileString(_T("Motion"), _T("PIN_POS0_X"), pDoc->WorkingInfo.Motion.sPinPosX[nCam], sPath);
		::WritePrivateProfileString(_T("Motion"), _T("PIN_POS0_Y"), pDoc->WorkingInfo.Motion.sPinPosY[nCam], sPath);
	}
	else if(nCam==1)
	{
		::WritePrivateProfileString(_T("Motion"), _T("PIN_POS1_X"), pDoc->WorkingInfo.Motion.sPinPosX[nCam], sPath);
		::WritePrivateProfileString(_T("Motion"), _T("PIN_POS1_Y"), pDoc->WorkingInfo.Motion.sPinPosY[nCam], sPath);
	}
}

void CDlgMenu02::OnBtnPinSave() 
{
	// TODO: Add your control notification handler code here
	if(IDNO == pView->MsgBox(_T("Do you want to save Pin Position?"), 0, MB_YESNO))
	{
		return;
	}

	CfPoint ptPnt;
	ptPnt.x = pView->m_dEnc[AXIS_X0];
	ptPnt.y = pView->m_dEnc[AXIS_Y0];

	SetPinPos(0, ptPnt);

	//if(pView->m_nMkStAuto > MK_ST+11 && pView->m_nMkStAuto < MK_ST+29)
	//	pView->m_nMkStAuto = MK_ST+11;

// 	if(pDoc->m_pSpecLocal)
// 	{
// 		pDoc->SetMkPnt(CAM_LF);
//// 		if(myBtn[16].GetCheck())
//// 		{
//// 			if(m_pDlgUtil03)
//// 				m_pDlgUtil03->Disp(ROT_NONE);
//// // 				m_pDlgUtil03->Disp(ROT_CCW_90);
//// 		}
// 	}


	if (pView->m_pVision[0]->UploadPinImg())
		pDoc->m_bUploadPinImg = TRUE;
	else
		pView->MsgBox(_T("카메라 정렬이미지 저장에 실패하였습니다."), 0, MB_OK);

}


// void CDlgMenu02::PinSave() 
// {
// 	CfPoint ptPnt;
// 	ptPnt.x = pView->m_dEnc[AXIS_X];
// 	ptPnt.y = pView->m_dEnc[AXIS_Y];
// 
// 	SetPinPos(ptPnt);
// 
// 	if(pDoc->m_pSpecLocal)
// 	{
// //		pDoc->SetMkPnt(CfPoint(pDoc->m_pSpecLocal->m_dPcsOffsetX, pDoc->m_pSpecLocal->m_dPcsOffsetY));
// 	}
// }

void CDlgMenu02::OnBtnHomeMove() 
{
	// TODO: Add your control notification handler code here
	ResetLight();

	double pTgtPos[2];
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	double dCurrX = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);
	double dCurrY = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);

	//if(pView->ChkCollision(AXIS_X0, pTgtPos[0]))
	//{
	//	CfPoint ptPnt;
	//	ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
	//	ptPnt.y = pView->m_dEnc[AXIS_Y1];
	//	pView->Move1(ptPnt);
	//}

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt( ((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)) );
	if(fLen > 0.001)
	{
		if(pView->m_pMotion)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc))
			{
				if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}
	}
}


void CDlgMenu02::OnBtnHomeSave() 
{
	// TODO: Add your control notification handler code here
//	pView->ShiftMsgPos(0, -430);
//	if(IDNO == pView->DoMyMsgBox(_T("Do you want to save Marking Start Position?"), MB_YESNO))
	if(IDNO == pView->MsgBox(_T("Do you want to save Marking Start Position?"), 0, MB_YESNO))
	{
//		pView->ShiftMsgPos(0, 0);
		return;
	}
//	pView->ShiftMsgPos(0, 0);

	double dX, dY;
	dX = pView->m_dEnc[AXIS_X0];
	dY = pView->m_dEnc[AXIS_Y0];

	if(dX > 50.0)
	{
		pView->DispMsg(_T("설정범위 초과"),_T("저장 실패 : X축 50.0 이하만 설장됨 ..."),RGB_GREEN,2000,TRUE);
		return;
	}

	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dX);
	pDoc->WorkingInfo.Motion.sStPosX[0] = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_POSITION_X0"), sData, sPath);
	
	sData.Format(_T("%.3f"), dY);
	pDoc->WorkingInfo.Motion.sStPosY[0] = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_POSITION_Y0"), sData, sPath);
}

void CDlgMenu02::OnChkResPosSt() 
{
	// TODO: Add your control notification handler code here
#ifdef USE_VISION
	if(!pView->m_pVision[0])
		return;

	myBtn[16].SetCheck(TRUE);	//	IDC_CHK_RES_POS_ST
	myBtn[16].EnableWindow(FALSE);

	pView->m_pVision[0]->CalcCameraPixelSize();

	myBtn[16].EnableWindow(TRUE);
	myBtn[16].SetCheck(FALSE);	//	IDC_CHK_RES_POS_ST

	myStcData[1].SetText(pDoc->WorkingInfo.Vision[0].sResX);
	myStcData[2].SetText(pDoc->WorkingInfo.Vision[0].sResY);
#else
	return;
#endif
}


void CDlgMenu02::OnChkMkOffsetSt() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[18].GetCheck();
	if(bOn)
	{
		myBtn[18].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ST
		m_dStOffsetX = pView->m_dEnc[AXIS_X0];
		m_dStOffsetY = pView->m_dEnc[AXIS_Y0];

		SwMarking();

		CString str = _T("Find mark with jog button.");
#ifdef USE_VISION
		if(pView->m_pVision[0])
		{
			pView->m_pVision[0]->DrawText(str, 200, 160, RGB_RED);
			pView->m_pVision[0]->DrawCenterMark();
		}
#endif
		myBtn[18].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
		myBtn[19].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ED
	}
	else
		myBtn[19].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED
}


void CDlgMenu02::OnChkMkOffsetEd() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn0 = myBtn[19].GetCheck();
	BOOL bOn1 = myBtn[18].GetCheck();
	if(bOn0 && bOn1)
	{
		myBtn[19].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED
		myBtn[18].SetCheck(FALSE);

		if(!pView->m_pMotion)
		{
			myBtn[18].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
			return;
		}
		
		double dCurPosX, dCurPosY;
		dCurPosX = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
		dCurPosY = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);	

		double dMkOffsetX, dMkOffsetY;
		if(pDoc->WorkingInfo.Vision[0].sMkOffsetX.IsEmpty())
			dMkOffsetX = 0.0;
		else
			dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);

		if(pDoc->WorkingInfo.Vision[0].sMkOffsetY.IsEmpty())
			dMkOffsetY = 0.0;
		else
			dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);

		dMkOffsetX += m_dStOffsetX-dCurPosX;
		dMkOffsetY += m_dStOffsetY-dCurPosY;

		CString sData, sPath=PATH_WORKING_INFO;

		sData.Format(_T("%.3f"), dMkOffsetX);
		pDoc->WorkingInfo.Vision[0].sMkOffsetX = sData;
		::WritePrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_X"), sData, sPath);
		myStcData[3].SetText(sData);	// IDC_STC_CAM_MK_OFFSET_X

		sData.Format(_T("%.3f"), dMkOffsetY);
		pDoc->WorkingInfo.Vision[0].sMkOffsetY = sData;
		::WritePrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_Y"), sData, sPath);
		myStcData[4].SetText(sData);	// IDC_STC_CAM_MK_OFFSET_Y

#ifdef USE_VISION
		if(pView->m_pVision[0])
		{
			pView->m_pVision[0]->ClearOverlay();
			pView->m_pVision[0]->DrawCenterMark();
		}
#endif
		myBtn[19].SetCheck(FALSE);
	}
}


void CDlgMenu02::OnBtnAlignMove() 
{
	// TODO: Add your control notification handler code here
	MovePinPos();

	//if(m_nMoveAlign[0] == 0)
	//{
	//	MoveAlign0(0);
	//	m_nMoveAlign[0] = 1;
	//}
	//else
	//{
	//	MoveAlign0(1);
	//	m_nMoveAlign[0] = 0;
	//}
	
}

void CDlgMenu02::OnBtnGrab() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double dX, dY, dAgl, dScr;

	if (!pDoc->m_bUploadPinImg)
	{
		pView->MsgBox(_T("카메라 정렬이미지가 저장되지 않았습니다.\r\n핀위치를 저장하세요."), 0, MB_OK);
		return;
	}

	double dCurrX = pView->m_dEnc[AXIS_X0];
	double dCurrY = pView->m_dEnc[AXIS_Y0];

	double pPos[8];
	pPos[0] = pView->m_pMotion->m_dPinPosX[0];
	pPos[1] = pView->m_pMotion->m_dPinPosY[0];
	//pPos[0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0];
	//pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0];
	//pPos[2] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0];
	//pPos[3] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0];

	//if(pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
	//{
	//	pPos[4] = pDoc->m_Master[0].m_stAlignMk.X2 + pView->m_pMotion->m_dPinPosX[0];
	//	pPos[5] = pDoc->m_Master[0].m_stAlignMk.Y2 + pView->m_pMotion->m_dPinPosY[0];
	//	pPos[6] = pDoc->m_Master[0].m_stAlignMk.X3 + pView->m_pMotion->m_dPinPosX[0];
	//	pPos[7] = pDoc->m_Master[0].m_stAlignMk.Y3 + pView->m_pMotion->m_dPinPosY[0];
	//}

	int nPos=-1;
	if( (dCurrX > pPos[0]-0.1 && dCurrX < pPos[0]+0.1) && (dCurrY > pPos[1]-0.1 && dCurrY < pPos[1]+0.1) )
		nPos = 0;
	//else if( (dCurrX > pPos[2]-0.1 && dCurrX < pPos[2]+0.1) && (dCurrY > pPos[3]-0.1 && dCurrY < pPos[3]+0.1) )
	//	nPos = 1;
	//if (pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
	//{
	//	if ((dCurrX > pPos[4] - 0.1 && dCurrX < pPos[4] + 0.1) && (dCurrY > pPos[5] - 0.1 && dCurrY < pPos[5] + 0.1))
	//		nPos = 2;
	//	else if ((dCurrX > pPos[6] - 0.1 && dCurrX < pPos[6] + 0.1) && (dCurrY > pPos[7] - 0.1 && dCurrY < pPos[7] + 0.1))
	//		nPos = 3;
	//}
	if (nPos == -1)
	{
		AfxMessageBox(_T("Pin위치가 아닙니다."));
		return;
	}

#ifdef USE_VISION
	if(pView->m_pVision[0]->Grab(nPos))
	{
		GetPmRst0(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
		myStcData[5].SetText(str);

		str.Format(_T("%.1f"), dY);
		pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
		myStcData[6].SetText(str);

		str.Format(_T("%.1f"), dAgl);
		pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
		myStcData[7].SetText(str);

		str.Format(_T("%.1f"), dScr);
		pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
		myStcData[8].SetText(str);
	}
	else
	{
		myStcData[5].SetText(_T(""));
		myStcData[6].SetText(_T(""));
		myStcData[7].SetText(_T(""));
		myStcData[8].SetText(_T(""));
	}
#endif

	pView->OnePointAlign0(0);
}

BOOL CDlgMenu02::GetPmRst0(double &dX, double &dY, double &dAgl, double &dScr)
{
#ifdef USE_VISION
	if(!pView->m_pVision[0])
		return FALSE;

	dX = pView->m_pVision[0]->PtMtRst.dX;
	dY = pView->m_pVision[0]->PtMtRst.dY;
	dAgl = pView->m_pVision[0]->PtMtRst.dAngle;
	dScr = pView->m_pVision[0]->PtMtRst.dScore;
#endif
	return TRUE;
}

BOOL CDlgMenu02::GetPmRst1(double &dX, double &dY, double &dAgl, double &dScr)
{
#ifdef USE_VISION
	if(!pView->m_pVision[1])
		return FALSE;

	dX = pView->m_pVision[1]->PtMtRst.dX;
	dY = pView->m_pVision[1]->PtMtRst.dY;
	dAgl = pView->m_pVision[1]->PtMtRst.dAngle;
	dScr = pView->m_pVision[1]->PtMtRst.dScore;
#endif
	return TRUE;
}

BOOL CDlgMenu02::Grab(int nPos, BOOL bMove)
{
	// TODO: Add your control notification handler code here
	if(bMove)
	{
		MovePinPos();
		Sleep(100);
	}

	CString str = _T("");
#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
		DWORD dwSt = GetTickCount();
		if(pView->m_pVision[0]->Grab(nPos))
		{
			double dX, dY, dAgl, dScr;
			GetPmRst0(dX, dY, dAgl, dScr);
			str.Format(_T("%.1f"), dX);
			myStcData[5].SetText(str);
			str.Format(_T("%.1f"), dY);
			myStcData[6].SetText(str);
			str.Format(_T("%.1f"), dAgl);
			myStcData[7].SetText(str);
			str.Format(_T("%.1f"), dScr);
			myStcData[8].SetText(str);

			str.Format(_T("%d [mSec]"), GetTickCount()-dwSt);
// 			pView->DispStsBar(str, 0);
			return TRUE;
		}
		else
		{
			myStcData[5].SetText(_T(""));
			myStcData[6].SetText(_T(""));
			myStcData[7].SetText(_T(""));
			myStcData[8].SetText(_T(""));
		}
	}
#endif
// 	myStcData[7].SetText(str);
// 	myStcData[8].SetText(str);
// 	myStcData[9].SetText(str);
// 	myStcData[10].SetText(str);

	return FALSE;		
}


void CDlgMenu02::OnBtnAlign() 
{
	// TODO: Add your control notification handler code here
	//int nAlignNum = m_nBtnAlignCam0Pos;

	//if (m_nBtnAlignCam0Pos == 0)
	//{
	//	if(IDNO == pView->MsgBox(_T("Do you want to Two Point Align Test?"), 0, MB_YESNO))
	//		return;

	//	TwoPointAlign0(nAlignNum);
	//	m_nBtnAlignCam0Pos = 1;
	//}
	//else if (m_nBtnAlignCam0Pos == 1)
	//{
	//	TwoPointAlign0(nAlignNum);
	//	m_nBtnAlignCam0Pos = 0;
	//}
}


BOOL CDlgMenu02::OnePointAlign(CfPoint &ptPnt)
{
	double dRefPinX, dRefPinY; // Cam Pos.
	double dTgtPinX, dTgtPinY; // Grab Pos.
	double dResX, dResY, dResCam;
	CString str;

	m_dMkFdOffsetX[0][0] = 0.0;

#ifdef USE_VISION
	if(!pView->m_pMotion || !pView->m_pVision[0])
		return FALSE;

	if(pDoc->WorkingInfo.Vision[0].sResX.IsEmpty() || pDoc->WorkingInfo.Vision[0].sResY.IsEmpty())
		return FALSE;

	if(pView->m_pVision[0]->Grab(0))
	{
		double dX, dY, dAgl, dScr;
		GetPmRst0(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		myStcData[5].SetText(str);
		str.Format(_T("%.1f"), dY);
		myStcData[6].SetText(str);
		str.Format(_T("%.1f"), dAgl);
		myStcData[7].SetText(str);
		str.Format(_T("%.1f"), dScr);
		myStcData[8].SetText(str);
	}
	else
	{
		myStcData[5].SetText(_T(""));
		myStcData[6].SetText(_T(""));
		myStcData[7].SetText(_T(""));
		myStcData[8].SetText(_T(""));
		return FALSE;
	}

	int nCamSzX, nCamSzY;
	pView->m_pVision[0]->GetCameraSize(nCamSzX, nCamSzY);

	dResX = _tstof(pDoc->WorkingInfo.Vision[0].sResX);
	dResY = _tstof(pDoc->WorkingInfo.Vision[0].sResY);
	dResCam = _tstof(pDoc->WorkingInfo.Vision[0].sCamPxlRes) / 10000.0;

	dRefPinX = (double)pDoc->m_Master[0].m_pCellRgn->nCADPinPosPixX * dResCam;
	dRefPinY = (double)pDoc->m_Master[0].m_pCellRgn->nCADPinPosPixY * dResCam;

	double dX, dY, dAgl, dScr;
	GetPmRst0(dX, dY, dAgl, dScr);

	dTgtPinX = pView->m_pMotion->m_dPinPosX[0] + (dX - double(nCamSzX/2)) * dResX;
	dTgtPinY = pView->m_pMotion->m_dPinPosY[0] + (dY - double(nCamSzY/2)) * dResY;
	m_dMkFdOffsetX[0][0] = (double(nCamSzX / 2) - dX) * dResX;
	m_dMkFdOffsetY[0][0] = (double(nCamSzY / 2) - dY) * dResY;
	//dTgtPinY = pView->m_pMotion->m_dPinPosY + m_dMkFdOffsetX;

	ptPnt.x = dTgtPinX;
	ptPnt.y = dTgtPinY;

	//pView->m_Align[0].SetAlignData(dRefPinX, dRefPinY, dTgtPinX, dTgtPinY);
	//pDoc->m_pPcsRgn->SetMkPnt();
#endif
	return TRUE;
}

// TwoPointAlign==================================================================
//
//BOOL CDlgMenu02::TwoPointAlign(int nPos)
//{
//	BOOL bRtn[2];
//	bRtn[0] = TwoPointAlign0(nPos);
//
//	if(bRtn[0])
//		return TRUE;
//	return FALSE;
//}
//
//BOOL CDlgMenu02::TwoPointAlign0(int nPos)
//{
//#ifdef USE_VISION
//	if(!pView->m_pMotion || !pView->m_pVision[0])
//		return FALSE;
//#endif
//	if(pDoc->WorkingInfo.Vision[0].sResX.IsEmpty() || pDoc->WorkingInfo.Vision[0].sResY.IsEmpty())
//		return FALSE;
//
//	MoveAlign0(nPos);
//
//	return (DoAlign0(nPos, TRUE));
//}
//
//BOOL CDlgMenu02::DoAlign0(int nPos, BOOL bDraw)
//{
//	double dRefPinX, dRefPinY; // Cam Pos.
//	double dTgtPinX, dTgtPinY; // Grab Pos.
//	double dResX, dResY, dResCam;
//	double fLen, fVel, fAcc, fJerk;
//	CString str;
//	double pPos[2];
//	CfPoint ptPnt;
//	double dX, dY, dAgl, dScr;
//
//#ifdef USE_VISION
//	if(pView->m_pVision[0]->Grab(nPos, bDraw))
//	{
//		GetPmRst0(dX, dY, dAgl, dScr);
//		str.Format(_T("%.1f"), dX);
//		pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
//		myStcData[5].SetText(str);
//
//		str.Format(_T("%.1f"), dY);
//		pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
//		myStcData[6].SetText(str);
//
//		str.Format(_T("%.1f"), dAgl);
//		pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
//		myStcData[7].SetText(str);
//
//		str.Format(_T("%.1f"), dScr);
//		pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
//		myStcData[8].SetText(str);
//	}
//	else
//	{
//		myStcData[5].SetText(_T(""));
//		myStcData[6].SetText(_T(""));
//		myStcData[7].SetText(_T(""));
//		myStcData[8].SetText(_T(""));
//		return FALSE;
//	}
//
// 	int nIdsSzX, nIdsSzY;
// 	pView->m_pVision[0]->GetCameraSize(nIdsSzX, nIdsSzY);
//
// 	dResX = _tstof(pDoc->WorkingInfo.Vision[0].sResX);
// 	dResY = _tstof(pDoc->WorkingInfo.Vision[0].sResY);
//	dResCam = _tstof(pDoc->WorkingInfo.Vision[0].sCamPxlRes) / 10000.0;
//
// 	GetPmRst0(dX, dY, dAgl, dScr);
//
//	if(nPos == 0)
//	{
//		m_dMkFdOffsetX[0] = (double(nIdsSzX/2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
//		m_dMkFdOffsetY[0] = (double(nIdsSzY/2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
//
//		//pView->IoWrite("ML45066", (long)(-1.0 * m_dMkFdOffsetX[0] * 1000.0));	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
//		pView->m_pMpe->Write(_T("ML45066"), (long)(-1.0 * m_dMkFdOffsetX[0] * 1000.0));
//	}
//
//	if (nPos == 1)
//	{
//		double dMkFdOffsetX = (double(nIdsSzX/2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
//		double dMkFdOffsetY = (double(nIdsSzY/2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
//
//		// Cam의 원점 기준의 Marking 이미지 좌표.
//		double dRefAlignX0 = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0]; // PCB좌표
//		double dRefAlignY0 = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0]; // PCB좌표
//		double dRefAlignX1 = pDoc->m_Master[0].m_stAlignMk.X2 + pView->m_pMotion->m_dPinPosX[0]; // PCB좌표
//		double dRefAlignY1 = pDoc->m_Master[0].m_stAlignMk.Y2 + pView->m_pMotion->m_dPinPosY[0]; // PCB좌표
//
//		// PCB상의 원점 기준의 Marking 이미지 좌표.
//		double dTgtAlignX0 = (pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0]) - m_dMkFdOffsetX[0];
//		double dTgtAlignY0 = (pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0]) - m_dMkFdOffsetY[0];
//		double dTgtAlignX1 = (pDoc->m_Master[0].m_stAlignMk.X2 + pView->m_pMotion->m_dPinPosX[0]) - dMkFdOffsetX;
//		double dTgtAlignY1 = (pDoc->m_Master[0].m_stAlignMk.Y2 + pView->m_pMotion->m_dPinPosY[0]) - dMkFdOffsetY;
//		
//		int nNodeX=0, nNodeY=0;
//		if(pDoc->m_Master[0].m_pPcsRgn)
//		{
//			nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
//			nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
//		}
//
//		pView->m_Align[0].SetAlignData(dRefAlignX0, dRefAlignY0, dRefAlignX1, dRefAlignY1, dTgtAlignX0, dTgtAlignY0, dTgtAlignX1, dTgtAlignY1);
//
//		CfPoint ptRef, ptTgt;
//		int nCol, nRow, idx=0;
//		for(nCol=0; nCol<nNodeX; nCol++)
//		{
//			for(nRow=0; nRow<nNodeY; nRow++)
//			{
//				ptRef.x = pDoc->m_Master[0].m_stPcsMk[idx].X + pView->m_pMotion->m_dPinPosX[0];
//				ptRef.y = pDoc->m_Master[0].m_stPcsMk[idx].Y + pView->m_pMotion->m_dPinPosY[0];
//				pView->m_Align[0].LinearAlignment(ptRef, ptTgt); //.LinearAlignment(ptRef, ptTgt);
//				if(pDoc->m_Master[0].m_pPcsRgn)
//				{
//					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[0][idx].x = ptTgt.x;
//					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[0][idx].y = ptTgt.y;
//				}
//				idx++;
//			}
//		}
//
//	}
//#endif
//	return TRUE;
//}



void CDlgMenu02::SwMarking()
{
	//if(!pView->m_pMotion)
	//	return;

	//double dCurPosX = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
	//double dCurPosY = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);	
	//double dStPosX = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	//double dStPosY = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	//if(dCurPosX < (dStPosX+10.0) && dCurPosX > (dStPosX-10.0)
	//	&& dCurPosY < (dStPosY+10.0) && dCurPosY > (dStPosY-10.0))
	//	return;

	//SetMkCurPos(0);

	//double dMkOffsetX, dMkOffsetY;
	//if(pDoc->WorkingInfo.Vision[0].sMkOffsetX.IsEmpty())
	//	dMkOffsetX = 0.0;
	//else
	//	dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);

	//if(pDoc->WorkingInfo.Vision[0].sMkOffsetY.IsEmpty())
	//	dMkOffsetY = 0.0;
	//else
	//	dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);

	//double pPos[2];
	//pPos[0] = m_dCurPosX[0] + dMkOffsetX;
	//pPos[1] = m_dCurPosY[0] + dMkOffsetY;

	//if(pView->ChkCollision(AXIS_X0, pPos[0]))
	//{
	//	CfPoint ptPnt;
	//	ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
	//	ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
	//	pView->Move1(ptPnt);
	//}

	//double fLen, fVel, fAcc, fJerk;
	//fLen = sqrt( dMkOffsetX*dMkOffsetX + dMkOffsetY*dMkOffsetY );
	//if(fLen > 0.001)
	//{
	//	pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
	//	if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
	//	{
	//		if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
	//			AfxMessageBox(_T("Move XY Error..."));
	//	}
	//}

	//SetMkDlyOff(0);

}

//void CDlgMenu02::SwMarking2()
//{
//	if(!pView->m_pMotion)
//		return;
//
//	double dCurPosX = pView->m_dEnc[AXIS_X1]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
//	double dCurPosY = pView->m_dEnc[AXIS_Y1]; // pView->m_pMotion->GetActualPosition(AXIS_Y);	
//	double dStPosX = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
//	double dStPosY = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
//	if(dCurPosX < (dStPosX+10.0) && dCurPosX > (dStPosX-10.0)
//		&& dCurPosY < (dStPosY+10.0) && dCurPosY > (dStPosY-10.0))
//		return;
//
//	SetMkCurPos(1);
//
//	double dMkOffsetX, dMkOffsetY;
//	if(pDoc->WorkingInfo.Vision[1].sMkOffsetX.IsEmpty())
//		dMkOffsetX = 0.0;
//	else
//		dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX);
//
//	if(pDoc->WorkingInfo.Vision[1].sMkOffsetY.IsEmpty())
//		dMkOffsetY = 0.0;
//	else
//		dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY);
//
//	double pPos[2];
//	pPos[0] = m_dCurPosX[1] + dMkOffsetX;
//	pPos[1] = m_dCurPosY[1] + dMkOffsetY;
//
//	if(pView->ChkCollision(AXIS_X1, pPos[0]))
//	{
//		CfPoint ptPnt;
//		ptPnt.x = 0.0;
//		ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y0];
//		pView->Move0(ptPnt);
//	}
//
//	double fLen, fVel, fAcc, fJerk;
//	fLen = sqrt( dMkOffsetX*dMkOffsetX + dMkOffsetY*dMkOffsetY );
//	if(fLen > 0.001)
//	{
//		pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
//		if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
//		{
//			if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
//				AfxMessageBox(_T("Move XY Error..."));
//		}
//	}
//
//	SetMkDlyOff(1);
//
//}

void CDlgMenu02::SetMkCurPos(int nCam)
{
	if (nCam == 0)
	{
		m_dCurPosX[0] = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
		m_dCurPosY[0] = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);	
	}
	//else
	//{
	//	m_dCurPosX[1] = pView->m_dEnc[AXIS_X1]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
	//	m_dCurPosY[1] = pView->m_dEnc[AXIS_Y1]; // pView->m_pMotion->GetActualPosition(AXIS_Y);
	//}
	
}


void CDlgMenu02::SetMkDlyOff(int nCam, int mSec)
{
	if (nCam == 0)
		SetTimer(TIM_MARKING_OFF, WAIT_MARKING, NULL);
}

void CDlgMenu02::MarkingOff()
{
	if(!pView->m_pMotion)
		return;
	
	// Ink Marking OFF
// 	pDoc->m_pSliceIo[7] &= ~(0x01<<12);	// 마킹부 마킹 콘트롤러 출력 1(전원공급) -> Purge
// 	pDoc->m_pSliceIo[7] &= ~(0x01<<13);	// 마킹부 마킹 콘트롤러 출력 2(전원공급) -> Sen

	double dMkOffsetX, dMkOffsetY;
	if(pDoc->WorkingInfo.Vision[0].sMkOffsetX.IsEmpty())
		dMkOffsetX = 0.0;
	else
		dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);

	if(pDoc->WorkingInfo.Vision[0].sMkOffsetY.IsEmpty())
		dMkOffsetY = 0.0;
	else
		dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt( dMkOffsetX*dMkOffsetX + dMkOffsetY*dMkOffsetY );
	if(fLen > 0.001)
	{
		double pPos[2];
		pPos[0] = m_dCurPosX[0];
		pPos[1] = m_dCurPosY[0];

		//if(pView->ChkCollision(AXIS_X0, pPos[0]))
		//{
		//	CfPoint ptPnt;
		//	ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
		//	ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
		//	pView->Move1(ptPnt);
		//}

		pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
		{
			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
				pView->MsgBox(_T("Move XY Error..."));
				//AfxMessageBox(_T("Move XY Error..."));
		}
	}
}


//void CDlgMenu02::DispBufEnc()
//{
// 	CString str;
// 	double dBufEnc = (double)pDoc->m_pMpeData[0][1]	/ 1000.0	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
// 	str.Format(_T("%.3f"), dBufEnc);
//	myStcData[11].SetText(str);
//}

void CDlgMenu02::DispCenterMark()
{
#ifdef USE_VISION
	if (pView->m_pVision[0])
	{
		pView->m_pVision[0]->DrawCross();
	}

	if (pView->m_pVision[1])
	{
		pView->m_pVision[1]->DrawCross();
	}
#endif
}

void CDlgMenu02::DispAxisPos()
{
#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
#ifdef TEST_MODE
		pView->m_pVision[0]->DispAxisPos(TRUE);
#else
		pView->m_pVision[0]->DispAxisPos();
#endif
	}

	if(pView->m_pVision[1])
	{
#ifdef TEST_MODE
		pView->m_pVision[1]->DispAxisPos(TRUE);
#else
		pView->m_pVision[1]->DispAxisPos();
#endif
	}
#endif
}


// void CDlgMenu02::SetMkPos(CfPoint ptOfst)
// {
// 	if(!pView->m_pVision[0])
// 		return;
// 	int nSzCtrlX, nSzCtrlY, nSzImgX, nSzImgY;
// // 	m_pPcsGL->GetPixelInfo(nSzCtrlX, nSzCtrlY, nSzImgX, nSzImgY);
// 	pView->m_pVision[0]->GetPixelInfo(nSzCtrlX, nSzCtrlY, nSzImgX, nSzImgY);
// 	double dImgPixelRes = pDoc->m_Master[0].MasterInfo.dPixelSize / 1000.0; // [mm]
// // 	double dImgPixelRes = pDoc->MasterInfo.dPixelSize / 1000.0; // [mm]
// 	int nPixX = int(ptOfst.x/dImgPixelRes);
// 	int nPixY = int(ptOfst.y/dImgPixelRes);
// 	CfPoint ptPnt;
// 	ptPnt.x = nSzImgX/2 + nPixX;
// 	ptPnt.y = nSzImgY/2 + nPixY;
// // 	m_pPcsGL->SetMkPos(ptPnt);
// 	pView->m_pVision[0]->SetMkPos(ptPnt);
// }

// void CDlgMenu02::DispLocalSpec()
// {
// 	CString str;
// 	str.Format(_T("%.3f"), pDoc->m_pSpecLocal->m_dPcsOffsetX);
// 	myStcData[0].SetText(str);
// 	str.Format(_T("%.3f"), pDoc->m_pSpecLocal->m_dPcsOffsetY);
// 	myStcData[1].SetText(str);
// }

void CDlgMenu02::ChgModel()
{
// 	m_pPcsGL->LoadPcsImg(PATH_PCS_IMG);

#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
// 		pView->m_pVision[0]->ShowDispPcs(nLayer);
 		pView->m_pVision[0]->ShowDispPin(0);
//		pView->m_pVision[0]->ShowDispAlign();
	}

	if(pView->m_pVision[1])
	{
// 		pView->m_pVision[0]->ShowDispPcs(nLayer);
 		pView->m_pVision[1]->ShowDispPin(0);
//		pView->m_pVision[1]->ShowDispAlign();
	}
#endif
}

void CDlgMenu02::ChgModelUp()
{
#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
// 		pView->m_pVision[0]->ShowDispPcs(nLayer);
 		pView->m_pVision[0]->ShowDispPin(0);
//		pView->m_pVision[0]->ShowDispAlign();
	}
#endif
}

void CDlgMenu02::ChgModelDn()
{
#ifdef USE_VISION
	if(pView->m_pVision[1])
	{
// 		pView->m_pVision[0]->ShowDispPcs(nLayer);
 		pView->m_pVision[1]->ShowDispPin(0);
//		pView->m_pVision[1]->ShowDispAlign();
	}
#endif
}

BOOL CDlgMenu02::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef USE_VISION
	if(pView->m_pVision[0] && pView->m_pVision[1] && pView->m_pMotion)
	{
		CString sMsg;
		int nCamSzX, nCamSzY;
		pView->m_pVision[0]->GetCameraSize(nCamSzX, nCamSzY);

		double dResX, dResY;
		double dCenterX[2] = { 323, 963 };
		double dCenterY[2] = { 318, 318 };
		double pPos[2];

		if(pMsg->message == WM_LBUTTONDBLCLK && GetDlgItem(IDC_STC_VISION)->GetSafeHwnd() == pMsg->hwnd)
		{
			dResX = _tstof(pDoc->WorkingInfo.Vision[0].sResX);
			dResY = _tstof(pDoc->WorkingInfo.Vision[0].sResY);
			sMsg.Format(_T("%d,%d"), pMsg->pt.x, pMsg->pt.y);
			pFrm->DispStatusBar(sMsg, 6);

			double fLen, fVel, fAcc, fJerk;
			fLen = 1.0 * sqrt(((pMsg->pt.x - dCenterX[0]) * dResX * (pMsg->pt.x - dCenterX[0]) * dResX) + ((pMsg->pt.y - dCenterY[0]) * dResY * (pMsg->pt.y - dCenterY[0]) * dResY));
			if (fLen > 0.001)
			{
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);

				pPos[0] = pView->m_dEnc[AXIS_X0] + (1.0 * (pMsg->pt.x - dCenterX[0]) * dResX);
				pPos[1] = pView->m_dEnc[AXIS_Y0] + (1.0 * (pMsg->pt.y - dCenterY[0]) * dResY);
				if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
				{
					if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
						AfxMessageBox(_T("Move X0Y0 Error..."));
				}
			}
		}
	}
#endif
	if (pMsg->message != WM_KEYDOWN)
		return CDialog::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		case 'S':
		case 's':
			if(GetKeyState(VK_CONTROL) < 0) // Ctrl 키가 눌려진 상태
			{
				WINDOWPLACEMENT wndPlace;
				AfxGetMainWnd()->GetWindowPlacement(&wndPlace);
				wndPlace.showCmd |= SW_MAXIMIZE; 
				AfxGetMainWnd()->SetWindowPlacement(&wndPlace);	
			}
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgMenu02::ShowKeypad(int nCtlID, CPoint ptSt, int nDir)
{
	BOOL bAdj = TRUE;
	CString strData, strPrev;
	GetDlgItemText(nCtlID, strData);
	strPrev = strData;

	CString strMin, strMax;
	CRect rect(0,0,0,0);
	CDlgKeyNum *pDlg = new CDlgKeyNum(&strData, &rect, ptSt, nDir);
	pDlg->m_strCurr = strData;
	pDlg->m_strMin = strMin = pDoc->GetMin(IDD_DLG_MENU_01, nCtlID);
	pDlg->m_strMax = strMax = pDoc->GetMax(IDD_DLG_MENU_01, nCtlID);
	pDlg->DoModal();
	delete pDlg;

	if(!strMin.IsEmpty() && !strMax.IsEmpty())
	{
		if(_tstof(strData) < _tstof(strMin) || 
			_tstof(strData) > _tstof(strMax))
		{
			SetDlgItemText(nCtlID, strPrev);
			pView->DispMsg(_T("입력 범위를 벗어났습니다."), _T("주의"), RGB_YELLOW);
		}
		else
			SetDlgItemText(nCtlID, strData);
	}
	else
		SetDlgItemText(nCtlID, strData);
	
	return bAdj;
}

void CDlgMenu02::OnStcAlignStdScr() 
{
	// TODO: Add your control notification handler code here
	myStcData[9].SetBkColor(RGB_RED);
	myStcData[9].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_ALIGN_STD_SCR)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_ALIGN_STD_SCR, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[9].SetBkColor(RGB_WHITE);
	myStcData[9].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_ALIGN_STD_SCR)->GetWindowText(sData);
	pDoc->WorkingInfo.Vision[0].sStdScr = sData;
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_STD_SCORE"), sData, PATH_WORKING_INFO);
}


void CDlgMenu02::ShowDlg(int nID)
{
	switch(nID) 
	{
		;
	}
}

void CDlgMenu02::ResetMkTestBtn()
{
	m_lChk = 0;
	SetTimer(TIM_SHOW_MK_TEST, 30, NULL);
}



BOOL CDlgMenu02::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB_DLG_FRM);
	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}



void CDlgMenu02::Disp()
{
	CString str;
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveFdBarcodeOffset));
	myStcData[10].SetText(str);	// IDC_STC_5	각인기\rOffset

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sAoiFdBarcodeOffset));
	myStcData[26].SetText(str);					// IDC_STC_38	AOI\rOffset

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkFdBarcodeOffset));
	myStcData[27].SetText(str);					// IDC_STC_191	Punching\rOffset

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sFdBarcodeOffsetVel));
	myStcData[11].SetText(str);					// IDC_STC_007	속도

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sFdBarcodeOffsetAcc));
	myStcData[12].SetText(str);					// IDC_STC_010	가속도

	
	myStcData[17].SetText(pDoc->m_sOrderNum);	// IDC_STC_32	리더기 오더번호
	myStcData[18].SetText(pDoc->m_sShotNum);	// IDC_STC_34	리더기 Shot번호
	DispOneShotRemainLen();						// IDC_STC_36	1Shot - 리더기Offset

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveLaserAdjOffSetLimitAuto));
	myStcData[13].SetText(str);					// IDC_STC_12	자동보정

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveLaserAdjOffSetManual));
	myStcData[14].SetText(str);					// IDC_STC_40	수동보정

	myStcData[15].SetText(pDoc->m_sOrderNum);	// IDC_STC_17	각인기 오더번호
	myStcData[16].SetText(pDoc->m_sShotNum);	// IDC_STC_19	리더기 Shot번호

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveFdLead));
	myStcData[20].SetText(str);	// IDC_STC_45	Lead\rPitch

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveFdVacOff));
	myStcData[21].SetText(str);	// IDC_STC_180	Push Off

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveTq));
	myStcData[22].SetText(str);	// IDC_STC_184	토크

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist));
	myStcData[23].SetText(str);	// IDC_STC_218	각인기 ~ AOI 거리

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveLmtFdErr));
	myStcData[28].SetText(str);	// IDC_STC_49	한계값

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveLmtFdAdjOffSet));
	myStcData[29].SetText(str);	// IDC_STC_52	인정값

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveLmtFdOvrNum));
	myStcData[30].SetText(str);	// IDC_STC_55	인정횟수

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sEngraveStBufPos));
	myStcData[24].SetText(str);	// IDC_STC_225	초기위치
}

void CDlgMenu02::Input_myStcData(int nIdx, int nCtlId) //, CPoint ptSt,  int nDir)
{
	myStcData[nIdx].SetBkColor(RGB_RED);
	myStcData[nIdx].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(nCtlId)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(nCtlId, pt, TO_BOTTOM | TO_RIGHT);

	myStcData[nIdx].SetBkColor(RGB_WHITE);
	myStcData[nIdx].RedrawWindow();
}

void CDlgMenu02::OnStnClickedStc5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(10, IDC_STC_5);

	CString sPath = PATH_WORKING_INFO;
	CString sData;
	GetDlgItem(IDC_STC_5)->GetWindowText(sData);
	pDoc->SetEngraveReaderDist(_tstoi(sData));

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->Set2DEngLen();	//_ItemInx::_2DEngLen
#endif

	DispOneShotRemainLen();
}

void CDlgMenu02::DispOneShotRemainLen()
{
	CString sData;
	m_dOneShotRemainLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen) - _tstof(pDoc->WorkingInfo.Motion.sEngraveFdBarcodeOffset);
	sData.Format(_T("%.1f"), m_dOneShotRemainLen);
	GetDlgItem(IDC_STC_36)->SetWindowText(sData);
}

void CDlgMenu02::OnStnClickedStc007()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(11, IDC_STC_007);

	CString sData;
	GetDlgItem(IDC_STC_007)->GetWindowText(sData);
	pDoc->Set2DReaderPosMoveVel(_tstof(sData));
}

void CDlgMenu02::OnStnClickedStc010()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(12, IDC_STC_010);

	CString sData;
	GetDlgItem(IDC_STC_010)->GetWindowText(sData);
	pDoc->Set2DReaderPosMoveAcc(_tstof(sData));
}

void CDlgMenu02::OnStnClickedStc12()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(13, IDC_STC_12);	// \r각인기\r미세조정 한계값
}

void CDlgMenu02::OnStnClickedStc40()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(14, IDC_STC_40);	// 각인기 미세조정 Step값
}

void CDlgMenu02::OnStnClickedStc36()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//Input_myStcData(19, IDC_STC_36); // 1Shot - 리더기Offset
}

void CDlgMenu02::OnStnClickedStc45()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(20, IDC_STC_45);	// Lead\rPitch

	CString sData;
	GetDlgItem(IDC_STC_45)->GetWindowText(sData);
	pView->SetEngraveFdPitch(_tstof(sData));
}

void CDlgMenu02::OnStnClickedStc180()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(21, IDC_STC_180);	// Push Off

	CString sPath = PATH_WORKING_INFO;
	CString sData;
	GetDlgItem(IDC_STC_180)->GetWindowText(sData);
	pDoc->WorkingInfo.Motion.sEngraveFdVacOff = sData;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_VACUUM_OFF"), sData, sPath);
}

void CDlgMenu02::OnStnClickedStc184()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(22, IDC_STC_184);	// 토크

	CString sData;
	GetDlgItem(IDC_STC_184)->GetWindowText(sData);
	pDoc->SetMarkingToq(_tstoi(sData));

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetMkTqVal();	//_ItemInx::_MkTqVal
//#endif
}

void CDlgMenu02::OnStnClickedStc218()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(23, IDC_STC_218);

	CString sData;
	GetDlgItem(IDC_STC_218)->GetWindowText(sData);
	pDoc->SetEngraveAoiDist(_tstof(sData));

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetEngraveAoiDist();	//_stItemInx::_EngAoiLen
#endif
}

void CDlgMenu02::OnStnClickedStc225()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(24, IDC_STC_225);	// 버퍼롤러 초기위치

	CString sData;
	GetDlgItem(IDC_STC_225)->GetWindowText(sData);
	pDoc->SetEngraveBufInitPos(_tstof(sData));

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetEngBuffInitPos();	//_stItemInx::_EngBuffInitPos
#endif
}

void CDlgMenu02::DispBufEnc()
{
	//CString str;
	//double dBufEnc = (double)pDoc->m_pMpeData[1][1] / 1000.0;	// 각인부 버퍼 엔코더 값(단위 mm * 1000)
	//str.Format(_T("%.1f"), dBufEnc);
	//myStcData[25].SetText(str);			// IDC_STC_BUF_POS1
}


void CDlgMenu02::OnBnClickedBtnBuffInitSave1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!pView->m_pMotion)
		return;

	if (IDNO == pView->MsgBox(_T("Do you want to save Buffer Position?"), 0, MB_YESNO))
		return;

	//double dBufEnc = (double)pDoc->m_pMpeData[1][1] / 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
	//pView->SetBufInitPos(dBufEnc);	// ML45016	,	버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
}


void CDlgMenu02::OnBnClickedBtnBuffInitMove1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pView->DispMsg(_T("Moving"), _T("Searching Buffer Initial Position..."), RGB_GREEN, 2000, TRUE);
	if (pView->m_pMpe)
		pView->m_pMpe->Write(_T("MB44015A"), 1);	// 마킹부 버퍼 초기위치 이동(PC가 ON, PLC가 OFF)

	if (pView->m_pDlgMenu03)
		pView->m_pDlgMenu03->ChkBufInitDone();
}


void CDlgMenu02::OnStnClickedStc38()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(26, IDC_STC_38);

	CString sPath = PATH_WORKING_INFO;
	CString sData;
	GetDlgItem(IDC_STC_38)->GetWindowText(sData);
	pDoc->SetAoiReaderDist(_tstoi(sData));
}


void CDlgMenu02::OnStnClickedStc191()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(27, IDC_STC_191);

	CString sPath = PATH_WORKING_INFO;
	CString sData;
	GetDlgItem(IDC_STC_191)->GetWindowText(sData);
	pDoc->SetMkReaderDist(_tstoi(sData));
}


void CDlgMenu02::OnBnClickedBtn2dReading()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pView->m_pSr1000w->DoRead2DCode())
	{
		WaitResponse();
	}
	else
	{
		pView->MsgBox(_T("Error - DoRead2DCode()"));
		//AfxMessageBox(_T("Error - DoRead2DCode()"));
	}
}

BOOL CDlgMenu02::Disp2dCode()
{
	CString sData;
	if(pView->m_pSr1000w->Get2DCode(sData))
	{
		int nPos = sData.ReverseFind('-');
		if (nPos != -1)
		{
			pDoc->m_sOrderNum = sData.Left(nPos);
			pDoc->m_sShotNum = sData.Right(sData.GetLength() - nPos - 1);
			pDoc->m_nShotNum = _tstoi(pDoc->m_sShotNum);

			GetDlgItem(IDC_STC_32)->SetWindowText(pDoc->m_sOrderNum);
			GetDlgItem(IDC_STC_34)->SetWindowText(pDoc->m_sShotNum);
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

BOOL CDlgMenu02::DispTest2dCode()
{
	CString sData;
	if (pView->m_pSr1000w->Get2DCode(sData))
	{
		int nPos = sData.ReverseFind('-');
		if (nPos != -1)
		{
			pDoc->m_sTestOrderNum = sData.Left(nPos);
			pDoc->m_sTestShotNum = sData.Right(sData.GetLength() - nPos - 1);
			pDoc->m_nTestShotNum = _tstoi(pDoc->m_sTestShotNum);

			GetDlgItem(IDC_STC_32)->SetWindowText(pDoc->m_sTestOrderNum);
			GetDlgItem(IDC_STC_34)->SetWindowText(pDoc->m_sTestShotNum);
		}
		else
		{
			AfxMessageBox(sData);
		}

		return TRUE;
	}

	return FALSE;
}


void CDlgMenu02::OnStnClickedStc17()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(15, IDC_STC_17);

	// Order Number 설정

	CString sOrder, sShot, sData;
	GetDlgItem(IDC_STC_17)->GetWindowText(sOrder);
	GetDlgItem(IDC_STC_19)->GetWindowText(sShot);

	if (pView->m_pMdx2500->SetMdxOrderShotNum(sOrder, sShot, SET_ORDER))
	{
		//m_bTIM_MDX_RESPONSE = TRUE;
		//SetTimer(TIM_MDX_RESPONSE, 100, NULL);

		//if (pView->m_pMdx2500->GetMdxSettingData(sOrder, sShot))
		//{
		//	while (pView->m_pMdx2500->Is2DLaserRunning())
		//	{
		//		if (GetTickCount64() - time > 3000)
		//		{
		//			pView->m_pMdx2500->ResetWaitForResponse();
		//			AfxMessageBox(_T("Error-GetMdxSettingData"));
		//			return;
		//		}
		//		Sleep(100);
		//	}
		//	pView->m_pMdx2500->GetMdxResponse(sData);

		//	int nPos = sData.ReverseFind('-');
		//	if (nPos != -1)
		//	{
		//		CString sOrderNum = sData.Left(nPos);
		//		CString sShotNum = sData.Right(sData.GetLength() - nPos - 1);

		//		GetDlgItem(IDC_STC_17)->SetWindowText(sOrderNum);
		//		GetDlgItem(IDC_STC_19)->SetWindowText(sShotNum);
		//	}
		//	else
		//	{
		//		AfxMessageBox(sData);
		//	}
		//}
	}
}

void CDlgMenu02::OnStnClickedStc19()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(16, IDC_STC_19);

	// Shot Number 설정

	CString sOrder, sShot, sData;
	GetDlgItem(IDC_STC_17)->GetWindowText(sOrder);
	GetDlgItem(IDC_STC_19)->GetWindowText(sShot);

	if (pView->m_pMdx2500->SetMdxOrderShotNum(sOrder, sShot, SET_SHOT))
	{
		//m_bTIM_MDX_RESPONSE = TRUE;
		//SetTimer(TIM_MDX_RESPONSE, 100, NULL);

		//if (pView->m_pMdx2500->GetMdxSettingData(sOrder, sShot))
		//{
		//	while (pView->m_pMdx2500->Is2DLaserRunning())
		//	{
		//		if (GetTickCount64() - time > 3000)
		//		{
		//			pView->m_pMdx2500->ResetWaitForResponse();
		//			AfxMessageBox(_T("Error-GetMdxSettingData"));
		//			return;
		//		}
		//		Sleep(100);
		//	}
		//	pView->m_pMdx2500->GetMdxResponse(sData);

		//	int nPos = sData.ReverseFind('-');
		//	if (nPos != -1)
		//	{
		//		CString sOrderNum = sData.Left(nPos);
		//		CString sShotNum = sData.Right(sData.GetLength() - nPos - 1);

		//		GetDlgItem(IDC_STC_17)->SetWindowText(sOrderNum);
		//		GetDlgItem(IDC_STC_19)->SetWindowText(sShotNum);
		//	}
		//	else
		//	{
		//		AfxMessageBox(sData);
		//	}
		//}
	}
}

BOOL CDlgMenu02::ChkMdxResponse()
{
	CString sData;
	if (pView->m_pMdx2500->GetMdxResponse(sData))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CDlgMenu02::IsMdxReady()
{
	CString sData;
	if (!pView->m_pMdx2500->IsLaserReady())
	{
		pView->MsgBox(_T("Error - IsLaserReady()"));
		return FALSE;
	}
	return TRUE;
}

BOOL CDlgMenu02::ChkMdxReady()
{
	CString sData;

	if(pView->m_pMdx2500->IsRunning())
	{
		if (GetTickCount64() - m_stTime > 30000)
		{
			pView->m_pMdx2500->ResetWaitForResponse();
			//pView->MsgBox(_T("Error-Is2DLaserRunning"));
			//AfxMessageBox(_T("Error-Is2DLaserRunning"));
			return TRUE;
		}
	}
	pView->m_pMdx2500->GetMdxResponse(sData);

	int nPos = sData.Find(_T(MDX_RX_OK));
	if (nPos != -1)
	{
		return TRUE;
	}

	return FALSE;
}


void CDlgMenu02::OnBnClickedBtnLaserAlign()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = myBtn[24].GetCheck();	// IDC_BTN_LASER_ALIGN

	if(pView->m_pMdx2500->GuideLaserRect(bOn))
	{
		WaitResponse();

		if (bOn)
		{
			//m_bTIM_MDX_READY = TRUE;
			//SetTimer(TIM_MDX_READY, 100, NULL);
		}
		else
			m_bTIM_MDX_READY = FALSE;
	}
}

void CDlgMenu02::OnBnClickedBtnLaserMarking()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = myBtn[25].GetCheck();	// IDC_BTN_LASER_MARKING
	if (bOn)
	{
		CString sData;
		if(pView->m_pMdx2500->LaserMarking())
		{
			//m_bTIM_MDX_RESPONSE = TRUE;
			//SetTimer(TIM_MDX_RESPONSE, 100, NULL);
		}
		else
		{
			myBtn[25].SetCheck(FALSE);	// IDC_BTN_LASER_MARKING;
		}
	}
	else
	{
		if(pView->m_pMdx2500->IsRunning())
		{
			if (pView->m_pMdx2500->LaserMarking(FALSE))
			{
				//m_bTIM_MDX_RESPONSE = TRUE;
				//SetTimer(TIM_MDX_RESPONSE, 100, NULL);
			}
			else
			{
				myBtn[25].SetCheck(FALSE);	// IDC_BTN_LASER_MARKING;
			}
		}
		else
		{
			if (pView->m_pMdx2500->IsRunning())
			{
				if (pView->m_pMdx2500->LaserMarking(FALSE))
				{
					//m_bTIM_MDX_RESPONSE = TRUE;
					//SetTimer(TIM_MDX_RESPONSE, 100, NULL);
				}
				else
				{
					;
				}
			}
		}

	}
}


void CDlgMenu02::OnBnClickedBtnLaserAdjustZero()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = myBtn[24].GetCheck();	// IDC_BTN_LASER_ALIGN
	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(FALSE))
			WaitResponse();
	}

	CString sData;
	if (pView->m_pMdx2500->SetLaserPosZero())
	{
		WaitResponse();

		// X_org,Y_org,X_offset,Y_offset,Theta_offset
		pDoc->WorkingInfo.LastJob.sEngraveOrgX = _T("0.0");								// X_org
		pDoc->WorkingInfo.LastJob.sEngraveOrgY = _T("0.0");								// Y_org
		pDoc->WorkingInfo.LastJob.sEngravePosOffsetX = _T("0.0");						// X_offset
		pDoc->WorkingInfo.LastJob.sEngravePosOffsetY = _T("0.0");						// Y_offset
		pDoc->WorkingInfo.LastJob.sEngravePosTheta = _T("0.0");							// Theta_offset

		::WritePrivateProfileString(_T("Last Job"), _T("Engrave Offset X"), pDoc->WorkingInfo.LastJob.sEngravePosOffsetX, PATH_WORKING_INFO);
		::WritePrivateProfileString(_T("Last Job"), _T("Engrave Offset Y"), pDoc->WorkingInfo.LastJob.sEngravePosOffsetY, PATH_WORKING_INFO);
		::WritePrivateProfileString(_T("Last Job"), _T("Engrave Pos Theta"), pDoc->WorkingInfo.LastJob.sEngravePosTheta, PATH_WORKING_INFO);
	}

	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(TRUE))
			WaitResponse();
	}
}


void CDlgMenu02::OnBnClickedBtnLaserAdjustUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = myBtn[24].GetCheck();	// IDC_BTN_LASER_ALIGN
	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(FALSE))
			WaitResponse();
	}

	CString sData;
	GetDlgItem(IDC_STC_40)->GetWindowText(sData);

	double pData[5]; // X_org,Y_org,X_offset,Y_offset,Theta_offset
	pData[0] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgX);								// X_org
	pData[1] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgY);								// Y_org
	pData[2] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetX);						// X_offset
	pData[3] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetY) + _tstof(sData);		// Y_offset
	pData[4] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosTheta);							// Theta_offset

	if (pView->m_pMdx2500->SetLaserPos(pData))
	{
		WaitResponse();

		sData.Format(_T("%f"), pData[3]);
		pDoc->WorkingInfo.LastJob.sEngravePosOffsetY = sData;
		::WritePrivateProfileString(_T("Last Job"), _T("Engrave Offset Y"), sData, PATH_WORKING_INFO);
	}

	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(TRUE))
			WaitResponse();
	}
}


void CDlgMenu02::OnBnClickedBtnLaserAdjustRt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = myBtn[24].GetCheck();	// IDC_BTN_LASER_ALIGN
	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(FALSE))
			WaitResponse();
	}

	CString sData;
	GetDlgItem(IDC_STC_40)->GetWindowText(sData);

	double pData[5]; // X_org,Y_org,X_offset,Y_offset,Theta_offset
	pData[0] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgX);								// X_org
	pData[1] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgY);								// Y_org
	pData[2] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetX) + _tstof(sData);		// X_offset
	pData[3] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetY);						// Y_offset
	pData[4] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosTheta);							// Theta_offset

	if (pView->m_pMdx2500->SetLaserPos(pData))
	{
		WaitResponse();

		sData.Format(_T("%f"), pData[2]);
		pDoc->WorkingInfo.LastJob.sEngravePosOffsetX = sData;
		::WritePrivateProfileString(_T("Last Job"), _T("Engrave Offset X"), sData, PATH_WORKING_INFO);
	}

	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(TRUE))
			WaitResponse();
	}
}


void CDlgMenu02::OnBnClickedBtnLaserAdjustDn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = myBtn[24].GetCheck();	// IDC_BTN_LASER_ALIGN
	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(FALSE))
			WaitResponse();
	}

	CString sData;
	GetDlgItem(IDC_STC_40)->GetWindowText(sData);

	double pData[5]; // X_org,Y_org,X_offset,Y_offset,Theta_offset
	pData[0] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgX);								// X_org
	pData[1] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgY);								// Y_org
	pData[2] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetX);						// X_offset
	pData[3] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetY) - _tstof(sData);		// Y_offset
	pData[4] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosTheta);							// Theta_offset

	if (pView->m_pMdx2500->SetLaserPos(pData))
	{
		WaitResponse();

		sData.Format(_T("%f"), pData[3]);
		pDoc->WorkingInfo.LastJob.sEngravePosOffsetY = sData;
		::WritePrivateProfileString(_T("Last Job"), _T("Engrave Offset Y"), sData, PATH_WORKING_INFO);
	}

	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(TRUE))
			WaitResponse();
	}
}


void CDlgMenu02::OnBnClickedBtnLaserAdjustLf()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = myBtn[24].GetCheck();	// IDC_BTN_LASER_ALIGN
	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(FALSE))
			WaitResponse();
	}

	CString sData;
	GetDlgItem(IDC_STC_40)->GetWindowText(sData);

	double pData[5]; // X_org,Y_org,X_offset,Y_offset,Theta_offset
	pData[0] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgX);								// X_org
	pData[1] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgY);								// Y_org
	pData[2] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetX) - _tstof(sData);		// X_offset
	pData[3] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetY);						// Y_offset
	pData[4] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosTheta);							// Theta_offset

	if (pView->m_pMdx2500->SetLaserPos(pData))
	{
		WaitResponse();

		sData.Format(_T("%f"), pData[2]);
		pDoc->WorkingInfo.LastJob.sEngravePosOffsetX = sData;
		::WritePrivateProfileString(_T("Last Job"), _T("Engrave Offset X"), sData, PATH_WORKING_INFO);
	}

	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(TRUE))
			WaitResponse();
	}
}


void CDlgMenu02::OnBnClickedBtnLaserAdjustCw2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = myBtn[24].GetCheck();	// IDC_BTN_LASER_ALIGN
	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(FALSE))
			WaitResponse();
	}

	CString sData;
	GetDlgItem(IDC_STC_40)->GetWindowText(sData);

	double pData[5]; // X_org,Y_org,X_offset,Y_offset,Theta_offset
	pData[0] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgX);								// X_org
	pData[1] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgY);								// Y_org
	pData[2] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetX);						// X_offset
	pData[3] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetY);						// Y_offset
	pData[4] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosTheta) - _tstof(sData);			// Theta_offset

	if (pView->m_pMdx2500->SetLaserPos(pData))
	{
		WaitResponse();

		sData.Format(_T("%f"), pData[4]);
		pDoc->WorkingInfo.LastJob.sEngravePosTheta = sData;
		::WritePrivateProfileString(_T("Last Job"), _T("Engrave Pos Theta"), sData, PATH_WORKING_INFO);
	}

	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(TRUE))
			WaitResponse();
	}
}


void CDlgMenu02::OnBnClickedBtnLaserAdjustCcw2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = myBtn[24].GetCheck();	// IDC_BTN_LASER_ALIGN
	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(FALSE))
			WaitResponse();
	}

	CString sData;
	GetDlgItem(IDC_STC_40)->GetWindowText(sData);

	double pData[5]; // X_org,Y_org,X_offset,Y_offset,Theta_offset
	pData[0] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgX);								// X_org
	pData[1] = _tstof(pDoc->WorkingInfo.LastJob.sEngraveOrgY);								// Y_org
	pData[2] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetX);						// X_offset
	pData[3] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosOffsetY);						// Y_offset
	pData[4] = _tstof(pDoc->WorkingInfo.LastJob.sEngravePosTheta) + _tstof(sData);			// Theta_offset


	if (pView->m_pMdx2500->SetLaserPos(pData))
	{
		WaitResponse();

		sData.Format(_T("%f"), pData[4]);
		pDoc->WorkingInfo.LastJob.sEngravePosTheta = sData;
		::WritePrivateProfileString(_T("Last Job"), _T("Engrave Pos Theta"), sData, PATH_WORKING_INFO);
	}

	if (bOn)
	{
		if (pView->m_pMdx2500->GuideLaserRect(TRUE))
			WaitResponse();
	}
}


void CDlgMenu02::OnBnClickedBtnShotRemainCw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//BOOL bOn = (pDoc->m_pMpeSignal[7] & (0x01 << 15)) > 0 ? TRUE : FALSE;	// 각인부 Remain 피딩 CW ON (PLC가 피딩완료 후 OFF)
	//if (bOn)
	//{
	//	pView->MsgBox(_T("각인부 Remain 피딩 중입니다."));
	//	//AfxMessageBox(_T("각인부 Remain 피딩 중입니다."));
	//	return;
	//}

	//if (pView->m_pMpe)
	//	pView->m_pMpe->Write(_T("MB44018F"), 1);							// 각인부 Remain 피딩 CW ON (PLC가 피딩완료 후 OFF)
}


void CDlgMenu02::OnBnClickedBtnShotRemainCcw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//BOOL bOn = (pDoc->m_pMpeSignal[7] & (0x01 << 14)) > 0 ? TRUE : FALSE;	// 각인부 Remain 피딩 CCW ON (PLC가 피딩완료 후 OFF)
	//if (bOn)
	//{
	//	pView->MsgBox(_T("각인부 Remain 피딩 중입니다."));
	//	//AfxMessageBox(_T("각인부 Remain 피딩 중입니다."));
	//	return;
	//}

	//if (pView->m_pMpe)
	//	pView->m_pMpe->Write(_T("MB44018E"), 1);							// 각인부 Remain 피딩 CCW ON (PLC가 피딩완료 후 OFF)
}


void CDlgMenu02::OnBnClickedBtnOffsetEngraveCw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//BOOL bOn = (pDoc->m_pMpeSignal[7] & (0x01 << 13)) > 0 ? TRUE : FALSE;	// 각인부 2D Camera 피딩 CW ON (PLC가 피딩완료 후 OFF)
	//if (bOn)
	//{
	//	pView->MsgBox(_T("각인부 2D Camera 피딩 중입니다."));
	//	//AfxMessageBox(_T("각인부 2D Camera 피딩 중입니다."));
	//	return;
	//}

	//if (pView->m_pMpe)
	//	pView->m_pMpe->Write(_T("MB44018D"), 1);							// 각인부 2D Camera 피딩 CW ON (PLC가 피딩완료 후 OFF)
}


void CDlgMenu02::OnBnClickedBtnOffsetEngraveCcw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//BOOL bOn = (pDoc->m_pMpeSignal[7] & (0x01 << 12)) > 0 ? TRUE : FALSE;	// 각인부 2D Camera 피딩 CCW ON (PLC가 피딩완료 후 OFF)
	//if (bOn)
	//{
	//	pView->MsgBox(_T("각인부 2D Camera 피딩 중입니다."));
	//	//AfxMessageBox(_T("각인부 2D Camera 피딩 중입니다."));
	//	return;
	//}

	//if (pView->m_pMpe)
	//	pView->m_pMpe->Write(_T("MB44018C"), 1);							// 각인부 2D Camera 피딩 CCW ON (PLC가 피딩완료 후 OFF)
}


void CDlgMenu02::OnBnClickedBtnOffsetAoiCw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//BOOL bOn = (pDoc->m_pMpeSignal[7] & (0x01 << 11)) > 0 ? TRUE : FALSE;	// AOI 2D Camera 피딩 CW ON (PLC가 피딩완료 후 OFF)
	//if (bOn)
	//{
	//	pView->MsgBox(_T("AOI 2D Camera 피딩 중입니다."));
	//	//AfxMessageBox(_T("AOI 2D Camera 피딩 중입니다."));
	//	return;
	//}

	//if (pView->m_pMpe)
	//	pView->m_pMpe->Write(_T("MB44018B"), 1);							// AOI 2D Camera 피딩 CW ON (PLC가 피딩완료 후 OFF)
}


void CDlgMenu02::OnBnClickedBtnOffsetAoiCcw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//BOOL bOn = (pDoc->m_pMpeSignal[7] & (0x01 << 10)) > 0 ? TRUE : FALSE;	// AOI 2D Camera 피딩 CCW ON (PLC가 피딩완료 후 OFF)
	//if (bOn)
	//{
	//	pView->MsgBox(_T("AOI 2D Camera 피딩 중입니다."));
	//	//AfxMessageBox(_T("AOI 2D Camera 피딩 중입니다."));
	//	return;
	//}

	//if (pView->m_pMpe)
	//	pView->m_pMpe->Write(_T("MB44018A"), 1);							// AOI 2D Camera 피딩 CCW ON (PLC가 피딩완료 후 OFF)
}


void CDlgMenu02::OnBnClickedBtnOffsetMkCw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//BOOL bOn = (pDoc->m_pMpeSignal[7] & (0x01 << 9)) > 0 ? TRUE : FALSE;	// 마킹부 2D Camera 피딩 CW ON (PLC가 피딩완료 후 OFF)
	//if (bOn)
	//{
	//	pView->MsgBox(_T("마킹부 2D Camera 피딩 중입니다."));
	//	//AfxMessageBox(_T("마킹부 2D Camera 피딩 중입니다."));
	//	return;
	//}

	//if (pView->m_pMpe)
	//	pView->m_pMpe->Write(_T("MB440189"), 1);							// 마킹부 2D Camera 피딩 CW ON (PLC가 피딩완료 후 OFF)
}


void CDlgMenu02::OnBnClickedBtnOffsetMkCcw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//BOOL bOn = (pDoc->m_pMpeSignal[7] & (0x01 << 8)) > 0 ? TRUE : FALSE;	// 마킹부 2D Camera 피딩 CCW ON (PLC가 피딩완료 후 OFF)
	//if (bOn)
	//{
	//	pView->MsgBox(_T("마킹부 2D Camera 피딩 중입니다."));
	//	//AfxMessageBox(_T("마킹부 2D Camera 피딩 중입니다."));
	//	return;
	//}

	//if (pView->m_pMpe)
	//	pView->m_pMpe->Write(_T("MB440188"), 1);							// 마킹부 2D Camera 피딩 CCW ON (PLC가 피딩완료 후 OFF)
}


void CDlgMenu02::OnStnClickedStc49()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(28, IDC_STC_49);

	// 틀어짐 보정 한계값

	CString sData;
	GetDlgItem(IDC_STC_49)->GetWindowText(sData);
	pDoc->SetEngraveFdErrLmt(_tstof(sData));
}


void CDlgMenu02::OnStnClickedStc52()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(29, IDC_STC_52);

	// 틀어짐 보정 인정값

	CString sData;
	GetDlgItem(IDC_STC_52)->GetWindowText(sData);
	pDoc->SetEngraveFdErrRng(_tstof(sData));
}


void CDlgMenu02::OnStnClickedStc55()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Input_myStcData(30, IDC_STC_55);

	// 틀어짐 보정 인정횟수

	CString sData;
	GetDlgItem(IDC_STC_55)->GetWindowText(sData);
	pDoc->SetEngraveFdErrNum(_tstoi(sData));
}

void CDlgMenu02::WaitResponse()
{
	MSG message;

	while (pView->m_pMdx2500->IsRunning())
	{
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(10);
	}
}

void CDlgMenu02::UpdateData()
{
	Disp();
}
