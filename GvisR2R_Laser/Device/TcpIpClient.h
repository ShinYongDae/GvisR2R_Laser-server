#pragma once

#include "CEasyThread.h"
#include "SocketComm.h"

class CTcpIpClient : public CSocketComm, public CEasyThread
{
	HWND m_hParentWnd;
	CWnd* m_pParent;
	BOOL m_bConnected;	// Connection is done.
	CString m_strServerIP;
	CString m_strClientIP;
	int m_nBufferId;
	CString m_strReceived;

	BYTE* m_pReceiveBuffer;
	BYTE* m_pCurrentBuffer;

	int ReadComm();
	int ReadCommData(SOCKET_DATA &SocketData, DWORD dwSize, DWORD dwTimeout);
	int ReadPacketData(SOCKET_DATA &SocketData, DWORD dwSize, DWORD dwTimeout);

public:
	CTcpIpClient(CWnd* pParent = NULL);
	virtual ~CTcpIpClient();
	virtual void OnEvent(UINT uEvent);

	CRITICAL_SECTION m_sc;
	int m_nNetworkPort;

	void Init(CString strClientIP, CString strServerIP, int nPort);
	void Start();
	BOOL Stop();

	SOCKET_DATA GetSocketData();

	virtual int Running();
	virtual void OnDataReceived();
};

