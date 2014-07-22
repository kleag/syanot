project(syanot)

cmake_minimum_required(VERSION 2.8)

enable_testing()

set(CMAKE_MODULE_PATH 
  ${CMAKE_MODULE_PATH}
  ${CMAKE_SOURCE_DIR}/cmake/modules
  ${CMAKE_SOURCE_DIR}/skanlite/cmake/modules
)

# search packages used by KDE
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)
include(MacroOptionalDependPackage)


set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

# are these two really required ?
find_package(ZLIB REQUIRED)
if (APPLE)
  find_package(Carbon REQUIRED)
endif (APPLE)
#find_package(KdepimLibs REQUIRED)
#macro_optional_find_package(GraphViz)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DQT3_SUPPORT)

remove_definitions(-DQT_NO_STL) 

include_directories (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})


configure_file (config-syanot.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-syanot.h )

add_subdirectory(src)

install(FILES)

########### packaging ###############
include(SystemSpecificInformations.cmake)
include(InstallRequiredSystemLibraries)

set(CPACK_MONOLITHIC_INSTALL 1)
set(CPACK_PACKAGE_NAME "syanot")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A natural language syntactic parses editor")
set(CPACK_PACKAGE_VENDOR "")
set(CPACK_PACKAGE_CONTACT "Gaël de Chalendar <kleag@free.fr>")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
set(CPACK_STRIP_FILES "")
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
set(CPACK_GENERATOR ${SPECIFIC_SYSTEM_PREFERED_CPACK_GENERATOR})
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  set(CPACK_RPM_PACKAGE_REQUIRES "")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
else (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  set(CPACK_RPM_PACKAGE_REQUIRES "")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
endif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")

include(CPack)
