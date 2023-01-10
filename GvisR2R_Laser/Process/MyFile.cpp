// MyFile.cpp : implementation file
//

#include "stdafx.h"
//#include "gvisr2r.h"
#include "MyFile.h"
#include "../Global/GlobalFunc.h"
#include "../Global/GlobalDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




//************************************************************************************************************

class CFExeption
{
public:
	CFExeption(DWORD dwErrCode);
	CFExeption(CString sErrText);
	CString GetErrorText() {return m_sError;}
	DWORD GetErrorCode() {return m_dwError;}

private:
	CString m_sError;
	DWORD m_dwError;
};

CFExeption::CFExeption(DWORD dwErrCode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			      NULL, dwErrCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	m_sError = (LPTSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);
	m_dwError = dwErrCode;
}


CFExeption::CFExeption(CString sErrText)
{
	m_sError = sErrText;
	m_dwError = 0;
}

//************************************************************************************************************


/////////////////////////////////////////////////////////////////////////////
// CMyFile

CMyFile::CMyFile(CWnd* pParent)
{
	m_pParent = pParent;
	m_sError = _T("");
	
// 	if(!Create(NULL, _T("MyFile"), WS_CHILD, CRect(0,0,0,0), m_pParent, (UINT)this))
// 		AfxMessageBox(_T("CMyFile::Create() Failed!!!"));
}

CMyFile::~CMyFile()
{
}


BEGIN_MESSAGE_MAP(CMyFile, CWnd)
	//{{AFX_MSG_MAP(CMyFile)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyFile message handlers

// ������ copy�Ѵ�.
BOOL CMyFile::Copy(CString sSource, CString sDest)
{
	if (CheckSelfCopy(sSource, sDest)) return true;
	bool bRes;
	try
	{
		DoCopy(sSource, sDest);
		bRes = true;
	}
	catch(CFExeption* e)
	{
		m_sError = e->GetErrorText();
		m_dwError = e->GetErrorCode();
		delete e;
		if (m_dwError == 0) bRes = true;
		bRes = false;
	}
	m_iRecursionLimit = -1;
	return bRes;
}

// �ҽ������� �״�� ����ġ�� copy�ϴ��� Ȯ��.
BOOL CMyFile::CheckSelfCopy(CString sSource, CString sDest)
{
	bool bRes = false;
	if (CheckPath(sSource) == PATH_IS_FOLDER)
	{
		CString sTmp = sSource;
		int pos = sTmp.ReverseFind('\\');
		if (pos != -1)
		{
			sTmp.Delete(pos, sTmp.GetLength() - pos);
			if (sTmp.CompareNoCase(sDest) == 0) bRes = true;
		}
	}
	return bRes;
}

// Path�� �������� �������� �׸��� �����ϴ��� Ȯ��.
int CMyFile::CheckPath(CString sPath)
{
	DWORD dwAttr = GetFileAttributes(sPath);
	if (dwAttr == 0xffffffff) 
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND) 
			return PATH_NOT_FOUND;
		return PATH_ERROR;
	}
	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) return PATH_IS_FOLDER;
	return PATH_IS_FILE;
}

// Path�ڿ� "\\"�� �ѹ��� ����.
void CMyFile::PreparePath(CString &sPath)
{
	if(sPath.Right(1) != "\\") sPath += "\\";
}

// Path���� ������ ���� ���� Parsing�ϰ�, ������ Path�� �����Ѵ�.
CString CMyFile::ParseFolderName(CString sPathName, CString &sRemainPathName)
{
	CString sFolderName = sPathName;
	int pos = sFolderName.ReverseFind('\\');
	if (pos != -1) 
	{
		sFolderName.Delete(pos, sFolderName.GetLength() - pos);
		sRemainPathName = sFolderName;
	}

	pos = sFolderName.ReverseFind('\\');
	if (pos != -1) 
	{
		sFolderName = sFolderName.Right(sFolderName.GetLength() - pos - 1);
		sRemainPathName.Delete(pos, sRemainPathName.GetLength() - pos);
	}
	else 
	{
		sRemainPathName = sFolderName;
		sFolderName.Empty();
	}

	return sFolderName;
}

//�ݺ��Ͽ� sDest�� ���� ������ Count�Ѵ�.
void CMyFile::CheckSelfRecursion(CString sSource, CString sDest)
{
	if (sDest.Find(sSource) != -1)
	{
		int i = 0, count1 = 0, count2 = 0;
		for(i = 0; i < sSource.GetLength(); i ++)	if (sSource[i] == '\\') count1 ++;
		for(i = 0; i < sDest.GetLength(); i ++)	if (sDest[i] == '\\') count2 ++;
		if (count2 >= count1) m_iRecursionLimit = count2 - count1;
	}
}

