REM Build script for cEngin
@ECHO OFF
SetLocal EnableDelayedExpansion

REM clang compiler
set clangCall="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang.exe"

REM Get a list of all the .c files
set cFileNames=
FOR /R %%f in (*.c) do (
    set cFileNames=!cFileNames! %%f
)

REM echo "Files: " %cFileNames%
SET assembly=testbed
SET compilerFlags=-g 
REM -Wall -Werror
SET includeFlags=-Isrc -I../engine/src/
SET linkerFlags=-L../bin/ -lengine.lib
SET defines=-D_DEBUG -DKIMPORT

ECHO "Building %assembly%%... "
call %clangCall% %cFileNames% %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%