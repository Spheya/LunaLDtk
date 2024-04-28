cmake_minimum_required (VERSION 3.8)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("LunaLDtk")

add_library(lunaldtk INTERFACE)
target_include_directories(lunaldtk INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")

add_subdirectory("external")
target_link_libraries(lunaldtk INTERFACE luna)
target_link_libraries(lunaldtk INTERFACE LDtkLoader)

install(FILES LunaLDTK.hpp DESTINATION .)

option(LUNALDTK_BUILD_EXAMPLE "Build the LunaLDtk example program" ON)
if(LUNALDTK_BUILD_EXAMPLE)
	add_subdirectory ("example")
endif()