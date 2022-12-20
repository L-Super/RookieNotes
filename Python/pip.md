pip 是 Python 的包管理工具

## 安装第三方库
```python
pip install requests

pip install psutil==5.9.4 #安装指定版本

pip install psutil>=1.0.4 #安装最小版本

pip install -r requirements.txt #安装指定文件中的包
```

## 卸载第三方库

```
pip uninstall xlrd
```

## 列出安装的库

```
pip list
pip list -o #查看可升级的包
```

## 以一定格式安装的输出包信息

`pip freeze` 输出格式可以提供给其他虚拟环境使用，`pip install` 则可以根据 [requirement.txt](requirement.txt.md)文件安装指定版本的包。

```
pip freeze #查看已经安装的包以及版本信息

pip freeze > requirements.txt
```


## 使用镜像源

常用的下载源

+ 清华：https://pypi.tuna.tsinghua.edu.cn/simple
+ 阿里云：http://mirrors.aliyun.com/pypi/simple/
+ 中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/
+ 豆瓣：http://pypi.douban.com/simple/


### 临时更换下载源
下载 requests 库时临时使用清华源进行下载

```
pip install -i https://pypi.tuna.tsinghua.edu.cn/simple requests
```


### 永久更换下载源
#### Linux
修改 ~/.pip/pip.conf (没有就创建一个)， 内容如下：

```
[global]

index-url = https://pypi.tuna.tsinghua.edu.cn/simple

```

#### Windows用户

直接在user目录中创建一个pip目录，如：C:\Users\xx\pip，在pip 目录下新建文件pip.ini，添加下面的代码或者按快捷键 win+R 打开用户目录%HOMEPATH%，在此目录下创建 pip 文件夹，在 pip 目录下创建 pip.ini 文件, 内容如下

```
[global] 
timeout = 6000 
index-url = https://pypi.tuna.tsinghua.edu.cn/simple 
trusted-host = pypi.tuna.tsinghua.edu.cn

```

## 升级第三方库

```python
pip install --upgrade SomePackage
```

## 显示版本

```
pip --version  # 显示版本和路径
```

## 升级pip

```
pip install --upgrade pip
```

## 查看库的信息

```python
pip show psutil
pip show psutil #查看指定包的详细信息
```

## 搜索库

```python
pip search SomePackage
```

