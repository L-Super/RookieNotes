
## 创建项目：

```
flutter create my_first_app
```

> [!note]
> 项目名称必须使用小写字母和下划线，不能包含空格或特殊字符。

## 运行项目

```
flutter run
```

指定运行设备

如果电脑连接了多个设备（或模拟器），可以使用 `-d` 参数指定运行设备：

| 设备           | 命令                   |
| ------------- | --------------------- |
| Android 模拟器 | `flutter run -d android` |
| iOS 模拟器     | `flutter run -d iphone`  |
| Chrome 浏览器  | `flutter run -d chrome`  |
| Windows 桌面   | `flutter run -d windows` |

## 查看可用设备

运行以下命令查看所有可用的运行设备：

```
flutter devices
```

输出示例：

```
2 connected devices:
Chrome (web)      • chrome           • web-javascript • Chrome  • web
Windows (windows) • windows          • windows        • x64    • Windows 10.0.0
```

## 依赖管理

### 添加依赖

使用命令添加依赖：

```
flutter pub add http
```

或者手动编辑 `pubspec.yaml` 后运行：

```
flutter pub get
```

### 常用依赖命令

| 命令                       | 说明               |
| ------------------------- | ----------------- |
| `flutter pub get`            | 获取所有依赖       |
| `flutter pub add package`    | 添加依赖           |
| `flutter pub remove package` | 移除依赖           |
| `flutter pub outdated`       | 检查过时的依赖     |
| `flutter pub upgrade`        | 升级依赖到最新版本 |

