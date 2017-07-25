program TWDLLTESTAPP;

uses
  Forms,
  Unit1 in 'Unit1.pas' {Form1},
  ComSendData in '..\ComSendData.pas',
  GenComm in '..\GenComm.pas',
  PicToData in '..\PicToData.pas';
//  UdpSendData in 'UdpSendData.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
