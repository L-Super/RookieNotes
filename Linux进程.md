# 进程

描述程序执行过程和资源共享的基本单位

目的：控制和协调程序的执行

**进程相关函数：**

+ 用户与组ID函数

+ 创建进程：`system()`、`fork()`、`exec()`

+ 终止进程：`kill()`

+ 等待进程终止：`wait()`、`waitpid()`

## 进程组

有一个或多个相关联的进程组成，目的是为了进程控制。

特征：

+ 信号可以发送给进程组中所有的进程，并使该进程组中的所有进程终止、停止或继续运行。

+ 每个进程都属于某个进程组

函数：

+ 获取进程组ID：`pid_t getpgid(pid_t pid);`
  + 返回pid进程的进程组ID；若pid为0，则返回当前进程的进程组ID；出错时返回-1，并设errno值。
+ 设置进程组ID：`int setpgid(pid_t pid, pig_t pgid);`
  + pid为0，则使用调用者PID；若pgid为0，则将pid进程的进程PID设为进程组ID；成功时返回0.出错返回-1，并设errno值。

## 会话（session）

会话为一个或多个进程组的集合，包括登录用户的全部活动，并具有一个控制终端。

登录进程为每个用户创建一个会话，用户登录shell进程成为会话首领。其PID设为会话ID。

非会话首领进程通过调用`setsid()`创建新会话，并成为首领。

函数：

+ 获取会话ID：`pid_t getsid(pid_t pid);`
  + 返回pid进程的会话ID；若pid为0，则返回当前进程的会话ID；成功时返回会话ID，出错时返回-1，并设errno值。
+ 设置会话ID：`pid_t setsid();`
  + 成功时返回新创建的会话ID，出错时返回-1，并设errno值。

## 信号

信号（signal）：进程通讯机制

信号是发送给进程的特殊异步消息

当进程接收到信息时立即处理，此时并不需要完成当前函数调用甚至当前代码行。

`kill -l`查看系统支持的信号列表

