cmake_minimum_required(VERSION 3.9)
project(REDSDK)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

add_library(${PROJECT_NAME} STATIC
    src/base64.c
    src/c_list.c
    src/clist.cpp
    src/decorated_string.c
    src/rcp_api.c
    src/rcp_parser2.c
    src/rcp_utils.cpp
    src/strlcat.c
    src/strlcpy.c
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

target_include_directories(${PROJECT_NAME} PUBLIC
                          $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
if(MSVC)
  target_link_libraries(${PROJECT_NAME} PUBLIC
    optimized $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/R3DSDK-2017MD.lib>
    optimized $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/lib/R3DSDK-2017MD.lib>
    debug $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/R3DSDK-2017MDd.lib>
    debug $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/lib/R3DSDK-2017MDd.lib>)
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/libR3DSDKPIC-cpp11.a>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/lib/libR3DSDKPIC-cpp11.a>
      dl)
endif()

if(NOT TARGET uninstall)
  configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in
      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
      IMMEDIATE @ONLY)

  add_custom_target(uninstall
      COMMAND ${CMAKE_COMMAND} -P
      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  export(TARGETS FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake)
endif()

file(WRITE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
  "include(CMakeFindDependencyMacro)\n"
  "find_dependency(Boost COMPONENTS filesystem thread REQUIRED)\n"
  "include(\${CMAKE_CURRENT_LIST_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake)\n")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  FILES_MATCHING PATTERN "*.h")

install(FILES ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
        DESTINATION lib/cmake/${CMAKE_PROJECT_NAME})

if(MSVC)
  install(FILES ${CMAKE_SOURCE_DIR}/lib/R3DSDK-2017MD.lib
          DESTINATION lib
          CONFIGURATIONS Release)
  install(FILES ${CMAKE_SOURCE_DIR}/lib/R3DSDK-2017MDd.lib
          DESTINATION lib
          CONFIGURATIONS Debug)
  install(FILES ${CMAKE_SOURCE_DIR}/bin/REDR3D-x64.dll
          DESTINATION bin)
else()
  install(FILES ${CMAKE_SOURCE_DIR}/lib/libR3DSDKPIC-cpp11.a DESTINATION lib)
  install(FILES ${CMAKE_SOURCE_DIR}/lib/REDR3D-x64.so DESTINATION lib)
endif()

install(EXPORT ${CMAKE_PROJECT_NAME}Targets NAMESPACE red::
        DESTINATION lib/cmake/${CMAKE_PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
