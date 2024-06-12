### 轻量标签

轻量标签（lightweight tag）仅仅是一个指向特定提交的引用，它不会存储任何额外的信息。创建轻量标签的命令如下：

```sh
git tag {标签名} {提交ID}
```

例如，创建一个指向最新提交的轻量标签：

```sh
git tag v1.0.0
```
### 附注标签

附注标签（annotated tag）是存储在Git数据库中的一个完整对象，它有一个标签名，标签信息，标签签名等信息。创建附注标签的命令如下：

```sh
git tag -a {标签名} -m "{标签信息}" {提交ID}
```

例如，创建一个指向最新提交的附注标签：

```sh
git tag -a v1.0.0 -m "Release version 1.0.0" HEAD
```
### 查看标签

查看当前项目中的所有标签：

```sh
git tag
```

如果想查看某个具体标签的信息，可以使用以下命令：

```sh
git show {标签名}
```
### 推送标签

默认情况下，`git push`命令不会将标签推送到远程服务器，需要使用以下命令将标签推送到远程服务器：

```sh
git push origin {标签名}
```

如果要一次性推送所有本地标签，可以使用以下命令：

```sh
git push origin --tags
```

### 删除标签

删除本地标签：

```sh
git tag -d {标签名}
```

删除远程标签：

```sh
git push origin :refs/tags/{标签名}
git push origin --delete <tagname>
```
