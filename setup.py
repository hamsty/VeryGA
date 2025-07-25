from VeryGAPlugin.verygaplugin import VeryGAPlugin
import os

os.system("git clone https://github.com/hamsty/VeryGA.git /content/VeryGA")

from distutils.core import setup

setup(
    name="VeryGAPlugin",
    version="1.0",
    author="Tálles de Sousa Costa",
    author_email="tallesdesousacosta@gmail.com",
    py_modules= [verygaplugin.VeryGAPlugin],
    url="https://github.com/hamsty/VeryGA",
    license="LICENSE",
    description="Google Colaboratory plugin to run Verilog VGA Simulation"
)