// ���ϰ� ������ �����Ͽ� copy�Ѵ�.
void CMyFile::DoCopy(CString sSource, CString sDest, bool bDelteAfterCopy)
{
	CString sRemainPathName, sFolderName;

	if(sDest.Find(_T("."),0) != -1)
	{
		sFolderName = ParseFolderName(sDest, sRemainPathName);
		PreparePath(sRemainPathName);
		sDest = sRemainPathName + sFolderName;
	}

	CheckSelfRecursion(sSource, sDest);
	// source not found
	if (CheckPath(sSource) == PATH_NOT_FOUND)
	{
		CString sError = sSource + CString(" not found");
		throw new CFExeption(sError);
	}
	// dest not found
	if (CheckPath(sDest) == PATH_NOT_FOUND)
	{
		CString sError = sDest + CString(" not found");
		throw new CFExeption(sError);
	}
	// folder to file
	if (CheckPath(sSource) == PATH_IS_FOLDER && CheckPath(sDest) == PATH_IS_FILE) 
	{
		throw new CFExeption(_T("Wrong operation"));
	}
	// folder to folder
	if (CheckPath(sSource) == PATH_IS_FOLDER && CheckPath(sDest) == PATH_IS_FOLDER) 
	{
		CFileFind ff;
		CString sError = sSource + CString(" not found");
		PreparePath(sSource);
		PreparePath(sDest);
		sSource += "*.*";
		if (!ff.FindFile(sSource)) 
		{
			ff.Close();
			throw new CFExeption(sError);
		}
		if (!ff.FindNextFile()) 
		{
			ff.Close();
			throw new CFExeption(sError);
		}
		sFolderName = ParseFolderName(sSource, sRemainPathName);
		if (!sFolderName.IsEmpty()) // the source is not drive
		{
			sDest += sFolderName;
			PreparePath(sDest);
			if (!CreateDirectory(sDest, NULL))
			{
				DWORD dwErr = GetLastError();
				if (dwErr != 183)
				{
					ff.Close();
					throw new CFExeption(dwErr);
				}
			}
		}
		ff.Close();
		DoFolderCopy(sSource, sDest, bDelteAfterCopy);
	}
	// file to file
	if (CheckPath(sSource) == PATH_IS_FILE && CheckPath(sDest) == PATH_IS_FILE) 
	{
		DoFileCopy(sSource, sDest);
	}
	// file to folder
	if (CheckPath(sSource) == PATH_IS_FILE && CheckPath(sDest) == PATH_IS_FOLDER) 
	{
		PreparePath(sDest);
		char drive[MAX_PATH], dir[MAX_PATH], name[MAX_PATH], ext[MAX_PATH];
		char* pRtn = NULL;
		_splitpath(pRtn=StringToChar(sSource), drive, dir, name, ext);
		sDest = sDest + CString(name) + CString(ext);
		DoFileCopy(sSource, sDest);
		if(pRtn)
			delete pRtn;
	}
}

// ���ϰ� ������ �����Ͽ� copy�Ѵ�.
void CMyFile::DoFolderCopy(CString sSourceFolder, CString sDestFolder, bool bDelteAfterCopy)
{
	CFileFind ff;
	CString sPathSource = sSourceFolder;
	BOOL bRes = ff.FindFile(sPathSource);
	while (bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots()) continue;
		if (ff.IsDirectory()) // source is a folder
		{
			if (m_iRecursionLimit == 0) continue;
			sPathSource = ff.GetFilePath() + CString("\\") + CString("*.*");
			CString sPathDest = sDestFolder + ff.GetFileName() + CString("\\");
			if (CheckPath(sPathDest) == PATH_NOT_FOUND) 
			{
				if (!CreateDirectory(sPathDest, NULL))
				{
					ff.Close();
					throw new CFExeption(GetLastError());
				}
			}
			if (m_iRecursionLimit > 0) m_iRecursionLimit --;
			DoFolderCopy(sPathSource, sPathDest, bDelteAfterCopy);
		}
		else // source is a file
		{
			CString sNewFileName = sDestFolder + ff.GetFileName();
			DoFileCopy(ff.GetFilePath(), sNewFileName, bDelteAfterCopy);
		}
	}
	ff.Close();
}

