import subprocess
from tqdm import tqdm_notebook

from verygaplugin.verygaplugin import VeryGAPlugin


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

done_steps = []


def load_ipython_extension(ipython):
    global install_steps, done_steps

    try:
      for steps in tqdm_notebook(install_steps.items(),desc="Install Requirements"):
        if steps[0] not in done_steps:
          for command in tqdm_notebook(steps[1],desc=f"Install {steps[0]}"):
            output = subprocess.check_output(command,stderr=subprocess.STDOUT, shell=True, executable="/bin/bash")
          done_steps.append(steps[0])
    except subprocess.CalledProcessError as e:
      for l in e.output.decode("utf8").split("\n"):
          print(l)
            
    ipython.register_magics(VeryGAPlugin)