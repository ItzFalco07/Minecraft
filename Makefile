CXX = g++
LIBS = -lglfw -lGL -lGLU -lGLEW

IMGUI_SRC = ./packages/imgui/imgui.cpp \
            ./packages/imgui/imgui_draw.cpp \
            ./packages/imgui/imgui_widgets.cpp \
            ./packages/imgui/imgui_tables.cpp \
            ./packages/imgui/imgui_impl_glfw.cpp \
            ./packages/imgui/imgui_impl_opengl3.cpp

all: Minecraft.exe

Minecraft.exe: main.cpp resource.o
	$(CXX) main.cpp resource.o $(IMGUI_SRC) -o Minecraft.exe $(LIBS)

resource.o: resource.rc
	windres resource.rc -o resource.o

clean:
	rm -f Minecraft.exe resource.o
