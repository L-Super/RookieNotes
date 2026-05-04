当使用 flutter create 创建项目后，会得到以下目录结构：

```
my_app/
├── android/          # Android 平台相关代码
├── ios/              # iOS 平台相关代码
├── lib/              # Dart 代码目录（主要开发目录）
│   └── main.dart     # 应用入口文件
├── web/              # Web 平台相关代码
├── windows/          # Windows 桌面相关代码
├── macos/            # macOS 桌面相关代码
├── linux/            # Linux 桌面相关代码
├── test/             # 测试代码目录
├── pubspec.yaml      # 项目配置文件
├── pubspec.lock      # 依赖锁定文件（自动生成）
├── analysis_options.yaml  # Dart 分析器配置
└── README.md         # 项目说明文件
```

- lib/ - dart 核心代码目录
- android/ - Android 原生项目文件
    - 配置应用名称和图标
    - 设置应用权限
    - 集成原生 SDK（如 Google Play 服务）
- ios/ - iOS 平台原生项目文件（Xcode 项目）
    - 配置应用名称和图标
    - 设置 iOS 特有的配置
    - 集成 CocoaPods 依赖
- test/  - 测试代码，包含应用的测试代码
    - 单元测试
    - Widget 测试
    - 集成测试
- pubspec.yaml - 项目配置文件，定义项目名称、版本、依赖等。
