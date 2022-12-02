
// MainFrm.cpp : CMainFrame Ŭ������ ����
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
	//ID_SEPARATOR,           // ���� �� ǥ�ñ�
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	pFrm = this;
	m_bProgressCreated = FALSE; // status bar�� Progress bar control ���� �÷��� �ʱ�ȭ 

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
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	CRect rect;
	this->GetWindowRect(&rect);  //get client rect... 	
	int nScreenWidth = 1230;

	// Panel ũ������ 
	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 4 / 120);
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 32 / 120);
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 5 / 120);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 52 / 120);
	m_wndStatusBar.SetPaneInfo(4, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 5 / 120);
	m_wndStatusBar.SetPaneInfo(5, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 5 / 120);
	m_wndStatusBar.SetPaneInfo(6, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 5 / 120);
	m_wndStatusBar.SetPaneInfo(7, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 12 / 120);

	//CreateProgressBar(); // statusbar�� Progress bar control ����
	//m_Progress.SetPos(0);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	CGvisR2R_LaserApp *app = (CGvisR2R_LaserApp *)AfxGetApp();
	cs.lpszClass = app->m_singleInstance.GetClassName();

	cs.style = WS_VISIBLE | WS_POPUP;

	return TRUE;
}

// CMainFrame ����

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


// CMainFrame �޽��� ó����


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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
			if (GetKeyState(VK_CONTROL) < 0) // Ctrl Ű�� ������ ����
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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

void CMainFrame::CreateProgressBar()
{
	if (m_bProgressCreated == FALSE)
	{
		RECT MyRect;
		m_ProgressID = m_wndStatusBar.CommandToIndex(ID_INDICATOR_PROGRESS_PANE);

		m_wndStatusBar.GetItemRect(m_ProgressID, &MyRect);

		//Create the progress control
		m_Progress.Create(WS_VISIBLE | WS_CHILD, MyRect, &m_wndStatusBar, 1);

		m_Progress.SetRange(0, 100); //Set the range to between 0 and 100
		m_Progress.SetStep(1); // Set the step amount
		m_bProgressCreated = TRUE;
	}
}

void CMainFrame::IncProgress(int nVal)
{
	// 	return;

	if (nVal == 0)
		m_Progress.StepIt();
	else
		m_Progress.SetPos(nVal);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	if (m_bProgressCreated)
	{
		RECT rc;
		m_wndStatusBar.GetItemRect(m_ProgressID, &rc);
		// Reposition the progress control correctly!
		m_Progress.SetWindowPos(&wndTop, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 0);
	} 
}



void CMainFrame::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	pView->DispDatabaseConnection();
}

void CMainFrame::OnAppMinimize()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	WINDOWPLACEMENT wndPlace;
	GetWindowPlacement(&wndPlace);
	wndPlace.showCmd |= SW_MINIMIZE;
	SetWindowPlacement(&wndPlace);
}
