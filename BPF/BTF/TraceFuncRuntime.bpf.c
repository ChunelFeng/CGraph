#include <uapi/linux/ptrace.h>

struct data_t {
    u64 pid;
    u32 tid;
    u64 duration_ns;
};

BPF_HASH(start, u64, u64);
BPF_PERF_OUTPUT(events);

int cgraph_trace_entry(struct pt_regs *ctx) {
    u64 pid = bpf_get_current_pid_tgid();
    u64 ts = bpf_ktime_get_ns();
    start.update(&pid, &ts);
    
    return 0;
}

int cgraph_trace_return(struct pt_regs *ctx) {
    u64 pid = bpf_get_current_pid_tgid();
    u64 *start_ts = start.lookup(&pid);
    if (!start_ts) return 0;

    u64 end_ts = bpf_ktime_get_ns();
    u64 duration = end_ts - *start_ts;

    struct data_t data = {};
    data.pid = pid >> 32;
    data.tid = bpf_get_current_pid_tgid();
    data.duration_ns = duration;

    events.perf_submit(ctx, &data, sizeof(data));
    start.delete(&pid);
    return 0;
}