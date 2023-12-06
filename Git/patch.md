
Git 提供了两种补丁方案，一是用 git diff 生成的 UNIX 标准补丁. diff 文件，二是 git format-patch 生成的 Git 专用. patch 文件

## 生成 patch

方法一：

```bash
git diff > xxx.patch
```

把所有修改的文件都达成 patch

方法二：

```bash
git format-patch HEAD^       #生成最近的1次commit的patch
git format-patch HEAD^^      #生成最近的2次commit的patch
git format-patch <r1>..<r2>  #生成两个commit间的修改的patch（生成的patch不包含r1. <r1>和<r2>都是具体的commit号)
git format-patch -1 <r1>     #生成单个commit的patch
git format-patch <r1>        #生成某commit以来的修改patch（不包含该commit）
git format-patch --root <r1> #生成从根到r1提交的所有patch
```

## 打 patch

有两种方式：

+ `git am` 会直接将 patch 的所有信息打上去，而且不用重新 git add 和 git commit，author 也是 patch 的 author 而不是打 patch 的人。
+ `git apply` 并不会将 commit message 等打上去，打完 patch 后需要重新 git add 和 git commit。


查看 patch 的情况

```bash
$ git apply --stat xxx.patch 
```

比如：

```bash
$ git apply --stat .\qcefview-src-11a7e7f-2b91e84.patch
 src/details/QCefWindow.cpp       |    2 ++
 include/QCefSetting.h            |   15 ++++++++++++++-
 src/QCefSetting.cpp              |   16 +++++++++++++++-
 src/details/QCefSettingPrivate.h |    2 ++
 src/details/QCefViewPrivate.cpp  |   20 +++++++++++++++++---
 src/details/QCefWindow.cpp       |    1 -
 6 files changed, 50 insertions(+), 6 deletions(-)
```

检查 patch 是否能够打上，如果没有任何输出，则说明无冲突，可以打上

```bash
git apply --check xxx.patch
```

打 patch

```bash
git apply xxx.patch
或者
git am xxx.patch
```

