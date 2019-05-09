CMAKE_MINIMUM_REQUIRED(VERSION 3.8) 
project(software-rasterizer)

message( ${PROJECT_SOURCE_DIR} )

#TODO(Dan): Mark this if as needed only if VCPKG_ROOT is not defined.
# Support direct users of VCPKG ?
if (WIN32)
	#Decided against setting PREFIX_PATH since people cloning/forking might 
	#be surprised it is changed since it isn't standard practice
	#set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/externals")

	#Set the dirs in which there exist .config files for all used libraries
	set(SDL2_DIR "${PROJECT_SOURCE_DIR}/externals/sdl2/cmake")
endif()


find_package(SDL2 CONFIG REQUIRED)


add_executable( ${PROJECT_NAME}
	code/main.cpp
	code/math/vector2d.h
	code/math/vector3d.h
	code/math/vector4d.h
	code/defs.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/code)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/code/math)

#Libaries installed by vcpkg contain only targets-debug.cmake and targets-release.cmake.
#We want RelWithDebInfo to link to release libraries though, for speed purposes. This 'hack'
#seems to be internally done by vcpkg. Issue: https://github.com/Microsoft/vcpkg/issues/5621
#TODO(DAN): This might not be needed if directly using vcpkg either anymore.
if(WIN32)
	set_target_properties(SDL2::SDL2 SDL2::SDL2main PROPERTIES
		MAP_IMPORTED_CONFIG_MINSIZEREL Release
		MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
		MAP_IMPORTED_CONFIG_Debug Debug 
	)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)

#We need to copy-dlls to build folder on Windows.
if(WIN32)

	#TODO(Dan): Is this useful?
	#set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/externals/bin")

	set(EXTERNAL_BIN_DIR ${PROJECT_SOURCE_DIR}/externals/bin/)
	set(EXTERNAL_BIN_DEBUG ${PROJECT_SOURCE_DIR}/externals/debug/bin)

	#TODO(Dan): Is it better to GLOB all the dlls together?
	#How to do globs with generator expressions?
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	#SDL
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${EXTERNAL_BIN_DEBUG}/sdl2d.dll> $<$<NOT:$<CONFIG:Debug>>:${EXTERNAL_BIN_DIR}/sdl2.dll> $<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
	#Glob alternative:
	#file(GLOB DLLS "${PROJECT_SOURCE_DIR}/externals/bin/*.dll")
	#file(COPY ${DLLS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

endif()
