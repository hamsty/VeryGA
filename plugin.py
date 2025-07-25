import subprocess
from tqdm import tqdm_notebook

from verygaplugin.verygaplugin import VeryGAPlugin

def load_ipython_extension(ipython):
    """
    Any module file that define a function named `load_ipython_extension`
    can be loaded via `%load_ext module.path` or be configured to be
    autoloaded by IPython at startup time.
    """
    # You can register the class itself without instantiating it.  IPython will
    # call the default constructor on it.

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