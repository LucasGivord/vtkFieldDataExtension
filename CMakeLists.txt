cmake_minimum_required(VERSION 3.26)
project(vtkFieldDatas VERSION 0.1.0)

## Config

set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_TESTING "Enable testing suite" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

## Deps
set(ENABLE_TESTS OFF)
if (BUILD_TESTING)
  # XXX: vtk macro for scanning module cannot take False/True value but only OFF/ON,
  # we use an intermediary variable here as workaround
  set(ENABLE_TESTS ON)
  include(CTest)
  enable_testing()

  # Enable catch2 as test framework
  Include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.6.0
  )
  
  FetchContent_MakeAvailable(Catch2)
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
  include(Catch)

endif()

## VTK Module

find_package(VTK
  COMPONENTS
    CommonCore
    CommonDataModel
  OPTIONAL_COMPONENTS
    CommonSystem
    TestingCore)

vtk_module_find_modules(vtk_field_data_module_files "${CMAKE_CURRENT_SOURCE_DIR}/Filters")

vtk_module_scan(
  MODULE_FILES     ${vtk_field_data_module_files}
  PROVIDES_MODULES vtk_field_data_provided_modules
  WANT_BY_DEFAULT  ON
  ENABLE_TESTS     ${ENABLE_TESTS})

vtk_module_build(
  MODULES         ${vtk_field_data_provided_modules}
  INSTALL_EXPORT  vtk_field_data_module
  INSTALL_HEADERS ON
  CMAKE_DESTINATION   "${CMAKE_INSTALL_LIBDIR}/cmake/vtk_field_data_module"
  HEADERS_DESTINATION "include")
