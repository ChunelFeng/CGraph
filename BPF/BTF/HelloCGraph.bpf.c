int kprobe__sys_clone(void *ctx) { 
    bpf_trace_printk("Hello, CGraph!\\n"); 
    return 0; 
}