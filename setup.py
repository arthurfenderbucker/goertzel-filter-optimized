from distutils.core import setup, Extension
MOD = "goertzel"
setup(name = MOD, ext_modules = [Extension(MOD,sources=['goertzel.c'])],
description = "C Extension Module for python 3 with goertzel filter implementation")
