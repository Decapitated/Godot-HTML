# Prerequisites
- Setup [compiling](https://docs.godotengine.org/en/stable/contributing/development/compiling/index.html#toc-devel-compiling) for Godot
- Download [Ultralight](https://ultralig.ht/download/)

# Building
## Build Godot C++ bindings
```
cd godot-cpp
scons platform=<platform>
```
## Build plugin
From the main folder.
```
scons platform=<platform>
```

# Project Structure
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
+--godot-cpp/             # C++ bindings
|
+--src/                   # source code of the extension we are building
|
+--ultralight/            # Ultralight SDK
```