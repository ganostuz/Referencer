# Referencer

Reference desktop app for artists with 2D and 3D viewports. It uses C++20,
OpenGL, GLFW, GLAD, Dear ImGui, Assimp, GLM, libcurl, stb_image, and toml11.

## Linux

Install a C++ compiler, GNU Make, pkg-config, GLFW, Assimp, libcurl, OpenGL
development files, and either Zenity or KDialog for native file dialogs.

Arch Linux:

```sh
sudo pacman -S --needed base-devel pkgconf glfw assimp curl mesa zenity
```

Ubuntu/Debian:

```sh
sudo apt install build-essential pkg-config libglfw3-dev libassimp-dev \
  libcurl4-openssl-dev libgl1-mesa-dev zenity
```

Build and run from the repository root:

```sh
make
make run
```

Use `make debug` for an unoptimized build with debug symbols. The executable
is written to `bin/Referencer`, and runtime resources are copied to
`bin/resources`.

Referencer prefers X11 (or XWayland in a Wayland session) when `DISPLAY` is
available because Dear ImGui native multi-viewports require global window
positioning. On pure Wayland, the app falls back to a single native window and
keeps all ImGui viewports inside it.

## Windows

The Windows build uses GNU Make and the MinGW64 toolchain from
[MSYS2](https://www.msys2.org/). In an MSYS2 MinGW64 shell, install:

```sh
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-make \
  mingw-w64-x86_64-pkgconf mingw-w64-x86_64-glfw \
  mingw-w64-x86_64-assimp mingw-w64-x86_64-curl
```

Build and run:

```sh
mingw32-make
mingw32-make run
```

Create a relocatable artifact directory:

```sh
mingw32-make package
```

This writes `dist/Referencer-windows-x64` with `Referencer.exe`, resources, and
all required MinGW runtime DLLs. The executable keeps the Win32 entry point,
native dialogs, and embedded icon.

