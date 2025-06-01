#!/usr/bin/python3

from bcc import BPF
from pathlib import Path
import os

if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.abspath(__file__))
    btf_path = os.path.join(script_dir, "BTF","{}.bpf.c".format(Path(__file__).stem))
    if os.path.exists(btf_path):
        with open(btf_path, 'r') as f:
            btf_file = f.read()
        BPF(text=btf_file).trace_print()
    else:
        print(f"BTF file is not exist: {btf_path}")