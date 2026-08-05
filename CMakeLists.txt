cmake_minimum_required(VERSION 3.25)

project(Chess)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

find_package(SFML COMPONENTS Graphics Window System REQUIRED)

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
)
target_link_options(Chess PRIVATE "/ENTRY:mainCRTStartup")

target_include_directories(Chess PRIVATE
    src
    src/pieces
    C:/vcpkg/installed/x64-windows/include
    C:/libs/onnxruntime-win-x64-1.28.0/include
)

target_link_libraries(Chess PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

target_link_directories(Chess PRIVATE
    C:/libs/onnxruntime-win-x64-1.28.0/lib
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
    C:/libs/onnxruntime-win-x64-1.28.0/lib/onnxruntime.dll
    $<TARGET_FILE_DIR:Chess>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    C:/libs/onnxruntime-win-x64-1.28.0/lib/onnxruntime_providers_shared.dll
    $<TARGET_FILE_DIR:Chess>
)

# NOTE: SFML and its transitive dependencies (freetype, libpng, zlib,
# brotli, bz2, etc.) are copied automatically next to Chess.exe by
# vcpkg's applocal deps feature (VCPKG_APPLOCAL_DEPS, on by default)
# — but only if you configure with the vcpkg toolchain file, e.g.:
#
#   cmake -B build-release -G Ninja -DCMAKE_BUILD_TYPE=Release ^
#         -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake