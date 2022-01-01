@echo off
set /p fmtpath="Enter path to fmt directory: "
cd %fmtpath%
mkdir build\cmake
cd build\cmake
cmake -DCMAKE_INSTALL_PREFIX="%fmtpath%\\build\\fmt" -DFMT_TEST=0 ..\..
cmake --build . --target ALL_BUILD --config Release
cmake --install .
echo "Remember to specify -DFMT_PREFIX=%fmtpath%/build to your cmake command for ConverterPIX"
