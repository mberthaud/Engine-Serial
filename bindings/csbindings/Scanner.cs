using System;
using System.Runtime.InteropServices;
using System.Text;
namespace Overwrite.Jam.Scanner
{
    public delegate void ScannerCallback(Span<byte> data);
    public unsafe partial class Scanner : IDisposable
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void NativeCallback(byte* data, int len);
        private ScannerHandle* scannerHandle;

        public event ScannerCallback OnDataReceived;
        public string PortName { get; private set; }

        public Scanner(string portName)
        {
            PortName = portName;
            IntPtr portPtr = Marshal.StringToCoTaskMemUTF8(portName);
            IntPtr nativeCallbackInstance = Marshal.GetFunctionPointerForDelegate<NativeCallback>(OnDataRecv);
            scannerHandle = MakeScanner((byte*)portPtr.ToPointer(), nativeCallbackInstance.ToPointer());
            Marshal.ZeroFreeCoTaskMemUTF8(portPtr);
            if (scannerHandle == null)
            {
                throw new Exception($"Unable to create Scanner at port {portName}");
            }
        }

        public void Close()
        {
            if (scannerHandle != null)
            {
                DeleteScanner(scannerHandle);
                scannerHandle = null;
            }
        }

        public void Dispose()
        {
            Close();
        }

        public static string AsString(Span<byte> data)
        {
            return Encoding.UTF8.GetString(data);
        }

        private void OnDataRecv(byte* data, int len)
        {
            OnDataReceived?.Invoke(new Span<byte>(data, len));
        }
    }
}
