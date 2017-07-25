unit ComSendData;
interface

uses
  Windows,
  SysUtils,
  Messages,
  Dialogs,
  Graphics,
  Classes,
  inifiles,
  GenComm,
  PicToData;

type sendarray = array of byte;
  var
      OpenFlag:   boolean;             //打开标志
      sendflag:   boolean;
      m_handle     :   THandle;             //串口句柄
      recbuff   : array[0..255] of byte;
      buff:       array[0..64,0..64] of byte;
      currx,curry : integer;
      commandtype : integer;
      inifile : Tinifile;


  function GetBcc(buff:array of byte;start,stop:integer):byte;
  function ComShut(ComPort:integer;bps:longint;address,index:integer):integer;stdcall;

  function ComHandShake(mm_handle:integer;address,CommandByte,Znum:integer):integer;
  function ComPicSendData(mm_handle,address,CommandByte:integer;var buff:array of byte;Width,Height,PColorSign,SendMode:integer):integer;stdcall;
//  function ComHandShakeOnce(mm_handle:integer;address,CommandByte,ZNum,PCode,showspeed,showtime:integer):integer;
  function ComListSend(mm_handle,address:integer;var buff:array of byte;bufflen:integer):integer;stdcall;
  function ComSendList(ComPort:integer;bps:longint;address:integer;var buff:array of byte;bufflen:integer):integer;stdcall;
//  function ComPicSendOnce(mm_handle,address:integer;var buff:array of byte;Width,Height,PColorSign,SendMode,showmode,showspeed,showtime:integer):integer;stdcall;
//  function ComSendPicOnce(ComPort:integer;bps:longint;address:integer;GDC:hdc;Width,Height,PColorSign,SendMode,showspeed,showtime:integer):integer;stdcall;
  function ComSendPicData(ComPort:integer;bps:longint;address,CommandByte:integer;GDC:hdc;Width,Height,PColorSign,SendMode:integer):integer;stdcall;

implementation
  {$H+}
function ComSendList(ComPort:integer;bps:longint;address:integer;var buff:array of byte;bufflen:integer):integer;
var
  mm_handle:Thandle;
  i,ret:integer;
begin
  try
    mm_handle:=portopen('Com' + inttostr(ComPort) , bps , 'n',8,1);
    if mm_handle<=0 then
      result:=-1
    else
    begin
      ret:=ComListSend(mm_handle,address,buff,bufflen);
      result:=ret  ;
    end;
  finally
    portclose(mm_handle);
  end;
end;

function ComListSend(mm_handle,address:integer;var buff:array of byte;bufflen:integer):integer;
var
  i,j,ret:integer;
  sendbuff : array of byte;
  recbuff:array of byte;
  sendlength:integer;
begin
  setlength(sendbuff,4096);
  setlength(recbuff,256);
  try
    ret:=ComHandShake(mm_handle,address,2,1);
    if ret<0 then
      result :=-1
    else
    begin
      sendbuff[0]:=0;
      sendbuff[1]:=1;
      sendbuff[2]:=bufflen div 256;
      sendbuff[3]:=bufflen mod 256;
      for i:=0 to bufflen-1 do
        sendbuff[i+4]:=buff[i];
      sendbuff[bufflen+4]:= getbcc(sendbuff,0,bufflen+3);
      sendlength:=bufflen+5;
      send(mm_handle,sendbuff,sendlength);
      ret:=gets(mm_handle,recbuff);
      if ((ret<>1) or (recbuff[0]<>sendbuff[bufflen+4])) then
        result:=-2
      else
        result:=0;
    end;
  except
    result:=-3;
  end;
end;
function ComSendPicData(ComPort:integer;bps:longint;address,CommandByte:integer;GDC:hdc;Width,Height,PColorSign,SendMode:integer):integer;stdcall;
var
  mm_handle:Thandle;
  i,ret:integer;
  sendbuff:array of byte;
begin
  ret:=((width-1) div 8 +1) * height * PcolorSign;
  setlength(sendbuff,ret);
  try
    mm_handle:=portopen('Com' + inttostr(ComPort) , bps , 'n',8,1);
    if mm_handle<=0 then
      result:=-1
    else
    begin
      ret:=GetArrayFromPic(GDC,sendbuff,width,Height,PcolorSign);
      ret:=ComPicSendData(mm_handle,address,Commandbyte,sendbuff,Width,Height,PColorSign,SendMode);
      result:=ret  ;
    end;
  finally
    portclose(mm_handle);
  end;
end;

function ComPicSendData(mm_handle,address,CommandByte:integer;var buff:array of byte;Width,Height,PColorSign,SendMode:integer):integer;stdcall;
var
  i,j,k,ret:integer;
  sendbuff:array of byte;
  recbuff:array of byte;
  sendlength:integer;
  sendsize,k2,k3,k4:integer;
