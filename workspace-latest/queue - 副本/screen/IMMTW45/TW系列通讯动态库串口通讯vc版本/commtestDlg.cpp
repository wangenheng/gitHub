// commtestDlg.cpp : implementation file
//
#include "stdafx.h"
#include "commtest.h"
#include "commtestDlg.h"
#include "commdll.h"
#include<windows.h>
#include "winsock2.h" 
#pragma   comment(lib,"wsock32.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
byte  buff[1024];
byte databuff[600000];
int i,ret;
CString  str,str1;
long  Sockid;
byte  recbuff[4096];
HDC  GDC;

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
// CCommtestDlg dialog

CCommtestDlg::CCommtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommtestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommtestDlg)
	m_SendData = _T("");
	m_CommPort = _T("");
	m_Bps = _T("");
	m_Address = _T("");
	m_DescPort = _T("");
	m_Width = _T("");
	m_Height = _T("");
	m_PicNo = _T("");
	m_ShowSpeed = _T("");
	m_ShowTime = _T("");
	m_ShowMode = _T("");
	m_FontName = _T("");
	m_FontSize = _T("");
	m_TextWidth = _T("");
	m_TextTop = _T("");
	m_TextLeft = _T("");
	m_TextHeight = _T("");
	m_Text = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCommtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommtestDlg)
	DDX_Control(pDX, IDC_DescIP, m_DescIP);
	DDX_Control(pDX, IDC_STATIC1, m_Picture1);
	
	DDX_Text(pDX, IDC_CommPort, m_CommPort);
	DDX_Text(pDX, IDC_BPS, m_Bps);
	DDX_Text(pDX, IDC_Address, m_Address);

	DDX_Text(pDX, IDC_SENDDATA, m_SendData);
	DDX_Text(pDX, IDC_WIDTH, m_Width);
	DDX_Text(pDX, IDC_HEIGHT, m_Height);
	DDX_Text(pDX, IDC_PICNO, m_PicNo);
	DDX_Text(pDX, IDC_SHOWSPEED, m_ShowSpeed);
	DDX_Text(pDX, IDC_SHOWTIME, m_ShowTime);
	DDX_Text(pDX, IDC_SHOWMODE, m_ShowMode);
	DDX_Text(pDX, IDC_FONTNAME, m_FontName);
	DDX_Text(pDX, IDC_FONTSIZE, m_FontSize);
	DDX_Text(pDX, IDC_TEXTWIDTH, m_TextWidth);
	DDX_Text(pDX, IDC_TEXTTOP, m_TextTop);
	DDX_Text(pDX, IDC_TEXTLEFT, m_TextLeft);
	DDX_Text(pDX, IDC_TEXTHEIGHT, m_TextHeight);
	DDX_Text(pDX, IDC_TEXT, m_Text);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCommtestDlg, CDialog)
	//{{AFX_MSG_MAP(CCommtestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPENNET, OnOpennet)
	ON_BN_CLICKED(IDC_DATASEND, OnDatasend)
	ON_BN_CLICKED(IDC_RECEIVEDATA, OnReceivedata)
	ON_BN_CLICKED(IDC_CLOSENET, OnClosenet)
	ON_BN_CLICKED(IDC_COLLIGATESENDDATA, OnColligatesenddata)
	ON_BN_CLICKED(IDC_OPENSCREEN, OnOpenscreen)
	ON_BN_CLICKED(IDC_CLOSESCREEN, OnClosescreen)
	ON_BN_CLICKED(IDC_SENDPIC, OnSendpic)
	ON_BN_CLICKED(IDC_SENDPLAN, OnSendplan)
	ON_BN_CLICKED(IDC_INTERCUT, OnIntercut)
	ON_BN_CLICKED(IDC_INITIALIZE, OnInitialize)
	ON_BN_CLICKED(IDC_GENEDATA, OnGenedata)
	ON_BN_CLICKED(IDC_SENDGENEPIC, OnSendgenepic)
	ON_BN_CLICKED(IDC_RELEASEPIC, OnReleasepic)
	ON_BN_CLICKED(IDC_Wtite, OnWtite)
	ON_BN_CLICKED(IDC_Read, OnRead)
	ON_EN_CHANGE(IDC_BPS, OnChangeBps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommtestDlg message handlers

BOOL CCommtestDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
	test_Initialize();
	CFileFind finder;
	char strBuff[256];
	CString strFilePath;
    GetCurrentDirectory(256,strBuff);
	strFilePath.Format("%s\\Test.ini",strBuff);
	if(finder.FindFile(strFilePath))
	    CCommtestDlg::OnRead(); 
	else
	{  
		CString   strIP="192.0.168.198";   
        DWORD   dwIP;   
        dwIP=inet_addr(strIP);   
        unsigned char *pIP=(unsigned   char*)&dwIP;   
        m_DescIP.SetAddress(*pIP,*(pIP+1),*(pIP+2),*(pIP+3)); 	
        m_CommPort="1";
		m_Bps="38400";
		m_Address="1";
		m_Width="192";
		m_Height="64";
		m_DescPort="1";
        m_LocalPort="38400";
        m_SendData="222222";
		m_PicNo="1";
        m_ShowMode="0";
		m_ShowSpeed="1";
		m_ShowTime="1";
        m_TextLeft="0";
		m_TextTop="0";
		m_TextWidth="192";
		m_TextHeight="48";
		m_FontName="宋体";
		m_FontSize="12";
		m_Text="欢迎光临";
        UpdateData(FALSE);
        ((CComboBox*)GetDlgItem(IDC_FONTCOLOR))->SetCurSel(2);
	}
	GetDlgItem(IDC_Read)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_Wtite)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_OPENNET)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_DATASEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RECEIVEDATA)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_CLOSENET)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLLIGATESENDDATA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SENDDATA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	// Add "About..." menu item to system menu.

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

void CCommtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCommtestDlg::OnPaint() 
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
// the minimized window.
HCURSOR CCommtestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
  

void CCommtestDlg::OnOpennet() 
{
    UpdateData(TRUE); 
}

void CCommtestDlg::OnDatasend() 
{ 
    UpdateData(TRUE); 
    str=m_SendData;
    for(int i=0;i<str.GetLength();i++) 
    {   
	    int j=(int)(str.GetAt(i)); 
	    buff[i]=(byte)(j); 
    }
    byte   nField[4];   
    m_DescIP.GetAddress(nField[0],nField[1],nField[2],nField[3]);   
    CString   csAddr;   
    csAddr.Format("%d.%d.%d.%d",nField[0],nField[1],nField[2],nField[3]);
}

void CCommtestDlg::OnReceivedata() 
{
    char buffer[20];
    UpdateData(TRUE); 
    // for(int i=0;i<ret;i++)
    //     buff[i]=recbuff[i];
    itoa(ret,buffer,10);
    CWnd::SetWindowText(buffer);
    str="";
    for(int i=1;i<=atoi(buffer);i++) 
	    str=str+(char)recbuff[i-1];
    SetDlgItemText(IDC_EDIT1,str);
}

void CCommtestDlg::OnClosenet() 
{
}

void CCommtestDlg::OnColligatesenddata() 
{
    UpdateData(TRUE);
    str=m_SendData;
    for(int i=0;i<str.GetLength();i++) 
    {   
	    int j=(int)(str.GetAt(i)); 
	    buff[i]=(byte)(j); 
    } 
    __try
	{
//        Sockid=SocketOpen(atoi(m_LocalPort));
        if(Sockid<0)
            MessageBox("open error");
        else
		{
		    struct   in_addr   inAddr;
	     	DWORD  dwip;
	    	m_DescIP.GetAddress(dwip);
            inAddr.S_un.S_addr=dwip;   //   你的整数ip地址   
            char   *pIp=inet_ntoa(inAddr);
//            ret=SendData(Sockid,pIp,atoi(m_DescPort),buff,sizeof(buff)/sizeof(byte),str.GetLength());
            if(ret<0)
                MessageBox("send error");
            else
			{
	            char buffer[20];
                Sleep(50);
//                ret=ReadData(Sockid,recbuff,sizeof(recbuff)/sizeof(byte));
                for(i=0;i<ret;i++)
                    buff[i]=recbuff[i];
                itoa(ret,buffer,10);
                CWnd::SetWindowText(buffer);
                str1="";
                for(i=1;i<=atoi(buffer);i++) 
                    str1= str1 + char(buff[i]);
                SetDlgItemText(IDC_EDIT1,str1);
			}
		}
	}
    __finally
	{  
//	    SocketClose(Sockid);
	}	
}		


