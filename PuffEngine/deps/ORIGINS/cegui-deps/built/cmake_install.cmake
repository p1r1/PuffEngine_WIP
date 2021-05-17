# Install script for directory: C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/CEGUI-DEPS")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/zlib-1.2.5/CEGUI-BUILD/cmake_install.cmake")
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/libpng-1.4.7/CEGUI-BUILD/cmake_install.cmake")
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/jpeg-8c/CEGUI-BUILD/cmake_install.cmake")
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/freetype-2.4.4/CEGUI-BUILD/cmake_install.cmake")
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/pcre-8.12/CEGUI-BUILD/cmake_install.cmake")
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/expat-2.0.1/CEGUI-BUILD/cmake_install.cmake")
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/SILLY/CEGUI-BUILD/cmake_install.cmake")
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/glew-1.7.0/CEGUI-BUILD/cmake_install.cmake")
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/glfw-2.7.2/CEGUI-BUILD/cmake_install.cmake")
  include("C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/src/glm-0.9.4.5/CEGUI-BUILD/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/Faruk/Documents/Visual Studio 2015/Projects/AKIRAMERO/graphics 1/ZombieGame/deps/ORIGINS/cegui-deps/built/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
