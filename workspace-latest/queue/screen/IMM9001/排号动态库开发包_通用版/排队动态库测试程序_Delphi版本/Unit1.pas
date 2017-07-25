unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;//, senddata;

type
  TForm1 = class(TForm)
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    Edit4: TEdit;
    Edit5: TEdit;
    Edit6: TEdit;
    Edit7: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Edit8: TEdit;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Edit9: TEdit;
    Edit10: TEdit;
    Label9: TLabel;
    Label10: TLabel;
    Button6: TButton;
    Edit11: TEdit;
    Label11: TLabel;
    Label12: TLabel;
    ComboBox1: TComboBox;
    Button7: TButton;
    Timer1: TTimer;
    Label13: TLabel;
    Edit12: TEdit;
    ListBox1: TListBox;
    ListBox2: TListBox;
    Button8: TButton;
    Edit14: TEdit;
    Label14: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    Label17: TLabel;
    Button9: TButton;
    Edit13: TEdit;
    Button10: TButton;
    Label18: TLabel;
    Edit15: TEdit;
    Button11: TButton;
    Label19: TLabel;
    Edit16: TEdit;
    Button12: TButton;
    Button13: TButton;
    Button14: TButton;
    Button15: TButton;
    Button16: TButton;
    Button17: TButton;
    Label20: TLabel;
    Label21: TLabel;
    Label22: TLabel;
    Label23: TLabel;
    CheckBox1: TCheckBox;
    Edit17: TEdit;
    Edit18: TEdit;
    Button18: TButton;
    Label24: TLabel;
    Edit19: TEdit;
    Label25: TLabel;
    Button19: TButton;
    Button20: TButton;
    Button21: TButton;
    Button22: TButton;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button7Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Button8Click(Sender: TObject);
    procedure Button9Click(Sender: TObject);
    procedure Button10Click(Sender: TObject);
    procedure Button11Click(Sender: TObject);
    procedure Button12Click(Sender: TObject);
    procedure Button13Click(Sender: TObject);
    procedure Button14Click(Sender: TObject);
    procedure Button15Click(Sender: TObject);
    procedure Button16Click(Sender: TObject);
    procedure Button17Click(Sender: TObject);
    procedure CheckBox1Click(Sender: TObject);
    procedure Button18Click(Sender: TObject);
    procedure Button19Click(Sender: TObject);
    procedure Button20Click(Sender: TObject);
    procedure Button21Click(Sender: TObject);
    procedure Button22Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  mm_handle:thandle;
  count:integer;
  timercount:integer;

implementation
//系统自动根据日期指定图号
  function SendDatafun(comport:integer;bps:longint;address:integer;sendData:pchar;color:integer;showMode,showSpeed,showTime:integer):integer;
    StdCall external '../dlltpzp.DLL';
//自己指定图号显示
  function SendDatagen(comport:integer;bps:longint;address:integer;commbyte:integer;sendData:pchar;color:integer;showMode,showSpeed,showTime:integer):integer;
    StdCall external '../dlltpzp.DLL';
//自己指定图号显示，CK16C专用，增加页面时间参数。
  function SendDatagen16c(comport:integer;bps:longint;address:integer;commbyte:integer;sendData:pchar;color:integer;showMode,showSpeed,showTime,pageTime:integer):integer;
    StdCall external '../dlltpzp.DLL';
//系统自动根据日期指定图号,外部控制串口打开
  function    dataSend(mm_handle:integer;address:integer;sendData:pchar;color,showMode,showSpeed,showTime:integer):integer;
    StdCall external '../dlltpzp.DLL';
//自己指定图号显示,外部控制串口打开
  function    dataSendgen(mm_handle:integer;address:integer;commandbyte:integer;sendData:pchar;color,showMode,showSpeed,showTime:integer):integer;
    StdCall external '../dlltpzp.DLL';
//系统自动根据日期指定图号,外部控制串口打开
  function    dataSendRec(mm_handle:integer;address:integer;sendData:pchar;color,showMode,showSpeed,showTime:integer):integer;
    StdCall external '../dlltpzp.DLL';
//自己指定图号显示,外部控制串口打开
  function    dataSendgenRec(mm_handle:integer;address:integer;commandbyte:integer;sendData:pchar;color,showMode,showSpeed,showTime:integer):integer;
    StdCall external '../dlltpzp.DLL';
//读取串口返回数据
  function   GetRecData(mm_handle:Thandle;var Recbuff:array of byte;recNums,waitSec,WaitSecs:integer):integer;
    StdCall external '../dlltpzp.DLL';
