using UnityEngine;
using Overwrite.Jam.Scanner;
using System;

public class SerialTester : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        var ports = Scanner.GetPorts();
        if (ports.Length == 0)
        {
            Debug.LogWarning("no scanners found!");
            return;
        }
        Scanner s = new Scanner(ports[0]);
        s.OnDataReceived += OnDataRecv;
    }
    void OnDataRecv(Span<byte> data)
    {
        UnityEngine.Debug.Log(Scanner.AsString(data));
    }

    // Update is called once per frame
    void Update()
    {

    }
}
