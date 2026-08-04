@echo off
setlocal
 
rem --- Locate the VS install with C++ tools, and load its dev environment ---
for /f "usebackq tokens=*" %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set VSPATH=%%i
)
 
if not defined VSPATH (
    echo [build_release.bat] Could not find a Visual Studio install with the C++ workload.
    exit /b 1
)
 
call "%VSPATH%\VC\Auxiliary\Build\vcvarsall.bat" x64
if errorlevel 1 exit /b 1
 
if not exist build mkdir build
if not exist build\obj mkdir build\obj
 
rem --- Compile + link ---
cl.exe /EHsc /MP /W4 /permissive- /std:c++latest /nologo /MD /O2 /DNDEBUG /Fd:build\Chess.pdb /Fo:build\obj\ /Fe:build\Chess.exe ^
  /I "src" /I "src\pieces" ^
  /I "C:\vcpkg\installed\x64-windows\include" ^
  /I "C:\libs\onnxruntime-win-x64-1.28.0\include" ^
  src\main.cpp src\Board.cpp src\Game.cpp src\NNEvaluator.cpp ^
  src\pieces\Bishop.cpp src\pieces\King.cpp src\pieces\Knight.cpp ^
  src\pieces\Pawn.cpp src\pieces\Piece.cpp src\pieces\Queen.cpp src\pieces\Rook.cpp ^
  /link ^
  /ILK:"build\obj\Chess.ilk" ^
  /LIBPATH:"C:\vcpkg\installed\x64-windows\lib" ^
  /LIBPATH:"C:\libs\onnxruntime-win-x64-1.28.0\lib" ^
  sfml-graphics.lib sfml-window.lib sfml-system.lib onnxruntime.lib
 
if errorlevel 1 (
    echo [build.bat] Compile/link failed.
    exit /b 1
)
 
rem --- Copy runtime DLLs next to the exe ---
for %%D in (sfml-graphics-3.dll sfml-window-3.dll sfml-system-3.dll freetype.dll libpng16.dll z.dll bz2.dll brotlicommon.dll brotlidec.dll) do (
    copy /Y "C:\vcpkg\installed\x64-windows\bin\%%D" build\ >nul
)
copy /Y C:\libs\onnxruntime-win-x64-1.28.0\lib\onnxruntime.dll build\ >nul
copy /Y C:\libs\onnxruntime-win-x64-1.28.0\lib\onnxruntime_providers_shared.dll build\ >nul
 
echo [build.bat] Build succeeded: build\Chess.exe
endlocal