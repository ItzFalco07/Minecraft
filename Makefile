CXX = g++
LIBS = -lglfw -lGL -lGLU -lGLEW

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
	$(CXX) main.cpp resource.o -o Minecraft.exe $(LIBS)

main: main.cpp
	$(CXX) main.cpp -o main $(LIBS)

resource.o: resource.rc
	windres resource.rc -o resource.o

clean:
	rm -f Minecraft.exe main resource.o