//发送单幅图片
  function    picSend(mm_handle:integer;address:integer;commandbyte:integer;sendData:pchar;color:integer):integer;
    StdCall external '../dlltpzp.DLL';
//发送显示计划
  function    plainSend(mm_handle:integer;address:integer;plainnum:integer;plainData:array of byte):integer;
    StdCall external '../dlltpzp.DLL';
//打开串口
//  function    portOpen(com:string;bps:longint;par:char;dbit,sbit:byte):THandle;
  function    portOpen(com:integer;bps:longint;par:char;dbit,sbit:byte):THandle;
    StdCall external '../dlltpzp.DLL';
//关闭串口
  function   portClose(mm_handle:Thandle):integer;
    StdCall external '../dlltpzp.DLL';
//设置窗口号
  function    setWindowNo(mm_handle:integer;address:integer;cols:integer;color:integer;WinNo:pchar):integer;
    StdCall External '../dlltpzp.Dll';
  function    setWindowNogen(comport,bps:integer;address:integer;cols:integer;color:integer;WinNo:pchar):integer;
    StdCall External '../dlltpzp.Dll';
//颜色设置：0x11:红色，0x22：绿色，0x33：黄色；高位设置字母和数字颜色，后一位设置汉字颜色。如字母显示为红色，其他为黄色则为0x13（即19）.
//动作设置：0：立即显示，1：从右向左，3：从左向右展开，4百叶窗从左向右展开，7：闪烁显示，8：连续左移。
//速度设置：1-16：1最慢，16最快。

function    ComGetCardWidth(ComPort:integer;bps:longint;address:integer):integer;
    StdCall External '../dlltpzp.Dll';
function    SendDataZH(comport:integer;bps:longint;address:integer;sendData:pchar;color,showmode,showspeed,showtime:integer):integer;
    StdCall external '../dlltpzp.DLL';
function    SendDataZHLine(comport:integer;bps:longint;address:integer;sendData:pchar;color:integer):integer;
    StdCall external '../dlltpzp.DLL';
function    SendDataZHScr(comport:integer;bps:longint;address:integer;sendData:pchar;color:integer):integer;
    StdCall external '../dlltpzp.DLL';
function    SendDataOnly(comport:integer;bps:longint;address,commandbyte:integer;sendData:pchar;color:integer):integer;
    StdCall external '../dlltpzp.DLL';
function    ComSendList(comport:integer;bps:longint;address:integer;plainnum:integer;plainData:array of byte):integer;
    StdCall external '../dlltpzp.DLL';
function    ComShut(comport:integer;bps:longint;address,index:integer):integer;
    StdCall external '../dlltpzp.DLL';
function    ComCheckScr(comport:integer;bps:longint;address:integer):integer;
    StdCall external '../dlltpzp.DLL';
function    HandleCheckScr(mm_handle:integer;address:integer):integer;
    StdCall external '../dlltpzp.DLL';
function    geterrmsg():string;
    StdCall external '../dlltpzp.DLL';
function    SetFullFont(value:integer):integer;
    StdCall external '../dlltpzp.DLL';
function    SetWaitTime(waitMilliSecond,waitMilliSeconds:integer):integer;
    StdCall external '../dlltpzp.DLL';
function    SetWaitTime1(value:integer):integer;
    StdCall external '../dlltpzp.DLL';

//获取条屏长度
{$R *.dfm}
//系统自动根据日期指定图号
procedure TForm1.Button1Click(Sender: TObject);
var
  i : integer;
  strtemp : string;
begin
i:=SendDatafun(strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),pchar(edit4.Text),17,strtoint(edit6.Text),strtoint(edit7.Text),strtoint(edit8.Text));
form1.Caption:=inttostr(i);
end;
//自己指定图号显示
procedure TForm1.Button2Click(Sender: TObject);
var
  i : integer;
  strtemp : string;
begin
i:=SendDatagen(strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),strtoint(edit5.Text),pchar(edit4.Text),19,strtoint(edit6.Text),strtoint(edit7.Text),strtoint(edit8.Text));
form1.Caption:=inttostr(i);
end;

procedure TForm1.Button3Click(Sender: TObject);
var
  ret,i : integer;
  strtemp : string;
  Plain:array[0..255] of byte;
begin
i:=SendDatazhLine (strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),pchar(edit4.Text),19);
  form1.Caption:=inttostr(i);
