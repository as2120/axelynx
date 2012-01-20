; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "AxelynxSDK"
#define MyAppVersion "1.02"
#define MyAppPublisher "likosoft, Inc."
#define MyAppURL "http://www.code.google.com/p/axelynx"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{45C6D281-7C04-4DA9-B4FA-091FC2815B2D}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=C:/{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir=D:\axelynx
OutputBaseFilename=setup
SetupIconFile=C:\AxelynxSDK\tools\Wizards\MSVS2010\AxelynxApp\AxelynxApp.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Types]
Name: "full"; Description: "Full installation"
Name: "source"; Description: "Sources for manual compilation"
Name: "precompiled"; Description: "Precompiled SDK"

[Components]
Name: "binaries"; Description: "Program Files"; Types: precompiled full
Name: "sources"; Description: "Readme File"; Types: source full
Name: "SDK"; Description: "Readme File"; Types: full source precompiled  

[Files]
;Base SDK
Source: "C:\AxelynxSDK\axelynx\*";        DestDir: "{app}\axelynx\";        Components: SDK;      Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\AxelynxSDK\bin\*";            DestDir: "{app}\bin\";            Components: binaries; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\AxelynxSDK\documentation\*";  DestDir: "{app}\documentation\";  Components: SDK;      Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\AxelynxSDK\lib\*";            DestDir: "{app}\lib\";            Components: binaries; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\AxelynxSDK\libs\*";           DestDir: "{app}\libs\";           Components: sources;  Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\AxelynxSDK\resource\*";       DestDir: "{app}\resource\";       Components: sources;  Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\AxelynxSDK\samples\*";        DestDir: "{app}\samples\";        Components: SDK;      Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\AxelynxSDK\solutions\*";      DestDir: "{app}\solutions\";      Components: SDK;      Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\AxelynxSDK\source\*";         DestDir: "{app}\source\";         Components: sources;  Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\AxelynxSDK\tools\*";          DestDir: "{app}\tools\";          Components: SDK;      Flags: ignoreversion recursesubdirs createallsubdirs

;Wizards
;CB
Source: "C:\AxelynxSDK\tools\Wizards\CB\*";          DestDir: "{userappdata}\codeblocks\UserTemplates\";          Components: SDK;      Flags: ignoreversion recursesubdirs createallsubdirs

;MSVS
Source: "C:\AxelynxSDK\tools\Wizards\MSVS2010\AxelynxApp\AxelynxApp.ico";          DestDir: "{userdocs}\Visual Studio 2010\Wizards\";          Components: SDK; Flags: ignoreversion
Source: "C:\AxelynxSDK\tools\Wizards\MSVS2010\AxelynxApp\AxelynxApp.vsdir";          DestDir: "{userdocs}\Visual Studio 2010\Wizards\";          Components: SDK;Flags: ignoreversion
Source: "C:\AxelynxSDK\tools\Wizards\MSVS2010\AxelynxApp\AxelynxApp.vsz";          DestDir: "{userdocs}\Visual Studio 2010\Wizards\";          Components: SDK; Flags: ignoreversion


; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "likosoft\{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "likosoft\{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "likosoft\{group}\solutions\msvs2010.sln"; Filename: "{app}\solutions\msvs2010\axelynx.sln"
Name: "likosoft\{group}\solutions\codeblocks.cbp"; Filename: "{app}\solutions\cb\axelynx.cbp"

[Registry]
Root: HKCU; Subkey: "Environment"; ValueType: string; ValueName: "AXELYNX_SDK"; ValueData: "{app}\"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Environment"; ValueType: string; ValueName: "AXELYNX_SDK"; ValueData: "{app}\";
[CODE]
procedure RePlaceTextinFile(FilePath:string; find:string; replace:string);
var
  S: TArrayOfString;
  i : Integer;
begin
 // if not FileExists(FilePath) then begin MsgBox)'���� �� ������' + FilePath, mbError, mb_Ok); Exit;end; 
  LoadStringsFromFile(FilePath, S); 
  for i:= 0 to GetArrayLength(S)-1 do begin
    if (Pos(AnsiUpperCase(find), AnsiUpperCase(S[i])) > 0) then begin
      StringChangeEx(S[i], find, replace, True);
    end;
  end;
  SaveStringsToFile(FilePath, S, False); 
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  case CurPageID of
    wpFinished:
		begin
			RePlaceTextinFile(ExpandConstant('{userdocs}')+'\Visual Studio 2010\Wizards\AxelynxApp.vsz','$(AXELYNX_SDK)',ExpandConstant('{app}'));
//      MsgBox('CurPageChanged:' #13#13 'Welcome to final page of this demo. Click Finish to exit.', mbInformation, MB_OK);
			
		end;

			
  end;
end;