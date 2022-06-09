//Rs232.cpp Rs232c����� �ϱ� ���� Ŭ����
#include "stdafx.h"
#include "Rs232.h"
#include "../Global/GlobalDefine.h"
#include "../Global/GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRs232, CObject)



CRs232::CRs232()
{
	m_hComDev = NULL;
	m_nFlowCtrl = FC_XONXOFF ;
	m_bConnected = FALSE ;

	m_osRead.hEvent = NULL;
	m_osWrite.hEvent = NULL;

	m_dwBaudRate = CBR_9600;
	m_bCR = FALSE;
	m_bLF = FALSE;
	m_bOnLine = FALSE;

	//memset(m_cRxBuffer, NULL, sizeof(BYTE)*MAXBLOCK );
	m_cRxBuffer = _T("");
}

CRs232::~CRs232()
{
	if (m_bConnected)
		CloseConnection();
//	DestroyRs232();
}

//BEGIN_MESSAGE_MAP(CRs232, CObject)
	//{{AFX_MSG_MAP(CRs232)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRs232 message handlers
//Rs232WatchProc()
//����� �ϴ� ���μ��� �� ����Ÿ�� �������� �����ϴ�
//��ƾ ����ƾ�� OpenComPort �Լ� ����� ���ν����� �����
//OpenComPort �Լ� ����
DWORD Rs232WatchProc(LPVOID lpData)
{
	DWORD       dwEvtMask ;
	OVERLAPPED  os ;
	CRs232*      npRs232 = (CRs232*) lpData ;
	char InData[MAXBLOCK + 1];
	int        nLength ;
	//idRs232Dev ��� �ڵ鿡 �ƹ��� com ��Ʈ�� �Ⱥپ� ������
	// ���� ����
	if ( npRs232 == NULL || !npRs232->IsKindOf( RUNTIME_CLASS( CRs232 ) ) )
		return (DWORD)(-1);
	if ( !npRs232->m_hComDev )
		return (DWORD)(-1);

	memset( &os, 0, sizeof( OVERLAPPED ) ) ;


	os.hEvent = CreateEvent(	NULL,    // no security
								TRUE,    // explicit reset req
								FALSE,   // initial event reset
								NULL ) ; // no name
	if ( os.hEvent == NULL )
	{
		MessageBox( NULL, _T("Failed to create event for thread!"), _T("Rs232 Error!"),
					MB_ICONEXCLAMATION | MB_OK ) ;
		return ( FALSE ) ;
	}

	if (!SetCommMask(npRs232->m_hComDev, EV_RXCHAR ))
		return ( FALSE ) ;

	while (npRs232->m_bConnected)
	{
		dwEvtMask = 0 ;

		WaitCommEvent(npRs232->m_hComDev, &dwEvtMask, NULL );

		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{

			do
			{
				if(!npRs232->m_bConnected)
					break;

				memset(InData, 0, sizeof(char)*MAXBLOCK);
				if (nLength = npRs232->ReadRs232Block((LPSTR) InData, MAXBLOCK ))
				{
					npRs232->SetReadData(InData);
					//�̰����� ����Ÿ�� �޴´�.
				}
			}
			while ( nLength > 0 ) ;
		}
	}


	CloseHandle( os.hEvent ) ;


	return( TRUE ) ;

} 

//����Ÿ�� �ϰ� ����Ÿ�� �о��ٴ�
//�޼����� �����Ѵ�.
void CRs232::SetReadData(LPSTR data)
{
	//lstrcpy((LPSTR)m_cRxBuffer,(LPSTR)data);
	m_cRxBuffer = CharToString(data);
	//ConverData
	//������ �����쿡 WM_RECEIVEDATA�޼�����
	//�����־� ���� ����Ÿ�� ���Դٴ°���
	//�˷��ش�.
	SendMessage(m_hwnd,WM_RECEIVE_RS232,0,0);
}

//�޼����� ������ hwnd����
void CRs232::SetHwnd(HWND hwnd)
{
	m_hwnd=hwnd;
}

