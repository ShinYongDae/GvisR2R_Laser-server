// Yield.h: interface for the CYield class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YIELD_H__E3F08C3B_D5E8_4EA8_9D65_0FEF61E2C772__INCLUDED_)
#define AFX_YIELD_H__E3F08C3B_D5E8_4EA8_9D65_0FEF61E2C772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYield  
{
	BOOL bValid;
	int nCol, nRow;		// Panel�� Col, Row �� Pcs��.
	int m_nSerial;
	double dYield[4], dYieldAll;	// 4��
	int nTotDef[4];

	int nTot;
	int nAllDef;

public:
	CYield();
	virtual ~CYield();

	BOOL IsValid();
	void SetDef(int nSerial, int *pDef);	// int pDef[4];
	double GetYield();
	double GetYield(int nStrip);
	int GetDef();
	int GetDef(int nStrip);
	int GetGood();
	int GetGood(int nStrip);
};

#endif // !defined(AFX_YIELD_H__E3F08C3B_D5E8_4EA8_9D65_0FEF61E2C772__INCLUDED_)