end;
procedure TForm1.Button4Click(Sender: TObject);
begin
  mm_handle:=portOpen( strtoint(edit1.text),strtoint(edit2.Text),'n',8,1);
  form1.Caption:=inttostr(mm_handle);

end;

procedure TForm1.Button5Click(Sender: TObject);
begin
  portclose(mm_handle);

end;

procedure TForm1.Button6Click(Sender: TObject);
var
  ret:integer;
begin
  ret:=setWindowNogen(strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),strtoint(edit9.Text),1,pchar(edit10.Text ));
end;

procedure TForm1.Button7Click(Sender: TObject);
begin

  timer1.Enabled :=not timer1.Enabled ;
  if timer1.Enabled then
  begin
    button7.Caption :='停止测试';
    button9.Enabled :=false;
  end
  else
  begin
    button7.Caption :='开始测试';
    button9.Enabled :=true;
  end;
  timercount:=0;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var
  i,j,k,ret :integer;
  str:string;
  yy,mm,dd,hh,mi,ss,ms:word;
  str1:string;

begin
  timercount:=timercount+1;
  if timercount>strtoint(edit12.Text) then timercount:=1;
  if timercount>1 then exit;
  if count>=100 then count:=0;
  count:=count+1;
  for i:=1 to strtoint(edit11.Text ) do
  begin
    j:=i+ 256 * combobox1.ItemIndex ;
    str:=inttostr(i) + '号屏' + inttostr(count) +edit4.Text ;
decodetime(now,hh,mi,ss,ms);
str1:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
str1:=str1 + '开始发送' + inttostr(i) + '号屏';
if button9.Caption = '停止测试' then
    ret:=SendDatazhline(strtoint(edit1.Text),strtoint(edit2.Text ),j-1+strtoint(edit3.Text ),pchar(str),19)
else
    ret:=SendDatagen(strtoint(edit1.Text),strtoint(edit2.Text ),j-1+strtoint(edit3.Text ),count,pchar(str),19,strtoint(edit6.Text),strtoint(edit7.Text),strtoint(edit8.Text));

    form1.Caption :=inttostr(ret);
decodetime(now,hh,mi,ss,ms);
if ret<0 then
begin
  listbox2.Items.Add(str1);
  str1:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox2.Items.Add(str1 +inttostr(i) + '号屏'+'发送失败' + ':' + inttostr(ret) + '||'+ geterrmsg());
end
else
begin
  listbox1.Items.Add(str1);
  str1:=inttostr(hh)+':'+inttostr(mi)+ ':' + inttostr(ss) + ':' +inttostr(ms) + ' ';
  listbox1.Items.Add(str1 + inttostr(i) + '号屏' + '发送成功');
end;
    edit13.Text :=inttostr(listbox1.Count DIV 2);
    edit14.Text :=inttostr(listbox2.Count div 2);
  end;

end;

procedure TForm1.Button8Click(Sender: TObject);
begin
  listbox1.Clear;
  listbox2.Clear;
  edit13.Text :=inttostr(listbox1.Count);
  edit14.Text :=INTTOSTR(LISTBOX2.Count );
end;

procedure TForm1.Button9Click(Sender: TObject);
begin

  timer1.Enabled :=not timer1.Enabled ;
  if timer1.Enabled then
  begin
    button9.Caption :='停止测试';
    button7.Enabled :=false;
  end
  else
  begin
    button9.Caption :='开始测试';
    button7.Enabled :=true;
  end;
  timercount:=0;

end;

procedure TForm1.Button10Click(Sender: TObject);
var
  i : integer;
  strtemp : string;
begin
i:=SendDatagen16c(strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),strtoint(edit5.Text),pchar(edit4.Text),19,strtoint(edit6.Text),strtoint(edit7.Text),strtoint(edit8.Text),strtoint(edit15.Text));
form1.Caption:=inttostr(i);
end;

procedure TForm1.Button11Click(Sender: TObject);
var ret:integer;
begin
   ret:=ComGetCardWidth(strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text));
     edit16.Text :=inttostr(ret)
end;

procedure TForm1.Button12Click(Sender: TObject);
var
  ret,i : integer;
  strtemp : string;
  Plain:array[0..255] of byte;
begin
i:=SendDatazhScr (strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),pchar(edit4.Text),19);
  form1.Caption:=inttostr(i);
end;

procedure TForm1.Button13Click(Sender: TObject);
var
  ret,i : integer;
  strtemp : string;
  Plain:array[0..255] of byte;
