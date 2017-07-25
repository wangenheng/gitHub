// LcLedComm.h : main header file for the LCLEDCOMM DLL
//

#if !defined(AFX_LCLEDCOMM_H__932A7EA0_C2F4_4630_8BAE_32BAFC86DF20__INCLUDED_)
#define AFX_LCLEDCOMM_H__932A7EA0_C2F4_4630_8BAE_32BAFC86DF20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLcLedCommApp
// See LcLedComm.cpp for the implementation of this class
//

class CLcLedCommApp : public CWinApp
{
public:
	CLcLedCommApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLcLedCommApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLcLedCommApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LCLEDCOMM_H__932A7EA0_C2F4_4630_8BAE_32BAFC86DF20__INCLUDED_)
