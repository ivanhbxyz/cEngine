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

SET assembly=cEngin
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Isrc -I%VULKAN_SDK%/Include
SET linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib
set defines=-D_DEBUG -DKEXPORT -D_CRT_SECURE_NO_WARNINGS


ECHO "Building %assembly%%... "
call %clangCall% %cFileNames% -o ../bin/%assembly%.dll %defines% %includeFlags% %linkerFlags% 