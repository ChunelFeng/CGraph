"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: setup
@Time: 2025/3/6 23:25
@Desc:
"""

import glob
from setuptools import setup, Extension
import pybind11

_sources = ['PyCGraph.cpp'] + glob.glob("../src/**/*.cpp", recursive=True)
_extra_compile_args = ["-pthread", "-std=c++11", '-fvisibility=hidden']
_include_dirs = [pybind11.get_include(), "../src"]

ext_modules = [
    Extension(
        name = "PyCGraph",
        sources = _sources,
        extra_compile_args = _extra_compile_args,
        include_dirs = _include_dirs,
    ),
]

setup(
    name = "PyCGraph",
    version = "1.0.0",
    author = "Chunel Feng",
    author_email = "chunel@foxmail.com",
    description = "CGraph with python api wrapper by pybind11",
    url = "https://github.com/ChunelFeng/CGraph",
    license = "Apache-2.0",
    ext_modules = ext_modules,
    zip_safe = False,
)
