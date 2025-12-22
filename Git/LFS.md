Git 大文件存储 (LFS) 用 Git 内部的文本指针替换大文件，比如音频样本、视频、数据集和图形，同时将文件内容存储在 GitHub.com 这样的远程服务器上。
## 安装 Git LFS

- macOS：
```bash
brew install git-lfs
```

- Windows：
前往 [git-lfs.com](https://git-lfs.com/) 下载
- Linux：
前往 [git-lfs.com](https://git-lfs.com/) 下载

然后 git 安装 lfs 扩展
```bash
$ git lfs install
> Git LFS initialized.
```

## 配置
将存储库中的文件类型与 Git LFS 相关联
 ```bash
 git lfs track "*.jpg"
 ```
 此命令将修改存储库的 .gitattributes 文件，并将大文件与 Git LFS 相关联。
别忘了追踪 `.gitattributes` 文件：
```bash
git add .gitattributes
```

## 常用命令
```bash
# 查看当前使用 Git LFS 管理的匹配列表
git lfs track

# 使用 Git LFS 管理指定的文件
git lfs track "*.psd"

# 不再使用 Git LFS 管理指定的文件
git lfs untrack "*.psd"

# 类似 `git status`，查看当前 Git LFS 对象的状态
git lfs status

# 枚举目前所有被 Git LFS 管理的具体文件
git lfs ls-files

# 检查当前所用 Git LFS 的版本
git lfs version
```

将之前的提交历史记录中的文件加入 LFS 可以使用 `git lfs migrate`命令

>  https://git-lfs.com/
>  https://docs.github.com/zh/repositories/working-with-files/managing-large-files/about-git-large-file-storage