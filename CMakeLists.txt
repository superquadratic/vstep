cmake_minimum_required(VERSION 2.8)

project(VSTEP)


# VST Framework

set(VSTSDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vstsdk2.4)
set(VSTSDK_INCLUDE_DIRS ${VSTSDK_DIR})
set(VSTSDK_PLUGIN_SOURCE
  ${VSTSDK_DIR}/public.sdk/source/vst2.x/audioeffectx.cpp
  ${VSTSDK_DIR}/public.sdk/source/vst2.x/audioeffect.cpp
  ${VSTSDK_DIR}/public.sdk/source/vst2.x/vstplugmain.cpp
)

include_directories(${VSTSDK_INCLUDE_DIRS})


# Plugin Source

set(VSTEP_SOURCE
  src/midieventbuffer.h
  src/midieventbuffer.cpp
  src/oscpatternlistener.h
  src/oscpatternlistener.cpp
  src/pattern.h
  src/pattern.cpp
  src/vstep.h
  src/vstep.cpp
  PkgInfo
)

include_directories(src)

set_source_files_properties(PkgInfo PROPERTIES
  MACOSX_PACKAGE_LOCATION .
)


# Build Plugin

set(CMAKE_CXX_FLAGS -m32)
set(CMAKE_LDFLAGS -m32)

add_library(vStep MODULE ${VSTEP_SOURCE} ${VSTSDK_PLUGIN_SOURCE})
target_link_libraries(vStep lo)

set_target_properties(vStep PROPERTIES
  BUNDLE true
  BUNDLE_EXTENSION vst
  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
  MACOSX_BUNDLE_GUI_IDENTIFIER net.superquadratic.vstep
  MACOSX_BUNDLE_BUNDLE_NAME vStep
)


# Test Source

set(UNITTEST_CPP_SOURCE
  vendor/UnitTest++/src/AssertException.cpp
  vendor/UnitTest++/src/Checks.cpp
  vendor/UnitTest++/src/CurrentTest.cpp
  vendor/UnitTest++/src/DeferredTestReporter.cpp
  vendor/UnitTest++/src/DeferredTestResult.cpp
  vendor/UnitTest++/src/MemoryOutStream.cpp
  vendor/UnitTest++/src/ReportAssert.cpp
  vendor/UnitTest++/src/Test.cpp
  vendor/UnitTest++/src/TestDetails.cpp
  vendor/UnitTest++/src/TestList.cpp
  vendor/UnitTest++/src/TestReporter.cpp
  vendor/UnitTest++/src/TestReporterStdout.cpp
  vendor/UnitTest++/src/TestResults.cpp
  vendor/UnitTest++/src/TestRunner.cpp
  vendor/UnitTest++/src/TimeConstraint.cpp
  vendor/UnitTest++/src/XmlTestReporter.cpp
  vendor/UnitTest++/src/Posix/SignalTranslator.cpp
  vendor/UnitTest++/src/Posix/TimeHelpers.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/UnitTest++/src)

set(VSTEP_TESTS
  tests/main.cpp
  tests/oscpatternlistener-tests.cpp
  tests/pattern-tests.cpp
)


# Build Tests

add_executable(vStepTests ${VSTEP_TESTS} ${UNITTEST_CPP_SOURCE} ${VSTEP_SOURCE} ${VSTSDK_PLUGIN_SOURCE})
target_link_libraries(vStepTests lo)
