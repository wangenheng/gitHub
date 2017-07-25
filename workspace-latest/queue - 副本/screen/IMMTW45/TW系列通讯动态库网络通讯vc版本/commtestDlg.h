// commtestDlg.h : header file
//

#if !defined(AFX_COMMTESTDLG_H__AA9681B2_E786_4A77_8695_DA00DE47AB0A__INCLUDED_)
#define AFX_COMMTESTDLG_H__AA9681B2_E786_4A77_8695_DA00DE47AB0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCommtestDlg dialog

class CCommtestDlg : public CDialog
{
// Construction
public:
	CCommtestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCommtestDlg)
	enum { IDD = IDD_COMMTEST_DIALOG };
	CIPAddressCtrl	m_DescIP;
	CStatic	m_Picture1;
	CString	m_LocalPort;
	CString	m_SendData;
	CString	m_DescPort;
	CString	m_Width;
	CString	m_Height;
	CString	m_PicNo;
	CString	m_ShowSpeed;
	CString	m_ShowTime;
	CString	m_ShowMode;
	CString	m_FontName;
	CString	m_FontSize;
	CString	m_TextWidth;
	CString	m_TextTop;
	CString	m_TextLeft;
	CString	m_TextHeight;
	CString	m_Text;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommtestDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCommtestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpennet();
	afx_msg void OnDatasend();
	afx_msg void OnReceivedata();
	afx_msg void OnClosenet();
	afx_msg void OnColligatesenddata();
	afx_msg void OnOpenscreen();
	afx_msg void OnClosescreen();
	afx_msg void OnSendpic();
	afx_msg void OnSendplan();
	afx_msg void OnIntercut();
	afx_msg void OnInitialize();
	afx_msg void OnGenedata();
	afx_msg void OnSendgenepic();
	afx_msg void OnReleasepic();
	afx_msg void OnWtite();
	afx_msg void OnRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTESTDLG_H__AA9681B2_E786_4A77_8695_DA00DE47AB0A__INCLUDED_)
