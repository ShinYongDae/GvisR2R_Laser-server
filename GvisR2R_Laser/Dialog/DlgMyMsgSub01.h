#if !defined(AFX_DLGMYMSGSUB01_H__E7D31633_6CB0_44F6_BFD3_93ECDEEDFDC5__INCLUDED_)
#define AFX_DLGMYMSGSUB01_H__E7D31633_6CB0_44F6_BFD3_93ECDEEDFDC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMyMsgSub01.h : header file
//
#include "../Process/ThreadTask.h"

#include "MyPic.h"
#include "MyBtn.h"
#include "MyEdit.h"
#include "DlgMyMsg.h"

#ifndef FONT_SIZE
	#define FONT_SIZE	16
#endif
#ifndef LINE_SPACE
	#define LINE_SPACE	5
#endif

#define TIM_MYMSG01			215
#define TIM_KILL_FOCUS01		216

/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsgSub01 dialog

class CDlgMyMsgSub01 : public CDlgMyMsg
{
	BOOL m_bLoadImg;
// 	HWND m_hParentWnd;
	CRect* m_pRect;
	CWnd* m_pParent;
	CMyPic myPic;
	CMyBtn myBtn00, myBtn01;

	
	CMyEdit myEdit00;

	BOOL m_bThreadAlive;
	CThreadTask m_ThreadTask;

	void AtDlgShow();
	void AtDlgHide();
	void InitPic();
	void InitEdit();
	void InitBtn();
	void LoadImg();
	void DelImg();
	void KillFocus(int nID);
//	BOOL CloseMsgDlg();

	void StartThread();
	void StopThread();
	static BOOL ThreadProc(LPVOID lpContext);

// Construction
public:
	CDlgMyMsgSub01(CWnd* pParent=NULL, int nIDD=IDD);   // standard constructor
	~CDlgMyMsgSub01();

	HWND m_hParentWnd;

	BOOL Create();
	afx_msg LRESULT OnMyBtnDown(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyBtnUp(WPARAM wPara, LPARAM lPara);
	void SetHwnd(HWND hParentWnd);
	afx_msg LRESULT OnMyMsg01(WPARAM wPara, LPARAM lPara);
	BOOL CloseMsgDlg();

	void ClickYes();
	void ClickNo();

// Dialog Data
	//{{AFX_DATA(CDlgMyMsgSub01)
	enum { IDD = IDD_DLG_MY_MSG_SUB01 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMyMsgSub01)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMyMsgSub01)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMYMSGSUB01_H__E7D31633_6CB0_44F6_BFD3_93ECDEEDFDC5__INCLUDED_)
