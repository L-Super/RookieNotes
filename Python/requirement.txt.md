requirements.txt，是用于记录所有依赖包及其精确的版本号的文本文件。

1. 自动生成requirement.txt

```shell
pip freeze > requirements.txt
```

执行成功后，会自动生成requirement.txt文件

更换环境，分享项目的同时，带上requirement.txt文件，方便其他人配置。

2. 安装配置requirement.txt，执行命令即可一键安装完所需要的第三方库。

```shell
pip install -r requirements.txt
```

   
