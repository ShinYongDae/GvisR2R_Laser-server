// GlobalFunc.h: 
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////                          GlobalFunc.h Ver 1.6                                 ////
////                             Developer: CAM PART                               ////
////                               Corp   : GIGAVIS                                ////
////																			   ////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


#ifndef _GLOBAL_FUNC_
#define _GLOBAL_FUNC_

// #include "GlobalDefine.h"
#include "MyData.h"
#include "SafeLock.h"


char* StringToChar(CString str);
void StringToChar(CString str, char* pCh);
char* StringToChar(CString str, int &nLen);
TCHAR* StringToTCHAR(CString str);
wchar_t* StringToWCHAR(CString str, int &nLen);

CString CharToString(const char *str);
CString TCHARToString(TCHAR *str);
wchar_t* CharToWCHAR(const char *str, int &nLen);
char* WCHARToChar(const wchar_t *str);
TCHAR* CharToTCHAR(char *str);
char* TCHARToChar(TCHAR *str);
char* TCHARToChar(const TCHAR *tszStr);

char* UnicodeToMultibyte(CString strUnicode);
char* UnicodeToMultibyte(LPCWSTR szInput);
CString MultibyteToUnicode(LPCSTR pMultibyte);


CString DtoA(TCHAR* szFormat, double fTmp);
CString DtoA(double fTmp);
CString DtoA(TCHAR* szFormat, int nTmp);
CString DtoA(TCHAR* szFormat, double fTmp);
CString DtoA(int nTmp);
CString DtoA(UINT nTmp);

int OpenFileDialog(HWND hwnd,TCHAR* path,TCHAR *FileFilter,TCHAR* Title,int mode=1);
double D2R(double fDeg);
double R2D(double fRadian);
double mm2um(double fmm);
double um2mm(double fum);

void swap(double& p1,double& p2);

//+++++++++++++++++++++++++++++++++
// global math function
double GetInteriorAngle(double x1,double y1,double x2,double y2,double x3,double y3);
double GetInteriorAngle(FPOINT start,FPOINT middle,FPOINT end);
CfPoint RotatePoint(CfPoint fptRef,CfPoint fptCenter,double fRotAngle);
BOOL IsPerpendicular(CfPoint startPoint,CfPoint middlePoint,CfPoint endPoint);
CfPoint ArcCenter(CfPoint startPoint,CfPoint middlePoint,CfPoint endPoint);
double ArcRadius(CfPoint startPoint,CfPoint middlePoint,CfPoint endPoint);
double ArcAngle(CfPoint startPoint,CfPoint endPoint,CfPoint centerPoint);
double ArcAngle(CfPoint startPoint,CfPoint centerPoint,double fArcLength);
double ArcLength(CfPoint startPoint,CfPoint middlePoint,CfPoint endPoint);

BOOL CheckPointInQuad(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double x,double y);
void GetCornerPoint(double x1,double y1,double x2,double y2,double x3,double y3,double *x0,double *y0);

int GetVectorDir(double ax, double ay, double bx, double by, double cx, double cy);
int GetVectorDir(FPOINT start,FPOINT middle,FPOINT end);
int GetVectorDir(FPOINT3D start,FPOINT3D middle,FPOINT3D end);
double GetInteriorAngle(FPOINT start,FPOINT middle,FPOINT end);
double GetInteriorAngle(FPOINT3D start,FPOINT3D middle,FPOINT3D end);
int CheckIntersect(FLINE fLine1,FLINE fLine2);
BOOL GetIntersectPoint(FPOINT fPt1,FPOINT fPt2,FPOINT fPt3,FPOINT fPt4,FPOINT &fPt);
BOOL GetIntersectPoint(FPOINT3D fPt1,FPOINT3D fPt2,FPOINT3D fPt3,FPOINT3D fPt4,FPOINT3D &fPt);
BOOL GetIntersectPoint(FLINE fLine1,FLINE fLine2,FPOINT &fPt);
BOOL GetIntersectPoint(FLINE3D fLine1,FLINE3D fLine2,FPOINT3D &fPt);

double GetDistance(CfPoint3D FirstPoint,CfPoint3D SecondPoint); // ���� ������ �Ÿ��� ��´� 

void SaveLog(CString strMsg, int nType = 0);









//////////////////////////////////////////////////////////////////
// ���� ����ǰ� �ִ� ���α׷��� ���丮 �н��� �����Ѵ�.
CString  GetThisPath(HMODULE hModule=NULL);

//////////////////////////////////////////////////////////////////
// HDD�� Serial ��ȣ�� �����Ѵ�.
DWORD GetDiskSerial(int drv);
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// HDD�� CD�� Free Space�� �����Ѵ�.
DWORD GetDiskSpace(); 

//////////////////////////////////////////////////////////////////
// Function name    : IsRRN
// Description      : �Էµ� ���ڿ��� �˻��ؼ� �ֹε�Ϲ�ȣ���� ���θ� �Ǵ��Ѵ�.
// Return type      : BOOL 
// Argument         : CString sRRN
BOOL IsRRN(CString sRRN);

//////////////////////////////////////////////////////////////////
//
CString ExtractFirstWord(CString *str, CString sep,BOOL bTrim=FALSE);
//////////////////////////////////////////////////////////////////
//
CString ExtractFirstWord(CString& str, CString sep,BOOL bTrim=FALSE);
//////////////////////////////////////////////////////////////////
//
CString ExtractLastWord(CString *str, TCHAR sep,BOOL bTrim=FALSE);

CString ExtractLastWord(CString& str, TCHAR sep,BOOL bTrim=FALSE);

//////////////////////////////////////////////////////////////////
// Display Setting�� �ٲپ��ش�.
BOOL SetCurrentDisplayMode(DEVMODE *devMode);
//////////////////////////////////////////////////////////////////
// ���� Display Setting�� ���´�.
BOOL GetCurrentDisplayMode(DEVMODE *devMode);

#ifdef UNICODE
    #define MessageBoxTimeout MessageBoxTimeoutW
#else
    #define MessageBoxTimeout MessageBoxTimeoutA
#endif

typedef int (__stdcall *MSGBOXAAPI)(IN HWND hWnd,IN LPCSTR lpText, IN LPCSTR lpCaption,IN UINT uType, IN WORD wLanguageId,IN DWORD dwMilliseconds);
typedef int (__stdcall *MSGBOXWAPI)(IN HWND hWnd,IN LPCWSTR lpText, IN LPCWSTR lpCaption,IN UINT uType, IN WORD wLanguageId,IN DWORD dwMilliseconds);

int MessageBoxTimeoutA(HWND hWnd, LPCSTR lpText,LPCSTR lpCaption, UINT uType, WORD wLanguageId,DWORD dwMilliseconds);
int MessageBoxTimeoutW(HWND hWnd, LPCWSTR lpText,LPCWSTR lpCaption, UINT uType, WORD wLanguageId,DWORD dwMilliseconds);


#endif // !defined(_GLOBAL_FUNC_)
