@echo off

REM This file manually copies the lib and include files, because CMake refused to obey my
REM install prefix, and I'm too drunk and tired to continue searching for the reason why

set /p cityhashpath="Enter path to cityhash directory: "

REM We probably don't really need to care _this_ much about SSE 4.2, but since they require
REM a config.h anyway, let's give them one with the only relevant define. We do this before asking
REM for input, because it's just simpler to refer to the file locally
> NUL cl.exe /nologo /EHsc  test_sse42.cpp
test_sse42.exe "%cityhashpath%\src\config.h"
del test_sse42.exe test_sse42.obj

REM Cityhash does not come prepared for CMake, so we hack this... uglily
cd %cityhashpath%
 > CMakeLists.txt echo cmake_minimum_required(VERSION 3.10)
>> CMakeLists.txt echo project(Cityhash VERSION 1.0)
>> CMakeLists.txt echo if(NOT WIN32)
>> CMakeLists.txt echo message(FATAL_ERROR ^"This CMake configuration is only for Windows^")
>> CMakeLists.txt echo endif()
>> CMakeLists.txt echo set(CMAKE_CXX_STANDARD 11)
>> CMakeLists.txt echo add_library(Cityhash STATIC
>> CMakeLists.txt echo     src/city.cc
>> CMakeLists.txt echo )
>> CMakeLists.txt echo target_include_directories(Cityhash
>> CMakeLists.txt echo     PRIVATE src/
>> CMakeLists.txt echo )

REM Now proceed with the fun stuff
mkdir build\cmake
mkdir build\cityhash\include
mkdir build\cityhash\lib
copy src\city.h build\cityhash\include\
cd build\cmake
cmake ..\..
cmake --build . --target ALL_BUILD --config Release
copy "Release\Cityhash.lib" "..\cityhash\lib\"
echo "Remember to specify -DCITYHASH_PREFIX=%cityhashpath%/build to your cmake command for ConverterPIX"
