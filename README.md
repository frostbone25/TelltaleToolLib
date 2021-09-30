# TelltaleToolLib
A Library which implements the Meta part of the Telltale Tool Engine. The meta part of the engine is for serializing all structures in the game engine to disk. This library also implements many other types which are in the engine aswell, for example TTArchive2 bundles.

This is an updated version of my old LibTelltale library. This library will not only be more precise, but will have better memory management and is completely public.

This is of course a LIBRARY: this means that you can compile it to a dynamic library (.dll/.dylib/.so). However, it is designed so that you can (since its a library) use the code to read
files etc from telltale in your own mods.

### Where to start?
The most important part to wrap your head around is how and what exactly meta operations are, and how telltale serialize and keep track of runtime structures and compile them
so that they can be read and written aswell as many other meta operations (such as to string, from string etc).

I suggest reading the Formats.txt document written by Me (in Doc/Formats.txt). This explains a lot of the file structures, and this library implements that very similarly 
to how the telltale engine does (at least the one they used before and including TWD: Definitive Series). 

If you want to get farmiliar with the API (for C++) then there should always be some exampe code in Test/main.cpp

## IMPORTANT
This library is designed for TWD: Definitive but games newer than Minecraft: Story Mode - Season Two (inclusive) are likely to be compatible. This means most of the functions to read and write the files should be compatible. 
Older games may work, but its not guarunteed and I suggest you stick to the ones that should work:
Minecraft Story Mode: Season Two
Batman: The Enemy Within
The Walking Dead: Collection
The Walking Dead: The Final Season
