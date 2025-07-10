/*
* Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved.

* The fstrace_progs.h is dual licensed: you can use it either under the terms of
* the GPL V2, or the 3-Clause BSD license, at your option.
* See the LICENSE file in the root of this repository for complete details.
*/

/* This line is generated to truncate the original file */
/************************** fstrace BPF progs BEGIN *****************************/
#ifndef FSTRACE_PROGS_H
#define FSTRACE_PROGS_H

SEC("kprobe/do_sys_openat2")
int BPF_KPROBE(do_sys_openat2_entry, int dfd, const char __user* filename, struct open_how* how)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_OPENAT2;
    struct sys_openat2_args_t* args = &fs_se->openat2_args;
    args->dfd = dfd;
    args->filename = filename;
    emit_strtrace_event(fs_se->stime, fs_se->type, filename, FSTRACE);
    args->how = how;

    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

SEC("kretprobe/do_sys_openat2")
int BPF_KRETPROBE(do_sys_openat2_exit, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}

SEC("kprobe/ksys_read")
int BPF_KPROBE(ksys_read_entry, unsigned int fd, char __user* buf, size_t count)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_READ;
    struct sys_read_args_t* args = &fs_se->read_args;
    args->fd = fd;
    args->buf = buf;
    args->count = count;

    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

SEC("kretprobe/ksys_read")
int BPF_KRETPROBE(ksys_read_exit, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}

SEC("kprobe/ksys_write")
int BPF_KPROBE(ksys_write_entry, unsigned int fd, const char __user* buf, size_t count)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_WRITE;
    struct sys_write_args_t* args = &fs_se->write_args;
    args->fd = fd;
    args->buf = buf;
    args->count = count;

    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

SEC("kretprobe/ksys_write")
int BPF_KRETPROBE(ksys_write_exit, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}

SEC("kprobe/ksys_pread64")
int BPF_KPROBE(ksys_pread64_entry, unsigned int fd, char __user* buf, size_t count, loff_t pos)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_PREAD64;
    struct sys_pread64_args_t* args = &fs_se->pread64_args;
    args->fd = fd;
    args->buf = buf;
    args->count = count;
    args->pos = pos;

    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

SEC("kretprobe/ksys_pread64")
int BPF_KRETPROBE(ksys_pread64_exit, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}

SEC("kprobe/ksys_pwrite64")
int BPF_KPROBE(ksys_pwrite64_entry, unsigned int fd, const char __user* buf, size_t count, loff_t pos)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_PWRITE64;
    struct sys_pwrite64_args_t* args = &fs_se->pwrite64_args;
    args->fd = fd;
    args->buf = buf;
    args->count = count;
    args->pos = pos;

    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

SEC("kretprobe/ksys_pwrite64")
int BPF_KRETPROBE(ksys_pwrite64_exit, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}

SEC("kprobe/do_readv")
int BPF_KPROBE(do_readv_entry, unsigned long fd, const struct iovec __user* vec, unsigned long vlen, rwf_t flags)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_READV;
    struct sys_readv_args_t* args = &fs_se->readv_args;
    args->fd = fd;
    args->vec = vec;
    args->vlen = vlen;
    args->flags = flags;

    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

SEC("kretprobe/do_readv")
int BPF_KRETPROBE(do_readv_exit, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}

SEC("kprobe/do_writev")
int BPF_KPROBE(do_writev_entry, unsigned long fd, const struct iovec __user* vec, unsigned long vlen, rwf_t flags)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_WRITEV;
    struct sys_writev_args_t* args = &fs_se->writev_args;
    args->fd = fd;
    args->vec = vec;
    args->vlen = vlen;
    args->flags = flags;

    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

SEC("kretprobe/do_writev")
int BPF_KRETPROBE(do_writev_exit, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}

SEC("kprobe/do_preadv")
int BPF_KPROBE(do_preadv_entry,
               unsigned long fd, const struct iovec __user* vec,
               unsigned long vlen, loff_t pos, rwf_t flags)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_PREADV;
    struct sys_preadv_args_t* args = &fs_se->preadv_args;
    args->fd = fd;
    args->vec = vec;
    args->vlen = vlen;
    args->pos = pos;
    args->flags = flags;

    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

SEC("kretprobe/do_preadv")
int BPF_KRETPROBE(do_preadv_exit, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}

static inline loff_t pos_from_hilo(unsigned long high, unsigned long low)
{
#define HALF_LONG_BITS (__SIZEOF_LONG__ * 4)
    return (((loff_t)high << HALF_LONG_BITS) << HALF_LONG_BITS) | low;
}

