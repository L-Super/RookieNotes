Compose 是用于定义和运行多容器 Docker 应用程序的工具。通过 Compose，您可以使用 YML 文件来配置应用程序需要的所有服务。然后，使用一个命令，就可以从 YML 文件配置中创建并启动所有服务。

Compose 使用步骤：
- 使用 docker-compose.yml 定义构成应用程序的服务，这样它们可以在隔离环境中一起运行。
- 执行 `docker compose up` 命令来启动并运行整个应用程序。

想在后台执行该服务可以加上 -d 参数：

```
docker compose up -d
```

比如 nginx 的 docker-compse.yml 文件：
```yaml
version: '3'
services:
  nginx:
    container_name: nginx
    image: nginx
    restart: always
    ports:
      - '80:80'
      - '443:443'
    volumes:
      - './nginx.conf:/etc/nginx/nginx.conf'
      - './logs:/var/log/nginx'
      - './html/public:/usr/share/nginx/html'
      - './certs:/etc/nginx/certs'
```


### 修改 yml 文件后，如何重启容器
在有的场景下，需要对容器进行一些修改，比如对 nginx 加 443 端口，可以先修改 `docker-compose.yml` 文件。
然后停止并移除容器、网络、镜像和数据卷：
```bash
docker compose down
```
记得在 `docker-compose.yml` 同路径下执行。
最后再重新运行容器：
```bash
docker compose up -d
```
这样就完成了修改。

## 升级容器

```bash
docker compose pull
docker compose up -d
docker image prune  # prune 命令用来删除不再使用的 docker 对象。删除所有未被 tag 标记和未被容器使用的镜像
```