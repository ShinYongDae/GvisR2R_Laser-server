#include "stdafx.h"
#include "TcpIpServer.h"
#include "../Global/GlobalDefine.h"

/*
IMPLEMENT_DYNCREATE(CTcpIpServer, CWinThread)
BEGIN_MESSAGE_MAP(CTcpIpServer, CWinThread)
	//{{AFX_MSG_MAP(CTcpIpServer)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/
CTcpIpServer::CTcpIpServer(CWnd* pParent/*=NULL*/)
{
	::InitializeCriticalSection(&m_sc);

	int i = 0;
	m_pParent = pParent;
	m_hParentWnd = pParent->GetSafeHwnd();
	m_nConnectedID = -1;
	m_bStop = 0;

	for (i = 0; i < MAX_CLIENT; i++)
	{		
		m_pAcceptAddr[i] = NULL;
	}	

}

CTcpIpServer::~CTcpIpServer()
{
	
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if(m_pAcceptAddr[i])
		{ 
			m_pAcceptAddr[i]->Stop();
			//m_pAcceptAddr[i]->Close();
			delete m_pAcceptAddr[i];
			m_pAcceptAddr[i] = NULL;
			Sleep(30);
		}
	}
	
	::DeleteCriticalSection(&m_sc);
}


void CTcpIpServer::Init(CString strServerIP, int nPort)
{
	m_strServerIP = strServerIP;
	m_nNetworkPort = nPort;

	CString strPort;
	strPort.Format(_T("%d"), m_nNetworkPort);

	BOOL bSuccess = ListenFrom(m_strServerIP, strPort, AF_INET);	// 리슨(listen) 소켓이다 이건!!
																	// Should we run as server mode
}

BOOL CTcpIpServer::Start()
{
	//PrepareThread();
	//ResumeThread();

	HANDLE hThread;
	UINT uiThreadId = 0;

	//m_bReadCommData = TRUE; // 2006,05,04 추가 khc

	hThread = (HANDLE)_beginthreadex(NULL,	// Security attributes
		0,	// stack
		SocketThreadProc,	// Thread proc
		this,	// Thread param
		CREATE_SUSPENDED,	// creation mode
		&uiThreadId);	// Thread ID

	if (NULL != hThread)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
		ResumeThread(hThread);
		m_hThread = hThread;
		return TRUE;
	}
/*
	if (m_pThread == NULL)
	{
		m_bModify = TRUE;
		m_evtThread.ResetEvent();
		m_pThread = AfxBeginThread(SocketThreadProc, this);
		if (m_pThread)
			m_hThread = m_pThread->m_hThread;
		return TRUE;
	}
*/
	return FALSE;
}

void CTcpIpServer::StopThread() // Worker Thread 구동관련 Step9
{
	if (m_pThread != NULL)
	{
		m_evtThread.SetEvent();
		WaitUntilThreadEnd(m_hThread);
	}
	m_pThread = NULL;
	m_bModify = FALSE;
}

