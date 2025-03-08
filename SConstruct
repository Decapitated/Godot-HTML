#!/usr/bin/env python
import os
import sys

from methods import print_error


libname = "gdhtml"
projectdir = "demo"

localEnv = Environment(tools=["default"], PLATFORM="")

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()

submodule_initialized = False
dir_name = 'godot-cpp'
if os.path.isdir(dir_name):
    if os.listdir(dir_name):
        submodule_initialized = True

if not submodule_initialized:
    print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download godot-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")
sources = sources + Glob("src/*/*.cpp")
sources = sources + Glob("src/*/*/*.cpp")

env.Append(CPPPATH=["ultralight/include/"])

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

file = "{}{}{}".format(libname, env["suffix"], env["SHLIBSUFFIX"])
filepath = ""

if env["platform"] == "windows":
    env.Append(LIBPATH=["ultralight/lib/"])
elif env["platform"] == "linux":
    env.Append(LIBPATH=["ultralight/bin/linux/"])
elif env["platform"] == "macos" and env["arch"] == "arm64":
    env.Append(LIBPATH=["ultralight/bin/macos/"])
    env.Append(LINKFLAGS=['-arch', 'arm64', '-rpath', os.path.abspath("ultralight/bin/macos/arm64/")])
elif env["platform"] == "ios":
    filepath = "{}.framework/".format(env["platform"])
    file = "{}{}".format(libname, env["suffix"])

env.Append(LIBS=["Ultralight"])
env.Append(LIBS=["UltralightCore"])
env.Append(LIBS=["WebCore"])
env.Append(LIBS=["AppCore"])

libraryfile = "bin/{}/{}{}".format(env["platform"], filepath, file)
library = env.SharedLibrary(
    libraryfile,
    source=sources,
)

copy = env.InstallAs("{}/addons/{}/bin/{}/{}/{}{}".format(projectdir, libname, env["platform"], env["arch"], filepath, file), library)

default_args = [library, copy]
Default(*default_args)

if env["platform"] == "windows":
    Execute(Copy(
        f"{projectdir}/addons/{libname}/bin/windows/",
        [
            "ultralight/bin/windows/AppCore.dll",
            "ultralight/bin/windows/Ultralight.dll",
            "ultralight/bin/windows/UltralightCore.dll",
            "ultralight/bin/windows/WebCore.dll"
        ]
    ))
elif env["platform"] == "linux":
    Execute(Copy(
        f"{projectdir}/addons/{libname}/bin/linux/",
        [
            "ultralight/bin/linux/libAppCore.so",
            "ultralight/bin/linux/libUltralight.so",
            "ultralight/bin/linux/libUltralightCore.so",
            "ultralight/bin/linux/libWebCore.so"
        ]
    ))
elif env["platform"] == "macos":
    Execute(Copy(
        f"{projectdir}/addons/{libname}/bin/macos/{env["arch"]}/",
        [
            f"ultralight/bin/macos/{env["arch"]}/libAppCore.dylib",
            f"ultralight/bin/macos/{env["arch"]}/libUltralight.dylib",
            f"ultralight/bin/macos/{env["arch"]}/libUltralightCore.dylib",
            f"ultralight/bin/macos/{env["arch"]}/libWebCore.dylib"
        ]
    ))

Execute(Copy(
    f"{projectdir}/addons/{libname}/resources/",
    [ 
        "ultralight/resources/cacert.pem",
        "ultralight/resources/icudt67l.dat"
    ]
))
