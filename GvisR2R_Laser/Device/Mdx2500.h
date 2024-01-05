#pragma once
#include "afxwin.h"

#include "TcpIpClient.h"

#define SET_ITS				1
#define SET_SHOT		2
#define	SET_ITS_SHOT		3

class CMdx2500 : public CWnd
{
	HWND m_hParentWnd;
	CTcpIpClient* m_pClient;

	CString	m_strPortSvr, m_strAddrSvr;
	CString	m_strAddrCli;
	int m_nServerID;

	BOOL m_bWaitForResponse;
	CString m_strResponse;
	int m_nCmd;
	BOOL m_bCmdF;

	BOOL m_bGuideLaserRect;


	void StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr);
	void StopClient();
	BOOL CheckResponse(int nCmd, CString sResponse);

public:
	CMdx2500(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent=NULL);
	~CMdx2500();

	void SetHwnd(HWND hParentWnd);
	void ResetWaitForResponse();
	BOOL GetMdxResponse(CString &sData);	// MDX2500 - Response
	BOOL IsRunning();
	BOOL GuideLaserRect(BOOL bOn);
	BOOL LaserMarking(BOOL bOn = TRUE);
	BOOL IsLaserReady();
	BOOL SetMdxOrderShotNum(CString sOrder, CString sShot, int nSet=SET_ITS_SHOT);
	BOOL GetMdxSettingData(CString& sOrderNum, CString& sShotNum);
	BOOL SetLaserPosZero();
	BOOL SetLaserPos(double* pData); // CString pData[5] : A,B,C,D,E
	int WriteComm(CString sMsg, DWORD dwTimeout = INFINITE);

	BOOL StartLaserMarking();
	BOOL WaitForResponse(DWORD dwTimeOut=5000, DWORD dwTimeLate=0);
	BOOL IsOK();
	BOOL WaitUntilLaserReady();

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
	void Close();

	int OnClientReceived(WPARAM wParam, LPARAM lParam);
	BOOL IsConnected();

protected:
	afx_msg LRESULT wmClientReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT wmClientClosed(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

