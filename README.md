# salient

A fork of libtcod/umbra

An engine for creating Roguelike games

## How to setup

* Make sure you have the correct tools ready.
  * For example on Windows:
    Make sure you install the C++ compiler tools from [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/),
    install the latest [CMake](https://cmake.org/download/),
    and install [Visual Studio Code](https://code.visualstudio.com/download).
* [Create a new repository from ths template](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/creating-a-repository-on-github/creating-a-repository-from-a-template#creating-a-repository-from-a-template).  **You do not use the GitHub fork button unless you're contributing.**
* [Clone your new repository](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository-from-github/cloning-a-repository).
* Make sure to initialize the `vcpkg` [submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules).  If the `vcpkg` folder is empty or missing then the build process will fail.
  * The command to do this is: `git submodule update --init --recursive`
* Load this project in VS Code and install any recommended plugins.
  Most importantly: *C/C++*, *C++ Intellisense*, and *CMake Tools*.
* Using the CMake Tools plugin you can now build and run this project.  Dependencies will automatically be installed via Vcpkg.
  * On Windows, when CMake Tools asks for a toolkit you should select `Visual Studio Community ... - amd64`.
* The project will now build and run with F5.
