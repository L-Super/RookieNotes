
## 创建 submodule
使用 `git submodule add <submodule_url>` 命令可以在项目中创建一个子模块。此时项目仓库中会多出一个 `.gitmodules` 文件

`gitmodules` 保存的是子模块当前版本的版本号信息：
```
[submodule "SingleApplication"]
	path = SingleApplication
	url = https://github.com/itay-grudev/SingleApplication
```


