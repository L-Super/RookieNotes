## 为单个仓库指定 SSH Key

在仓库目录下执行：

```bash
git config core.sshCommand "ssh -i ~/.ssh/id_ed25519_work -o IdentitiesOnly=yes"
```

- `IdentitiesOnly=yes`:  防止 ssh-agent 中其他 key 被尝试，强制只用 ` -i ` 指定的 key。多账号（GitHub 个人 + 工作）场景下必加。

之后该仓库的 `git pull` / `git push` 自动使用此 key，仅此仓库生效。

```bash
# 查看
git config --get core.sshCommand

# 取消
git config --unset core.sshCommand
```

