// DlgUtil02.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Laser.h"
#include "DlgUtil02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_LaserDoc.h"
#include "../GvisR2R_LaserView.h"

extern CGvisR2R_LaserDoc* pDoc;
extern CGvisR2R_LaserView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil02 dialog


CDlgUtil02::CDlgUtil02(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUtil02::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUtil02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_sItsCode = _T("");
	m_sLotNum = _T("");
	m_sProcessNum = _T("");
}

CDlgUtil02::~CDlgUtil02()
{
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgUtil02::Create()
{
	return CDialog::Create(CDlgUtil02::IDD);
}

void CDlgUtil02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUtil02)
	DDX_Text(pDX, IDC_ITS_EDIT, m_sItsCode);
	DDX_Text(pDX, IDC_LOT_EDIT, m_sLotNum);
	DDX_Text(pDX, IDC_EDIT_PROCESS_NUM, m_sProcessNum);
	DDX_Control(pDX, IDC_LAYER_COMBO_DN, m_LayerComboDn);
	DDX_Control(pDX, IDC_LAYER_COMBO_UP, m_LayerComboUp);
	//DDX_Control(pDX, IDC_MODEL_COMBO_DN, m_ModelComboDn);
	DDX_Control(pDX, IDC_MODEL_COMBO_UP, m_ModelComboUp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUtil02, CDialog)
	//{{AFX_MSG_MAP(CDlgUtil02)
	ON_BN_CLICKED(IDC_BTN_CONFIRM, &CDlgUtil02::OnBnClickedBtnConfirm)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_MODEL_COMBO_UP, OnSelchangeModelComboUp)
	ON_CBN_SELCHANGE(IDC_LAYER_COMBO_UP, OnSelchangeLayerComboUp)
	//ON_CBN_SELCHANGE(IDC_MODEL_COMBO_DN, OnSelchangeModelComboDn)
	ON_CBN_SELCHANGE(IDC_LAYER_COMBO_DN, OnSelchangeLayerComboDn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil02 message handlers

void CDlgUtil02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		//m_pRect = new CRect;
		//
		//this->GetClientRect(m_pRect);
		//m_pRect->top = 375;
		//m_pRect->bottom += 375 + GetSystemMetrics(SM_CYSIZE);
		//m_pRect->left = 3;
		//m_pRect->right += 3;
		//this->MoveWindow(m_pRect, TRUE);
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

void CDlgUtil02::AtDlgShow()
{
	ModifyModelComboData(&m_ModelComboUp);
	//ModifyModelComboData(&m_ModelComboDn);

	m_sItsCode = pDoc->m_sItsCode;
	m_sLotNum = pDoc->m_sLotNum;
	m_sProcessNum = pDoc->m_sProcessNum;

	pDoc->m_sModelUp = pDoc->WorkingInfo.LastJob.sModelUp;
	//pDoc->m_sModelDn = pDoc->WorkingInfo.LastJob.sModelDn;
	pDoc->m_sLayerUp = pDoc->WorkingInfo.LastJob.sLayerUp;
	pDoc->m_sLayerDn = pDoc->WorkingInfo.LastJob.sLayerDn;

	m_ModelComboUp.SetWindowText(pDoc->WorkingInfo.LastJob.sModelUp);
	m_LayerComboUp.SetWindowText(pDoc->WorkingInfo.LastJob.sLayerUp);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		//m_ModelComboDn.SetWindowText(pDoc->WorkingInfo.LastJob.sModelDn);
		m_LayerComboDn.SetWindowText(pDoc->WorkingInfo.LastJob.sLayerDn);
	}
	else
	{
		//m_ModelComboDn.SetWindowText(_T(""));
		m_LayerComboDn.SetWindowText(_T(""));
	}

	UpdateData(FALSE);
}

void CDlgUtil02::AtDlgHide()
{

}

BOOL CDlgUtil02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	GetDlgItem(IDC_STATIC_MODEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MODEL_COMBO_DN)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgUtil02::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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

void CDlgUtil02::OnBnClickedBtnConfirm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DoConfirm();

	OnOK();
}

