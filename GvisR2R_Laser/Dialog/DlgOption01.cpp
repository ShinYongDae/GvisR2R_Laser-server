// DlgOption01.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "../GvisR2R_Laser.h"
#include "DlgOption01.h"
//#include "afxdialogex.h"


#include "../GvisR2R_LaserDoc.h"
#include "../GvisR2R_LaserView.h"

extern CGvisR2R_LaserDoc* pDoc;
extern CGvisR2R_LaserView* pView;

extern CString PATH_WORKING_INFO;

// CDlgOption01 ��ȭ �����Դϴ�.

//MPLEMENT_DYNAMIC(CDlgOption01, CDialog)

CDlgOption01::CDlgOption01(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_OPTION_01, pParent)
{
	m_pRect = NULL;
}

CDlgOption01::~CDlgOption01()
{
	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgOption01::Create()
{
	return CDialog::Create(CDlgOption01::IDD);
}

void CDlgOption01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgOption01, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgOption01::OnBnClickedCheck1)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgOption01 �޽��� ó�����Դϴ�.

void CDlgOption01::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!m_pRect)
	{
		m_pRect = new CRect;

		this->GetClientRect(m_pRect);
		m_pRect->top = 375;
		m_pRect->bottom += 375 + GetSystemMetrics(SM_CYSIZE);
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if (bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgOption01::AtDlgShow()
{
	CString sData;

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(pDoc->m_bUseAdjustLaser);
	sData.Format(_T("%.3f"), pDoc->m_dShiftAdjustRatio);
	GetDlgItem(IDC_EDIT_11)->SetWindowText(sData);
}

void CDlgOption01::AtDlgHide()
{

}

void CDlgOption01::OnBnClickedCheck1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	pDoc->m_bUseAdjustLaser = bOn;

	CString sPath = PATH_WORKING_INFO;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("USE_ADJUST_LASER"), _T("1"), sPath);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("USE_ADJUST_LASER"), _T("0"), sPath);
	}
}




void CDlgOption01::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CString sData, sPath = PATH_WORKING_INFO;

	GetDlgItem(IDC_EDIT_11)->GetWindowText(sData);
	pDoc->m_dShiftAdjustRatio = (double)_ttof(sData);
	::WritePrivateProfileString(_T("System"), _T("RTR_SHIFT_ADJUST_RATIO"), sData, sPath);

	CDialog::OnClose();
}
