#pragma once

#include "../Device/TcpIpServer.h"

// CDlgServer 대화 상자입니다.

class CDlgServer : public CDialog
{
	CString	m_strPort;
	CTcpIpServer* m_pServer;

	void StartServer(CString sAddr, CString sPort);
	void StopServer();
	void DispClientStatus();

	DECLARE_DYNAMIC(CDlgServer)

public:
	CDlgServer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgServer();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT wmAcceptReceived(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnClear();
	CEdit m_editMsgList;
};
