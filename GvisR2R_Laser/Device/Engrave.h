#pragma once

#include "afxwin.h"
#include "TcpIpServer.h"
#include "EngraveDef.h"

#define TIM_CONNECT	1

class CEngrave : public CWnd
{
	HWND m_hParentWnd;
	CTcpIpServer* m_pServer;

	CString	m_strPortSvr, m_strAddrSvr;
	CString	m_strAddrCli;
	int m_nServerID;

	BOOL m_bWaitForResponse;
	CString m_strResponse;
	int m_nCmd;
	BOOL m_bCmdF;

	SOCKET_DATA m_SocketData;
	BOOL m_bAutoConnect;

	void StartServer(CString sAddr, CString sPort);
	void StopServer();
	BOOL CheckResponse(int nCmd, CString sResponse);
	BOOL WaitResponse();

public:
	CEngrave(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent = NULL);
	~CEngrave();

	void SetHwnd(HWND hParentWnd);
	BOOL IsRunning();
	int WriteComm(CString sMsg, DWORD dwTimeout = INFINITE);
	BOOL ReadComm(CString &sData);
	SOCKET_DATA GetSocketData();
	BOOL IsConnected();

	// Thread
	CEvent      m_evtThread;
	CWinThread *m_pThread;
	HANDLE		m_hThread;
	BOOL		m_bModify;
	BOOL		m_bAlive;

	void StartThread();
	void StopThread();
	void WaitUntilThreadEnd(HANDLE hThread);
	static UINT RunThread(LPVOID pParam);

	int OnServerReceived(WPARAM wParam, LPARAM lParam);
	void Close();

	// General Function
	BOOL SendCommand(int nAcceptId, SOCKET_DATA SocketData);

protected:
	afx_msg LRESULT wmAcceptReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT wmServerClosed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateConnection(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

