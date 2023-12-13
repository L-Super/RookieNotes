## Ubuntu
1. 设置 Docker `apt` 仓库

```bash
# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl gnupg
sudo install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
sudo chmod a+r /etc/apt/keyrings/docker.gpg

# Add the repository to Apt sources:
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

sudo apt-get update
```
2. 安装 docker
```bash
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

安装指定版本：

1. 首先在存储库中列出可用版本：

   ```bash
   # List the available versions:
   $ apt-cache madison docker-ce | awk '{ print $3 }'
   
   5:24.0.0-1~ubuntu.22.04~jammy
   5:23.0.6-1~ubuntu.22.04~jammy
   ...
   ```

2. 选择所需的版本并安装：

   ```bash
   $ VERSION_STRING=5:24.0.0-1~ubuntu.22.04~jammy
   $ sudo apt-get install docker-ce=$VERSION_STRING docker-ce-cli=$VERSION_STRING containerd.io docker-buildx-plugin docker-compose-plugin
   ```


## UOS
适用于 UOS 桌面家庭版/桌面专业版/桌面教育版和 deepin 20. x 版

因为 UOS 桌面家庭版/桌面专业版/桌面教育版和 deepin 20. x 版是基于 debian 10开发的，所以我们可以按照官方文档 https://docs.docker.com/engine/install/debian/ 使用 debian 10的方式安装

主要区别就是修改这行：

```bash
echo "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian  buster stable" |  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```


[Install Docker Engine on Ubuntu | Docker Docs](https://docs.docker.com/engine/install/ubuntu/#install-using-the-repository)