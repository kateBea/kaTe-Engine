# kaTe-Engine
Simple engine written in moder C++

![kaTe Engine](assets/image/img1.png)

# Building

Clone the repository, at the moment I only tested building on Linux. 
The project already comes with a CMake file ready to use. 

The following libraries are required on Linux:
    
1. OpenGL

This project is done thanks to various third-party libraries:

1. [FMT (Modern formatting library)](https://github.com/fmtlib/fmt)
2. [GLEW (Open GL extension Wrangler)](https://glew.sourceforge.net/)
3. [GLFW (Multiplatform Library for Window, Event handling, etc.)](https://github.com/glfw/glfw)
4. [GLM (Open GL Mathematics Library for C++)](https://github.com/g-truc/glm)
5. [ImGui (Graphical User interface Library for C++)](https://github.com/ocornut/imgui)
6. [Spdlog (Fast C++ Logging Library)](https://github.com/gabime/spdlog)

The following tools are required on Linux:

1. G++ (GNU C++ Compiler)
2. CMake (Cross platform build system)

The GLFW library is not necessary to be installed on the system since it 
is included as a submodule and build along with the project.

- Clone the repository
    ```shell
    git clone --recursive https://github.com/kateBea/kaTe-Engine.git && cd kaTe-Engine
    ```
- Create a build directory
    ```shell
    mkdir build && cd build
    ```

- Run CMake
    ```shell
    cmake ..
    ```
  
- Build project using CMake
    ```shell
    cmake --build .
    ```
    
