#pragma once

#include "CEasyThread.h"
#include "SocketComm.h"

extern class CTcpIpServer;

class CTcpIpAccept : public CSocketComm //, public CEasyThread
{
	HWND m_hParentWnd;
	CTcpIpServer* m_pParent;

	CString m_strServerIP;
	sockaddr m_ClientIP;
	sockaddr_in m_ClientIP_IN;
	int m_nClientID;
	BYTE* m_pReceiveBuffer;
	int m_nBufferId;
	CString m_strReceived;
	BOOL m_bClose;

	int ReadComm();
	int ReadCommData(SOCKET_DATA &SocketData, DWORD dwSize, DWORD dwTimeout);
	int ReadPacketData(SOCKET_DATA &SocketData, DWORD dwSize, DWORD dwTimeout);

public:
	CTcpIpAccept(CTcpIpServer* pParent = NULL);
	virtual ~CTcpIpAccept();

	CRITICAL_SECTION m_sc;
	BOOL m_bStop;
	//CString m_sAcceptIp;

	void Init(SOCKET sock, sockaddr ClientIP, int nClientID);
	void Init(SOCKET sock, sockaddr_in ClientIP, int nClientID);
	BOOL Start();
	BOOL Stop();
	sockaddr GetIP();
	sockaddr_in GetIP_IN();
	CString GetAcceptIpAddr();
	void Close();
	SOCKET_DATA GetSocketData();


	BYTE* m_pCurrentBuffer;
	virtual void OnDataReceived();
	virtual int Running();

	// Thread
	CEvent      m_evtThread;
	CWinThread *m_pThread;
	HANDLE		m_hThread;
	BOOL		m_bModify;
	BOOL		m_bAlive;

	void StopThread();
	void WaitUntilThreadEnd(HANDLE hThread);


protected:
	static UINT WINAPI SocketThreadProc(LPVOID pParam);
};


