Systemd 是 Linux 系统工具，用来启动[守护进程](https://www.ruanyifeng.com/blog/2016/02/linux-daemon.html)

# 创建流程
## 1. 创建 `.service` 配置文件
比如创建 `test.service`
```
[Unit]
Description=monitor deamon of python version app 
After=rc-local.service

[Service]
Type=simple
User=root
Group=root
WorkingDirectory=/root/monitor
ExecStart=/root/monitor/monitor
Restart=always

[Install]
WantedBy=multi-user.target
```
## 2. 移动配置文件到`/etc/systemd/system`
配置文件主要放在 `/usr/lib/systemd/system` 目录，也可能在 `/etc/systemd/system` 目录。目前在 centos 上是 `/etc/systemd/system`

```shell
cp /home/test.service /etc/systemd/system/
```
## 3. 启动service

```bash
systemctl start test.service
systemctl start httpd
```
带不带后缀似乎无差异

# 常用命令
## 开机启动
```shell
$ systemctl enable test.service

# 以上命令相当于执行以下命令，把test.service添加到开机启动中
$ sudo ln -s  '/etc/systemd/system/test.service'  '/etc/systemd/system/multi-user.target.wants/test.service' 

```
## 查看服务状态

```bash
sudo systemctl status test
```
执行后：
```shell
[root@bogon ~]# systemctl status monitor_py_ver
● monitor_py_ver.service - monitor deamon of python version app
   Loaded: loaded (/etc/systemd/system/monitor_py_ver.service; disabled; vendor preset: disabled)
   Active: active (running) since Wed 2022-12-28 11:04:43 CST; 7min ago
 Main PID: 284962 (monitor)
   CGroup: /system.slice/monitor_py_ver.service
           ├─284962 /root/monitor/monitor
           └─284972 /root/monitor/monitor

Dec 28 11:04:43 bogon systemd[1]: Started monitor deamon of python version app.
Dec 28 11:04:43 bogon monitor[284962]: [2022-12-28 11:04:43,602] [main] [INFO] [main.p...3.4
Dec 28 11:07:20 bogon monitor[284962]: [2022-12-28 11:07:20,346] 
[main] [INFO] [main.p...1}}
Hint: Some lines were ellipsized, use -l to show in full.
```
输出结果含义如下：

> - `Loaded`行：配置文件的位置，是否设为开机启动
> - `Active` 行：表示正在运行
> - `Main PID` 行：主进程 ID
> - `Status`行：由应用本身（这里是 httpd ）提供的软件当前状态
> - `CGroup`块：应用的所有子进程
> - 日志块：应用的日志

## 停止服务

终止正在运行的服务
**停掉该systemd时，服务也会随之停掉**


> ```bash
> systemctl stop test.service
> ```

有时候，该命令可能没有响应，服务停不下来。这时候就不得不"杀进程"了，向正在运行的进程发出`kill`信号。

> ```bash
systemctl kill test.service
> ```

此外，重启服务要执行`systemctl restart`命令。

> ```bash
systemctl restart test. service
> ```

## 重新加载配置文件
当修改了 `.service` 文件后，需要重载才能生效
```bash
# 重新加载一个服务的配置文件
systemctl reload test.service

# 重载所有修改过的配置文件
systemctl daemon-reload
```
# `.service` 文件解析

##  [Unit] 区块
启动顺序与依赖关系。unit 本身的说明，以及与其他相依 daemon 的设置，包括在什么服务之后才启动此 unit 之类的设置值

| 设置参数      | 参数意义说明                                                 |
| ------------- | ------------------------------------------------------------ |
| Description   | 服务的简单描述。当我们使用 systemctl list-units 时，会输出给管理员看的简易说明以及使用 `systemctl status` 输出的此服务的说明 |
| Documentation | 这个项目在提供管理员能够进行进一步的文件查询的功能！提供的文件可以是如下的数据：`Documentation=http://www....` `Documentation=man:sshd（8）` `Documentation=file:/etc/ssh/sshd_config` |
| After         | 说明此 unit 是在哪个 daemon 启动之后才启动。基本上仅是说明服务启动的顺序而已，并没有强制要求里头的服务一定要启动后此 unit 才能启动。 以 sshd.service 的内容为例，该文件提到 After 后面有 network.target 以及 sshd-keygen.service，但是若这两个 unit 没有启动而强制启动 sshd.service 的话， 那么 sshd.service 应该还是能够启动的！这与 Requires 的设置是有差异的喔！ |
| Before        | 与 After 的意义相反，在什么服务启动前最好启动这个服务。不过这仅是规范服务启动的顺序，并非强制要求的意思。 |
| Requires      | 明确的定义此 unit 需要在哪个 daemon 启动后才能够启动！设置相依服务，如果在此项设置的前导服务没有启动，此 unit 就不会被启动。 |
| Wants         | 与 Requires 刚好相反，规范的是这个 unit 之后最好还要启动什么服务比较好。不过，并没有明确的规范。 因此，这个 Wants 后面接的服务如果没有启动，其实不会影响到这个 unit 本身！ |
| Conflicts     | 代表冲突的服务。即这个项目后面接的服务如果有启动，那么我们这个 unit 本身就不能启动！我们 unit 有启动，则此项目后的服务就不能启动！ 即冲突性的检查 |

## [Service] 区块
`Service` 区块定义如何启动当前服务。规范服务启动的脚本、环境配置文件文件名、重新启动的方式等等。

| 设置参数        | 参数意义说明                                                 |
| --------------- | ------------------------------------------------------------ |
| Type            | 启动类型                                                     |
| EnvironmentFile | 许多软件都有自己的环境参数文件，该文件可以用`EnvironmentFile`字段读取。可以指定启动脚本的环境配置文件。例如 sshd.service 的配置文件写入到 /etc/sysconfig/sshd 当中。你也可以使用 Environment= 后面接多个不同的 Shell 变量来给予设置。 |
| ExecStart       | 执行此 daemon 的指令或脚本程序。你也可以使用 ExecStartPre （之前） 以及 ExecStartPost （之后） 两个设置项目来在实际启动服务前，进行额外的指令行为。 |
| ExecStop        | 与 systemctl stop 的执行有关，关闭此服务时所进行的指令。     |
| ExecReload      | 与 systemctl reload 有关的指令行为                           |
| Restart         | Systemd 的重启方式                                           |
| RemainAfterExit | 当设置为 RemainAfterExit=1 时，则当这个 daemon 所属的所有程序都终止之后，此服务会再尝试启动。这对于 Type=oneshot 的服务很有帮助 |
| TimeoutSec      | 若这个服务在启动或者是关闭时，因为某些缘故导致无法顺利“正常启动或正常结束”的情况下，则我们要等多久才进入“强制结束”的状态！ |
| KillMode        | Systemd 如何停止服务                                         |
| RestartSec      | 与 Restart 有点相关性，如果这个服务被关闭，然后需要重新启动时，大概要 sleep 多少时间再重新启动的意思。默认是 100ms （毫秒）。 |

具体参数解析：

**Type**：

+ simple：默认值，由 ExecStart 启动的进程为主进程，启动后常驻于内存中。
+ forking：以`fork()`方式启动，此时父进程将会退出，子进程将成为主进程。
+ oneshot：与 simple 类似，但只执行一次，不会常驻在内存中。
+ dbus：与 simple 类似，但会等待 D-Bus 信号后启动。因此设置这个项目时，通常也要设置 BusName= 才行。
+ idle：与 simple 类似，执行这个 daemon 必须要所有的工作都顺利执行完毕后才会执行。这类的 daemon 通常是开机到最后才执行即可的服务

**Restart**：

- no（默认值）：退出后不会重启
- on-success：只有正常退出时（退出状态码为0），才会重启
- on-failure：非正常退出时（退出状态码非0），包括被信号终止和超时，才会重启
- on-abnormal：只有被信号终止和超时，才会重启
- on-abort：只有在收到没有捕捉到的信号终止时，才会重启
- on-watchdog：超时退出，才会重启
- always：不管是什么退出原因，总是重启

**KillMode**:

- control-group（默认值）：当前控制组里面的所有子进程，都会被杀掉
- process：只杀主进程
- mixed：主进程将收到 SIGTERM 信号，子进程收到 SIGKILL 信号
- none：没有进程会被杀掉，只是执行服务的 stop 命令。


## [Install] 区块
定义如何安装这个配置文件，即怎样做到开机启动。

| 设置参数 | 参数意义说明                                                 |
| -------- | ------------------------------------------------------------ |
| WantedBy | 该服务所在的 Target。`Target`的含义是服务组，表示一组服务。`WantedBy=multi-user.target`指的是，sshd 所在的 Target 是`multi-user.target`。默认的启动 Target 是`multi-user.target`。在这个组里的所有服务，都将开机启动。这就是为什么`systemctl enable`命令能设置开机启动的原因。 |
| Also     | 当目前这个 unit 本身被 enable 时，Also 后面接的 unit 也请 enable 的意思。也就是具有相依性的服务可以写在这里。 |
| Alias    | 进行一个链接的别名的意思。当 systemctl enable 相关的服务时，则此服务会进行链接文件的创建！以 multi-user. target 为例，这个家伙是用来作为默认操作环境 default. target 的规划，因此当你设置用成 default. target 时，这个 /etc/systemd/system/default. target 就会链接到 /usr/lib/systemd/system/multi-user. target 啰！ |




> [Systemd 入门教程：实战篇](http://www.ruanyifeng.com/blog/2016/03/systemd-tutorial-part-two.html)
>
> [systemd实现python的守护进程](https://blog.csdn.net/luckytanggu/article/details/53467687)