//����Ʈ�� �����Ѵ�.
void CRs232::SetComPort(int port,DWORD rate,BYTE bytesize,BYTE stop,BYTE parity)
{
	m_nPort=port;
	m_dwBaudRate=rate;
	m_nByteSize=bytesize;
	m_nStopBits=stop;
	m_nParity=parity;
}

//XonOff �� ���ϰ� ���� ����
void CRs232::SetXonOff(BOOL chk)
{
	m_bXonXoff=chk;
}

void CRs232::SetDtrRts(BYTE chk)
{
	m_nFlowCtrl=chk;
}

void CRs232::ClearReceive()
{
	//memset(m_cRxBuffer, NULL, sizeof(BYTE)*MAXBLOCK );
	m_cRxBuffer = _T("");
}

//����Ʈ ������ �����.
//�̰��� ���鶧 ������ ������
// SetComPort(); -> SetXonOff() ->SetDtrRts() �Ѵ��� �����Ѵ�.
BOOL CRs232::CreateRs232Info()
{


	m_osWrite.Offset = 0 ;
	m_osWrite.OffsetHigh = 0 ;
	m_osRead.Offset = 0 ;
	m_osRead.OffsetHigh = 0 ;

	//�̺�Ʈ â�� ����
	m_osRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ; 
	if (m_osRead.hEvent == NULL)
	{
		return FALSE ;
	}
	m_osWrite.hEvent = CreateEvent( NULL,   TRUE,  FALSE,   NULL ) ;
	if (m_osWrite.hEvent == NULL)
	{
		CloseHandle( m_osRead.hEvent ) ;
		return FALSE;
	}


	return TRUE ;

} 

//com ��Ʈ�� ���� ������ �õ��Ѵ�.
//OpenComport()
BOOL CRs232::OpenComPort()
{
	m_bOnLine = FALSE;

	if(m_dwBaudRate <=110)
		m_dwBaudRate = CBR_110;
	else if(m_dwBaudRate <=300)
		m_dwBaudRate = CBR_300;
	else if(m_dwBaudRate <=600)
		m_dwBaudRate = CBR_600;  
	else if(m_dwBaudRate <=1200)
		m_dwBaudRate = CBR_1200; 
	else if(m_dwBaudRate <=2400)
		m_dwBaudRate = CBR_2400; 
	else if(m_dwBaudRate <=4800)
		m_dwBaudRate = CBR_4800; 
	else if(m_dwBaudRate <=9600)
		m_dwBaudRate = CBR_9600; 
	else if(m_dwBaudRate <=14400)
		m_dwBaudRate = CBR_14400; 
	else if(m_dwBaudRate <=19200)
		m_dwBaudRate = CBR_19200; 
	else if(m_dwBaudRate <=38400)
		m_dwBaudRate = CBR_38400; 
	else if(m_dwBaudRate <=56000)
		m_dwBaudRate = CBR_56000; 
	else if(m_dwBaudRate <=57600)
		m_dwBaudRate = CBR_57600; 
	else if(m_dwBaudRate <=115200)
		m_dwBaudRate = CBR_115200; 
	else if(m_dwBaudRate <=128000)
		m_dwBaudRate = CBR_128000; 
	else if(m_dwBaudRate <=256000)
		m_dwBaudRate = CBR_256000; 
	else
		m_dwBaudRate = CBR_9600;

	if(!CreateRs232Info())
		return FALSE;

	//char       szPort[ 15 ];
	CString szPort;

	BOOL       bRetVal=FALSE;
	COMMTIMEOUTS  CommTimeOuts;


	if (m_nPort > MAXPORTS)
		szPort.Format(_T("%s"), _T("\\\\.\\TELNET"));		//lstrcpy(szPort, "\\\\.\\TELNET");
	else
		szPort.Format(_T("COM%d"), m_nPort);

	// COMM device�� ȭ���������� �����Ѵ�.
	if(m_hComDev)
	{
		CloseHandle(m_hComDev);
		m_hComDev = NULL;
	}

	if( (m_hComDev = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE,
																  0,                    // exclusive access
																  NULL,                 // no security attrs
																  OPEN_EXISTING,
																  FILE_ATTRIBUTE_NORMAL | 
																  FILE_FLAG_OVERLAPPED, // overlapped I/O
																  NULL )) == (HANDLE) -1 )
	{
		if(m_osRead.hEvent)
		{
			CloseHandle( m_osRead.hEvent );
			m_osRead.hEvent = NULL;
		}
		if(m_osWrite.hEvent)
		{
			CloseHandle( m_osWrite.hEvent );
			m_osWrite.hEvent = NULL;
		}

		m_hComDev = NULL;
		return ( FALSE ) ;
	}
	else
	{
		//����Ʈ���� ����Ÿ�� ��ȯ�ϴ� ����� char������ �⺻���� ����
		//����
		SetCommMask( m_hComDev, EV_RXCHAR ) ;
		SetupComm( m_hComDev, 4096, 4096 ) ;
		//����̽��� �����Ⱑ ������ �𸣴ϱ� ������ û�Ҹ� ����!
		PurgeComm( m_hComDev, PURGE_TXABORT | PURGE_RXABORT |
									  PURGE_TXCLEAR | PURGE_RXCLEAR ) ;


		CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;//1;// 0 ;
		CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0;//2*CBR_9600 / m_dwBaudRate;//0 ;
		CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;
		SetCommTimeouts( m_hComDev, &CommTimeOuts ) ;
	}

	bRetVal = SetupConnection() ;

	if (bRetVal)//������ �Ǿ��ٸ� fRetVal TRUE�̹Ƿ�
	{
		m_bConnected = TRUE ;//����Ǿ��ٰ� ������
		//���ν����� CommWatchProc�� �����ϴϱ� ���߿� ����Ÿ�� �Դٰ���
		//�ϸ� ��� ������ CommWatchProc�� ����Ѵ�.
		AfxBeginThread((AFX_THREADPROC)Rs232WatchProc,(LPVOID)this);
	}
	else
	{
		DestroyRs232();
	}

	m_bOnLine = bRetVal;
	return ( bRetVal ) ;
} 

