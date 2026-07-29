cmake_minimum_required(VERSION 3.20)

project(clickGame LANGUAGES C CXX)

# --------------------------------------------------
# Language settings
# --------------------------------------------------

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allow CMake to find libraries installed through MSYS2 UCRT64.
list(PREPEND CMAKE_PREFIX_PATH
    "C:/msys64/ucrt64"
)

# --------------------------------------------------
# Installed dependencies
# --------------------------------------------------

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

# --------------------------------------------------
# RmlUi
# --------------------------------------------------

# RmlUi should be located here:
# external/RmlUi/CMakeLists.txt
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/RmlUi/CMakeLists.txt")
    message(FATAL_ERROR
        "RmlUi was not found.\n"
        "Expected: ${CMAKE_CURRENT_SOURCE_DIR}/external/RmlUi/CMakeLists.txt"
    )
endif()

add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/external/RmlUi"
    "${CMAKE_CURRENT_BINARY_DIR}/RmlUi"
)

# RmlUi recommends compiling the matching platform and renderer
# backend files directly into the application.
set(RMLUI_BACKEND_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/external/RmlUi/Backends"
)

add_library(rmlui_backend_glfw_gl3 INTERFACE)

target_sources(rmlui_backend_glfw_gl3 INTERFACE
    "${RMLUI_BACKEND_PATH}/RmlUi_Platform_GLFW.cpp"
    "${RMLUI_BACKEND_PATH}/RmlUi_Renderer_GL3.cpp"
)

target_include_directories(rmlui_backend_glfw_gl3 INTERFACE
    "${RMLUI_BACKEND_PATH}"
)

# Tell RmlUi's OpenGL 3 renderer to use the GLAD loader
# already included in your game.
target_compile_definitions(rmlui_backend_glfw_gl3 INTERFACE
    "RMLUI_GL3_CUSTOM_LOADER=<glad/glad.h>"
)

target_link_libraries(rmlui_backend_glfw_gl3 INTERFACE
    glfw
    OpenGL::GL
)

# --------------------------------------------------
# Game source files
# --------------------------------------------------

file(GLOB GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# --------------------------------------------------
# Dear ImGui source files
# --------------------------------------------------

file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/*.cpp"
)

set(IMGUI_BACKEND_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp"
)

# --------------------------------------------------
# SoLoud source files
# --------------------------------------------------

file(GLOB SOLOUD_CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/soloud/core/*.cpp"
)

file(GLOB SOLOUD_WAV_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/soloud/audiosource/wav/*.cpp"
)

file(GLOB SOLOUD_MINIAUDIO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/soloud/backend/miniaudio/*.cpp"
)

# --------------------------------------------------
# Executable
# --------------------------------------------------

add_executable(clickGame
    ${GAME_SOURCES}

    # GLAD implementation
    "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c"

    # Dear ImGui
    ${IMGUI_SOURCES}
    ${IMGUI_BACKEND_SOURCES}

    # SoLoud
    ${SOLOUD_CORE_SOURCES}
    ${SOLOUD_WAV_SOURCES}
    ${SOLOUD_MINIAUDIO_SOURCES}

    "${CMAKE_CURRENT_SOURCE_DIR}/src/soloud/audiosource/wav/stb_vorbis.c"
)

# --------------------------------------------------
# Compile definitions
# --------------------------------------------------

target_compile_definitions(clickGame PRIVATE
    WITH_MINIAUDIO
)

# --------------------------------------------------
# Include directories
# --------------------------------------------------

target_include_directories(clickGame PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/soloud"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends"
)

# Keep this only if you have locally stored .a libraries
# inside Clicker-Game/lib.
target_link_directories(clickGame PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# --------------------------------------------------
# Link libraries
# --------------------------------------------------

target_link_libraries(clickGame PRIVATE
    # RmlUi core and backend
    RmlUi::RmlUi
    rmlui_backend_glfw_gl3

    # Existing game dependencies
    glfw
    Freetype::Freetype
    assimp::assimp
    JsonCpp::JsonCpp
    OpenGL::GL
    winmm
)

# --------------------------------------------------
# Copy resources beside the executable
# --------------------------------------------------

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources")
    add_custom_command(
        TARGET clickGame
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:clickGame>/resources"

        COMMENT "Copying game resources"
    )
endif()