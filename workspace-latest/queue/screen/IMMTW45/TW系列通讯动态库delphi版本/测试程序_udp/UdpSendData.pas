unit UdpSendData;

interface
uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls,
    UdpSockteUnit ,PicToData;

  function NetShut(LocalPort:integer;DescIP:Pchar;DescPort,index:integer):integer;stdcall;
  function NetGetData(Sockid:LongInt;var recbuff:array of byte):integer;
  function NetPicSendData(LocalPort:integer;DescIP:Pchar;DescPort,CommandByte:integer;var buff:array of byte;Width,Height,PColorSign,SendMode:integer):integer;stdcall;
  function HandShake(SockId:Longint;DescIp:Pchar;DescPort,CommandByte,ZNum,PCode:integer):integer;
  function HandShakeOnce(SockId:Longint;DescIp:Pchar;DescPort,CommandByte,ZNum,PCode,showmode,showspeed,showtime:integer):integer;
  function NetListSend(LocalPort:integer;DescIP:Pchar;DescPort:integer;var buff:array of byte;bufflen:integer):integer;stdcall;
  function NetPicSendOnce(LocalPort:integer;DescIP:Pchar;DescPort:integer;var buff:array of byte;Width,Height,PColorSign,SendMode,showmode,showspeed,showtime:integer):integer;stdcall;
  function NetSendPicOnce(LocalPort:integer;DescIP:Pchar;DescPort:integer;GDC:hdc;Width,Height,PColorSign,SendMode,showmode,showspeed,showtime:integer):integer;stdcall;
  function NetSendPicData(LocalPort:integer;DescIP:Pchar;DescPort,CommandByte:integer;GDC:hdc;Width,Height,PColorSign,SendMode:integer):integer;stdcall;

implementation
{
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
}
function NetSendPicOnce(LocalPort:integer;DescIP:Pchar;DescPort:integer;GDC:hdc;Width,Height,PColorSign,SendMode,showmode,showspeed,showtime:integer):integer;
var
  databuff:array of byte;
  datalen:integer;
  ret:integer;
begin
  datalen:=((width-1) div 8 +1) * height * Pcolorsign;
  setlength(databuff,datalen);
  result:=GetArrayFromPic(GDC,databuff,width,height,PColorSign);
  if result<0 then exit;
  result:=NetPicSendOnce(LocalPort,DescIP,DescPort,databuff,Width,Height,PColorSign,SendMode,showmode,showspeed,showtime);
end;

function NetSendPicData(LocalPort:integer;DescIP:Pchar;DescPort,CommandByte:integer;GDC:hdc;Width,Height,PColorSign,SendMode:integer):integer;
var
  databuff:array of byte;
  datalen:integer;
  ret:integer;
begin
  datalen:=((width-1) div 8 +1) * height * Pcolorsign;
  setlength(databuff,datalen);
  result:=GetArrayFromPic(GDC,databuff,width,height,PColorSign);
  if result<0 then exit;
  result:=NetPicSendData(LocalPort,DescIP,DescPort,CommandByte,databuff,Width,Height,PColorSign,SendMode);

end;
function NetListSend(LocalPort:integer;DescIP:Pchar;DescPort:integer;var buff:array of byte;bufflen:integer):integer;
var
  ret,i,j,sendbufflen : integer;
  sockid:longint;
  sendbuff:array[0..4096] of byte;
  recbuff:array[0..4096] of byte;

begin
  sockid:=socketopen(localport);
  if sockid<0 then
  begin
    result:=-1;
    exit;
  end;
  try
    ret:=handShake(SockId,DescIp,DescPort,2,1,0);
    result:=ret;
    begin
     if result>=0 then
     begin
      sendbufflen:=bufflen+4;

      sendbuff[0]:=0;
      sendbuff[1]:=1;
      sendbuff[2]:=bufflen div 256;
      sendbuff[3]:=bufflen mod 256;
      for j:=0 to bufflen -1 do
          sendbuff[j+4]:=buff[j];
      ret:=senddata(sockid,descip,descport,sendbuff,sendbufflen);
      if ret<0 then
        result:=-2  //发送失败
      else
      begin
        ret:=NetGetdata(sockid,recbuff);
        if ret<1 then
          result:=-3  //没有返回
        else
        begin
          if ret=19 then
            result:=0  //成功
          else
            result:=-4; //返回错误
        end;
      end;

     end;
    end;
  finally
    socketclose( sockid);
  end;

