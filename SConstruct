#!/usr/bin/env python
import os
import sys


libname = "verylatega"


try:
    Import("env")
except Exception:
    # Default tools with no platform defaults to gnu toolchain.
    # We apply platform specific toolchains via our custom tools.
    env = Environment(tools=["default"], PLATFORM="")

env.PrependENVPath("PATH", os.getenv("PATH"))

if "platform" not in env:
    env.Append(platform="linux")

if "sufix" not in env:
    env.Append(suffix="")

if "build_library" not in env:
    env.Append(build_library="yes")

# Custom options and profile flags.
customs = ["custom.py"]
try:
    customs += Import("customs")
except Exception:
    pass
profile = ARGUMENTS.get("profile", "")
if profile:
    if os.path.isfile(profile):
        customs.append(profile)
    elif os.path.isfile(profile + ".py"):
        customs.append(profile + ".py")
opts = Variables(customs, ARGUMENTS)
opts.Update(env)

Help(opts.GenerateHelpText(env))


scons_cache_path = os.environ.get("SCONS_CACHE")
if scons_cache_path is not None:
    CacheDir(scons_cache_path)
    Decider("MD5")

verilate = Builder(
    action="verilator --cc -y $INCLUDE_DIRS --top $TOP_MODULE \
                   --Mdir $MDIR $VERILATOR_ARGS $SOURCE"
)

env.Append(BUILDERS={"Verilate": verilate})

opts = "'-O3 -fstrict-aliasing -march=native'"

include_dirs = "../VeryMaze/designs"

top_module = "veryga"

verilator_args = "-O3 --x-assign fast --x-initial fast --noassert"

verilator_args += (
    " --debug" if "target" in env and env["target"] == "template_debug" else ""
)

source = "../VeryMaze/designs/veryga.v"

target = ["./obj_dir/Vveryga.h", "./obj_dir/Vveryga.cpp"]

Mdir = env.Dir("./obj_dir")

env.Verilate(
    OPT_FAST=[opts],
    OPT_SLOW=[opts],
    OPT_GLOBAL=[opts],
    INCLUDE_DIRS=include_dirs,
    TOP_MODULE=top_module,
    MDIR=Mdir,
    source=source,
    target=target,
)


CCFLAGS = "-O3 -march=native  -MMD  -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TIMING=0\
            -DVM_TRACE=0 -DVM_TRACE_FST=0  -DVM_TRACE_VCD=0 -faligned-new \
            -fcf-protection=none -Wno-bool-operation  -Wno-shadow \
            -Wno-sign-compare -Wno-tautological-compare -Wno-uninitialized \
            -Wno-unused-but-set-parameter  -Wno-unused-but-set-variable \
            -Wno-unused-parameter -Wno-unused-variable -fstrict-aliasing -fPIC -DVL_IGNORE_UNKNOWN_ARCH"

env.Append(CPPPATH=[env.Dir("/usr/local/share/verilator/include")])
env.Append(CPPPATH=[env.Dir("/usr/local/share/verilator/include/vltstd")])

target = "./obj_dir/verilated" + env["OBJSUFFIX"]
source = "/usr/local/share/verilator/include/verilated.cpp"

obj_verilated = StaticObject(target, source, CCFLAGS=CCFLAGS)

target = "./obj_dir/verilated_thread" + env["OBJSUFFIX"]
source = "/usr/local/share/verilator/include/verilated_threads.cpp"

obj_verilated_thread = StaticObject(target, source, CCFLAGS=CCFLAGS)

sources = [obj_verilated_thread, obj_verilated]

v_name = "./obj_dir/verilated" + env["LIBSUFFIX"]

verilated_library = StaticLibrary(
    v_name, sources, CCFLAGS=CCFLAGS
)

env.Append(CPPPATH=[env.Dir("./obj_dir")])


sources = Glob("./obj_dir/*.cpp")

library_name = "Vveryga" + env["LIBSUFFIX"]

vveryga_lib = env.StaticLibrary("./obj_dir/%s" % library_name, sources, LIBS=[verilated_library], CCFLAGS=CCFLAGS)

env.Append(CPPPATH=[env.Dir("./src")])

sources = ["./src/verylatega.cpp"]

library_name = libname + env["SHLIBSUFFIX"]


if env["build_library"]:
    
    library = env.SharedLibrary("./bin/%s" % library_name, sources, LIBS=[vveryga_lib,verilated_library], CCFLAGS=CCFLAGS)

    default_args = [library]

    env.Default(*default_args)

env.AppendUnique(LIBS=[library,vveryga_lib,verilated_library])

Return("env")