![image-20220228110343838](https://gitee.com/listening2020/md_pic/raw/master/img-workpc/image-20220228110343838.png)

进程间发送的信号

- SIGTERM、SIGKILL：终止进程信号，前者是请求（接收信号的进程可以忽略），后者是强制
- SIGUSR、SIGUSR2：用户自定义信号，可用于向进程发送命令

信号处理

+ 进程接收到信号后，根据信号配置进行处理
+ 缺省配置：在程序没有处理时，确定信号该如何处理
+ 程序处理信号的方式：按照信号处理例程的函数指针形式定义一个函数，然后调用

### `sigaction()`函数

设置信号配置

+ 原型：`int sigaction(int signum, const struct sigaction* act,struct sigaction* oldact);`
+ signum为信号编号，act和oldact分别为指向信号结构体struct sigaction的指针，前者为新配置，后者为需要保存的老配置

信号结构体`struct sigaction`

+ sa_handler：取值为SIG_DFL(使用信号缺省配置)、SIG_IGN(忽略该信号)或指向信号处理例程的函数指针（以信号编号为参数，无返回值）

处理信号时注意事项：

+ 信号是异步操作，当处理信号时，主程序非常脆弱
+ 信号处理例程应尽可能短小，甚至可能会被新信号所中断
+ 尽量不在信号处理例程中进行I/O操作，频繁调用系统或库函数
+ 信号处理例程中进行复杂的赋值操作是危险的，它可能不是原子操作，因而可能在执行期间被中断
+ 如果需要赋值，使用sig_atomic_t类型的全局变量

## 进程管理

### system()

用于在程序中执行一条命令

```c
int system(const char* cmd);
```

在shell中，系统会创建一个子进程运行被调命令。

如果shell不能运行，返回127；发生错误返回-1。

例如

```c
int ret_val = system("ls -l/");
```

### fork()

创建当前进程的副本作为子进程

```c
pid_t fork();
```

返回为0（新创建的子进程）和子进程的PID（父进程）

### kill()

终止进程

头文件`"sys/typeds.h"`和`"signal.h"`

```c
int kill(pid_t pid, int sig);
```

sig：进程终止信号SIGTERM

### wait()

等待进程结束函数

```c
pid_t wait(int* status);
pid_t waitpid(pid_t pid, int* status, int options);
```

阻塞主调进程，直到一个子进程结束

WEXITSTATUS：查看子进程的退出码

WIFEXITED：确定子进程的退出状态是正常退出，还是未处理信号导致的意外死亡。

### 子进程异步清除

SIGCHILD信号：子进程终止时，向父进程自动发送，编写此信号处理例程，异步清除子进程。

## 守护进程

创建守护进程的步骤：

1. 创建新进程：新进程将成为未来的守护进程
2. 守护进程的父进程退出：保证祖父进程确认父进程已结束，且守护进程不是组长进程
3. 守护进程创建新进程组和新会话：并成为两者的首进程，此时刚创建的新会话还没有关联控制终端。
4. 改变工作目录：守护进程一般随系统启动，工作目录不应继续使用继承的工作目录
5. 重设文件权限掩码：不需要继承任何掩码
6. 关闭所有文件描述符：不需要继承任何打开的文件描述符
7. 标准流重定向到`/dev/null`

### daemon()

守护进程创建函数，减轻编写守护进程负担

```c
int daemon(int nochdir, int noclose);
```

若nochdir为0，不更改工作目录；若noclose非0，不关闭所有打开的文件描述符；一般均设为0。

返回值：成功时返回0，失败时返回-1，并设置errno值

## 进程间通信

+ 管道：相关进程间的顺序通信
+ 进程信号量：进程间通信的同步控制机制
+ 共享内存：允许多个进程读写同一片内存区域
+ 映射内存：与共享内存意义相同，但与文件相关联
+ 消息队列：在进程间传递二进制块数据
+ 套接字：支持无关进程，甚至不同计算机进行通信

### 管道

允许单向通信的自动同步设备（半双工），为串行设备，数据的读取顺序与写入顺序完全相同。

用途：

只能用于有亲缘关系的进程，例如父进程和子进程之间通信

注意事项：

+ 管道数据容量有限，一般为一个内存页面

+ 如果写入速度超过读取速度，写入进程将阻塞，直到容量有空闲

+ 如果读取速度超过写入速度，读取进程将阻塞，直到管道有数据

#### pipe（）

创建管道

头文件`unistd.h`和`fcntl.h`

```c
int pip(int pipefd[2]);
```

0为读取文件描述符，1为写入文件描述符

返回值：成功时返回0，不成功时返回1，并设置errno值

#### 管道重定向

等位文件描述符：共享相同的文件位置和状态标志设置

**dup()函数**：

将两个文件描述符等位处理

```c
//创建`oldfd`的一份拷贝，选择数值最小的未用文件描述符作为新的文件描述符。
int dup(int olddf);
// 选择newfd作为新的文件描述符，拷贝前尝试关闭newfd
int dup2(int oldfd, int newfd);
```

返回值：成功时返回新文件描述符，失败时返回-1，并设errno值。

### 进程信号量

进程信号量：System V信号量

是一类特殊的计数器，其值为非负整数，用于进程或线程同步

可以使用同步机制确定进程的先后执行关系

头文件：`sys/types.h`、`sys/ipc.h`和`sys/sem.h`

**操作：**

+ 等待（wait）操作（P）：信号量的值递减1后返回；如果值为0，则阻塞，直到信号量值为正（其他进程或线程释放了信号量），然后递减1后返回
+ 发布（post）操作（V）：信号量的值递增1后返回；如果信号量值原为0，则其中一个等待该信号量的进程或线程取消阻塞

两个版本：进程信号量多用于进程同步，POSIX标准多用于线程同步。

使用进程信号量时的注意事项：

+ 每次创建和管理的进程信号不是一个，而是一个集合（数组），该集合可能包含多个进程信号量，集合可能包含多个进程信号量
+ 使用键值key关联进程信号量集，但进程信号量集本身由进程信号量的标识符semid标识，函数调用时几乎总是使用semid。可以理解为semid对内，key对外。

#### semget()

创建或获取进程信号量集

```c
int semget(key_t key, int nsems, int semflg);
```

key: 多个进程可以通过此键值访问同一进程信号量

nsems：需要创建的进程信号量集的进程信号量元素个数（不是进程信号量的信号数）

semflg：访问标志

返回值：成功时返回进程信号量集的标识符，失败时返回-1，并设置errno值。

要获取已分配的进程信号量集，使用原先键值查询，此时进程信号量的元素个数可设置为0

键值`IPC_PRIVATE`用于创建当前进程的私有进程信号量集

使用`IPC_CREAT`和`IPC_EXCL`创建进程信号量集，后者要求创建新的唯一的进程信号量集，若其已存在，则出错。

#### semctl()

控制和管理进程信号量集

```c
int semctl(int semid, int semnum, int cmd,...);
```

+ semid：进程信号量集的标识符

+ semnum：进程信号量集的元素下标

+ cmd：为指定操作

+ arg：可有可无，与cmd有关

返回值：成功时与cmd有关，失败时返回-1，并设置errno值

调用semctl()函数的进程的有效用户ID必须与分配进程信号量集的用户权限匹配

##### 清除进程信号量

释放（删除）进程信号量集：IP_RMID

+ 最后一个使用进程信号量的负责清除进程信号量集
+ 进程信号量集释放后，内存自动释放

说明：cmd为IPC_RMID，semnum被忽略，arg不需要；如果需要arg，定义union_semun类型的变量并作为参数，部分系统可能未定义union_semun类型。

```c
union semun
{
    int val;//供SETVAL使用的值
	stuctsemid_ds* buf;//供IPC_STAT、IPC_SET使用的缓冲区
	unsigned short int* array；//供GETALL、SETALL使用的数组
	struct seminfo* _buf;//供IPC_INFO使用的缓冲区
}
```

##### 初始化进程信号量

SETALL

#### setmop()

等待与发布

```c
int semop(int semid, struct sembuf* sops, size_t nsops);
```

+ semid：待操作的进程信号量集的标识符
+ sops：操作数组
+ nsops：操作数组的元素个数

返回值：成功时为进程信号量集的标识符，失败时返回-1，并设置errno值

### 共享内存

本地通信机制

头文件`sys/ipc.h`和`sys/shm.h`

原则：

系统没有对共享内存操作提供任何缺省同步行为，如果需要，自行设计同步策略（使用进程信号量）

#### 获取或分配一段共享内存 shmget()

```c
int shmget(key_t key, size_t size, int shmflg);
```

+ key：内存段整数键值
  + 其他进程通过键值key访问该内存段，任意设定的键值可能和其他进程的共享内存段键值冲突，使用IPC_PRIVATE以确保无冲突
+ size：内存段分配的字节数（4096字节的整数倍）
+ shmflg：创建共享内存段的位标志
  + IPC_CREAT（创建）
  + IPC_EXCL（独占）
  + 合并使用，如果键值已使用，则创建失败。如果未设IPC_EXCL，则键值已存在是，返回其代表的共享内存段，而不是创建一个新的共享内存段

#### 连接与拆卸共享内存

连接共享内存

```c
void* shmat(int shmid, const void* shmaddr, int shmflg);
```

- shmid：共享内存段标识符（shmget()的返回值）
- shmaddr：指向共享内存段的映射地址，如果传递NULL，Linux自动选择合适地址
- shmflg：连接标志
  - SHM_RND：shmaddr指定的映射地址向下圆整到页面尺寸的整数倍；如果未指定，则传递shmaddr时必须手工对齐页面地址
  - SHM_RDONLY：共享内存段组只读

返回值：成功时返回所连接的共享内存段的地址

拆卸共享内存段

```c
int shmdt(const void* shmaddr);
```

### 映射内存

映射共享文件到内存；文件被分割成页面大小装载；使用内存读写操作访问文件，速度更快；对映射内存的写入自动反映到文件中

头文件`sys/mman.h`

```c
void* mmap(void* addr,size_t length, int prot, int flags, int fd, off_t offset);
```

- addr：映射目的地的内存地址，NULL表示有Linux自动选择合适的内存地址
- length：映射内存的大小，以字节为单位
- prot：指定映射内存的保护权限，为PROT_READ（允许读取）、PROT_WRITE(允许写入)、PROT_EXEC（允许执行）或三者的组合
- flag：附加选项标志
  - MAP_FIXED：如果指定此标志，则Linux使用用户指定的地址映射文件，地址本身必须页对齐
  - MAP_PRIVATE：内存写入不回写至外部文件，本进程保留写入的文件副本
  - MAP_SHARED：内存写入立即反映到映射文件中
  - MAP_PRIVATE与MAP_SHARED不能混用
- fd：待映射文件的文件描述符
- offset：指定映射数据在文件中的偏移量

返回值：成功调用时返回映射内存的基地址，失败时返回MAP_FAILED

释放映射内存

```c
int* munmap(void* addr, size_t length);
```

- addr：映射内存的基地址
- length：映射内存的大小

返回值：成功时返回0，失败时返回-1并设errno值

映射内存同步

```c
int* msync(void* addr, size_t length, int flags);
```

- addr：映射内存的基地址
- length：映射内存的大小
- flag：同步标志
  - MS_ASYNC：数据更新被调度，但函数返回前并不一定会被执行
  - MS_SYNC：数据更新立即执行，在完成前调用进程被阻塞
  - MS_INVALIDATE：通知其他进程数据已无效，并自动提供新数据
  - MS_ASYNC与MS_SYNC不能混用

返回值：成功时返回0，失败时返回-1并设errno值

### 消息队列

在两个进程间传递二进制块数据

数据块具有类别信息，接收方可根据消息类别有选择地接收

头文件：`sys/typeh`、`sys/ipc.h`和`sys/msg.h`

**创建或获取消息队列**

```c
int msgget(ket_t key, int msgflg);
```

- key：为键值，标识全局唯一的消息队列
- msgflg：为创建标志，与semget()的标志相同

返回值：成功时返回正整数作为消息队列的标识符，失败时返回-1，并设errno值

如果用于创建消息队列，相关内核数据结构`struct msqid_ds`将被创建并初始化

**将消息添加到消息队列中**

```c
int msgsnd(int msqid, const void* msg_ptr, size_t msg_sz, int msgflg);
```

- msqid为msggetO返回的消息队列标识符
- msg_ptr指向准备发送的消息；
- msg_sz为消息数据长度
- msgflg控制消息发送行为，一般仅支持IPC_NOWAIT标志，即以非阻塞的方式发送消息

返回值：成功时返回0，失败时返回-1并设errno值

**控制消息队列的某些属性**

```c
int msgctl(int msqid, int cmd, struct msqid_ds* buf);
```

- msqid为msggetO返回的消息队列标识符
- cmd指定要执行的命令，支持的命令有IPC_STAT、IPC_SET、IPC RMID、IPC INFO、MSG INFO、MSG_STAT
- buf的意义与cmd参数有关

返回值：成功时返回值取决于cmd参数，失败时返回-1并设errno值

# 线程

+ 内核线程

+ 轻量级进程（LWP）

- 用户线程

## 线程创建

头文件`pthread.h`

```c
int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine)(void*),void* arg);
```



//TODO：、、、、

## 线程同步机制

### 互斥