void CDlgUtil02::DoConfirm()
{
	UpdateData(TRUE);
	pDoc->m_sItsCode = m_sItsCode;

	m_sLotNum.MakeUpper();
	pDoc->m_sLotNum = m_sLotNum;
	pDoc->m_sProcessNum = m_sProcessNum;


/*
	pDoc->m_sLotNum.Delete(0, 1);

	//int nStrLength, nLength;
	//nStrLength = pDoc->m_sProcessNum.Find(_T("VS"), 0);

	//if (nStrLength == -1)
	//	nStrLength = pDoc->m_sProcessNum.Find(_T("vs"), 0);

	//if (nStrLength == -1)
	//	pDoc->m_sProcessNum = _T("VS90");
	//else
	//{
	//	pDoc->m_sProcessNum.Delete(0, nStrLength);
	//	nLength = pDoc->m_sProcessNum.GetLength();
	//	pDoc->m_sProcessNum.Delete(4, nLength - 4);
	//}
*/
	pView->m_bLoadMstInfo = FALSE;

	if ((pDoc->WorkingInfo.LastJob.sModelUp != pDoc->m_sModelUp) && !pDoc->m_sModelUp.IsEmpty())
	{
		pDoc->WorkingInfo.LastJob.sModelUp = pDoc->m_sModelUp;
		//pView->m_bLoadMstInfo = TRUE;
	}
	//if ((pDoc->WorkingInfo.LastJob.sModelDn != pDoc->m_sModelDn) && !pDoc->m_sModelDn.IsEmpty())
	//{
	//	pDoc->WorkingInfo.LastJob.sModelDn = pDoc->m_sModelDn;
	//	pView->m_bLoadMstInfo = TRUE;
	//}
	if ((pDoc->WorkingInfo.LastJob.sLayerUp != pDoc->m_sLayerUp) && !pDoc->m_sLayerUp.IsEmpty())
	{
		pDoc->WorkingInfo.LastJob.sLayerUp = pDoc->m_sLayerUp;
		pDoc->m_bLoadMstInfo[0] = TRUE;
		pView->m_bLoadMstInfo = TRUE;
	}
	if ((pDoc->WorkingInfo.LastJob.sLayerDn != pDoc->m_sLayerDn) && !pDoc->m_sLayerDn.IsEmpty())
	{
		pDoc->WorkingInfo.LastJob.sLayerDn = pDoc->m_sLayerDn;
		pDoc->m_bLoadMstInfo[1] = TRUE;
		pView->m_bLoadMstInfo = TRUE;
	}
	if ((pDoc->WorkingInfo.LastJob.sLotUp != pDoc->m_sLotNum) && !pDoc->m_sLayerUp.IsEmpty())
	{
		pDoc->WorkingInfo.LastJob.sLotUp = pDoc->m_sLotNum;
		//pView->m_bLoadMstInfo = TRUE;
	}
	if ((pDoc->WorkingInfo.LastJob.sLotDn != pDoc->m_sLotNum) && !pDoc->m_sLayerDn.IsEmpty())
	{
		pDoc->WorkingInfo.LastJob.sLotDn = pDoc->m_sLotNum;
		//pView->m_bLoadMstInfo = TRUE;
	}

	//pDoc->SetModelInfoUp();
	//pDoc->SetModelInfoDn();
	//pDoc->SetModelInfoProcessNum();
	pDoc->SetCurrentInfo();
}

void CDlgUtil02::ModifyModelComboData(CComboBox *pComboBox)
{
	TCHAR FN[100];
	int i, nCount, nIndex;
	CString strBuf;

	pComboBox->ResetContent();

	_stprintf(FN, _T("%s*.*"), pDoc->WorkingInfo.System.sPathCamSpecDir);

	pComboBox->Dir(0x8010, FN);
	pComboBox->DeleteString(0);
	nIndex = pComboBox->FindStringExact(-1, _T("[..]"));
	pComboBox->DeleteString(nIndex);
	nCount = pComboBox->GetCount();

	int t = 0;
	for (i = 0; i < nCount; i++)
	{
		pComboBox->GetLBText(i, strBuf);

		if (strBuf.GetLength() < 3)
			continue;

		CString strBuf2 = strBuf.Mid(1, strBuf.GetLength() - 2);

		pComboBox->InsertString(t, strBuf2);
		pComboBox->DeleteString(t + 1);
		t++;
	}

	nCount = pComboBox->GetCount();

	for (i = 0; i < nCount; i++)
	{
		pComboBox->GetLBText(i, strBuf);

		if (strBuf == _T("LightPreset") || strBuf == _T("SpecPreset"))
		{
			pComboBox->DeleteString(i);
			nCount = pComboBox->GetCount();
			continue;
		}
	}
	nCount = pComboBox->GetCount();
}

