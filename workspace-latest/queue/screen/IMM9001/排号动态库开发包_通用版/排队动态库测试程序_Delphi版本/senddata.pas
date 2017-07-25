unit senddata;
interface

uses
  Windows,
  SysUtils,
  Messages,
  Dialogs,
  Classes;

type sendarray = array of byte;
  var
      OpenFlag:   boolean;             //�򿪱�־
      m_handle     :   THandle;             //���ھ��
      buff:       array[0..2047,0..1023] of byte;
      function    SendDatafun(comport:integer;bps:longint;address:integer;sendData:pchar;color,showMode,showSpeed,showTime:integer):integer;stdcall;export;
      function    dataSend(mm_handle:integer;address:integer;sendData:pchar;color,showMode,showSpeed,showTime:integer):integer;stdcall;export;
      procedure   portOpen(com:string;bps:longint;par:char;dbit,sbit:byte);stdcall;
      procedure   portClose;stdcall;
      procedure   SendHex(S:   String);stdcall;
      function     Gets:string;stdcall;
      procedure   ClearInBuf;stdcall;
      function   strtoneima(neistr:string;hzcolor : integer):integer;

      procedure   Send(var   buf;len:integer);
      procedure   dodelaytime(delaytime:integer);

implementation
  {$H+}

  const
      CommInQueSize     =   4096;         //���뻺������С
      CommOutQueSize   =   4096;         //�����������С
procedure   dodelaytime(delaytime:integer);
var
  currenttime : longint;
begin
      currentTime:=GetTickCount ;
      while ((GetTickCount )<(currentTime+ 100)) do
      ;

end;

function    SendDatafun(comport:integer;bps:longint;address:integer;sendData:pchar;color,showMode,showSpeed,showTime:integer):integer;
begin
    if openFlag then portclose;
    portopen('Com' + inttostr(comport),bps,'n',8,1);
      dataSend(m_handle,address,senddata,color,showmode,showspeed,showtime);
    portclose;

end;
  function    dataSend(mm_handle:integer;address:integer;sendData:pchar;color,showMode,showSpeed,showTime:integer):integer;
  var
    ret: string;
    i,j,k,commandbyte,packagecounter,packagesize: integer;
    STX : array[0..3] of byte;
    bcc : array[0..0] of byte;
    STArray:  array[0..6] of byte;
    indexA:   array[0..3] of byte;
    FSign:    array[0..3] of byte;
    senddataarray1: array[0..7] of byte;
    senddataarray: array[0..255] of byte;
//    senddataarray: sendarray;
    strtemp: string;
    sendStatus : boolean;
    begintime,endtime,DelayTime: integer;
    currdate : string;
    currmm,currdd :integer;

  begin
    m_handle:=mm_handle;
    i:=strtoneima(sendData,color);
    packagecounter:=(i-2) * 256 +1;
    packagesize:=i;
//    setlength(senddataarray,packagesize +3);
//    currdate:=formatdatetime("mm/dd",now);
    currmm:=strtoint(formatdatetime('mm',now));
    currdd:=strtoint(formatdatetime('dd',now));
    commandbyte:=(currmm mod 3) * 31 + currdd +17;
 //   commandbyte:=118;
    STX[0]:=170;
    STX[1]:=85;
    if (address=-1) then
    begin
      STX[2]:=$FF;
      STX[3]:=$FF;
    end
    else
    begin
      STX[2]:=address div 256;
      STX[3]:=address mod 256;
    end;
    starray[0]:=4;
    starray[1]:=0;
    starray[2]:=address div 256;
    starray[3]:=address mod 256;
    starray[4]:=commandbyte;
    starray[5]:=packagecounter div 256;
    starray[6]:=packagecounter mod 256;
    packagecounter:=packagecounter mod 256;

    send(stx,4);
    ret:=Gets;
