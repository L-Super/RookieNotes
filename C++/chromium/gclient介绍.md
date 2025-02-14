gclient 是由 Google 用 Python 开发的一套跨平台的**git仓库管理工具**,它的作用类似 git 的 submodule，用来将多个git仓库组成一个solution进行管理，比如chromium项目是由80多个独立的git仓库构成的。这样的好处是，一个git仓库可以被多个solution共用，类似CBB的机制，只不过它是直接引用代码仓库，而CBB是引用编译好的程序。

具体来讲，gclient 的核心功能是**将solution中由DEPS文件定义的所有git仓库拉取到指定的目录**。围绕着这个功能，它添加了一些其它的辅助功能，比如`Hooks`，`include_rules`等功能。

## 关键概念

- `gclient`: 一个跨平台的代码仓库管理工具，用于管理由多个代码仓库组成的项目；
- `repo`: 作用同gclient，是gclient的前辈，gclient是更新的工具；
- `roll_deps`: 一个gclient 的辅助工具，用于更新DEPS文件中某个项目的代码版本；
- `Gerrit/Rietveld`: 一个Code Review系统，可以和git/svn集成；
- `Hooks`: gclient中的术语，当gclient拉完代码之后执行的额外脚本；
- `Solution`: 一个包含DEPS文件的仓库，DEPS文件中记录了该solution所有依赖的项目；
- `.gclient`: 一个由 gclient config 命令创建出来的文件，内部记录了要拉取的solution；
- `DEPS`: 一个文件，gclient 用于管理项目依赖的一个文件，记录了项目的依赖关系；
- `CL`: Change List ，类似 git 的 diff 和 pack；
- `LKGR`: Last Known Good Revision，一个git tag，记录了最新的经过完整测试的版本；
- `managed`: gclient 的一种模式，该模式已过时，不推荐使用；该模式用于帮助那些对git不熟悉的开发者更简单的使用gclient;
- `inlcude_rules`: 指定当前目录下哪些目录/文件可以被其他代码inlcude，那些不可以被inlcude;
- `specific_include_rules`: 作用同include_rules，但是可以使用通配符;

## gclient 命令解释

### `gclient config`

该命令用来初始化要拉取的solution，它会在命令执行目录下生成 .gclient 文件； .gclient 文件中记录了要拉取的solution的仓库地址，以及拉取后要保存的位置，如下：

```
solutions = [
  {
    "url": "https://chromium.googlesource.com/chromium/src.git", # 要拉取的仓库地址，solution 地址
    "managed": False,  # 使用 unmanaged 模式
    "name": "src", # 拉去后存放的目录
    "deps_file": ".DEPS.git", # deps 文件名（如果找不到，会找DEPS?）
    "custom_deps": {}, # 自定义的 project 的仓库地址
  },
]
```

### `gclient sync`

该命令用来同步solution。它有很多的参数可选：

- `--force` 拉取solution中的所有仓库
- `--revision <version>` 将代码切换到 version 版本
- `--no-history` 拉取的时候不拉取git历史记录，可以减少拉取的大小，从而加快拉取的速度
- `--nohooks` 拉取代码后不执行hooks

### 其他

- `gclient runhooks`: 对本地已经修改的文件执行hooks
- `gclient status`: 输出当前solution中所有仓库的状态信息
- `gclient diff`: 显示整个solution中所有项目的diff信息
- `gclient fetch`: 相当于在solution中的每一个仓库都执行了git fetch 操作
- `gclient flatten`: 将当前solution“扁平化”，生成一个 DEPS 文件，使用 –pin-all-deps 参数可以使生成的 DEPS 文件中记录每一个仓库的commit号
- `gclient grep`: 类似 grep,用于在当前 solution 中搜索字符串
- `gclient pack`: 内部对每一个仓库使用 git diff 来生成一个 patch，这个patch 可以用 ’patch –p0 < patchfile‘ 命令来应用到solution中
- `gclient recurse`: 在solution中的每一个仓库中执行一条命令
- `gclient revert`: 在solution中的每一个仓库中执行git reset –hard,将solution回退到一个clean的状态，所有本地的修改都会撤销
- `gclient revinfo`: 显示当前solution中每一个仓库的版本信息
- `gclient root`: 输出当前solution根目录的路径

## 使用 gclient 的开发流程

```shell
# 首次拉代码：
mkdir chromium
cd chromium
gclient config git@gitlab.gz.cvte.cn:CrOS/src.git --unmanaged # <1s
gclinet sync --reversion src@c/master --nohooks # >10m,不需要水管
gclient runhooks # >10m，需要水管
...... # 编译
......  # 修改代码
...... # 使用 git  进行代码提交

# 更新代码：
cd src
git pull # <10s
gclient sync # <30s
```