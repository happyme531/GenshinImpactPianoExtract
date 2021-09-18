cmake_minimum_required(VERSION 3.16.0)
project(GenshinImpactPianoExtract VERSION 0.1.1)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
set(OpenCV_DIR D:/Downloads/opencv/build/)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})



if(WIN32)
add_definitions(-D_HAS_STD_BYTE=0)
add_compile_options(/utf-8)
endif()

if(CMAKE_BUILD_TYPE)
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
  if(WIN32)
  add_compile_options(/fsanitize=address)
  else()
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
  endif()
  endif()
endif()


aux_source_directory(src GenshinImpactPianoExtract_SRC)
add_executable(GenshinImpactPianoExtract ${GenshinImpactPianoExtract_SRC})


add_subdirectory(dependencies)

find_package(OpenCV REQUIRED)

target_include_directories(GenshinImpactPianoExtract PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(GenshinImpactPianoExtract PUBLIC ${ftxui_INCLUDE_DIRS})
target_include_directories(GenshinImpactPianoExtract PUBLIC dependencies)
target_include_directories(GenshinImpactPianoExtract PUBLIC src)

configure_file(${PROJECT_SOURCE_DIR}/src/buildconfig.h.in ${PROJECT_SOURCE_DIR}/src/buildconfig.h)

target_link_libraries(GenshinImpactPianoExtract PRIVATE ${OpenCV_LIBS})

target_link_libraries(GenshinImpactPianoExtract
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component 
  PRIVATE midifile
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(GenshinImpactPianoExtract PRIVATE OpenMP::OpenMP_CXX)
endif()
set_target_properties(GenshinImpactPianoExtract PROPERTIES CXX_STANDARD 20)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
