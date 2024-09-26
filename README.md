# Lumi

Lumi is a C++ project designed for rendering intricate 3D scenes with realistic lighting and materials using OpenGL. Lumi provides a modular architecture for scene setup, camera control, and rendering. It leverages modern C++ capabilities and provides a simple GUI using ImGui for real-time interaction.

Basic Diffuse Lighting     |  Cornell Box
:-------------------------:|:-------------------------:
<img width="496" alt="sphere_lit_by_diffuse" src="https://github.com/user-attachments/assets/3a81a962-c323-4cbe-b8ed-f9ba6802fafa"> | <img width="496" alt="cornell_box" src="https://github.com/user-attachments/assets/45c19019-ddaa-4d8f-b4f6-d573612a36de">

This guide will walk you through the setup process for the `lumi` OpenGL Path tracer using CMake and vcpkg for dependency management.

## Prerequisites

- CMake (version 3.15 or newer)
- A C++ compiler with C++20 support
- OpenGL drivers installed on your system

## Clone the Repository

Start by cloning the main project repository to your local machine:

```bash
git clone https://github.com/silvanias/Lumi.git
cd lumi
```

## Set up vcpkg

Create a `vcpkg` directory inside the project and set up vcpkg, a C++ library manager:

```bash
mkdir vcpkg
cd vcpkg
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh # On Windows use .\vcpkg\bootstrap-vcpkg.bat
```

Install the required libraries:

```bash
./vcpkg/vcpkg install glm
./vcpkg/vcpkg install glfw3
./vcpkg/vcpkg install glad
```

Again, on Windows, you would use `.\vcpkg\vcpkg` instead of `./vcpkg/vcpkg`.

## Clone Dear ImGui

Clone the Dear ImGui library inside the `lib` directory of your project:

```bash
mkdir lib
git clone https://github.com/ocornut/imgui.git lib/imgui
```

## Configure CMake

Create a `build` directory and navigate into it:

```bash
mkdir build
cd build
```

Run CMake to generate the build files:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
```

## Build the Application

Within the `build` directory, compile the application:

```bash
cmake --build .
```

## Run the Application

After successfully building the application, execute it with the following command:

```bash
./vender
```

On Windows, you might need to navigate to the directory containing the generated executable and run `vender.exe`.

## Troubleshooting

If you encounter any issues during setup or compilation, ensure that:

- Your CMake and Git versions meet the minimum requirements.
- All the prerequisites are properly installed.
- The vcpkg toolchain file path in the CMake command is correct.

For additional help, refer to the documentation of the respective tools or libraries, or consider reaching out to their communities.

## Usage

Modify main.cpp or any other source files to customize the rendering logic and scene setup. The project is structured to allow easy extension of geometry, materials, and shaders.
