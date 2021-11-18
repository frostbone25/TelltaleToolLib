# TelltaleToolLib - Telltale Tool Modding Library
A Library which implements the Meta part of the Telltale Tool Engine. 
This is quite a large library, and requires that you understand the fundamentals.

### Library Information
This is a 64bit WINDOWS C++ library and is only meant to be built for windows. It should ideally be linked as a static library; however, if you are going to built it into your application then the header and source files are all in the same directories. Ideally you would copy the whole source into your project (note, credit must be given), and use it like that; this is also a template library for some types so this would benefit. This library should be shipped with/in your applications alongside the oo2core Oodle compression library DLL.
This library can also be compiled as an application, the CLI folder should be deleted to not compile the CLI translation unit.

### Setting up
Once you have the code included or static library added to the linker arguments then you can start by including TelltaleToolLib.h first.
This header contains all the library exported functions and functions which will should be used to perform all operations in the library (although, other headers can be included and functions in them called, but unless they are in the Types folder then they arent documented for you to be using them.

#### Fundamental structures
Telltale Games store all 'files' in a sectioned wrapping file format called a `MetaStream`. A Meta Stream is a file stream, which can be written to and read from, and has three main sections: the `header` section, `default` section, `debug` section, and `async` section.
The header section contains the section sizes in the meta stream, the meta version, and a list of type symbol CRC64s (with their version CRC32s) for all types serialized using the Meta namespace default serialization (more on that later). The default section contains the main chunk of serialized data, most data read in files is from this section. The debug section, as the name implies, contains debug data used when making games in Tool. The last section, the asynchronous section, contains data which would need to be accessed by other threads in the runtime Game Engine. This is normally data such as texture data. 

