#include "stdafx.h"
#include "TcpIpAccept.h"
#include "TcpIpServer.h"


CTcpIpAccept::CTcpIpAccept(CTcpIpServer* pParent/*=NULL*/)
{
	::InitializeCriticalSection(&m_sc);

	m_pParent = pParent;
	m_nClientID = -1;

	m_pReceiveBuffer = new BYTE[16384]; // 1mb
	m_nBufferId = 0;
	m_pCurrentBuffer = new BYTE[16384]; // 1mb

	m_bStop = FALSE;
	m_bClose = FALSE;
	//m_sAcceptIp = _T("");
}

CTcpIpAccept::~CTcpIpAccept()
{
	//m_sAcceptIp.ReleaseBuffer();

	//if(m_pReceiveBuffer)
	//{
	//	delete[] m_pReceiveBuffer;
	//	m_pReceiveBuffer = NULL;
	//}
	//if (m_pCurrentBuffer)
	//{
	//	delete[] m_pCurrentBuffer; 
	//	m_pCurrentBuffer = NULL;
	//}

	//::DeleteCriticalSection(&m_sc);

	Close();
}


void CTcpIpAccept::Init(SOCKET sock, sockaddr ClientIP, int nClientID)
{
	m_hComm = (HANDLE)sock;
	m_ClientIP = ClientIP;
	m_nClientID = nClientID;
}

void CTcpIpAccept::Init(SOCKET sock, sockaddr_in ClientIP, int nClientID)
{
	m_hComm = (HANDLE)sock;
	m_ClientIP_IN = ClientIP;
	m_nClientID = nClientID;
}

sockaddr CTcpIpAccept::GetIP()
{
	return m_ClientIP;
}

sockaddr_in CTcpIpAccept::GetIP_IN()
{
	return m_ClientIP_IN;
}

CString CTcpIpAccept::GetAcceptIpAddr()
{
	/*
	char ip[MAX_PATH];
	//CString sIP;
	//LPTSTR strAddress = sIP.GetBuffer(0);
	LPTSTR strAddress = m_sAcceptIp.GetBuffer();
	inet_ntop(AF_INET, &(m_ClientIP_IN.sin_addr), ip, INET_ADDRSTRLEN);
	MultiByteToWideChar(CP_ACP, 0, ip, -1, strAddress, sizeof(ip));
	//return sIP;
	*/
	char ip[MAX_PATH];
	inet_ntop(AF_INET, &(m_ClientIP_IN.sin_addr), ip, INET_ADDRSTRLEN);
	return CharToString(ip);
}

BOOL CTcpIpAccept::Start()
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

void CTcpIpAccept::StopThread() // Worker Thread 구동관련 Step9
{
	if (m_pThread != NULL)
	{
		m_evtThread.SetEvent();
		WaitUntilThreadEnd(m_hThread);
	}
	//m_pThread = NULL;
	m_bModify = FALSE;
}

void CTcpIpAccept::WaitUntilThreadEnd(HANDLE hThread) // Worker Thread 구동관련 Step6
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
UINT WINAPI CTcpIpAccept::SocketThreadProc(LPVOID pParam)
{
	CTcpIpAccept* pThis = reinterpret_cast<CTcpIpAccept*>(pParam);
	_ASSERTE(pThis != NULL);
	pThis->m_bAlive = TRUE;

	while (!pThis->m_bStop)
	{
		int nExit = pThis->Running();
		if (nExit == 0)
			break;
	}
	//pThis->Close();

	//pThis->m_bAlive = FALSE;
	Sleep(10);
	return 1L;
} // end SocketThreadProc


int CTcpIpAccept::Running()
{
	if (GetServerID() == ID_SR1000W || GetServerID() == ID_MDX2500)
	{
		return ReadComm();
	}
	else if (GetServerID() == ID_ENGRAVE)
	{
		return ReadCommData(m_SocketData, sizeof(m_SocketData), INFINITE);
		//return ReadCommData(m_SocketData, sizeof(m_SocketData), 50000);
	}

	return 0; // Terminate Thread
}

