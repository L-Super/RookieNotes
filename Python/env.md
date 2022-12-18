虚拟环境

用于创建和管理虚拟环境的模块称为 [`venv`](https://docs.python.org/zh-cn/3/library/venv.html#module-venv)

-   创建虚拟环境

```python
python3 -m venv tutorial-env
```

可能会出现以下报错：

```shell
leoya@DESKTOP-LMR:~/monitor-py$ python3 -m venv monitor
The virtual environment was not created successfully because ensurepip is not
available.  On Debian/Ubuntu systems, you need to install the python3-venv
package using the following command.

    apt install python3.8-venv

You may need to use sudo with that command.  After installing the python3-venv
package, recreate your virtual environment.

Failing command: ['/home/leoya/monitor-py/monitor/bin/python3', '-Im', 'ensurepip', '--upgrade', '--default-pip']
```

因为没有venv环境，所以需要运行`apt install python3.8-venv`进行安装后即可使用

-   激活虚拟环境

在Windows上，运行:

```
tutorial-env\Scripts\activate.bat
```

在Unix或MacOS上，运行:

```
source tutorial-env/bin/activate
```
激活虚拟环境将改变你所用终端的提示符，以显示你正在使用的虚拟环境，并修改环境以使 python 命令所运行的将是已安装的特定 Python 版本。

```
$ leoya@DESKTOP-LMR:~/monitor-py$ source monitor/bin/activate
(monitor) leoya@DESKTOP-LMR:~/monitor-py$
  ...
```

-   关闭虚拟环境

```python
deactivate
```



注：再次进入重新虚拟环境，重新输入`source tutorial-env/bin/activate`即可