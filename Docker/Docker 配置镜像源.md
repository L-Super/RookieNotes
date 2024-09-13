在 `/etc/docker` 路径下，创建 `daemon.json` 文件

```bash
vim /etc/docker/daemon.json
```
填入对应镜像源，如：
```json
{
  "registry-mirrors": ["https://jupixquq.mirror.aliyuncs.com"]
}
```
重启 Docker
```bash
systemctl restart docker
```

查看是否修改成功
```bash
docker info
```
有 `registry-mirrors` 字段就说明设置成功。

移除镜像源，删除 `daemon.json` 文件，重启 Docker 即可。