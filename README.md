```
⚠️WINDOWS ONLY⚠️
This is very early stages. Feel free to contribute.
```

# Godot HTML

A GDExtension, using Ultralight, to render HTML.

<img src="docs/images/EditorView.png">
<img src="docs/images/InspectorView.png">

## Versions
Godot v4.2<br>
Ultralight v1.3

## CAN-DO
- Render HTML & CSS, with JS capabilities.
- HTML Inspector
- Mouse Input

## TO-DO
- Keyboard Input
- JS Interop
- Live code updates (Allow for html to update in the editor when file changed)
- ?? More will arrise I assume ??

## Prerequisites
- Setup [compiling](https://docs.godotengine.org/en/stable/contributing/development/compiling/compiling_for_windows.html) for Godot
- Download [Ultralight](https://ultralig.ht/download/)

## Project Structure
Your project should look like this before building the plugin.
```
main_folder/
|
+--demo/                  # game example/demo to test the extension
  |
  +--plugin_folder/
    |
    +--bin/               # plugin build folder / Other libraries
      +--Ultralight.dll
      +--UltralightCore.dll
      +--WebCore.dll
      +--AppCore.dll
    |
    +--resources/         # Ultralight resources folder (Copied from SDK folder)
  |
  +--inspector/           # Ultralight inspector folder (Copied from SDK folder)
|
+--godot-cpp/             # C++ bindings
|
+--src/                   # Source code of the extension we are building
|
+--ultralight/            # Ultralight SDK
```

## Building

### Clone godot-cpp
```
git submodule update --init --remote
```

### Build Godot C++ bindings
```
cd godot-cpp
scons platform=<platform>
```

### Build plugin

From the main folder.
```
scons platform=<platform>
```