unit PicToData;

interface
uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

  function GetArrayFromPic(GDC:hdc;var buff:array of byte;width,height,PColorSign:integer):integer;stdcall;
  function DrawRectString1(GDC:hdc;UserData:Pchar;FontName:Pchar;FontSize,FontColor,Left,Top,Width,Height:integer):integer;stdcall;

implementation
function DrawRectString1(GDC:hdc;UserData:Pchar;FontName:Pchar;FontSize,FontColor,Left,Top,Width,Height:integer):integer;stdcall;
var
  i,j,k,ret : integer;
  xx,yy,zz:integer;
  xxx,yyy,zzz:integer;
  bitmap:TBitmap;
begin
  bitmap:=Tbitmap.Create ;
  bitmap.Width :=width;
  bitmap.Height :=height;
  bitmap.Canvas.Brush.Color :=clBlack;
  bitmap.Canvas.Font.Name :=FontName;
  bitmap.Canvas.Font.Size :=fontSize;
  bitmap.Canvas.Font.Color:=ClYellow;
  if Fontcolor=1 then
    bitmap.canvas.font.color:=ClRed;
  if fontcolor=2 then
    bitmap.canvas.font.color:=clGreen;

  bitmap.canvas.textout(0,0,userdata);
  bitblt(gdc,left,top,width,height,bitmap.canvas.handle,0,0,srccopy);

//  bitmap.Canvas.Handle:=gdc;
//  bitblt(bitmap.Canvas.Handle ,0,0,width-1,height-1,gdc,0,0,SRCCOPY);
//  bitblt(gdc,100,50,width,height,bitmap.Canvas.Handle,0,0,srccopy);
  bitmap.Handle:=0;
  bitmap.Free ;
  result:=0;
end;


function GetArrayFromPic(GDC:hdc;var buff:array of byte;width,height,PColorSign:integer):integer;
var
  i,j,k,ret : integer;
  xx,yy,zz:integer;
  xxx,yyy,zzz:integer;
  bitmap:TBitmap;
begin
  bitmap:=Tbitmap.Create ;
  bitmap.Canvas.Handle:=gdc;
//  bitblt(bitmap.Canvas.Handle ,0,0,width-1,height-1,gdc,0,0,SRCCOPY);
//  bitblt(gdc,100,50,width,height,bitmap.Canvas.Handle,0,0,srccopy);
  for j:=0 to height-1 do
  begin
    for i:=0 to (width -1) div 8  do
    begin
      xxx:=0;
      yyy:=0;
      zzz:=0;
      for k:=0 to 7 do
      begin

        zz:=bitmap.Canvas.Pixels[i*8+k,j];
        if pcolorsign=1 then
        begin
          zzz:=zzz*2;
          if zz>64 then
          zzz:=zzz+1;
        end
        else
        begin
          xxx:=xxx*2;
          yyy:=YYY*2;
          xx:=zz mod 256;
          if xx>64 then xxx:=xxx+1;
          yy:=(zz div 256)mod 256;
          if yy>64 then yyy:=yyy+1;

        end;
      end;
      if pcolorsign=1 then
        buff[i+j*((width -1 )div 8 +1)]:=zzz
      else
      begin
        buff[i+2*j*((width -1 )div 8 +1)]:=xxx;
        buff[i+(2*j+1)*((width -1 )div 8 +1)]:=yyy;
      end;
    end;
  end;
  bitmap.Free ;
  bitmap.Handle:=0;
  result:=0;
end;

end.