void CCommtestDlg::OnOpenscreen() 
{
    int ret;
    char buffer[20];
    UpdateData(TRUE); 
	byte   nField[4];   
    m_DescIP.GetAddress(nField[0],nField[1],nField[2],nField[3]);   
    CString   csAddr;   
    csAddr.Format("%d.%d.%d.%d",nField[0],nField[1],nField[2],nField[3]);
    ret=ComShut(atoi(m_CommPort),atoi(m_Bps),atoi(m_Address),1);
    itoa(ret,buffer,10);
    CWnd::SetWindowText(buffer);	
}

void CCommtestDlg::OnClosescreen() 
{
    int ret;
    char buffer[20];
    UpdateData(TRUE); 
	byte   nField[4];   
    m_DescIP.GetAddress(nField[0],nField[1],nField[2],nField[3]);   
    CString   csAddr;   
    csAddr.Format("%d.%d.%d.%d",nField[0],nField[1],nField[2],nField[3]);
    ret=ComShut(atoi(m_CommPort),atoi(m_Bps),atoi(m_Address),0);
    itoa(ret,buffer,10);
    CWnd::SetWindowText(buffer);	
}

void CCommtestDlg::OnSendpic() 
{
    //for i:=0 to 8000 do databuff[i]:=7;
    char buffer[20];
    UpdateData(TRUE); 
    CDC *pDC= GetDlgItem(IDC_STATIC1)->GetDC();
    HDC hdc=pDC->GetSafeHdc(); 
	byte   nField[4];   
    m_DescIP.GetAddress(nField[0],nField[1],nField[2],nField[3]);   
    CString   csAddr;   
    csAddr.Format("%d.%d.%d.%d",nField[0],nField[1],nField[2],nField[3]);
    ret=ComSendPicData(atoi(m_CommPort),atoi(m_Bps),atoi(m_Address),atoi(m_PicNo),hdc,atoi(m_Width),atoi(m_Height),2,2);
    itoa(ret,buffer,10);
    CWnd::SetWindowText(buffer);	
}

void CCommtestDlg::OnSendplan() 
{
    int ret;
    byte sendbuff[1024];
    char buffer[20];
    UpdateData(TRUE); 
    int a=atoi(m_PicNo);
    sendbuff[0]=(byte)a;
    int b=atoi(m_ShowMode);
    sendbuff[1]=(byte)b;
    int c=atoi(m_ShowSpeed);
    sendbuff[2]=(byte)c;
    int d=atoi(m_ShowTime);
    sendbuff[3]=(byte)d;
	byte   nField[4];   
    m_DescIP.GetAddress(nField[0],nField[1],nField[2],nField[3]);   
    CString   csAddr;   
    csAddr.Format("%d.%d.%d.%d",nField[0],nField[1],nField[2],nField[3]);
    ret=ComSendList(atoi(m_CommPort),atoi(m_Bps),atoi(m_Address),sendbuff,sizeof(sendbuff)/sizeof(byte),4);
    itoa(ret,buffer,10);
    CWnd::SetWindowText(buffer); 
}

