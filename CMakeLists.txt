cmake_minimum_required(VERSION 3.25)

project(Chess)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

add_executable(Chess
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

add_custom_command(TARGET Chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:Chess>/assets
)

add_custom_command(TARGET Chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<IF:$<CONFIG:Debug>,C:/vcpkg/installed/x64-windows/debug/bin/sfml-graphics-d-3.dll,C:/vcpkg/installed/x64-windows/bin/sfml-graphics-3.dll>"
    $<TARGET_FILE_DIR:Chess>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<IF:$<CONFIG:Debug>,C:/vcpkg/installed/x64-windows/debug/bin/sfml-window-d-3.dll,C:/vcpkg/installed/x64-windows/bin/sfml-window-3.dll>"
    $<TARGET_FILE_DIR:Chess>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<IF:$<CONFIG:Debug>,C:/vcpkg/installed/x64-windows/debug/bin/sfml-system-d-3.dll,C:/vcpkg/installed/x64-windows/bin/sfml-system-3.dll>"
    $<TARGET_FILE_DIR:Chess>
)