#pragma once


// CDlgOption01 대화 상자입니다.

class CDlgOption01 : public CDialog
{
	//DECLARE_DYNAMIC(CDlgOption01)

	CRect* m_pRect;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

public:
	CDlgOption01(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgOption01();

// 대화 상자 데이터입니다.
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OPTION_01 };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnBnClickedCheck11();
};
