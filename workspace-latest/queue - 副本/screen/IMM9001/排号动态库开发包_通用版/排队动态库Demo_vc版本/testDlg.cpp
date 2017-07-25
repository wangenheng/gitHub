// testDlg.cpp : implementation file
//
#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "testdll.h"
#include <windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
HANDLE   mm_handle;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_Address = _T("");
	m_Content = _T("");
	m_ShowMode = _T("");
	m_ShowTime = _T("");
	m_SerialNo = _T("");
	m_BaudRate = _T("");
	m_LeafNo = _T("");
	m_ShowSpeed = _T("");
	m_WinColWidth = _T("");
	m_WinNo = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Text(pDX, IDC_ADDRESS, m_Address);
	DDX_Text(pDX, IDC_CONTENT, m_Content);
	DDX_Text(pDX, IDC_SHOWMODE, m_ShowMode);
	DDX_Text(pDX, IDC_SHOWTIME, m_ShowTime);
	DDX_Text(pDX, IDC_SERIALNUMBER, m_SerialNo);
	DDX_Text(pDX, IDC_BAUDRATE, m_BaudRate);
	DDX_Text(pDX, IDC_LEAFNUMBER, m_LeafNo);
	DDX_Text(pDX, IDC_SHOWSPEED, m_ShowSpeed);
	DDX_Text(pDX, IDC_WINCOLWIDTH, m_WinColWidth);
	DDX_Text(pDX, IDC_WINNO, m_WinNo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_AUTOSEND, OnAutosend)
	ON_BN_CLICKED(IDC_PICTURESEND, OnPicturesend)
	ON_BN_CLICKED(IDC_OPENSERIALNO, OnOpenserialno)
	ON_BN_CLICKED(IDC_SCREENTEST, OnScreentest)
	ON_BN_CLICKED(IDC_CLOSESERIALNO, OnCloseserialno)
	ON_BN_CLICKED(IDC_SETWINNO, OnSetwinno)
	ON_BN_CLICKED(IDC_WriteIni, OnWriteIni)
	ON_BN_CLICKED(IDC_ReadIni, OnReadIni)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{   
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.
    test_Initialize();
    CFileFind finder;
    char strBuff[256];
    CString strFilePath;
	GetCurrentDirectory(256,strBuff);
    strFilePath.Format("%s\\Test.ini",strBuff);
    if(finder.FindFile(strFilePath))
        CTestDlg::OnReadIni(); 
    else
	{
		SetDlgItemText(IDC_SERIALNUMBER,"1");
		SetDlgItemText(IDC_BAUDRATE,"9600");
		SetDlgItemText(IDC_ADDRESS,"1");
		SetDlgItemText(IDC_CONTENT,"欢迎光临");
		SetDlgItemText(IDC_LEAFNUMBER,"2");
		SetDlgItemText(IDC_SHOWMODE,"1");
		SetDlgItemText(IDC_SHOWTIME,"1");
		SetDlgItemText(IDC_SHOWSPEED,"1");
		SetDlgItemText(IDC_WINCOLWIDTH,"16");
		SetDlgItemText(IDC_WINNO,"12");
	}   
	GetDlgItem(IDC_ReadIni)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_WriteIni)->ShowWindow(SW_HIDE);
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDlg::OnAutosend() 
{
	int i;
    char str[100]; 
    UpdateData(TRUE);  
    i=SendDatafun(atoi(m_SerialNo),atoi(m_BaudRate),atoi(m_Address),m_Content.GetBuffer(0),17,atoi(m_ShowMode),atoi(m_ShowTime),atoi(m_ShowSpeed)); 
    CWnd::SetWindowText(itoa(i,str,10));
}

void CTestDlg::OnPicturesend() 
{	
    int i;
    char str[100]; 
    UpdateData(TRUE);
    i=SendDatagen(atoi(m_SerialNo),atoi(m_BaudRate),atoi(m_Address),atoi(m_LeafNo),m_Content.GetBuffer(0),18,atoi(m_ShowMode),atoi(m_ShowTime),atoi(m_ShowSpeed));
    CWnd::SetWindowText(itoa(i,str,10));	
}

void CTestDlg::OnOpenserialno() 
{

    int i;
    char str[100]; 
    UpdateData(TRUE);
    i=ComShut(atoi(m_SerialNo),atoi(m_BaudRate),atoi(m_Address),1);
    CWnd::SetWindowText(itoa(i,str,10));	
}