//    dodelaytime(500);
//    i:=165;
    result:=0;
    if strlen(pchar(ret))>0 then //and ret[0]=165 then
    begin
      for i:=0 to packagecounter -1 do
      begin
        senddataarray[0]:=(i+1) div 256;
        senddataarray[1]:=(i+1) mod 256;
        senddataarray[2]:=packagesize div 256;
        senddataarray[3]:=packagesize mod 256;
        for j:=0 to packagesize -1 do
        begin
          senddataarray[j+4]:=buff[i,j];
        end;
//        bcc:=jiao(senddataarray,0);
        k:=0;
        for j:=0 to packagesize+3 do
        begin
          k:=k xor senddataarray[j];
        end;
        bcc[0]:=k;
        send(starray,7);
        ret:=gets;
        if strlen(pchar(ret))<1 then
        begin
          result:=-1;
          exit;
        end;
        send(senddataarray,packagesize +4);
        send(bcc,1);
        ret:=gets;
        if strlen(pchar(ret))<1 then
        begin
          result:=-1;
          exit;
        end;
//        if gets<>bcc[0] then
//        begin
//          result:=-1;
//        end;

      end;
    end
    else
    begin
      result:=-1;
    exit;
    end;

    starray[4]:=2;
    starray[5]:=1 div 256;
    starray[6]:=1 mod 256;
    packagecounter:=1 ;
    packagesize:=4;
    buff[0,0]:=commandbyte-16;
    buff[0,1]:=showmode;
    buff[0,2]:=showspeed;
    buff[0,3]:=showtime;
    send(stx,4);
    result:=0;
        ret:=gets;
        if strlen(pchar(ret))<1 then
        begin
          result:=-1;
          exit;
        end;
      for i:=0 to packagecounter -1 do
      begin
        senddataarray1[0]:=(i+1) div 256;
        senddataarray1[1]:=(i+1) mod 256;
        senddataarray1[2]:=4 div 256;
        senddataarray1[3]:=4 mod 256;
        for j:=0 to packagesize -1 do
        begin
          senddataarray1[j+4]:=buff[i,j];
        end;
//        bcc:=jiao(senddataarray,0);
        k:=0;
        for j:=0 to packagesize+3 do
        begin
          k:=k xor senddataarray1[j];
        end;
        bcc[0]:=k;
        send(starray,7);
        ret:=gets;
        if strlen(pchar(ret))<1 then
        begin
          result:=-1;
          exit;
        end;
        send(senddataarray1,packagesize +4);
        send(bcc,1);
        ret:=gets;
        if strlen(pchar(ret))<1 then
        begin
          result:=-1;
          exit;
        end;
//        if gets<>bcc[0] then
//        begin
//          result:=-1;
//        end;

      end;

