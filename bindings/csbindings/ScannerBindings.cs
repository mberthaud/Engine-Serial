using System.Runtime.InteropServices;
namespace Overwrite.Jam.Scanner
{
    public unsafe partial class Scanner
    {
        [DllImport("libMySerial", EntryPoint = "scanner_create", ExactSpelling = true)]
        private extern static ScannerHandle* MakeScanner(byte* str, void* callback);
        [DllImport("libMySerial", EntryPoint = "scanner_destroy")]
        private extern static void DeleteScanner(ScannerHandle* handle);
        [DllImport("libMySerial", EntryPoint = "get_scanner_ports_c")]
        private extern static void GetScannerPorts();

        private struct ScannerHandle
        {
        }
    }

}


