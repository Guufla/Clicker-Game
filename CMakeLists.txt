cmake_minimum_required(VERSION 3.20)

project(clickGame LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(PREPEND CMAKE_PREFIX_PATH
    "C:/msys64/ucrt64"
)

# Find libraries installed through MSYS2 UCRT64.
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# All normal game .cpp files inside src/.
file(GLOB GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Dear ImGui core files.
file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/*.cpp"
)

# SoLoud source files.
file(GLOB SOLOUD_CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/soloud/core/*.cpp"
)

file(GLOB SOLOUD_WAV_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/soloud/audiosource/wav/*.cpp"
)

file(GLOB SOLOUD_MINIAUDIO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/soloud/backend/miniaudio/*.cpp"
)

add_executable(clickGame
    ${GAME_SOURCES}

    # GLAD
    src/glad.c

    # Dear ImGui
    ${IMGUI_SOURCES}
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp

    # SoLoud
    ${SOLOUD_CORE_SOURCES}
    ${SOLOUD_WAV_SOURCES}
    ${SOLOUD_MINIAUDIO_SOURCES}
    src/soloud/audiosource/wav/stb_vorbis.c
)

target_compile_definitions(clickGame PRIVATE
    WITH_MINIAUDIO
)

target_include_directories(clickGame PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/soloud"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends"
)

target_link_directories(clickGame PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

target_link_libraries(clickGame PRIVATE
    glfw
    Freetype::Freetype
    assimp::assimp
    OpenGL::GL
    winmm
)

# Copy the resources folder beside the executable.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources")
    add_custom_command(
        TARGET clickGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:clickGame>/resources"
    )

add_custom_command(
    TARGET clickGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:clickGame>/resources"
)
endif()