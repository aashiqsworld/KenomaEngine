# Install script for directory: C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Project")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-build/TracyClient.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tracy" TYPE FILE FILES
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/tracy/TracyC.h"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/tracy/Tracy.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/tracy/TracyD3D11.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/tracy/TracyD3D12.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/tracy/TracyLua.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/tracy/TracyOpenCL.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/tracy/TracyOpenGL.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/tracy/TracyVulkan.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/client" TYPE FILE FILES
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/tracy_concurrentqueue.h"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/tracy_rpmalloc.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/tracy_SPSCQueue.h"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyArmCpuTable.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyCallstack.h"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyCallstack.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyCpuid.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyDebug.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyDxt1.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyFastVector.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyLock.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyProfiler.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyRingBuffer.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyScoped.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyStringHelpers.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracySysPower.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracySysTime.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracySysTrace.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/client/TracyThread.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/common" TYPE FILE FILES
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/tracy_lz4.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/tracy_lz4hc.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyAlign.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyAlloc.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyApi.h"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyColor.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyForceInline.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyMutex.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyProtocol.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyQueue.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracySocket.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyStackFrames.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracySystem.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyUwp.hpp"
    "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-src/public/common/TracyYield.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyTargets.cmake"
         "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-build/CMakeFiles/Export/7430802ac276f58e70c46cf34d169c6f/TracyTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/Tracy/TracyTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Tracy" TYPE FILE FILES "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-build/CMakeFiles/Export/7430802ac276f58e70c46cf34d169c6f/TracyTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Tracy" TYPE FILE FILES "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-build/CMakeFiles/Export/7430802ac276f58e70c46cf34d169c6f/TracyTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Tracy" TYPE FILE FILES "C:/Users/aashi/Documents/Dev/Graphics/KenomaEngine/cmake-build-debug/_deps/tracy-build/TracyConfig.cmake")
endif()

