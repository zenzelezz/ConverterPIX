@echo off
set /p zlibpath="Enter path to zlib directory: "
cd %zlibpath%
mkdir build\cmake
cd build\cmake
cmake -DCMAKE_INSTALL_PREFIX="%zlibpath%\\build\\zlib" ..\..
cmake --build . --target ALL_BUILD --config Release
cmake --install .
echo "Remember to specify -DZLIB_PREFIX=%fmtpath%/build to your cmake command for ConverterPIX"
