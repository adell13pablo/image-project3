LIBDIR = C:/libraries/lib
LIB = -L $(LIBDIR) -lOpenGL32 -lopencv_core340 -lopencv_imgproc340 -lopencv_objdetect340 -lopencv_highgui340 -lopencv_imgcodecs340 -lopencv_videoio340

DLL = $(LIBDIR)/libglew32.dll.a $(LIBDIR)/libglfw3dll.a

INC = C:/libraries/include
IMGUI = $(INC)/imgui/imgui.cpp $(INC)/imgui/imgui_draw.cpp

OPT =

all:
	g++ $(OPT) -std=c++11 main.cpp $(IMGUI) -I $(INC) $(DLL) $(LIB) -o tra

asm:
	g++ -S $(OPT) -std=c++11 main_prueba.cpp $(IMGUI) -I $(INC) $(DLL) $(LIB)
