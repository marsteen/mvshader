# mvshader

Experimental tool for testing shaders from www.shadertoy.com

### Prerequisites development software

g++  
make  
cmake  

### Prerequisites Windows / mingw (64 bit)

Install SDL2 and GLEW development-libraries with pacman:  
``pacman -S mingw-w64-x86_64-glew``  
``pacman -S mingw-w64-x86_64-SDL2``  

### Prerequisites Linux

Install SDL2 and GLEW development-libraries with apt-get:  
``sudo apt-get install libsdl2-dev``__
``sudo apt-get libglew-dev``__

## Building the executable with the command line

- change to project folder: ``cd project``
- create a build folder: ``mkdir build``
- change to build folder: ``cd build``
- call cmake: ``cmake ..``
- make the executable: ``make``

### Coding style

* [Allman style](https://en.wikipedia.org/wiki/Indentation_style#Allman_style)

## Authors

Martin Steen

## License

todo

## Acknowledgments

* [Shadertoy](https://www.shadertoy.com)

