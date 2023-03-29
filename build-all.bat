@echo off

echo "WILL NOW BUILD EVERYTHING.. "

pushd engine
call build.bat
popd
if %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)


pushd testbed
echo "Engine build completed correctly..."
echo "Now starting test build script..."
call build.bat
popd
if %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

echo "All assemblies built successfully"