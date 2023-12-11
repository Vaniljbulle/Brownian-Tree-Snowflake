cmake_minimum_required(VERSION 3.26)
project(Brownian-Tree-Snowflake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXECUTABLE_SUFFIX ".html")

set(EMSCRIPTEN_INCLUDE_DIR "" CACHE PATH "Path to Emscripten include directory")
set(SDL2_INCLUDE_DIR "" CACHE PATH "Path to SDL2 include directory")

if(EMSCRIPTEN_INCLUDE_DIR)
    include_directories(${EMSCRIPTEN_INCLUDE_DIR})
endif()

if(SDL2_INCLUDE_DIR)
    include_directories(${SDL2_INCLUDE_DIR})
endif()

set(EM_FLAGS "")
#set(EM_FLAGS "${EM_FLAGS} -std=c++17 -fsanitize=address --profiling")
set(EM_FLAGS "${EM_FLAGS} --shell-file \"../src/index.html\"")
##set(EM_FLAGS "${EM_FLAGS} -O0 -g -gsource-map --source-map-base http://localhost:8080/build/")
set(EM_FLAGS "${EM_FLAGS} -s USE_SDL=2 -s USE_SDL_TTF=2 -s ALLOW_MEMORY_GROWTH=1")
#set(EM_FLAGS "${EM_FLAGS} -s DEMANGLE_SUPPORT=1 -s INITIAL_MEMORY=256MB -s TOTAL_MEMORY=256MB -s ALLOW_MEMORY_GROWTH=1")

add_executable(BrownianTreeSnowflake
        src/main.cpp
        src/Snowflake.cpp
        include/SnowflakeParticle.h)

target_compile_features(BrownianTreeSnowflake PRIVATE cxx_std_17)
set_target_properties(BrownianTreeSnowflake PROPERTIES LINK_FLAGS ${EM_FLAGS})