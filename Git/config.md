查看所有的配置以及它们所在的文件： 
```sh
git config --list --show-origin
```

列出所有 Git 当时能找到的配置：
```sh
git config --list # all
git config --global --list # global
git config --local --list # local
```
获取对应 key 配置：
```sh
git config user.name # 先从本地仓库开始获取
git config --global user.name # global config
```