int CTcpIpAccept::ReadCommData(SOCKET_DATA &SocketData, DWORD dwSize, DWORD dwTimeout)
{
	::EnterCriticalSection(&m_sc);
	CString sMsg;

	if (m_bStop)
	{
		::LeaveCriticalSection(&m_sc);
		return 0; // Terminate Thread
	}

	//int i = 0;
	//CString strPort;
	//strPort.Format(_T("%d"), m_nNetworkPort);

	//m_bConnected = ConnectTo(m_strClientIP, m_strServerIP, strPort, AF_INET);

	//if (m_bStop)
	//{
	//	::LeaveCriticalSection(&m_sc);
	//	return 0; // Terminate Thread
	//}

	//if (m_bConnected == 0)		// Server is failed.
	//{
	//	sMsg = _T("Fail to connect.");
	//	::SendMessage(m_hParentWnd, WM_CLIENT_RECEIVED, (WPARAM)GetServerID(), (LPARAM)(LPCTSTR)sMsg);
	//	::PostMessage(m_hParentWnd, WM_CLIENT_CLOSED, (WPARAM)GetServerID(), (LPARAM)0);

	//	::LeaveCriticalSection(&m_sc);
	//	return 0; // Terminate Thread
	//}

	//OnEvent(EVT_CONSUCCESS);
	int	dwBytes = 0L;

	while (IsOpen() && !m_bStop)
	{
		// Blocking mode: Wait for event
		dwBytes = ReadPacketData(m_SocketData, sizeof(m_SocketData), dwTimeout);
		// Error? - need to signal error
		if (dwBytes >= 0)
		{
			OnDataReceived();
		}
		else
			dwBytes = (DWORD)(-1L);

		// Error? - need to signal error
		if (dwBytes == (DWORD)-1L)
		{
			// Do not send event if we are closing
			//if (IsOpen())
			//	OnEvent(EVT_CONDROP); // lost connection

			// 연결 상태 끊어짐 확인.
			((CTcpIpServer*)m_pParent)->wmAcceptClosed((WPARAM)m_nClientID, (LPARAM)0);

			break;
		}


		Sleep(0);
	}

	StopComm();

	::LeaveCriticalSection(&m_sc);
	return 0; // Terminate Thread
}

int CTcpIpAccept::ReadPacketData(SOCKET_DATA &SocketData, DWORD dwSize, DWORD dwTimeout)
{
	_ASSERTE(IsOpen());

	if (dwSize < 1L)
	{
		return -1;
	}

	fd_set	fdRead = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if (INFINITE != dwTimeout) {
		stTime.tv_sec = dwTimeout / 1000;
		stTime.tv_usec = (dwTimeout * 1000) % 1000000;
		pstTime = &stTime;
	}

	SOCKET s = (SOCKET)m_hComm;
	// Set Descriptor
	if (!FD_ISSET(s, &fdRead))
		FD_SET(s, &fdRead);

	// Select function set read timeout
	int dwBytesRead = 0L;
	int res = select(s + 1, &fdRead, NULL, NULL, pstTime);
	if (res > 0)
	{
		memset(SocketData.strData, 0, BUFFER_DATA_SIZE);

		//res = recv(s,(LPSTR)&SocketData,dwSize,0);

		int nIdx = 4;
		res = recv(s, (LPSTR)&SocketData, nIdx, 0);
		if (res > 0)
		{
			int nLeft = ntohl(SocketData.nMsgLen) - nIdx; // 호스트 오더로 변컖E
			while (nLeft > 0)
			{
				res = select(s + 1, &fdRead, NULL, NULL, pstTime);
				if (res > 0)
				{
					res = recv(s, (LPSTR)&SocketData + nIdx, nLeft, 0);
					if (res > 0)
					{
						nLeft -= res;// 남은 바이트
						nIdx += res;
					}
					else if (res <= 0)
					{
						dwBytesRead = (-1);
						return dwBytesRead;
					}
				}
			}

			SocketData.nMsgLen = ntohl(SocketData.nMsgLen);
			SocketData.nMsgID = ntohl(SocketData.nMsgID);
			SocketData.nCmdCode = ntohl(SocketData.nCmdCode);
			SocketData.nTxPC = ntohl(SocketData.nTxPC);
			SocketData.nRxPC = ntohl(SocketData.nRxPC);
			SocketData.nData1 = ntohl(SocketData.nData1);
			SocketData.nData2 = ntohl(SocketData.nData2);
			SocketData.nData3 = ntohl(SocketData.nData3);
			SocketData.nData4 = ntohl(SocketData.nData4);
			SocketData.fData1 = ntohl(SocketData.fData1);
			SocketData.fData2 = ntohl(SocketData.fData2);
			SocketData.fData3 = ntohl(SocketData.fData3);
			SocketData.fData4 = ntohl(SocketData.fData4);
			res = SocketData.nMsgLen - 52;
			SocketData.strData[res] = NULL;
			dwBytesRead = (DWORD)((SocketData.nMsgLen > 0) ? (SocketData.nMsgLen) : (-1L));
		}
		else
			dwBytesRead = -1;
	}

	return dwBytesRead;
}


