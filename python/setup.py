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
import os
import sys
import subprocess
from setuptools.command.build_ext import build_ext as _build_ext

__PYCGRAPH_NAME__ = "PyCGraph"
__PYCGRAPH_VERSION__ = "3.1.2"
__PYCGRAPH_AUTHOR__ = "Chunel"
__PYCGRAPH_AUTHOR_EMAIL__ = "chunel@foxmail.com"
__PYCGRAPH_DESCRIPTION__ = "CGraph with python api wrapper by pybind11"
__PYCGRAPH_URL__ = "https://github.com/ChunelFeng/CGraph"
__PYCGRAPH_LICENSE__ = "MIT"
__PYCGRAPH_LONG_DESCRIPTION__ = 'You can get everything in CGraph github main page : [CGraph](https://github.com/ChunelFeng/CGraph)'
__PYCGRAPH_KEYWORDS__ = [__PYCGRAPH_NAME__,
                         "CGraph",
                         "dag",
                         "parallel",
                         "airflow",
                         "taskflow",
                         "workflow",
                         "pybind11"]

_sources = ['PyCGraph.cpp'] + glob.glob("../src/**/*.cpp", recursive=True)
_extra_compile_args = ["-pthread",
                       "-std=c++11",
                       "-fvisibility=hidden"]
_include_dirs = [pybind11.get_include(),
                 "../src"]
_ext_modules = [
    Extension(
        name=__PYCGRAPH_NAME__,
        sources=_sources,
        extra_compile_args=_extra_compile_args,
        include_dirs=_include_dirs,
    ),
]

class build_ext(_build_ext):
    def run(self):
        super().run()
        # 自动生成 stub 文件
        try:
            import pybind11_stubgen
        except ImportError:
            subprocess.check_call([sys.executable, "-m", "pip", "install", "pybind11-stubgen"])
        # 生成 stub 文件到当前目录
        subprocess.check_call([
            sys.executable, "-m", "pybind11_stubgen", __PYCGRAPH_NAME__, "--output-dir=."
        ])
        # 拷贝生成的 .pyi 文件到包目录
        build_lib = self.build_lib
        stub_path = os.path.join(os.path.dirname(__file__), f"{__PYCGRAPH_NAME__}.pyi")
        dst_pyi = os.path.join(os.path.dirname(__file__), build_lib, "PyCGraph.pyi")
        if os.path.exists(stub_path):
            import shutil
            shutil.copy(stub_path, dst_pyi)

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
    include_package_data=True,
    package_data={"": ["*.pyi"]},  # 关键：包含所有 .pyi 文件
    cmdclass={"build_ext": build_ext},
    long_description=__PYCGRAPH_LONG_DESCRIPTION__,
    long_description_content_type="text/markdown",
    python_requires=">=3.8",
    keywords=__PYCGRAPH_KEYWORDS__,
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Developers",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: Python :: 3.13",
        "Programming Language :: C++",
        "Operating System :: Unix",
        "Operating System :: MacOS :: MacOS X",
        "Operating System :: Microsoft :: Windows",
        "License :: OSI Approved :: MIT License",
        "Natural Language :: Chinese (Simplified)",
        "Natural Language :: English",
    ]
)
