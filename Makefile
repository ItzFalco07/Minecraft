CXX = g++
LIBS = -lglfw -lGL -lGLU -lGLEW

IMGUI_SRC = ./packages/imgui/imgui.cpp \
            ./packages/imgui/imgui_draw.cpp \
            ./packages/imgui/imgui_widgets.cpp \
            ./packages/imgui/imgui_tables.cpp \
            ./packages/imgui/imgui_impl_glfw.cpp \
            ./packages/imgui/imgui_impl_opengl3.cpp

all:
	@echo "Do you want to compile as 'Minecraft.exe' or './main'? (Enter 1 for Minecraft.exe, 2 for main)"
	@read choice; \
	if [ $$choice -eq 1 ]; then \
		make Minecraft.exe; \
	elif [ $$choice -eq 2 ]; then \
		make main && ./main; \
	else \
		echo "Invalid choice. Exiting."; \
	fi

Minecraft.exe: main.cpp resource.o
	$(CXX) main.cpp resource.o $(IMGUI_SRC) -o Minecraft.exe $(LIBS)

main: main.cpp
	$(CXX) main.cpp $(IMGUI_SRC) -o main $(LIBS)

resource.o: resource.rc
	windres resource.rc -o resource.o

clean:
	rm -f Minecraft.exe main resource.o
