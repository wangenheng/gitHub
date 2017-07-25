// commtest.h : main header file for the COMMTEST application
//

#if !defined(AFX_COMMTEST1_H__981E6C42_A271_4A8D_93BC_D292323FEDCA__INCLUDED_)
#define AFX_COMMTEST1_H__981E6C42_A271_4A8D_93BC_D292323FEDCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCommtestApp:
// See commtest.cpp for the implementation of this class
//

class CCommtestApp : public CWinApp
{
public:
	CCommtestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommtestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCommtestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTEST_H__981E6C42_A271_4A8D_93BC_D292323FEDCA__INCLUDED_)
