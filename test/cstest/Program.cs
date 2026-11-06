using System;
using Overwrite.Jam.Scanner;
class Program
{
    static void Main(string[] args)
    {
        Scanner.NativeLibraryDirectory = "../../build";
        Scanner s = new Scanner("/dev/cu.usbmodem00000000050C1");
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
}

