#pragma once

#include "../Device/TcpIpServer.h"

// CDlgServer ��ȭ �����Դϴ�.

class CDlgServer : public CDialog
{
	CString	m_strPort;
	CTcpIpServer* m_pServer;

	void StartServer(CString sAddr, CString sPort);
	void StopServer();
	void DispClientStatus();

	DECLARE_DYNAMIC(CDlgServer)

public:
	CDlgServer(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgServer();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