void CTestDlg::OnScreentest() 
{
	
    int i;
    char str[100]; 
    UpdateData(TRUE);
    i=SendDataZHLine(atoi(m_SerialNo),atoi(m_BaudRate),atoi(m_Address),m_Content.GetBuffer(0),19); 
    CWnd::SetWindowText(itoa(i,str,10));	
}

void CTestDlg::OnCloseserialno() 
{ 
    int i;
    char str[100]; 
    UpdateData(TRUE);
    i=ComShut(atoi(m_SerialNo),atoi(m_BaudRate),atoi(m_Address),0);
    CWnd::SetWindowText(itoa(i,str,10));	
}

void CTestDlg::OnSetwinno() 
{
    int ret;
    ret=setWindowNogen(atoi(m_SerialNo),atoi(m_BaudRate),atoi(m_Address),atoi(m_WinColWidth),17,m_WinNo.GetBuffer(0));
}

void CTestDlg::OnWriteIni() 
{
    
    CString strSection        = "带字库的测试参数";
    CString strSectionKey     = "SerialNumber";
    char strBuff[256];
    CString strValue        = _T("");
    CString strFilePath;

    GetCurrentDirectory(256,strBuff);   //获取当前路径
    strFilePath.Format("%s\\Test.ini",strBuff);

    GetDlgItemText(IDC_SERIALNUMBER,strValue);      //获取文本框内容：即串口号
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);   //写入ini文件中相应字段

    strSectionKey="BaudRate";
    GetDlgItemText(IDC_BAUDRATE,strValue);    //获取文本框内容：即波特率
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="Address";
    GetDlgItemText(IDC_ADDRESS,strValue);    //获取文本框内容：即地址
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="Content";
    GetDlgItemText(IDC_CONTENT,strValue);    //获取文本框内容：即内容
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="LeafNumber";
    GetDlgItemText(IDC_LEAFNUMBER,strValue);    //获取文本框内容：即叶号
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="ShowMode";
    GetDlgItemText(IDC_SHOWMODE,strValue);    //获取文本框内容：即显示方式
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="ShowTime";
    GetDlgItemText(IDC_SHOWTIME,strValue);    //获取文本框内容：即显示时间
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="ShowSpeed";
    GetDlgItemText(IDC_SHOWSPEED,strValue);    //获取文本框内容：即显示速度
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="WinColWidth";
    GetDlgItemText(IDC_WINCOLWIDTH,strValue);    //获取文本框内容：即窗口号列度
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="WinNumber";
    GetDlgItemText(IDC_WINNO,strValue);    //获取文本框内容：即窗口号
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);
}

void CTestDlg::OnReadIni() 
{

    CString strSection        = "带字库的测试参数";
    CString strSectionKey     = "SerialNumber";
    char strBuff[256];
    CString strValue        = _T("");
    CString strFilePath;

    GetCurrentDirectory(256,strBuff);   //获取当前路径
    strFilePath.Format("%s\\Test.ini",strBuff);

    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath); //读取ini文件中相应字段的内容
    strValue=strBuff;
    SetDlgItemText(IDC_SERIALNUMBER,strValue);

    strSectionKey="BaudRate";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_BAUDRATE,strValue);

    strSectionKey="Address";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_ADDRESS,strValue);

    strSectionKey="Content";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_CONTENT,strValue);

    strSectionKey="LeafNumber";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_LEAFNUMBER,strValue);

    strSectionKey="ShowMode";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_SHOWMODE,strValue);


    strSectionKey="ShowTime";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_SHOWTIME,strValue);

    strSectionKey="ShowSpeed";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_SHOWSPEED,strValue);

    strSectionKey="WinColWidth";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_WINCOLWIDTH,strValue);

    strSectionKey="WinNumber";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_WINNO,strValue);	
}


BOOL CTestDlg::DestroyWindow() 
{
	CTestDlg::OnWriteIni();
	return CDialog::DestroyWindow();
}

void CTestDlg::OnButton1() 
{
    int i;
    char str[100]; 
    UpdateData(TRUE);
    i=SendDatagen16c(atoi(m_SerialNo),atoi(m_BaudRate),atoi(m_Address),atoi(m_LeafNo),m_Content.GetBuffer(0),18,atoi(m_ShowMode),atoi(m_ShowTime),atoi(m_ShowSpeed),20);
    CWnd::SetWindowText(itoa(i,str,10));	
	
}