//ȭ�Ϸ� ������ ����Ʈ�� ���� ��Ʈ�� ������ ��Ų��.
//SetupConnection ������ CreateComPort�� ���־�� �Ѵ�.
BOOL CRs232::SetupConnection()
{
	BOOL       bRetVal ;
//	BYTE       bSet ;
	DCB        dcb ;

	dcb.DCBlength = sizeof( DCB ) ;

	GetCommState( m_hComDev, &dcb ) ;//dcb�� �⺻���� �޴´�.

	//�̺κ��� �����ؾ� �մϴ�.
	dcb.BaudRate = m_dwBaudRate;//���ۼӵ�
	dcb.ByteSize = 8;//bByteSize ;//����Ÿ��Ʈ
	dcb.Parity = 0;//m_nParity;//�и�Ƽ üũ ( 0 ~ 4 = no,odd,even,mark,space )
	dcb.StopBits = ONESTOPBIT;//m_nStopBits;//�����Ʈ 
// 	ONESTOPBIT		1	stop bit 
// 	ONE5STOPBITS	1.5 stop bits 
// 	TWOSTOPBITS		2	stop bits 

/*
	dcb.fOutxDsrFlow =0 ;//Dsr Flow
	dcb.fDtrControl = DTR_CONTROL_DISABLE;// DTR_CONTROL_ENABLE ;//Dtr Control
	dcb.fOutxCtsFlow = 0 ;//Cts Flow
	dcb.fRtsControl = RTS_CONTROL_DISABLE;// RTS_CONTROL_ENABLE ; //Ctr Control
	dcb.fInX = dcb.fOutX = 1 ; //XON/XOFF ���Ѱ�
	dcb.XonChar = ASCII_XON ;
	dcb.XoffChar = ASCII_XOFF ;
	dcb.XonLim = 100 ;
	dcb.XoffLim = 100 ;

	dcb.fBinary = FALSE ;
	dcb.fParity = FALSE ;
// 	dcb.fBinary = TRUE ;
// 	dcb.fParity = TRUE ;
*/
	bRetVal = SetCommState( m_hComDev, &dcb ) ; //����� Dcb ����

	return ( bRetVal ) ;

} 