//    datasend:=strtemp;
  end;

  //д����   buf:���Ҫд�����ݣ�len:Ҫд�ĳ���
  procedure     send(var   buf;len:integer);
  var
      i   :   dword;
  begin
      WriteFile(m_handle,Buf,len,i,nil);     //д����
  end;

  //��ʮ�����Ƶ���ʽ��com�˿ڷ�������
  procedure   SendHex(S:   String);
  var
      s2:string;
      buf1:array[0..50000]   of   char;
      i:integer;
  begin
      s2:='';
      for   i:=1   to     length(s)   do
      begin
          if   ((copy(s,i,1)>='0')   and   (copy(s,i,1)<='9'))or((copy(s,i,1)>='a')   and   (copy(s,i,1)<='f'))
                  or((copy(s,i,1)>='A')   and   (copy(s,i,1)<='F'))   then
          begin
                  s2:=s2+copy(s,i,1);
          end;
      end;
      for   i:=0   to   (length(s2)   div   2-1)   do
          buf1[i]:=char(strtoint('$'+copy(s2,i*2+1,2)));
      send(buf1,(length(s2)   div   2));
  end;


  //��COM�˿ں���
  procedure   portOpen(com:string;bps:longint;par:char;dbit,sbit:byte);
  var
      dcb       :   TDCB;
  begin
      OpenFlag:=false;   //��ʼ������
      m_handle:=CreateFile(PChar(com),
                            GENERIC_READ+GENERIC_WRITE,
                            0,
                            nil,
                            OPEN_EXISTING,
                            0,
                            0   );
      if   m_handle   =   INVALID_HANDLE_VALUE   then
          begin
              showmessage('�޷��򿪴���:'   +   com);
              exit;
          end;
      GetCommState(m_handle,dcb);
      with   dcb   do
      begin
          BaudRate:=bps;                                 //   ������
          if   dbit   in   [4,5,6,7,8]   then       //   ����λ
              ByteSize:=dbit
          else
              ByteSize:=8;
          case   sbit   of                                     //   ֹͣλ
              1:   StopBits:=0;                           //   1
              2:   StopBits:=2;                           //   2
              else   StopBits:=0;                       //1
              //else   StopBits:=1;                       //   1.5
          end;
          case   par   of                                       //У��
              'n','N':   Parity:=0;                   //no
              'o','O':   Parity:=1;                   //odd
              'e','E':   Parity:=2;                   //even
              'm','M':   Parity:=3;                   //mark
              's','S':   Parity:=4;                   //space
              else           Parity:=0;                   //no
          end;
      end;
      SetCommState(m_handle,dcb);
      SetupComm(m_handle,CommOutQueSize,CommInQueSize);
      OpenFlag:=true;
  end;



  //�رմ���
  procedure   portClose;
  begin
        if   OpenFlag   then
            CloseHandle(m_handle);
        OpenFlag:=false;
  end;




  //������뻺�����е��ַ���
  function   InbufChars:longint;
  var
      ErrCode   :   dword;
      Stat         :   TCOMSTAT;
  begin
      result:=0;
      if   not   OpenFlag   then   exit;
      ClearCommError(m_handle,ErrCode,@Stat);
      result:=stat.cbInQue;
  end;

  //�������������е��ַ���
  function   OutBufChars:Longint;
  var
      ErrCode   :   dword;
      Stat         :   TCOMSTAT;
  begin
      result:=0;
      if   not   OpenFlag   then   exit;
      ClearCommError(m_handle,ErrCode,@Stat);
      result:=stat.cbOutQue;
  end;


  //����COM���˿��ж�ȡ�����ַ���������
  function   Gets:string;
  Var
      d:   array[0..255]   of   Char;
      BytesRead:   dword;
      itemp : longint;
      i,j : integer;
  begin
    result:='';
    for i:=0 to 6 do
    begin
    if strlen(pchar(result))>0 then exit;
    begin
     dodelaytime(500);
     itemp:=inbufchars;
     if itemp>0 then
     begin
     ReadFile   (m_handle,   d,   256,   BytesRead,   Nil);

      if bytesRead>0 then
      begin
        for j:=0 to bytesread-1 do
        result:=result + d[j];
//        result:=ord(d[0]);
      end
      else
      begin
        result:='';
      end;
      end;
     end;
    end;
  end;


  //����ջ�����
  procedure   ClearInBuf;
  begin
      if   not   OpenFlag   then   exit;
            PurgeComm(m_handle,PURGE_RXABORT or PURGE_RXCLEAR or
                   PURGE_TXABORT or PURGE_TXCLEAR);

  end;
function strtoneima(neistr:string;hzcolor:integer):integer;
var
  zhuan1,zhuan,z : longint;
  count : integer;
  i,x,y : integer;
begin
  count:=1;
  for i:=1 to strlen(pchar(neistr)) do
  begin
    zhuan1:=ord(neistr[i]);
    if (zhuan1>13) or (zhuan1<0) then
      zhuan:= 65536 + zhuan1;
      if zhuan<65536 then
      begin
        x:=count div 264;
        y:=count mod 264;
        z:=zhuan div 256;
        buff[x][y]:=z;
        buff[(count +1) div 264][(count+1) mod 264]:=zhuan mod 256;
        count:=count+2;
      end
      else
      begin
         buff[count div 264][count mod 264]:=zhuan1;
         count:=count+1;
      end;
  end;
  buff[0][0]:=hzcolor;
  result:=count;
end;
end.

