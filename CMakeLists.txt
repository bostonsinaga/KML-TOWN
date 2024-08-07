project (kmltown)
cmake_minimum_required (VERSION 3.1.0)

find_package (GeographicLib REQUIRED)
include_directories (${GeographicLib_INCLUDE_DIRS})

if (NOT MSVC)
  set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

add_executable (${PROJECT_NAME} test.cpp)
target_link_libraries (${PROJECT_NAME} ${GeographicLib_LIBRARIES})

if (MSVC)
  get_target_property (_LIBTYPE ${GeographicLib_LIBRARIES} TYPE)
  if (_LIBTYPE STREQUAL "SHARED_LIBRARY")
    # On Windows systems, copy the shared library to build directory
    add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E
      copy $<TARGET_FILE:${GeographicLib_LIBRARIES}> ${CMAKE_CFG_INTDIR}
      COMMENT "Copying shared library for GeographicLib")
  endif ()
endif () 