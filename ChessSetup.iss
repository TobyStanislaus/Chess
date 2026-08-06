; -----------------------------------------------------------------------
; Inno Setup script for Chess
; Builds an installer from the build-release output folder
; -----------------------------------------------------------------------

#define MyAppName "Chess"
#define MyAppVersion "1.0"
#define MyAppExeName "Chess.exe"

; Adjust this to wherever the release build lives on your machine
#define BuildDir "C:\Users\toby\OneDrive\Documents\Github\Chess\build-release"

[Setup]
AppId={{A5B9C1F0-3E2D-4B7A-9F1C-8D2E4A6B7C90}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputDir=.
OutputBaseFilename=ChessSetup
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64compatible
; Uncomment if you get a code-signing cert later:
; SignTool=signtool

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
; Main exe
Source: "{#BuildDir}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; All DLLs sitting next to it (SFML, ONNX Runtime, zlib/brotli/png/freetype)
Source: "{#BuildDir}\*.dll"; DestDir: "{app}"; Flags: ignoreversion

; Assets folder, recursively
Source: "{#BuildDir}\assets\*"; DestDir: "{app}\assets"; Flags: ignoreversion recursesubdirs createallsubdirs

; Note: MSVC runtime is statically linked (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded"),
; so vc_redist.x64.exe is no longer needed here.

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional shortcuts:"; Flags: unchecked

[Run]
; Optional: launch app after install finishes
Filename: "{app}\{#MyAppExeName}"; Description: "Launch {#MyAppName}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{app}"
