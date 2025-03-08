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
    env.Append(LIBPATH=[f"ultralight/bin/linux/{env["arch"]}/"])
elif env["platform"] == "macos":
    env.Append(LIBPATH=[f"ultralight/bin/macos/{env["arch"]}/"])
    if env["arch"] == "arm64":
        env.Append(LINKFLAGS=['-arch', 'arm64', '-rpath', os.path.abspath("ultralight/bin/macos/arm64/")])
    if env["arch"] == "x86_64":
        env.Append(LINKFLAGS=['-arch', 'x86_64', '-rpath', os.path.abspath("ultralight/bin/macos/x86_64/")])
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

copy_libraries = None

if env["platform"] == "windows":
    copy_libraries = env.Install(
        f"{projectdir}/addons/{libname}/bin/windows/{env["arch"]}",
        source = [
            f"ultralight/bin/windows/AppCore.dll",
            f"ultralight/bin/windows/Ultralight.dll",
            f"ultralight/bin/windows/UltralightCore.dll",
            f"ultralight/bin/windows/WebCore.dll"
        ]
    )
elif env["platform"] == "linux":
    copy_libraries = env.Install(
        f"{projectdir}/addons/{libname}/bin/linux/{env["arch"]}",
        source = [
            f"ultralight/bin/linux/{env["arch"]}/libAppCore.so",
            f"ultralight/bin/linux/{env["arch"]}/libUltralight.so",
            f"ultralight/bin/linux/{env["arch"]}/libUltralightCore.so",
            f"ultralight/bin/linux/{env["arch"]}/libWebCore.so"
        ]
    )
elif env["platform"] == "macos":
    copy_libraries = env.Install(
        f"{projectdir}/addons/{libname}/bin/macos/{env["arch"]}",
        source = [
            f"ultralight/bin/macos/{env["arch"]}/libAppCore.dylib",
            f"ultralight/bin/macos/{env["arch"]}/libUltralight.dylib",
            f"ultralight/bin/macos/{env["arch"]}/libUltralightCore.dylib",
            f"ultralight/bin/macos/{env["arch"]}/libWebCore.dylib"
        ]
    )

copy_resources = env.Install(
    f"{projectdir}/addons/{libname}/resources",
    source = [ 
        "ultralight/resources/cacert.pem",
        "ultralight/resources/icudt67l.dat"
    ]
)

default_args = [library, copy, copy_libraries, copy_resources]
Default(*default_args)
