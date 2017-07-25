VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox Text1 
      Height          =   270
      Left            =   1080
      TabIndex        =   4
      Text            =   "Text1"
      Top             =   120
      Width           =   1095
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   3000
      Left            =   3600
      Top             =   120
   End
   Begin VB.CommandButton Command4 
      Caption         =   "发送"
      Height          =   375
      Left            =   120
      TabIndex        =   3
      Top             =   1560
      Width           =   1695
   End
   Begin VB.CommandButton Command3 
      Caption         =   "停止测试"
      Height          =   375
      Left            =   2160
      TabIndex        =   2
      Top             =   1560
      Width           =   2055
   End
   Begin VB.CommandButton Command2 
      Caption         =   "开始测试"
      Height          =   375
      Left            =   2160
      TabIndex        =   1
      Top             =   840
      Width           =   2055
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   240
      TabIndex        =   0
      Top             =   720
      Width           =   1575
   End
   Begin VB.Label Label1 
      Caption         =   "串口号"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   855
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function SendDatafun Lib "dlltpzp.DLL" (ByVal comport As Long, ByVal bps As Long, ByVal address As Long, ByVal senddata As String, ByVal color As Long, ByVal showMode As Long, ByVal showSpeed As Long, ByVal showTime As Long) As Long
Private Declare Function picSend Lib "dlltpzp.DLL" (ByVal mm_handle As Long, ByVal address As Long, ByVal commandbyte As Long, ByVal senddata As String, ByVal color As Long) As Long
Private Declare Function plainSend Lib "dlltpzp.DLL" (ByVal mm_handle As Long, ByVal address As Long, ByVal plainnum As Long, ByRef plainData As Byte, ByVal sendplainDatenum As Long) As Long
Private Declare Function portOpen Lib "dlltpzp.DLL" (ByVal com As Long, ByVal bps As Long, ByVal par As Byte, ByVal dbit As Byte, ByVal sbit As Byte) As Long
Private Declare Function portClose Lib "dlltpzp.DLL" (ByVal mm_handle As Long) As Integer

Private Sub Command1_Click()
Dim ret As Long
  ret = SendDatafun(4, 38400, 1, "1231251sidld", 17, 2, 6, 6)
  Caption = ret
End Sub

Private Sub Command2_Click()
  Timer1.Enabled = True

End Sub

Private Sub Command3_Click()
  Timer1.Enabled = False
  
End Sub

Private Sub Command4_Click()
    Dim mm_handle As Long
    Dim ret As Long
    
    Dim comport As Long
    Dim bps As Long
    Dim par As Byte
    Dim dbit As Byte
    Dim sbit As Byte
    
    Dim data() As Byte
    Dim datanum As Long
    Dim i As Long
    Dim plainnum As Long
    Dim commandbyte As Long
    Dim senddata As String
    
    
    'Initialize COM2
    comport = 9
    bps = 9600
    par = 110
    dbit = 8
    sbit = 1

    mm_handle = portOpen(comport, bps, par, dbit, sbit)
    senddata = "测试数据1"
    commandbyte = 5
    ret = picSend(mm_handle, 1, commandbyte, senddata, 51)
    Caption = mm_handle & " " & ret
    commandbyte = 6
    senddata = "测试数据2"
    ret = picSend(mm_handle, 1, commandbyte, senddata, 51)
    Caption = Caption & ret
    
    plainnum = 2
    ReDim data(plainnum * 4) As Byte
    
    For i = 1 To plainnum
        If i Mod 2 <> 0 Then
            data(4 * i - 4) = 5 'commandbyte
            data(4 * i - 3) = 8 'showmode
            data(4 * i - 2) = 1 'showspeed
            data(4 * i - 1) = 3   'showtime
        Else
            data(4 * i - 4) = 6 'commandbyte
            data(4 * i - 3) = 0 'showmode
            data(4 * i - 2) = 1 'showspeed
            data(4 * i - 1) = 1   'showtime
        End If
    Next i
    
    datanum = plainnum * 4 - 1
    ret = plainSend(mm_handle, 1, plainnum, data(0), datanum)
    Caption = Caption & ret
'     LogFile "addressID = " & addressID & " , plainSendreturnvalue = " & plainSendreturnvalue
    portClose (mm_handle)

End Sub

Private Sub Timer1_Timer()
  Call Command4_Click
End Sub
