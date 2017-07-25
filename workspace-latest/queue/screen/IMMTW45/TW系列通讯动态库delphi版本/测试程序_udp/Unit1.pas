unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls{,UdpSendData,UdpSockteUnit,PicToData},GenComm,ComSendData,
  ComCtrls;

type
  TForm1 = class(TForm)
    Label11: TLabel;
    Label12: TLabel;
    Timer1: TTimer;
    Label41: TLabel;
    Panel1: TPanel;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Button6: TButton;
    Button2: TButton;
    Edit5: TEdit;
    Edit4: TEdit;
    Label4: TLabel;
    Panel2: TPanel;
    ScrollBox1: TScrollBox;
    Image1: TImage;
    ScrollBox2: TScrollBox;
    Image2: TImage;
    Button17: TButton;
    Button9: TButton;
    Button18: TButton;
    Label13: TLabel;
    Label14: TLabel;
    Label30: TLabel;
    Label31: TLabel;
    Label32: TLabel;
    Label33: TLabel;
    Label35: TLabel;
    Label36: TLabel;
    Label37: TLabel;
    Label38: TLabel;
    Label39: TLabel;
    Label45: TLabel;
    Label46: TLabel;
    Label47: TLabel;
    Button1: TButton;
    Button7: TButton;
    Button8: TButton;
    Button10: TButton;
    Button11: TButton;
    Button12: TButton;
    Button13: TButton;
    Button14: TButton;
    Button15: TButton;
    Button16: TButton;
    Edit10: TEdit;
    Edit11: TEdit;
    Button28: TButton;
    Button29: TButton;
    Edit12: TEdit;
    PicX: TEdit;
    PicY: TEdit;
    PicW: TEdit;
    PicH: TEdit;
    Button30: TButton;
    Button31: TButton;
    Edit18: TEdit;
    Edit19: TEdit;
    Button32: TButton;
    Panel3: TPanel;
    Label1: TLabel;
    Edit1: TEdit;
    Label9: TLabel;
    ScrW: TEdit;
    Label10: TLabel;
    ScrH: TEdit;
    Label48: TLabel;
    comColor: TComboBox;
    Button21: TButton;
    Edit2: TEdit;
    Edit3: TEdit;
    Label15: TLabel;
    COMPORT: TEdit;
    Label16: TLabel;
    BPS: TEdit;
    Label17: TLabel;
    ADDRESS: TEdit;
    Label2: TLabel;
    Label3: TLabel;
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    Panel6: TPanel;
    Label34: TLabel;
    Label18: TLabel;
    Label50: TLabel;
    ListBox1: TListBox;
    Edit13: TEdit;
    Edit14: TEdit;
    ListBox2: TListBox;
    Button20: TButton;
    Panel4: TPanel;
    GroupBox1: TGroupBox;
    Label29: TLabel;
    Label49: TLabel;
    comCheckTime: TComboBox;
    txtCheckTime: TEdit;
    Button25: TButton;
    GroupBox2: TGroupBox;
    Label24: TLabel;
    Label23: TLabel;
    Label19: TLabel;
    Label21: TLabel;
    Label22: TLabel;
    Label20: TLabel;
    Label25: TLabel;
    openH: TEdit;
    openM: TEdit;
    CloseH: TEdit;
    CloseM: TEdit;
    ComAutoOpenRule: TComboBox;
    Button22: TButton;
    GroupBox3: TGroupBox;
    Label26: TLabel;
    comLightness: TComboBox;
    Button23: TButton;
    GroupBox4: TGroupBox;
    Label27: TLabel;
    Label28: TLabel;
    NewLocalPort: TEdit;
    NewIP: TEdit;
    Button24: TButton;
    Button27: TButton;
    Button19: TButton;
    Button26: TButton;
    Panel5: TPanel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Label40: TLabel;
    Label42: TLabel;
    Label43: TLabel;
    Label44: TLabel;
    Edit6: TEdit;
    Edit7: TEdit;
    Edit8: TEdit;
    Edit9: TEdit;
    Edit15: TEdit;
    Edit16: TEdit;
    Edit17: TEdit;
    Memo1: TMemo;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button7Click(Sender: TObject);
    procedure Button8Click(Sender: TObject);
    procedure Button10Click(Sender: TObject);
    procedure Button11Click(Sender: TObject);
    procedure Button9Click(Sender: TObject);
    procedure Button12Click(Sender: TObject);
    procedure Button13Click(Sender: TObject);
    procedure Button14Click(Sender: TObject);
    procedure Button15Click(Sender: TObject);
    procedure Button17Click(Sender: TObject);
    procedure Button18Click(Sender: TObject);
    procedure Button19Click(Sender: TObject);
    procedure Button20Click(Sender: TObject);
    procedure Button21Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button22Click(Sender: TObject);
    procedure Button23Click(Sender: TObject);
    procedure Button24Click(Sender: TObject);
    procedure Button25Click(Sender: TObject);
    procedure Button26Click(Sender: TObject);
    procedure Button16Click(Sender: TObject);
    procedure Button27Click(Sender: TObject);
    procedure Button28Click(Sender: TObject);
    procedure Button29Click(Sender: TObject);
    procedure Edit15Change(Sender: TObject);
    procedure Button31Click(Sender: TObject);
    procedure Button32Click(Sender: TObject);


  private
    { Private declarations }
  public
    { Public declarations }  
      procedure   HearBeatProc(Wnd:   HWnd;   Msg,   Event,   dwTime:   Integer);   stdcall;

  end;
