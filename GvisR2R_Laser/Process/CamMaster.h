#if !defined(AFX_CAMMASTER_H__0311C7CE_23AD_4FEF_9FEF_093106E40668__INCLUDED_)
#define AFX_CAMMASTER_H__0311C7CE_23AD_4FEF_9FEF_093106E40668__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamMaster.h : header file
//

#include "CamMasterDefine.h"
#include "PcsRgn.h"
#include "../Global/GlobalDefine.h"

#define RGN_STRIP_VARIABLE_NUM	23		// REGION_STRIP ����ü���� �迭�� ������ ������ ����
#define MAX_NodeNumX			200
#define MAX_NodeNumY			200
#define MAX_NMSWATH				100

#define MAX_PCS					10000
#ifndef MAX_STRIP
	#define MAX_STRIP				4
#endif

#define MAX_PIECE_NODE_X_NUM	600		// 130319 jsy edit
#define MAX_PIECE_NODE_Y_NUM	600

#define MAX_FRAME_RGN_NUM		1000		// Maximum number of Strip regions
#define MAX_STRIP_X_NUM			10			//2012.08.07 hyk
#define MAX_STRIP_Y_NUM			10			//2012.08.07 hyk
#define MAX_PIECE_RGN_NUM		(MAX_PIECE_NODE_X_NUM * MAX_PIECE_NODE_Y_NUM)		// Maximum number of Cell regions

/////////////////////////////////////////////////////////////////////////////
// CCamMaster window

class CCamMaster : public CWnd
{
	_MasterPanel MstPnl;

	short FrameRgnNum;									// Panel�� �� ��Ʈ�� ��
	int PieceRgnNum;									// Panel�� �� �ǽ� ��
	int m_nCornerNum;									// �ǽ��� ������ ��
	int m_nDummy[MAX_PATH];
	int m_nPieceNum[MAX_STRIP];							// Strip�� �� �ǽ� ��
	REGIONS_FRAME FrameRgnPix[MAX_FRAME_RGN_NUM];
	REGIONS_FRAME_ID FrameRgnID[MAX_FRAME_RGN_NUM];
	REGIONS_PIECE_2 PieceRgnPix[MAX_PIECE_RGN_NUM];
//	REGIONS_PIECE_ID PieceRgnID[MAX_PIECE_RGN_NUM];
	CPoint	**PolygonPoints;

	void AllocPolygonRgnData();
	void FreePolygonRgnData();

	CString m_sPathCamSpecDir, m_sModel, m_sLayer, m_sLayerUp;
	long m_PinFileSize, m_PcsFileSize, m_AlignFileSize[4];

	BOOL LoadMasterSpec();
	void LoadPinImg();
	void LoadAlignImg();
 	CString GetCamPxlRes();
	BOOL LoadStripRgnFromCam();
	BOOL LoadPcsRgnFromCam();
	BOOL LoadStripPieceRegion_Binary();
	//void LoadPcsImg(CString sPath);
	void LoadCadImg();
	BOOL LoadCadMk();
	BOOL LoadCadMk2PntAlign(CString sPath);
	BOOL LoadCadMk4PntAlign(CString sPath);
	void LoadPcsImg();

	BOOL PinImgBufAlloc(TCHAR *strCADImg, BOOL bOppLayerF);
	void PinImgFree();
	void DeleteFileInFolder(CString sPathDir);
	int CheckPath(CString strPath);
	void PcsImgFree();
	BOOL PcsImgBufAlloc(TCHAR *strCADImg, BOOL bOppLayerF);
	void CADLinkImgFree();
	BOOL CADImgBufAlloc(TCHAR *strCADImg, int CellNum, BOOL bOppLayerF);
	void AlignImgFree(int nPos=-1); // -1 : All
	BOOL AlignImgBufAlloc(TCHAR *strCADImg, int nPos);


// Construction
public:
	CCamMaster();

// Attributes
public:
	BOOL m_bUse;

	stMasterInfo MasterInfo;
	REGION_STRIP* m_pCellRgn;
	CPcsRgn* m_pPcsRgn;
	UCHAR *m_pPinImg, *m_pPcsImg, *m_pAlignImg[4];

	UCHAR *m_pCADCellImg[MAX_CELL_NUM];
	long m_CADFileSize[MAX_CELL_NUM];
	int   CellLoadID[MAX_CELL_NUM];
	int   CellInspID[MAX_CELL_NUM];

	int m_lPcsNum;									// ķ�����Ϳ��� ������ ��ŷ ���ؽ��� ��.
	//long m_lPcsNum;									// ķ�����Ϳ��� ������ ��ŷ ���ؽ��� ��.
	stAlignMark m_stAlignMk;						// ķ�����Ϳ��� ������ 2point ���Ŀ��� �̹��� ��ǥ.
	stAlignMark2 m_stAlignMk2;						// ķ�����Ϳ��� ������ 4point ���Ŀ��� �̹��� ��ǥ.

	stPieceMark m_stPcsMk[MAX_PCS];					// ķ�����Ϳ��� ������ ��ŷ �ε��� ��ǥ.
	stPieceMark m_stPcsMkAdj[2][MAX_PCS];			// 2Point ��������� ������ ��ǥ[��/��].


// Operations
public:
	void Init(CString sPathSpec, CString sModel, CString sLayer, CString sLayerUp=_T(""));
	BOOL LoadMstInfo();
	BOOL WriteStripPieceRegion_Text(CString sBasePath, CString sLot);

	void SetMasterPanelInfo(); // for DTS
	BOOL LoadCadAlignMkPos();
	BOOL LoadCad2PntAlignMkPos(CString sPath);
	BOOL LoadCad4PntAlignMkPos(CString sPath);
	void SetCad2PntAlignMkPos();
	void SetCad4PntAlignMkPos();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamMaster)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCamMaster();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCamMaster)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMMASTER_H__0311C7CE_23AD_4FEF_9FEF_093106E40668__INCLUDED_)
