cmake_minimum_required(VERSION 3.20)

# set the project name
project(ConverterPIX VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(ConverterPIX
    src/callbacks.cpp
    src/cmd/_main.cpp
    src/config.cpp
    src/fs/file.cpp
    src/fs/filesystem.cpp
    src/fs/hashfilesystem.cpp
    src/fs/hashfs_file.cpp
    src/fs/sysfilesystem.cpp
    src/fs/sysfs_file.cpp
    src/fs/uberfilesystem.cpp
    src/fs/zipfilesystem.cpp
    src/fs/zipfs_file.cpp
    #src/gui/_maingui.cpp
    #src/gui/converter.cpp
    src/material/material.cpp
    src/model/animation.cpp
    src/model/collision.cpp
    src/model/model.cpp
    src/model/piece.cpp
    src/pix/pix.cpp
    src/prefab/prefab.cpp
    src/prerequisites.cpp
    src/resource_lib.cpp
    src/structs/dds.cpp
    src/texture/texture.cpp
    src/texture/texture_object.cpp
    src/utils/string_tokenizer.cpp
    src/utils/token.cpp
)

function(locate_module_path module filename suffix)
    string(TOUPPER "${module}" module)
    string(CONCAT MODULE_PREFIX "${module}" "_PREFIX")
    set("PREFIX" "${${MODULE_PREFIX}}")
    string(COMPARE EQUAL "${module}" "" PREFIX_MISSING)

    if(PREFIX_MISSING)
        message(FATAL_ERROR "The path to ${module} must be passed in the ${MODULE_PREFIX} variable")
    endif()

    file(GLOB_RECURSE FILE_PATH
        "${PREFIX}/*${filename}"
    )

    if(NOT FILE_PATH)
        message(FATAL_ERROR "Unable to locate include path for ${module}")
    endif()

    set("${module}_${suffix}" "${FILE_PATH}" PARENT_SCOPE)
endfunction()

# Find include directories (some are not directly in /include)
locate_module_path("CityHash" "city.h"   "INCLUDE_DIR")
locate_module_path("fmt"      "format.h" "INCLUDE_DIR")
locate_module_path("glm"      "glm.hpp"  "INCLUDE_DIR")
locate_module_path("zlib"     "zlib.h"   "INCLUDE_DIR")

# GLM is special in that it does not use an /include folder
string(REGEX REPLACE "(.*/include)[\\/]?.*[\\/]city.h"   "\\1" CITYHASH_INCLUDE_DIR "${CITYHASH_INCLUDE_DIR}")
string(REGEX REPLACE "(.*/include)[\\/]?.*[\\/]format.h" "\\1" FMT_INCLUDE_DIR      "${FMT_INCLUDE_DIR}")
string(REGEX REPLACE "(.*/)[\\/]?.*[\\/]glm.hpp"         "\\1" GLM_INCLUDE_DIR      "${GLM_INCLUDE_DIR}")
string(REGEX REPLACE "(.*/include)[\\/]?.*[\\/]zlib.h"   "\\1" ZLIB_INCLUDE_DIR     "${ZLIB_INCLUDE_DIR}")

# Find libraries
if(WIN32)
    locate_module_path("CityHash" "Cityhash.lib"   "LIB_PATH")
    locate_module_path("fmt"      "fmt.lib"        "LIB_PATH")
    locate_module_path("zlib"     "zlibstatic.lib" "LIB_PATH")
else()
    locate_module_path("CityHash" "libcityhash.a" "LIB_PATH")
    locate_module_path("fmt"      "libfmt.a"      "LIB_PATH")
    locate_module_path("zlib"     "libz.a"        "LIB_PATH")
endif()

target_include_directories(ConverterPIX PUBLIC
    "${CITYHASH_INCLUDE_DIR}"
    "${FMT_INCLUDE_DIR}"
    "${GLM_INCLUDE_DIR}"
    "${ZLIB_INCLUDE_DIR}"
)

target_link_libraries(ConverterPIX
    PRIVATE "${CITYHASH_LIB_PATH}"
    PRIVATE "${FMT_LIB_PATH}"
    PRIVATE "${ZLIB_LIB_PATH}"
)