//����Ʈ�� ���� ����Ÿ�� �д´�.
int CRs232::ReadRs232Block(LPSTR lpszBlock, int nMaxLength )
{
	BOOL       bReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength;

	// only try to read number of bytes in queue 
	ClearCommError( m_hComDev, &dwErrorFlags, &ComStat ) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;

	if (dwLength > 0)
	{
		bReadStat = ReadFile( m_hComDev, lpszBlock,
							dwLength, &dwLength, &m_osRead ) ;
		if (!bReadStat)
		{
			//�̰��� ���� �ִ°��̴�.
			//�� ReadFile ������ ����Ÿ�� ����� �ȳ����� bReadState�� ����
			//���� �ڵ带 �����Ѵ�. �̶� �����Ҽ������� ������ ��������
			//������ �Ұ����ϴ� ���� ������� �ش޶�� �޼����� ���ִ°���
			//����.

			dwLength = 0;
			ClearCommError( m_hComDev, &dwErrorFlags, &ComStat );
		}
	}

	return ( dwLength ) ;

} 

//����Ʈ�� ������ �����Ѵ�.

BOOL CRs232::DestroyRs232()
{
	if (m_bConnected)
		CloseConnection() ;

	if(m_osRead.hEvent)
	{
		CloseHandle( m_osRead.hEvent ) ;
		m_osRead.hEvent = NULL;
	}
	if(m_osWrite.hEvent)
	{
		CloseHandle( m_osWrite.hEvent ) ;
		m_osWrite.hEvent = NULL;
	}

	return TRUE;

} 

//������ �ݴ´�.
BOOL CRs232::CloseConnection()
{
	if(m_hComDev)
	{
		// set connected flag to FALSE

		m_bConnected = FALSE ;

		// disable event notification and wait for thread
		// to halt

		SetCommMask( m_hComDev, 0 ) ;
		Sleep(30);
		EscapeCommFunction( m_hComDev, CLRDTR ) ;
		PurgeComm( m_hComDev, PURGE_TXABORT | PURGE_RXABORT |
								PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
		CloseHandle( m_hComDev ) ;
		m_hComDev = NULL;
	}
	return TRUE;

} 



BOOL CRs232::WriteRs232Block( LPSTR lpByte , DWORD dwBytesToWrite)
{

	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	char		*pBuff;
	int			nLenBuff;

	if(m_hComDev)
	{
		if(m_bCR && m_bLF)
		{
			pBuff = new char[dwBytesToWrite+3];
			strcpy(pBuff, lpByte);
			pBuff[dwBytesToWrite] = ASCII_CR;
			pBuff[dwBytesToWrite+1] = ASCII_LF;
			nLenBuff = dwBytesToWrite + 2;
		}
		else if(m_bCR && !m_bLF)
		{
			pBuff = new char[dwBytesToWrite+2];
			strcpy(pBuff, lpByte);
			pBuff[dwBytesToWrite] = ASCII_CR;
			nLenBuff = dwBytesToWrite + 1;
		}
		else if(!m_bCR && m_bLF)
		{
			pBuff = new char[dwBytesToWrite+2];
			strcpy(pBuff, lpByte);
			pBuff[dwBytesToWrite] = ASCII_LF;
			nLenBuff = dwBytesToWrite + 1;
		}
		else
		{
			pBuff = new char[dwBytesToWrite+1];
			strcpy(pBuff, lpByte);
			nLenBuff = dwBytesToWrite;
		}

// 		fWriteStat = WriteFile( m_hComDev, lpByte, dwBytesToWrite,
// 												&dwBytesWritten, &m_osWrite ) ;
		fWriteStat = WriteFile( m_hComDev, pBuff, nLenBuff,
												&dwBytesWritten, &m_osWrite ) ;

		delete pBuff;

		if (!fWriteStat) 
		{
			//����Ʈ�� ����Ÿ�� ����� ����� ����������̴�.
			//�̶��� ��� �ұ� �װ��� ����� �����̰ڴ�.
			//�ٽ� ������ ������ ���ͼ����� �ϸ� �ȴ�.
			//�׷��� ������ ���� ������ ���� �ִٴ����� �����϶�
		}
		return TRUE;
	}

	return FALSE;
} 
