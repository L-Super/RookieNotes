git restore 是 Git 2.23 引入的命令，用来更清晰地“还原文件内容”，把工作区或暂存区恢复到指定版本（通常是暂存区或某个提交）。它把原来由 git checkout 和 git reset 混合承担的“还原文件”功能分离出来，搭配 git switch（切换分支）更易理解。

主要用来 **还原工作区的文件内容**，可分为两种常见场景：

1. **丢弃当前工作区的修改**（从暂存区或历史提交中恢复文件）。
2. **撤销暂存区的改动**（从暂存区撤回到工作区）。

git restore 可以有两种作用：
- 不带 `--staged`：把工作区文件还原（默认从“暂存区”取内容）。
- 带 `--staged`：把暂存区文件还原（默认从当前提交 HEAD 取内容）。

- 也可以用 `--source=<commit>` 指定从某个提交或分支恢复，而不是默认来源。

## 常用场景

### 撤销工作区的修改（未 add 到暂存区）

**场景**：修改了 `file1.txt`，不想要这部分修改，想恢复到上一次提交的状态。

```bash
git restore file1.txt
```

**结果**：`file1.txt` 会被还原回最后一次提交时的样子，你的修改会丢失。

还原所有文件：

```bash
git restore .
```

相当于旧用法的：`git checkout -- 路径/文件`

### 撤销已 add 但未 commit 的文件（从暂存区恢复到工作区）

**场景**：用 `git add file2.txt` 暂存了文件，但又想撤销这次暂存，回到修改工作区的状态。

```bash
git restore --staged file2.txt
```

**结果**：`file2.txt` 会从暂存区移除，但保留你对工作区的修改。

取消所有暂存：

```bash
git restore --staged .
```

等价于：`git reset HEAD 路径/文件` 或 `git reset HEAD`

### 同时丢弃已暂存和未暂存的改动（让一切回到 HEAD）

回到最后一次 commit 时的状态

1. 先让暂存区回到 HEAD：

```bash
git restore --staged .
```

2. 再让工作区还原：

```bash
git restore .
```

### 工作区直接回到某个提交（绕过暂存区当前内容）

```bash
git restore --source=HEAD --worktree .
```

指定具体文件：

```bash
git restore --source=<commit> --worktree 路径/文件
```

与其他命令的对比
- git reset：主要是移动 HEAD 和重置暂存区；也能影响工作区（--hard）。常用于“回退提交”或“取消暂存”，但容易一把把 HEAD、暂存区、工作区都改掉。
- git checkout（旧用法）：既能切分支又能还原文件，语义混杂。Git 2.23 之后推荐用 git switch 切分支、git restore 还原文件。
- 对应关系：
  - 取消暂存：git restore --staged ≈ git reset HEAD 路径
  - 还原工作区文件：git restore 路径 ≈ git checkout -- 路径