int CTcpIpAccept::ReadComm()
{
	::EnterCriticalSection(&m_sc);

/*	char buffer[1024];
	sprintf(buffer, "Connection is success.");
	int nLen = strlen(buffer);

	WriteComm((BYTE*)buffer, nLen, 5000);
*/
	//m_bConnected = 1;
	//OnEvent(EVT_CONSUCCESS);

	int i = 0;

	DWORD	dwBytes = 0L;
	DWORD	dwTimeout = INFINITE;
	//DWORD	dwTimeout = 5000;

	while (IsOpen() && !m_bStop)
	{
		fd_set	fdRead = { 0 };
		TIMEVAL	stTime;
		TIMEVAL	*pstTime = NULL;

		//160107 LGH 
		if (INFINITE != dwTimeout) {
			stTime.tv_sec = dwTimeout / 1000;
			stTime.tv_usec = (dwTimeout * 1000) % 1000000;
			pstTime = &stTime;
		}

		SOCKET s = (SOCKET)m_hComm;
		// Set Descriptor
		if (!FD_ISSET(s, &fdRead))
			FD_SET(s, &fdRead);

		// Select function set read timeout
		DWORD dwBytesRead = 0L;
		int res = select(s + 1, &fdRead, NULL, NULL, pstTime);
		if (res > 0)
		{
			BYTE buffer[1024] = { 0, };
			int nIdx = 1024; //protocol의 헤더 길이만큼 딴다

			res = recv(s, (LPSTR)&buffer, nIdx, 0);
			if (res > 0)
			{
				memcpy(&m_pReceiveBuffer[m_nBufferId], &buffer[0], res);
				memset(m_pCurrentBuffer, 0, 16384);
				memcpy(m_pCurrentBuffer, &m_pReceiveBuffer[m_nBufferId], res);
				m_pCurrentBuffer[res] = _T('\0');
				m_nBufferId += res;

				OnDataReceived();

				int nLeftLength = m_nBufferId - (i+1);
				int nStartID = m_nBufferId - i;//3-1= id[2]

				if (nLeftLength > 0)
					memmove(&m_pReceiveBuffer[0], &m_pReceiveBuffer[nStartID], nLeftLength);
				else
				{
					m_nBufferId = 0;
					memset(m_pReceiveBuffer, 0, 16384);
				}

				dwBytesRead = res;
			}
			else
			{
				// 연결 상태 끊어짐 확인.
				((CTcpIpServer*)m_pParent)->wmAcceptClosed((WPARAM)m_nClientID, (LPARAM)0);

				::LeaveCriticalSection(&m_sc);
				return 0; // Terminate Thread
			}
		}
		else
		{
			dwBytesRead = (DWORD)(-1L); 
		}

		// Error? - need to signal error
		if (dwBytes == (DWORD)-1L)
		{
			// Do not send event if we are closing

			break;
		}

		Sleep(0);
	}

	StopComm();

	::LeaveCriticalSection(&m_sc);
	return 0; // Terminate Thread
}

void CTcpIpAccept::OnDataReceived()
{
	//if (GetServerID() == ID_SR1000W || GetServerID() == ID_MDX2500)
	//{
	//	m_strReceived = CharToString((char*)m_pCurrentBuffer);
	//	((CTcpIpServer*)m_pParent)->wmAcceptReceived((WPARAM)m_nClientID, (LPARAM)(LPCTSTR)m_strReceived);
	//}
	//else if (GetServerID() == ID_ENGRAVE)
	//{
	//	((CTcpIpServer*)m_pParent)->wmAcceptReceived((WPARAM)m_nClientID, (LPARAM)&m_SocketData);
	//}

	if (GetServerID() == ID_ENGRAVE)
	{
		((CTcpIpServer*)m_pParent)->wmAcceptReceived((WPARAM)m_nClientID, (LPARAM)&m_SocketData);
	}
}

BOOL CTcpIpAccept::Stop()
{
	m_bStop = 1;

	if (IsOpen())
	{
		StopComm();
		Sleep(100);
		return TRUE;
	}

	return FALSE;
}

void CTcpIpAccept::Close()
{
	if (m_bClose)
		return;
	m_bClose = TRUE;

	if (m_pReceiveBuffer)
	{
		delete[] m_pReceiveBuffer;
		m_pReceiveBuffer = NULL;
	}
	if (m_pCurrentBuffer)
	{
		delete[] m_pCurrentBuffer;
		m_pCurrentBuffer = NULL;
	}

	::DeleteCriticalSection(&m_sc);
}

SOCKET_DATA CTcpIpAccept::GetSocketData()
{
	return m_SocketData;
}