end;
function NetPicSendOnce(LocalPort:integer;DescIP:Pchar;DescPort:integer;var buff:array of byte;Width,Height,PColorSign,SendMode,showmode,showspeed,showtime:integer):integer;stdcall;
var
  ret,i,j,SendSize :integer;
  sockid:longint;
  sendbuff:array[0..4096] of byte;
  recbuff:array[0..256] of byte;
  sendbufflen:integer;
  k1,k2,k3,k4 : integer;
begin
  if sendmode=1 then
    sendsize:=264 //图文01、02
  else
    sendsize:=528; //图文03，04数据包长度

//  k1 := (height * PColorSign * ((width -1) div 8 +1)) mod sendsize;
  k2 := ((height * PColorSign * ((width-1) div 8 +1))-1) div sendsize  ;
  k3 := (height * PColorSign * ((width-1) div 8 +1)) mod 256 ;
  k4 := (height * PColorSign * ((width-1) div 8 +1)) div 256 ;

  sockid:=SocketOpen(LocalPort);
  if Sockid<0 then
  begin
    result :=-1;  //打开网络失败
    exit;
  end;
  try
    ret:=handShakeOnce(SockId,DescIp,DescPort,17,k2+1,0,showmode,showspeed,showtime);
    result:=ret;
    for i:=0 to k2 do
    begin
     if result>=0 then
     begin
      if i=0 then
      begin
        sendbufflen:=sendsize+8;
        sendbuff[sendsize+4]:=(width-1) div 8 +1;
        sendbuff[sendsize+5]:=(height-1) div 8 +1;
        sendbuff[sendsize+6]:=k4;
        sendbuff[sendsize+7]:=k3;
      end
      else
        sendbufflen:=sendsize+8;

 //     sendbufflen:=sendsize;

      sendbuff[0]:=(i+1) div 256;
      sendbuff[1]:=(i+1) mod 256;
      sendbuff[2]:=sendsize div 256;
      sendbuff[3]:=sendsize mod 256;
      for j:=0 to sendsize -1 do
          sendbuff[j+4]:=buff[j + sendsize * i];
      ret:=senddata(sockid,descip,descport,sendbuff,sendbufflen);
      if ret<0 then
        result:=-2  //发送失败
      else
      begin
        ret:=NetGetdata(sockid,recbuff);
        if ret<1 then
          result:=-3  //没有返回
        else
        begin
          if ret=19 then
            result:=0  //成功
          else
            result:=-4; //返回错误
        end;
      end;

     end;
    end;
  finally
    socketclose( sockid);
  end;
end;

function NetPicSendData(LocalPort:integer;DescIP:Pchar;DescPort,CommandByte:integer;var buff:array of byte;Width,Height,PColorSign,SendMode:integer):integer;
var
  ret,i,j,SendSize :integer;
  sockid:longint;
  sendbuff:array[0..4096] of byte;
  recbuff:array[0..256] of byte;
  sendbufflen:integer;
  k1,k2,k3,k4 : integer;
begin
  if sendmode=1 then
    sendsize:=264 //图文01、02
  else
    sendsize:=528; //图文03，04数据包长度

//  k1 := (height * PColorSign * ((width -1) div 8 +1)) mod sendsize;
  k2 := ((height * PColorSign * ((width-1) div 8 +1))-1) div sendsize  ;
  k3 := (height * PColorSign * ((width-1) div 8 +1)) mod 256 ;
  k4 := (height * PColorSign * ((width-1) div 8 +1)) div 256 ;
{
  For i := 0 To (height * PColorSign) - 1 do
  begin
    For j := 0 To (width-1) div 8  do
    begin
      k1 := k1 + 1;
      If k1 >= sendSize Then
      begin
        k1 := 0;
        k2 := k2 + 1;
      End;
      k3 := k3 + 1;
      If k3 >= 256 Then
      begin
        k4 := k4 + 1;
        k3 := 0;
      End ;
    end;
  end;
  If k1 = 0 Then k2 := k2 - 1;
 }

  sockid:=SocketOpen(LocalPort);
  if Sockid<0 then
  begin
    result :=-1;  //打开网络失败
    exit;
  end;
  try
    ret:=handShake(SockId,DescIp,DescPort,CommandByte+16,k2+1,0);
    result:=ret;
    for i:=0 to k2 do
    begin
     if result>=0 then
     begin
      if i=0 then
      begin
        sendbufflen:=sendsize+8;
        sendbuff[sendsize+4]:=(width-1) div 8 +1;
        sendbuff[sendsize+5]:=(height-1) div 8 +1;
        sendbuff[sendsize+6]:=k4;
        sendbuff[sendsize+7]:=k3;
      end
      else
        sendbufflen:=sendsize+4;

      sendbuff[0]:=(i+1) div 256;
      sendbuff[1]:=(i+1) mod 256;
      sendbuff[2]:=sendsize div 256;
      sendbuff[3]:=sendsize mod 256;
      for j:=0 to sendsize -1 do
          sendbuff[j+4]:=buff[j + sendsize * i];
      ret:=senddata(sockid,descip,descport,sendbuff,sendbufflen);
      if ret<0 then
        result:=-2  //发送失败
      else
      begin
        ret:=NetGetdata(sockid,recbuff);
        if ret<1 then
          result:=-3  //没有返回
        else
        begin
          if ret=19 then
            result:=0  //成功
          else
            result:=-4; //返回错误
        end;
      end;

     end;
    end;
  finally
    socketclose( sockid);
  end;


