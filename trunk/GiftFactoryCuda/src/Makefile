# Makefile
# Template Fluids Simulation
# Nadine Dommanget dommange@univ-mlv.fr




MACHINE = $(shell uname -s)

ifeq ($(MACHINE),Darwin)
	OPENGL_INC = -FOpenGL
	OPENGL_LIB = -framework OpenGL -lm
	SDL_INC = `sdl-config --cflags`
	SDL_LIB = `sdl-config --libs`
	#CXX=g++-mp-4.5
	CXX=g++
else
	OPENGL_INC = -I/usr/X11R6/include
	OPENGL_LIB = -I/usr/lib64 -lGLEW -lGL -lGLU -lSDL_image -lm -lrt
	#OPENGL_LIB = -lGL -lGLU -lm
	SDL_INC = -I/usr/include/SDL/
	SDL_LIB = `sdl-config --libs`
	CXX=g++
endif


COMPILER_FLAGS=-g -I. 
COMPILER_FLAGS_WARN=-g -Wall -I.

EXE=GiftFactory

INCLUDE= $(SDL_INC) $(OPENGL_INC) -I../api
LIBS= $(SDL_LIB) $(OPENGL_LIB)

OBJS = Utils.o Application.o Camera.o cuda_functions.o model_obj.o Object.o Scene.o ShaderManager.o Spline.o TextureManager.o

all : $(EXE) 

$(EXE) : $(OBJS)
	$(CXX) $(COMPILER_FLAGS_WARN) $^ $(LIBS) main.cpp -o $@

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CXX) -c $(COMPILER_FLAGS_WARN) $(INCLUDE) $<
	@$(CXX)  -MM $(INCLUDE) $< > $*.d

.PHONY : clean ultraclean

clean::
	rm -f *.o *~ *.d *.oD 

bigclean : clean
	rm -f $(EXE) $(LIB)
