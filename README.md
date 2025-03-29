# How to Run the Code?
You can run the code directly from your terminal (Command Prompt) or compile it as an `.exe` file.

## Prerequisites
Make sure you have the following installed:
- OpenGL
- GLFW
- GLU
- GLEW
- Make
- G++ ( C++ compiler )

## Run the Code Directly
If you prefer running the code without compiling it as an executable, you can use the following command:
```bash
./main
```

## Compile as .exe (For Windows)
Follow these steps to compile the code as a `.exe` file:

1. **Install OpenGL and Required Packages:**
Run the following command to install necessary dependencies using your package manager (for example, using MSYS2 on Windows):
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-opengl mingw-w64-x86_64-glfw mingw-w64-x86_64-glu mingw-w64-x86_64-glew
```

2. **Compile the Code:**
Use the `make` command to compile the executable.
```bash
make
```
This will generate a `Minecraft.exe` file in the project directory.

3. **Run the Executable:**
```bash
./Minecraft.exe
```

## Troubleshooting
- If `make` is not recognized, ensure it is installed and added to your system's PATH.
- Confirm `windres` is available if you are using a resource file for the icon.
- If there are missing library errors, ensure all dependencies are installed and correctly linked.

## Additional Notes
- `main` is the default executable on Linux or macOS.
- `Minecraft.exe` is the output for Windows when using `make`.
- `imgui.ini` contains ImGui configuration data and is automatically generated or modified during runtime.

Happy Coding!

