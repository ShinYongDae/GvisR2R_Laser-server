// ReelMap.cpp : implementation file
//

#include "stdafx.h"
#include "ReelMap.h"
#include "DataFile.h"
#include "../Global/GlobalFunc.h"
#include "../Dialog/DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MyFile.h"
#include "../GvisR2R_LaserDoc.h"
#include "../GvisR2R_LaserView.h"

extern CGvisR2R_LaserDoc* pDoc;
extern CGvisR2R_LaserView* pView;

/////////////////////////////////////////////////////////////////////////////
// CReelMap

CReelMap::CReelMap(int nLayer, int nPnl, int nPcs, int nDir)
{
	m_nLayer = -1;
	nTotPnl = nPnl;
	nTotPcs = nPcs;
	nDir = ROT_NONE;

	m_dTotLen = 0.0;
	m_bUseLotSep = FALSE;
	m_bContFixDef = FALSE;
	m_dAdjRatio = 1.0;
	m_dTotLen = 0.0;
	m_dPnlLen = 0.0;
	m_dLotLen = 0.0;
	m_dTempPauseLen = 0.0;
	m_dLotCutPosLen = 0.0;
	m_nSerial = 0;
	m_nLastShot = 0;
	m_nCompletedShot = 0;

	//m_nCntFixPcs = 0;
	m_nPrevSerial[0] = 0;	// --
	m_nPrevSerial[1] = 0;	// ++

	m_rgbDef[DEF_NONE]=(RGB_WHITE); // (RGB_GREEN)
	m_rgbDef[DEF_NICK]=(RGB_MAGENTA);
	m_rgbDef[DEF_PROTRUSION]=(RGB_SKYBLUE);
	m_rgbDef[DEF_SPACE]=(RGB_LTGREEN);
	m_rgbDef[DEF_OPEN]=(RGB_LTRED);
	m_rgbDef[DEF_SHORT]=(RGB_RED);
	m_rgbDef[DEF_USHORT]=(RGB_LTCYAN);
	m_rgbDef[DEF_PINHOLE]=(RGB_LLTGREEN);
	m_rgbDef[DEF_HOLE_MISS]=(RGB_LTBLUE);
	m_rgbDef[DEF_EXTRA]=(RGB_CLOUDBLUE);
	m_rgbDef[DEF_PAD]=(RGB_LTPURPLE);
	m_rgbDef[DEF_HOLE_POSITION]=(RGB_PINK);
	m_rgbDef[DEF_POI]=(RGB_LTMAGENTA);
	m_rgbDef[DEF_VH_POSITION]=(RGB_LTYELLOW);
	m_rgbDef[DEF_VH_MISS]=(RGB_BOON);
	m_rgbDef[DEF_HOLE_DEFECT]=(RGB_LTPINK);
	m_rgbDef[DEF_HOLE_OPEN]=(RGB_YELLOWGREEN);
	m_rgbDef[DEF_VH_OPEN]=(RGB_RED);
	m_rgbDef[DEF_VH_DEF]=(RGB_BROWN);
	m_rgbDef[DEF_LIGHT]=(RGB_YELLOW);

	m_cBigDef[0] = '*';		//	NONE
	m_cBigDef[1] = 'N';		//	NICK
	m_cBigDef[2] = 'D';		//	PROTRUSION
	m_cBigDef[3] = 'A';		//	SPACE
	m_cBigDef[4] = 'O';		//	OPEN
	m_cBigDef[5] = 'S';		//	SHORT
	m_cBigDef[6] = 'U';		//	USHORT
	m_cBigDef[7] = 'I';		//	PINHOLE
	m_cBigDef[8] = 'H';		//	HOLE_MISS
	m_cBigDef[9] = 'E';		//	EXTRA
	m_cBigDef[10] = 'P';	//	PAD
	m_cBigDef[11] = 'L';	//	HOLE_POSITION
	m_cBigDef[12] = 'X';	//	POI
	m_cBigDef[13] = 'T';	//	VH_POSITION
	m_cBigDef[14] = 'M';	//	VH_MISS
	m_cBigDef[15] = 'F';	//	HOLE_DEFECT
	m_cBigDef[16] = 'C';	//	HOLE_OPEN
	m_cBigDef[17] = 'G';	//	VH_OPEN
	m_cBigDef[18] = 'V';	//	VH_DEF

	m_cSmallDef[0] = '*';
	m_cSmallDef[1] = 'n';
	m_cSmallDef[2] = 'd';
	m_cSmallDef[3] = 'a';
	m_cSmallDef[4] = 'o';
	m_cSmallDef[5] = 's';
	m_cSmallDef[6] = 'u';
	m_cSmallDef[7] = 'i';
	m_cSmallDef[8] = 'h';
	m_cSmallDef[9] = 'e';
	m_cSmallDef[10] = 'p';
	m_cSmallDef[11] = 'l';
	m_cSmallDef[12] = 'x';
	m_cSmallDef[13] = 't';
	m_cSmallDef[14] = 'm';
	m_cSmallDef[15] = 'f';
	m_cSmallDef[16] = 'c';
	m_cSmallDef[17] = 'g';
	m_cSmallDef[18] = 'v';

	m_pPnlNum = NULL;
	m_pPnlDefNum = NULL;
	pPcsDef = NULL;
	pFrmRgn = NULL;
	pPcsRgn = NULL;
// 	pMkInfo = NULL;

	m_pPnlBuf = NULL;
	m_nPnlBuf = 0;

	if(nPnl>0 && nPcs>0)
	{
		pFrmRgn = new CRect[nPnl];
		m_pPnlNum = new int[nPnl];
		m_pPnlDefNum = new int[nPnl];
	}

// 	int k, i;
// 	if(nPnl>0 && nPcs>0)
// 	{
// 		pPcsDef = new int*[nPnl];
// 		pFrmRgn = new CRect[nPnl];
// 		pPcsRgn = new CRect*[nPnl];
// 		m_pPnlNum = new int[nPnl];
// 
// 		for(k=0; k<nPnl; k++)
// 		{
// 			pPcsDef[k] = new int[nPcs];
// 			pPcsRgn[k] = new CRect[nPcs];
// 			for(i=0; i<nPcs; i++)
// 				pPcsDef[k][i] = DEF_NONE;
// 		}
// 	}
// 
// 	pMkInfo = new CString[nPcs];

	m_bThreadAliveRemakeReelmap = FALSE;
	m_sPathOnThread = _T("");
	
	m_bThreadAliveReloadRst = FALSE;
	m_nLastOnThread = 0;
	m_nTotalProgressReloadRst = 0;
	m_nProgressReloadRst = 0;
	m_bDoneReloadRst = FALSE;

	m_nSelMarkingPnl = 2;
	m_nWritedSerial = 0;

	m_nLayer = nLayer;
	m_sPathBuf = GetRmapPath(m_nLayer);
	m_sPathYield = GetYieldPath(m_nLayer);


	LoadConfig();
 	InitRst();
	InitPcs();
	ClrPnlNum();	
	ClrPnlDefNum();
	ClrFixPcs();

	m_nStartSerial = 0;

	// ITS
	//m_pPnlBufIts = NULL;
	//m_nPnlBufIts = 0;

	//ResetYield();

}

CReelMap::~CReelMap()
{
	int k;

	CloseRst();

// 	if(pMkInfo)
// 	{
// 		delete[] pMkInfo;
// 		pMkInfo = NULL;
// 	}

	if(m_pPnlNum)
	{
		delete[] m_pPnlNum;
		m_pPnlNum = NULL;
	}

	if(m_pPnlDefNum)
	{
		delete[] m_pPnlDefNum;
		m_pPnlDefNum = NULL;
	}

	if(pFrmRgn)
	{
		delete[] pFrmRgn;
		pFrmRgn = NULL;
	}

	if(pPcsRgn)
	{
		for(k=0; k<nTotPnl; k++)
		{
			delete[] pPcsRgn[k];
			pPcsRgn[k] = NULL;
		}
		delete[] pPcsRgn;
		pPcsRgn = NULL;
	}

	if(pPcsDef)
	{
		for(k=0; k<nTotPnl; k++)
		{
			delete[] pPcsDef[k];
			pPcsDef[k] = NULL;
		}

		delete[] pPcsDef;
		pPcsDef = NULL;
	}
}


BEGIN_MESSAGE_MAP(CReelMap, CWnd)
	//{{AFX_MSG_MAP(CReelMap)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CReelMap message handlers
void CReelMap::LoadConfig()
{
	TCHAR szData[MAX_PATH];
	TCHAR sep[] = { _T(",;\r\n\t") };
	CString sVal;

	if (0 < ::GetPrivateProfileString(_T("REELMAP"), _T("BackGround"), NULL, szData, sizeof(szData), PATH_CONFIG))
	{
		sVal = _tcstok(szData, sep);
		m_nBkColor[0] = _tstoi(sVal);
		sVal = _tcstok(NULL,sep);
		m_nBkColor[1] = _tstoi(sVal);
		sVal = _tcstok(NULL,sep);
		m_nBkColor[2] = _tstoi(sVal);
	}

	//if (pView && pView->m_pDts)
	//{
	//	if (pView->m_pDts->IsUseDts())
	//	{
	//		LoadDefectTableDB();
	//		return;
	//	}
	//	else
	//	{
	//		LoadDefectTableIni();
	//	}
	//}
	//else
	{
		LoadDefectTableIni();
	}
}

//BOOL CReelMap::LoadDefectTableDB()
//{
//	if (pView && pView->m_pDts)
//	{
//		COLORREF rgbDef[MAX_PATH];
//		int nDefCode[MAX_PATH], nMaxR, nMaxC;
//		CString sEngN[MAX_PATH], sKorN[MAX_PATH];
//
//		return pView->m_pDts->LoadDefectTable(nDefCode, rgbDef, sKorN, sEngN, &nMaxR, &nMaxC);
//	}
//
//	return FALSE;
//}

BOOL CReelMap::LoadDefectTableIni()
{
	TCHAR szData[200];
	TCHAR sep[] = { _T(",;\r\n\t") };
	CString sIdx, sVal;
	int k;

	for(k=1; k < MAX_DEF; k++)
	{
		sIdx.Format(_T("%d"), k);
		if (0 < ::GetPrivateProfileString(_T("DEFECT"), sIdx, NULL, szData, sizeof(szData), PATH_CONFIG))
		{
			sVal = _tcstok(szData, sep);
			m_sEngDef[k].Format(_T("%s"), sVal);
			sVal = _tcstok(NULL,sep);
			m_sKorDef[k].Format(_T("%s"), sVal);
			sVal = _tcstok(NULL,sep);
			m_cBigDef[k] = sVal.GetAt(0);
			sVal = _tcstok(NULL,sep);
			m_cSmallDef[k] = sVal.GetAt(0);
			sVal = _tcstok(NULL,sep);
			m_rgbDef[k] = (COLORREF)_tstoi(sVal);
			sVal = _tcstok(NULL,sep);
			m_nOdr[k] = _tstoi(sVal);
		}
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - LoadDefectTableIni()"));
			return FALSE;
		}
	}

	return TRUE;
}

void CReelMap::SetRgbDef(int nDef, COLORREF rgbVal)
{
	m_rgbDef[nDef]=rgbVal;
}

void CReelMap::SetPnlNum(int *pPnlNum)
{
	if(!m_pPnlNum)
		return;

	int k;
	if(pPnlNum)
	{
		for(k=0; k<nTotPnl; k++)
			m_pPnlNum[k]=pPnlNum[k];
	}
	else
	{
		for(k=0; k<nTotPnl; k++)
			m_pPnlNum[k]=-1;
	}
}

void CReelMap::SetPnlDefNum(int *pPnlDefNum)
{
	if(!m_pPnlDefNum)
		return;

	int k;
	if(pPnlDefNum)
	{
		for(k=0; k<nTotPnl; k++)
			m_pPnlDefNum[k]=pPnlDefNum[k];
	}
	else
	{
		for(k=0; k<nTotPnl; k++)
			m_pPnlDefNum[k]=-1;
	}
}

void CReelMap::ClrPnlNum()
{
	for(int k=0; k<nTotPnl; k++)
		m_pPnlNum[k]=-1;
}

void CReelMap::ClrPnlDefNum()
{
	for(int k=0; k<nTotPnl; k++)
		m_pPnlDefNum[k]=-1;
}

// Panel Index k increases from 0 to (nTotPnl-1)...
void CReelMap::IncPnlNum()
{
	if(!m_pPnlNum)
		return;

	int k;
	for(k=nTotPnl-1; k>0; k--)
		m_pPnlNum[k] = m_pPnlNum[k-1];
	m_pPnlNum[0]++;
}

void CReelMap::AddPnlDefNum(int nDef)
{
	if(!m_pPnlDefNum)
		return;

	int k;
	for(k=nTotPnl-1; k>0; k--)
		m_pPnlDefNum[k] = m_pPnlDefNum[k-1];
	m_pPnlDefNum[0] = nDef;
}

void CReelMap::AddPnlNum(int nNum)
{
	if(!m_pPnlNum)
		return;

	int k;
	for(k=nTotPnl-1; k>0; k--)
		m_pPnlNum[k] = m_pPnlNum[k-1];
	m_pPnlNum[0] = nNum;
}

void CReelMap::SelMarkingPnl(int nNum)
{
	m_nSelMarkingPnl = nNum;
}

void CReelMap::SetAdjRatio(double dRatio)
{
	if(dRatio < 1.0)
		dRatio = 1.0;

	m_dAdjRatio = dRatio;
}

double CReelMap::GetAdjRatio()
{
	return m_dAdjRatio;
}

BOOL CReelMap::Open(CString sPath)
{
	if (pDoc->GetTestMode() == MODE_OUTER)
		MakeItsReelmapHeader();
		//pDoc->MakeLayerMappingHeader();

	return MakeHeader(sPath);
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//	int nNodeX = 0, nNodeY = 0;
//#ifndef TEST_MODE
//	nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
//	nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
//#endif
//
//	int k, i;
//	FILE *fp = NULL;
//	char FileName[MAX_PATH];
//
//	BOOL bExist = FALSE;
//	CFileFind findfile;
//	if (findfile.FindFile(sPath))
//	{
//		bExist = TRUE;
//		return TRUE;
//	}
//	else
//		MakeDirRmap();
//
//	StrToChar(sPath, FileName);
//
//	fp = fopen(FileName, "a+");
//	if (fp == NULL)
//	{
//		pView->MsgBox(_T("It is trouble to open ReelMap.txt"));
//// 		AfxMessageBox(_T("It is trouble to open ReelMap.txt"),MB_ICONWARNING|MB_OK);
//		return FALSE; 
//	}
//
//	//m_sPathShare.Format(_T("%s"), sPath);
//
//	char* pRtn = NULL;
//	if(!bExist)
//	{
//		InitRst();
//		InitPcs();
//
//		fprintf(fp, "[Info]\n");
//		fprintf(fp, "설  비  명 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "운  용  자 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "모      델 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sModelUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "로      트 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLotUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "상면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		if (bDualTest)
//		{
//			fprintf(fp, "하면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerDn)); if (pRtn) delete pRtn; pRtn = NULL;
//		}
//		fprintf(fp, "\n");
//		fprintf(fp, "Process Code = \n");
//		fprintf(fp, "Entire Speed = \n");
//		fprintf(fp, "\n");
//
//		fprintf(fp, "CamPcsX = %d\n", nNodeX);
//		fprintf(fp, "CamPcsY = %d\n", nNodeY);
//		
//		fprintf(fp, "\n");
//		fprintf(fp, "0 -> 양품\n");
//		for (i = 1; i < MAX_DEF; i++)
//		{
//			fprintf(fp, "%d -> %s\n", i, pRtn = StrToChar(m_sKorDef[i])); if (pRtn) delete pRtn; pRtn = NULL; // m_cBigDef[i]
//		}
//		fprintf(fp, "? - Missed Align Panel(i.e 노광불량)\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Lot Start=\n");
//		fprintf(fp, "Lot Run=\n");
//		fprintf(fp, "Lot End=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Marked Shot=\n");
//		fprintf(fp, "Marked Date=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "// < 수율 정보 > \n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Start Serial=\n");
//		fprintf(fp, "End Serial=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Total Pcs=\n");
//		fprintf(fp, "Good Pcs=\n");
//		fprintf(fp, "Bad Pcs=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Strip0=\n");
//		fprintf(fp, "Strip1=\n");
//		fprintf(fp, "Strip2=\n");
//		fprintf(fp, "Strip3=\n");
//		for(i=1; i<MAX_DEF; i++)
//			fprintf(fp, "%d=\n", i); // m_cBigDef[i]
//		fprintf(fp, "\n");
//
//		for(k=0; k<MAX_STRIP_NUM; k++)
// 	{
//			fprintf(fp, "[Strip%d]\n", k);
//			for(i=1; i<MAX_DEF; i++)
//				fprintf(fp, "%d=\n", i); // m_cBigDef[i]
//			fprintf(fp, "\n");
// 	}
//
//		fprintf(fp, "[StripOut]\n");
//		fprintf(fp, "Total=\n");
//		for(k=0; k<MAX_STRIP_NUM; k++)
//			fprintf(fp, "%d=\n", k);
//		fprintf(fp, "\n");
//	}
//
//	fclose(fp);
//
//
//	return TRUE;
}

BOOL CReelMap::OpenUser(CString sPath)
{
	if (pDoc->GetTestMode() == MODE_OUTER)
		MakeItsReelmapHeader();

	//	pDoc->MakeLayerMappingHeader();

	return MakeHeader(sPath);
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//
//	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
//	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
//
//	int k, i;
//	FILE *fp = NULL;
//	char FileName[MAX_PATH];
//
//
//	CString sFile=_T(""), sUserRmapPath=sPath;
//	int nPos = sUserRmapPath.ReverseFind('\\');
//	if (nPos != -1)
//	{
//		sFile = sUserRmapPath.Right(sUserRmapPath.GetLength()-nPos-1);
//		sUserRmapPath.Delete(nPos, sPath.GetLength() - nPos);
//	}
//
//	BOOL bExist = FALSE;
//	CFileFind findfile;
//	sPath = sUserRmapPath + _T("\\Reelmap");
//	if(findfile.FindFile(sPath))
//		bExist = TRUE;
//	else
//		MakeDirRmap();
//
//// 	if(bExist)
//// 	{
//// 		char szData[100];
//// 		if (0 >= ::GetPrivateProfileString(_T("Info"), _T("모      델"), NULL, szData, sizeof(szData), sPath))
//// 			bExist = FALSE;
//// 	}
//
//	sPath = sUserRmapPath + _T("\\Reelmap\\") + sFile;
//	//_tcscpy(FileName, sPath);
//	StrToChar(sPath, FileName);
//
//
//	fp = fopen(FileName, "a+");
//	if (fp == NULL)
//	{
//		pView->MsgBox(_T("It is trouble to open ReelMap.txt"));
//		return FALSE; 
//	}
//	char* pRtn = NULL;
//	if(!bExist)
//	{
//		InitRst();
//		InitPcs();
//
//		fprintf(fp, "[Info]\n");
//		fprintf(fp, "설  비  명 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "운  용  자 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "모      델 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sModelUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "로      트 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLotUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "상면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		if (bDualTest)
//		{
//			fprintf(fp, "하면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerDn)); if (pRtn) delete pRtn; pRtn = NULL;
//		}
//		fprintf(fp, "\n");
//		fprintf(fp, "Process Code = \n");
//		fprintf(fp, "Entire Speed = \n");
//		fprintf(fp, "양폐 스트립 수율[%%] : %.1f\n", _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio));
//		fprintf(fp, "\n");
//		
//		fprintf(fp, "CamPcsX = %d\n", nNodeX);
//		fprintf(fp, "CamPcsY = %d\n", nNodeY);
//		
//		fprintf(fp, "\n");
//		fprintf(fp, "0 -> 양품\n");
//		for (i = 1; i < MAX_DEF; i++)
//		{
//			fprintf(fp, "%c -> %s\n", pDoc->m_cBigDefCode[i], pRtn = StrToChar(m_sKorDef[i])); if (pRtn) delete pRtn; pRtn = NULL; // m_cBigDef[i]
//		}
//		fprintf(fp, "? - Missed Align Panel(i.e 노광불량)\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Lot Start=\n");
//		fprintf(fp, "Lot Run=\n");
//		fprintf(fp, "Lot End=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Marked Shot=\n");
//		fprintf(fp, "Marked Date=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "// < 수율 정보 > \n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Start Serial=\n");
//		fprintf(fp, "End Serial=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Total Pcs=\n");
//		fprintf(fp, "Good Pcs=\n");
//		fprintf(fp, "Bad Pcs=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Strip0=\n");
//		fprintf(fp, "Strip1=\n");
//		fprintf(fp, "Strip2=\n");
//		fprintf(fp, "Strip3=\n");
//		for(i=1; i<MAX_DEF; i++)
//			fprintf(fp, "%d=\n", i); // m_cBigDef[i]
//		fprintf(fp, "\n");
//
//		for(k=0; k<MAX_STRIP_NUM; k++)
// 	{
//			fprintf(fp, "[Strip%d]\n", k);
//			for(i=1; i<MAX_DEF; i++)
//				fprintf(fp, "%d=\n", i); // m_cBigDef[i]
//			fprintf(fp, "\n");
//		}
// 	}
//
//	fclose(fp);
//
//	return TRUE;
}

