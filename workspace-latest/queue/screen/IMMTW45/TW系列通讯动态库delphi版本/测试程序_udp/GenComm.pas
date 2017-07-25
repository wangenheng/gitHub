unit GenComm;
interface

uses
  Windows,
  SysUtils,
  Messages,
  Dialogs,
  Graphics,
  Classes,
  inifiles;



      function    portOpen(com:string;bps:longint;par:char;dbit,sbit:byte):THandle;stdcall;
      function   portClose(mm_handle:Thandle):integer;stdcall;
      procedure   SendHex(mm_handle:Thandle;S:String);stdcall;
      function    Gets(mm_handle:THandle;var recbuff :array of byte):integer;stdcall;
      function    Gets0(mm_handle:THandle):integer;stdcall;
      procedure   ClearInBuf(mm_handle:Thandle);stdcall;
      procedure   Send(mm_handle:THandle;var buf:array of byte;len:integer);
implementation
  {$H+}

  const
      CommInQueSize     =   4096;         //输入缓冲区大小
      CommOutQueSize   =   4096;         //输出缓冲区大小



  //写串口   buf:存放要写的数据，len:要写的长度
  procedure     send(mm_handle:THandle;var   buf : array of byte;len:integer);
  var
      i   :   dword;
  begin
      WriteFile(mm_handle,Buf,len,i,nil);     //写串口
  end;

  //以十六进制的形式向com端口发送数据
  procedure   SendHex(mm_handle:Thandle;S:   String);
  var
      s2:string;
      buf1:array[0..50000]   of   byte;
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
          buf1[i]:=(strtoint('$'+copy(s2,i*2+1,2)));
      send(mm_handle,buf1,(length(s2)   div   2));
  end;


  //打开COM端口函数
  function   portOpen(com:string;bps:longint;par:char;dbit,sbit:byte):THandle;
  var
      dcb       :   TDCB;
      commTmo   :   TCommTimeouts;
      m_handle  :   THandle;
  begin
//      OpenFlag:=false;   //初始化串口
      m_handle:=CreateFile(PChar(com),
                            GENERIC_READ or GENERIC_WRITE,
                            0,
                            nil,
                            OPEN_EXISTING,
                            0,
                            0   );
      if   m_handle   =   INVALID_HANDLE_VALUE   then
          begin
              //showmessage('无法打开串口:'   +   com);
              result:=0;
              exit;
          end;
      SetupComm(m_handle,4096,4096);
      GetCommState(m_handle,dcb);
      with   dcb   do
      begin
          BaudRate:=bps;                                 //   波特率
          if   dbit   in   [4,5,6,7,8]   then       //   数据位
              ByteSize:=dbit
          else
              ByteSize:=8;
          case   sbit   of                                     //   停止位
              1:   StopBits:=0;                           //   1
              2:   StopBits:=2;                           //   2
              else   StopBits:=0;                       //1
              //else   StopBits:=1;                       //   1.5
          end;
          case   par   of                                       //校验
              'n','N':   Parity:=0;                   //no
              'o','O':   Parity:=1;                   //odd
              'e','E':   Parity:=2;                   //even
              'm','M':   Parity:=3;                   //mark
              's','S':   Parity:=4;                   //space
              else           Parity:=0;                   //no
          end;
          flags:=17;
          eofchar:=#26;
      end;
      if (not SetCommState(m_handle,dcb)) then
      begin
        result:=0;
        closeHandle(m_handle);
        M_handle:=INVALID_HANDLE_VALUE;
        exit;
      end;

      if not GetCommTimeOuts( m_handle,  commTMO ) then // can't get port time outs
      begin
         result := 0;
         CloseHandle( m_handle );
         m_handle := INVALID_HANDLE_VALUE;
         exit;
      end;{ if }
      with commTMO do // setup port time outs
      begin
       ReadIntervalTimeOut:=MAXDWORD;
       ReadTotalTimeOutMultiplier:=0;
       ReadTotalTimeOutConstant:=0;
      end;
      if not SetCommTimeOuts( m_handle,  commTMO ) then // can't set port time outs
      begin
       result := 0;
         CloseHandle( m_handle );
         m_handle := INVALID_HANDLE_VALUE;
         exit;
      end;{ if }

      if not SetupComm(m_handle,CommOutQueSize,CommInQueSize) then result:=3;
      setcommMask(m_handle,EV_RXCHAR or EV_TXEMPTY);