// ������ �����ϴ��� Ȯ���Ѵ�.
bool CMyFile::IsFileExist(CString sPathName)
{
	HANDLE hFile;
	hFile = CreateFile(sPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return false;
	CloseHandle(hFile);
	return true;
}

// ������ copy�Ѵ�.
void CMyFile::DoFileCopy(CString sSourceFile, CString sDestFile, bool bDelteAfterCopy)
{
	BOOL bOvrwriteFails = FALSE;
	if (!m_bOverwriteMode)
	{
		while (IsFileExist(sDestFile)) 
		{
			sDestFile = ChangeFileName(sDestFile);
		}
		bOvrwriteFails = TRUE;
	}
	if (!CopyFile(sSourceFile, sDestFile, bOvrwriteFails)) throw new CFExeption(GetLastError());
	if (bDelteAfterCopy)
	{
		DoDelete(sSourceFile);
	}
}

// ����� �ִ��� �Ӽ��� �ľ��Ѵ�.
bool CMyFile::CanDelete(CString sPathName)
{
	DWORD dwAttr = GetFileAttributes(sPathName);
	if (dwAttr == -1) return false;
	if (dwAttr & FILE_ATTRIBUTE_READONLY)
	{
		if (m_bAskIfReadOnly)
		{
			CString sTmp = sPathName;
			int pos = sTmp.ReverseFind('\\');
			if (pos != -1) sTmp.Delete(0, pos + 1);
			CString sText = sTmp + CString(" is read olny. Do you want delete it?");
			int iRes = MessageBox(sText, _T("Warning"), MB_YESNOCANCEL | MB_ICONQUESTION);
			switch (iRes)
			{
				case IDYES:
				{
					if (!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL)) return false;
					return true;
				}
				case IDNO:
				{
					return false;
				}
				case IDCANCEL:
				{
					m_bAborted = true;
					throw new CFExeption(0);
					return false;
				}
			}
		}
		else
		{
			if (!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL)) return false;
			return true;
		}
	}
	return true;
}

// �����̳� ���� �׸��� ������ �������� �����.
BOOL CMyFile::DeleteFolerOrFile(CString strFolerOrFilePath)
{
	CFileFind finder;
	if(finder.FindFile(strFolerOrFilePath))
	{
		DoDelete(strFolerOrFilePath);
		return TRUE;
	}
	else
		return FALSE;
}

// �����̳� ���� �׸��� ������ �������� �����.
void CMyFile::DoDelete(CString sPathName)
{
	CFileFind ff;
	CString sPath = sPathName;

	if (CheckPath(sPath) == PATH_IS_FILE)
	{
		if (!CanDelete(sPath)) 
		{
			m_bAborted = true;
			return;
		}
		if (!DeleteFile(sPath)) throw new CFExeption(GetLastError());
		return;
	}

	PreparePath(sPath);
	sPath += "*.*";

	BOOL bRes = ff.FindFile(sPath);
	while(bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots()) continue;
		if (ff.IsDirectory())
		{
			sPath = ff.GetFilePath();
			DoDelete(sPath);
		}
		else DoDelete(ff.GetFilePath());
	}
	ff.Close();
	if (!RemoveDirectory(sPathName) && !m_bAborted) 
	{
		//throw new CFExeption(GetLastError());
	}
}

// ���� ������ �����ϸ� ���� copy�� ���� �տ� "Copy of ", �׸��� �� �ݺ��ÿ��� "Copy (1) of "�� (?)�� �ݺ����� ǥ��.
CString CMyFile::ChangeFileName(CString sFileName)
{
	CString sName, sNewName, sResult;
	char drive[MAX_PATH];
	char dir  [MAX_PATH];
	char name [MAX_PATH];
	char ext  [MAX_PATH];
	char* pRtn = NULL;
	_splitpath(pRtn=StringToChar(sFileName), drive, dir, name, ext);
	sName = name;

	int pos = sName.Find(_T("Copy "));
	if (pos == -1)
	{
		sNewName = CString("Copy of ") + sName + CString(ext);
	}
	else
	{
		int pos1 = sName.Find('(');
		if (pos1 == -1)
		{
			sNewName = sName;
			sNewName.Delete(0, 8);
			sNewName = CString("Copy (1) of ") + sNewName + CString(ext);
		}
		else
		{
			CString sCount;
			int pos2 = sName.Find(')');
			if (pos2 == -1)
			{
				sNewName = CString("Copy of ") + sNewName + CString(ext);
			}
			else
			{
				sCount = sName.Mid(pos1 + 1, pos2 - pos1 - 1);
				sName.Delete(0, pos2 + 5);
				int iCount = _tstoi(sCount);
				iCount ++;
				sNewName.Format(_T("%s%d%s%s%s"), _T("Copy ("), iCount, _T(") of "), (LPCTSTR)sName, ext);
			}
		}
	}

	sResult = CString(drive) + CString(dir) + sNewName;
	if(pRtn)
		delete pRtn;
	return sResult;
}

void CMyFile::ShowError() //Show error message
{
	MessageBox(m_sError, _T("Error"), MB_OK | MB_ICONERROR);
}

