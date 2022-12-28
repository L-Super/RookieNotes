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
Dec 28 11:07:20 bogon monitor[284962]: [2022-12-28 11:07:20,346] [main] [INFO] [main.p...ew'
Dec 28 11:07:20 bogon monitor[284962]: [2022-12-28 11:07:20,348] [main] [INFO] [main.p...1}}
Dec 28 11:07:21 bogon monitor[284962]: [2022-12-28 11:07:21,351] [main] [INFO] [main.p...ew'
Dec 28 11:07:21 bogon monitor[284962]: [2022-12-28 11:07:21,353] [main] [INFO] [main.p...1}}
Dec 28 11:07:22 bogon monitor[284962]: [2022-12-28 11:07:22,356] [main] [INFO] [main.p...ew'
Dec 28 11:07:22 bogon monitor[284962]: [2022-12-28 11:07:22,358] [main] [INFO] [main.p...1}}
Dec 28 11:07:23 bogon monitor[284962]: [2022-12-28 11:07:23,361] [main] [INFO] [main.p...ew'
Dec 28 11:07:23 bogon monitor[284962]: [2022-12-28 11:07:23,363] [main] [INFO] [main.p...1}}
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
| Description   | 当我们使用 systemctl list-units 时，会输出给管理员看的简易说明以及使用 `systemctl status` 输出的此服务的说明 |
| Documentation | 这个项目在提供管理员能够进行进一步的文件查询的功能！提供的文件可以是如下的数据：`Documentation=http://www....` `Documentation=man:sshd（8）` `Documentation=file:/etc/ssh/sshd_config` |
| After         | 说明此 unit 是在哪个 daemon 启动之后才启动。基本上仅是说明服务启动的顺序而已，并没有强制要求里头的服务一定要启动后此 unit 才能启动。 以 sshd.service 的内容为例，该文件提到 After 后面有 network.target 以及 sshd-keygen.service，但是若这两个 unit 没有启动而强制启动 sshd.service 的话， 那么 sshd.service 应该还是能够启动的！这与 Requires 的设置是有差异的喔！ |
| Before        | 与 After 的意义相反，是在什么服务启动前最好启动这个服务的意思。不过这仅是规范服务启动的顺序，并非强制要求的意思。 |
| Requires      | 明确的定义此 unit 需要在哪个 daemon 启动后才能够启动！就是设置相依服务啦！如果在此项设置的前导服务没有启动，那么此 unit 就不会被启动！ |
| Wants         | 与 Requires 刚好相反，规范的是这个 unit 之后最好还要启动什么服务比较好的意思！不过，并没有明确的规范就是了！主要的目的是希望创建让使用者比较好操作的环境。 因此，这个 Wants 后面接的服务如果没有启动，其实不会影响到这个 unit 本身！ |
| Conflicts     | 代表冲突的服务！亦即这个项目后面接的服务如果有启动，那么我们这个 unit 本身就不能启动！我们 unit 有启动，则此项目后的服务就不能启动！ 反正就是冲突性的检查啦！ |

## [Service] 区块
`Service` 区块定义如何启动当前服务。规范服务启动的脚本、环境配置文件文件名、重新启动的方式等等。

