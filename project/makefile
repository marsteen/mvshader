#**********************************************************************
#
# makefile
#
# written 2012 by Martin Steen
# martin@martin-steen.de
#
#**********************************************************************
#
# C++ : Name of C++ Compiler
#
C++       = g++

#
.SUFFIXES : .c .cpp .o
#
thismakefile := $(lastword $(MAKEFILE_LIST))
TARGET     = mvshader
TARGET_EXE = $(TARGET)

#
#
#
ifeq ($(OS),Windows_NT)
  OS_SYS   = Windows
else
  OS_SYS := $(shell uname -s)
endif


INCPATH = -I. -I./source/include
LDFLAGS =
CCFLAGS = -O2 $(INCPATH)


ifeq ($(OS_SYS),Windows)
  LIBPATH  = -L../lib-extern/windows64
  LDFLAGS += $(LIBPATH) -lm -lSDL2 -lSDL2.dll -lglu32 -lopengl32 -lglew32 -lglew32.dll -lz -lwinmm -s
  TARGET_EXE = $(TARGET).exe

endif

ifeq ($(OS_SYS),Linux)
  LIBPATH  = -L../lib-extern/linux64
  LDFLAGS += $(LIBPATH) -lSDL2 -lGL -lGLU -lGLEW -lz -s
  INCPATH += -I/usr/include/GL
  CCFLAGS += -DGLEW_STATIC
endif

ifeq ($(OS_SYS),Darwin)
  LIBPATH    = -L./lib-extern/mac
  INCPATH   += -I/usr/include/GL -I/usr/local/include/GL
  INCPATH   += -I/Library/Frameworks/SDL2.framework/Versions/A/Headers
  FRAMEWORKS = -framework IOKit \
               -framework Foundation \
               -framework Cocoa   -framework CoreAudio -framework Carbon -framework OpenGL  -framework ForceFeedback  -framework AudioUnit
  LDFLAGS   += $(LIBPATH) $(FRAMEWORKS) -lSDL2 -lGLEW -liconv

endif
#
#
#
#
#
#

#************************************************************
#
# Liste der C SourceFiles
#
include cppfiles.mk
#
#

#
#
default : $(TARGET)
#
#
.cpp.o:
	$(C++) -c $(CCFLAGS) -o$@ $<
#
#
#
.c.o:
	gcc -c $(CCFLAGS) -o$@ $<


#
clean:
	rm -fv $(OBJECTS1) $(OBJECTS2)
#
OBJECTS1 = $(CPPFILES:.cpp=.o)
OBJECTS2 = $(CFILES:.c=.o)

#
#
# Making the target-exe
#
ifeq ($(OS_SYS),Windows)
$(TARGET) : $(OBJECTS1) $(OBJECTS2)
	$(C++) -o $(TARGET_EXE) $(OBJECTS1)  $(OBJECTS2) $(LDFLAGS)
	cp -v $(TARGET_EXE) ./bin/Windows64
endif

ifeq ($(OS_SYS),Linux)
$(TARGET) : $(OBJECTS1) $(OBJECTS2)
	$(C++) -s -o $(TARGET) $(OBJECTS1)  $(OBJECTS2) $(LDFLAGS)
	cp -v $(TARGET_EXE) ./bin/Linux
endif


ifeq ($(OS_SYS),Darwin)
$(TARGET) : $(OBJECTS1) $(OBJECTS2)
	$(C++) -s -o $(TARGET) $(OBJECTS1)  $(OBJECTS2) $(LDFLAGS)
#	cp -v $(TARGET) ./bin/mac/$(TARGET_EXE).app/Contents/MacOS
endif