void CCommtestDlg::OnIntercut() 
{
    int showmode,showspeed,showtime;
    char buffer[20];
    UpdateData(TRUE); 
	CDC *pDC= GetDlgItem(IDC_STATIC2)->GetDC();
    HDC hdc=pDC->GetSafeHdc(); 
    showmode=atoi(m_ShowMode);
    showspeed=atoi(m_ShowSpeed);
    showtime=atoi(m_ShowTime);
	byte   nField[4];   
    m_DescIP.GetAddress(nField[0],nField[1],nField[2],nField[3]);   
    CString   csAddr;   
    csAddr.Format("%d.%d.%d.%d",nField[0],nField[1],nField[2],nField[3]);
    //for i=0 to 8000 do databuff[i]:=17;
    itoa(ret,buffer,10);
    CWnd::SetWindowText(buffer);	
}

void CCommtestDlg::OnInitialize() 
{
    UpdateData(TRUE); 
    GDC=GDCCreate(atoi(m_Width),atoi(m_Height));		
}

void CCommtestDlg::OnGenedata() 
{
    CString  str2;
    CRect rect;
    UpdateData(TRUE); 
    CDC *pDC= GetDlgItem(IDC_STATIC1)->GetDC();
    GetDlgItemText(IDC_TEXT ,str2); 
    int j=((CComboBox*)GetDlgItem(IDC_FONTCOLOR))->GetCurSel();
    DrawRectString(GDC,str2.GetBuffer(0),m_FontName.GetBuffer(0),atoi(m_FontSize),j+1,atoi(m_TextLeft),atoi(m_TextTop),atoi(m_TextWidth),atoi(m_TextHeight));
    GetDlgItem(IDC_STATIC1)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    GetDlgItem(IDC_STATIC1)->MoveWindow(rect.left,rect.top,atoi(m_Width),atoi(m_Height),true);
    CDC *pDC1=CDC::FromHandle(GDC);
    ((CStatic*)GetDlgItem(IDC_STATIC1))->SetBitmap(NULL);
    ((CStatic*)GetDlgItem(IDC_STATIC2))->SetBitmap(NULL);
    pDC->BitBlt(0,0,atoi(m_Width),atoi(m_Height),pDC1,0,0,SRCCOPY); 

	CDC *pDC2= GetDlgItem(IDC_STATIC2)->GetDC();
    pDC2->BitBlt(0,0,atoi(m_Width),atoi(m_Height),pDC,0,0,SRCCOPY);
}

void CCommtestDlg::OnSendgenepic() 
{
    char  buffer[20];
    UpdateData(TRUE); 
	byte   nField[4];   
    m_DescIP.GetAddress(nField[0],nField[1],nField[2],nField[3]);   
    CString   csAddr;   
    csAddr.Format("%d.%d.%d.%d",nField[0],nField[1],nField[2],nField[3]);
    ret=ComSendPicData(atoi(m_CommPort),atoi(m_Bps),atoi(m_Address),atoi(m_PicNo),GDC,atoi(m_Width),atoi(m_Height),2,2);
    itoa(ret,buffer,10);
    CWnd::SetWindowText(buffer);	
}

void CCommtestDlg::OnReleasepic() 
{
    GDCFree(GDC);	
}

