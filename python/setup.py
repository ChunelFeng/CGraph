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

__PYCGRAPH_NAME__ = "PyCGraph"
__PYCGRAPH_VERSION__ = "1.5.1"
__PYCGRAPH_AUTHOR__ = "Chunel"
__PYCGRAPH_AUTHOR_EMAIL__ = "chunel@foxmail.com"
__PYCGRAPH_DESCRIPTION__ = "CGraph with python api wrapper by pybind11"
__PYCGRAPH_URL__ = "https://github.com/ChunelFeng/CGraph"
__PYCGRAPH_LICENSE__ = "MIT"

_sources = ['PyCGraph.cpp'] + glob.glob("../src/**/*.cpp", recursive=True)
_extra_compile_args = ["-pthread", "-std=c++11", '-fvisibility=hidden']
_include_dirs = [pybind11.get_include(), "../src"]
_ext_modules = [
    Extension(
        name=__PYCGRAPH_NAME__,
        sources=_sources,
        extra_compile_args=_extra_compile_args,
        include_dirs=_include_dirs,
    ),
]

setup(
    name=__PYCGRAPH_NAME__,
    version=__PYCGRAPH_VERSION__,
    author=__PYCGRAPH_AUTHOR__,
    author_email=__PYCGRAPH_AUTHOR_EMAIL__,
    description=__PYCGRAPH_DESCRIPTION__,
    url=__PYCGRAPH_URL__,
    license=__PYCGRAPH_LICENSE__,
    ext_modules=_ext_modules,
    zip_safe=False,
)
