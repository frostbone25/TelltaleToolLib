using System;
using System.Runtime.InteropServices;
public class _Main {

    class Link {

        [DllImport("LibTelltaleTool.dll")]
       extern unsafe public static IntPtr TelltaleToolLib_GetVersion();

        [DllImport("LibTelltaleTool.dll")]
        extern public static bool TelltaleToolLib_Initialize(IntPtr game_id);

        [DllImport("LibTelltaleTool.dll")]
        extern public static bool TelltaleToolLib_Initialized();

        [DllImport("LibTelltaleTool.dll")]
        extern public static Int32 TelltaleToolLib_GetMetaTypesCount();

        [DllImport("LibTelltaleTool.dll")]
        extern public static IntPtr TelltaleToolLib_FindMetaClassDescription(IntPtr name, bool isByNameOrExtension);

        [DllImport("LibTelltaleTool.dll")]
        extern public static IntPtr TelltaleToolLib_Alloc_GetFixed1024ByteStringBuffer();

        [DllImport("LibTelltaleTool.dll")]
        extern public static IntPtr TelltaleToolLib_Alloc_GetFixed8BytePointerBuffer();

        [DllImport("LibTelltaleTool.dll")]
        extern public static void TelltaleToolLib_GetMetaClassDescriptionInfo(IntPtr clazz, IntPtr dest, Int32 paramType);

    }

    static string PtrToStr(IntPtr ptr)
    {
        return Marshal.PtrToStringAnsi(ptr);
    }

    static void exit(string reason)
    {
        Console.WriteLine(reason);
        Console.ReadKey();
    }

    static unsafe void store_string(byte* buf, string value)
    {
        for(int i = 0; i < value.Length; i++)
        {
            buf[i] = (byte)value[i];
        }
        buf[value.Length] = (byte)'\0';
    }

    public static unsafe void Main(string[] args)
    {
        Console.WriteLine("Loading libraries..");
        string version = PtrToStr(Link.TelltaleToolLib_GetVersion());
        bool init = Link.TelltaleToolLib_Initialize(IntPtr.Zero);//default to WDC
        if (!init) exit("Could not initialize LibTelltaleTool.");
        Console.WriteLine("Initialized LibTelltaleTool version {0} successfully. Total of {1} types found.", version,
            Link.TelltaleToolLib_GetMetaTypesCount());
        while (true)
        {
            Console.WriteLine("Please type the file extension of the file you would like to open (or, if you want to by type name" +
                " then enter #<typename> eg #T3Texture). Enter nothing to exit");
            string fileext = Console.ReadLine();
            string typename = "";
            if (fileext.Length == 0) exit("Exiting..");
            bool isName = false;
            if (fileext.StartsWith("#"))
            {
                isName = true;
                fileext = fileext.Substring(1);
            }
            byte* strbuf = (byte*)Link.TelltaleToolLib_Alloc_GetFixed1024ByteStringBuffer().ToPointer();
            store_string(strbuf, fileext);
            IntPtr clazz = Link.TelltaleToolLib_FindMetaClassDescription(new IntPtr(strbuf), isName);
            IntPtr ptrbuf = Link.TelltaleToolLib_Alloc_GetFixed8BytePointerBuffer();
            if(clazz.ToPointer() == (void*)0)
            {
                Console.WriteLine("Could not find a type for that name or extension!\n\n");
                continue;
            }
            Console.WriteLine("File type set.");
            Marshal.WriteIntPtr(ptrbuf, new IntPtr(strbuf));
            Link.TelltaleToolLib_GetMetaClassDescriptionInfo(clazz, ptrbuf, isName ? 1 : 2);
            if (isName)
            {
                typename = fileext;
                fileext = Marshal.PtrToStringAnsi(new IntPtr(strbuf));
            }
            else
            {
                typename = Marshal.PtrToStringAnsi(new IntPtr(strbuf));
            }
            while (true)
            {
                Console.WriteLine("Enter the file location of a file to open as a {0} ({1}), or nothing to move on.\n", fileext,typename);
                string fileloc = Console.ReadLine();
                if (fileloc.Length == 0) break;
                
            }
        }
        Console.WriteLine("Type anything to exit!");
        Console.ReadKey();
    }

}