/////////////////////////////////////////////////////////////////////////////
// for R2R

int CMyFile::CopyPcrAll(CString sSrc, CString sDest)  // return : Serial
{
	CString sFolder, sRemain, sSrcFolder, sFile;
	CString sLot, sFilePath, sDestFolder;
	CFileFind cFile;
	BOOL bExist;
	int nPos;

	// ���ϸ��� ����.
	nPos = sDest.ReverseFind('\\');
	sFile = sDest.Right(sDest.GetLength()-nPos-1);
	if(sFile.Find(_T("."), 0) != -1)
	{
		nPos = sFile.Find(_T("."), 0);
		sFile.Delete(nPos, sFile.GetLength() - nPos);
	}
	sFolder = ParseFolderName(sDest, sRemain);
	if(sFolder.IsEmpty()) return -1; // ������ �������� ����.
	sDestFolder = sRemain + _T("\\") + sFolder + _T("\\");

	sFolder = ParseFolderName(sSrc, sRemain);
	if(sFolder.IsEmpty()) return -1; // ������ �������� ����.
	sSrcFolder = sRemain + _T("\\") + sFolder + _T("\\");

	bExist = cFile.FindFile(sSrcFolder + _T("*.pcr"));
	if(!bExist)
	{
		return -2; // pcr������ �������� ����.
	}

	int nCopyed=0, nSerial;

	BeginWaitCursor();
	while(bExist)
	{

		bExist = cFile.FindNextFile();
		if(cFile.IsDots()) continue;
		if(!cFile.IsDirectory())
		{
			// ���ϸ��� ����.
			sFile = cFile.GetFileName();
			
			// ���� Path�� ����.
			sFilePath = cFile.GetFilePath();

			m_sError = _T("No error");
			m_dwError = 0; // �����ڵ� �ʱ�ȭ.
			m_bAskIfReadOnly = FALSE; // File�� Read Only�� ���� ����.
			m_bOverwriteMode = TRUE; // File�� Over Write��.
			m_bAborted = FALSE; // ���ϰ��� ó���� Abort���� �ʴ´�.
			m_iRecursionLimit = -1; // ���������� ������ �ʱ�ȭ�Ѵ�. ������� �ǹ� ����.

			sDest = sDestFolder + sFile;
			if (!Copy(sFilePath, sDest))
			{
				ShowError();
				return -3; // Copy ����.
			}
			
			nCopyed++;
			nSerial = _tstoi(sFile);
		}
	}
	EndWaitCursor();
	
	if(nCopyed > 0)
		return nSerial;

	return 0;
}

void CMyFile::DelPcrAll(CString strPath)
{
	CString strFileName;
	CString strFilePath;
	CFileFind cFile;
	BOOL bExist;

	bExist = cFile.FindFile(strPath + _T("*.pcr"));
	if(!bExist)
		return;

	BeginWaitCursor();
	while(bExist)
	{
		bExist = cFile.FindNextFile();
		if(cFile.IsDots()) continue;
		if(!cFile.IsDirectory())
		{
			// ���ϸ��� ����.
			strFileName = cFile.GetFileName();
			
			// ���� Path�� ����.
			strFilePath = cFile.GetFilePath();

			m_sError = _T("No error");
			m_dwError = 0; // �����ڵ� �ʱ�ȭ.
			m_bAskIfReadOnly = FALSE; // File�� Read Only�� ���� ����.
			m_bOverwriteMode = TRUE; // File�� Over Write��.
			m_bAborted = FALSE; // ���ϰ��� ó���� Abort���� �ʴ´�.
			m_iRecursionLimit = -1; // ���������� ������ �ʱ�ȭ�Ѵ�. ������� �ǹ� ����.
		
			if (!DeleteFolerOrFile(strFilePath))
			{
				ShowError();
				return; // ����.
			}
		}
	}
	EndWaitCursor();
	
	return; // Sucess.
}

BOOL CMyFile::ChkLotEnd(CString sPath)
{
	// ������ �д´�.
	char FileD[MAX_PATH];
	char *FileData;
	CString strFileData;
	size_t nFileSize, nRSize;
	int nTemp;
	CString strFileName;
	FILE *fp;

	//strcpy(FileD, sPath);
	//_tcscpy(FileD, sPath);
	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		//FileData = (char*)malloc( nFileSize );
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		fclose(fp);
		free(FileData);
	}
	else
		return FALSE;

	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;

	//strHeaderErrorInfo
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);		// 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	//strTotalBadPieceNum = strFileData;
	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;


	if (_tstoi(strHeaderErrorInfo) == -2)
		return TRUE;

	return FALSE;
}

