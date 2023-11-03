# OpenGL_GP1
This repo is for my project in subject Graphics Programming using C++ and OpenGL to generate graphical images in 2D and 3D.

First exercise in this subject is to create Menger Sponge using OpenGL, control the rotation axis in X and Y, and also change the color of the cube using ImGUI.

Tutorial, which I was using, is on the official site of OpenGL called LearnOpenGL.

## What's in this project
To build this project, you need to enter the building command for cmake:
```c++
cmake -B build
```
After that, you can load your project in any IDE (VS Code, CLion etc.) and build it.

This exercise concentrated to creating a Menger Sponge, which is recursively made using a function. You can change the level of the recursion using the ImGui Slider.
Also, you can change the perspective of the cube using sliders to change the rotation on X and Y axis. Additionally, you can zoom in or zoom out of the cube.
Color Picker gives you the ability to change the color of the cube, as well as textures that are on the cube.

