using Godot;
using System;
using Overwrite.Jam.Scanner;

[GlobalClass]
public partial class testScan : Node
{
    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        Scanner.NativeLibraryDirectory = "addons/";
        Scanner s = new Scanner("/dev/ttyACM0");
        s.OnDataReceived += OnRead;
        s.OnError += OnError;
    }

    static void OnRead(Span<byte> recv)
    {
        string data = Scanner.AsString(recv);
        GD.Print($"recv from scanner: {data}");
    }
    static void OnError(string s)
    {
        Console.WriteLine(s);
    }

    // Called every frame. 'delta' is the elapsed time since the previous frame.
    public override void _Process(double delta)
    {
    }
}