begin
i:=SendDataOnly (strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),3,pchar(edit4.Text),19);
  form1.Caption:=inttostr(i);
end;

procedure TForm1.Button14Click(Sender: TObject);
var
  ret,i : integer;
  strtemp : string;
  Plain:array[0..255] of byte;
begin
i:=comShut(strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),1);
  form1.Caption:=inttostr(i);
end;

procedure TForm1.Button15Click(Sender: TObject);
var
  ret,i : integer;
  strtemp : string;
  Plain:array[0..255] of byte;
begin
i:=comShut(strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),0);
  form1.Caption:=inttostr(i);
end;

procedure TForm1.Button16Click(Sender: TObject);
var
  ret,i : integer;
  strtemp : string;
  Plain:array of byte;
begin
  setlength(plain,12);
 for i:=0 to 2 do
 begin

  ret:=SendDataOnly (strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),strtoint(edit5.Text )+i,pchar(inttostr(i) + pchar(edit4.Text)),19);
  form1.Caption:=inttostr(ret);
  plain[i*4]:=strtoint(edit5.Text ) +i;
  plain[i*4+1]:=strtoint(edit6.Text );
  plain[i*4+2]:=strtoint(edit7.Text );
  plain[i*4+3]:=strtoint(edit8.Text );
 end;
  ret:=comsendlist(strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),3,plain);
  form1.caption:=inttostr(ret);
end;

procedure TForm1.Button17Click(Sender: TObject);
var
  ret:integer;
begin
  ret:=comcheckscr(strtoint(edit1.Text),strtoint(edit2.Text),strtoint(edit3.Text));
  case ret of
  0: showmessage('显示屏通讯正常');
  -1: showmessage('打开串口失败');
  -2: showmessage('显示屏无应答');
  else
      showmessage('其他错误');
  end;
end;

procedure TForm1.CheckBox1Click(Sender: TObject);
begin
  if checkbox1.Checked then
    setfullfont(1)
  else
    setfullfont(0);
end;

procedure TForm1.Button18Click(Sender: TObject);
begin
  setwaittime(strtoint(edit18.Text),strtoint(edit17.Text));
end;

procedure TForm1.Button19Click(Sender: TObject);
begin
  setwaittime1(strtoint(edit19.Text ));
end;

procedure TForm1.Button20Click(Sender: TObject);
var
  ret1,ret2,ret3 : integer;
  recbuff: array[0..255] of byte;
  strtemp : string;
  mm_handle:integer;
begin
  mm_handle:=portOpen(strtoint(edit1.text),strtoint(edit2.Text),'n',8,1);
  ret1:=dataSendRec(mm_handle,strtoint(edit3.Text),pchar(edit4.Text),17,strtoint(edit6.Text),strtoint(edit7.Text),strtoint(edit8.Text));
  ret2:=getrecdata(mm_handle,recbuff,1,20,1000);
  portclose(mm_handle);
  form1.Caption:=inttostr(ret1) + '  ' + inttostr(ret2) + '  ' + inttostr(recbuff[0]) + '  ' + inttostr(recbuff[1]);
end;

procedure TForm1.Button21Click(Sender: TObject);
var
  ret1,ret2,ret3 : integer;
  recbuff: array[0..255] of byte;
  strtemp : string;
  mm_handle:integer;
begin
  mm_handle:=portOpen(strtoint(edit1.text),strtoint(edit2.Text),'n',8,1);
  ret1:=dataSendgenRec(mm_handle,strtoint(edit3.Text),strtoint(edit5.Text ),pchar(edit4.Text),17,strtoint(edit6.Text),strtoint(edit7.Text),strtoint(edit8.Text));
  ret2:=getrecdata(mm_handle,recbuff,1,20,1000);
  form1.Caption:=inttostr(ret1) + '  ' + inttostr(ret2) + '  ' + inttostr(recbuff[0]) + '  ' + inttostr(recbuff[1]);
  portclose(mm_handle);
end;

procedure TForm1.Button22Click(Sender: TObject);
var
  ret,i : integer;
  strtemp : string;
  Plain:array[0..255] of byte;
begin
i:=SendDataZH(strtoint(edit1.Text),strtoint(edit2.Text ),strtoint(edit3.Text),pchar(edit4.Text),19,strtoint(edit6.Text),strtoint(edit7.Text),strtoint(edit8.Text));
  form1.Caption:=inttostr(i);
end;

end.
