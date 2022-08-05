
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

class CMainFrame : public CFrameWnd
{
	int m_ProgressID;
	BOOL m_bProgressCreated;
	CProgressCtrl m_Progress;

	BOOL m_bLockDispStsBar;
	CString m_sDispMsg[10];

protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:
	CCriticalSection m_cs;
	CStringArray m_strStatusMsgArray;

// �۾��Դϴ�.
public:
	void DispStatusBar(CString strMsg, int nStatusBarID);
	void CreateProgressBar();
	void IncProgress(int nVal = 0);

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CStatusBar        m_wndStatusBar;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnAppMinimize();
	afx_msg void OnDatabaseConnection();
};


