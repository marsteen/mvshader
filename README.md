# mvshader

Experimental tool for testing shaders from www.shadertoy.com

### Prerequisites development tools

g++  
make  
cmake  

### Prerequisites Windows / mingw (64 bit)

Install SDL2 and GLEW development-libraries with pacman:  
``pacman -S mingw-w64-x86_64-glew``  
``pacman -S mingw-w64-x86_64-SDL2``  

### Prerequisites Linux

Install SDL2 and GLEW development-libraries with apt-get:  
``sudo apt-get install libsdl2-dev``  
``sudo apt-get install libglew-dev``  

### Building the executable with the command line

- change to project folder: ``cd project``
- create a build folder: ``mkdir build``
- change to build folder: ``cd build``
- call cmake: ``cmake ..``
- make the executable: ``make``

### Run the executable (examples)

From the ``build``-folder:  
* ``./mvshader``
* ``./mvshader -fshader ../shaders/frag/01.glsl``
* ``./mvshader -vshader ../shaders/vert/01.glsl -fshader ../shaders/frag/01.glsl``__
  
  
From the ``project`` folder  
* ./run-example.sh 01  
* ./run-example.sh 02  
* ..  
* ./run-example.sh 25__

### Coding style

* [Allman style](https://en.wikipedia.org/wiki/Indentation_style#Allman_style)
* 4 spaces, no tabs

## Authors

Martin Steen

## License

todo

## Acknowledgments

* [Shadertoy](https://www.shadertoy.com)