const
      Frequency   =   1000;//每隔五秒钟整理一次内存，速度很快的   ：）

var
  Form1: TForm1;
  buff : array[1..1024] of byte;
  databuff:array[0..600000] of byte;
  i : integer;
  str,str1: string;
  sockid : longint;
  recbuff: array[0..4096] of byte;
  ret: integer;
  GDC:hdc;
  count : integer;
  InProc:   Boolean;
  sendtimes:array of integer;

implementation

  function NetGetErrmsg(var ErrValue:Pchar):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetSetSenddataNum(value:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';

  function NetShut(LocalPort:integer;DescIP:Pchar;index:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetPicSendData(LocalPort:integer;DescIP:Pchar;CommandByte:integer;var buff:array of byte;Width,Height,PColorSign,SendMode:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetListSend(LocalPort:integer;DescIP:Pchar;buff:array of byte;bufflen:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetListSendNew(LocalPort:integer;DescIP:Pchar;buff:array of byte;bufflen:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetPicSendOnce(LocalPort:integer;DescIP:Pchar;var buff:array of byte;Width,Height,PColorSign,SendMode,showmode,showspeed,showtime:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetPicSendOnceNew(LocalPort:integer;DescIP:Pchar;var buff:array of byte;PicX,PicY,Width,Height,PColorSign,SendMode,showmode,showspeed,showtime:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
  function GetArrayFromPic(GDC:hdc;var buff:array of byte;width,height,PColorSign:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetSendPicOnce(LocalPort:integer;DescIP:Pchar;GDC:hdc;Width,Height,PColorSign,SendMode,showmode,showspeed,showtime:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetSendPicOnceNew(LocalPort:integer;DescIP:Pchar;GDC:hdc;PicX,PicY,Width,Height,PColorSign,SendMode,showmode,showspeed,showtime:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetSendPicOnceTW04(LocalPort:integer;DescIP:Pchar;ScrNo:integer;GDC:hdc;PicX,PicY,Width,Height,PColorSign,SendMode,showmode,showspeed,showtime,pagetime:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetSendPicOnceNew9G(LocalPort:integer;DescIP:Pchar;GDC:hdc;ScrNo,PicX,PicY,Width,Height,picwidth,picheight,PColorSign,SendMode,showmode,showspeed,showtime,pagetime:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
  function NetSendPicData(LocalPort:integer;DescIP:Pchar;CommandByte:integer;GDC:hdc;Width,Height,PColorSign,SendMode:integer):integer;stdcall;
    StdCall external '../EV_SocketUdpDll.DLL';
function    SocketOpen(LocalPort:integer):longint;
    StdCall external '../EV_SocketUdpDll.DLL';
function    SocketClose(SockID:longint):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function    SendData(Sockid:longint;DescIP:Pchar;DescPort:integer; buf:array of byte;bufLen:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function    funDataSend(LocalPort:integer;DescIP:Pchar;DescPort:integer;bufLen:integer;var buf:array of byte):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function    ReadData(SockId:longint;var recbuff:array of byte):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function    DrawRectString(GDC:hdc;UserData:Pchar;FontName:Pchar;FontSize,FontColor,Left,Top,Width,Height:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function DrawLine(GDC:hdc;x0,y0,x,y,linewidth,linecolor:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function    GDCCreate(Width,Height:integer):hdc;
    StdCall external '../EV_SocketUdpDll.DLL';
function    GDCFree(GDC:hdc):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function NetSetAutoOpen(LocalPort:integer;DescIP:Pchar;openHour,openMinute,closeHour,closeMinute,autoRule:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function NetSetLightness(LocalPort:integer;DescIP:Pchar;lightValue:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function NetIPSet(LocalPort:integer;DescIP:Pchar;NewPort:integer;NewIP:Pchar):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function NetCardParamGet(LocalPort:integer;DescIP:Pchar;var buff:array of byte):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function NetCheckDateTime(LocalPort:integer;DescIP:Pchar;checkValue:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
function ClearMemory():integer;
    StdCall external '../EV_SocketUdpDll.DLL';

  function ComShut(ComPort:integer;bps:longint;address,index:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComSetAutoOpen(ComPort:integer;bps:longint;address,openHour,openMinute,closeHour,closeMinute,autoRule:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComCheckDateTime(ComPort:integer;bps:longint;address,checkValue:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComSetLightness(ComPort:integer;bps:longint;address,lightValue:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';

  function ComPicSendData(mm_handle,address,CommandByte:integer;var buff:array of byte;Width,Height,PColorSign,SendMode:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComListSend(mm_handle,address:integer;var buff:array of byte;bufflen:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComTimeSend(mm_handle,address:integer;var buff:array of byte;bufflen:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComIPComSet(mm_handle,address,newAddress,NewBps:integer;NewIP:Pchar):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComComSet(mm_handle,address,newAddress,NewBps:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComCardParamSet(mm_handle,address:integer;var buff:array of byte;buffLen:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComCardParamGet(mm_handle,address:integer;var buff:array of byte):integer;
    StdCall external '../EV_SocketUdpDll.DLL';

  function ComSendList(ComPort:integer;bps:longint;address:integer;var buff:array of byte;bufflen:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComSendListNew(ComPort:integer;bps:longint;address:integer;var buff:array of byte;bufflen:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComSendTime(ComPort:integer;bps:longint;address,id,left,top,width,height,size,color:integer;SendData:Pchar;PointDatetime:Tdatetime):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComSendPicData(ComPort:integer;bps:longint;address,CommandByte:integer;GDC:hdc;Width,Height,PColorSign,SendMode:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComSetIPCom(ComPort:integer;bps:longint;address,NewAddress,NewBps:integer;NewIP:Pchar):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComSetCom(ComPort:integer;bps:longint;address,NewAddress,NewBps:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComSetCardParam(ComPort:integer;bps:longint;address:integer;var buff:array of byte;buffLen:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComGetCardParam(ComPort:integer;bps:longint;address:integer;var buff:array of byte):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComPicSendDataOnce(mm_handle,address:integer;var buff:array of byte;Width,Height,PColorSign,SendMode,ShowTime:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';
  function ComSendPicDataOnce(ComPort:integer;bps:longint;address:integer;GDC:hdc;Width,Height,PColorSign,SendMode,ShowTime:integer):integer;
    StdCall external '../EV_SocketUdpDll.DLL';

{$R *.dfm}

procedure TForm1.Button1Click(Sender: TObject);
var
  ret : integer;
begin
  ret:=netshut(strtoint(edit3.text),pchar(edit1.text),1);
  form1.Caption :=inttostr(ret);
end;

procedure TForm1.Button2Click(Sender: TObject);
var
  k : integer;
begin

   str:=edit4.text;
   k:=length(str );
   for i:=1 to k do
     buff[i]:=ord(str[i]);
  try
    Sockid:=SocketOpen(strtoint(edit3.Text ));
    if Sockid<0 then
      showmessage ('open error')
    else
    begin
      ret:=Senddata(SockId,pchar(edit1.Text) ,strtoint(edit2.Text ),buff,length(str));
      if ret<0 then
        showmessage('send error')
      else
      begin
        sleep (50);
        ret:=readdata(sockid,recbuff);
        for i:=0 to ret-1 do
          buff[i]:=recbuff[i];
        form1.Caption :=inttostr(ret);
        str1:='';
        for i:=1 to strtoint(form1.Caption ) do
           str1:= str1 + char(buff[i]);
        edit5.Text :=str1;

      end;
    end;
  finally
    SocketClose(SockID);
  end;

end;

procedure TForm1.Button3Click(Sender: TObject);
begin
    Sockid:=SocketOpen(strtoint(edit3.Text ));

end;

procedure TForm1.Button4Click(Sender: TObject);
begin
   str:=edit4.text;
   for i:=1 to length(str ) do
     buff[i]:=ord(str[i]);
      ret:=Senddata(SockId,pchar(edit1.Text) ,strtoint(edit2.Text ),buff,length(str));
      if ret<0 then
        showmessage('send error');

end;

procedure TForm1.Button5Click(Sender: TObject);
begin
        ret:=readdata(sockid,recbuff);
//        for i:=0 to ret-1 do
//          buff[i]:=recbuff[i];
        form1.Caption :=inttostr(ret);
        str:='';
        for i:=1 to strtoint(form1.Caption ) do
           str:= str + char(recbuff[i-1]);
        edit5.Text :=str;

end;

procedure TForm1.Button6Click(Sender: TObject);
begin

    SocketClose(SockID);


end;

procedure TForm1.Button7Click(Sender: TObject);
var
  yy,mm,dd,hh,mi,ss,ms:word;
  str:string;
  p:pchar;
begin
//for i:=0 to 8000 do databuff[i]:=7;
netsetsenddatanum(strtoint(edit15.Text));
decodetime(now,hh,mi,ss,ms);
str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
//listbox1.Items.Add(str + '开始插播');
ret:=netSendpicData(strtoint(edit3.text),
                    pchar(edit1.text),
                    strtoint(edit6.Text ),
                    GDC,
                    strtoint(scrw.text),
                    strtoint(scrH.text),
                    comcolor.ItemIndex +1,
                    2);

decodetime(now,hh,mi,ss,ms);
str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
//if ret<0 then listbox1.Items.Add(str + '插播失败')
//else listbox1.Items.Add(str + '插播成功');
netgeterrmsg(p);
form1.Caption :=inttostr(ret) + '  ' + p;
end;

procedure TForm1.Button8Click(Sender: TObject);
var
  ret,bufflen:integer;
  sendbuff:array[0..12] of byte;
begin
 bufflen:=4;
 sendbuff[0]:=strtoint(edit6.Text );
 sendbuff[1]:=strtoint(edit7.Text );
 sendbuff[2]:=strtoint(edit8.text);
 sendbuff[3]:=strtoint(edit9.Text );
 ret:=NetListSend(strtoint(edit3.text),pchar(edit1.text),sendbuff,4);
 form1.Caption :=inttostr(ret);
end;

procedure TForm1.Button10Click(Sender: TObject);
var
  ret : integer;
begin
  ret:=netshut(strtoint(edit3.text),pchar(edit1.text),0);
  form1.Caption :=inttostr(ret);
end;

procedure TForm1.Button11Click(Sender: TObject);
var
  showmode,showspeed,showtime : integer;
  i:integer;
  yy,mm,dd,hh,mi,ss,ms:word;
  str1,str:string;
  p:pchar;

begin
setlength(sendtimes,strtoint(edit15.text) +3);
netsetsenddatanum(strtoint(edit15.Text));
showmode:=strtoint(edit7.Text );
showspeed:=strtoint(edit8.Text );
showtime:=strtoint(edit9.Text );
//for i:=0 to 8000 do databuff[i]:=17;
//for i:=0 to 7 do
decodetime(now,hh,mi,ss,ms);
str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
str:=str + '开始插播';
//ret:=netSendPicOnce(strtoint(edit3.text),pchar(edit1.text),
//      GDC, //image2.Canvas.Handle ,
//      strtoint(ScrW.text),strtoint(scrH.Text),2,2,showmode,showspeed,showtime);
ret:=netSendPicOnceNew(strtoint(edit3.text),pchar(edit1.text),
      GDC, //image2.Canvas.Handle ,
      strtoint(PIcx.text),strtoint(PicY.text), strtoint(PicW.text),strtoint(PicH.Text),comcolor.ItemIndex +1,2,showmode,showspeed,showtime);
netgeterrmsg(p);
//form1.Caption :=inttostr(ret);
decodetime(now,hh,mi,ss,ms);
if ret<0 then
begin
  listbox2.Items.Add(str);
  str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox2.Items.Add(str + '插播失败:' + inttostr(ret));
  listbox2.Items.Add(p);
end
else
begin
  listbox1.Items.Add(str);
  str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox1.Items.Add(str + '插播成功：' + inttostr(ret));
  listbox1.Items.Add(p);
  sendtimes[ret]:=sendtimes[ret]+1;
end;
edit13.Text :=inttostr(listbox1.Items.Count div 3 );
edit14.Text :=inttostr(listbox2.Items.Count div 3);
image2.Picture.Assign(nil);
str1:='重发次数：';
for i:=1 to high(sendtimes) do
  str1:=str1+ '   '  + inttostr(sendtimes[i]);
//  str1:=str1+ '   ' + inttostr(i) + '次:' + inttostr(sendtimes[i]);
label41.Caption :=str1;
  bitblt(image2.Canvas.Handle ,0,0,image1.width,image1.height,gdc ,0 ,0,srccopy);
end;

procedure TForm1.Button9Click(Sender: TObject);
var
  yy,mm,dd,hh,mi,ss,ms:word;
  str:string;
begin
  decodetime(now,hh,mi,ss,ms);
  str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ' ';
  inc(count);
  count:=count mod 1000000;
  caption:=inttostr(DrawRectString(GDC,pchar(str),'宋体',12,3,strtoint(edit11.Text ),0,1000,24));
  caption:=inttostr(DrawRectString(GDC,pchar(edit10.Text + 'di1'+inttostr(count)),'宋体',12,3,strtoint(edit11.Text ),20,1000,24));
  caption:=inttostr(DrawRectString(GDC,pchar(edit10.Text + 'di2'+inttostr(count)),'宋体',12,3,strtoint(edit11.Text ),40,1000,24));
  caption:=inttostr(DrawRectString(GDC,pchar(edit10.Text + 'di3'+inttostr(count)),'宋体',12,3,strtoint(edit11.Text ),60,1000,24));
  caption:=inttostr(DrawRectString(GDC,pchar(edit10.Text + 'di4'+inttostr(count)),'宋体',12,3,strtoint(edit11.Text ),80,1000,24));
  caption:=inttostr(DrawRectString(GDC,pchar(edit10.Text + 'di5'+inttostr(count)),'宋体',12,3,strtoint(edit11.Text ),110,1000,24));
  caption:=inttostr(DrawRectString(GDC,pchar(edit10.Text + 'di6'+inttostr(count)),'宋体',12,3,strtoint(edit11.Text ),130,1000,24));
  caption:=inttostr(DrawRectString(GDC,pchar(edit10.Text + 'di7'+inttostr(count)),'宋体',12,3,strtoint(edit11.Text ),150,1000,24));
  caption:=inttostr(drawline(gdc,0,0,100,100,1,2));  
  image1.Picture.Assign(nil);
  bitblt(image1.canvas.Handle ,0,0,strtoint(scrw.Text),strtoint(scrh.Text),gdc  ,0,0,srccopy);



end;

procedure TForm1.Button12Click(Sender: TObject);
var
  ret : integer;
begin
  ret:=ComShut(strtoint(Comport.Text ),strtoint(BPS.Text ),strtoint(Address.Text ),1);
  form1.Caption :=inttostr(ret);
end;

procedure TForm1.Button13Click(Sender: TObject);
var
  ret : integer;
begin
  ret:=ComShut(strtoint(Comport.text),strtoint(BPS.Text ),strtoint(Address.Text ),0);
  form1.Caption :=inttostr(ret);
end;

procedure TForm1.Button14Click(Sender: TObject);
begin
ret:=ComSendpicData(strtoint(Comport.text),StrToint(bps.text),strtoint(address.text),
      strtoint(edit6.Text ),image1.canvas.handle,strtoint(scrw.text),strtoint(scrH.text),
      comcolor.ItemIndex +1,2);
form1.Caption :=inttostr(ret);

end;

procedure TForm1.Button15Click(Sender: TObject);
var
  ret,bufflen:integer;
  sendbuff:array[0..1024] of byte;
begin
 bufflen:=4;
 sendbuff[0]:=strtoint(edit6.Text );
 sendbuff[1]:=strtoint(edit7.Text );
 sendbuff[2]:=strtoint(edit8.text);
 sendbuff[3]:=strtoint(edit9.Text );
 ret:=ComSendList(strtoint(ComPort.Text ),strtoint(Bps.Text ),strtoint(address.text),sendbuff,4);
 form1.Caption :=inttostr(ret);
end;





procedure TForm1.Button17Click(Sender: TObject);
begin
  image1.Picture.Assign(nil);
  image2.Picture.Assign(nil);
  image1.Width :=strtoint(scrw.Text );
  image1.Height  :=strtoint(scrh.Text );
  image2.Width :=image1.Width ;
  image2.Height :=image1.Height ;
  GDC:=GDCCreate(strtoint(scrw.Text ),strtoint(scrh.Text ));
  caption:=inttostr(gdc);
end;

procedure TForm1.Button18Click(Sender: TObject);
begin
  GDCFree(GDC);
end;

procedure TForm1.Button19Click(Sender: TObject);
begin
  if button19.Caption ='网络插播测试' then
  begin
      button19.Caption :='网络停止测试';
      InProc:=false;
      SetTimer(handle ,   1,   strtoint(edit12.Text) * 1000,   @Tform1.HearBeatProc);   //创建一个定时器
  end
  else
  begin
      button19.Caption :='网络插播测试';
      KillTimer(handle, 1);//  TimerID);
      InProc:=True;
  end;
  timer1.Enabled :=not timer1.Enabled ;
end;
  procedure   SaveMemory;
  begin
      if   Win32Platform   =   VER_PLATFORM_WIN32_NT   then
      begin   //整理内存
          SetProcessWorkingSetSize(GetCurrentProcess,   $FFFFFFFF,   $FFFFFFFF);
      end;
  end;

  //定时器要执行的回调函数
  procedure   Tform1.HearBeatProc(Wnd:   HWnd;   Msg,   Event,   dwTime:   Integer);   stdcall;
  begin
      if   (InProc   =   False)   then
      begin
          InProc   :=   True;
          try
            if msg=1 then
            begin
              form1.caption:='1';
              form1.button9click(nil);
              form1.button11click(nil);
//              form1.Button7Click(nil);
//            SaveMemory;
            end
            else
            begin
              form1.Caption :='2';
              form1.Button9Click(nil);
              form1.Button16Click(nil);
            end;
          finally
              InProc   :=   False;
          end;
      end;

  end;


procedure TForm1.Button20Click(Sender: TObject);
var
  i:integer;
begin
listbox1.Clear;
listbox2.Clear;
edit13.Text :='0';
edit14.Text :='0';
for i:= low(sendtimes) to high(sendtimes) do
  sendtimes[i]:=0;
end;

procedure TForm1.Button21Click(Sender: TObject);
var
  ret:integer;
  ParamBuff:array[0..255] of byte;
begin
  ret:=NetCardParamGet(strtoint(edit3.text),pchar(edit1.text),parambuff);
  if ret=18 then
  begin
      ret:=(Parambuff[11]*256 + Parambuff[9]+1)*8;
      scrw.Text :=inttostr(ret );

      ret:=(parambuff[10]+1) * 8;
      scrh.Text :=inttostr(ret);
      ret:=parambuff[5] div 64;
      comcolor.ItemIndex :=ret;
  end
  else
    showmessage('获取属性失败');

end;

procedure TForm1.FormCreate(Sender: TObject);
var
  i:integer;
begin
  comAutoopenrule.Clear;
  comautoopenrule.Items.Add('每周相同');
  comautoopenrule.Items.Add('周末除外');
  COMAUTOopenrule.ItemIndex :=0;
  with comlightness do
  begin
    clear;
    for i:=1 to 16 do
      items.Add(inttostr(i));
    itemindex:=15
  end;

  with comchecktime do
  begin
    clear;
    items.Add('+');
    items.Add('-');
    itemindex:=0;
  end;
  with comcolor do
  begin
    clear;
    items.Add('单色');
    items.Add('双色');
    items.Add('七彩');
    itemindex:=1;
  end;
  pagecontrol1.Pages[0].Caption :='批量插播';
  pagecontrol1.Pages[1].Caption :='功能设置';
  pagecontrol1.ActivePageIndex :=1;



end;

procedure TForm1.Button22Click(Sender: TObject);
var
  ret:integer;
begin
    ret:= NetSetAutoOpen(strtoint(edit3.text),
                  pchar(edit1.text),
                  strtoint(OpenH.text),
                  strtoint(OpenM.Text),
                  strtoint(CloseH.Text),
                  strtoint(closem.Text),
                  comautoopenrule.ItemIndex );
  if ret<0 then
    showmessage('设置失败')
  else
    showMessage('设置成功');


end;

procedure TForm1.Button23Click(Sender: TObject);
var
  i:integer;
begin
    ret:= NetSetLightness(strtoint(edit3.text),
                  pchar(edit1.text),
                  comlightness.ItemIndex +1);
  if ret<0 then
    showmessage('设置失败')
  else
    showMessage('设置成功');

end;

procedure TForm1.Button24Click(Sender: TObject);
var
  ret:integer;
begin
    ret:=NetIPSet(strtoint(edit3.text),
                  pchar(edit1.text),
                  strtoint(newlocalport.Text),
                  Pchar(newip.Text));
  if ret<0 then
  begin
    showmessage('设置失败');
    exit;
  end
  else
  begin
    showmessage('设置成功');
    edit1.text:=newip.text;
    edit3.text:=newlocalport.text;
//    winexec('arp /d',SW_NORMAL);
  end;
end;

procedure TForm1.Button25Click(Sender: TObject);
var
  ret,CheckValue:integer;
begin
  if comCheckTime.ItemIndex =0 then
    checkValue:=strtoint(txtcheckTime.Text)
  else
    checkValue:= 0 - strtoint(txtcheckTime.Text );

    ret:= NetCheckDateTime(strtoint(edit3.text),
                  pchar(edit1.text),
                  CheckValue);
  if ret<0 then
    showmessage('校时失败')
  else
    showMessage('校时成功');

end;

procedure TForm1.Button26Click(Sender: TObject);
begin
  ClearMemory;
end;

procedure TForm1.Button16Click(Sender: TObject);
var
  showmode,showspeed,showtime : integer;
  i:integer;
  yy,mm,dd,hh,mi,ss,ms:word;
  str:string;
begin

showmode:=strtoint(edit7.Text );
showspeed:=strtoint(edit8.Text );
showtime:=strtoint(edit9.Text );
//for i:=0 to 8000 do databuff[i]:=17;
//for i:=0 to 7 do
decodetime(now,hh,mi,ss,ms);
str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
str:=str + '开始插播';
ret:=ComSendpicDataOnce(strtoint(Comport.text),StrToint(bps.text),strtoint(address.text),
       image1.canvas.handle,strtoint(scrw.text),strtoint(scrH.text),comcolor.ItemIndex +1,2,showtime);

form1.Caption :=inttostr(ret);
decodetime(now,hh,mi,ss,ms);
str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
if ret<0 then
begin
  listbox2.Items.Add(str);
  str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox2.Items.Add(str + '插播失败');
  listbox2.Items.Add(' ');
end
else
begin
  listbox1.Items.Add(str );
  str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox1.Items.Add(str + '插播成功');
  listbox2.Items.Add(' ');
end;
  edit13.Text :=inttostr(listbox1.Items.Count div 3 );
  edit14.Text :=inttostr(listbox2.Items.Count div 3);
  image2.Picture.Assign(nil);

  bitblt(image2.Canvas.Handle ,0,0,image1.width,image1.height,gdc ,0 ,0,srccopy);
end;

procedure TForm1.Button27Click(Sender: TObject);
begin
  if button27.Caption ='串口插播测试' then
  begin
      button27.Caption :='串口停止测试';
      InProc:=false;
      SetTimer(handle ,   2,   strtoint(edit12.Text) * 1000,   @Tform1.HearBeatProc);   //创建一个定时器
  end
  else
  begin
      button27.Caption :='串口插播测试';
      KillTimer(handle, 2);//  TimerID);
      InProc:=True;
  end;
  timer1.Enabled :=not timer1.Enabled ;

end;

procedure TForm1.Button28Click(Sender: TObject);
var
  i:integer;
  ret,bufflen:integer;
  sendbuff:array[0..1024] of byte;
begin
for i:=0 to 5 do
begin
  button9click(nil);
  ret:=ComSendpicData(strtoint(Comport.text),StrToint(bps.text),strtoint(address.text),strtoint(edit6.Text )+ i,image1.canvas.handle,strtoint(scrw.text),strtoint(scrH.text),comcolor.ItemIndex +1,2);
end;
 bufflen:=11;
 sendbuff[0]:=0;
 sendbuff[1]:=strtoint(edit6.Text );
 sendbuff[2]:=strtoint(edit7.Text );
 sendbuff[3]:=strtoint(edit8.text);
 sendbuff[4]:=strtoint(edit9.Text );
 sendbuff[5]:=6;
 sendbuff[6]:=sendbuff[1];
 sendbuff[7]:=sendbuff[2]+1;
 sendbuff[8]:=sendbuff[3];
 sendbuff[9]:=sendbuff[4];
 sendbuff[10]:=sendbuff[5];

 sleep(100);
 ret:=ComSendList(strtoint(ComPort.Text ),strtoint(Bps.Text ),strtoint(address.text),sendbuff,11);
 form1.Caption :=inttostr(ret);

end;

procedure TForm1.Button29Click(Sender: TObject);
var
  i:integer;
  ret,bufflen:integer;
  sendbuff:array[0..12] of byte;
begin
for i:=0 to 5 do
begin
  button9click(nil);
  ret:=netSendpicData(strtoint(edit3.text),
                    pchar(edit1.text),
                    strtoint(edit6.Text ) + i,
                    GDC,
                    strtoint(scrw.text),
                    strtoint(scrH.text),
                    comcolor.ItemIndex +1,
                    2);
 form1.Caption :=inttostr(ret);
end;
 bufflen:=11;
 sendbuff[0]:=0;
 sendbuff[1]:=strtoint(edit6.Text );
 sendbuff[2]:=strtoint(edit7.Text );
 sendbuff[3]:=strtoint(edit8.text);
 sendbuff[4]:=strtoint(edit9.Text );
 sendbuff[5]:=6;
 sendbuff[6]:=sendbuff[1];
 sendbuff[7]:=sendbuff[2]+1;
 sendbuff[8]:=sendbuff[3];
 sendbuff[9]:=sendbuff[4];
 sendbuff[10]:=sendbuff[5];
 sleep(1000);
 ret:=NetListSend(strtoint(edit3.text),pchar(edit1.text),sendbuff,11);
 form1.Caption :=inttostr(ret);

end;

procedure TForm1.Edit15Change(Sender: TObject);
var
  i:integer;
begin
  setlength(sendtimes,strtoint(edit15.text) +1);
end;

procedure TForm1.Button31Click(Sender: TObject);
var
  showmode,showspeed,showtime : integer;
  i:integer;
  yy,mm,dd,hh,mi,ss,ms:word;
  str1,str:string;
  p:pchar;

begin
setlength(sendtimes,strtoint(edit15.text) +3);
netsetsenddatanum(strtoint(edit15.Text));
showmode:=strtoint(edit7.Text );
showspeed:=strtoint(edit8.Text );
showtime:=strtoint(edit9.Text );
//for i:=0 to 8000 do databuff[i]:=17;
//for i:=0 to 7 do
decodetime(now,hh,mi,ss,ms);
str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
str:=str + '开始插播';
//ret:=netSendPicOnce(strtoint(edit3.text),pchar(edit1.text),
//      GDC, //image2.Canvas.Handle ,
//      strtoint(ScrW.text),strtoint(scrH.Text),2,2,showmode,showspeed,showtime);
ret:=netSendPicOnceNew9G(strtoint(edit3.text),pchar(edit1.text),
      GDC, //image2.Canvas.Handle ,
      strtoint(edit16.text),strtoint(PIcx.text),strtoint(PicY.text), strtoint(PicW.text),
      strtoint(PicH.Text),strtoint(edit18.Text ),strtoint(edit19.Text),
      comcolor.ItemIndex +1,2,showmode,showspeed,showtime,strtoint(edit17.Text));
netgeterrmsg(p);
//form1.Caption :=inttostr(ret);
decodetime(now,hh,mi,ss,ms);
if ret<0 then
begin
  listbox2.Items.Add(str);
  str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox2.Items.Add(str + '插播失败:' + inttostr(ret));
  listbox2.Items.Add(p);
end
else
begin
  listbox1.Items.Add(str);
  str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox1.Items.Add(str + '插播成功：' + inttostr(ret));
  listbox1.Items.Add(p);
  sendtimes[ret]:=sendtimes[ret]+1;
end;
edit13.Text :=inttostr(listbox1.Items.Count div 3 );
edit14.Text :=inttostr(listbox2.Items.Count div 3);
image2.Picture.Assign(nil);
str1:='重发次数：';
for i:=1 to high(sendtimes) do
  str1:=str1+ '   '  + inttostr(sendtimes[i]);
//  str1:=str1+ '   ' + inttostr(i) + '次:' + inttostr(sendtimes[i]);
label41.Caption :=str1;
  bitblt(image2.Canvas.Handle ,0,0,image1.width,image1.height,gdc ,0 ,0,srccopy);
end;

procedure TForm1.Button32Click(Sender: TObject);
var
  showmode,showspeed,showtime : integer;
  i:integer;
  yy,mm,dd,hh,mi,ss,ms:word;
  str1,str:string;
  p:pchar;

begin
setlength(sendtimes,strtoint(edit15.text) +3);
netsetsenddatanum(strtoint(edit15.Text));
showmode:=strtoint(edit7.Text );
showspeed:=strtoint(edit8.Text );
showtime:=strtoint(edit9.Text );
//for i:=0 to 8000 do databuff[i]:=17;
//for i:=0 to 7 do
decodetime(now,hh,mi,ss,ms);
str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
str:=str + '开始插播';
//ret:=netSendPicOnce(strtoint(edit3.text),pchar(edit1.text),
//      GDC, //image2.Canvas.Handle ,
//      strtoint(ScrW.text),strtoint(scrH.Text),2,2,showmode,showspeed,showtime);
ret:=netSendPicOnceTW04(strtoint(edit3.text),pchar(edit1.text),
      strtoint(edit16.Text),GDC, //image2.Canvas.Handle ,
      strtoint(PIcx.text),strtoint(PicY.text), strtoint(PicW.text),strtoint(PicH.Text),comcolor.ItemIndex +1,2,showmode,showspeed,showtime,strtoint(edit17.Text));
netgeterrmsg(p);
//form1.Caption :=inttostr(ret);
decodetime(now,hh,mi,ss,ms);
if ret<0 then
begin
  listbox2.Items.Add(str);
  str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox2.Items.Add(str + '插播失败:' + inttostr(ret));
  listbox2.Items.Add(p);
end
else
begin
  listbox1.Items.Add(str);
  str:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox1.Items.Add(str + '插播成功：' + inttostr(ret));
  listbox1.Items.Add(p);
  sendtimes[ret]:=sendtimes[ret]+1;
end;
edit13.Text :=inttostr(listbox1.Items.Count div 3 );
edit14.Text :=inttostr(listbox2.Items.Count div 3);
image2.Picture.Assign(nil);
str1:='重发次数：';
for i:=1 to high(sendtimes) do
  str1:=str1+ '   '  + inttostr(sendtimes[i]);
//  str1:=str1+ '   ' + inttostr(i) + '次:' + inttostr(sendtimes[i]);
label41.Caption :=str1;
  bitblt(image2.Canvas.Handle ,0,0,image1.width,image1.height,gdc ,0 ,0,srccopy);
end;

end.