void CTcpIpServer::WaitUntilThreadEnd(HANDLE hThread) // Worker Thread 구동관련 Step6
{
	TRACE("WaitUntilThreadEnd(0x%08x:RunThread) Entering\n", hThread);
	MSG message;
	const DWORD dwTimeOut = 500000;
	DWORD dwStartTick = GetTickCount();
	DWORD dwExitCode;
	while (GetExitCodeThread(hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE && m_bAlive) {
		// Time Out Check
		if (GetTickCount() >= (dwStartTick + dwTimeOut))
		{
			//pView->MsgBox("WaitUntilThreadEnd() Time Out!!!");
			AfxMessageBox(_T("WaitUntilThreadEnd() Time Out!!!", NULL, MB_OK | MB_ICONSTOP));
			return;
		}
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
	TRACE("WaitUntilThreadEnd(0x%08x:DispMsgThread) Exit\n", hThread);
}

///////////////////////////////////////////////////////////////////////////////
// SocketThreadProc
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//     Socket Thread function.  This function is the main thread for socket
//     communication - Asynchronous mode.
// PARAMETERS:
//     LPVOID pParam : Thread parameter - a CSocketComm pointer
// NOTES:
///////////////////////////////////////////////////////////////////////////////
UINT WINAPI CTcpIpServer::SocketThreadProc(LPVOID pParam)
{
	CTcpIpServer* pThis = reinterpret_cast<CTcpIpServer*>(pParam);
	_ASSERTE(pThis != NULL);
	pThis->m_bAlive = TRUE;
	while(!pThis->m_bStop)
	{
		int nExit = pThis->Running(); 
		if (nExit == 0)
			break;
	}
	pThis->m_bAlive = FALSE;
	Sleep(10);
	return 1L;
} // end SocketThreadProc


int CTcpIpServer::Running()
{
	::EnterCriticalSection(&m_sc);

	int i = 0;

	if (m_bStop)
	{
		::LeaveCriticalSection(&m_sc);
		return 0; // Terminate Thread
	}


	SOCKET		sock = (SOCKET)m_hComm;
	//sockaddr	client_addr;
	sockaddr_in	client_addr;
	socklen_t	client_addr_size = sizeof(client_addr);

	//sock = WaitForConnection(sock, (sockaddr *)&client_addr, (int*)&client_addr_size); //클라이언트 소켓을 받고 (accept)
	sock = WaitForConnection(sock, (sockaddr_in *)&client_addr, (int*)&client_addr_size); //클라이언트 소켓을 받고 (accept)
	
	//if (!m_bStop)
	//{ 
	//	char ip[MAX_PATH];
	//	CString sIP;
	//	LPTSTR strAddress = sIP.GetBuffer();
	//	inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
	//	MultiByteToWideChar(CP_ACP, 0, ip, -1, strAddress, sizeof(ip));
	//	m_sAcceptIp.Format(_T("%s"), sIP);
	//	sIP.ReleaseBuffer();
	//}

	if (sock != INVALID_SOCKET)
	{
		for (i = 0; i < MAX_CLIENT; i++)
		{
			if (!m_pAcceptAddr[i])
			{
				//CTcpIpAccept* pClientAddr = new CTcpIpAccept(this);
				m_nConnectedID = i;
				m_pAcceptAddr[m_nConnectedID] = new CTcpIpAccept(this);
				//m_pAcceptAddr[m_nConnectedID]->Init(sock, (sockaddr)client_addr, m_nConnectedID);
				m_pAcceptAddr[m_nConnectedID]->Init(sock, (sockaddr_in)client_addr, m_nConnectedID);
				m_pAcceptAddr[m_nConnectedID]->Start();
				m_pAcceptAddr[m_nConnectedID]->SetServer(GetServerID());
				break;
			}
		}

		if (i >= MAX_CLIENT)
		{
			::LeaveCriticalSection(&m_sc);
			return 1;
		}

	}
	else
	{
		// Do not send event if we are closing

		if (m_bStop)
		{
			::LeaveCriticalSection(&m_sc);
			return 0; // Terminate Thread
		}

		::LeaveCriticalSection(&m_sc);
		return 1; 
	}

	::LeaveCriticalSection(&m_sc);
	return 1;
}



void CTcpIpServer::wmAcceptReceived(WPARAM wParam, LPARAM lParam)
{
	int nClientID = (int)wParam;

	//if (GetServerID() == ID_SR1000W || GetServerID() == ID_MDX2500)
	//{
	//	CString sReceived = (LPCTSTR)lParam;
	//	::SendMessage(m_hParentWnd, WM_SERVER_RECEIVED, (WPARAM)nClientID, (LPARAM)(LPCTSTR)sReceived);
	//}
	//else if (GetServerID() == ID_ENGRAVE)
	//{
	//	if (m_pAcceptAddr[nClientID])
	//	{
	//		m_SocketData = m_pAcceptAddr[nClientID]->GetSocketData();			
	//		::SendMessage(m_hParentWnd, WM_SERVER_RECEIVED, (WPARAM)nClientID, (LPARAM)&m_SocketData);
	//	}
	//}

	if (GetServerID() == ID_ENGRAVE)
	{
		if (m_pAcceptAddr[nClientID])
		{
			m_SocketData = m_pAcceptAddr[nClientID]->GetSocketData();			
			::SendMessage(m_hParentWnd, WM_SERVER_RECEIVED, (WPARAM)nClientID, (LPARAM)&m_SocketData);
		}
	}
}

void CTcpIpServer::wmAcceptClosed(WPARAM wParam, LPARAM lParam)
{
	int nClientID = (int)wParam;

	if (m_pAcceptAddr[nClientID])
	{
		m_pAcceptAddr[nClientID]->Stop();
		delete m_pAcceptAddr[nClientID];
		m_pAcceptAddr[nClientID] = NULL;
		Sleep(30);
	}
}

BOOL CTcpIpServer::Stop()
{
	int i = 0;


	m_bStop = 1;	// 1: Stop CTcpIpServer Running()

	if (IsOpen())
	{
		for (i = 0; i < MAX_CLIENT; i++)
		{
			if(m_pAcceptAddr[i])
			{
				CTcpIpAccept* pAccept = m_pAcceptAddr[i];
				pAccept->Stop();
				Sleep(10);
				pAccept->StopThread();
				Sleep(10);
				pAccept->Close();
				//m_pAcceptAddr[i]->Close();
				//delete pAccept;
				if (m_pAcceptAddr[i])
					delete m_pAcceptAddr[i];
				m_pAcceptAddr[i] = NULL;
				Sleep(10);
			}
		}

		StopComm();
		return TRUE;
	}
	else
	{
		Shutdown();
		Sleep(30);
		return TRUE;
	}

	return FALSE;
}

BOOL CTcpIpServer::IsConnected(int nClientID)
{
	if (nClientID >= MAX_CLIENT)
		return FALSE;

	return (m_pAcceptAddr[nClientID]->IsOpen());
}

int CTcpIpServer::WriteComm(int nClientID, CString sMsg)
{
	if (m_pAcceptAddr[nClientID])
	{
		char buffer[1024];
		StringToChar(sMsg, buffer);
		int nLen = strlen(buffer);

		m_pAcceptAddr[nClientID]->WriteComm((BYTE*)buffer, nLen, 5000);
	}

	return 0;
}

DWORD CTcpIpServer::WriteSocketData(int nClientID, SOCKET_DATA SocketData, DWORD dwTimeout, DWORD dwStrLen)
{
	if (m_pAcceptAddr[nClientID])
	{
		m_pAcceptAddr[nClientID]->WriteCommData(SocketData, dwTimeout);
	}

	return 0;
}

BOOL CTcpIpServer::IsConnected(CString addrClient, int& nClientId)
{
	nClientId = -1;
	for (int nClientID = 0; nClientID < MAX_CLIENT; nClientID++)
	{
		if(m_pAcceptAddr[nClientID])
		{
			CString sIP = m_pAcceptAddr[nClientID]->GetAcceptIpAddr();
			if (!addrClient.Compare(sIP)) // Equal
			{
				if (IsConnected(nClientID))
				{
					nClientId = nClientID; 
					return TRUE;
				}

				return FALSE;
			}
		}
	}

	return FALSE;
}

BOOL CTcpIpServer::WriteComm(CString addrClient, CString sMsg)
{
	//for (int nClientID = 0; nClientID < MAX_CLIENT; nClientID++)
	//{
	//	//LPTSTR sIP;
	//	wchar_t *sIP = NULL;
	//	sIP = new WCHAR[MAX_PATH];

	//	m_pAcceptAddr[nClientID]->GetLocalAddress(sIP, MAX_PATH);

	//	if (!addrClient.Compare(sIP)) // Equal
	//	{
	//		WriteComm(nClientID, sMsg);
	//		return TRUE;
	//	}
	//}

	return FALSE;
}

SOCKET_DATA CTcpIpServer::GetSocketData()
{
	return m_SocketData;
}

int CTcpIpServer::CharToAddr(struct in_addr *addr, const char *str)
{
	struct in_addr addr_t;
	struct hostent *hostent_t;

	if (inet_pton(AF_INET, str, &addr_t)) 
	{
		*addr = addr_t;
		return 0;
	}

	if ((hostent_t = gethostbyname(str)) == NULL)
		return -1;

	if (hostent_t->h_addrtype != AF_INET)
		return -1;

	*addr = *((struct in_addr *) (hostent_t->h_addr_list[0]));

	return 0;

}

BOOL CTcpIpServer::AddrToString(LPTSTR strAddress, struct sockaddr_in addr)
{
	//return inet_ntop(AF_INET, &addr, str, len);
	char buff[MAX_PATH] = { 0 };

	sprintf(buff, "%d.%d.%d.%d",
		addr.sin_addr.s_addr & 0x00FF,
		addr.sin_addr.s_addr >> 8 & 0x00FF,
		addr.sin_addr.s_addr >> 16 & 0x00FF,
		addr.sin_addr.s_addr >> 24 & 0x00FF);
#ifdef _UNICODE
	return (0 != MultiByteToWideChar(CP_ACP, 0, buff, -1, strAddress, sizeof(buff)));
#else
	_tcscpy(strAddress, strHost);
	return TRUE;
#endif

	return FALSE;
}

int CTcpIpServer::GetConnectedId()
{
	return m_nConnectedID;
}
