# TelltaleToolLib
A Library which implements the Meta part of the Telltale Tool Engine. The meta part of the engine is for serializing all structures in the game engine to disk.

This is an updated version of my old LibTelltale library. This library will not only be more precise, but will have better memory management and is completely public.

This is a library, it is designed to be made into a dynamic library. The library is not 100% portable but I have made some endian checks and platform specific definitions which may be useful. If you were to compile on a different plaform (ie not windows) you made need to do some tinkering. This library is to make telltale modding easier, and to handle all file serialization.

### Where to start?
The Library's external exported functions and all type definitions can be found in TelltaleToolLibrary.h. I suggest looking here first and reading the docs.
If you want to get farmiliar with the API (for C++) then there should always be some example code in Test/main.cpp.

### What does this library do?
It provides an API for opening and reading the files aswell as writing them. You can open any file from any telltale game with this library (read its meta header). However to read the payload of the files (the data after the header) you need to make sure the game is supported. You can then write files back to disc, but only if they are using MSV5 or MSV6 meta versions. The payload file data is what is version dependent. In Src/Tests I will put examples and tests for how the API is used.

## IMPORTANT
This library is designed for TWD: Definitive but games newer than Minecraft: Story Mode - Season Two (inclusive) are likely to be compatible (this is not guaranteed). This means most of the functions to read and write the files should be compatible. 
Older games may work, but its not guarunteed and I suggest you stick to the ones that should work:
Minecraft Story Mode: Season Two
Batman: The Enemy Within
The Walking Dead: Collection
The Walking Dead: The Final Season
