虚拟环境

用于创建和管理虚拟环境的模块称为 [`venv`](https://docs.python.org/zh-cn/3/library/venv.html#module-venv)

## 创建虚拟环境

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

## 激活虚拟环境

### Windows

在Windows上
cmd：

```
tutorial-env\Scripts\activate.bat
```
Powershell：
```
tutorial-env\Scripts\activate
```
注意：Powershell 终端默认禁用执行 `.ps1` 脚本，需要启用才可执行。
```
tutorial-env\Scripts\activate : 无法加载文件 C:\Users\Listening\...\tutorial-env\Scripts\Activate.ps1，因为在此系统上禁止运行脚本。有关详细信息，请参阅 https:/go.microsoft.com/fwlink/
?LinkID=135170 中的 about_Execution_Policies。
```
因为在 Windows 上默认执行策略 `Restricted`会阻止所有脚本运行，包括在本地计算机上编写的脚本。 有关详细信息，请参阅 [about_Execution_Policies](https://learn.microsoft.com/zh-cn/powershell/module/microsoft.powershell.core/about/about_execution_policies?view=powershell-7.3)。

执行策略保存在注册表中，因此只需在每台计算机上更改一次。

更改执行策略，管理员打开PowerShell：

```powershell
Set-ExecutionPolicy AllSigned
```

或

```powershell
Set-ExecutionPolicy RemoteSigned
```

更改立即生效。

### Linux或MacOS

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

## 关闭虚拟环境

```python
deactivate
```



注：再次进入重新虚拟环境，重新输入`source tutorial-env/bin/activate`即可