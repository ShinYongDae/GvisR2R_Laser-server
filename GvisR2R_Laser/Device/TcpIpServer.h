#pragma once

#include "CEasyThread.h"
#include "TcpIpAccept.h"
#include "SocketComm.h"

#define MAX_CLIENT	1

class CTcpIpServer : public CSocketComm//, public CEasyThread
{
	//DECLARE_DYNCREATE(CTcpIpServer)

	HWND m_hParentWnd;
	CWnd* m_pParent;

	CTcpIpAccept* m_pAcceptAddr[MAX_CLIENT]; // Number of Max connection is 10...
	int m_nConnectedID;
	int m_nNetworkPort;
	CString m_strServerIP;

	BOOL AddrToString(LPTSTR strAddress, struct sockaddr_in addr);
	int CharToAddr(struct in_addr *addr, const char *str);

public:
	CTcpIpServer(CWnd* pParent = NULL);
	virtual ~CTcpIpServer();

	CRITICAL_SECTION m_sc;
	BOOL m_bStop;

	void Init(CString strServerIP, int nPort);
	BOOL Start();
	BOOL Stop();
	void wmAcceptReceived(WPARAM wParam, LPARAM lParam);
	void wmAcceptClosed(WPARAM wParam, LPARAM lParam);

	// General Function
	BOOL IsConnected(int nClientID);
	int WriteComm(int nClientID, CString sMsg);
	BOOL IsConnected(CString addrClient, int& nClientId);
	BOOL WriteComm(CString addrClient, CString sMsg);
	SOCKET_DATA GetSocketData();
	virtual int Running();

	DWORD WriteSocketData(int nClientID, SOCKET_DATA SocketData, DWORD dwTimeout, DWORD dwStrLen = -1);

	// Thread
	CEvent      m_evtThread;
	CWinThread *m_pThread;
	HANDLE		m_hThread;
	BOOL		m_bModify;
	BOOL		m_bAlive;

	void StopThread();
	void WaitUntilThreadEnd(HANDLE hThread);

	int GetConnectedId();

// Generated message map functions
protected:
	static UINT WINAPI SocketThreadProc(LPVOID pParam);

	//{{AFX_MSG(CTcpIpServer)

	//DECLARE_MESSAGE_MAP()
};


extern CTcpIpServer g_sr1000Data;