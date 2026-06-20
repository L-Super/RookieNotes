`logrotate` 是 Linux/Unix 系统中**用于日志轮转（log rotation）的标准工具**，几乎所有发行版都会默认安装。它的主要作用是**自动管理不断增长的日志文件**，防止日志占满磁盘空间。

随着服务运行，日志文件会不断变大，例如：

- `/var/log/nginx/access.log`
- `/var/log/mysql/mysql.log`

`logrotate` 可以自动完成以下工作：

✅ 按时间或大小切割日志
✅ 旧日志重命名、压缩（gzip）
✅ 保留指定数量的历史日志
✅ 删除过期日志
✅ 轮转后自动重启/重载服务
✅ 支持按服务单独配置

## logrotate 的工作方式

### 触发方式

- **通常由 cron 定时执行**
- 常见路径：
  ```bash
  /etc/cron.daily/logrotate
  ```

系统每天执行一次 `logrotate`，是否真正轮转取决于配置条件。

### 核心配置文件

#### 主配置文件
```bash
/etc/logrotate.conf
```

#### 服务单独配置目录
```bash
/etc/logrotate.d/
```

例如：
```bash
/etc/logrotate.d/nginx
/etc/logrotate.d/mysql
```

## 基本配置示例

### 示例 1：轮转 nginx 日志

 `/etc/logrotate.d/nginx` 文件：

```conf
/var/log/nginx/*.log {
    daily                # 每天轮转
    rotate 7              # 保留 7 个旧日志
    compress              # 压缩旧日志（.gz）
    delaycompress         # 延迟一次压缩
    missingok             # 日志不存在不报错
    notifempty            # 空文件不轮转
    create 0640 nginx adm # 创建新日志文件
    sharedscripts
    postrotate
        [ -s /run/nginx.pid ] && kill -USR1 $(cat /run/nginx.pid)
    endscript
}
```

轮转结果示例：
```text
access.log
access.log.1
access.log.2.gz
access.log.3.gz
...
```
注意：写入 `/etc/logrotate.d/nginx` 文件内容时，不能带注释，否则无法执行。
## 常用配置参数说明

| 参数 | 作用 |
|----|----|
| `daily` / `weekly` / `monthly` | 按时间轮转 |
| `size 100M` | 日志大于 100MB 时轮转 |
| `rotate N` | 保留 N 个旧日志 |
| `compress` | 使用 gzip 压缩 |
| `delaycompress` | 延迟压缩最近一次旧日志 |
| `missingok` | 日志不存在时不报错 |
| `notifempty` | 空日志不轮转 |
| `copytruncate` | 复制后清空（适合不能重启的进程） |
| `create` | 轮转后创建新日志文件 |
| `postrotate/endscript` | 轮转后执行命令 |

## copytruncate vs 重启服务

### `copytruncate`
- ✅ 不需要重启服务
- ❌ 轮转瞬间可能丢日志

```conf
copytruncate
```

### `postrotate + reload`
- ✅ 更安全
- ❌ 需要服务支持 reload

```conf
postrotate
    systemctl reload nginx
endscript
```

## 手动测试 logrotate

### 测试配置（不真正执行）
```bash
logrotate -d /etc/logrotate.d/nginx
```

### 强制执行
```bash
logrotate -f /etc/logrotate.d/nginx
```

### 查看轮转状态
```bash
cat /var/lib/logrotate/status
```