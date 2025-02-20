#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import glob
from setuptools import setup, Extension
import pybind11

sources = ['PyCGraph.cpp'] + glob.glob("../src/**/*.cpp", recursive=True)
extra_compile_args = ["-pthread", "-std=c++11", '-fvisibility=hidden']
include_dirs = [pybind11.get_include(), "../src"]

ext_modules = [
    Extension(
        name = "PyCGraph",
        sources = sources,
        extra_compile_args = extra_compile_args,
        include_dirs = include_dirs,
    ),
]

setup(
    name="PyCGraph",
    version="0.0.1",
    author="Chunel Feng",
    description="CGraph with python API",
    ext_modules=ext_modules,
    zip_safe=False,
)
