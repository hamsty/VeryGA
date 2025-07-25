from google.colab import output as goutput
from IPython.core.magic import Magics, cell_magic, magics_class
from IPython.display import clear_output
import subprocess, argparse, os
from tqdm import tqdm_notebook





install_steps = {
    "CMAKE":[
    "apt-get update",
    "apt-get install cmake clang"], 
    "Verilator":[
    "chmod 777 /content/VeryGA/verilator/bin/verilator_bin",
    "mkdir VeryMaze",
    "mkdir VeryMaze/designs",
    "mkdir VeryGA/build",
    "cp /content/VeryGA/assets/ /content/VeryGA/build/ -d -r",
    "apt-get install help2man perl libfl2 libfl-dev zlib1g zlib1g-dev ccache \
    mold libgoogle-perftools-dev numactl perl-doc"], 
    "EMSCRIPTEN": [
    "git clone https://github.com/emscripten-core/emsdk.git",
    "./emsdk/emsdk install latest",
    "./emsdk/emsdk activate latest",
    "source ./emsdk/emsdk_env.sh"]
}

compile_commands = [
    "./emsdk/upstream/emscripten/emcmake cmake ./VeryGA/ -DVERILATOR_ROOT=/content/VeryGA/verilator -Dverilator_DIR=/content/VeryGA/verilator -B VeryGA/build",
    "rm VeryGA/build/CMakeFiles/verylatega.dir/linkLibs.rsp",
    "echo '-mt' >> VeryGA/build/CMakeFiles/verylatega.dir/linkLibs.rsp",
    "cd VeryGA/build && make"
]

server_up = False

old_cell = ""

@magics_class
class VeryGAPlugin(Magics):

   def __init__(self, shell):
        super(VeryGAPlugin, self).__init__(shell)

   @cell_magic
   def veryga(self, line, cell):
        global server_up, old_cell
        try:
          argparser = argparse.ArgumentParser("VeryGA")
          argparser.add_argument("--top", "-t", action='store_true')
          argparser.add_argument("filename", type= str, nargs="?")
          args = argparser.parse_args(line.split(" "))
          filename = "veryga.v"
          if not args.top:
            if (filename is not None) and (len(filename) > 0): 
              filename = args.filename if args.filename.endswith(".v") else f"{filename}.v"
            else:
              argparser.print_help()
          with open(os.path.join("/content/VeryMaze/designs",filename),"w") as verygav:
            verygav.write(cell)
          if args.top:
            if old_cell != cell:
              for command in tqdm_notebook(compile_commands,desc=f"Compiling"):
                output = subprocess.check_output(command,stderr=subprocess.STDOUT, shell=True, executable="/bin/bash")
            else:
              print("No changes detected, skipping compile!")
            old_cell = cell
            if not server_up:
              get_ipython().system_raw("python -m http.server --directory VeryGA/build &")
              server_up = True
            clear_output(wait=True)
            goutput.serve_kernel_port_as_iframe(8000, height=800)
          else:
            print(f"{filename} saved!")
        except subprocess.CalledProcessError as e:
          for l in e.output.decode("utf8").split("\n"):
              print(l)
      
