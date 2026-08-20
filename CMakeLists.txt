cmake_minimum_required(VERSION 3.25)

project(Chess)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------

find_package(SFML COMPONENTS Graphics Window System REQUIRED)

# ONNX Runtime is supplied as a prebuilt release in third_party/onnxruntime.
# The directory must contain:
#
#   third_party/onnxruntime/
#   ├── include/
#   └── lib/
#
# This avoids requiring a machine-specific ONNXRUNTIME_DIR environment
# variable or a separate vcpkg build of ONNX Runtime.
set(ONNXRUNTIME_DIR
    "${CMAKE_SOURCE_DIR}/third_party/onnxruntime"
)

if(NOT EXISTS "${ONNXRUNTIME_DIR}/include")
    message(FATAL_ERROR
        "ONNX Runtime include directory was not found.\n"
        "Expected: ${ONNXRUNTIME_DIR}/include\n"
        "Download the Windows x64 ONNX Runtime release and extract it to:\n"
        "${ONNXRUNTIME_DIR}"
    )
endif()

if(NOT EXISTS "${ONNXRUNTIME_DIR}/lib")
    message(FATAL_ERROR
        "ONNX Runtime lib directory was not found.\n"
        "Expected: ${ONNXRUNTIME_DIR}/lib\n"
        "Download the Windows x64 ONNX Runtime release and extract it to:\n"
        "${ONNXRUNTIME_DIR}"
    )
endif()

# --------------------------------------------------------------------------
# Executable
# --------------------------------------------------------------------------

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

    chess.rc
)

target_link_options(Chess PRIVATE "/ENTRY:mainCRTStartup")

# --------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------

target_include_directories(Chess PRIVATE
    src
    src/pieces
    "${ONNXRUNTIME_DIR}/include"
)

# --------------------------------------------------------------------------
# Libraries
# --------------------------------------------------------------------------

target_link_libraries(Chess PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

target_link_directories(Chess PRIVATE
    "${ONNXRUNTIME_DIR}/lib"
)

target_link_libraries(Chess PRIVATE
    onnxruntime
)

# --------------------------------------------------------------------------
# Copy assets
# --------------------------------------------------------------------------

add_custom_command(TARGET Chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:Chess>/assets"
)

# --------------------------------------------------------------------------
# Copy ONNX Runtime DLLs
# --------------------------------------------------------------------------

add_custom_command(TARGET Chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${ONNXRUNTIME_DIR}/lib/onnxruntime.dll"
    "$<TARGET_FILE_DIR:Chess>"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${ONNXRUNTIME_DIR}/lib/onnxruntime_providers_shared.dll"
    "$<TARGET_FILE_DIR:Chess>"
)