cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0144 NEW)

project(verylatega VERSION 0.1.0)

find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if(NOT verilator_FOUND)
    message(
        FATAL_ERROR
        "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable"
    )
endif()

set(VERBOSE 1)

#add_library(verylatega SHARED ./src/VerylateGA.cpp)

add_executable(verylatega ./src/main.cpp)


# add_subdirectory(glad)
# target_link_libraries(verylatega  PUBLIC glad)

target_compile_features(verylatega PUBLIC cxx_std_14)



option(BUILD_SHARED_LIBS "Build shared library" OFF)

message(STATUS ${CMAKE_SYSTEM_NAME})

if(${CMAKE_SYSTEM_NAME} MATCHES  "Emscripten")
    set_target_properties(verylatega PROPERTIES OUTPUT_NAME "index")
    set_target_properties(verylatega PROPERTIES SUFFIX ".html")
    target_link_options(verylatega PUBLIC --bind -O3  -DNDEBUG 
    -sINITIAL_MEMORY=1GB -sMAX_WEBGL_VERSION=2 -sFETCH=1 -sWASM_BIGINT=1
    -sSTACK_SIZE=5MB -sASYNCIFY_IMPORTS=qt_asyncify_suspend_js,qt_asyncify_resume_js --use-preload-plugins
    -sERROR_ON_UNDEFINED_SYMBOLS=1 -sUSE_SDL=2 
    # -sOFFSCREEN_FRAMEBUFFER=1 -pthread -lpthread -sPROXY_TO_PTHREAD=1 -sASSERTIONS=2 
    --embed-file ./assets --pre-js ${CMAKE_CURRENT_SOURCE_DIR}/preconfig.js
    )
    target_compile_options(verylatega PUBLIC -O3
    #  -pthread 
    -DVL_IGNORE_UNKNOWN_ARCH -sUSE_SDL=2 )
else()
    find_package(SDL2 REQUIRED)
    target_link_libraries(verylatega PUBLIC  
        ${SDL2_LIBRARIES}
    )
    include_directories(verylatega ${SDL2_INCLUDE_DIRS})
    # find_package(OpenGL REQUIRED)
    # target_link_libraries(verylatega PUBLIC  
    #     glfw
    #     OpenGL
    # )
endif()



# Add the Verilated circuit to the target
verilate(verylatega 
  OPT_GLOBAL -O3 -fstrict-aliasing
  OPT_SLOW  -O3 -fstrict-aliasing
  OPT_FAST -O3 -fstrict-aliasing
  INCLUDE_DIRS ../VeryMaze/designs
  VERILATOR_ARGS -O3 --x-assign fast --x-initial fast --noassert --threads 1
  SOURCES ../VeryMaze/designs/veryga.v
)

target_include_directories(verylatega
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(verylatega PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



# Create a new executable target that will contain all your sources
