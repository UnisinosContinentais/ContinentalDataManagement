cmake_minimum_required(VERSION 3.5)
project(ContinentalDataManagement VERSION 1.0.0 LANGUAGES CXX)

add_definitions(-DMAKE_CONTINENTALDATAMANAGEMENT_LIB)
add_definitions(-D_USE_MATH_DEFINES)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

#Make sure that custom modules like FindRapidJSON are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

##############################################
# Declare dependencies
find_package(Qt5Core CONFIG REQUIRED)

##############################################
# Create target and set properties

add_library(ContinentalDataManagement
    SHARED
    src/ResampleData.cpp
    src/SpatialAnalyst.cpp
    src/Raster.cpp
    src/RasterFile.cpp
    src/ReclassParameters.cpp
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(ContinentalDataManagement::ContinentalDataManagement ALIAS ContinentalDataManagement)

set_target_properties(ContinentalDataManagement PROPERTIES DEBUG_POSTFIX "_d")

#Set target properties
target_include_directories(ContinentalDataManagement
    PUBLIC
       $<INSTALL_INTERFACE:include>
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(ContinentalDataManagement PRIVATE cxx_auto_type)
target_compile_options(ContinentalDataManagement PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

target_link_libraries(ContinentalDataManagement
#    PUBLIC
#        Boost::boost RapidJSON::RapidJSON
#    PRIVATE
#        Boost::regex
    PUBLIC Qt5::Core
)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/ContinentalDataManagement)

install(TARGETS ContinentalDataManagement
    EXPORT ContinentalDataManagement-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION "lib"
)

#This is required so that the exported target has the name ContinentalDataManagement and not ContinentalDataManagement
set_target_properties(ContinentalDataManagement PROPERTIES EXPORT_NAME ContinentalDataManagement)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT ContinentalDataManagement-targets
  FILE
    ContinentalDataManagementTargets.cmake
  NAMESPACE
    ContinentalDataManagement::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalDataManagementConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/ContinentalDataManagementConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalDataManagementConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
#     ${CMAKE_CURRENT_LIST_DIR}/cmake/FindRapidJSON.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalDataManagementConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalDataManagementConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree
# configure_file(
#     ${CMAKE_CURRENT_LIST_DIR}/cmake/FindRapidJSON.cmake
#     ${CMAKE_CURRENT_BINARY_DIR}/FindRapidJSON.cmake
#     COPYONLY)

export(EXPORT ContinentalDataManagement-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/ContinentalDataManagementTargets.cmake NAMESPACE ContinentalDataManagement::)

# add_subdirectory(test)
