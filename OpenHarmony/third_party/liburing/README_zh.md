# liburing

由于直接使用系统调用较为复杂，Jens Axboe 还提供了封装好的用户态库liburing，简化了io_uring的使用。
liburing提供了初始化和释放io_uring实例的帮助程序，以及为不需要（或不希望）处理完整内核端实现的应用程序提供简化的接口。

## 目录结构

```
debian/
example/                #样例代码
man/
src/liburing            #库源代码
test/                   #测试脚本代码
```


## OpenHarmony如何使用liburing

OpenHarmony中系统部件需要在BUILD.gn中引用liburing部件以使用liburing。

```
// BUILD.gn
external_deps += [ "liburing:liburing" ]
```

## 使用liburing库开发步骤

1. 初始化
   ```
   extern int io_uring_queue_init_params(unsigned entries, struct io_uring ring,struct io_uring_params p);
   extern int io_uring_queue_init(unsigned entries, struct io_uring ring, unsigned flags);
   entries 表示队列大小
   ring 就是需要初始化的io_uring结构指针
   flags是标志参数，此值会改变io_uring_params p->flags
   io_uring_params *p更多的设置
   ```
2. 创建请求（获取一个sqe请求并初始化）
   ```
   extern struct io_uring_sqe io_uring_get_sqe(struct io_uring ring);
   static inline void io_uring_prep_readv(struct io_uring_sqe sqe,int fd, const struct iovec iovecs, unsigned nr_vecs,off_t offset);
   static inline void io_uring_prep_writev(struct io_uring_sqe sqe,int fd, const struct iovec iovecs, unsigned nr_vecs,off_t offset);
   sqe即前面获取的sqe结构指针
   fd为需要读写的文件描述符，可以是磁盘文件也可以是socket
   iovecs为iovec数组，具体使用请参照readv和writev
   nr_vecs 为iovecs数组元素个数
   offset  为文件操作的偏移量
   ```
3. 传入用户数据
   ```
   static inline void io_uring_sqe_set_data(struct io_uring_sqe *sqe, void *data);
   ```
4. 提交请求
   ```
   extern int io_uring_submit(struct io_uring *ring);
   extern int io_uring_submit_and_wait(struct io_uring *ring, unsigned wait_nr);
   wait_nr    等待事件数量
   ```
5. 获取结果（提取完成事件）
   ```
   static inline int io_uring_peek_cqe(struct io_uring *ring, struct io_uring_cqe **cqe_ptr);
   static inline int io_uring_wait_cqe(struct io_uring *ring, struct io_uring_cqe **cqe_ptr);

    cqe_ptr    输出参数，是cqe指针变量的地址
    注意：io_uring_peek_cqe如果没有已完成的IO操作时，也会立即返回，cqe_ptr被置空；而io_uring_wait_cqe会阻塞线程，等待IO操作完成。
   ```
6. 获取数据
   ```
   static inline void *io_uring_cqe_get_data(const struct io_uring_cqe *cqe);
   ```
7. 清理处理完成的结果
   ```
   static inline void io_uring_cqe_seen(struct io_uring *ring, struct io_uring_cqe *cqe);
   ```
8. 释放
   ```
   extern void io_uring_queue_exit(struct io_uring *ring);
   ```

## 相关仓<a name="section178mcpsimp"></a>

- [**基础文件访问接口**](https://gitee.com/openharmony/filemanagement_file_api)
