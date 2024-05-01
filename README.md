# Space Rider

## Abstract
This project work was given to us as a minor project for our academic session B.E. (Computer) Second Year First Part as prescribed in the syllabus designed by IOE, TU. We developed a 3D game, named SpaceRider, from scratch using C++ and DirectX APIs where the player has to direct a spaceship to dodge the asteroids, planets and all other objects of space and to safely arrive at Earth from any point like Mars or Jupiter. The scoring is based on player’s speed and time it takes to reach the earth.

## Methodology
This project is based on C++ programming language utilizing DirectX API and “Object Oriented Programming” concept. Different classes were created with required number of private and public member data and member functions for smooth running of the program preserving the concept of data hiding. The concept of code reusability, data abstraction were implemented in the project.
We used VisualStudio2019 community as C++ IDE , Windows 10 as target platform and DirectX 11 SDK(already installed) for 3D game engine.
In the program, these classes are used, serving their own purposes:
- SwapChain: This class handles swapchain which is collection of frame buffers that is used to show the render frame on the screen and also used for frame stabilization uses double buffering technique.
- DeviceContext: This class handles device context which is an extension of Directx device that allows us to generate rendering commands to send to the video driver for execution. The driver then redirects the command to the GPU/CPU for final elaboration or final rendering on the screen and also allows us set pipeline states
- VertexShader: This handles the program that executes on the GPU that allows us to transform only the vertices.
- PixelShader: This handles the pixel rendering functions on the screen.
- ConstantBuffer: This is like a vertex buffer but any data can be sent to either pixel shader, or vertex shader or hull shader.
- Math: This class handled transformation matrices viz. translation matrix, rotation matrix, and scale matrix which manipulates the coordinates of coordinate systems we used namely model coordinate space which is the system in which all the original position of 3D model are placed, world coordinate space which is the system in which we plac..

## Project Report
The detailed project report can be found [here](https://github.com/99-NinetyNine/3D-game-Engine-Graphics/blob/main/OOP_Final_Report.pdf).

## Presentation Slide
The presentation slides can be found [here](https://github.com/99-NinetyNine/3D-game-Engine-Graphics/blob/main/OOP%20Presentation.pptx).

## Screenshots
Here are some screenshots of the Space Rider game:

1. ![1](https://github.com/99-NinetyNine/3D-game-Engine-Graphics/blob/main/a.png)
2. ![2](https://github.com/99-NinetyNine/3D-game-Engine-Graphics/blob/main/b.png)
3. ![3](https://github.com/99-NinetyNine/3D-game-Engine-Graphics/blob/main/c.png)

## How to Run
To run the project:
```bash
git clone #link
```
```bash
run in debug mode. ensure directX11 is installed.
```
