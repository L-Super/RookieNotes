`docker compose up`
创建并启动容器

|选项|默认值|描述|
|---|---|---|
|`--abort-on-container-exit`||如果任何容器停止，则停止所有容器。与 -d 不兼容|
|`--abort-on-container-failure`||如果任何容器退出并出现故障，则停止所有容器。与 -d 不兼容|
|`--always-recreate-deps`||重新创建依赖容器。与 --no-recreate 不兼容。|
|`--attach`||限制连接到指定的服务。与 --attach-dependencies 不兼容。|
|`--attach-dependencies`||自动连接到依赖服务的日志输出|
|`--build`||在启动容器之前构建镜像|
|`-d, --detach`||分离模式：在后台运行容器|
|`--exit-code-from`||返回所选服务容器的退出代码。隐含 --abort-on-container-exit|
|`--force-recreate`||即使容器的配置和镜像没有更改，也强制重新创建容器|
|`--menu`||在连接运行时启用交互式快捷键。与 --detach 不兼容。也可以通过设置 COMPOSE_MENU 环境变量来启用/禁用。|
|`--no-attach`||不连接（流式传输日志）到指定的服务|
|`--no-build`||不构建镜像，即使是策略要求|
|`--no-color`||生成单色输出|
|`--no-deps`||不启动关联的服务|
|`--no-log-prefix`||在日志中不打印前缀|
|`--no-recreate`||如果容器已经存在，则不重新创建。与 --force-recreate 不兼容。|
|`--no-start`||创建服务后不启动它们|
|`--pull`|`策略`|运行前拉取镜像（"always"\|"missing"\|"never"）|
|`--quiet-pull`||拉取时不安静输出进度信息|
|`--remove-orphans`||移除 Compose 文件中未定义的服务的容器|
|`-V, --renew-anon-volumes`||重新创建匿名卷，而不是从先前的容器中检索数据|
|`--scale`||将 SERVICE 缩放到 NUM 个实例。如果 Compose 文件中存在 `scale` 设置，则此选项会覆盖它。|
|`-t, --timeout`||连接或容器已在运行时，使用此超时值（秒）关闭容器|
|`--timestamps`||显示时间戳|
|`--wait`||等待服务处于 running\|healthy 状态。隐含分离模式。|
|`--wait-timeout`||等待项目处于 running\|healthy 状态的最大持续时间（秒）|
|`-w, --watch`||监视源代码，并在文件更新时重建/刷新容器。|
|`-y, --yes`||假设对所有提示都回答“yes”，并以非交互模式运行|