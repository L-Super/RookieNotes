
## 创建 submodule
使用 `git submodule add <submodule_url>` 命令可以在项目中创建一个子模块。此时项目仓库中会多出一个 `.gitmodules` 文件

`gitmodules` 保存的是子模块当前版本的版本号信息：
```
[submodule "SingleApplication"]
	path = SingleApplication
	url = https://github.com/itay-grudev/SingleApplication
```
**指定 submodule 的位置**
比如，想要将第三方库放在 `src/third_party` 路径下：
```
git submodule add https://github.com/itay-grudev/SingleApplication src/third_party/SingleApplication
```
`gitmodules` 信息就会变成：
```
[submodule "src/third_party/SingleApplication"]
	path = src/third_party/SingleApplication
	url = https://github.com/itay-grudev/SingleApplication
```

## 获取 submodule
### 方式一
在克隆主项目的时候带上参数 `--recurse-submodules`，这样会递归地将项目中所有子模块的代码拉取。
```bash
git clone https://github.com/L-Super/QClipboard --recurse-submodules
```

### 方式二
在当前主项目中执行：

```bash
git submodule init  
git submodule update
```

## 子模块内容的更新

对于主项目而言，子模块的内容发生变动时，通常有三种情况：

1. 当前项目下子模块文件夹内的内容发生了未跟踪的内容变动
2. 当前项目下子模块文件夹内的内容发生了版本变化
3. 当前项目下子模块文件夹内的内容没变，远程有更新

### 子模块有未跟踪的内容变动

通常是在开发环境中，直接修改子模块文件夹中的代码导致的。

此时在主项目中使用 `git status` 能够看到关于子模块尚未暂存以备提交的变更

```bash
$ git status
On branch main
Your branch is up to date with 'origin/main'.

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
  (commit or discard the untracked or modified content in submodules)
        modified:   src/third_party/SingleApplication (modified content)

no changes added to commit (use "git add" and/or "git commit -a")

```

在此情景下，通常需要进入子模块文件夹，按照子模块内部的版本控制体系提交代码。

### 子模块有版本变化

当子模块版本变化时，在主项目中使用 `git status` 查看仓库状态时，会显示子模块有新的提交：

```bash
$ git status
位于分支 master
您的分支与上游分支 'origin/master' 一致。
尚未暂存以备提交的变更：
（使用 "git add <文件>..." 更新要提交的内容）
（使用 "git checkout -- <文件>..." 丢弃工作区的改动）
修改： project-sub-1 (新提交)
修改尚未加入提交（使用 "git add" 和/或 "git commit -a"）
```

在这种情况下，可以使用 `git add/commit` 将其添加到主项目的代码提交中，实际的改动就是那个子模块文件所表示的版本信息：

```bash
$ git diff HEAD HEAD^
diff --git a/project-sub-1 b/project-sub-1
index ace9770..7097c48 160000
--- a/project-sub-1
+++ b/project-sub-1
@@ -1 +1 @@
-Subproject commit ace977071f94f4f88935f9bb9a33ac0f8b4ba935
+Subproject commit 7097c4887798b71cee360e99815f7dbd1aa17eb4
```

通常当子项目更新后，主项目修改其所依赖的版本时，会产生类似这种情景的 commit 提交信息。

### 子模块远程有更新
主项目可以使用 `git submodule update` 更新子模块的代码，但那是指 **当前主项目文件夹下的子模块目录内容**与 **当前主项目记录的子模块版本**不一致时，会参考后者进行更新。

但现在 **当前主项目记录的子模块版本** 还没有变化，在主项目看来当前情况一切正常。此时，需要让主项目主动进入子模块拉取新版代码，进行更新操作。
通常流程进入子模块，然后 `git pull` ：

```bash
cd submodule_dir
git pull origin master
```

子模块目录下的代码版本会发生变化，转到情况 2 的流程进行主项目的提交。

当主项目的子项目特别多时，可以使用 `git submodule` 的一个命令 `foreach` 执行：

```bash
git submodule foreach 'git pull origin master'
```

