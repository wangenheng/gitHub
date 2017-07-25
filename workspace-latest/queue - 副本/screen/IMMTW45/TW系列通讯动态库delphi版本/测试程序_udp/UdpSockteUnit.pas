unit UdpSockteUnit;

interface
uses
      Windows,   Winsock,   Messages,   SysUtils,   Variants,   Classes,   Graphics,   Controls,
      Forms,
      Dialogs,   StdCtrls;



//  var
//          s:   TSocket;
//          addr:   TSockAddr;
//          FSockAddrIn:   TSockAddrIn;
//          buff:       array[0..64,0..64] of byte;
//          currx,curry : integer;
//          commandtype : integer;


          function    SocketOpen(LocalPort:integer):TSocket;stdcall;
          function    SocketClose(SockID:TSocket):integer;stdcall;
          function    SendData(Sockid:TSocket;DescIP:Pchar;DescPort:integer; buf:array of byte;bufLen:integer):integer;stdcall;
          function    funDataSend(LocalPort:integer;DescIP:Pchar;DescPort:integer;bufLen:integer;var buf:array of byte):integer;stdcall;
          function    ReadData(SockId:TSocket;var recbuff:array of byte):integer;stdcall;

implementation

  function   ReadData(SockID:TSocket;var recbuff:array of byte):integer;
  var
      buffer:   array[0..4096]   of   byte;
      len:   integer;
      flen:   integer;
      i : integer;
      FSockAddrIn:   TSockAddrIn;
      LocalPort: integer;
      ul : integer;
  begin
      ul:=1;
      LocalPort:=11025;
      flen   :=   sizeof(FSockAddrIn);
      FSockAddrIn.SIn_Family   :=   AF_INET;
      FSockAddrIn.sin_port :=htons(LocalPort);
      ioctlsocket(  sockid,FIONBIO,ul );
//      FSockAddrIn.SIn_Port   :=   htons(UDPPORT);
//     Event   :=   WSAGetSelectEvent(Message.LParam);
//      if   Event   =   FD_READ   then
      begin
          len   :=   recvfrom(sockid,   buffer,   sizeof(buffer),   0,   FSockAddrIn,   flen);
          result:=len;
        for i:=0 to len-1 do
          recbuff[i]  := buffer[i];

      end;
  end;

  function SendData(Sockid:TSocket;DescIP:Pchar;DescPort:integer;buf:array of byte;bufLen:integer):integer;
  var
      len:   integer;
      FSockAddrIn:   TSockAddrIn;

  begin
      result:=0;
  //    FSockAddrIn.SIn_Addr.S_addr   :=   INADDR_BROADCAST;
      FSockAddrIn.SIn_Family   :=   AF_INET;
      FSockAddrIn.SIn_Addr.S_addr   :=   inet_addr(pchar(DescIP));   //INADDR_BROADCAST;       //INADDR_BROADCAST   =   -1   ?
      FSockAddrIn.sin_port :=htons(DescPort);
//      value   :=   Content;
      len   :=   sendto(Sockid,   buf,   bufLen,   0,   FSockAddrIn,  sizeof(FSockAddrIn));
      if   (WSAGetLastError()   <>   WSAEWOULDBLOCK)   and   (WSAGetLastError()   <>   0)   then
        result:=-1; // showmessage(inttostr(WSAGetLastError()));
      if   len   =   SOCKET_ERROR   then
        result:=-2;  //showmessage('send   fail');
      if   len   <>  buflen then  // Length(value)   then
        result:=-3;  //showmessage('Not   Send   all');
      
  end;

function    SocketOpen(LocalPort:integer):TSocket;
  var
      TempWSAData:   TWSAData;
      optval:   integer;
      s:   TSocket;
      addr:   TSockAddr;
      FSockAddrIn:   TSockAddrIn;

  begin
      //   初始化SOCKET
      if   WSAStartup($101,   TempWSAData)   =   1   then
      begin
          result:=-1;//showmessage('StartUp   Error!');
          exit;
      end;
      s   :=   Socket(AF_INET,   SOCK_DGRAM,   0);
      if   (s   =   INVALID_SOCKET)   then   //Socket创建失败
      begin
          result:=-2; //showmessage(inttostr(WSAGetLastError())   +   '     Socket创建失败');
          CloseSocket(s);
          
          exit;
      end;
      //发送方SockAddr绑定
      addr.sin_family   :=   AF_INET;
      addr.sin_addr.S_addr   :=   INADDR_ANY;
      addr.sin_port   :=   htons(LocalPort);
      if   Bind(s,   addr,   sizeof(addr))   <>   0   then
      begin
          result:=-3;//showmessage('bind   fail');
          closeSocket(s);
          exit;
      end;

      optval   :=   1;
      if   setsockopt(s,SOL_SOCKET, SO_BROADCAST, pchar(@optval), sizeof(optval)) =  SOCKET_ERROR   then
      begin
          result:=-4;//showmessage('无法进行UDP广播');
          closeSocket(s);
          exit;
      end;

      //接收端SockAddrIn设定
      FSockAddrIn.SIn_Family   :=   AF_INET;
      FSockAddrIn.SIn_Port   :=   htons(LocalPort);
      result:=s;
  end;

function   SocketClose(SockID:TSocket):integer;
  begin
      CloseSocket(SockID);
      result:=0;
  end;

function    funDataSend(LocalPort:integer;DescIP:Pchar;DescPort:integer;buflen:integer;var buf:array of byte):integer;
var
    recbuff: array[0..4096] of byte;
    SockId:Tsocket;
    ret: integer;
    i: integer;
begin
  try
    Sockid:=SocketOpen(LocalPort);
    if Sockid<0 then
      Result:=Sockid
    else
    begin
      ret:=Senddata(SockId,DescIP,DescPort,buf,buflen);
      if ret<0 then
        result:=-4
      else
      begin
        sleep (50);
        ret:=readdata(sockid,recbuff);
        for i:=0 to ret-1 do
          buf[i]:=recbuff[i];
        result:=ret;
      end;
    end;
  finally
    SocketClose(SockID);
  end;
end;
end.