BOOL CReelMap::Open()
{
	CString sPath = GetRmapPath(m_nLayer);
	//m_sPathShare = sPath;
	if (pDoc->GetTestMode() == MODE_OUTER)
		MakeItsReelmapHeader();

	return MakeHeader(sPath);
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//
//	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
//	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
//
//	int k, i;
//	FILE *fp = NULL;
//	char FileName[MAX_PATH];
//
//	BOOL bExist = FALSE;
//	CFileFind findfile;
//	if(findfile.FindFile(sPath))
//		bExist = TRUE;
//	else
//		MakeDirRmap(sModel, sLayer, sLot);
//
//// 	if(bExist)
//// 	{
//// 		char szData[100];
//// 		if (0 >= ::GetPrivateProfileString(_T("Info"), _T("모      델"), NULL, szData, sizeof(szData), sPath))
//// 			bExist = FALSE;
//// 	}
//
//	//_tcscpy(FileName, sPath);
//	StrToChar(sPath, FileName);
//
//	fp = fopen(FileName, "a+");
//	if (fp == NULL)
//	{
//		pView->MsgBox(_T("It is trouble to open ReelMap.txt"));
//// 		AfxMessageBox(_T("It is trouble to open ReelMap.txt"),MB_ICONWARNING|MB_OK);
//		return FALSE; 
//	}
//
//	m_sPathShare = sPath;
//	char* pRtn = NULL;
//	if(!bExist)
//	{
//		InitRst();
//		InitPcs();
//
//		fprintf(fp, "[Info]\n");
//		fprintf(fp, "설  비  명 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "운  용  자 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "모      델 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sModelUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "로      트 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLotUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "상면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		if (bDualTest)
//		{
//			fprintf(fp, "하면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerDn)); if (pRtn) delete pRtn; pRtn = NULL;
//		}
//		fprintf(fp, "\n");
//		fprintf(fp, "Process Code = \n");
//		fprintf(fp, "Entire Speed = \n");
//		fprintf(fp, "\n");
//		
//		fprintf(fp, "CamPcsX = %d\n", nNodeX);
//		fprintf(fp, "CamPcsY = %d\n", nNodeY);
//		
//		fprintf(fp, "\n");
//		fprintf(fp, "0 -> 양품\n");
//		for (i = 1; i < MAX_DEF; i++)
//		{
//			fprintf(fp, "%d -> %s\n", i, pRtn = StrToChar(m_sKorDef[i])); if (pRtn) delete pRtn; pRtn = NULL; // m_cBigDef[i]
//		}
//		fprintf(fp, "? - Missed Align Panel(i.e 노광불량)\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Lot Start=\n");
//		fprintf(fp, "Lot Run=\n");
//		fprintf(fp, "Lot End=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Marked Shot=\n");
//		fprintf(fp, "Marked Date=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "// < 수율 정보 > \n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Start Serial=\n");
//		fprintf(fp, "End Serial=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Total Pcs=\n");
//		fprintf(fp, "Good Pcs=\n");
//		fprintf(fp, "Bad Pcs=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Strip0=\n");
//		fprintf(fp, "Strip1=\n");
//		fprintf(fp, "Strip2=\n");
//		fprintf(fp, "Strip3=\n");
//		for(i=1; i<MAX_DEF; i++)
//			fprintf(fp, "%d=\n", i); // m_cBigDef[i]
//		fprintf(fp, "\n");
//
//		for(k=0; k<MAX_STRIP_NUM; k++)
// 	{
//			fprintf(fp, "[Strip%d]\n", k);
//			for(i=1; i<MAX_DEF; i++)
//				fprintf(fp, "%d=\n", i); // m_cBigDef[i]
//			fprintf(fp, "\n");
// 	}
//	}
//
//	fclose(fp);
//// 	if(!OpenUser(sPath, sModel, sLayer, sLot))
//// 		return FALSE;
//
//	return TRUE;
}

BOOL CReelMap::OpenUser(CString sPath, CString sModel, CString sLayer, CString sLot)
{
	if (pDoc->GetTestMode() == MODE_OUTER)
		MakeItsReelmapHeader();

	return MakeHeader(sPath);
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//
//	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
//	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
//
//	int k, i;
//	FILE *fp = NULL;
//	char FileName[MAX_PATH];
//
//	CString sFile=_T(""), sUserRmapPath=sPath;
//	int nPos = sUserRmapPath.ReverseFind('\\');
//	if (nPos != -1)
//	{
//		sFile = sUserRmapPath.Right(sUserRmapPath.GetLength()-nPos-1);
//		sUserRmapPath.Delete(nPos, sPath.GetLength() - nPos);
//	}
//
//	BOOL bExist = FALSE;
//	CFileFind findfile;
//	sPath = sUserRmapPath + _T("\\Reelmap");
//
//	if(findfile.FindFile(sPath))
//		bExist = TRUE;
//	else
//		MakeDirRmap(sModel, sLayer, sLot);
//
//// 	if(bExist)
//// 	{
//// 		char szData[100];
//// 		if (0 >= ::GetPrivateProfileString(_T("Info"), _T("모      델"), NULL, szData, sizeof(szData), sPath))
//// 			bExist = FALSE;
//// 	}
//
//	sPath = sUserRmapPath + _T("\\Reelmap\\") + sFile;
//	//_tcscpy(FileName, sPath);
//	StrToChar(sPath, FileName);
//
//	fp = fopen(FileName, "a+");
//	if (fp == NULL)
//	{
//		pView->MsgBox(_T("It is trouble to open ReelMap.txt"));
//		return FALSE; 
//	}
//	char* pRtn = NULL;
//	if(!bExist)
//	{
//		InitRst();
//		InitPcs();
//
//		fprintf(fp, "[Info]\n");
//		fprintf(fp, "설  비  명 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "운  용  자 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "모      델 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sModelUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "로      트 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLotUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "상면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		if (bDualTest)
//		{
//			fprintf(fp, "하면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerDn)); if (pRtn) delete pRtn; pRtn = NULL;
//		}
//		fprintf(fp, "\n");
//		fprintf(fp, "Process Code = \n");
//		fprintf(fp, "Entire Speed = \n");
//		fprintf(fp, "양폐 스트립 수율[%%] : %.1f\n", _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio));
//		fprintf(fp, "\n");
//		
//		fprintf(fp, "CamPcsX = %d\n", nNodeX);
//		fprintf(fp, "CamPcsY = %d\n", nNodeY);
//		
//		fprintf(fp, "\n");
//		fprintf(fp, "0 -> 양품\n");
//		for (i = 1; i < MAX_DEF; i++)
//		{
//			fprintf(fp, "%c -> %s\n", pDoc->m_cBigDefCode[i], pRtn = StrToChar(m_sKorDef[i])); if (pRtn) delete pRtn; pRtn = NULL; // m_cBigDef[i]
//		}
//		fprintf(fp, "? - Missed Align Panel(i.e 노광불량)\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Lot Start=\n");
//		fprintf(fp, "Lot Run=\n");
//		fprintf(fp, "Lot End=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Marked Shot=\n");
//		fprintf(fp, "Marked Date=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "// < 수율 정보 > \n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Start Serial=\n");
//		fprintf(fp, "End Serial=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Total Pcs=\n");
//		fprintf(fp, "Good Pcs=\n");
//		fprintf(fp, "Bad Pcs=\n");
//		fprintf(fp, "\n");
//		fprintf(fp, "Strip0=\n");
//		fprintf(fp, "Strip1=\n");
//		fprintf(fp, "Strip2=\n");
//		fprintf(fp, "Strip3=\n");
//		for(i=1; i<MAX_DEF; i++)
//			fprintf(fp, "%d=\n", i); // m_cBigDef[i]
//		fprintf(fp, "\n");
//
//		for(k=0; k<MAX_STRIP_NUM; k++)
// 	{
//			fprintf(fp, "[Strip%d]\n", k);
//			for(i=1; i<MAX_DEF; i++)
//				fprintf(fp, "%d=\n", i); // m_cBigDef[i]
//			fprintf(fp, "\n");
// 	}
//	}
//
//	fclose(fp);
//
//	return TRUE;
}

int CReelMap::Read(CString &sRead)
{
	return 0;

	FILE *fp;
	char *FileData;
	char FileD[200];
	int nFileSize, nRSize, i;
	CString sMsg;

	//_tcscpy(FileD, m_sPathShare);
	StrToChar(m_sPathShare, FileD);

	if((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize, sizeof(char));

		nRSize = 0;
		for( i=0; i < nFileSize ; i++ )
		{
			if( feof( fp ) != 0 )
				break;
			FileData[i] = fgetc(fp);
			nRSize++;
		}

		sRead.Format(_T("%s"), CharToString(FileData));

		fclose(fp);
		free( FileData );
	}
	else
	{
		sMsg.Format(_T("릴맵파일을 찾지 못했습니다.\r\n%s"), m_sPathShare);
		pView->MsgBox(sMsg);
// 		AfxMessageBox(sMsg);
		return 0;
	}

	return nRSize;
}

