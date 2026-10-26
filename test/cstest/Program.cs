using System;
using Overwrite.Jam.Scanner;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using System.Reflection;
using System.IO;
class Program
{
    static void Main(string[] args)
    {
        AssemblyLoadContext.GetLoadContext(typeof(Scanner).Assembly).ResolvingUnmanagedDll += ResolveAssembly;
        Scanner s = new Scanner("/dev/ttyACM0");
        s.OnDataReceived += OnRead;
        while (true)
        {
        }
    }

    static void OnRead(Span<byte> recv)
    {
        string data = Scanner.AsString(recv);
        Console.WriteLine($"recv from scanner: {data}");
    }

    public static nint ResolveAssembly(Assembly assembly, string libName)
    {
        string extension;
        if (OperatingSystem.IsWindows())
        {
            extension = "dll";
        }
        else if (OperatingSystem.IsLinux())
        {
            extension = "so";
        }
        else if (OperatingSystem.IsMacOS())
        {
            extension = "dylib";
        }
        else
        {
            throw new PlatformNotSupportedException();
        }

        //@NOTE: for the sake of testing we just point directly to the built shared lib
        //during native lib resolution
        string path = $"../../build/{libName}.{extension}";
        nint handle = 0;
        if (File.Exists(path))
        {
            handle = NativeLibrary.Load(path);
        }
        else
        {
            Console.WriteLine($"could not find {libName} at {path}");
        }
        return handle;
    }
}
