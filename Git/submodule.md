
## 创建 submodule
使用 `git submodule add <submodule_url>` 命令可以在项目中创建一个子模块。此时项目仓库中会多出一个 `.gitmodules` 文件

`gitmodules` 保存的是子模块当前版本的版本号信息：
```
[submodule "SingleApplication"]
	path = SingleApplication
	url = https://github.com/itay-grudev/SingleApplication
```
**指定 submodule 的位置**
比如，想要将第三方库放在 `src/third_party` 路径下：
```
git submodule add https://github.com/itay-grudev/SingleApplication src/third_party/SingleApplication
```
`gitmodules` 信息就会变成：
```
[submodule "src/third_party/SingleApplication"]
	path = src/third_party/SingleApplication
	url = https://github.com/itay-grudev/SingleApplication
```

## 获取 submodule
### 方式一
在克隆主项目的时候带上参数 `--recurse-submodules`，这样会递归地将项目中所有子模块的代码拉取。
```bash
git clone https://github.com/L-Super/QClipboard --recurse-submodules
```

### 方式二
在当前主项目中执行：

```bash
git submodule init  
git submodule update
```