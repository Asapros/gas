cmake_minimum_required(VERSION 4.3)

project(gas CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(EMSCRIPTEN)
    include(FetchContent)

    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )

    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(raylib)
else()
    find_package(raylib REQUIRED)
endif()

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.h")

add_executable(gas ${SRC})

target_compile_options(gas PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(gas PRIVATE raylib)

add_custom_target(run
    COMMAND gas
    DEPENDS gas
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

if (EMSCRIPTEN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
  target_link_options(gas PRIVATE
        "-sEXPORTED_RUNTIME_METHODS=HEAPF32"
  )
  target_link_options(gas PRIVATE
      "--preload-file"
      "${CMAKE_SOURCE_DIR}/assets@assets"
  )

  target_link_options(gas PRIVATE
        "--shell-file"
        "${CMAKE_SOURCE_DIR}/web/shell.html"
    )
endif ()

