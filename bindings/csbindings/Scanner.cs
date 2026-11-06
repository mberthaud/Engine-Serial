using System;
using System.Runtime.InteropServices;
using System.Text;
#if NET5_0_OR_GREATER
using System.Reflection;
using System.IO;
#endif
namespace Overwrite.Jam.Scanner
{
    public delegate void ScannerCallback(Span<byte> data);
    public unsafe partial class Scanner : IDisposable
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void NativeDataCallback(byte* data, int len);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void NativeErrorCallback(byte* what, int len);
        private ScannerHandle* scannerHandle;

        public event ScannerCallback OnDataReceived;
        public event Action<string> OnError;
        public string PortName { get; private set; }
        private readonly NativeDataCallback dataCallback;
        private readonly NativeErrorCallback errorCallback;

        public Scanner(string portName)
        {
            PortName = portName;
            IntPtr portPtr = Marshal.StringToCoTaskMemUTF8(portName);
            dataCallback = OnDataRecv;
            errorCallback = OnErrorRecv;
            IntPtr nativeDataCallbackInstance = Marshal.GetFunctionPointerForDelegate(dataCallback);
            IntPtr nativeErrorCallbackInstance = Marshal.GetFunctionPointerForDelegate(errorCallback);
            scannerHandle = MakeScanner((byte*)portPtr.ToPointer(), nativeDataCallbackInstance.ToPointer(), nativeErrorCallbackInstance.ToPointer());
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

        private void OnErrorRecv(byte* what, int len)
        {
            string msg = AsString(new Span<byte>(what, len));
            OnError?.Invoke(msg);
        }

#if NET5_0_OR_GREATER
        //@NOTE: for non unity projects the user must be able to set the directory to find the native libs
        public static string NativeLibraryDirectory { get; set; } = string.Empty;
        static Scanner()
        {
            NativeLibrary.SetDllImportResolver(typeof(Scanner).Assembly, ResolveNativeLibrary);
        }

        private static nint ResolveNativeLibrary(string libName, Assembly assembly, DllImportSearchPath? searchPath)
        {
            if (NativeLibraryDirectory == string.Empty)
            {
                return default;
            }

            string fileName;
            if (OperatingSystem.IsWindows())
            {
                fileName = $"{libName}.dll";
            }
            else if (OperatingSystem.IsMacOS())
            {
                fileName = $"lib{libName}.dylib";
            }
            else
            {
                fileName = $"lib{libName}.so";
            }

            nint handle = 0;
            string filePath = Path.Combine(NativeLibraryDirectory, fileName);
            if (File.Exists(filePath))
            {
                handle = NativeLibrary.Load(filePath);
            }
            return handle;
        }
#endif
    }
}
