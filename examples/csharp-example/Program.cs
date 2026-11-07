using System;
using Overwrite.Jam.Scanner;
class Program
{
    static void Main(string[] args)
    {

        //@NOTE: this is to set the directory of where your .lib/.so/.dylib is.
        Scanner.NativeLibraryDirectory = "../../build";

        var ports = Scanner.GetPorts();
        if (ports.Length == 0)
        {
            Console.WriteLine("no scanners found");
            return;
        }
        foreach (var p in ports)
        {
            Console.WriteLine(p);
        }


        Scanner s = new Scanner(ports[0]);
        s.OnDataReceived += OnRead;
        //@NOTE: just to hold the program
        while (true)
        {
        }
    }

    static void OnRead(Span<byte> recv)
    {
        string data = Scanner.AsString(recv);
        Console.WriteLine($"recv from scanner: {data}");
    }
}

