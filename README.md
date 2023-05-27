# kaTe-Engine
Simple engine written in moder C++

# Building

Clone the repository, at the moment I only tested building on Linux. 
The project already comes with a CMake file ready to use. 

The following libraries are required on Linux:
    
1. OpenGL

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
    
