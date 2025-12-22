cmake_minimum_required(VERSION 3.21)
project(DX11_Lab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ASSIMP_ROOT "C:/dev/External/assimp-master-build")

# 1. 실행 파일 선언
add_executable(DX11_Lab WIN32
    src/main.cpp
    src/App.cpp
    src/App.h
    src/Window/Win32Window.cpp
    src/Window/Win32Window.h
    src/Renderer/DX11Renderer.cpp
    src/Renderer/DX11Renderer.h
    src/Renderer/AssimpLoader.cpp
    src/Renderer/AssimpLoader.h
    src/Renderer/Model.cpp
    src/Renderer/Model.h
    src/Renderer/Mesh.cpp
    src/Renderer/Mesh.h
    src/Renderer/VertexTypes.h
)

# 2. 헤더 포함 경로
target_include_directories(DX11_Lab PRIVATE 
    src
    "${ASSIMP_ROOT}/include"
)

# 3. ImGui 찾기
find_package(imgui CONFIG REQUIRED)

# 4. 라이브러리 경로 설정
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ASSIMP_LIB_FILE "${ASSIMP_ROOT}/lib/Debug/assimp-vc143-mtd.lib")
    set(ZLIB_LIB_FILE "${ASSIMP_ROOT}/contrib/zlib/Debug/zlibstaticd.lib")
else()
    set(ASSIMP_LIB_FILE "${ASSIMP_ROOT}/lib/Release/assimp-vc143-mt.lib")
    set(ZLIB_LIB_FILE "${ASSIMP_ROOT}/contrib/zlib/Release/zlibstatic.lib")
endif()

# 5. 모든 라이브러리 연결
target_link_libraries(DX11_Lab
    PRIVATE
    d3d11
    dxgi
    d3dcompiler
    imgui::imgui
    "${ASSIMP_LIB_FILE}"
    "${ZLIB_LIB_FILE}"
)

# Assimp가 DLL로 빌드되었다면 이 정의가 필요
add_definitions(-DASSIMP_DLL)