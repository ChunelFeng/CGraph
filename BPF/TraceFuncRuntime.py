#!/usr/bin/python3

from bcc import BPF
import time
import os
from pathlib import Path
import argparse

def path_check(path):
    if os.path.exists(path):
        return path
    else:
        raise argparse.ArgumentTypeError(f"The path {path} does not exist")

def print_event(cpu, data, size):
    event = b["events"].event(data)
    print(f"PID: {event.pid} TID: {event.tid}| Duration: {event.duration_ns / 1e6:.5f} ms.")

if __name__ == '__main__':

    examples = """examples:
    sudo python3 TraceFuncRuntime.py -b /usr/bin/helloword -f hello                                        # trace c lang project hello() function
    sudo python3 TraceFuncRuntime.py -b /usr/bin/test-performance-04 -f _ZN6CGraph8DAnnNode3runEv          # trace c++ lang project AnnNode class's run() function
"""

    parser = argparse.ArgumentParser(
        description="Trace a function's runtime",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=examples)

    parser.add_argument("-b", "--binary", type=path_check, dest="trace_binary", required=True,
        help="trace binary path, like /usr/bin/helloword, use absolute path")
    parser.add_argument("-f", "--function", type=str, dest="trace_function", required=True,
        help="user trace function name")

    args = parser.parse_args()

    script_dir = os.path.dirname(os.path.abspath(__file__))
    btf_path = os.path.join(script_dir, "BTF","{}.bpf.c".format(Path(__file__).stem))
    if os.path.exists(btf_path):
        with open(btf_path, 'r') as f:
            btf_text = f.read()
        b = BPF(text=btf_text)
        b.attach_uprobe(name=args.trace_binary, sym=args.trace_function, fn_name="cgraph_trace_entry")
        b.attach_uretprobe(name=args.trace_binary, sym=args.trace_function, fn_name="cgraph_trace_return")

        b["events"].open_perf_buffer(print_event)
        while True:
            try:
                b.perf_buffer_poll()
            except KeyboardInterrupt:
                exit()
    else:
        print(f"BTF file is not exist: {btf_path}")