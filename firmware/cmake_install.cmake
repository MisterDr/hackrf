# Install script for directory: /Volumes/mrdr/research/hackrf/firmware

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/djenadrazic/arm-cs-tools/arm-none-eabi")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Volumes/mrdr/research/hackrf/firmware/blinky/cmake_install.cmake")
  include("/Volumes/mrdr/research/hackrf/firmware/mixertx/cmake_install.cmake")
  include("/Volumes/mrdr/research/hackrf/firmware/sgpio/cmake_install.cmake")
  include("/Volumes/mrdr/research/hackrf/firmware/sgpio-rx/cmake_install.cmake")
  include("/Volumes/mrdr/research/hackrf/firmware/simpletx/cmake_install.cmake")
  include("/Volumes/mrdr/research/hackrf/firmware/startup/cmake_install.cmake")
  include("/Volumes/mrdr/research/hackrf/firmware/startup_systick/cmake_install.cmake")
  include("/Volumes/mrdr/research/hackrf/firmware/startup_systick_perfo/cmake_install.cmake")
  include("/Volumes/mrdr/research/hackrf/firmware/hackrf_usb/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Volumes/mrdr/research/hackrf/firmware/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
