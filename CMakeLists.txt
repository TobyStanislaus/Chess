cmake_minimum_required(VERSION 3.25)

project(Chess)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

find_package(SFML COMPONENTS Graphics Window System REQUIRED)

# --------------------------------------------------------------------------
# ONNX Runtime location
#
# ONNX Runtime isn't managed by vcpkg here, so its location must be told to
# CMake explicitly. Set this either:
#   - via the CMakePresets.json cacheVariables (reads $env{ONNXRUNTIME_DIR}), or
#   - by setting an ONNXRUNTIME_DIR environment variable before configuring, or
#   - by passing -DONNXRUNTIME_DIR=... directly to cmake.
#
# It should point at the extracted ONNX Runtime release folder, i.e. the
# directory that directly contains "include/" and "lib/".
# --------------------------------------------------------------------------
if(NOT DEFINED ONNXRUNTIME_DIR OR ONNXRUNTIME_DIR STREQUAL "")
    if(DEFINED ENV{ONNXRUNTIME_DIR})
        set(ONNXRUNTIME_DIR "$ENV{ONNXRUNTIME_DIR}")
    endif()
endif()

if(NOT DEFINED ONNXRUNTIME_DIR OR ONNXRUNTIME_DIR STREQUAL "")
    message(FATAL_ERROR
        "ONNXRUNTIME_DIR is not set.\n"
        "Download an ONNX Runtime win-x64 release from "
        "https://github.com/microsoft/onnxruntime/releases, extract it, and "
        "either set an ONNXRUNTIME_DIR environment variable pointing at the "
        "extracted folder, or pass -DONNXRUNTIME_DIR=<path> to CMake.\n"
        "See BUILDING.md for details."
    )
endif()

if(NOT EXISTS "${ONNXRUNTIME_DIR}/include" OR NOT EXISTS "${ONNXRUNTIME_DIR}/lib")
    message(FATAL_ERROR
        "ONNXRUNTIME_DIR is set to '${ONNXRUNTIME_DIR}' but it doesn't contain "
        "the expected include/ and lib/ subfolders. Check the path."
    )
endif()

add_executable(Chess WIN32
    src/main.cpp
    src/Board.cpp
    src/Game.cpp
    src/NNEvaluator.cpp

    src/pieces/Bishop.cpp
    src/pieces/King.cpp
    src/pieces/Knight.cpp
    src/pieces/Pawn.cpp
    src/pieces/Piece.cpp
    src/pieces/Queen.cpp
    src/pieces/Rook.cpp

    ${SOURCES}
    chess.rc
)
target_link_options(Chess PRIVATE "/ENTRY:mainCRTStartup")

target_include_directories(Chess PRIVATE
    src
    src/pieces
    ${ONNXRUNTIME_DIR}/include
)

target_link_libraries(Chess PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

target_link_directories(Chess PRIVATE
    ${ONNXRUNTIME_DIR}/lib
)

target_link_libraries(Chess PRIVATE
    onnxruntime
)

# Copy game assets next to the built executable
add_custom_command(TARGET Chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:Chess>/assets
)

# onnxruntime isn't managed by vcpkg, so it's not covered by
# VCPKG_APPLOCAL_DEPS auto-copying — copy its DLLs manually.
add_custom_command(TARGET Chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${ONNXRUNTIME_DIR}/lib/onnxruntime.dll
    $<TARGET_FILE_DIR:Chess>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${ONNXRUNTIME_DIR}/lib/onnxruntime_providers_shared.dll
    $<TARGET_FILE_DIR:Chess>
)

# NOTE: SFML and its transitive dependencies (freetype, libpng, zlib,
# brotli, bz2, etc.) are copied automatically next to Chess.exe by
# vcpkg's applocal deps feature (VCPKG_APPLOCAL_DEPS, on by default)
# as long as you configure with the vcpkg toolchain file — see
# CMakePresets.json, which does this and also auto-installs SFML itself
# via vcpkg manifest mode (vcpkg.json in the repo root).