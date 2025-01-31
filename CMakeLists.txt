cmake_minimum_required(VERSION 3.16)
PROJECT(ChimeraTK-DeviceAccess-TangoBackend)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

set(${PROJECT_NAME}_MAJOR_VERSION 00)
set(${PROJECT_NAME}_MINOR_VERSION 01)
set(${PROJECT_NAME}_PATCH_VERSION 00)
include(cmake/set_version_numbers.cmake)

include(cmake/set_default_build_to_release.cmake)
include(cmake/set_default_flags.cmake)
include(cmake/enable_code_coverage_report.cmake)

FIND_PACKAGE(ChimeraTK-DeviceAccess 03.11 REQUIRED)
FIND_PACKAGE(Boost COMPONENTS thread unit_test_framework REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
FIND_PACKAGE(PkgConfig REQUIRED)

PKG_CHECK_MODULES(Tango REQUIRED IMPORTED_TARGET tango)
PKG_CHECK_MODULES(Json REQUIRED IMPORTED_TARGET nlohmann_json)

set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include/)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# C++ library
aux_source_directory(${CMAKE_SOURCE_DIR}/src library_sources)
file(GLOB headers "${CMAKE_SOURCE_DIR}/include/*.h")
add_library(${PROJECT_NAME} SHARED ${library_sources} ${headers})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_FULL_LIBRARY_VERSION} SOVERSION ${${PROJECT_NAME}_SOVERSION})
target_link_libraries(${PROJECT_NAME}
  PUBLIC ChimeraTK::ChimeraTK-DeviceAccess
  PRIVATE PkgConfig::Tango)

# --no-as-needed: force linking against this library. This is required for proper registering.
target_link_options(${PROJECT_NAME} PUBLIC "-Wl,--no-as-needed")

# Enable documentation
include(cmake/enable_doxygen_documentation.cmake)

# Install the library and the executables
# this defines architecture-dependent ${CMAKE_INSTALL_LIBDIR}
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

option(BUILD_TESTS "Build test programs" ON)

if(BUILD_TESTS)
  enable_testing()

  file(GLOB testServerSources "${CMAKE_SOURCE_DIR}/tests/generated/*.cpp")
  file(GLOB testServerHeaders "${CMAKE_SOURCE_DIR}/tests/generated/*.h")

  # Compile an object library for the Tango test server
  add_executable(tangoTestServer ${testServerSources} ${testServerHeaders})
  target_link_libraries(tangoTestServer PRIVATE PkgConfig::Tango)

  aux_source_directory(${CMAKE_SOURCE_DIR}/tests/executables_src testExecutables)

  file(GLOB testLibSources "${CMAKE_SOURCE_DIR}/tests/lib/*.cpp")
  file(GLOB testLibHeaders "${CMAKE_SOURCE_DIR}/tests/lib/*.h")
  add_library(testLibrary OBJECT ${testLibSources} ${testLibHeaders})
  target_link_libraries(testLibrary PRIVATE PkgConfig::Tango)

  foreach(testExecutableSrcFile ${testExecutables})
    # NAME_WE means the base name without path and (longest) extension
    get_filename_component(excutableName ${testExecutableSrcFile} NAME_WE)

    add_executable(${excutableName} $<TARGET_OBJECTS:testLibrary> ${testExecutableSrcFile})
    target_include_directories(${excutableName} PRIVATE "${CMAKE_SOURCE_DIR}/tests/lib")
    target_link_libraries(${excutableName}
      PRIVATE ChimeraTK::ChimeraTK-DeviceAccess PkgConfig::Tango ${PROJECT_NAME} Boost::unit_test_framework)
    add_test(${excutableName} ${excutableName})
  endforeach(testExecutableSrcFile)

  file(COPY tests/unifiedBackendTestCache.json DESTINATION ${CMAKE_BINARY_DIR})
endif()

# we support our cmake EXPORTS as imported targets
set(PROVIDES_EXPORTED_TARGETS 1)

# we need the public dependencies so create_cmake_config_files can find them as implicit dependencies
list(APPEND ${PROJECT_NAME}_PUBLIC_DEPENDENCIES "ChimeraTK-DeviceAccess")
include(${CMAKE_SOURCE_DIR}/cmake/create_cmake_config_files.cmake)
