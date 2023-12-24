`docker network rm`命令用于删除一个或多个网络。按名称或标识符删除一个或多个网络。 要删除网络，必须要先断开连接到它的任何容器的网络。

**用法**

```shell
docker network rm NETWORK [NETWORK...]
```

**选项**

| 名称，简写    | 默认  | 说明         |
| ------------- | ----- | ------------ |
| `--force, -f` | false | 不要提示确认 |

## 相关命令

| 命令名称                  | 说明                         |
| ------------------------- | ---------------------------- |
| docker network connect    | 将容器连接到网络             |
| docker network create     | 创建一个网络                 |
| docker network disconnect | 断开容器的网络               |
| docker network inspect    | 显示一个或多个网络的详细信息 |
| docker network ls         | 列出网络                     |
| docker network prune      | 删除所有未使用的网络         |
| docker network rm         | 删除一个或多个网络           |

## 示例

删除名为 `my-network` 的网络：

```shell
$ docker network rm my-network
```

删除多个网络：

要在单个 `docker network rm` 命令中删除多个网络，请提供多个网络名称或 ID 列表。以下示例删除标识为 `3695c422697f` 的网络和名为 `my-network` 的网络：

```shell
$ docker network rm 3695c422697f my-network
```

指定多个网络时，该命令会依次删除每个网络。如果删除一个网络失败，则该命令继续到列表中的下一个，并尝试删除。该命令报告每个删除的成功或失败。
