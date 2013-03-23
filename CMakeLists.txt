cmake_minimum_required(VERSION 2.8)

project(VSTEP)

set(VSTSDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vstsdk2.4)
set(VSTSDK_INCLUDE_DIRS ${VSTSDK_DIR})
set(VSTSDK_PLUGIN_SOURCE
  ${VSTSDK_DIR}/public.sdk/source/vst2.x/audioeffectx.cpp
  ${VSTSDK_DIR}/public.sdk/source/vst2.x/audioeffect.cpp
  ${VSTSDK_DIR}/public.sdk/source/vst2.x/vstplugmain.cpp
)

include_directories(${VSTSDK_INCLUDE_DIRS})

set(VSTEP_SOURCE
  src/vstep.h
  src/vstep.cpp
  PkgInfo
)

set_source_files_properties(PkgInfo PROPERTIES
  MACOSX_PACKAGE_LOCATION .
)

set(CMAKE_CXX_FLAGS -m32)
set(CMAKE_LDFLAGS -m32)

add_library(vStep MODULE ${VSTEP_SOURCE} ${VSTSDK_PLUGIN_SOURCE})
set_target_properties(vStep PROPERTIES
  BUNDLE true
  BUNDLE_EXTENSION vst
  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
  MACOSX_BUNDLE_GUI_IDENTIFIER net.superquadratic.vstep
  MACOSX_BUNDLE_BUNDLE_NAME vStep
)