void CCommtestDlg::OnWtite() 
{
    CString strSection        = "通讯动态库的测试参数";
    CString strSectionKey     = "DescIP";
    char strBuff[256];
    CString strValue        = _T("");
    CString strFilePath;

    GetCurrentDirectory(256,strBuff);   //获取当前路径
    strFilePath.Format("%s\\Test.ini",strBuff);

	byte   nField[4];   
    m_DescIP.GetAddress(nField[0],nField[1],nField[2],nField[3]);      
    strValue.Format("%d.%d.%d.%d",nField[0],nField[1],nField[2],nField[3]);     
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);   

    strSectionKey="Width";
    GetDlgItemText(IDC_WIDTH,strValue);   
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="Height";
    GetDlgItemText(IDC_HEIGHT,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="DescPort";
    GetDlgItemText(IDC_DESCPORT,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="LocalPort";
    GetDlgItemText(IDC_LOCALPORT,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="SendData";
    GetDlgItemText(IDC_SENDDATA,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="PictureNo";
    GetDlgItemText(IDC_PICNO,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="ShowMode";
    GetDlgItemText(IDC_SHOWMODE,strValue);   
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="ShowSpeed";
    GetDlgItemText(IDC_SHOWSPEED,strValue);   
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="ShowTime";
    GetDlgItemText(IDC_SHOWTIME,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="Textleft";
    GetDlgItemText(IDC_TEXTLEFT,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="TextTop";
    GetDlgItemText(IDC_TEXTTOP,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="TextWidth";
    GetDlgItemText(IDC_TEXTWIDTH,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="TextHeight";
    GetDlgItemText(IDC_TEXTHEIGHT,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="FontName";
    GetDlgItemText(IDC_FONTNAME,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="FontSize";
    GetDlgItemText(IDC_FONTSIZE,strValue);    
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

    strSectionKey="FontColor";   
    ((CComboBox*)GetDlgItem(IDC_FONTCOLOR))->GetWindowText(strValue);  
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);	

    strSectionKey="Text";   
    ((CEdit*)GetDlgItem(IDC_TEXT))->GetWindowText(strValue);  
    WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);	
}

void CCommtestDlg::OnRead() 
{
	CString strSection        = "通讯动态库的测试参数";
    CString strSectionKey     = "DescIP";
    char strBuff[256];
    CString strValue        = _T("");
    CString strFilePath;

    GetCurrentDirectory(256,strBuff);   //获取当前路径
    strFilePath.Format("%s\\Test.ini",strBuff);

    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath); 
    strValue=strBuff;
	DWORD   dwIP;   
    dwIP=inet_addr(strValue);   
    unsigned char *pIP=(unsigned   char*)&dwIP;   
    m_DescIP.SetAddress(*pIP,*(pIP+1),*(pIP+2),*(pIP+3));

    strSectionKey="Width";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_WIDTH,strValue);

    strSectionKey="Height";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_HEIGHT,strValue);

    strSectionKey="DescPort";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_DESCPORT,strValue);

    strSectionKey="LocalPort";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_LOCALPORT,strValue);

    strSectionKey="SendData";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_SENDDATA,strValue);

    strSectionKey="PictureNo";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_PICNO,strValue);

    strSectionKey="ShowMode";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_SHOWMODE,strValue);

    strSectionKey="ShowSpeed";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_SHOWSPEED,strValue);

    strSectionKey="ShowTime";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_SHOWTIME,strValue);

    strSectionKey="Textleft";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_TEXTLEFT,strValue);

    strSectionKey="TextTop";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_TEXTTOP,strValue);

    strSectionKey="TextWidth";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_TEXTWIDTH,strValue);

    strSectionKey="TextHeight";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_TEXTHEIGHT,strValue);

    strSectionKey="FontName";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_FONTNAME,strValue);

    strSectionKey="FontSize";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_FONTSIZE,strValue);

    strSectionKey="FontColor";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    int d=((CComboBox*)GetDlgItem(IDC_FONTCOLOR))->SelectString(0,strValue); 
    ((CComboBox*)GetDlgItem(IDC_FONTCOLOR))->SetCurSel(d);

    strSectionKey="Text";
    GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);
    strValue=strBuff;
    SetDlgItemText(IDC_TEXT,strValue);
}

BOOL CCommtestDlg::DestroyWindow() 
{
	CCommtestDlg::OnWtite(); 
	return CDialog::DestroyWindow();
}

void CCommtestDlg::OnChangeBps() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