BOOL CReelMap::Write(int nSerial, int nLayer)
{
	return TRUE;

	if(nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.62"));
		return 0;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
		MakeItsReelmapHeader();

	MakeHeader(m_sPathShare);

	if (pDoc->GetTestMode() == MODE_INNER)
		pDoc->SetItsSerialInfo(nSerial);

	int nIdx = pDoc->GetPcrIdx1(nSerial, pDoc->m_bNewLotShare[1]);
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nStripY = pDoc->m_Master[0].m_pPcsRgn->nRow / 4; // Strip(1~4);
	int nTotDefPcs = 0;

	if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
	{
		if (pDoc->m_pPcr[nLayer])
		{
			if (pDoc->m_pPcr[nLayer][nIdx])
			nTotDefPcs = pDoc->m_pPcr[nLayer][nIdx]->m_nTotDef;
	}
	}
	else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
	{
		if (pDoc->m_pPcrInner[nLayer])
		{
			if (pDoc->m_pPcrInner[nLayer][nIdx])
				nTotDefPcs = pDoc->m_pPcrInner[nLayer][nIdx]->m_nTotDef;
		}
	}
	else if (m_nLayer == RMAP_ITS)
	{
		if (pDoc->m_pPcrIts)
		{
			if (pDoc->m_pPcrIts[nIdx])
				nTotDefPcs = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
		}
	}
	else
		return 0;

	short **pPnlBuf;
	int i, nC, nR, nPcsId, nDefCode;//, nTot, nDef, nGood;
	pPnlBuf = new short*[nNodeY];
	for (i = 0; i < nNodeY; i++)
	{
		pPnlBuf[i] = new short[nNodeX];
		memset(pPnlBuf[i], 0, sizeof(short)*nNodeX);
		memset(m_pPnlBuf[nSerial - 1][i], 0, sizeof(short)*nNodeX);
	}

	CString strData, strTemp;

	for (i = 0; i < nTotDefPcs; i++)
	{
		if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
		{
			if (pDoc->m_pPcr[nLayer][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
			{
				if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
	{
					switch (pDoc->m_Master[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
		{
					case 0:
						nPcsId = pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i];
						break;
					case 1:
						nPcsId = pDoc->MirrorLR(pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					case 2:
						nPcsId = pDoc->MirrorUD(pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					case 3:
						nPcsId = pDoc->Rotate180(pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					default:
						nPcsId = pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i];
						break;
					}
				}
				else
			nPcsId = pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i];

			nDefCode = pDoc->m_pPcr[nLayer][nIdx]->m_pDefType[i];

			nC = int(nPcsId / nNodeY);
				if (nC % 2)	// 홀수.
					nR = nNodeY * (nC + 1) - nPcsId - 1;
				else		// 짝수.
					nR = nPcsId - nNodeY * nC;
				pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
				if (m_pPnlBuf)
					m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			}
		}
		else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
		{
			if (pDoc->m_pPcrInner[nLayer][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
			{
				if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
				{
					switch (pDoc->m_MasterInner[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
					{
					case 0:
						nPcsId = pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i];
						break;
					case 1:
						nPcsId = pDoc->MirrorLR(pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					case 2:
						nPcsId = pDoc->MirrorUD(pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					case 3:
						nPcsId = pDoc->Rotate180(pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					default:
						nPcsId = pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i];
						break;
					}
				}
				else
					nPcsId = pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i];

				nDefCode = pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefType[i];

				nC = int(nPcsId / nNodeY);
				if (nC % 2)	// 홀수.
					nR = nNodeY * (nC + 1) - nPcsId - 1;
				else		// 짝수.
					nR = nPcsId - nNodeY * nC;
				pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
				if (m_pPnlBuf)
					m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			}
		}
		else if (m_nLayer == RMAP_ITS)
		{
			if (pDoc->m_pPcrIts[nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
			{
				if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
				{
					switch (pDoc->m_Master[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
					{
					case 0:
						nPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i];
						break;
					case 1:
						nPcsId = pDoc->MirrorLR(pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i]);
						break;
					case 2:
						nPcsId = pDoc->MirrorUD(pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i]);
						break;
					case 3:
						nPcsId = pDoc->Rotate180(pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i]);
						break;
					default:
						nPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i];
						break;
					}
				}
				else
					nPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i];

				nDefCode = pDoc->m_pPcrIts[nIdx]->m_pDefType[i];

				nC = int(nPcsId / nNodeY);
				if (nC % 2)	// 홀수.
					nR = nNodeY * (nC + 1) - nPcsId - 1;
			else		// 짝수.
				nR = nPcsId - nNodeY * nC;
			pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
				if (m_pPnlBuf)
					m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
		}
	}
		else
			return 0;
	}

	CString sPnl, sRow;
	i = 0;
	sPnl.Format(_T("%d"), nSerial);
	strData.Format(_T("%d"), nTotDefPcs);
	::WritePrivateProfileString(sPnl, _T("Total Defects"), strData, m_sPathShare);

	for(int nRow=0; nRow<nNodeX; nRow++)			// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Row : Shot의 첫번째 Col부터 시작해서 밑으로 내려감.
	{
		sRow.Format(_T("%02d"), nRow);
// 		sRow.Format(_T("%2d"), nRow+1);
		strData.Format(_T(""));
		strTemp.Format(_T(""));

// 		if(nRow==26)
// 			int brk=0;

		for(int nCol=0; nCol<nNodeY; nCol++)		// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Col : 4열 3열 2열 1열 스트립으로 표시됨.
		{
			nR = (nNodeY-1)-nCol;				// 릴맵상의 Row
			nC = nRow;							// 릴맵상의 Col

			if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
			{
				if (pDoc->m_pPcr[nLayer][nIdx]->m_nErrPnl == -1 || pDoc->m_pPcr[nLayer][nIdx]->m_nErrPnl == -2)
				{
					nDefCode = DEF_LIGHT;
					m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
				}
				else
					nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
			}
			else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
			{
				if (pDoc->m_pPcrInner[nLayer][nIdx]->m_nErrPnl == -1 || pDoc->m_pPcrInner[nLayer][nIdx]->m_nErrPnl == -2)
				{
					nDefCode = DEF_LIGHT;
					m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
				}
				else
					nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
			}
			else if (m_nLayer == RMAP_ITS)
			{
				if (pDoc->m_pPcrIts[nIdx]->m_nErrPnl == -1 || pDoc->m_pPcrIts[nIdx]->m_nErrPnl == -2)
			{
				nDefCode = DEF_LIGHT;
					m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
			}
			else
				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
			}
			else
				return 0;

			strTemp.Format(_T("%2d,"), nDefCode);	// 불량코드를 2칸으로 설정
			
			if(!nCol)								// strData에 처음으로 데이터를 추가
				strData.Insert(0, strTemp);
			else
			{
				int nLen = strData.GetLength();
				if( !(nCol%nStripY) )				// Separate Strip (스트립 마다)
				{
					strData.Insert(nLen, _T("  "));
					nLen = strData.GetLength();
				}
				strData.Insert(nLen, strTemp);
			}
		}

		int nPos = strData.ReverseFind(',');		// 릴맵 Text 맨 우측의 ','를 삭제
		strData.Delete(nPos, 1);
		::WritePrivateProfileString(sPnl, sRow, strData, m_sPathShare); // 한 라인씩 릴맵 Text를 기록.
	}	// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Row : Shot의 마지막 Col까지 기록하고 끝남.

	for(i=0; i<nNodeY; i++)
		delete[]  pPnlBuf[i];
	delete[] pPnlBuf;

	m_nWritedSerial = nSerial;

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//	pDoc->WriteReelmapIts(nSerial);

	return TRUE;
}


BOOL CReelMap::Write(int nSerial, int nLayer, CString sPath)
{
	return TRUE;

	if(nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.63"));
		return 0;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
		MakeItsReelmapHeader();

	MakeHeader(sPath);

	if (pDoc->GetTestMode() == MODE_INNER)
		pDoc->SetItsSerialInfo(nSerial);

	int nIdx = pDoc->GetPcrIdx1(nSerial, pDoc->m_bNewLotShare[1]);
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nStripY = pDoc->m_Master[0].m_pPcsRgn->nRow / MAX_STRIP_NUM; // Strip(1~4);
	int nTotDefPcs = 0;

	if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
	{
		if (pDoc->m_pPcr[nLayer])
	{
			if (pDoc->m_pPcr[nLayer][nIdx])
			nTotDefPcs = pDoc->m_pPcr[nLayer][nIdx]->m_nTotDef;
	}
	}
	else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
	{
		if (pDoc->m_pPcrInner[nLayer])
		{
			if (pDoc->m_pPcrInner[nLayer][nIdx])
				nTotDefPcs = pDoc->m_pPcrInner[nLayer][nIdx]->m_nTotDef;
		}
	}
	else if (m_nLayer == RMAP_ITS)
	{
		if (pDoc->m_pPcrIts)
		{
			if (pDoc->m_pPcrIts[nIdx])
				nTotDefPcs = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
		}
	}
	else
		return 0;

	short **pPnlBuf;
	int i, nC, nR, nPcsId, nDefCode;//, nTot, nDef, nGood;
	pPnlBuf = new short*[nNodeY];
	for(i=0; i<nNodeY; i++)
	{ 
		pPnlBuf[i] = new short[nNodeX];
		memset(pPnlBuf[i], 0, sizeof(short)*nNodeX);
		memset(m_pPnlBuf[nSerial - 1][i], 0, sizeof(short)*nNodeX);
	}

	CString strData, strTemp;

	if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
	{
		for (i = 0; i < nTotDefPcs; i++)
		{
			if (pDoc->m_pPcr[nLayer][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
				if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
				{
					switch (pDoc->m_Master[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
					{
					case 0:
						nPcsId = pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i];
						break;
					case 1:
						nPcsId = pDoc->MirrorLR(pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					case 2:
						nPcsId = pDoc->MirrorUD(pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					case 3:
						nPcsId = pDoc->Rotate180(pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					default:
			nPcsId = pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i];
						break;
					}
				}
				else
					nPcsId = pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i];

			nDefCode = pDoc->m_pPcr[nLayer][nIdx]->m_pDefType[i];

			nC = int(nPcsId / nNodeY);
				if (nC % 2)	// 홀수.
					nR = nNodeY * (nC + 1) - nPcsId - 1;
				else		// 짝수.
					nR = nPcsId - nNodeY * nC;
				pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
				if (m_pPnlBuf)
					m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC];   // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용. #define PNL_TOT
			}
		}
	}
	else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
	{
		for (i = 0; i < nTotDefPcs; i++)
		{
			if (pDoc->m_pPcrInner[nLayer][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
			{
				if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
				{
					switch (pDoc->m_Master[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
					{
					case 0:
						nPcsId = pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i];
						break;
					case 1:
						nPcsId = pDoc->MirrorLR(pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					case 2:
						nPcsId = pDoc->MirrorUD(pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					case 3:
						nPcsId = pDoc->Rotate180(pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i]);
						break;
					default:
						nPcsId = pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i];
						break;
					}
				}
				else
					nPcsId = pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i];

				nDefCode = pDoc->m_pPcrInner[nLayer][nIdx]->m_pDefType[i];

				nC = int(nPcsId / nNodeY);
				if (nC % 2)	// 홀수.
					nR = nNodeY * (nC + 1) - nPcsId - 1;
			else		// 짝수.
				nR = nPcsId - nNodeY * nC;
			pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
				if (m_pPnlBuf)
					m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC];   // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용. #define PNL_TOT
			}
		}
	}
	else if (m_nLayer == RMAP_ITS)
	{
		for (i = 0; i < nTotDefPcs; i++)
		{
			if (pDoc->m_pPcrIts[nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
			{
				if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
				{
					switch (pDoc->m_Master[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
					{
					case 0:
						nPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i];
						break;
					case 1:
						nPcsId = pDoc->MirrorLR(pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i]);
						break;
					case 2:
						nPcsId = pDoc->MirrorUD(pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i]);
						break;
					case 3:
						nPcsId = pDoc->Rotate180(pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i]);
						break;
					default:
						nPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i];
						break;
					}
				}
				else
					nPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i];

				nDefCode = pDoc->m_pPcrIts[nIdx]->m_pDefType[i];

				nC = int(nPcsId / nNodeY);
				if (nC % 2)	// 홀수.
					nR = nNodeY * (nC + 1) - nPcsId - 1;
				else		// 짝수.
					nR = nPcsId - nNodeY * nC;
				pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
				if (m_pPnlBuf)
					m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC];   // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용. #define PNL_TOT
			}
		}
	}
	else
		return 0;


	CString sPnl, sRow;
	i = 0;
	sPnl.Format(_T("%d"), nSerial);
	strData.Format(_T("%d"), nTotDefPcs);
	::WritePrivateProfileString(sPnl, _T("Total Defects"), strData, sPath);

	for(int nRow=0; nRow<nNodeX; nRow++)
	{
		sRow.Format(_T("%02d"), nRow);
// 		sRow.Format(_T("%2d"), nRow+1);
		strData.Format(_T(""));
		strTemp.Format(_T(""));

// 		if(nRow==26)
// 			int brk=0;

		for(int nCol=0; nCol<nNodeY; nCol++)
		{
			nR = (nNodeY-1)-nCol;
			nC = nRow;

			if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
			{
				if (pDoc->m_pPcr[nLayer][nIdx]->m_nErrPnl == -1 || pDoc->m_pPcr[nLayer][nIdx]->m_nErrPnl == -2)
			{
				nDefCode = DEF_LIGHT;
					m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
			}
			else
				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
			}
			else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
			{
				if (pDoc->m_pPcrInner[nLayer][nIdx]->m_nErrPnl == -1 || pDoc->m_pPcrInner[nLayer][nIdx]->m_nErrPnl == -2)
				{
					nDefCode = DEF_LIGHT;
					m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
				}
				else
					nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
			}
			else if (m_nLayer == RMAP_ITS)
			{
				if (pDoc->m_pPcrIts[nIdx]->m_nErrPnl == -1 || pDoc->m_pPcrIts[nIdx]->m_nErrPnl == -2)
				{
					nDefCode = DEF_LIGHT;
					m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
				}
				else
					nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
			}
			else
				return 0;

			strTemp.Format(_T("%2d,"), nDefCode);
			
			if(!nCol)
				strData.Insert(0, strTemp);
			else
			{
				int nLen = strData.GetLength();
				if( !(nCol%nStripY) ) // Separate Strip
				{
					strData.Insert(nLen, _T("  "));
					nLen = strData.GetLength();
				}
				strData.Insert(nLen, strTemp);
			}
		}

		int nPos = strData.ReverseFind(',');
		strData.Delete(nPos, 1);
		::WritePrivateProfileString(sPnl, sRow, strData, sPath);
	}

	for(i=0; i<nNodeY; i++)
		delete[]  pPnlBuf[i];
	delete[] pPnlBuf;

	m_nWritedSerial = nSerial;
	
	//if (pDoc->GetTestMode() == MODE_OUTER)
	//	pDoc->WriteReelmapIts(nSerial);

	return TRUE;
}


void CReelMap::InitPcs()
{
	int k, i;
	if(nTotPnl>0 && nTotPcs>0)
	{
		if(!pPcsRgn)
		{
			pPcsRgn = new CRect*[nTotPnl];
			for(k=0; k<nTotPnl; k++)
			{
				pPcsRgn[k] = new CRect[nTotPcs];
			}
		}

		if(!pPcsDef)
		{
			pPcsDef = new int*[nTotPnl];
			for(k=0; k<nTotPnl; k++)
			{
				if (nTotPcs > MAX_PCS)
				{
					pView->ClrDispMsg();
					AfxMessageBox(_T("MAX_PCS 초과 Error."));
				}
				pPcsDef[k] = new int[MAX_PCS];
				for(i=0; i<MAX_PCS; i++)
					pPcsDef[k][i] = DEF_NONE;
			}
		}

// 		if(!pMkInfo)
// 			pMkInfo = new CString[nTotPcs];
	}
	else
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-InitPcs."));
	}
}

// void CReelMap::ResetReelmap()
// {
// 	int k;
// // 	if(pMkInfo)
// // 	{
// // 		delete[] pMkInfo;
// // 		pMkInfo = NULL;
// // 	}
// 
// 	if(pPcsRgn)
// 	{
// 		for(k=0; k<nTotPnl; k++)
// 		{
// 			delete[] pPcsRgn[k];
// 			pPcsRgn[k] = NULL;
// 		}
// 		delete[] pPcsRgn;
// 		pPcsRgn = NULL;
// 	}
// 
// // 	CloseRst();
// 
// // 	if(pPcsDef)
// // 	{
// // 		for(k=0; k<nTotPnl; k++)
// // 		{
// // 			delete[] pPcsDef[k];
// // 			pPcsDef[k] = NULL;
// // 		}
// // 
// // 		delete[] pPcsDef;
// // 		pPcsDef = NULL;
// // 	}
// 
// }

void CReelMap::ClrPcs()
{
// 	int k;
// 	if(pMkInfo)
// 	{
// 		delete[] pMkInfo;
// 		pMkInfo = NULL;
// 	}
// 
// 	if(pPcsRgn)
// 	{
// 		for(k=0; k<nTotPnl; k++)
// 		{
// 			delete[] pPcsRgn[k];
// 			pPcsRgn[k] = NULL;
// 		}
// 		delete[] pPcsRgn;
// 		pPcsRgn = NULL;
// 	}
// 
// 	if(pPcsDef)
// 	{
// 		for(k=0; k<nTotPnl; k++)
// 		{
// 			delete[] pPcsDef[k];
// 			pPcsDef[k] = NULL;
// 		}
// 
// 		delete[] pPcsDef;
// 		pPcsDef = NULL;
// 	}


	for(int k=0; k<nTotPnl; k++)
	{
		for(int i=0; i<nTotPcs; i++)
		{
			pPcsDef[k][i] = DEF_NONE;
		}
	}
}

BOOL CReelMap::Disp(int nMkPnl, BOOL bDumy)
{
	//if(!bDumy)
	//{
	//	if(!pPcsDef || !m_pPnlNum || !m_pPnlDefNum)// || !pMkInfo)
	//	{
	//		pView->MsgBox(_T("Reelmap Memory Error."));
	//		return FALSE;
	//	}
	//}

	//if(!pDoc->m_Master[0].m_pPcsRgn)
	//{
	//	pView->MsgBox(_T("PCS RGN Error."));
	//	return FALSE;
	//}

	m_sPathBuf = GetRmapPath(m_nLayer);

	CDataFile *pDataFile = new CDataFile;
	CString sMsg;
	int nRepeat = 0;

	while(!pDataFile->Open(m_sPathBuf))
	{
		Sleep(500);
		if (!pDataFile->Open(m_sPathBuf))
	{
			Sleep(500);
			if (!pDataFile->Open(m_sPathBuf))
		{
				Sleep(500);
				if (!pDataFile->Open(m_sPathBuf))
			{
					if (nRepeat > 20)
				{
						sMsg.Format(_T("릴맵파일을 읽지 못했습니다.\r\n%s\r\n릴맵파일을 다시 읽으시겠습니까?"), m_sPathBuf);
						if (IDNO == pView->MsgBox(sMsg, 0, MB_YESNO))
					{
							;
					}
				}
					else
					{
						nRepeat++;
						Sleep(500);
			}
		}
				else
					break;
			}
			else
				break;
		}
		else
			break;
	}
	
	m_nSerial = nMkPnl; // 8

	int nMkedPnl = nTotPnl-m_nSelMarkingPnl-1; // 8-2-1 = 5
	int nPrevPnl = m_nSelMarkingPnl; // 2

	int nFromPnl = nMkPnl-nMkedPnl-1; // 8-5-1 = 2

	CString sPnl, sRow, sVal;
	TCHAR sep[] = { _T(",/;\r\n\t") };
	TCHAR szData[MAX_PATH];
	int k, i, nR, nC, nP, nInc=0;
	//int nCol, nRow;
	int nLoadPnl, nDefCode;
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol; // 10
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow; // 5

	for(k=nTotPnl-1; k>=0; k--) // k = 7 ~ 0
	{
		nInc++;
		nLoadPnl = nInc+nFromPnl; // 1 + 2 = 3 ~ 8 + 2 = 10

		if(pDoc->WorkingInfo.LastJob.bLotSep)
		{
			if(nLoadPnl > pDoc->m_nLotLastShot && !pDoc->m_bDoneChgLot)
			{
				for(int aa=k; aa>=0; aa--)
				{
					m_pPnlNum[aa] = nLoadPnl = -1;
					m_pPnlDefNum[aa] = -1;
				}
				break;
			}
		}


		if( nLoadPnl > (pView->m_nLotEndSerial+pDoc->AoiDummyShot[0]) && pView->m_nLotEndSerial > 0)
		{
			for(int bb=k; bb>=0; bb--)
			{
				m_pPnlNum[bb] = nLoadPnl = -1;
				m_pPnlDefNum[bb] = -1;
			}
			break;
		}
		else if(pView->ChkLastProc() && (nLoadPnl > pView->m_nLotEndSerial && pView->m_nLotEndSerial > 0))
		{
			for(int bb=k; bb>=0; bb--)
			{
				m_pPnlNum[bb] = nLoadPnl = -1;
				m_pPnlDefNum[bb] = -1;
			}
			break;
		}
		else if(nLoadPnl > pView->m_nLotEndSerial && pView->m_nLotEndSerial > 0)
		{
			for(int cc=k; cc>=0; cc--)
			{
				m_pPnlDefNum[cc] = -1;
			}
		}


		if(nLoadPnl <= 0)
		{
			m_pPnlNum[k] = -1;
			m_pPnlDefNum[k] = -1;
			for(i=0; i<nTotPcs; i++)
				pPcsDef[k][i] = DEF_NONE;
		}
		else
		{
			if(pDoc->WorkingInfo.LastJob.bLotSep && nLoadPnl > pDoc->m_nLotLastShot && !pDoc->m_bDoneChgLot)
			{
				m_pPnlNum[k] = 0;
				m_pPnlDefNum[k] = -1;
				break;
			}
			else if (nLoadPnl > pDoc->m_ListBuf[0].GetLast())
			{
				m_pPnlNum[k] = 0;
				m_pPnlDefNum[k] = -1;
				break;
			}

			m_pPnlNum[k] = nLoadPnl; // 3 ~ 10
			if (nLoadPnl <= pView->m_nLotEndSerial || pView->m_nLotEndSerial == 0)
			{
				if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
				m_pPnlDefNum[k] = pDoc->m_pPcr[RMAP_ALLUP][pDoc->GetPcrIdx1(nLoadPnl)]->m_nTotDef;
				else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
					m_pPnlDefNum[k] = pDoc->m_pPcrInner[RMAP_ALLUP][pDoc->GetPcrIdx1(nLoadPnl)]->m_nTotDef;
				else if (m_nLayer == RMAP_ITS)
					m_pPnlDefNum[k] = pDoc->m_pPcrIts[pDoc->GetPcrIdx1(nLoadPnl)]->m_nTotDef;
				else
					return 0;
			}

			for(nR=0; nR<nNodeX; nR++)  // nR = 0 ~ 5
			{
				sPnl.Format(_T("%d"), nLoadPnl);
				sRow.Format(_T("%02d"), nR);
				
				if (0 < ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), m_sPathBuf)) // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일
				{
					for(nC=0; nC<nNodeY; nC++)
					{
						if(nC==0)
							sVal = _tcstok(szData,sep);
						else
							sVal = _tcstok(NULL,sep);
						
						if(nR%2)	// 홀수.
							nP = nC + nNodeY * nR;
						else		// 짝수.				
							nP = nNodeY * (nR+1) - (nC+1);

						if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
						{
							switch (pDoc->m_Master[1].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
							{
							case 0:
								break;
							case 1:
								nP = pDoc->MirrorLR(nP);
								break;
							case 2:
								nP = pDoc->MirrorUD(nP);
								break;
							case 3:
								nP = pDoc->Rotate180(nP);
								break;
							default:
								break;
							}
						}

						nDefCode = _tstoi(sVal);
						pPcsDef[k][nP] = nDefCode; // k=7, nP = PcsIdx : 좌상단에서 nP 0이 시작하여 ZigZeg로 우하단으로 증가

					}
				}
				else
				{
					Sleep(300);

					if (0 < ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), m_sPathBuf))
					{
						for (nC = 0; nC < nNodeY; nC++)
						{
							if (nC == 0)
								sVal = _tcstok(szData, sep);
							else
								sVal = _tcstok(NULL, sep);

							if (nR % 2)	// 홀수.
								nP = nC + nNodeY * nR;
							else		// 짝수.				
								nP = nNodeY * (nR + 1) - (nC + 1);

							if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
							{
								switch (pDoc->m_Master[1].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
								{
								case 0:
									break;
								case 1:
									nP = pDoc->MirrorLR(nP);
									break;
								case 2:
									nP = pDoc->MirrorUD(nP);
									break;
								case 3:
									nP = pDoc->Rotate180(nP);
									break;
								default:
									break;
								}
							}

							nDefCode = _tstoi(sVal);
							pPcsDef[k][nP] = nDefCode; // k=7, nP = PcsIdx : 좌상단에서 nP 0이 시작하여 ZigZeg로 우하단으로 증가
					}
				}
				else
				{
						if (!bDumy)
					{
							if (!pView->m_bLastProc && !pDoc->WorkingInfo.LastJob.bSampleTest)
						{
							pView->Stop();
							sMsg.Format(_T("릴맵 데이타를 불러 오지 못했습니다. - [Pnl: %s] [Row: %d]\r\n%s"), sPnl, nR, m_sPathBuf);
 							//sMsg.Format(_T("ReelmapData missed. - [Pnl: %d] [Row: %d]"), k, nR);
							pView->MsgBox(sMsg); // 20160725-Temp
								if (pDataFile)
							{
								delete pDataFile;
								pDataFile = NULL;
							}
							//AfxMessageBox(sMsg);
							return FALSE;
						}
						else
						{
							m_pPnlNum[k] = 0;
							m_pPnlDefNum[k] = -1;

								for (nC = 0; nC < nNodeY; nC++)
							{
									if (nR % 2)	// 홀수.
									nP = nC + nNodeY * nR;
								else		// 짝수.				
										nP = nNodeY * (nR + 1) - (nC + 1);

									if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
									{
										switch (pDoc->m_Master[1].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
										{
										case 0:
											break;
										case 1:
											nP = pDoc->MirrorLR(nP);
											break;
										case 2:
											nP = pDoc->MirrorUD(nP);
											break;
										case 3:
											nP = pDoc->Rotate180(nP);
											break;
										default:
											break;
										}
									}

									nDefCode = DEF_NONE;
									pPcsDef[k][nP] = nDefCode; // k=7, nP = PcsIdx : 좌상단에서 nP 0이 시작하여 ZigZeg로 우하단으로 증가
							}

						}
					}
					else
					{
							for (nC = 0; nC < nNodeY; nC++)
						{
								if (nC == 0)
									sVal = _tcstok(szData, sep);
							else
									sVal = _tcstok(NULL, sep);

								if (nR % 2)	// 홀수.
								nP = nC + nNodeY * nR;
							else		// 짝수.
									nP = nNodeY * (nR + 1) - (nC + 1);

								if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
								{
									switch (pDoc->m_Master[1].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
									{
									case 0:
										break;
									case 1:
										nP = pDoc->MirrorLR(nP);
										break;
									case 2:
										nP = pDoc->MirrorUD(nP);
										break;
									case 3:
										nP = pDoc->Rotate180(nP);
										break;
									default:
										break;
									}
								}

								nDefCode = 0;
								pPcsDef[k][nP] = nDefCode; // k=7, nP = PcsIdx : 좌상단에서 nP 0이 시작하여 ZigZeg로 우하단으로 증가

	 							//if(pDoc->WorkingInfo.LastJob.bContFixDef)
	 							//	SetFixPcs(nC, nR);
							}
						}
					}
				}
			}
		}
	}

	if(pDataFile)
	{
		delete pDataFile;
		pDataFile = NULL;
	}

	return TRUE;
}

void CReelMap::SetLotSt()
{
	CString sData;
	int nYear, nMonth, nDay, nHour, nMin, nSec;
	nYear = pDoc->WorkingInfo.Lot.StTime.nYear; 
	nMonth = pDoc->WorkingInfo.Lot.StTime.nMonth; 
	nDay = pDoc->WorkingInfo.Lot.StTime.nDay; 
	nHour = pDoc->WorkingInfo.Lot.StTime.nHour; 
	nMin = pDoc->WorkingInfo.Lot.StTime.nMin; 
	nSec = pDoc->WorkingInfo.Lot.StTime.nSec;

	if(!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		sData = _T("");
	else
		sData.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);
	::WritePrivateProfileString(_T("Info"), _T("Lot Start"), sData, m_sPathBuf);
}

CString CReelMap::GetLotSt()
{
	CString sLot = _T("");
	//char szData[MAX_PATH];
	TCHAR szData[MAX_PATH];
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot Start"), NULL, szData, sizeof(szData), m_sPathBuf))
		sLot = CString(szData);
	return sLot;
}

void CReelMap::SetLotEd()
{
	CString sData;
	int nYear, nMonth, nDay, nHour, nMin, nSec;
	nYear = pDoc->WorkingInfo.Lot.EdTime.nYear; 
	nMonth = pDoc->WorkingInfo.Lot.EdTime.nMonth; 
	nDay = pDoc->WorkingInfo.Lot.EdTime.nDay; 
	nHour = pDoc->WorkingInfo.Lot.EdTime.nHour; 
	nMin = pDoc->WorkingInfo.Lot.EdTime.nMin; 
	nSec = pDoc->WorkingInfo.Lot.EdTime.nSec;

	if(!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		sData = _T("");
	else
		sData.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);
	::WritePrivateProfileString(_T("Info"), _T("Lot End"), sData, m_sPathBuf);

	if((pView->m_dwLotEd-pView->m_dwLotSt) > 0)
	{
		int nDiff = (pView->m_dwLotEd-pView->m_dwLotSt)/1000;
		nHour = int(nDiff/3600);
		nMin = int((nDiff-3600*nHour)/60);
		nSec = nDiff % 60;
		sData.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);
		::WritePrivateProfileString(_T("Info"), _T("Lot Run"), sData, m_sPathBuf);
	}
}

CString CReelMap::GetLotEd()
{
	CString sLot = _T("");
	//char szData[MAX_PATH];
	TCHAR szData[MAX_PATH];
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot End"), NULL, szData, sizeof(szData), m_sPathBuf))
		sLot = CString(szData);
	return sLot;
}

void CReelMap::SetLastSerial(int nSerial) 	// 릴맵 텍스트 파일의 수율정보를 업데이트함.
{
	if(nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.64"));
		return;
	}

	m_nLastShot = nSerial;

	CString sData;
	sData.Format(_T("%d"), nSerial);
	::WritePrivateProfileString(_T("Info"), _T("Marked Shot"), sData, m_sPathBuf);

	int nYear, nMonth, nDay, nHour, nMin, nSec;
	nYear = pDoc->WorkingInfo.Lot.CurTime.nYear; 
	nMonth = pDoc->WorkingInfo.Lot.CurTime.nMonth; 
	nDay = pDoc->WorkingInfo.Lot.CurTime.nDay; 
	nHour = pDoc->WorkingInfo.Lot.CurTime.nHour; 
	nMin = pDoc->WorkingInfo.Lot.CurTime.nMin; 
	nSec = pDoc->WorkingInfo.Lot.CurTime.nSec;

	if(!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		sData = _T("");
	else
		sData.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);
	::WritePrivateProfileString(_T("Info"), _T("Marked Date"), sData, m_sPathBuf);

	//UpdateRst();
}

void CReelMap::SetCompletedSerial(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.67"));
		return;
	}

	m_nCompletedShot = nSerial;

	CString sData;
	sData.Format(_T("%d"), nSerial);
	::WritePrivateProfileString(_T("Info"), _T("Completed Shot"), sData, m_sPathBuf);

	int nYear, nMonth, nDay, nHour, nMin, nSec;
	nYear = pDoc->WorkingInfo.Lot.CurTime.nYear;
	nMonth = pDoc->WorkingInfo.Lot.CurTime.nMonth;
	nDay = pDoc->WorkingInfo.Lot.CurTime.nDay;
	nHour = pDoc->WorkingInfo.Lot.CurTime.nHour;
	nMin = pDoc->WorkingInfo.Lot.CurTime.nMin;
	nSec = pDoc->WorkingInfo.Lot.CurTime.nSec;

	if (!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		sData = _T("");
	else
		sData.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);
	::WritePrivateProfileString(_T("Info"), _T("Completed Date"), sData, m_sPathBuf);

	//UpdateRst(nSerial);
}

BOOL CReelMap::GetRst(int nFrom, int nTo)
{
	return TRUE;
}

BOOL CReelMap::GetRst(int nSerial)
{
	return TRUE;
}


BOOL CReelMap::InitRst()
{
	int nPnl, nRow;
	int TotPnl = PNL_TOT;
	int nDumyPnl = 20;
// 	int nNodeX = pDoc->m_pPcsRgn->nCol;
// 	int nNodeY = pDoc->m_pPcsRgn->nRow;
	int nNodeX = PNLBUF_X;
	int nNodeY = PNLBUF_Y;

	int k, i;

 	CloseRst();

	m_nPnlBuf = TotPnl+nDumyPnl;
	if(!m_pPnlBuf)
	{
		m_pPnlBuf = new short**[m_nPnlBuf];
		for(nPnl=0; nPnl<m_nPnlBuf; nPnl++)
		{
			m_pPnlBuf[nPnl] = new short*[nNodeY];
			for(nRow=0; nRow<nNodeY; nRow++)
				m_pPnlBuf[nPnl][nRow] = new short[nNodeX];
		}
	}

	m_nTotPcs = 0;
	m_nGoodPcs = 0;
	m_nBadPcs = 0;
	for(i=0; i<MAX_DEF; i++)
		m_nDef[i] = 0;

	m_nTotStOut = 0;
	for(k=0; k<MAX_STRIP_NUM; k++)
	{
		m_nStripOut[k] = 0;
		m_nDefStrip[k] = 0;
		for(i=0; i<MAX_DEF; i++)
			m_nDefPerStrip[k][i] = 0;
	}

	return TRUE;
}

void CReelMap::CloseRst()
{
	int nPnl, nRow;
// 	int nNodeX = pDoc->m_pPcsRgn->nCol;
// 	int nNodeY = pDoc->m_pPcsRgn->nRow;
	int nNodeX = PNLBUF_X;
	int nNodeY = PNLBUF_Y;

	if(m_pPnlBuf)
	{
		for(nPnl=0; nPnl<m_nPnlBuf; nPnl++)
		{
			for(nRow=0; nRow<nNodeY; nRow++)
				delete[] m_pPnlBuf[nPnl][nRow];
			delete[] m_pPnlBuf[nPnl];
		}

		delete[] m_pPnlBuf;
		m_pPnlBuf = NULL;
		m_nPnlBuf = 0;
	}
}

void CReelMap::ClrRst()
{
	m_nTotPcs = 0;
	m_nGoodPcs = 0;
	m_nBadPcs = 0;

	int k, i;
	for(i=1; i<MAX_DEF; i++)
		m_nDef[i] = 0;

	m_nTotStOut = 0;
	for(k=0; k<MAX_STRIP_NUM; k++)
	{
		m_nStripOut[k] = 0;
		m_nDefStrip[k] = 0;
		for(i=1; i<MAX_DEF; i++)
			m_nDefPerStrip[k][i] = 0;
	}

// 	double dN0;
// 	if(m_dTotLen > m_dPnlLen)
// 		dN0 = m_dTotLen / m_dPnlLen;
// 	else
// 		dN0 = (double)PNL_TOT;
// 	int nN0 = int(m_dTotLen / m_dPnlLen);
// 	int TotPnl = (dN0 - (double)nN0) > 0.0 ? (nN0+1) : nN0 ;
// 	if(TotPnl > PNL_TOT)
// 		TotPnl = PNL_TOT;

	int nPnl, nRow, nCol;
// 	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
// 	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int TotPnl = PNL_TOT;
	int nNodeX = PNLBUF_X;
	int nNodeY = PNLBUF_Y;

	if(m_pPnlBuf)
	{
		for(nPnl=0; nPnl<TotPnl; nPnl++)
		{
			for(nRow=0; nRow<nNodeY; nRow++)
			{
				for(nCol=0; nCol<nNodeX; nCol++)
					m_pPnlBuf[nPnl][nRow][nCol] = 0;
			}
		}
	}
}

CString CReelMap::GetYieldPath(int nRmap)
{
	CString sPath;
	//BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CString str;

	//if (bDualTest)
	{
		switch (nRmap)
		{
		case RMAP_UP:
			str = _T("YieldUp.txt");
				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
					pDoc->WorkingInfo.LastJob.sModelUp,
					pDoc->WorkingInfo.LastJob.sLotUp,
					pDoc->WorkingInfo.LastJob.sLayerUp,
					str);
			//if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
			//{
			//	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			//		pDoc->WorkingInfo.LastJob.sModelUp,
			//		pDoc->WorkingInfo.LastJob.sLotUp,
			//		pDoc->WorkingInfo.LastJob.sLayerUp,
			//		str);
			//}
			//else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
			//{
			//	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			//		pDoc->WorkingInfo.LastJob.sModelUp,
			//		pDoc->Status.PcrShare[0].sLot,
			//		pDoc->WorkingInfo.LastJob.sLayerUp,
			//		str);
			//}
			break;
		case RMAP_ALLUP:
			str = _T("YieldAll.txt");
				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
					pDoc->WorkingInfo.LastJob.sModelUp,
					pDoc->WorkingInfo.LastJob.sLotUp,
					pDoc->WorkingInfo.LastJob.sLayerUp,
					str);
			//if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
			//{
			//	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			//		pDoc->WorkingInfo.LastJob.sModelUp,
			//		pDoc->WorkingInfo.LastJob.sLotUp,
			//		pDoc->WorkingInfo.LastJob.sLayerUp,
			//		str);
			//}
			//else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
			//{
			//	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			//		pDoc->WorkingInfo.LastJob.sModelUp,
			//		pDoc->Status.PcrShare[0].sLot,
			//		pDoc->WorkingInfo.LastJob.sLayerUp,
			//		str);
			//}
			break;
		case RMAP_DN:
			str = _T("YieldDn.txt");
				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
					pDoc->WorkingInfo.LastJob.sModelUp,
					pDoc->WorkingInfo.LastJob.sLotUp,
					pDoc->WorkingInfo.LastJob.sLayerDn,
					str);
			//if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
			//{
			//	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			//		//pDoc->WorkingInfo.LastJob.sModelDn,
			//		//pDoc->WorkingInfo.LastJob.sLotDn,
			//		pDoc->WorkingInfo.LastJob.sModelUp,
			//		pDoc->WorkingInfo.LastJob.sLotUp,
			//		pDoc->WorkingInfo.LastJob.sLayerDn,
			//		str);
			//}
			//else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
			//{
			//	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			//		//pDoc->WorkingInfo.LastJob.sModelDn,
			//		//pDoc->Status.PcrShare[1].sLot,
			//		pDoc->WorkingInfo.LastJob.sModelUp,
			//		pDoc->Status.PcrShare[0].sLot,
			//		pDoc->WorkingInfo.LastJob.sLayerDn,
			//		str);
			//}
			break;
		case RMAP_ALLDN:
			str = _T("YieldAll.txt");
				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
					pDoc->WorkingInfo.LastJob.sModelUp,
					pDoc->WorkingInfo.LastJob.sLotUp,
					pDoc->WorkingInfo.LastJob.sLayerDn,
					str);
		//if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
			//{
			//	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			//		//pDoc->WorkingInfo.LastJob.sModelDn,
			//		//pDoc->WorkingInfo.LastJob.sLotDn,
			//		pDoc->WorkingInfo.LastJob.sModelUp,
			//		pDoc->WorkingInfo.LastJob.sLotUp,
			//		pDoc->WorkingInfo.LastJob.sLayerDn,
			//		str);
			//}
			//else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
			//{
			//	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			//		//pDoc->WorkingInfo.LastJob.sModelDn,
			//		//pDoc->Status.PcrShare[1].sLot,
			//		pDoc->WorkingInfo.LastJob.sModelUp,
			//		pDoc->Status.PcrShare[0].sLot,
			//		pDoc->WorkingInfo.LastJob.sLayerDn,
			//		str);
			//}
			break;
		case RMAP_INNER_UP:
			str = _T("YieldUp.txt");
				sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
					pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
					pDoc->WorkingInfo.LastJob.sInnerLotUp, pDoc->WorkingInfo.LastJob.sInnerLayerUp,
				str);
			break;
		case RMAP_INNER_DN:
			str = _T("YieldDn.txt");
			sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
				pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
				pDoc->WorkingInfo.LastJob.sInnerLotUp, pDoc->WorkingInfo.LastJob.sInnerLayerDn,
				str);
			break;
		case RMAP_INNER_ALLUP:
			str = _T("YieldAll.txt");
			sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
				pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
				pDoc->WorkingInfo.LastJob.sInnerLotUp, pDoc->WorkingInfo.LastJob.sInnerLayerUp,
				str);
			break;
		case RMAP_INNER_ALLDN:
			str = _T("YieldAll.txt");
			sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
				pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
				pDoc->WorkingInfo.LastJob.sInnerLotUp, pDoc->WorkingInfo.LastJob.sInnerLayerDn,
				str);
			break;
		case RMAP_INOUTER_UP:
			break;
		case RMAP_INOUTER_DN:
			break;
		case RMAP_ITS:
			//pDoc->GetCurrentInfoEng();
			str = _T("YieldIts.txt");
			sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
				pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode, //pDoc->m_sItsCode,
				str);
			break;
		}
	}
	//else
	//{
	//	str = _T("YieldUp.txt");
	//	if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->WorkingInfo.LastJob.sLotUp,
	//			pDoc->WorkingInfo.LastJob.sLayerUp,
	//			str);
	//	}
	//	else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->Status.PcrShare[0].sLot,
	//			pDoc->WorkingInfo.LastJob.sLayerUp,
	//			str);
	//	}
	//}
	return sPath;
}