static __always_inline int pwritev_entry(unsigned long fd, const struct iovec __user* vec,
    unsigned long vlen, loff_t pos, rwf_t flags)
{
    if (check_current_pid(-1, -1) != 0) {
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_PWRITEV;
    struct sys_pwritev_args_t* args = &fs_se->pwritev_args;
    args->fd = fd;
    args->vec = vec;
    args->vlen = vlen;
    args->pos = pos;
    args->flags = flags;

    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

static __always_inline int pwritev_exit(void* ctx, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}

SEC("kprobe/__arm64_sys_pwritev")
int BPF_KPROBE(__arm64_sys_pwritev_entry,
               unsigned long fd, const struct iovec __user * vec,
               unsigned long vlen, unsigned long pos_l, unsigned long pos_h)
{
    loff_t pos = pos_from_hilo(pos_h, pos_l);
    return pwritev_entry(fd, vec, vlen, pos, 0);
}

SEC("kretprobe/__arm64_sys_pwritev")
int BPF_KRETPROBE(__arm64_sys_pwritev_exit, int64_t retval)
{
    return pwritev_exit(ctx, retval);
}

SEC("kprobe/__arm64_sys_pwritev2")
int BPF_KPROBE(__arm64_sys_pwritev2_entry,
               unsigned long fd, const struct iovec __user * vec,
               unsigned long vlen, unsigned long pos_l, unsigned long pos_h, rwf_t flags)
{
    loff_t pos = pos_from_hilo(pos_h, pos_l);
    if (pos == -1) {
        return 0;
    }
    return pwritev_entry(fd, vec, vlen, pos, flags);
}

SEC("kretprobe/__arm64_sys_pwritev2")
int BPF_KRETPROBE(__arm64_sys_pwritev2_exit, int64_t retval)
{
    return pwritev_exit(ctx, retval);
}

SEC("kprobe/__arm64_compat_sys_pwritev")
int BPF_KPROBE(__arm64_compat_sys_pwritev_entry,
               compat_ulong_t fd, const struct iovec __user* vec,
               compat_ulong_t vlen, u32 pos_low, u32 pos_high)
{
    loff_t pos = ((loff_t)pos_high << 32) | pos_low;
    return pwritev_entry(fd, vec, vlen, pos, 0);
}

SEC("kretprobe/__arm64_compat_sys_pwritev")
int BPF_KRETPROBE(__arm64_compat_sys_pwritev_exit, int64_t retval)
{
    return pwritev_exit(ctx, retval);
}

SEC("kprobe/__arm64_compat_sys_pwritev2")
int BPF_KPROBE(__arm64_compat_sys_pwritev2_entry,
               compat_ulong_t fd, const struct iovec __user* vec,
               compat_ulong_t vlen, u32 pos_low, u32 pos_high, rwf_t flags)
{
    loff_t pos = ((loff_t)pos_high << 32) | pos_low;
    if (pos == -1) {
        return 0;
    }
    return pwritev_entry(fd, vec, vlen, pos, flags);
}

SEC("kretprobe/__arm64_compat_sys_pwritev2")
int BPF_KRETPROBE(__arm64_compat_sys_pwritev2_exit, int64_t retval)
{
    return pwritev_exit(ctx, retval);
}

SEC("kprobe/__close_fd")
int BPF_KPROBE(__close_fd_entry, struct files_struct* files, unsigned fd)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    struct start_event_t start_event = {};
    __builtin_memset(&start_event, 0, sizeof(start_event));
    struct fstrace_start_event_t *fs_se = &start_event.fs_se;
    u64 pid_tgid = bpf_get_current_pid_tgid();
    // get timestamp of the start of system call
    fs_se->stime = bpf_ktime_get_ns();
    // get argument of the system call
    fs_se->type = SYS_CLOSE;
    struct sys_close_args_t* args = &fs_se->close_args;
    args->files = files;
    args->fd = fd;
    emit_strtrace_event(fs_se->stime, fs_se->type, args, FSTRACE);
    // store the start event with pid as key
    int err = (int) bpf_map_update_elem(&start_event_map, &pid_tgid, &start_event, BPF_ANY);
    if (err != 0) {
        BPFLOGE(BPF_TRUE, "failed to store fstrace start event");
        return -1;
    }
    return 0;
}

SEC("kretprobe/__close_fd")
int BPF_KRETPROBE(__close_fd_exit, int64_t retval)
{
    if (check_current_pid(-1, -1) != 0) {
        // not any one of target processes, skip it
        return 0;
    }
    return emit_event(ctx, retval, FSTRACE);
}
/*************************** fstrace BPF progs END ******************************/
#endif // FSTRACE_PROGS_H
