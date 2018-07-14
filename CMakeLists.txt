cmake_minimum_required(VERSION 2.8.11)
PROJECT(hptzip C)
set(hptzip_VERSION 1.9.0)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(BUILD_SHARED_LIBS "Build shared libs" ON)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-g -DDEBUG -D_DEBUG)
  set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "My multi config types" FORCE)
else(CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-DNDEBUG)
  set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "My multi config types" FORCE)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

if (MSVC)
  if (BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-D_DLL -D_MAKE_DLL)
  endif()
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../out/lib")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../out/bin")
  SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
  ADD_DEFINITIONS(-DWIN32 -D_WINDOWS -D_CONSOLE -D_CRT_SECURE_NO_WARNINGS)
  if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(ZLIB_ROOT "zlib-win32")
  else()
    set(ZLIB_ROOT "zlib-win64")
  endif()
  include_directories("${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/hptzip;${CMAKE_CURRENT_SOURCE_DIR}/../huskylib;")
  if(CMAKE_BUILD_TYPE MATCHES Debug)
    list( APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../out/lib/Debug" )
  else(CMAKE_BUILD_TYPE MATCHES Debug)
    list( APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../out/lib/Release" )
  endif(CMAKE_BUILD_TYPE MATCHES Debug)
else ()
  include_directories("hptzip")
  ADD_DEFINITIONS(-Wall -DUNIX)
  include(CheckSymbolExists)
  CHECK_SYMBOL_EXISTS("_Z_OF" "zlib.h" HAVE_Z_OF)
  if (${HAVE_Z_OF})
    ADD_DEFINITIONS(-DOF=_Z_OF)
  endif()
endif (MSVC)

include(FindZLIB)
find_library(husky_LIB NAMES husky PATHS "${CMAKE_FIND_ROOT_PATH}")
include_directories("${ZLIB_INCLUDE_DIRS}")

message("-------------------------")
message("System            : ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")
message("Compiler          : ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message("Prefix            : ${CMAKE_INSTALL_PREFIX} (run \"cmake . -DCMAKE_INSTALL_PREFIX:PATH=/other\" for other prefix)")
message("libhusky          : ${husky_LIB}")
message("zlib              : ${ZLIB_LIBRARIES}")
message("Build type        : ${CMAKE_CONFIGURATION_TYPES}")
message("-------------------------")

SET(hptzip_SOURCES
  src/hptzip.c
  src/ioapi.c
  src/unzip.c
  src/zip.c
)
SET(hptzip_HEADERS
  hptzip/crypt.h
  hptzip/hptzipdl.h
  hptzip/hptzip.h
  hptzip/ioapi.h
  hptzip/iowin32.h
  hptzip/unzip.h
  hptzip/version.h
  hptzip/zip.h
)
if (WIN32)
  list(APPEND hptzip_SOURCES src/iowin32.c)
endif()



ADD_LIBRARY(hptzip SHARED ${hptzip_SOURCES})
set_target_properties(hptzip PROPERTIES VERSION ${hptzip_VERSION})
set_target_properties(hptzip PROPERTIES PUBLIC_HEADER "${hptzip_HEADERS}")
target_link_libraries(hptzip ${husky_LIB} ${ZLIB_LIBRARIES})
if (UNIX)
  set_target_properties(hptzip PROPERTIES COMPILE_FLAGS "-O0")
endif()
if (NOT MSVC)
INSTALL(TARGETS hptzip
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include/hptzip
)
  set(CPACK_GENERATOR ${CPACK_GENERATOR} TGZ)
endif (NOT MSVC)
set (CPACK_PACKAGE_VERSION_MAJOR "1")
set (CPACK_PACKAGE_VERSION_MINOR "9")
file(READ cvsdate.h CPACK_PACKAGE_VERSION_PATCH LIMIT 10 OFFSET 17)
string(SUBSTRING ${CPACK_PACKAGE_VERSION_PATCH} 0 10 CPACK_PACKAGE_VERSION_PATCH)

INCLUDE(CPack)
