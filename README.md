[![Build GDExtension](https://github.com/Decapitated/Godot-HTML/actions/workflows/builds.yml/badge.svg)](https://github.com/Decapitated/Godot-HTML/actions/workflows/builds.yml)

# Godot HTML
A GDExtension, using Ultralight, to render HTML.

<img src="docs/images/WebView.png">
<img src="docs/images/EditorView.png">
<img src="docs/images/InspectorView.png">

## Versions
- Godot 4.3+
- Ultralight 1.4.0 Beta

## Platforms
| Platform | x64 | arm64 | x32 |
| - | :-: | :-: | :-: |
| Windows | :white_check_mark: |  :warning: | :x: |
| Linux | :white_check_mark: |  :warning: | :x: |
| Mac | :white_check_mark: | :white_check_mark: | :x: |

> :white_check_mark: = Successfully ran the demo project.

> :warning: = Supported, but not tested.

> :x: = Not supported.

## Table of contents
- [What can this plugin do?](#can-do)
- [What's left to do?](#to-do)
- [Limitations](#ultralight-limitations)
- [Prerequisites](#prerequisites)
- [Project Structure](#project-structure)
- [Building](#building)
  - [Clone godot-cpp](#clone-godot-cpp)
  - [Build Godot C++ bindings](#build-godot-cpp-bindings)
  - [Build plugin](#build-plugin)
- [JS Interop](#js-interop)

## CAN-DO
- Render HTML & CSS, with JS capabilities.
- HTML Inspector
- Mouse & Keyboard Input
- JS Interop
- GDScript Support

## TO-DO
- Accelerated Views. IE. GPU based rendering.
- ImageSource. IE. RenderTargets.
- Make sure remote websites can't access C++ code via JS interop.
- Live code updates. (Allow for html to update in the editor when file changed)
- ?? More will arrise I assume ??

## Ultralight Limitations
Ultralight works for most web features. Check out the [missing features](https://github.com/ultralight-ux/Ultralight/issues/178).

Some of the notable limitations are:
- `.webp`
- HTML5 Video/Audio

## Prerequisites
- Setup [compiling](https://docs.godotengine.org/en/stable/contributing/development/compiling/compiling_for_windows.html) for Godot
- Download [Ultralight](https://ultralig.ht/download/)

## Project Structure
Your project structure should look like this before running with this extension.
```
demo/                            # Godot project (Root)
  addons/                        # Addons
    gdhtml/                      # Godot-HTML extension
    | bin/                       # Extension libraries
    |   windows/                # Library platform
    |     x86_64/               # Platform arch
    |       gdhtml.*.dll        # Extension library
    |       Ultralight.dll      # Ultralight libraries
    |       UltralightCore.dll  # ...
    |       WebCore.dll         # ...
    |       AppCore.dll         # ...
    | inspector/                 # Ultralight inspector
    | resources/                 # Ultralight resources
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

## JS Interop
JS interop is as simple as returning a `Dictionary` from the virtual function `_on_dom_ready`. Then accessing it from JS using `window.godot`.

Checkout the [example](demo/app_example.gd) script.

Important:
- JS Symbols are not currently supported. Convert them to a string before passing them to Godot.

## Exporting Godot Project
Once you're ready to export your game/project make sure to:
<table>
  <tr>
    <td>Include html files in export via `Export Configuration`.</td>
    <td><img src="docs/images/ExportConfiguration.png"></td>
  </tr>
  <tr>
    <td>For auto imported resources, select `Keep File (No Import)`.</td>
    <td><img src="docs/images/NoResourceImport.png"></td>
  </tr>
<table>