void CReelMap::ResetYield()
{
	m_stYield.nTot = 0; m_stYield.nGood = 0; m_stYield.nDef = 0;
	m_stYield.nTotSriptOut = 0;
	m_stYield.nDefStrip[0] = 0; m_stYield.nDefStrip[1] = 0; m_stYield.nDefStrip[2] = 0; m_stYield.nDefStrip[3] = 0;
	m_stYield.nStripOut[0] = 0; m_stYield.nStripOut[1] = 0; m_stYield.nStripOut[2] = 0; m_stYield.nStripOut[3] = 0;

	for (int k = 0; k < MAX_DEF; k++)
	{
		m_stYield.nDefA[k] = 0;

		for (int i = 0; i < MAX_STRIP_NUM; i++)
		{
			m_stYield.nDefPerStrip[i][k] = 0;
		}
	}
}

BOOL CReelMap::ReadYield(int nSerial, CString sPath)
{
	int i, k;
	TCHAR szData[200];

	CString sCode, strMenu, strItem, sMsg;

	strMenu.Format(_T("%d"), nSerial);
	for (i = 1; i < MAX_DEF; i++)
	{
		sCode.Format(_T("%d"), i);
		if (0 < ::GetPrivateProfileString(strMenu, sCode, NULL, szData, sizeof(szData), sPath))
		{
			m_stYield.nDefA[i] = _ttoi(szData);
		}
		else
		{
			if (nSerial == 1)
			{
				m_stYield.nDefA[i] = 0;
			}
			else
			{
			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-불량(%d)\r\n%s"), nSerial, i, sPath);
			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
			return FALSE;
		}
	}
	}

	if (0 < ::GetPrivateProfileString(strMenu, _T("Total Pcs"), NULL, szData, sizeof(szData), sPath))
	{
		m_stYield.nTot = _ttoi(szData);
	}
	else
	{
		if (nSerial == 1)
		{
			m_stYield.nTot = 0;
		}
		else
		{
		sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Total Pcs\r\n%s"), nSerial, sPath);
		AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
		return FALSE;
	}
	}

	if (0 < ::GetPrivateProfileString(strMenu, _T("Good Pcs"), NULL, szData, sizeof(szData), sPath))
	{
		m_stYield.nGood = _ttoi(szData);
	}
	else
	{
		if (nSerial == 1)
		{
			m_stYield.nGood = 0;
		}
		else
		{
		sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Good Pcs\r\n%s"), nSerial, sPath);
		AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
		return FALSE;
	}
	}

	if (0 < ::GetPrivateProfileString(strMenu, _T("Bad Pcs"), NULL, szData, sizeof(szData), sPath))
	{
		m_stYield.nDef = _ttoi(szData);
	}
	else
	{
		if (nSerial == 1)
		{
			m_stYield.nDef = 0;
		}
		else
		{
		sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Bad Pcs\r\n%s"), nSerial, sPath);
		AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
		return FALSE;
	}
	}

	for (k = 0; k < MAX_STRIP; k++)
	{
		strItem.Format(_T("Strip%d"), k);
		if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
		{
			m_stYield.nDefStrip[k] = _ttoi(szData);
		}
		else
		{
			if (nSerial == 1)
			{
				m_stYield.nDefStrip[k] = 0;
			}
			else
			{
			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Strip%d\r\n%s"), nSerial, k, sPath);
			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
			return FALSE;
		}
		}

		strItem.Format(_T("StripOut_%d"), k);
		if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
		{
			m_stYield.nStripOut[k] = _ttoi(szData);
		}
		else
		{
			if (nSerial == 1)
			{
				m_stYield.nStripOut[k] = 0;
			}
			else
			{
			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-StripOut_%d\r\n%s"), nSerial, k, sPath);
			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
			return FALSE;
		}
		}

		for (i = 1; i < MAX_DEF; i++)
		{
			strItem.Format(_T("Strip%d_%d"), k, i);
			if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
			{
				m_stYield.nDefPerStrip[k][i] = _ttoi(szData);
			}
			else
			{
				if (nSerial == 1)
				{
					m_stYield.nDefPerStrip[k][i] = 0;
				}
				else
				{
				sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Strip%d_%d\r\n%s"), nSerial, k, i, sPath);
				AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
				return FALSE;
			}
		}
	}
	}

	if (0 < ::GetPrivateProfileString(strMenu, _T("StripOut_Total"), NULL, szData, sizeof(szData), sPath))
	{
		m_stYield.nTotSriptOut = _ttoi(szData);
	}
	else
	{
		if (nSerial == 1)
		{
			m_stYield.nTotSriptOut = 0;
		}
		else
		{
		sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-StripOut_Total\r\n%s"), nSerial, sPath);
		AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
		return FALSE;
	}
	}

	return TRUE;
}

//
//BOOL CReelMap::WriteYield(int nSerial, CString sPath)
//{
//	int nNodeX = 0, nNodeY = 0;
//#ifndef TEST_MODE
//	nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
//	nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
//#endif
//	CString sDefNum, strData;
//	int nPnl, nRow, nCol, nDefCode, nStrip;
//	int nTotPcs = nNodeX * nNodeY;
//	int nStripPcs = nTotPcs / 4;
//	double dStOutRto = _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
//	nPnl = nSerial - 1;
//
//	int i, k;
//	TCHAR szData[200];
//
//	CString sCode, strMenu, strItem, sMsg;
//	int nTot=0, nGood=0, nDef=0;
//	int nTotSriptOut = 0;
//
//	int nDefStrip[4], nDefA[MAX_DEF], nDefPerStrip[4][MAX_DEF], nStripOut[4];
//	nDefStrip[0] = 0; nDefStrip[1] = 0; nDefStrip[2] = 0; nDefStrip[3] = 0;
//	nStripOut[0] = 0; nStripOut[1] = 0; nStripOut[2] = 0; nStripOut[3] = 0;
//
//	nTot = nNodeX * nNodeY;
//
//	strMenu.Format(_T("%d"), nSerial);
//
//	strData.Format(_T("%d"), nSerial);
//	::WritePrivateProfileString(_T("Info"), _T("Total Shot"), strData, sPath);
//
//	for (k = 0; k < MAX_DEF; k++)
//	{
//		nDefA[k] = 0;
//
//		for (i = 0; i < 4; i++)
//		{
//			nDefPerStrip[i][k] = 0;
//		}
//	}
//
//	for (nRow = 0; nRow < nNodeY; nRow++)
//	{
//		for (nCol = 0; nCol < nNodeX; nCol++)
//		{
//			if (m_pPnlBuf)
//			{
//				nDefCode = (int)m_pPnlBuf[nPnl][nRow][nCol] < 0 ? 0 : (int)m_pPnlBuf[nPnl][nRow][nCol];
//				nDefA[nDefCode]++;
//
//				nStrip = int(nRow / (nNodeY / 4));
//				if (nStrip > -1 && nStrip < 4)
//				{
//					if (nDefCode > 0)
//					{
//						nDefStrip[nStrip]++;
//						nDefPerStrip[nStrip][nDefCode]++;
//					}
//				}
//			}
//		}
//	}
//
//	for (nStrip = 0; nStrip < 4; nStrip++)
//	{
//		if (nDefStrip[nStrip] >= nStripPcs * dStOutRto)
//			nStripOut[nStrip]++;
//	}
//
//	for (i = 1; i < MAX_DEF; i++)
//	{
//		sCode.Format(_T("%d"), i); 
//		m_stYield.nDefA[i] = m_stYield.nDefA[i] + nDefA[i];
//		sDefNum.Format(_T("%d"), m_stYield.nDefA[i]);
//		::WritePrivateProfileString(_T("Info"), sCode, sDefNum, sPath);
//		::WritePrivateProfileString(strMenu, sCode, sDefNum, sPath);
//
//		nDef += nDefA[i];
//	}
//
//	nGood = nTot - nDef;
//
//	m_stYield.nTot = m_stYield.nTot + nTot;
//	strData.Format(_T("%d"), m_stYield.nTot);
//	::WritePrivateProfileString(_T("Info"), _T("Total Pcs"), strData, sPath);
//	::WritePrivateProfileString(strMenu, _T("Total Pcs"), strData, sPath);
//
//	m_stYield.nGood = m_stYield.nGood + nGood;
//	strData.Format(_T("%d"), m_stYield.nGood);
//	::WritePrivateProfileString(_T("Info"), _T("Good Pcs"), strData, sPath);
//	::WritePrivateProfileString(strMenu, _T("Good Pcs"), strData, sPath);
//
//	m_stYield.nDef = m_stYield.nDef + nDef;
//	strData.Format(_T("%d"), m_stYield.nDef);
//	::WritePrivateProfileString(_T("Info"), _T("Bad Pcs"), strData, sPath);
//	::WritePrivateProfileString(strMenu, _T("Bad Pcs"), strData, sPath);
//
//	for (k = 0; k < 4; k++)
//	{
//		strItem.Format(_T("Strip%d"), k);
//		m_stYield.nDefStrip[k] = m_stYield.nDefStrip[k] + nDefStrip[k];
//		strData.Format(_T("%d"), m_stYield.nDefStrip[k]);
//		::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
//		::WritePrivateProfileString(strMenu, strItem, strData, sPath);
//
//		strItem.Format(_T("StripOut_%d"), k); 
//		m_stYield.nStripOut[k] = m_stYield.nStripOut[k] + nStripOut[k];
//		strData.Format(_T("%d"), m_stYield.nStripOut[k]);
//		::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
//		::WritePrivateProfileString(strMenu, strItem, strData, sPath);
//
//		nTotSriptOut += nStripOut[k];
//
//		for (i = 1; i < MAX_DEF; i++)
//		{
//			strItem.Format(_T("Strip%d_%d"), k, i);
//			m_stYield.nDefPerStrip[k][i] = m_stYield.nDefPerStrip[k][i] + nDefPerStrip[k][i];
//			strData.Format(_T("%d"), m_stYield.nDefPerStrip[k][i]);
//			::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
//			::WritePrivateProfileString(strMenu, strItem, strData, sPath);
//		}
//	}
//
//	m_stYield.nTotSriptOut = m_stYield.nTotSriptOut + nTotSriptOut;
//	strData.Format(_T("%d"), m_stYield.nTotSriptOut);
//	::WritePrivateProfileString(_T("Info"), _T("StripOut_Total"), strData, sPath);
//	::WritePrivateProfileString(strMenu, _T("StripOut_Total"), strData, sPath);
//
//	return TRUE;
//}

BOOL CReelMap::UpdateYield(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.68"));
		return 0;
	}

	m_sPathYield = GetYieldPath(m_nLayer);
	CString sPath = m_sPathYield;

	BOOL bExist = FALSE;
	CFileFind findfile;
	if (findfile.FindFile(sPath))
		bExist = TRUE;

	int nPnl = nSerial - 1;

	if (bExist && nPnl) // After first shot
	{
		ReadYield(nPnl, sPath);
		WriteYield(nSerial, sPath);
	}
	else // First Shot
	{
		ResetYield();
		WriteYield(nSerial, sPath);
	}
	Sleep(10);

	return TRUE;
}



BOOL CReelMap::UpdateRst()
{
	int k, i;
	CString strMenu, strItem, sCode, sDefNum, strData;

	m_nGoodPcs = m_stYield.nGood;
	m_nBadPcs =  m_stYield.nDef;

	strData.Format(_T("%d"), m_stYield.nTot);
	::WritePrivateProfileString(_T("Info"), _T("Total Pcs"), strData, m_sPathBuf);
	strData.Format(_T("%d"), m_nGoodPcs);
	::WritePrivateProfileString(_T("Info"), _T("Good Pcs"), strData, m_sPathBuf);
	strData.Format(_T("%d"), m_nBadPcs);
	::WritePrivateProfileString(_T("Info"), _T("Bad Pcs"), strData, m_sPathBuf);

	for (i = 1; i < MAX_DEF; i++)
	{
		m_nDef[i] = m_stYield.nDefA[i];
		sCode.Format(_T("%d"), i);
		sDefNum.Format(_T("%d"), m_nDef[i]);
		::WritePrivateProfileString(_T("Info"), sCode, sDefNum, m_sPathBuf);
	}

	int nTotStOut = 0;
	for (k = 0; k < MAX_STRIP; k++)
	{
		strItem.Format(_T("Strip%d"), k);
		m_nDefStrip[k] = m_stYield.nDefStrip[k];
		strData.Format(_T("%d"), m_nDefStrip[k]);
		::WritePrivateProfileString(_T("Info"), strItem, strData, m_sPathBuf);

		strItem.Format(_T("%d"), k);
		m_nStripOut[k] = m_stYield.nStripOut[k];
		strData.Format(_T("%d"), m_nStripOut[k]);
		::WritePrivateProfileString(_T("StripOut"), strItem, strData, m_sPathBuf);

		for (i = 1; i < MAX_DEF; i++)
		{
			strMenu.Format(_T("Strip%d"), k);
			strItem.Format(_T("%d"), i);
			m_nDefPerStrip[k][i] = m_stYield.nDefPerStrip[k][i];
			strData.Format(_T("%d"), m_nDefPerStrip[k][i]);
			::WritePrivateProfileString(strMenu, strItem, strData, m_sPathBuf);
		}
	}
	strData.Format(_T("%d"), m_stYield.nTotSriptOut);
	::WritePrivateProfileString(_T("StripOut"), _T("Total"), strData, m_sPathBuf);

	return TRUE;
}

int CReelMap::GetDefNum(int nDefCode)
{
	return m_stYield.nDefA[nDefCode];
	//return m_nDef[nDefCode];
}

int CReelMap::GetDefStrip(int nStrip)
{
	return m_stYield.nDefStrip[nStrip];
	//return m_nDefStrip[nStrip];
}

int CReelMap::GetDefStrip(int nStrip, int nDefCode)
{
	return m_stYield.nDefPerStrip[nStrip][nDefCode];
	//return m_nDefPerStrip[nStrip][nDefCode];
}

int CReelMap::GetStripOut(int nStrip)
{
	return m_stYield.nStripOut[nStrip];
	//return m_nStripOut[nStrip];
}

void CReelMap::GetPcsNum(int &nGood, int &nBad)
{
	nGood = m_stYield.nGood;
	nBad = m_stYield.nDef;
	//nGood = m_nGoodPcs;
	//nBad = m_nBadPcs;
}

void CReelMap::Clear()
{
	m_nSerial = 0;
	m_nLastShot = 0;
	m_nWritedSerial = 0;
	ClrFixPcs();
	ClrRst();
	ClrPcs();
	ClrPnlNum();
	ClrPnlDefNum();
}

int CReelMap::GetLastSerial()
{
	return m_nWritedSerial;
}

