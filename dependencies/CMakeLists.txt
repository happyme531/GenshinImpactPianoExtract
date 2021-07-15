include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY "https://github.com/ArthurSonzogni/FTXUI.git"
)


set(FTXUI_BUILD_DOCS FALSE)
set(FTXUI_BUILD_EXAMPLES FALSE)
set(FTXUI_ENABLE_INSTALL FALSE)


FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  message(STATUS "Fetching FTXUI...(This could take some time)")
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_subdirectory(ArduinoJson)
add_subdirectory(argparse)
add_subdirectory(midifile)

target_include_directories(GenshinImpactPianoExtract PUBLIC argparse/include)
target_include_directories(GenshinImpactPianoExtract PUBLIC ArduinoJson)
target_include_directories(GenshinImpactPianoExtract PUBLIC ArduinoJson/src)
target_include_directories(GenshinImpactPianoExtract PUBLIC midifile/include)