end;


function NetShut(LocalPort:integer;DescIP:Pchar;DescPort,index:integer):integer;
var
  sockid : LongInt;
  ret,i : integer;
  buff: array[0..4096] of byte;
  bufflen:integer;
begin
  bufflen:=19;
  for i:=0 to 7 do
    buff[i]:=0;
  buff[3]:=index * 64;
  for i:=8 to 18 do
    buff[i]:=170;
  sockid:=SocketOpen(LocalPort);
  if Sockid<0 then
  begin
    result :=-1;  //打开网络失败
    exit;
  end;
  try
    ret:=senddata(  sockid,descip,descport,buff,bufflen);
    if ret<0 then
      result:=-2  //送失败
    else
    begin
      ret:=Netgetdata(sockid,buff);
      if ret<1 then
        result:=-3  //没有返回
      else
      begin
        if ret=19 then
          result:=0  //成功
        else
          result:=-4; //返回错误
      end;
    end;

  finally
    socketclose( sockid);
  end;
end;

function NetGetData(Sockid:LongInt;var recbuff:array of byte):integer;
var
  i,ret:integer;
begin
  result :=-1;//没有返回
  for i:=0 to 20 do
    begin
      if result>0 then exit;
      sleep(50);
      ret:=readdata(sockid,recbuff);
      if ret>0 then
      begin
        result:=ret  //成功
      end;
   end;
end;
function HandShake(SockId:Longint;DescIp:Pchar;DescPort,CommandByte,ZNum,PCode:integer):integer;
var
  i,ret:integer;
  sendbuff:array[0..255] of byte;
  buff:array[0..255] of byte;
  sendbufflen : integer;
begin
  for i:=0 to 1 do  sendbuff[i]:=0;
  sendbuff[2]:=commandbyte;
  for i:=3 to 14 do sendbuff[i]:=170;
  sendbuff[15]:=ZNum div 256;
  sendbuff[16]:=znum mod 256;
  sendbuff[17]:=0;
  sendbuff[18]:=0;
  sendbufflen:=19;
      ret:=senddata(sockid,descip,descport,sendbuff,sendbufflen);
      if ret<0 then
        result:=-2  //发送失败
      else
      begin
        ret:=Netgetdata(sockid,buff);
        if ret<1 then
          result:=-3  //没有返回
        else
        begin
          if ret=19 then
            result:=0  //成功
          else
            result:=-4; //返回错误
        end;
      end;

end;
function HandShakeOnce(SockId:Longint;DescIp:Pchar;DescPort,CommandByte,ZNum,PCode,showmode,showspeed,showtime:integer):integer;
var
  i,ret:integer;
  sendbuff:array[0..255] of byte;
  buff:array[0..255] of byte;
  sendbufflen : integer;
begin
  for i:=0 to 1 do  sendbuff[i]:=0;
  sendbuff[2]:=commandbyte;
  sendbuff[3]:=1;
  sendbuff[4]:=1;
  sendbuff[5]:=showmode;
  sendbuff[6]:=showspeed;
  sendbuff[7]:=showtime;
  for i:=8 to 14 do sendbuff[i]:=170;
  sendbuff[15]:=ZNum div 256;
  sendbuff[16]:=znum mod 256;
  sendbuff[17]:=0;
  sendbuff[18]:=0;
  sendbufflen:=19;
      ret:=senddata(sockid,descip,descport,sendbuff,sendbufflen);
      if ret<0 then
        result:=-2  //发送失败
      else
      begin
        ret:=Netgetdata(sockid,buff);
        if ret<1 then
          result:=-3  //没有返回
        else
        begin
          if ret=19 then
            result:=0  //成功
          else
            result:=-4; //返回错误
        end;
      end;

end;

end.