void CReelMap::SetFixPcs(int nSerial)
{
// 	if(!pDoc->WorkingInfo.LastJob.bContFixDef)
// 		return;

	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nR, nC, nDefCode;

	for(nR=0; nR<nNodeY; nR++)
	{
		for(nC=0; nC<nNodeX; nC++)
		{
			nDefCode = m_pPnlBuf[nSerial-1][nR][nC]; // DefCode
			if(nDefCode > 0)
				SetFixPcs(nSerial, nC, nR);
		}
	}
}

void CReelMap::SetFixPcs(int nSerial, int nCol, int nRow) // nShot : 0 ~ 
{
	if(nSerial < 1)
		return;

	int nRange = _tstoi(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	int nS = (nSerial-1) % nRange;
	m_FixPcs[nS][nCol][nRow] = TRUE;
}

void CReelMap::ClrFixPcs()
{
	m_nPrevSerial[0] = 0;	// --
	m_nPrevSerial[1] = 0;	// ++

	for(int nS=0; nS<FIX_PCS_SHOT_MAX; nS++)
	{
		if(!nS)
		{
			m_nPrevSerial[0] = 0; // [0] : --
			m_nPrevSerial[1] = 0; // [1] : ++
		}

		for(int nC=0; nC<FIX_PCS_COL_MAX; nC++)
		{
			for(int nR=0; nR<FIX_PCS_ROW_MAX; nR++)
			{
				m_FixPcs[nS][nC][nR] = FALSE;
				if(!nS)
				{
					m_FixPcsPrev[nC][nR] = 0;
				}		
			}
		}
	}
}

void CReelMap::ClrFixPcs(int nCol, int nRow)
{
	for(int nS=0; nS<FIX_PCS_SHOT_MAX; nS++)
	{
		m_FixPcs[nS][nCol][nRow] = FALSE;
	}
	m_FixPcsPrev[nCol][nRow] = 0;
}

BOOL CReelMap::IsFixPcs(int nSerial, int &Col, int &Row)
{
	BOOL bRtn = FALSE;
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;

	BOOL bOn = pDoc->WorkingInfo.LastJob.bContFixDef;
	int nRange = _tstoi(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	int nJudge = _tstoi(pDoc->WorkingInfo.LastJob.sNumContFixDef);

// 	if(nSerial < nJudge)
// 		return bRtn;

	if(m_nPrevSerial[0] == nSerial)
		return bRtn;
	m_nPrevSerial[0] = nSerial;

	int nS;

	for(int nC=0; nC<nNodeX; nC++)
	{
		for(int nR=0; nR<nNodeY; nR++)
		{
			if(nSerial >= nRange)
			{
				nS = (nSerial-1) % nRange;
				if(nS == nRange-1)
					nS = 0;
				else
					nS++;

				if(m_FixPcs[nS][nC][nR]) // Defect PCS
				{
					if(m_FixPcsPrev[nC][nR] > 0)
						m_FixPcsPrev[nC][nR]--;
				}
				m_FixPcs[nS][nC][nR] = FALSE;
			}

			nS = (nSerial-1) % nRange;
			if(m_FixPcs[nS][nC][nR]) // Defect PCS
			{
// 				if(m_nPrevSerial[1] != nSerial) // [1] : ++
// 				{
//					m_nPrevSerial[1] = nSerial;
					m_FixPcsPrev[nC][nR]++;
//				}
			}

			if(bOn)
			{
				if(m_FixPcsPrev[nC][nR] >= nJudge)
				{
					Col = nC;
					Row = nR;
					ClrFixPcs(nC, nR);
					bRtn = TRUE;
				}				
			}
		}
	}

	return bRtn;
}

BOOL CReelMap::IsFixPcs(int nSerial, int* pCol, int* pRow, int &nTot) // nTot : total of PCS Over nJudge
{
	BOOL bRtn = FALSE;
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;

	BOOL bOn = pDoc->WorkingInfo.LastJob.bContFixDef;
	int nRange = _tstoi(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	int nJudge = _tstoi(pDoc->WorkingInfo.LastJob.sNumContFixDef);

// 	if(nSerial < nJudge)
// 		return bRtn;

	if(m_nPrevSerial[0] == nSerial)
		return bRtn;
	m_nPrevSerial[0] = nSerial;

	int nS = (nSerial-1) % nRange;
	nTot = 0;

	for(int nC=0; nC<nNodeX; nC++)
	{
		for(int nR=0; nR<nNodeY; nR++)
		{
			if(nSerial >= nRange)
			{
				nS = (nSerial-1) % nRange;
				if(nS == nRange-1)
					nS = 0;
				else
					nS++;

				if(m_FixPcs[nS][nC][nR]) // Defect PCS
				{
					if(m_FixPcsPrev[nC][nR] > 0)
						m_FixPcsPrev[nC][nR]--;
				}
				m_FixPcs[nS][nC][nR] = FALSE;
			}

			nS = (nSerial-1) % nRange;
			if(m_FixPcs[nS][nC][nR]) // Defect PCS
			{
				m_FixPcsPrev[nC][nR]++;
			}

			if(bOn)
			{
				if(m_FixPcsPrev[nC][nR] >= nJudge)
				{
					pCol[nTot] = nC;
					pRow[nTot] = nR;
					nTot++;
					ClrFixPcs(nC, nR);
					bRtn = TRUE;
				}				
			}
		}
	}

	return bRtn;
}


BOOL CReelMap::MakeDir()
{
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//
//#ifndef TEST_MODE
//	CFileFind finder;
//	CString str, sPath, Path[4];
//
//	if(bDualTest)
//	{
//		switch(m_nLayer)
//		{
//		case RMAP_UP:
//			str = _T("ReelMapDataUp.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			break;
//		case RMAP_ALLUP:
//			str = _T("ReelMapDataAll.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			break;
//		case RMAP_DN:
//			str = _T("ReelMapDataDn.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//			//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			break;
//		case RMAP_ALLDN:
//			str = _T("ReelMapDataAll.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//			//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			break;
//		}
//	}
//	else
//	{
//		str = _T("ReelMapDataUp.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//	}

	CString str, sPath, Path[5];

		switch(m_nLayer)
		{
		case RMAP_UP:
			str = _T("ReelMapDataUp.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
			break;
		case RMAP_ALLUP:
			str = _T("ReelMapDataAll.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
			break;
		case RMAP_DN:
			str = _T("ReelMapDataDn.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
			break;
		case RMAP_ALLDN:
			str = _T("ReelMapDataAll.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
			break;
	case RMAP_INNER_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INNER_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INOUTER_UP:
		break;
	case RMAP_INOUTER_DN:
		break;
	case RMAP_ITS:
		str = _T("ReelMapDataIts.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = _T("");
		Path[4] = _T("");
		break;
	}

	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
	{
	sPath.Format(_T("%s%s"), Path[0], Path[1]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

		if (!Path[2].IsEmpty())
		{
	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

			if (!Path[3].IsEmpty())
			{
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

				if (!Path[4].IsEmpty())
				{
					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
					if (!pDoc->DirectoryExists(sPath))
						CreateDirectory(sPath, NULL);
				}
			}
		}
	}
//#endif
	return TRUE;
}

BOOL CReelMap::MakeDirRmap()
{
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//
//#ifndef TEST_MODE
//	CFileFind finder;
//	CString str, sPath, Path[4];
//
//	if(bDualTest)
//	{
//		switch(m_nLayer)
//		{
//		case RMAP_UP:
//			str = _T("ReelMapDataUp.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			break;
//		case RMAP_ALLUP:
//			str = _T("ReelMapDataAll.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			break;
//		case RMAP_DN:
//			str = _T("ReelMapDataDn.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//			//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			break;
//		case RMAP_ALLDN:
//			str = _T("ReelMapDataAll.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//			//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			break;
//		}
//	}
//	else
//	{
//		str = _T("ReelMapDataUp.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//	}
//
//	sPath.Format(_T("%s%s"), Path[0], Path[1]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s\\Reelmap"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//#endif

	CString str, sPath, Path[5];

	switch (m_nLayer)
		{
		case RMAP_UP:
			str = _T("ReelMapDataUp.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
			break;
		case RMAP_ALLUP:
			str = _T("ReelMapDataAll.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
			break;
		case RMAP_DN:
			str = _T("ReelMapDataDn.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
			break;
		case RMAP_ALLDN:
			str = _T("ReelMapDataAll.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_INNER_UP:
		str = _T("YieldUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_DN:
		str = _T("YieldDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INNER_ALLUP:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_ALLDN:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INOUTER_UP:
			break;
	case RMAP_INOUTER_DN:
		break;
	case RMAP_ITS:
		str = _T("YieldIts.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = _T("");
		Path[4] = _T("");
		break;
	}

	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
	{
	sPath.Format(_T("%s%s"), Path[0], Path[1]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

		if (!Path[2].IsEmpty())
		{
	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

			if (!Path[3].IsEmpty())
			{
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

				if (!Path[4].IsEmpty())
				{
					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
				}
			}
		}
	}

	return TRUE;
}

CString CReelMap::MakeDirRmapRestore()
{
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//	CString sPath=_T("");
//	
//#ifndef TEST_MODE
//	CFileFind finder;
//	CString str, Path[4];
//
//	if(bDualTest)
//	{
//		switch(m_nLayer)
//		{
//		case RMAP_UP:
//			str = _T("ReelMapDataUp.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			break;
//		case RMAP_ALLUP:
//			str = _T("ReelMapDataAll.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			break;
//		case RMAP_DN:
//			str = _T("ReelMapDataDn.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//			//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			break;
//		case RMAP_ALLDN:
//			str = _T("ReelMapDataAll.txt");
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//			//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			break;
//		}
//	}
//	else
//	{
//		str = _T("ReelMapDataUp.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//	}
//
//	sPath.Format(_T("%s%s"), Path[0], Path[1]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s\\Reelmap"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s\\Reelmap\\Restore"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//#endif

	CString str, sPath, Path[5];

	switch (m_nLayer)
		{
		case RMAP_UP:
			str = _T("ReelMapDataUp.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
			break;
		case RMAP_ALLUP:
			str = _T("ReelMapDataAll.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
			break;
		case RMAP_DN:
			str = _T("ReelMapDataDn.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
			break;
		case RMAP_ALLDN:
			str = _T("ReelMapDataAll.txt");
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_INNER_UP:
		str = _T("YieldUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_DN:
		str = _T("YieldDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INNER_ALLUP:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_ALLDN:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INOUTER_UP:
		break;
	case RMAP_INOUTER_DN:
			break;
	case RMAP_ITS:
		str = _T("YieldIts.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = _T("");
		Path[4] = _T("");
		break;
	}

	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
	{
	sPath.Format(_T("%s%s"), Path[0], Path[1]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

		if (!Path[2].IsEmpty())
		{
	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

			if (!Path[3].IsEmpty())
			{
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

				if (!Path[4].IsEmpty())
				{
					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

					sPath.Format(_T("%s%s\\%s\\%s\\%s\\Restore"), Path[0], Path[1], Path[2], Path[3], Path[4]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
				}
				else
	{
					sPath.Format(_T("%s%s\\%s\\%s\\Restore"), Path[0], Path[1], Path[2], Path[3]);
					if (!pDoc->DirectoryExists(sPath))
						CreateDirectory(sPath, NULL);
				}
			}
			else
		{
				sPath.Format(_T("%s%s\\%s\\Restore"), Path[0], Path[1], Path[2]);
				if (!pDoc->DirectoryExists(sPath))
					CreateDirectory(sPath, NULL);
		}
	}
	else
		{
			sPath.Format(_T("%s%s\\Restore"), Path[0], Path[1]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
		}
	}
	else
	{
		pView->MsgBox(_T("Error- Path[0].IsEmpty() || Path[1].IsEmpty()"));
		sPath = _T("");
	}

	return sPath;
}

BOOL CReelMap::MakeDir(CString sModel, CString sLayer, CString sLot)
{
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//
//#ifndef TEST_MODE
//	CFileFind finder;
//	CString str, sPath, Path[4];
//
//	if(bDualTest)
//	{
//		switch(m_nLayer)
//		{
//		case RMAP_UP:
//			str = _T("ReelMapDataUp.txt");
//			break;
//		case RMAP_ALLUP:
//			str = _T("ReelMapDataAll.txt");
//			break;
//		case RMAP_DN:
//			str = _T("ReelMapDataDn.txt");
//			break;
//		case RMAP_ALLDN:
//			str = _T("ReelMapDataAll.txt");
//			break;
//		}
//	}
//	else
//		str = _T("ReelMapDataUp.txt");
//	
//	Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//	Path[1] = sModel;
//	Path[2] = sLot;
//	Path[3] = sLayer;
//
//	sPath.Format(_T("%s%s"), Path[0], Path[1]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//#endif
	return TRUE;
}

BOOL CReelMap::MakeDirRmap(CString sModel, CString sLayer, CString sLot)
{
	CString str, sPath, Path[5];

	switch (m_nLayer)
		{
		case RMAP_UP:
			str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = sModel;
		Path[2] = sLot;
		Path[3] = sLayer;
		Path[4] = _T("");
			break;
		case RMAP_ALLUP:
			str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = sModel;
		Path[2] = sLot;
		Path[3] = sLayer;
		Path[4] = _T("");
			break;
		case RMAP_DN:
			str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = sModel;
		Path[2] = sLot;
		Path[3] = sLayer;
		Path[4] = _T("");
			break;
		case RMAP_ALLDN:
			str = _T("ReelMapDataAll.txt");
	Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	Path[1] = sModel;
	Path[2] = sLot;
	Path[3] = sLayer;
		Path[4] = _T("");
		break;
	case RMAP_INNER_UP:
		str = _T("YieldUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = sLot;
		Path[4] = sLayer;
		break;
	case RMAP_INNER_DN:
		str = _T("YieldDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = sLot;
		Path[4] = sLayer;
		break;
	case RMAP_INNER_ALLUP:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = sLot;
		Path[4] = sLayer;
		break;
	case RMAP_INNER_ALLDN:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = sLot;
		Path[4] = sLayer;
		break;
	case RMAP_INOUTER_UP:
		break;
	case RMAP_INOUTER_DN:
		break;
	case RMAP_ITS:
		str = _T("YieldIts.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = _T("");
		Path[4] = _T("");
		break;
	}

	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
	{
	sPath.Format(_T("%s%s"), Path[0], Path[1]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

		if (!Path[2].IsEmpty())
		{
	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

			if (!Path[3].IsEmpty())
			{
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

				if (!Path[4].IsEmpty())
				{
					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
				}
			}
		}
	}


//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//
//#ifndef TEST_MODE
//	CFileFind finder;
//	CString str, sPath, Path[4];
//
//	if(bDualTest)
//	{
//		switch(m_nLayer)
//		{
//		case RMAP_UP:
//			str = _T("ReelMapDataUp.txt");
//			break;
//		case RMAP_ALLUP:
//			str = _T("ReelMapDataAll.txt");
//			break;
//		case RMAP_DN:
//			str = _T("ReelMapDataDn.txt");
//			break;
//		case RMAP_ALLDN:
//			str = _T("ReelMapDataAll.txt");
//			break;
//		}
//	}
//	else
//		str = _T("ReelMapDataUp.txt");
//	
//	Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//	Path[1] = sModel;
//	Path[2] = sLot;
//	Path[3] = sLayer;
//
//	sPath.Format(_T("%s%s"), Path[0], Path[1]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s\\Reelmap"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//#endif
	return TRUE;
}

CString CReelMap::MakeDirRmapRestore(CString sModel, CString sLayer, CString sLot)
{
	CString str, sPath, Path[5];

	switch (m_nLayer)
		{
		case RMAP_UP:
			str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = sModel;
		Path[2] = sLot;
		Path[3] = sLayer;
		Path[4] = _T("");
			break;
		case RMAP_ALLUP:
			str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = sModel;
		Path[2] = sLot;
		Path[3] = sLayer;
		Path[4] = _T("");
			break;
		case RMAP_DN:
			str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = sModel;
		Path[2] = sLot;
		Path[3] = sLayer;
		Path[4] = _T("");
			break;
		case RMAP_ALLDN:
			str = _T("ReelMapDataAll.txt");
	Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	Path[1] = sModel;
	Path[2] = sLot;
	Path[3] = sLayer;
		Path[4] = _T("");
		break;
	case RMAP_INNER_UP:
		str = _T("YieldUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = sLot;
		Path[4] = sLayer;
		break;
	case RMAP_INNER_DN:
		str = _T("YieldDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = sLot;
		Path[4] = sLayer;
		break;
	case RMAP_INNER_ALLUP:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = sLot;
		Path[4] = sLayer;
		break;
	case RMAP_INNER_ALLDN:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = sLot;
		Path[4] = sLayer;
		break;
	case RMAP_INOUTER_UP:
		break;
	case RMAP_INOUTER_DN:
		break;
	case RMAP_ITS:
		str = _T("YieldIts.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = Path[1] = sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = _T("");
		Path[4] = _T("");
		break;
	}

	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
	{
	sPath.Format(_T("%s%s"), Path[0], Path[1]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

		if (!Path[2].IsEmpty())
		{
	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

			if (!Path[3].IsEmpty())
			{
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

				if (!Path[4].IsEmpty())
				{
					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
				}
			}
		}
	}


//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//	CString sPath;
//#ifndef TEST_MODE
//	CFileFind finder;
//	CString str, Path[4];
//
//	if(bDualTest)
//	{
//		switch(m_nLayer)
//		{
//		case RMAP_UP:
//			str = _T("ReelMapDataUp.txt");
//			break;
//		case RMAP_ALLUP:
//			str = _T("ReelMapDataAll.txt");
//			break;
//		case RMAP_DN:
//			str = _T("ReelMapDataDn.txt");
//			break;
//		case RMAP_ALLDN:
//			str = _T("ReelMapDataAll.txt");
//			break;
//		}
//	}
//	else
//		str = _T("ReelMapDataUp.txt");
//	
//	Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//	Path[1] = sModel;
//	Path[2] = sLot;
//	Path[3] = sLayer;
//
//	sPath.Format(_T("%s%s"), Path[0], Path[1]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s\\Reelmap"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s\\Reelmap\\Restore"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//#endif
	return sPath;
}

void CReelMap::SetPathAtBuf()
{
	m_sPathBuf = GetRmapPath(m_nLayer);
}

//CString CReelMap::GetRmapPath(int nRmap)
//{
//	CString sPath;
//#ifdef TEST_MODE
//	sPath = PATH_REELMAP;
//#else
//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//
//	CString str;
//
//	if(bDualTest)
//	{
//		switch(nRmap)
//		{
//		case RMAP_UP:
//			str = _T("ReelMapDataUp.txt");
//			if(pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
//			{
//				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//												pDoc->WorkingInfo.LastJob.sModelUp, 
//												pDoc->WorkingInfo.LastJob.sLotUp, 
//												pDoc->WorkingInfo.LastJob.sLayerUp,
//												str);
//			}
//			else if(!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
//			{
//				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//												pDoc->WorkingInfo.LastJob.sModelUp, 
//												pDoc->Status.PcrShare[0].sLot, 
//												pDoc->WorkingInfo.LastJob.sLayerUp,
//												str);
//			}
//			break;
//		case RMAP_ALLUP:
//			#ifdef TEST_MODE
//			str = _T("ReelMapDataAllUp.txt");
//			#else
//			str = _T("ReelMapDataAll.txt");
//			#endif
//			if(pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
//			{
//				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//												pDoc->WorkingInfo.LastJob.sModelUp, 
//												pDoc->WorkingInfo.LastJob.sLotUp, 
//												pDoc->WorkingInfo.LastJob.sLayerUp,
//												str);
//			}
//			else if(!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
//			{
//				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//												pDoc->WorkingInfo.LastJob.sModelUp, 
//												pDoc->Status.PcrShare[0].sLot, 
//												pDoc->WorkingInfo.LastJob.sLayerUp,
//												str);
//			}
//			break;
//		case RMAP_DN:
//			str = _T("ReelMapDataDn.txt");
//			if(pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
//			{
//				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//												//pDoc->WorkingInfo.LastJob.sModelDn, 
//												//pDoc->WorkingInfo.LastJob.sLotDn, 
//												pDoc->WorkingInfo.LastJob.sModelUp,
//												pDoc->WorkingInfo.LastJob.sLotUp,
//												pDoc->WorkingInfo.LastJob.sLayerDn,
//												str);
//			}
//			else if(!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
//			{
//				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//												//pDoc->WorkingInfo.LastJob.sModelDn, 
//												//pDoc->Status.PcrShare[1].sLot, 
//												pDoc->WorkingInfo.LastJob.sModelUp,
//												pDoc->Status.PcrShare[0].sLot,
//												pDoc->WorkingInfo.LastJob.sLayerDn,
//												str);
//			}
//			break;
//		case RMAP_ALLDN:
//			#ifdef TEST_MODE
//			str = _T("ReelMapDataAllDn.txt");
//			#else
//			str = _T("ReelMapDataAll.txt");
//			#endif
//			if(pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
//			{
//				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//												//pDoc->WorkingInfo.LastJob.sModelDn, 
//												//pDoc->WorkingInfo.LastJob.sLotDn, 
//												pDoc->WorkingInfo.LastJob.sModelUp,
//												pDoc->WorkingInfo.LastJob.sLotUp,
//												pDoc->WorkingInfo.LastJob.sLayerDn,
//												str);
//			}
//			else if(!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
//			{
//				sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//												//pDoc->WorkingInfo.LastJob.sModelDn, 
//												//pDoc->Status.PcrShare[1].sLot, 
//												pDoc->WorkingInfo.LastJob.sModelUp,
//												pDoc->Status.PcrShare[0].sLot,
//												pDoc->WorkingInfo.LastJob.sLayerDn,
//												str);
//			}
//			break;
//		}
//	}
//	else
//	{
//		str = _T("ReelMapDataUp.txt");
//		if(pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
//		{
//			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//											pDoc->WorkingInfo.LastJob.sModelUp, 
//											pDoc->WorkingInfo.LastJob.sLotUp, 
//											pDoc->WorkingInfo.LastJob.sLayerUp,
//											str);
//		}
//		else if(!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
//		{
//			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile, 
//											pDoc->WorkingInfo.LastJob.sModelUp, 
//											pDoc->Status.PcrShare[0].sLot, 
//											pDoc->WorkingInfo.LastJob.sLayerUp,
//											str);
//		}
//	}
//#endif
//	return sPath;
//}

CString CReelMap::GetRmapPath(int nRmap)
{
	//CString sPath = _T("");
	//CString Path[5], str;

	//switch (nRmap)
	//{
	//case RMAP_UP:
	//	str = _T("ReelMapDataUp.txt");
	//	if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->WorkingInfo.LastJob.sLotUp,
	//			pDoc->WorkingInfo.LastJob.sLayerUp,
	//			str);
	//	}
	//	else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->Status.PcrShare[0].sLot,
	//			pDoc->WorkingInfo.LastJob.sLayerUp,
	//			str);
	//	}
	//	break;
	//case RMAP_ALLUP:
	//	str = _T("ReelMapDataAll.txt");
	//	if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->WorkingInfo.LastJob.sLotUp,
	//			pDoc->WorkingInfo.LastJob.sLayerUp,
	//			str);
	//	}
	//	else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->Status.PcrShare[0].sLot,
	//			pDoc->WorkingInfo.LastJob.sLayerUp,
	//			str);
	//	}
	//	break;
	//case RMAP_DN:
	//	str = _T("ReelMapDataDn.txt");
	//	if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->WorkingInfo.LastJob.sLotUp,
	//			pDoc->WorkingInfo.LastJob.sLayerDn,
	//			str);
	//	}
	//	else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->Status.PcrShare[0].sLot,
	//			pDoc->WorkingInfo.LastJob.sLayerDn,
	//			str);
	//	}
	//	break;
	//case RMAP_ALLDN:
	//	str = _T("ReelMapDataAll.txt");
	//	if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->WorkingInfo.LastJob.sLotUp,
	//			pDoc->WorkingInfo.LastJob.sLayerDn,
	//			str);
	//	}
	//	else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
	//	{
	//		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
	//			pDoc->WorkingInfo.LastJob.sModelUp,
	//			pDoc->Status.PcrShare[0].sLot,
	//			pDoc->WorkingInfo.LastJob.sLayerDn,
	//			str);
	//	}
	//	break;
	//case RMAP_INNER_UP:
	//	str = _T("ReelMapDataUp.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//	Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
	//	Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
	//	sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
	//	break;
	//case RMAP_INNER_DN:
	//	str = _T("ReelMapDataDn.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//	Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
	//	Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
	//	sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
	//	break;
	//case RMAP_INNER_ALLUP:
	//	str = _T("ReelMapDataAll.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//	Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
	//	Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
	//	sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
	//	break;
	//case RMAP_INNER_ALLDN:
	//	str = _T("ReelMapDataAll.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//	Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
	//	Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
	//	sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
	//	break;
	//case RMAP_INOUTER_UP:
	//	str = _T("ReelMapDataIO.txt");
	//	sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
	//		pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
	//		pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerUp,
	//		str);
	//	break;
	//case RMAP_INOUTER_DN:
	//	str = _T("ReelMapDataIO.txt");
	//	sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
	//		pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
	//		pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerDn,
	//		str);
	//	break;
	//case RMAP_ITS:
	//	pDoc->GetCurrentInfo();
	//	str = _T("ReelMapDataIts.txt");
	//	sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
	//		pDoc->WorkingInfo.LastJob.sModelUp,
	//		pDoc->WorkingInfo.LastJob.sEngItsCode,	//pDoc->m_sItsCode,
	//		str);
	//	break;
	//}

	CString sPath = pView->GetMkMenu01(_T("DispDefImg"), _T("ReelmapPath"));
	return sPath;
}

void CReelMap::UpdateProcessNum(CString sProcessNum, int nLayer)
{
	::WritePrivateProfileString(_T("Info"), _T("Process Code"), sProcessNum, GetRmapPath(nLayer));
}

void CReelMap::UpdateTotVel(CString sVel, int nLayer)
{
	::WritePrivateProfileString(_T("Info"), _T("Entire Speed"), sVel, GetRmapPath(nLayer));
}



void CReelMap::StartThreadRemakeReelmap() 
{
	return;

	m_bRtnThreadRemakeReelmap = FALSE;
	m_bThreadAliveRemakeReelmap = TRUE;	
	m_ThreadTaskRemakeReelmap.Start( GetSafeHwnd(),this,ThreadProcRemakeReelmap);// Start the thread
}

void CReelMap::StopThreadRemakeReelmap() 
{
	return;

	m_ThreadTaskRemakeReelmap.Stop();// Stop the thread
}

// Home thread body
BOOL CReelMap::ThreadProcRemakeReelmap( LPVOID lpContext )
{
	return TRUE;

	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CReelMap* pThread = reinterpret_cast< CReelMap* >( lpContext );
//	DWORD dwTimePeriod = 10; // 10ms sec sleep

	pThread->m_bRtnThreadRemakeReelmap = FALSE;
	pThread->m_bThreadAliveRemakeReelmap = TRUE;	
	
	pThread->m_cs.Lock();
	pThread->m_bRtnThreadRemakeReelmap = pThread->RemakeReelmap();
	pThread->m_cs.Unlock();

	pThread->m_bThreadAliveRemakeReelmap = FALSE;

	return (pThread->m_bRtnThreadRemakeReelmap);
}


void CReelMap::RemakeReelmap(CString sPath)
{
	return;

	m_sPathOnThread = sPath;
	StartThreadRemakeReelmap();
}

BOOL CReelMap::RemakeReelmap()
{
	return TRUE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CString sPath = m_sPathOnThread;
	FILE *fp = NULL;
	char FileName[MAX_PATH];
	CString strFileName, strPathName;
	int i;// , j, k, l;
	//BOOL bDefOn;
	CFileFind findfile;
	//BOOL bFileCreate;
	int nStripNumY, nPieceNumPerStrip;

	CString sModel, sLot, sLayer[2];
	int nLastShot, nPnl, nRow, nCol, nDefCode, nCompletedShot;// , nStrip;//, nC, nR;
	CString sPnl, sRow, sVal;
	TCHAR sep[] = {_T(",/;\r\n\t")};
	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Completed Shot"), NULL, szData, sizeof(szData), sPath))
		nCompletedShot = _tstoi(szData);
	else
	{
		nCompletedShot = 0; // Failed.
		//pView->MsgBox(_T("릴맵에 Completed Shot 정보가 없습니다."));
		//return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), sPath))
		nLastShot = _tstoi(szData); 
	else
	{
		nLastShot = 0; // Failed.
		pView->MsgBox(_T("릴맵에 Marked Shot 정보가 없습니다."));
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("모      델"), NULL, szData, sizeof(szData), sPath))
		sModel = CString(szData); 
	else
	{
		pView->MsgBox(_T("Model 정보가 없습니다."));
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("로      트"), NULL, szData, sizeof(szData), sPath))
		sLot = CString(szData); 
	else
	{
		pView->MsgBox(_T("Lot 정보가 없습니다."));
		return FALSE;
	}
	
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("상면레이어"), NULL, szData, sizeof(szData), sPath))
		sLayer[0] = CString(szData); 
	else
	{
		pView->MsgBox(_T("상면레이어 정보가 없습니다."));
		return FALSE;
	}

	if(bDualTest)
	{
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("하면레이어"), NULL, szData, sizeof(szData), sPath))
			sLayer[1] = CString(szData); 
		else
		{
			pView->MsgBox(_T("하면레이어 정보가 없습니다."));
			return FALSE;
		}
	}

	MakeDirRmap(sModel, sLayer[0], sLot);

	if(bDualTest)
		MakeDirRmap(sModel, sLayer[1], sLot);

	CString sFile=_T(""), sRmapPath=sPath;
	int nPos = sRmapPath.ReverseFind('\\');
	if (nPos != -1)
	{
		sFile = sRmapPath.Right(sRmapPath.GetLength()-nPos-1);
		sRmapPath.Delete(nPos, sPath.GetLength() - nPos);
	}

// 	if(bDualTest)
// 	{
// 		if(sFile == _T("ReelmapDataAll.txt"))
// 			strPathName = sRmapPath + _T("\\Reelmap\\") + _T("ReelMap-TOTAL_SIDE.txt");
// 		else if(sFile == _T("ReelmapDataUp.txt"))
// 			strPathName = sRmapPath + _T("\\Reelmap\\") + _T("ReelMap-TOP_SIDE.txt");
// 		else if(sFile == _T("ReelmapDataDn.txt"))
// 			strPathName = sRmapPath + _T("\\Reelmap\\") + _T("ReelMap-BOTTOM_SIDE.txt");
// 	}
// 	else
// 		strPathName = sRmapPath + _T("\\Reelmap\\") + _T("ReelMap-TOP_SIDE.txt");

	sFile.MakeUpper();
	if(bDualTest)
	{
		if(sFile == _T("REELMAPDATAALL.TXT"))
			strPathName = sRmapPath + _T("\\Reelmap\\") + _T("ReelMap-TOTAL_SIDE.txt");
		else if(sFile == _T("REELMAPDATAUP.TXT"))
			strPathName = sRmapPath + _T("\\Reelmap\\") + _T("ReelMap-TOP_SIDE.txt");
		else if(sFile == _T("REELMAPDATADN.TXT"))
			strPathName = sRmapPath + _T("\\Reelmap\\") + _T("ReelMap-BOTTOM_SIDE.txt");
	}
	else
		strPathName = sRmapPath + _T("\\Reelmap\\") + _T("ReelMap-TOP_SIDE.txt");

	if(findfile.FindFile(strPathName))
	{
		DeleteFile(strPathName);
	}


	int nNodeX=0;// = pDoc->m_pPcsRgn->nCol; // on Cam
	int nNodeY=0;// = pDoc->m_pPcsRgn->nRow; // on Cam
	
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("CamPcsX"), NULL, szData, sizeof(szData), sPath))
		nNodeX = _tstoi(szData); 
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("CamPcsY"), NULL, szData, sizeof(szData), sPath))
		nNodeY = _tstoi(szData); 

	int nX, nY, nSt=0;
	if(!nNodeX || !nNodeY)
	{
		for(nY=0; nY<10000; nY++)
		{
			sPnl = _T("1");
			sRow.Format(_T("%02d"), nY);
			if(0 >= ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), sPath))
			{
				nNodeX = nY;
				break;
			}
			else
			{
				if(!nNodeY)
				{
					CString sLine = CString(szData);
					for(nX=0; nX<100000; nX++)
					{
						nSt = sLine.Find(',', nSt+1);
						if(nSt < 0)
							break;
						else
							nNodeY++;
					}
					nNodeY++;
				}
			}
		}
	}
	if(!nNodeX || !nNodeY)
	{
		nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol; // on Cam
		nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow; // on Cam
	}

	nStripNumY = MAX_STRIP_NUM;
	nPieceNumPerStrip = nNodeY/nStripNumY;

// 	double dRatio=0.0;
// 	CDlgProgress dlg;
// 	sVal.Format(_T("On remaking [%s]"), sFile);
// 	dlg.Create(sVal);
	m_dProgressRatio = 0.0;


	//_tcscpy(FileName, strPathName);
	StringToChar(strPathName, FileName);

	fp = fopen(FileName, "a+");
	char* pRtn = NULL;
	if (fp != NULL)
	{
		fprintf(fp, "장비호기명 : %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "운  용  자 : %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "모      델 : %s\n", pRtn = StrToChar(sModel)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "로      트 : %s\n", pRtn = StrToChar(sLot)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "상면레이어 : %s\n", pRtn = StrToChar(sLayer[0])); if (pRtn) delete pRtn; pRtn = NULL;
		if(bDualTest)
		{
			fprintf(fp, "하면레이어 : %s\n", pRtn = StrToChar(sLayer[1])); if (pRtn) delete pRtn; pRtn = NULL;
		}
		fprintf(fp, "양폐 스트립 수율[%%] : %.1f\n", _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio)); if (pRtn) delete pRtn; pRtn = NULL;//atof
		
		for(i=1; i<DEF_UPPER; i++)
		{
			fprintf(fp, "%c - %s\n", pDoc->m_cBigDefCode[i], pRtn = StrToChar(m_sKorDef[i])); if (pRtn) delete pRtn; pRtn = NULL;
		}
		fprintf(fp, "? - Missed Align Panel(i.e 노광불량)\r\n\n");

		// Reload Reelmap.....
		for(nPnl=0; nPnl<nLastShot; nPnl++)
		{
			m_dProgressRatio =  double(nPnl+1)/double(nLastShot) * 100.0;
// 			dRatio = double(nPnl+1)/double(nLastShot) * 100.0;
// 			if(!(int(dRatio)%10))
// 				dlg.SetPos(int(dRatio));

			fprintf(fp, "%d\n", nPnl+1);
			for(nRow=0; nRow<nNodeX; nRow++)
			{
				sPnl.Format(_T("%d"), nPnl+1);
				sRow.Format(_T("%02d"), nRow);
				if (0 < ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), sPath))
				{
					for(nCol=0; nCol<nNodeY; nCol++)
					{
						if(nCol==0)
							sVal = _tcstok(szData,sep);
						else
							sVal = _tcstok(NULL,sep);

						nDefCode = _tstoi(sVal);

// 						nR = (nNodeY-1)-nCol;
// 						nC = nRow;

						if(nCol % nPieceNumPerStrip == 0)
							fprintf(fp, " ");

						fprintf(fp, "%c", pDoc->m_cBigDefCode[nDefCode]);
					}
					fprintf(fp, "\n");
				}
			}
			fprintf(fp, "\n");
		}		
	}
	else
	{
		pView->MsgBox(_T("It is trouble to remake ReelMap."),MB_ICONWARNING|MB_OK);
		return FALSE;
	}

// 	dlg.DestroyWindow();

	fclose(fp);
	return TRUE;
}

void CReelMap::RestoreReelmap()
{
	return;

	CString sPath = MakeDirRmapRestore();
	int nLastDir = GetLastRmapRestoreDir(sPath);

	CString sUserRestoreDir;
	sUserRestoreDir.Format(_T("%s\\%d"), sPath, nLastDir+1);
	
	CFileFind finder;
	if (!pDoc->DirectoryExists(sUserRestoreDir))
		CreateDirectory(sUserRestoreDir, NULL);


	//BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	//CString str, Path[4], sSrc, sDest;

	//if(bDualTest)
	//{
	//	switch(m_nLayer)
	//	{
	//	case RMAP_UP:
	//		str = _T("ReelMapDataUp.txt");
	//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
	//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
	//		break;
	//	case RMAP_ALLUP:
	//		str = _T("ReelMapDataAll.txt");
	//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
	//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
	//		break;
	//	case RMAP_DN:
	//		str = _T("ReelMapDataDn.txt");
	//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	//		//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
	//		//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
	//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
	//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
	//		break;
	//	case RMAP_ALLDN:
	//		str = _T("ReelMapDataAll.txt");
	//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	//		//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
	//		//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
	//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
	//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
	//		break;
	//	}
	//}
	//else
	//{
	//	str = _T("ReelMapDataUp.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
	//	Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
	//}

	//CString sPath = _T("");
	CString Path[5], str, sSrc, sDest;

	switch (m_nLayer)
		{
		case RMAP_UP:
			str = _T("ReelMapDataUp.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLotUp,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
			break;
		case RMAP_ALLUP:
			str = _T("ReelMapDataAll.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLotUp,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
			break;
		case RMAP_DN:
			str = _T("ReelMapDataDn.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLotUp,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
			break;
		case RMAP_ALLDN:
			str = _T("ReelMapDataAll.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLotUp,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
	}
		break;
	case RMAP_INNER_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INOUTER_UP:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_INOUTER_DN:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ITS:
		pDoc->GetCurrentInfo();
		str = _T("ReelMapDataIts.txt");
		sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,	//pDoc->m_sItsCode,
			str);
		break;
	}

	sSrc = sPath;
	sDest.Format(_T("%s\\%s"), sUserRestoreDir, str);

	CMyFile *pFile;
	pFile = new CMyFile();

	BOOL bRst = pFile->Copy(sSrc, sDest);			// 파일을 복사한다.

	delete pFile;
}

int CReelMap::GetLastRmapRestoreDir(CString strPath)
{
	CString strFileName;

	CFileFind cFile;
	BOOL bExist = cFile.FindFile(strPath);
	if(!bExist)
	{
		return 0; // 존재하지 않음.
	}

	int nFirstFolder = 1000000;
	int nLastFolder = 0;

	strPath += _T("\\*.*");
	bExist = cFile.FindFile(strPath);

	while(bExist)
	{
		bExist = cFile.FindNextFile();
		if(cFile.IsDots()) continue;
		if(cFile.IsDirectory())
		{
			// 폴더명을 얻음.
			strFileName = cFile.GetFileName();

			int nFileNum = _tstoi(strFileName);
			if(nFileNum < nFirstFolder)
				nFirstFolder = nFileNum;
			if(nFileNum > nLastFolder)
				nLastFolder = nFileNum;
		}
	}

	return nLastFolder;
}




void CReelMap::StartThreadReloadRst() 
{
	m_bRtnThreadReloadRst = FALSE;
	m_bThreadAliveReloadRst = TRUE;	
	m_ThreadTaskReloadRst.Start( GetSafeHwnd(),this,ThreadProcReloadRst);// Start the thread
}

void CReelMap::StopThreadReloadRst() 
{
	m_ThreadTaskReloadRst.Stop();// Stop the thread
}

// Home thread body
BOOL CReelMap::ThreadProcReloadRst( LPVOID lpContext )
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CReelMap* pThread = reinterpret_cast< CReelMap* >( lpContext );
//	DWORD dwTimePeriod = 10; // 10ms sec sleep

	pThread->m_bRtnThreadReloadRst = FALSE;
	pThread->m_bThreadAliveReloadRst = TRUE;	
	
	pThread->m_cs.Lock();
	int nSerial = pDoc->GetLastShotMk();	// m_pDlgFrameHigh에서 얻거나 없으면, sPathOldFile폴더의 ReelMapDataDn.txt에서 _T("Info"), _T("Marked Shot") 찾음.
	pThread->m_bRtnThreadReloadRst = pThread->ReloadRst(pThread->m_nLastOnThread);
	pThread->m_cs.Unlock();

	pThread->m_bThreadAliveReloadRst = FALSE;

	return (pThread->m_bRtnThreadReloadRst);
}


BOOL CReelMap::IsDoneReloadRst()
{
	return m_bDoneReloadRst;
}

int CReelMap::GetProgressReloadRst()
{
	if(m_nTotalProgressReloadRst <= 0)
		return 0;

	double dA = (double)m_nProgressReloadRst;
	double dB = (double)m_nTotalProgressReloadRst;
	double dC = 100.0 * dA / dB;
	int nC = int(dC);

	return nC;
}

BOOL CReelMap::ReloadRst()
{
	//int nSerial;
	BOOL bRtn;
	
	//nSerial = pDoc->GetLastShotMk();
	int nLastOnThread = pDoc->GetLastShotMk();	// m_pDlgFrameHigh에서 얻거나 없으면, sPathOldFile폴더의 ReelMapDataDn.txt에서 _T("Info"), _T("Marked Shot") 찾음.
	m_nLastOnThread = nLastOnThread;	// m_pDlgFrameHigh에서 얻거나 없으면, sPathOldFile폴더의 ReelMapDataDn.txt에서 _T("Info"), _T("Marked Shot") 찾음.

	//if(nSerial > 0)
	if(m_nLastOnThread > 0)
	{
		//bRtn = ReloadRst(nSerial);
		m_bDoneReloadRst = FALSE;
		m_nProgressReloadRst = 0;
		m_nTotalProgressReloadRst = 0;
		StartThreadReloadRst();
		bRtn = TRUE;
	}
	else 
		bRtn = FALSE;

	return bRtn;
}

BOOL CReelMap::ReloadRst(int nTo)
{
	if(!m_pPnlBuf)
	{
		AfxMessageBox(_T("Memory not alloced.- PnlBuf"));
		m_bDoneReloadRst = TRUE;
		return FALSE;
	}

	int nPnl, nRow, nCol, nDefCode, nStrip, nC, nR;
	CString sPnl, sRow, sVal;
	//char sep[] = {",/;\r\n\t"};
	//char szData[MAX_PATH];
	TCHAR sep[] = { _T(",/;\r\n\t") };
	TCHAR szData[MAX_PATH];

	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nTotPcs = nNodeX * nNodeY;
	int nStripPcs = nTotPcs / 4;

	int nDefStrip[4];
	

//  InitRst();
	ClrRst();
	m_nTotPcs = 0;
	m_nGoodPcs = 0;
	m_nBadPcs = 0;
	
	m_nTotalProgressReloadRst = nTo*(nNodeX*nNodeY+4)+4*MAX_DEF+MAX_DEF;
	m_nProgressReloadRst=0;

	for(nPnl=0; nPnl<nTo; nPnl++)
	{
		nDefStrip[0] = 0; nDefStrip[1] = 0; nDefStrip[2] = 0; nDefStrip[3] = 0;

		for(nRow=0; nRow<nNodeX; nRow++)
		{
			sPnl.Format(_T("%d"), nPnl+1);
			sRow.Format(_T("%02d"), nRow);
// 			sRow.Format(_T("%d"), nRow+1);
			if (0 < ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), m_sPathBuf))
			{
				for(nCol=0; nCol<nNodeY; nCol++)
				{
					m_nProgressReloadRst++;

					if(nCol==0)
						sVal = _tcstok(szData,sep);
					else
						sVal = _tcstok(NULL,sep);//strtok

					nDefCode = _tstoi(sVal);

					nR = (nNodeY-1)-nCol;
					nC = nRow;

					m_pPnlBuf[nPnl][nR][nC] = (short)nDefCode;
					m_nDef[nDefCode]++;

					nStrip = int(nR / (nNodeY/4));
					if(nStrip > -1 && nStrip < 4)
					{
						if(nDefCode > 0)
						{
							nDefStrip[nStrip]++;
							m_nDefStrip[nStrip]++;
							m_nDefPerStrip[nStrip][nDefCode]++;
						}
					}

					m_nTotPcs++;
					if(nDefCode > 0)
						m_nBadPcs++;
					else
						m_nGoodPcs++;
				}
			}
		}

		double dStOutRto = _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0; //atof
		for(nStrip=0; nStrip<4; nStrip++)
		{
			m_nProgressReloadRst++;

			if(nDefStrip[nStrip] >= nStripPcs * dStOutRto)
				m_nStripOut[nStrip]++;
		}
	}


	// 수율 데이타를 갱신함.

	// Piece infomation..............
	CString strData, strMenu, strItem;
	int k, i;

	strData.Format(_T("%d"), m_nTotPcs);
	::WritePrivateProfileString(_T("Info"), _T("Total Pcs"), strData, m_sPathBuf);

	strData.Format(_T("%d"), m_nGoodPcs);
	::WritePrivateProfileString(_T("Info"), _T("Good Pcs"), strData, m_sPathBuf);

	strData.Format(_T("%d"), m_nBadPcs);
	::WritePrivateProfileString(_T("Info"), _T("Bad Pcs"), strData, m_sPathBuf);

	int nTotStOut = 0;
	for(k=0; k<4; k++)
	{
		strMenu.Format(_T("Strip%d"), k);
		strData.Format(_T("%d"), m_nDefStrip[k]);
		::WritePrivateProfileString(_T("Info"), strMenu, strData, m_sPathBuf);
		
		strMenu.Format(_T("%d"), k);
		strData.Format(_T("%d"), m_nStripOut[k]);
		::WritePrivateProfileString(_T("StripOut"), strMenu, strData, m_sPathBuf);
		nTotStOut += m_nStripOut[k];

		for(i=1; i<MAX_DEF; i++)
		{
			m_nProgressReloadRst++;

			strItem.Format(_T("Strip%d"), k);
			strMenu.Format(_T("%d"), i);
			strData.Format(_T("%d"), m_nDefPerStrip[k][i]);
			::WritePrivateProfileString(strItem, strMenu, strData, m_sPathBuf);
		}			
	}
	strData.Format(_T("%d"), m_nTotStOut=nTotStOut);
	::WritePrivateProfileString(_T("StripOut"), _T("Total"), strData, m_sPathBuf);

	for(i=1; i<MAX_DEF; i++)
	{
		m_nProgressReloadRst++;

		strMenu.Format(_T("%d"), i);
		strData.Format(_T("%d"), m_nDef[i]); // 불량이름별 불량수
		::WritePrivateProfileString(_T("Info"), strMenu, strData, m_sPathBuf);
	}

	m_bDoneReloadRst = TRUE;
	return TRUE;
}


char* CReelMap::StrToChar(CString str) // char* returned must be deleted... 
{
	char*		szStr = NULL;
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 
	szStr = new char[nLenth];  //메모리 할당 

							   //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, szStr, nLenth, 0, 0);
	return szStr;
}

void CReelMap::StrToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

																			  //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}

BOOL CReelMap::MakeDirYield(CString sPath)
{
	CString str, Path[5];

	switch (m_nLayer)
	{
	case RMAP_UP:
		str = _T("YieldUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_ALLUP:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_DN:
		str = _T("YieldDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_ALLDN:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_INNER_UP:
		str = _T("YieldUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_DN:
		str = _T("YieldDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INNER_ALLUP:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_ALLDN:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INOUTER_UP:
		break;
	case RMAP_INOUTER_DN:
		break;
	case RMAP_ITS:
		str = _T("YieldIts.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = _T("");
		Path[4] = _T("");
		break;
	}

	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
	{
		sPath.Format(_T("%s%s"), Path[0], Path[1]);
		if (!pDoc->DirectoryExists(sPath))
			CreateDirectory(sPath, NULL);

		if (!Path[2].IsEmpty())
		{
			sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
			if (!pDoc->DirectoryExists(sPath))
				CreateDirectory(sPath, NULL);

			if (!Path[3].IsEmpty())
			{
				sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
				if (!pDoc->DirectoryExists(sPath))
					CreateDirectory(sPath, NULL);

				if (!Path[4].IsEmpty())
				{
					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
					if (!pDoc->DirectoryExists(sPath))
						CreateDirectory(sPath, NULL);
				}
			}
		}
	}

	return TRUE;


//	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
//	CFileFind finder;
//	CString str, Path[4];
//
//
//	if (bDualTest)
//	{
//		switch (m_nLayer)
//		{
//		case RMAP_UP:
//			str = _T("YieldUp.txt");
//			if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
//			{
//				Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//				Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//				Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//				Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			}
//			else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
//			{
//				Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//				Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//				Path[2] = pDoc->Status.PcrShare[0].sLot;
//				Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			}
//			break;
//		case RMAP_ALLUP:
//#ifdef TEST_MODE
//			str = _T("YieldAllUp.txt");
//#else
//			str = _T("YieldAll.txt");
//#endif
//			if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
//			{
//				Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//				Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//				Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//				Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			}
//			else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
//			{
//				Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//				Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//				Path[2] = pDoc->Status.PcrShare[0].sLot;
//				Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//			}
//			break;
//		case RMAP_DN:
//			str = _T("YieldDn.txt");
//			if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
//			{
//				Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//				Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//				Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//				//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//				//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
//				Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			}
//			else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
//			{
//				Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//				Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//				Path[2] = pDoc->Status.PcrShare[0].sLot;
//				//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//				//Path[2] = pDoc->Status.PcrShare[1].sLot;
//				Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			}
//			break;
//		case RMAP_ALLDN:
//#ifdef TEST_MODE
//			str = _T("YieldAllDn.txt");
//#else
//			str = _T("YieldAll.txt");
//#endif
//			if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
//			{
//				Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//				Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//				Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//				//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//				//Path[2] = pDoc->WorkingInfo.LastJob.sLotDn;
//				Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			}
//			else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
//			{
//				Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//				Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//				Path[2] = pDoc->Status.PcrShare[0].sLot;
//				//Path[1] = pDoc->WorkingInfo.LastJob.sModelDn;
//				//Path[2] = pDoc->Status.PcrShare[1].sLot;
//				Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//			}
//			break;
//		}
//	}
//	else
//	{
//		str = _T("YieldUp.txt");
//		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
//		{
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//		}
//		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
//		{
//			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//			Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//			Path[2] = pDoc->Status.PcrShare[0].sLot;
//			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//		}
//	}
//
//	sPath.Format(_T("%s%s"), Path[0], Path[1]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
//
//	sPath.Format(_T("%s%s\\%s\\%s\\Reelmap"), Path[0], Path[1], Path[2], Path[3]);
//	//if(!finder.FindFile(sPath))
//	if (!pDoc->DirectoryExists(sPath))
//		CreateDirectory(sPath, NULL);
}


BOOL CReelMap::WriteYield(int nSerial, CString sPath)
{
	return TRUE;

	int nNodeX = 0, nNodeY = 0;
#ifndef TEST_MODE
	nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
#endif
	CString sDefNum, strData;
	int nPnl, nRow, nCol, nDefCode, nStrip;
	int nTotPcs = nNodeX * nNodeY;
	int nStripPcs = nTotPcs / 4;
	double dStOutRto = _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	nPnl = nSerial - 1;

	int i, k;
	TCHAR szData[200];

	CString sCode, strMenu, strItem, sMsg;
	int nTot = 0, nGood = 0, nDef = 0;
	int nTotSriptOut = 0;

	int nDefStrip[MAX_STRIP], nDefA[MAX_DEF], nDefPerStrip[4][MAX_DEF], nStripOut[4];
	nDefStrip[0] = 0; nDefStrip[1] = 0; nDefStrip[2] = 0; nDefStrip[3] = 0;
	nStripOut[0] = 0; nStripOut[1] = 0; nStripOut[2] = 0; nStripOut[3] = 0;

	nTot = nNodeX * nNodeY;

	for (k = 0; k < MAX_DEF; k++)
	{
		nDefA[k] = 0;

		for (i = 0; i < MAX_STRIP; i++)
		{
			nDefPerStrip[i][k] = 0;
		}
	}

	for (nRow = 0; nRow < nNodeY; nRow++)
	{
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			if (m_pPnlBuf)
			{
				nDefCode = (int)m_pPnlBuf[nPnl][nRow][nCol] < 0 ? 0 : (int)m_pPnlBuf[nPnl][nRow][nCol];
				nDefA[nDefCode]++;

				nStrip = int(nRow / (nNodeY / 4));
				if (nStrip > -1 && nStrip < 4)
				{
					if (nDefCode > 0)
					{
						nDefStrip[nStrip]++;
						nDefPerStrip[nStrip][nDefCode]++;
					}
				}
			}
		}
	}

	for (nStrip = 0; nStrip < 4; nStrip++)
	{
		if (nDefStrip[nStrip] >= nStripPcs * dStOutRto)
			nStripOut[nStrip]++;
	}

	for (i = 1; i < MAX_DEF; i++)
	{
		m_stYield.nDefA[i] = m_stYield.nDefA[i] + nDefA[i];
		nDef += nDefA[i];
	}
	nGood = nTot - nDef;

	m_stYield.nTot = m_stYield.nTot + nTot;
	m_stYield.nGood = m_stYield.nGood + nGood;
	m_stYield.nDef = m_stYield.nDef + nDef;

	for (k = 0; k < MAX_STRIP; k++)
	{
		m_stYield.nDefStrip[k] = m_stYield.nDefStrip[k] + nDefStrip[k];
		m_stYield.nStripOut[k] = m_stYield.nStripOut[k] + nStripOut[k];
		nTotSriptOut += nStripOut[k];
		for (i = 1; i < MAX_DEF; i++)
			m_stYield.nDefPerStrip[k][i] = m_stYield.nDefPerStrip[k][i] + nDefPerStrip[k][i];
	}
	m_stYield.nTotSriptOut = m_stYield.nTotSriptOut + nTotSriptOut;


	FILE *fp = NULL;
	char FileName[MAX_PATH];

	BOOL bExist = FALSE;
	CFileFind findfile;
	if (findfile.FindFile(sPath))
		bExist = TRUE;
	else
		MakeDirYield(sPath);

	//_tcscpy(FileName, sPath);
	StrToChar(sPath, FileName);

	fp = fopen(FileName, "a+");
	if (fp == NULL)
	{
		pView->MsgBox(_T("It is trouble to open ReelMap.txt"));
		// 		AfxMessageBox(_T("It is trouble to open ReelMap.txt"),MB_ICONWARNING|MB_OK);
		return FALSE;
	}

	if (!bExist)
	{
		m_nStartSerial = nSerial;

		fprintf(fp, "[Info]\n");
		fprintf(fp, "Total Shot = \n\n");
		fprintf(fp, "Total Pcs = \n");
		fprintf(fp, "Good Pcs = \n");
		fprintf(fp, "Bad Pcs = \n\n");
		fprintf(fp, "Start Shot=%d\n", m_nStartSerial);
		fprintf(fp, "End Shot = \n\n");

		for (i = 1; i <= MAX_DEF; i++)
			fprintf(fp, "%d=\n", i);
		fprintf(fp, "\n");

		for (k = 0; k < MAX_STRIP; k++)
			fprintf(fp, "Strip%d = \n", k);
		fprintf(fp, "\n");

		for (k = 0; k < MAX_STRIP; k++)
		{
			for (i = 1; i <= MAX_DEF; i++)
				fprintf(fp, "Strip%d_%d = \n", k, i);
			fprintf(fp, "\n");
		}

		fprintf(fp, "StripOut_Total = \n");
		fprintf(fp, "StripOut_0 = \n");
		fprintf(fp, "StripOut_1 = \n");
		fprintf(fp, "StripOut_2 = \n");
		fprintf(fp, "StripOut_3 = \n");
		fprintf(fp, "\n");
	}
	
	// [Serial]

	fprintf(fp, "[%d]\n", nSerial);
	fprintf(fp, "Total Pcs = %d\n", m_stYield.nTot);
	fprintf(fp, "Good Pcs = %d\n", m_stYield.nGood);
	fprintf(fp, "Bad Pcs = %d\n\n", m_stYield.nDef);
	
	for (i = 1; i < MAX_DEF; i++)
	{
		fprintf(fp, "%d = %d\n", i, m_stYield.nDefA[i]);
	}
	fprintf(fp, "\n");

	fprintf(fp, "StripOut_Total = %d\n", m_stYield.nTotSriptOut);
	for (k = 0; k < MAX_STRIP; k++)
		fprintf(fp, "StripOut_%d = %d\n", k, m_stYield.nStripOut[k]);
	fprintf(fp, "\n");

	for (k = 0; k < MAX_STRIP; k++)
	{
		fprintf(fp, "Strip%d = %d\n", k, m_stYield.nDefStrip[k]);
		for (i = 1; i < MAX_DEF; i++)
			fprintf(fp, "Strip%d_%d = %d\n", k, i, m_stYield.nDefPerStrip[k][i]);
		fprintf(fp, "\n");
	}

	fclose(fp);


	for (i = 1; i < MAX_DEF; i++)
	{
		sCode.Format(_T("%d"), i);
		sDefNum.Format(_T("%d"), m_stYield.nDefA[i]);
		::WritePrivateProfileString(_T("Info"), sCode, sDefNum, sPath);
	}

	strData.Format(_T("%d"), nSerial);
	::WritePrivateProfileString(_T("Info"), _T("End Shot"), strData, sPath);

	strData.Format(_T("%d"), nSerial - m_nStartSerial + 1);
	::WritePrivateProfileString(_T("Info"), _T("Total Shot"), strData, sPath);

	strData.Format(_T("%d"), m_stYield.nTot);
	::WritePrivateProfileString(_T("Info"), _T("Total Pcs"), strData, sPath);

	strData.Format(_T("%d"), m_stYield.nGood);
	::WritePrivateProfileString(_T("Info"), _T("Good Pcs"), strData, sPath);

	strData.Format(_T("%d"), m_stYield.nDef);
	::WritePrivateProfileString(_T("Info"), _T("Bad Pcs"), strData, sPath);

	for (k = 0; k < MAX_STRIP; k++)
	{
		strItem.Format(_T("Strip%d"), k);
		strData.Format(_T("%d"), m_stYield.nDefStrip[k]);
		::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);

		strItem.Format(_T("StripOut_%d"), k);
		strData.Format(_T("%d"), m_stYield.nStripOut[k]);
		::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);

		for (i = 1; i < MAX_DEF; i++)
		{
			strItem.Format(_T("Strip%d_%d"), k, i);
			strData.Format(_T("%d"), m_stYield.nDefPerStrip[k][i]);
			::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
		}
	}

	strData.Format(_T("%d"), m_stYield.nTotSriptOut);
	::WritePrivateProfileString(_T("Info"), _T("StripOut_Total"), strData, sPath);

	return TRUE;
}

BOOL CReelMap::MakeHeader(CString sPath)
{
	FILE *fp = NULL;
	char FileName[MAX_PATH];
	BOOL bExist = FALSE;

	CFileFind findfile;
	if (findfile.FindFile(sPath))
	{
		bExist = TRUE;
		return TRUE;
	}

	m_sPathShare = sPath;

	MakeDirRmap();
	StrToChar(sPath, FileName);

	fp = fopen(FileName, "w+");
	if (fp == NULL)
	{
		pView->MsgBox(_T("It is trouble to open ReelMap.txt"));
		return FALSE;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	char* pRtn = NULL;
	int nNodeX = 0, nNodeY = 0, i = 0, k = 0;
#ifdef USE_CAM_MASTER
	nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
#endif

	//InitRst();
	//InitPcs();

	fprintf(fp, "[Info]\n");
	fprintf(fp, "설  비  명 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "운  용  자 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "모      델 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sModelUp)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "로      트 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLotUp)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "상면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerUp)); if (pRtn) delete pRtn; pRtn = NULL;
	if (bDualTest)
	{
		fprintf(fp, "하면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerDn)); if (pRtn) delete pRtn; pRtn = NULL;
	}
	fprintf(fp, "\n");
	fprintf(fp, "Process Code = \n");
	fprintf(fp, "Entire Speed = \n");
	fprintf(fp, "\n");

	fprintf(fp, "CamPcsX = %d\n", nNodeX);
	fprintf(fp, "CamPcsY = %d\n", nNodeY);

	fprintf(fp, "\n");
	fprintf(fp, "0 -> 양품\n");
	for (i = 1; i < MAX_DEF; i++)
	{
		fprintf(fp, "%d -> %s\n", i, pRtn = StrToChar(m_sKorDef[i])); if (pRtn) delete pRtn; pRtn = NULL; // m_cBigDef[i]
	}
	fprintf(fp, "? - Missed Align Panel(i.e 노광불량)\n");
	fprintf(fp, "\n");
	fprintf(fp, "Lot Start=\n");
	fprintf(fp, "Lot Run=\n");
	fprintf(fp, "Lot End=\n");
	fprintf(fp, "\n");
	fprintf(fp, "Marked Shot=\n");
	fprintf(fp, "Marked Date=\n");
	fprintf(fp, "\n");
	fprintf(fp, "\n");
	fprintf(fp, "// < 수율 정보 > \n");
	fprintf(fp, "\n");
	fprintf(fp, "Start Serial=\n");
	fprintf(fp, "End Serial=\n");
	fprintf(fp, "\n");
	fprintf(fp, "Total Pcs=\n");
	fprintf(fp, "Good Pcs=\n");
	fprintf(fp, "Bad Pcs=\n");
	fprintf(fp, "\n");
	fprintf(fp, "Strip0=\n");
	fprintf(fp, "Strip1=\n");
	fprintf(fp, "Strip2=\n");
	fprintf(fp, "Strip3=\n");
	for (i = 1; i < MAX_DEF; i++)
		fprintf(fp, "%d=\n", i); // m_cBigDef[i]
	fprintf(fp, "\n");

	for (k = 0; k < MAX_STRIP_NUM; k++)
	{
		fprintf(fp, "[Strip%d]\n", k);
		for (i = 1; i < MAX_DEF; i++)
			fprintf(fp, "%d=\n", i); // m_cBigDef[i]
		fprintf(fp, "\n");
	}

	fprintf(fp, "[StripOut]\n");
	fprintf(fp, "Total=\n");
	for (k = 0; k < MAX_STRIP_NUM; k++)
		fprintf(fp, "%d=\n", k);
	fprintf(fp, "\n");

	fclose(fp);

	return TRUE;
}


// ITS

CString CReelMap::GetPathReelmapIts()
{
	CString sName = _T("ReelMapDataIts.txt");
	CString str, sPath, Path[3];

	sPath = _T("");
	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode; // pDoc->m_sItsCode;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
		return _T("");

	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ReelmapIts.txt
	return sPath;
}

BOOL CReelMap::MakeItsReelmapHeader()
{
	FILE *fp = NULL;
	char FileName[MAX_PATH];
	BOOL bExist = FALSE;
	CString sName = _T("ReelMapDataIts.txt");
	CString str, sPath, Path[3];

	sPath = _T("");
	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode; // pDoc->m_sItsCode;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
		return FALSE;

	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ReelmapIts.txt

	CFileFind findfile;
	if (findfile.FindFile(sPath))
	{
		bExist = TRUE;
		return TRUE;
	}

	sPath.Format(_T("%s%s"), Path[0], Path[1]);					// 모델 폴더
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);	// ITS 코드 폴더
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ReelmapIts.txt

	StrToChar(sPath, FileName);

	fp = fopen(FileName, "a+");
	if (fp == NULL)
	{
		str.Format(_T("It is trouble to open ReelmapIts.txt\r\n%s"), sPath);
		pView->MsgBox(str);
		return FALSE;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	char* pRtn = NULL;
	int nNodeX = 0, nNodeY = 0, i = 0, k = 0;
#ifdef USE_CAM_MASTER
	nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
#endif

	fprintf(fp, "[Info]\n");
	fprintf(fp, "설  비  명 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "운  용  자 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "모      델 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sModelUp)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, " ITS Code  = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sEngItsCode)); if (pRtn) delete pRtn; pRtn = NULL;
	//fprintf(fp, " ITS Code  = %s\n", pRtn = StrToChar(pDoc->m_sItsCode)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "\n");
	fprintf(fp, "Process Code = \n");
	fprintf(fp, "Entire Speed = \n");
	fprintf(fp, "\n");

	fprintf(fp, "CamPcsX = %d\n", nNodeX);
	fprintf(fp, "CamPcsY = %d\n", nNodeY);

	fprintf(fp, "\n");
	fprintf(fp, "0 -> 양품\n");
	for (i = 1; i < MAX_DEF; i++)
	{
		fprintf(fp, "%d -> %s\n", i, pRtn = StrToChar(pDoc->m_pReelMapUp->m_sKorDef[i])); if (pRtn) delete pRtn; pRtn = NULL; // m_cBigDef[i]
	}
	fprintf(fp, "? - Missed Align Panel(i.e 노광불량)\n");
	fprintf(fp, "\n");
	fprintf(fp, "Lot Start=\n");
	fprintf(fp, "Lot Run=\n");
	fprintf(fp, "Lot End=\n");
	fprintf(fp, "\n");
	fprintf(fp, "Marked Shot=\n");
	fprintf(fp, "Marked Date=\n");
	fprintf(fp, "\n");
	fprintf(fp, "\n");
	fprintf(fp, "// < 수율 정보 > \n");
	fprintf(fp, "\n");
	fprintf(fp, "Start Serial=\n");
	fprintf(fp, "End Serial=\n");
	fprintf(fp, "\n");
	fprintf(fp, "Total Pcs=\n");
	fprintf(fp, "Good Pcs=\n");
	fprintf(fp, "Bad Pcs=\n");
	fprintf(fp, "\n");
	fprintf(fp, "Strip0=\n");
	fprintf(fp, "Strip1=\n");
	fprintf(fp, "Strip2=\n");
	fprintf(fp, "Strip3=\n");
	for (i = 1; i < MAX_DEF; i++)
		fprintf(fp, "%d=\n", i); // m_cBigDef[i]
	fprintf(fp, "\n");

	for (k = 0; k < MAX_STRIP_NUM; k++)
	{
		fprintf(fp, "[Strip%d]\n", k);
		for (i = 1; i < MAX_DEF; i++)
			fprintf(fp, "%d=\n", i); // m_cBigDef[i]
		fprintf(fp, "\n");
	}

	fprintf(fp, "[StripOut]\n");
	fprintf(fp, "Total=\n");
	for (k = 0; k < MAX_STRIP_NUM; k++)
		fprintf(fp, "%d=\n", k);
	fprintf(fp, "\n");

	fclose(fp);

	return TRUE;
}

BOOL CReelMap::WriteIts(int nItsSerial)
{
	return TRUE;

	if (nItsSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.163"));
		return FALSE;
	}

	if (pDoc->GetTestMode() != MODE_OUTER)
		return FALSE;

	CString str, sPath;
	sPath = GetPathReelmapIts(); // pDoc->GetItsReelmapPath();
	if (sPath.IsEmpty())
	{
		str.Format(_T("ReelmapITS Path is emptied."));
		pView->MsgBox(str);
		return FALSE;
	}

	if (!MakeItsReelmapHeader())	// 내외층 머징된 릴맵 헤드
	{
		str.Format(_T("It is trouble to open ReelmapIts.txt\r\n%s"), sPath);
		pView->MsgBox(str);
		return FALSE;
	}

	BOOL bDualTestInner;
	CString sLot, sLayerUp, sLayerDn;
	if (!pDoc->GetItsSerialInfo(nItsSerial, bDualTestInner, sLot, sLayerUp, sLayerDn))
	{
		str.Format(_T("It is trouble to read GetItsSerialInfo()."));
		pView->MsgBox(str);
		return FALSE;
	}

	int nIdx = pDoc->GetPcrIdx0(nItsSerial);
	int nNodeX = pDoc->m_MasterInner[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_MasterInner[0].m_pPcsRgn->nRow;
	int nStripY = pDoc->m_MasterInner[0].m_pPcsRgn->nRow / MAX_STRIP_NUM; // Strip(1~4);
	int nTotDefPcs = 0;
	if (pDoc->m_pPcrIts)
	{
		if (pDoc->m_pPcrIts[nIdx])
			nTotDefPcs = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
	}

	short **pPnlBuf;
	int i, nC, nR, nPcsId, nDefCode;//, nTot, nDef, nGood;
	pPnlBuf = new short*[nNodeY];
	for (i = 0; i < nNodeY; i++)
	{
		pPnlBuf[i] = new short[nNodeX];
		memset(pPnlBuf[i], 0, sizeof(short)*nNodeX);
		memset(m_pPnlBuf[nItsSerial - 1][i], 0, sizeof(short)*nNodeX);
	}

	CString strData, strTemp;

	for (i = 0; i < nTotDefPcs; i++)
	{
		if (pDoc->m_pPcrIts[nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
			{
				switch (pDoc->m_MasterInner[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
				{
				case 0:
					nPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i];
					break;
				case 1:
					nPcsId = pDoc->MirrorLR(pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i]);
					break;
				case 2:
					nPcsId = pDoc->MirrorUD(pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i]);
					break;
				case 3:
					nPcsId = pDoc->Rotate180(pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i]);
					break;
				default:
					nPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i];
					break;
				}
			}
			else
				nPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[i];

			nDefCode = pDoc->m_pPcrIts[nIdx]->m_pDefType[i];

			nC = int(nPcsId / nNodeY);
			if (nC % 2)	// 홀수.
				nR = nNodeY * (nC + 1) - nPcsId - 1;
			else		// 짝수.
				nR = nPcsId - nNodeY * nC;
			pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
			if (m_pPnlBuf)
				m_pPnlBuf[nItsSerial - 1][nR][nC] = pPnlBuf[nR][nC];   // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용. #define PNL_TOT
		}
	}

	CString sPnl, sRow;
	i = 0;
	sPnl.Format(_T("%d"), nItsSerial);
	strData.Format(_T("%d"), nTotDefPcs);
	::WritePrivateProfileString(sPnl, _T("Total Defects"), strData, sPath);

	for (int nRow = 0; nRow < nNodeX; nRow++)
	{
		sRow.Format(_T("%02d"), nRow);
		//sRow.Format(_T("%2d"), nRow+1);
		strData.Format(_T(""));
		strTemp.Format(_T(""));

		//if(nRow==26)
		//	int brk=0;

		for (int nCol = 0; nCol < nNodeY; nCol++)
		{
			nR = (nNodeY - 1) - nCol;
			nC = nRow;

			if (pDoc->m_pPcrIts[nIdx]->m_nErrPnl == -1 || pDoc->m_pPcrIts[nIdx]->m_nErrPnl == -2)
			{
				nDefCode = DEF_LIGHT;
				m_pPnlBuf[nItsSerial - 1][nR][nC] = (short)nDefCode;
			}
			else
				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];

			strTemp.Format(_T("%2d,"), nDefCode);

			if (!nCol)
				strData.Insert(0, strTemp);
			else
			{
				int nLen = strData.GetLength();
				if (!(nCol%nStripY)) // Separate Strip
				{
					strData.Insert(nLen, _T("  "));
					nLen = strData.GetLength();
				}
				strData.Insert(nLen, strTemp);
			}
		}

		int nPos = strData.ReverseFind(',');
		strData.Delete(nPos, 1);
		::WritePrivateProfileString(sPnl, sRow, strData, sPath);
	}

	for (i = 0; i < nNodeY; i++)
		delete[]  pPnlBuf[i];
	delete[] pPnlBuf;

	pDoc->m_nWritedItsSerial = nItsSerial;
	return TRUE;
}

/*

BOOL CReelMap::InitRstIts()
{
	int nPnl, nRow;
	int TotPnl = PNL_TOT;
	int nDumyPnl = 20;
	// 	int nNodeX = pDoc->m_pPcsRgn->nCol;
	// 	int nNodeY = pDoc->m_pPcsRgn->nRow;
	int nNodeX = PNLBUF_X;
	int nNodeY = PNLBUF_Y;

	int k, i;

	CloseRstIts();

	m_nPnlBufIts = TotPnl + nDumyPnl;
	if (!m_pPnlBufIts)
	{
		m_pPnlBufIts = new short**[m_nPnlBufIts];
		for (nPnl = 0; nPnl < m_nPnlBufIts; nPnl++)
		{
			m_pPnlBufIts[nPnl] = new short*[nNodeY];
			for (nRow = 0; nRow < nNodeY; nRow++)
				m_pPnlBufIts[nPnl][nRow] = new short[nNodeX];
		}
	}

	m_nTotPcsIts = 0;
	m_nGoodPcsIts = 0;
	m_nBadPcsIts = 0;
	for (i = 0; i < MAX_DEF; i++)
		m_nDefIts[i] = 0;

	m_nTotStOutIts = 0;
	for (k = 0; k < MAX_STRIP_NUM; k++)
	{
		m_nStripOutIts[k] = 0;
		m_nDefStripIts[k] = 0;
		for (i = 0; i < MAX_DEF; i++)
			m_nDefPerStripIts[k][i] = 0;
	}

	return TRUE;
}

void CReelMap::CloseRstIts()
{
	int nPnl, nRow;
	// 	int nNodeX = pDoc->m_pPcsRgn->nCol;
	// 	int nNodeY = pDoc->m_pPcsRgn->nRow;
	int nNodeX = PNLBUF_X;
	int nNodeY = PNLBUF_Y;

	if (m_pPnlBufIts)
	{
		for (nPnl = 0; nPnl < m_nPnlBufIts; nPnl++)
		{
			for (nRow = 0; nRow < nNodeY; nRow++)
				delete[] m_pPnlBufIts[nPnl][nRow];
			delete[] m_pPnlBufIts[nPnl];
		}

		delete[] m_pPnlBufIts;
		m_pPnlBufIts = NULL;
		m_nPnlBufIts = 0;
	}
}

void CReelMap::ClrRstIts()
{
m_nTotPcs = 0;
m_nGoodPcs = 0;
m_nBadPcs = 0;

int k, i;
for (i = 1; i < MAX_DEF; i++)
m_nDef[i] = 0;

m_nTotStOut = 0;
for (k = 0; k < MAX_STRIP_NUM; k++)
{
m_nStripOut[k] = 0;
m_nDefStrip[k] = 0;
for (i = 1; i < MAX_DEF; i++)
m_nDefPerStrip[k][i] = 0;
}

// 	double dN0;
// 	if(m_dTotLen > m_dPnlLen)
// 		dN0 = m_dTotLen / m_dPnlLen;
// 	else
// 		dN0 = (double)PNL_TOT;
// 	int nN0 = int(m_dTotLen / m_dPnlLen);
// 	int TotPnl = (dN0 - (double)nN0) > 0.0 ? (nN0+1) : nN0 ;
// 	if(TotPnl > PNL_TOT)
// 		TotPnl = PNL_TOT;

int nPnl, nRow, nCol;
// 	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
// 	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
int TotPnl = PNL_TOT;
int nNodeX = PNLBUF_X;
int nNodeY = PNLBUF_Y;

if (m_pPnlBuf)
{
for (nPnl = 0; nPnl < TotPnl; nPnl++)
{
for (nRow = 0; nRow < nNodeY; nRow++)
{
for (nCol = 0; nCol < nNodeX; nCol++)
m_pPnlBuf[nPnl][nRow][nCol] = 0;
}
}
}
}

void CReelMap::SetFixPcsIts(int nSerial)
{
	// 	if(!pDoc->WorkingInfo.LastJob.bContFixDef)
	// 		return;

	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nR, nC, nDefCode;

	for (nR = 0; nR < nNodeY; nR++)
	{
		for (nC = 0; nC < nNodeX; nC++)
		{
			nDefCode = m_pPnlBufIts[nSerial - 1][nR][nC]; // DefCode
			if (nDefCode > 0)
				SetFixPcsIts(nSerial, nC, nR);
		}
	}
}

void CReelMap::SetFixPcsIts(int nSerial, int nCol, int nRow) // nShot : 0 ~ 
{
	if (nSerial < 1)
		return;

	int nRange = _tstoi(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	int nS = (nSerial - 1) % nRange;
	m_FixPcsIts[nS][nCol][nRow] = TRUE;
}

void CReelMap::ClrFixPcsIts()
{
	m_nPrevSerial[0] = 0;	// --
	m_nPrevSerial[1] = 0;	// ++

	for (int nS = 0; nS < FIX_PCS_SHOT_MAX; nS++)
	{
		if (!nS)
		{
			m_nPrevSerial[0] = 0; // [0] : --
			m_nPrevSerial[1] = 0; // [1] : ++
		}

		for (int nC = 0; nC < FIX_PCS_COL_MAX; nC++)
		{
			for (int nR = 0; nR < FIX_PCS_ROW_MAX; nR++)
			{
				m_FixPcsIts[nS][nC][nR] = FALSE;
				if (!nS)
				{
					m_FixPcsPrevIts[nC][nR] = 0;
				}
			}
		}
	}
}

void CReelMap::ClrFixPcsIts(int nCol, int nRow)
{
	for (int nS = 0; nS < FIX_PCS_SHOT_MAX; nS++)
	{
		m_FixPcsIts[nS][nCol][nRow] = FALSE;
	}
	m_FixPcsPrevIts[nCol][nRow] = 0;
}

BOOL CReelMap::IsFixPcsIts(int nSerial, int &Col, int &Row)
{
	BOOL bRtn = FALSE;
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;

	BOOL bOn = pDoc->WorkingInfo.LastJob.bContFixDef;
	int nRange = _tstoi(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	int nJudge = _tstoi(pDoc->WorkingInfo.LastJob.sNumContFixDef);

	// 	if(nSerial < nJudge)
	// 		return bRtn;

	if (m_nPrevSerial[0] == nSerial)
		return bRtn;
	m_nPrevSerial[0] = nSerial;

	int nS;

	for (int nC = 0; nC < nNodeX; nC++)
	{
		for (int nR = 0; nR < nNodeY; nR++)
		{
			if (nSerial >= nRange)
			{
				nS = (nSerial - 1) % nRange;
				if (nS == nRange - 1)
					nS = 0;
				else
					nS++;

				if (m_FixPcs[nS][nC][nR]) // Defect PCS
				{
					if (m_FixPcsPrev[nC][nR] > 0)
						m_FixPcsPrev[nC][nR]--;
				}
				m_FixPcs[nS][nC][nR] = FALSE;
			}

			nS = (nSerial - 1) % nRange;
			if (m_FixPcs[nS][nC][nR]) // Defect PCS
			{
				// 				if(m_nPrevSerial[1] != nSerial) // [1] : ++
				// 				{
				//					m_nPrevSerial[1] = nSerial;
				m_FixPcsPrev[nC][nR]++;
				//				}
			}

			if (bOn)
			{
				if (m_FixPcsPrev[nC][nR] >= nJudge)
				{
					Col = nC;
					Row = nR;
					ClrFixPcs(nC, nR);
					bRtn = TRUE;
				}
			}
		}
	}

	return bRtn;
}

BOOL CReelMap::IsFixPcsIts(int nSerial, int* pCol, int* pRow, int &nTot) // nTot : total of PCS Over nJudge
{
	BOOL bRtn = FALSE;
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;

	BOOL bOn = pDoc->WorkingInfo.LastJob.bContFixDef;
	int nRange = _tstoi(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	int nJudge = _tstoi(pDoc->WorkingInfo.LastJob.sNumContFixDef);

	// 	if(nSerial < nJudge)
	// 		return bRtn;

	if (m_nPrevSerial[0] == nSerial)
		return bRtn;
	m_nPrevSerial[0] = nSerial;

	int nS = (nSerial - 1) % nRange;
	nTot = 0;

	for (int nC = 0; nC < nNodeX; nC++)
	{
		for (int nR = 0; nR < nNodeY; nR++)
		{
			if (nSerial >= nRange)
			{
				nS = (nSerial - 1) % nRange;
				if (nS == nRange - 1)
					nS = 0;
				else
					nS++;

				if (m_FixPcs[nS][nC][nR]) // Defect PCS
				{
					if (m_FixPcsPrev[nC][nR] > 0)
						m_FixPcsPrev[nC][nR]--;
				}
				m_FixPcs[nS][nC][nR] = FALSE;
			}

			nS = (nSerial - 1) % nRange;
			if (m_FixPcs[nS][nC][nR]) // Defect PCS
			{
				m_FixPcsPrev[nC][nR]++;
			}

			if (bOn)
			{
				if (m_FixPcsPrev[nC][nR] >= nJudge)
				{
					pCol[nTot] = nC;
					pRow[nTot] = nR;
					nTot++;
					ClrFixPcs(nC, nR);
					bRtn = TRUE;
				}
			}
		}
	}

	return bRtn;
}
*/

BOOL CReelMap::MakeItsFile(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
{
	MakeDirIts();

	CFileFind cFile;
	CString sPath = pDoc->GetItsPath(nSerial, nLayer);

	if (cFile.FindFile(sPath))
		DeleteFile(sPath);

	char FileName[MAX_PATH];
	StringToChar(sPath, FileName);

	char* pRtn = NULL;
	FILE *fp = NULL;
	fp = fopen(FileName, "w+");
	if (fp != NULL)
	{
		fprintf(fp, "%s", pRtn = StringToChar(GetItsFileData(nSerial, nLayer)));
		if (pRtn)
		{
			delete pRtn;
			pRtn = NULL;
		}
	}
	else
	{
		pView->MsgBox(_T("It is trouble to MakeItsFile."), MB_ICONWARNING | MB_OK);
		return FALSE;
	}

	fclose(fp);
	return TRUE;
}

CString CReelMap::GetItsFileData(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
{
	CString sPath = pDoc->GetItsPath(nSerial, nLayer);
	CString str = _T(""), sSide = _T(""), sTemp = _T(""), sItsData = _T("");
	CString sItsCode = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//CString sItsCode = pDoc->m_sItsCode;

	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
	int nStripY = pDoc->m_Master[0].m_pPcsRgn->nRow / 4; // Strip(1~4);
	int nIdx = pDoc->GetPcrIdx0(nSerial);

	int nTotDefPcs = 0;

	switch (nLayer)
	{
	case RMAP_UP:
		sSide = _T("T");
		if (pDoc->m_pPcr[nLayer])
		{
			if (pDoc->m_pPcr[nLayer][nIdx])
				nTotDefPcs = pDoc->m_pPcr[nLayer][nIdx]->m_nTotDef;
		}
		break;
	case RMAP_DN:
		sSide = _T("B");
		if (pDoc->m_pPcr[nLayer])
		{
			if (pDoc->m_pPcr[nLayer][nIdx])
				nTotDefPcs = pDoc->m_pPcr[nLayer][nIdx]->m_nTotDef;
		}
		break;
	case RMAP_INNER_UP:
		sSide = _T("T");
		if (pDoc->m_pPcrInner[0])
		{
			if (pDoc->m_pPcrInner[0][nIdx])
				nTotDefPcs = pDoc->m_pPcrInner[0][nIdx]->m_nTotDef;
		}
		//if (pDoc->m_pPcr[nLayer])
		//{
		//	if (pDoc->m_pPcr[nLayer][nIdx])
		//		nTotDefPcs = pDoc->m_pPcr[nLayer][nIdx]->m_nTotDef;
		//}
		break;
	case RMAP_INNER_DN:
		sSide = _T("B");
		if (pDoc->m_pPcrInner[1])
		{
			if (pDoc->m_pPcrInner[1][nIdx])
				nTotDefPcs = pDoc->m_pPcrInner[1][nIdx]->m_nTotDef;
		}
		//if (pDoc->m_pPcr[nLayer])
		//{
		//	if (pDoc->m_pPcr[nLayer][nIdx])
		//		nTotDefPcs = pDoc->m_pPcr[nLayer][nIdx]->m_nTotDef;
		//}
		break;
	}

	str.Format(_T("%d,%s,%04d\n"), nTotDefPcs, sItsCode, nSerial);
	sItsData = str;

	int nR, nRow, nCol, nDefCode;
	int nTotStrip = 0;

	// Strip A
	for (nRow = 0; nRow < nStripY; nRow++)
	{
		nR = nRow;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			if (nDefCode)
			{
				nTotStrip++;
				str.Format(_T("%s,%04d,%s,A,%d,%d,B%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
				sTemp += str;
			}
		}
	}
	str.Format(_T("%d,%s,%04d,%s,A\n"), nTotStrip, sItsCode, nSerial, sSide);
	sItsData += str;
	sItsData += sTemp;
	str.Format(_T("%s,%04d,%s,A,EOS\n"), sItsCode, nSerial, sSide);
	sItsData += str;
	sTemp = _T("");
	nTotStrip = 0;

	// Strip B
	for (nRow = 0; nRow < nStripY; nRow++)
	{
		nR = nRow + nStripY;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			if (nDefCode)
			{
				nTotStrip++;
				str.Format(_T("%s,%04d,%s,B,%d,%d,B%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
				sTemp += str;
			}
		}
	}
	str.Format(_T("%d,%s,%04d,%s,B\n"), nTotStrip, sItsCode, nSerial, sSide);
	sItsData += str;
	sItsData += sTemp;
	str.Format(_T("%s,%04d,%s,B,EOS\n"), sItsCode, nSerial, sSide);
	sItsData += str;
	sTemp = _T("");
	nTotStrip = 0;

	// Strip C
	for (nRow = 0; nRow < nStripY; nRow++)
	{
		nR = nRow + nStripY * 2;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			if (nDefCode)
			{
				nTotStrip++;
				str.Format(_T("%s,%04d,%s,B,%d,%d,C%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
				sTemp += str;
			}
		}
	}
	str.Format(_T("%d,%s,%04d,%s,C\n"), nTotStrip, sItsCode, nSerial, sSide);
	sItsData += str;
	sItsData += sTemp;
	str.Format(_T("%s,%04d,%s,C,EOS\n"), sItsCode, nSerial, sSide);
	sItsData += str;
	sTemp = _T("");
	nTotStrip = 0;

	// Strip D
	for (nRow = 0; nRow < nStripY; nRow++)
	{
		nR = nRow + nStripY * 3;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			if (nDefCode)
			{
				nTotStrip++;
				str.Format(_T("%s,%04d,%s,B,%d,%d,D%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
				sTemp += str;
			}
		}
	}
	str.Format(_T("%d,%s,%04d,%s,D\n"), nTotStrip, sItsCode, nSerial, sSide);
	sItsData += str;
	sItsData += sTemp;
	str.Format(_T("%s,%04d,%s,D,EOS\n"), sItsCode, nSerial, sSide);
	sItsData += str;
	str.Format(_T("%s,%04d,%s,EOP\n"), sItsCode, nSerial, sSide);
	sItsData += str;

	return sItsData;
}

BOOL CReelMap::MakeDirIts()
{
	CFileFind finder;
	CString  Path[3];
	CString sPath = _T("");

	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//Path[2] = pDoc->m_sItsCode;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
		return FALSE;

	sPath.Format(_T("%s%s"), Path[0], Path[1]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\Inner"), Path[0], Path[1], Path[2]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\Outer"), Path[0], Path[1], Path[2]);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	return TRUE;
}

void CReelMap::ResetReelmapPath()
{
	m_sPathBuf = GetRmapPath(m_nLayer);
	m_sPathYield = GetYieldPath(m_nLayer);
}