| 设置参数        | 参数意义说明                                                 |
| --------------- | -------------------------------------------------------- |
| Type            | 说明这个 daemon 启动的方式，会影响到 ExecStart 喔！一般来说，有下面几种类型 simple：默认值，这个 daemon 主要由 ExecStart 接的指令串来启动，启动后常驻于内存中。forking：由 ExecStart 启动的程序通过 spawns 延伸出其他子程序来作为此 daemon 的主要服务。原生的父程序在启动结束后就会终止运行。 传统的 unit 服务大多属于这种项目，例如 httpd 这个 WWW 服务，当 httpd 的程序因为运行过久因此即将终结了，则 systemd 会再重新生出另一个子程序持续运行后， 再将父程序删除。据说这样的性能比较好！！oneshot：与 simple 类似，不过这个程序在工作完毕后就结束了，不会常驻在内存中。dbus：与 simple 类似，但这个 daemon 必须要在取得一个 D-Bus 的名称后，才会继续运行！因此设置这个项目时，通常也要设置 BusName= 才行！idle：与 simple 类似，意思是，要执行这个 daemon 必须要所有的工作都顺利执行完毕后才会执行。这类的 daemon 通常是开机到最后才执行即可的服务！比较重要的项目大概是 simple, forking 与 oneshot 了！毕竟很多服务需要子程序 （forking），而有更多的动作只需要在开机的时候执行一次（oneshot），例如文件系统的检查与挂载啊等等的。 |
| EnvironmentFile | 可以指定启动脚本的环境配置文件！例如 sshd.service 的配置文件写入到 /etc/sysconfig/sshd 当中！你也可以使用 Environment= 后面接多个不同的 Shell 变量来给予设置！ |
| ExecStart       | 就是实际执行此 daemon 的指令或脚本程序。你也可以使用 ExecStartPre （之前） 以及 ExecStartPost （之后） 两个设置项目来在实际启动服务前，进行额外的指令行为。 但是你得要特别注意的是，指令串仅接受“指令 参数 参数...”的格式，不能接受 <, >, >>, \|, & 等特殊字符，很多的 bash 语法也不支持喔！ 所以，要使用这些特殊的字符时，最好直接写入到指令脚本里面去！不过，上述的语法也不是完全不能用，亦即，若要支持比较完整的 bash 语法，那你得要使用 Type=oneshot 才行喔！ 其他的 Type 才不能支持这些字符。 |
| ExecStop        | 与 systemctl stop 的执行有关，关闭此服务时所进行的指令。     |
| ExecReload      | 与 systemctl reload 有关的指令行为                           |
| Restart         | 当设置 Restart=1 时，则当此 daemon 服务终止后，会再次的启动此服务。举例来说，如果你在 tty2 使用文字界面登陆，操作完毕后登出，基本上，这个时候 tty2 就已经结束服务了。 但是你会看到屏幕又立刻产生一个新的 tty2 的登陆画面等待你的登陆！那就是 Restart 的功能！除非使用 systemctl 强制将此服务关闭，否则这个服务会源源不绝的一直重复产生！ |
| RemainAfterExit | 当设置为 RemainAfterExit=1 时，则当这个 daemon 所属的所有程序都终止之后，此服务会再尝试启动。这对于 Type=oneshot 的服务很有帮助！ |
| TimeoutSec      | 若这个服务在启动或者是关闭时，因为某些缘故导致无法顺利“正常启动或正常结束”的情况下，则我们要等多久才进入“强制结束”的状态！ |
| KillMode        | 可以是 process, control-group, none 的其中一种，如果是 process 则 daemon 终止时，只会终止主要的程序 （ExecStart 接的后面那串指令），如果是 control-group 时， 则由此 daemon 所产生的其他 control-group 的程序，也都会被关闭。如果是 none 的话，则没有程序会被关闭喔！ |
| RestartSec      | 与 Restart 有点相关性，如果这个服务被关闭，然后需要重新启动时，大概要 sleep 多少时间再重新启动的意思。默认是 100ms （毫秒）。 |

## [Install] 区块
定义如何安装这个配置文件，即怎样做到开机启动。

| 设置参数 | 参数意义说明                                                 |
| -------- | ------------------------------------------------------------ |
| WantedBy | 这个设置后面接的大部分是 *.target unit ！意思是，这个 unit 本身是附挂在哪一个 target unit 下面的！一般来说，大多的服务性质的 unit 都是附挂在 multi-user.target 下面！ |
| Also     | 当目前这个 unit 本身被 enable 时，Also 后面接的 unit 也请 enable 的意思！也就是具有相依性的服务可以写在这里呢！ |
| Alias    | 进行一个链接的别名的意思！当 systemctl enable 相关的服务时，则此服务会进行链接文件的创建！以 multi-user.target 为例，这个家伙是用来作为默认操作环境 default.target 的规划， 因此当你设置用成 default.target 时，这个 /etc/systemd/system/default.target 就会链接到 /usr/lib/systemd/system/multi-user.target 啰！ |