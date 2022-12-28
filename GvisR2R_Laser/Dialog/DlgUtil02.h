#if !defined(AFX_DlgUtil02_H__E791A0C6_0497_4E02_A125_EE9430BFE182__INCLUDED_)
#define AFX_DlgUtil02_H__E791A0C6_0497_4E02_A125_EE9430BFE182__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUtil02.h : header file
//

// #include "Vision.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil02 dialog

class CDlgUtil02 : public CDialog
{

	void DoConfirm();

// Construction
public:
	CDlgUtil02(CWnd* pParent = NULL);   // standard constructor
	~CDlgUtil02();

// 	CVision* m_pVision;
	CRect* m_pRect;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

	void ModifyModelComboData(CComboBox *pComboBox);
	void ModifyLayerComboData(CComboBox *pComboBox, int nAoi);

	CString m_sItsCode;
	CString m_sLotNum;
	CString m_sProcessNum;
	CComboBox m_LayerComboDn;
	CComboBox m_LayerComboUp;
	//CComboBox m_ModelComboDn;
	CComboBox m_ModelComboUp;


// Dialog Data
	//{{AFX_DATA(CDlgUtil02)
	enum { IDD = IDD_DLG_UTIL_02 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUtil02)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUtil02)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnConfirm();
	afx_msg void OnSelchangeModelComboUp();
	afx_msg void OnSelchangeLayerComboUp();
	afx_msg void OnSelchangeModelComboDn();
	afx_msg void OnSelchangeLayerComboDn();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgUtil02_H__E791A0C6_0497_4E02_A125_EE9430BFE182__INCLUDED_)
