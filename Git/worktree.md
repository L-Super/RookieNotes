Git Worktree（工作树）允许你从**同一个仓库**同时检出**多个分支**到不同的目录，共享同一个 `.git` 数据库，无需克隆多份仓库。

```
.git/  ←──────────────────────────── 共享同一个仓库
  │
  ├── 主工作树：  /project          (main 分支)
  ├── 附加工作树：/project-feature  (feature/login 分支)
  └── 附加工作树：/project-hotfix   (hotfix/bug123 分支)
```

## 对比克隆多个仓库

| 对比项   | `git clone` 多份      | `git worktree`     |
| -------- | --------------------- | ------------------ |
| 磁盘占用 | ❌ 每份都有完整 `.git` | ✅ 共享一个 `.git`  |
| 分支同步 | ❌ 需手动同步          | ✅ 自动共享所有引用 |
| 配置共享 | ❌ 各自独立            | ✅ 完全共享         |
| 适用场景 | 完全隔离的副本        | 同仓库多任务并行   |

## 基本命令

### 添加工作树

```bash
# 基本用法：检出已有分支
git worktree add ../project-hotfix hotfix/bug123

# 创建新分支并检出
git worktree add -b feature/login ../project-feature

# 检出到当前目录下的子目录
git worktree add ./worktrees/hotfix hotfix/bug123

# 仅创建目录，不检出文件（裸模式，常用于 bare 仓库）
git worktree add --detach ../project-detach HEAD
```

### 查看所有工作树

```bash
git worktree list

# 输出示例：
# /home/user/project          a1b2c3d [main]
# /home/user/project-feature  e4f5g6h [feature/login]
# /home/user/project-hotfix   i7j8k9l [hotfix/bug123]
```

### 删除工作树

```bash
git worktree remove ../project-hotfix

# 强制删除（有未提交修改时）
git worktree remove --force ../project-hotfix
```

### 修复损坏的工作树

```bash
# 清理无效的工作树引用
git worktree prune

# 移动工作树到新路径
git worktree move ../project-feature ../new-path/project-feature

# 锁定工作树（防止被 prune 清理，适合挂载到临时目录）
git worktree lock ../project-hotfix --reason "正在使用"
git worktree unlock ../project-hotfix
```

## 典型使用场景

### 场景一：紧急修复 Bug，不想打断当前工作

```bash
# 当前正在 feature 分支开发，不想 stash
# 直接新开一个工作树处理 hotfix
git worktree add ../hotfix-temp hotfix/bug123

cd ../hotfix-temp
# 修复 bug，提交
git commit -am "fix: 修复登录崩溃问题"
git push

# 回到原来的工作
cd ../project
# feature 分支的工作完好无损 ✅

# 清理临时工作树
git worktree remove ../hotfix-temp
```

### 场景二：同时运行多个分支的服务

```bash
# 主分支跑生产服务（端口 3000）
# 同时在 feature 分支开发新功能（端口 3001）

git worktree add ../project-dev feature/new-ui

# 终端1：主分支
cd ../project && npm run dev -- --port 3000

# 终端2：feature 分支
cd ../project-dev && npm run dev -- --port 3001

# 两个服务同时运行，互不干扰 ✅
```

### 场景三：Code Review 时本地跑起来对比

```bash
# 不切换自己的分支，单独检出 PR 分支来审查
git fetch origin pull/42/head:pr-42
git worktree add ../review-pr42 pr-42

cd ../review-pr42
npm install && npm test
# 审查完直接删掉
git worktree remove ../review-pr42
```
### 场景四：Bare 仓库 + Worktree（进阶工作流）

这是一种流行的"无主工作树"模式，特别适合长期多分支并行开发：

```bash
# 克隆为裸仓库
git clone --bare https://github.com/user/repo.git repo.git

repo.git/
  ├── HEAD
  ├── config
  └── ...  # 没有工作目录文件，只有 git 数据

# 按需添加工作树
cd repo.git
git worktree add ../repo-main main
git worktree add ../repo-feature feature/login
git worktree add ../repo-hotfix hotfix/bug123

# 目录结构
repo.git/          ← 裸仓库（.git 数据）
repo-main/         ← main 分支工作目录
repo-feature/      ← feature 分支工作目录
repo-hotfix/       ← hotfix 分支工作目录
```

## 注意事项

### 同一分支不能被两个工作树同时检出

```bash
git worktree add ../project-copy main
# error: fatal: 'main' is already checked out at '/home/user/project'
# ❌ 同一分支同一时间只能在一个工作树中
```

解决办法：基于该分支创建新分支再检出：
```bash
git worktree add -b main-copy ../project-copy main
```

## 命令速查表

```bash
# 增
git worktree add <路径> <分支>          # 检出已有分支
git worktree add -b <新分支> <路径>     # 创建新分支并检出

# 查
git worktree list                       # 列出所有工作树
git worktree list --porcelain           # 机器可读格式

# 改
git worktree move <旧路径> <新路径>     # 移动工作树
git worktree lock <路径>               # 锁定
git worktree unlock <路径>             # 解锁

# 删
git worktree remove <路径>             # 删除工作树
git worktree remove --force <路径>     # 强制删除
git worktree prune                     # 清理失效引用
```

> https://git-scm.cn/docs/git-worktree