void CDlgUtil02::ModifyLayerComboData(CComboBox *pComboBox, int nAoi)
{
	int i, t, nCount;
	CFileFind	filefind;
	BOOL		bContinue;
	CString		Dir, strFileName;
	CString		strTmp;

	pComboBox->ResetContent();

	//if (nAoi == 0)
	{
		Dir = pDoc->WorkingInfo.System.sPathCamSpecDir + pDoc->m_sModelUp + _T("\\*.ini");
	}
	//else
	//{
	//	Dir = pDoc->WorkingInfo.System.sPathCamSpecDir + pDoc->m_sModelDn + _T("\\*.ini");
	//}

	if (!(bContinue = filefind.FindFile(Dir)))
		return;
	while (bContinue)
	{
		bContinue = filefind.FindNextFile();
		strFileName = filefind.GetFileName();
		if (strFileName != _T(".") && strFileName != _T(".."))
		{
			// Check Subgroup spec		// 110404 jsy
			strTmp = strFileName.Left(strFileName.GetLength() - 5);
			if (strTmp.Right(1) != _T('_'))
			{
				pComboBox->AddString(strFileName);
			}
		}
	}
	filefind.Close();

	// ini
	CString strBuf, strBuf2;
	nCount = pComboBox->GetCount();
	for (i = t = 0; i < nCount; i++)
	{
		pComboBox->GetLBText(i, strBuf);
		CString strBuf2 = strBuf.Left(strBuf.GetLength() - 4);

		pComboBox->InsertString(t, strBuf2);
		pComboBox->DeleteString(t + 1);
		t++;
	}
}


void CDlgUtil02::OnSelchangeModelComboUp()
{
	int nIndex;
	CString strMsg, strTitleMsg;

	UpdateData(TRUE);

	nIndex = m_ModelComboUp.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	m_ModelComboUp.GetLBText(nIndex, pDoc->m_sModelUp);
	ModifyLayerComboData(&m_LayerComboUp, 0); // [0]:AOI-Up , [1]:AOI-Dn
	ModifyLayerComboData(&m_LayerComboDn, 1); // [0]:AOI-Up , [1]:AOI-Dn
}

void CDlgUtil02::OnSelchangeLayerComboUp()
{
	int nIndex;

	UpdateData(TRUE);

	nIndex = m_LayerComboUp.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	m_LayerComboUp.GetLBText(nIndex, pDoc->m_sLayerUp);

	//nIndex = m_LayerComboUp.FindStringExact(-1, pDoc->m_sLayerUp);
	//if (nIndex != LB_ERR)
	//{
	//	m_LayerComboUp.SetCurSel(nIndex);
	//}

	//UpdateData(FALSE);
}

//void CDlgUtil02::OnSelchangeModelComboDn()
//{
//	int nIndex;
//	CString strMsg, strTitleMsg;
//
//	UpdateData(TRUE);
//
//	nIndex = m_ModelComboDn.GetCurSel();
//	if (nIndex == LB_ERR)
//		return;
//
//	m_ModelComboDn.GetLBText(nIndex, pDoc->m_sModelDn);
//
//	ModifyLayerComboData(&m_LayerComboDn, 1); // [0]:AOI-Up , [1]:AOI-Dn
//}

void CDlgUtil02::OnSelchangeLayerComboDn()
{
	int nIndex;

	UpdateData(TRUE);

	nIndex = m_LayerComboDn.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	m_LayerComboDn.GetLBText(nIndex, pDoc->m_sLayerDn);

	//nIndex = m_LayerComboDn.FindStringExact(-1, pDoc->m_sLayerDn);
	//if (nIndex != LB_ERR)
	//{
	//	m_LayerComboDn.SetCurSel(nIndex);
	//	m_LayerComboDn.GetLBText(nIndex, pDoc->m_sLayerDn);
	//}

	//UpdateData(FALSE);
}

