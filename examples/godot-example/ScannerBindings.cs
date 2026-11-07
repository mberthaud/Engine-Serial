using System;
using System.Runtime.InteropServices;
namespace Overwrite.Jam.Scanner
{
    public unsafe partial class Scanner
    {
        [DllImport("MySerial", EntryPoint = "scanner_create", ExactSpelling = true)]
        private extern static ScannerHandle* MakeScanner(byte* str, void* dataCallback, void* errorCallback);
        [DllImport("MySerial", EntryPoint = "scanner_destroy")]
        private extern static void DeleteScanner(ScannerHandle* handle);
        [DllImport("MySerial", EntryPoint = "get_scanner_ports_c")]
        private extern static byte** GetScannerPorts(UIntPtr* count);
        [DllImport("MySerial", EntryPoint = "free_scanner_ports_c")]
        private extern static void FreeScannerPorts(byte** ports, int count);

        private struct ScannerHandle
        {
        }
    }

}



