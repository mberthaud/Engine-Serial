# Engine Integration

## Unity
download the csharp plugin from the releases page for your OS. extract the zip and drag and drop it into your **plugins** folder.

In your UnityPlayer Settings enable **Allow Unsafe code**. the scanner bindings needs unsafe code to interface with the native library.

In the example folder is an example monobehavior that you can drag into your project for testing.

## Godot

### GD-Script
Download The GD-script from the release Page. extract the zip and drop its contents into the addons folder in your godot project. if you do not 
already have an addons folder in your godot project make one.

### Csharp
download the csharp plugin from the releases page for your OS. extract the zip and drag and drop it into your addons folder. if you do not 
already have an addons folder in your godot project make one. 
in your .csproj add  **<AllowUnsafeBlocks>true</AllowUnsafeBlocks>** the scanner bindings needs unsafe code to interface with the native library

unlike unity that has a plugins folder, godot does not have a standard placed to look for native libraries to be loaded. as such add this line somewhere in your code. this will tell Godot to looks for the scanner .dll/.so/.dylib in the addons folder.
```cs
Scanner.NativeLibraryDirectory = "Addons/"
```


# Scanner Modes
Each scanner comes with a configuration sheet that can adjust its modes

## Keyboard mode
The default mode for the scanner is USB-HID keyboard. in this mode scanning a QR code or barcode will automatically write output to your keyboard.
In this mode utilizing the scanner is as simple as reading from standard IN. you would interface it as if you were reading input from your keyboard.
For this you dont need any SDK.

## COM Mode
To use multiple Scanner you must use the SDK provided. you can see examples in the Test/ folder
Make sure to find the COM/tty port the scanner is connected to in order to use this mode. we will be here to help
if needed.


# tips and tricks
## where do I go make QR/Bar codes
https://qr.io/ is a good site to use. it has a text feature where you can make a qr code that when scanned will automatically output text.

## Ways to diferntiate multiple scanners
on your scanner configuration sheet. there codes to make the scanner append/prepend a unique letter after any data it scans. you can use this 
to detect scanner just received input.


  
