# TelltaleToolLib
A Library which implements the Meta part of the Telltale Tool Engine. The meta part of the engine is for serializing all structures in the game engine to disk.

This is an updated version of my old LibTelltale library. This library will not only be more precise, but will have better memory management and is completely public.

This is a library, it is designed to be made into a dynamic library. The library is not 100% portable but I have made some endian checks and platform specific definitions which may be useful. If you were to compile on a different plaform (ie not windows) you made need to do some tinkering. This library is to make telltale modding easier, and to handle all file serialization.

#### Please note that if you are to rebuild this library or use it without building it beforehand (ie copying src and headers) you should not include Src/Test or Src/TelltaleEditor in your project. These folders are for project specific implementations.

### Where to start?
The Library's external exported functions and all type definitions can be found in TelltaleToolLibrary.h. I suggest looking here first and reading the docs.
If you want to get farmiliar with the API (for C++) then there should always be some example code in Test/main.cpp.

### What does this library do?
It provides an API for opening and reading the files aswell as writing them. You can open any file from any telltale game with this library (read its meta header). However to read the payload of the files (the data after the header) you need to make sure the game is supported. You can then write files back to disc, but only if they are using MSV5 or MSV6 meta versions. The payload file data is what is version dependent. In Src/Tests I will put examples and tests for how the API is used. You can also create files without opening a one previously (only for new games) and set data into those files and write them.

## IMPORTANT

When loading TTARCH2 bundles, MAKE SURE to have 'oo2core_5_win64.dll' is in the same directory as the executable. Preferably this dll should be kept along side this library's DLL.

This library is designed for TWD: Definitive but games newer than Minecraft: Story Mode - Season Two (inclusive) are likely to be compatible (this is not guaranteed). This means most of the functions to read and write the files should be compatible. 
Older games may work, but its not guarunteed and I suggest you stick to the ones that should work:
Minecraft Story Mode: Season Two
Batman: The Enemy Within
The Walking Dead: Collection
The Walking Dead: The Final Season

Please also note this is a windows library! You could toggle values to make it for another platform however. But note that a lot of the code expects that the machine is in little endian! Reading .ttarch2 archives will fail unless in little endian. However, you can read normal telltale files (MetaStream ones) if its in big endian.
