
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "GvisR2R_Laser.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "GvisR2R_LaserDoc.h"
#include "GvisR2R_LaserView.h"

extern CGvisR2R_LaserDoc* pDoc;
extern CGvisR2R_LaserView* pView;

// CMainFrame
CMainFrame* pFrm;


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_DATABASE_CONNECTION, &CMainFrame::OnDatabaseConnection)
	ON_WM_CLOSE()
	ON_COMMAND(ID_APP_MINIMIZE, &CMainFrame::OnAppMinimize)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR
	//ID_SEPARATOR,           // 상태 줄 표시기
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	pFrm = this;

	m_bLockDispStsBar = FALSE;
	for (int i = 0; i < 10; i++)
		m_sDispMsg[i] = _T("");
}

CMainFrame::~CMainFrame()
{
	pFrm = NULL;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	CRect rect;
	this->GetWindowRect(&rect);  //get client rect... 	
	int nScreenWidth = 1230;

	// Panel 크기조절 
	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 2 / 60);
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 16 / 60);
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 3 / 60);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 25 / 60);
	m_wndStatusBar.SetPaneInfo(4, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 3 / 60);
	m_wndStatusBar.SetPaneInfo(5, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 3 / 60);
	m_wndStatusBar.SetPaneInfo(6, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 2 / 60);
	m_wndStatusBar.SetPaneInfo(7, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 6 / 60);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	CGvisR2R_LaserApp *app = (CGvisR2R_LaserApp *)AfxGetApp();
	cs.lpszClass = app->m_singleInstance.GetClassName();

	cs.style = WS_VISIBLE | WS_POPUP;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기


void CMainFrame::DispStatusBar(CString strMsg, int nStatusBarID)
{
	m_cs.Lock();

	if (m_sDispMsg[nStatusBarID] != strMsg)
	{
		m_sDispMsg[nStatusBarID] = strMsg;

		m_wndStatusBar.SetPaneText(nStatusBarID, strMsg);
		// 		m_strStatusMsgArray.Add(strMsg);
		// 
		// 		int nCount = m_strStatusMsgArray.GetSize();
		// 		if(nCount>0)
		// 		{
		// 			CString strDispMsg;
		// 			for(int i=0; i < nCount; i++)
		// 			{
		// 				strDispMsg = m_strStatusMsgArray.GetAt(i);
		// 				m_strStatusMsgArray.RemoveAt(i);
		// 				m_wndStatusBar.SetPaneText(nStatusBarID,strDispMsg);
		// 			}
		// 		}
	}

	m_cs.Unlock();
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message != WM_KEYDOWN)
		return CFrameWnd::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		case 'S':
		case 's':
			if (GetKeyState(VK_CONTROL) < 0) // Ctrl 키가 눌려진 상태
			{
				WINDOWPLACEMENT wndPlace;
				AfxGetMainWnd()->GetWindowPlacement(&wndPlace);
				wndPlace.showCmd |= SW_MAXIMIZE;
				AfxGetMainWnd()->SetWindowPlacement(&wndPlace);
			}
			break;
		}
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
#ifdef USE_VISION
	//if (pView->m_pVision[1])
	//{
	//	delete pView->m_pVision[1];
	//	pView->m_pVision[1] = NULL;
	//}

	//if (pView->m_pVision[0])
	//{
	//	delete pView->m_pVision[0];
	//	pView->m_pVision[0] = NULL;
	//}
#endif
}


BOOL CMainFrame::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
#ifdef USE_VISION
	//if (pView->m_pVision[1])
	//{
	//	delete pView->m_pVision[1];
	//	pView->m_pVision[1] = NULL;
	//}

	//if (pView->m_pVision[0])
	//{
	//	delete pView->m_pVision[0];
	//	pView->m_pVision[0] = NULL;
	//}
#endif
	pView->DestroyView();

	return CFrameWnd::DestroyWindow();
}


void CMainFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
#ifdef USE_VISION
	//if (pView->m_pVision[1])
	//{
	//	delete pView->m_pVision[1];
	//	pView->m_pVision[1] = NULL;
	//}

	//if (pView->m_pVision[0])
	//{
	//	delete pView->m_pVision[0];
	//	pView->m_pVision[0] = NULL;
	//}
#endif
	CFrameWnd::OnClose();
}


void CMainFrame::OnDatabaseConnection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pView->DispDatabaseConnection();
}

void CMainFrame::OnAppMinimize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	WINDOWPLACEMENT wndPlace;
	GetWindowPlacement(&wndPlace);
	wndPlace.showCmd |= SW_MINIMIZE;
	SetWindowPlacement(&wndPlace);
}
