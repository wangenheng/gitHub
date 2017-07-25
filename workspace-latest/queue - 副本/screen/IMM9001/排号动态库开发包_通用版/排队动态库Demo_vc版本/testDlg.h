// testDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__A98935DB_B220_4F5C_A1F7_071A4BB6DED2__INCLUDED_)
#define AFX_TESTDLG_H__A98935DB_B220_4F5C_A1F7_071A4BB6DED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
	CString	m_Address;
	CString	m_Content;
	CString	m_ShowMode;
	CString	m_ShowTime;
	CString	m_SerialNo;
	CString	m_BaudRate;
	CString	m_LeafNo;
	CString	m_ShowSpeed;
	CString	m_WinColWidth;
	CString	m_WinNo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAutosend();
	afx_msg void OnPicturesend();
	afx_msg void OnOpenserialno();
	afx_msg void OnScreentest();
	afx_msg void OnCloseserialno();
	afx_msg void OnSetwinno();
	afx_msg void OnWriteIni();
	afx_msg void OnReadIni();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__A98935DB_B220_4F5C_A1F7_071A4BB6DED2__INCLUDED_)