begin
  setlength(sendbuff,4096);
  setlength(recbuff,256);
  if sendmode=1 then
    sendsize:=264 //图文01、02
  else
    sendsize:=528; //图文03，04数据包长度

//  k1 := (height * PColorSign * ((width -1) div 8 +1)) mod sendsize;
  k2 := ((height * PColorSign * ((width-1) div 8 +1))-1) div sendsize  ;
  k3 := (height * PColorSign * ((width-1) div 8 +1)) mod 256 ;
  k4 := (height * PColorSign * ((width-1) div 8 +1)) div 256 ;

  try
    ret:=comHandShake(mm_handle,address,CommandByte+16,k2+1);
    if ret<0 then
      result:=-1
    else
    begin
        result :=0;
        for i:=0 to k2 do
        if result>=0 then
        begin
          sendlength:=sendsize+5;
          sendbuff[0]:=(i+1) div 256;
          sendbuff[1]:=(i+1) mod 256;
          sendbuff[2]:=sendsize div 256;
          sendbuff[3]:=sendsize mod 256;
          for j:=0 to sendsize -1 do
            sendbuff[j+4]:=buff[j + sendsize * i];
          sendbuff[sendsize+4]:=getbcc(sendbuff,0,sendsize+3);
          send(mm_handle,sendbuff,sendlength);
          ret:=gets(mm_handle,recbuff);
          if ((ret<>1) or (recbuff[0]<>sendbuff[sendsize+4])) then
            result:=-3;
        end;
    end;
  except
    result:=-4;
  end;

end;
function GetBcc(buff:array of byte;start,stop:integer):byte;
var
  i,ret:integer;
begin
  ret:=0;
  for i:=start to stop do
  begin
    ret:=ret xor buff[i];
    ret:=ret mod 256;
  end;
  result:=ret;
end;
function ComHandShake(mm_handle:integer;address,CommandByte,Znum:integer):integer;
var
  ret,i,j : integer;
  sendbuff,recbuff : array of byte;
begin
  setlength(sendbuff,4096);
  setlength(recbuff,256);
  sendbuff[0]:=170;
  sendbuff[1]:=85;
  if address=-1 then
  begin
    sendbuff[2]:=255;
    sendbuff[3]:=255;
  end
  else
  begin
    sendbuff[2]:=address div 256;
    sendbuff[3]:=address mod 256;
  end;
  try
    ret:=gets0(mm_handle);
    send(mm_handle,sendbuff,4);
    ret:=gets(mm_handle,recbuff);
    if ((ret<>1) and (recbuff[0]<>165)) then
      result :=-1
    else
    begin
      sendbuff[0]:=4;
      sendbuff[1]:=0;
      sendbuff[2]:=address div 256;
      sendbuff[3]:=address mod 256;
      sendbuff[4]:=commandbyte;
      sendbuff[5]:=znum div 256;
      sendbuff[6]:=znum mod 256;
      send(mm_handle,sendbuff,7);
      ret:=gets(mm_handle,recbuff);
      if ((ret<>3) or (recbuff[0]<>sendbuff[4]) or (recbuff[1]<>sendbuff[5]) or (recbuff[2]<>sendbuff[6])) then
        result:=-2
      else
        result:=0;
    end;
  except
    result:=-2;
  end;
end;

function ComShut(ComPort:integer;bps:longint;address,index:integer):integer;
var
  mm_handle:THandle;
  i,j,ret : integer;
  sendbuff,recbuff :array of byte;
  sendlength : integer;
begin
  setlength(sendbuff,4096);
  setlength(recbuff,256);
  try
    mm_handle:=portopen('Com' + inttostr(ComPort),bps,'n',8,1);
    if mm_handle<=0 then
    begin
      result:=-1;
    end
    else
    begin
      ret:=ComHandShake(mm_handle,address,4,1);
      if ret<0 then
        result:=-2
      else
      begin
        ret:=gets0(mm_handle);
          sendlength:=17;
          sendbuff[0]:=0;
          sendbuff[1]:=1;
          sendbuff[2]:=0;
          sendbuff[3]:=12;
          sendbuff[4]:=index * 64;
          for i:=5 to 15 do
            sendbuff[i]:=0;
          sendbuff[16]:=GetBcc(sendbuff,0,15);
          send(mm_handle,sendbuff,sendlength);
          ret:=gets(mm_handle,recbuff);
          if ((ret<>1) or (recbuff[0]<>sendbuff[16])) then
            result:=-4
          else
            result :=0;

      end;
    end;

  finally
    portclose(mm_handle);
  end;

end;

end.