//      OpenFlag:=true;
      result:=m_handle;
  end;



  //关闭串口
  function   portClose(mm_handle:Thandle):integer;
  begin
//        if   OpenFlag   then
            CloseHandle(mm_handle);
            mm_handle := INVALID_HANDLE_VALUE;
//        OpenFlag:=false;
  end;




  //检测输入缓冲区中的字符数
  function   InbufChars(mm_handle:Thandle):longint;
  var
      ErrCode   :   dword;
      Stat         :   TCOMSTAT;
  begin
      result:=0;
//      if   not   OpenFlag   then   exit;
      ClearCommError(mm_handle,ErrCode,@Stat);
      result:=stat.cbInQue;
  end;

  //检测输出缓冲区中的字符数
  function   OutBufChars(mm_handle:THandle):Longint;
  var
      ErrCode   :   dword;
      Stat         :   TCOMSTAT;
  begin
      result:=0;
//      if   not   OpenFlag   then   exit;
      ClearCommError(mm_handle,ErrCode,@Stat);
      result:=stat.cbOutQue;
  end;


  //　从COM　端口中读取卡号字符串函数；
  function   Gets(mm_handle:Thandle;var recbuff : array of byte):integer;
  Var
      d:   array[0..255]   of   byte;
      BytesRead:   dword;
      itemp : longint;
      i,j : integer;
      hh,mm,ss,ms : word;
      ret1,ret2:integer;
      waittime:integer;
  begin
    result:=0;
    decodetime(now(),hh,mm,ss,ms);
    ret1:=ss*1000+ms;
    if mm_handle<1 then  exit;
    for i:=0 to 32 do
    begin
      if result>0 then
      begin
//        decodetime(now(),hh,mm,ss,ms);
//        ret2:=ss*1000+ms;
//        waittime:=ret2-ret1;
//        if (waittime>230) and (i>10) then
//          result:=waittime;
        exit;
      end;
//    if strlen(result)>0 then exit;
      begin
       itemp:=inbufchars(mm_handle);
       if itemp>0 then
       begin
       ReadFile   (mm_handle,   d,   256,   BytesRead,   Nil);

         if bytesRead>0 then
         begin
           for j:=0 to bytesread-1 do

             recbuff[j]:=d[j];
           result:=bytesread;
         end
         else
         begin
           result:=0;
           sleep(30);
         end;
       end
       else
         sleep(30);
     end;
    end;
    decodetime(now(),hh,mm,ss,ms);
    ret2:=ss*1000+ms;
    waittime:=ret2-ret1;
    if waittime>1 then
      result:=0;//waittime;
  end;

 function   Gets0(mm_handle:Thandle):integer;
  Var
      d:   array[0..255]   of   byte;
      BytesRead:   dword;
      itemp : longint;
      i,j : integer;
      hh,mm,ss,ms : word;
      ret1,ret2:integer;
      waittime:integer;
      recbuff:array[0..255] of byte;
  begin
    result:=0;
    decodetime(now(),hh,mm,ss,ms);
    ret1:=ss*1000+ms;
    if mm_handle<1 then  exit;
    for i:=0 to 2 do
    begin
      if result>0 then
      begin
//        decodetime(now(),hh,mm,ss,ms);
//        ret2:=ss*1000+ms;
//        waittime:=ret2-ret1;
//        if (waittime>230) and (i>10) then
//          result:=waittime;
        exit;
      end;
//    if strlen(result)>0 then exit;
      begin
       itemp:=inbufchars(mm_handle);
       if itemp>0 then
       begin
       ReadFile   (mm_handle,   d,   256,   BytesRead,   Nil);

         if bytesRead>0 then
         begin
           for j:=0 to bytesread-1 do

             recbuff[j]:=d[j];
           result:=bytesread;
         end
         else
         begin
           result:=0;
           sleep(10);
         end;
       end
       else
         sleep(10);
     end;
    end;
    decodetime(now(),hh,mm,ss,ms);
    ret2:=ss*1000+ms;
    waittime:=ret2-ret1;
    if waittime>1 then
      result:=0;//waittime;
  end;

  //清接收缓冲区
  procedure   ClearInBuf(mm_handle:THandle);
  begin
      if   mm_handle<1   then   exit;
            PurgeComm(mm_handle,PURGE_RXABORT or PURGE_RXCLEAR or
                   PURGE_TXABORT or PURGE_TXCLEAR);

  end;


end.

