# AdventoConsoleEngine
A console engine that makes rendering in console much easier!

# How to use?

Using Visual Studio
  All you need to do is to add "engine" folder to your project and you're ready to go!

Using MinGW
  1. Put the "engine "folder in the root directory
  2. Compile command: g++ -o main.exe main.cpp engine/*.cpp -static-libgcc -static-libstdc++

#WARNING
The engine uses WinApi to draw stuff to console so it only works for windows

# Examples
You can find example program in "src" directory (ParticlesDemoMain.cpp)
