`Image` 是 Flutter 中用于显示图像的组件，支持多种图片来源和丰富的配置选项。

## 图片来源构造函数

| 构造函数        | 用途         | 示例                                             |
| --------------- | ------------ | ------------------------------------------------ |
| `Image.network` | 网络图片     | `Image.network('https://example.com/image.jpg')` |
| `Image.asset`   | 本地资源图片 | `Image.asset('assets/images/logo.png')`          |
| `Image.file`    | 本地文件图片 | `Image.file(File('/storage/image.jpg'))`         |
| `Image.memory`  | 内存字节数组 | `Image.memory(bytes)`                            |

## 基本使用

### 网络图片
```dart
Image(
  image: NetworkImage(
      "https://picsum.photos/200/300"),
  width: 100,
)

// 构造函数 Image.network    
Image.network(
  'https://picsum.photos/200/300',
  width: 200,
  height: 200,
)
```

### 本地资源图片
```dart
// 1. 在 pubspec.yaml 中声明
flutter:
  assets:
    - assets/images/logo.png

Image(
  image: AssetImage("images/logo.png"),
);

// 构造函数Image.asset
Image.asset('assets/images/logo.png')
```

### 文件图片
```dart
import 'dart:io';

Image.file(File('/data/user/0/com.example.app/cache/photo.jpg'))
```

## 常用属性

```dart
const Image({
  super.key,
  required this.image,
  this.frameBuilder,
  this.loadingBuilder,
  this.errorBuilder,
  this.semanticLabel,
  this.excludeFromSemantics = false,
  this.width,
  this.height,
  this.color,
  this.opacity,
  this.colorBlendMode,
  this.fit,
  this.alignment = Alignment.center,
  this.repeat = ImageRepeat.noRepeat,
  this.centerSlice,
  this.matchTextDirection = false,
  this.gaplessPlayback = false,
  this.isAntiAlias = false,
  this.filterQuality = FilterQuality.medium,
});
```
`Image.network()` 等其他构造函数会有 `cacheWidth` / `cacheHeight` 参数。

| 属性                           | 类型                      | 说明                          |
| ---------------------------- | ----------------------- | --------------------------- |
| `width` / `height`           | `double?`               | 宽高（非必填，不设置则按原图尺寸）           |
| `fit`                        | `BoxFit?`               | 填充模式                        |
| `alignment`                  | `Alignment`             | 对齐方式（默认 `Alignment.center`） |
| `repeat`                     | `ImageRepeat`           | 重复模式                        |
| `color`                      | `Color?`                | 着色（需配合 `colorBlendMode`）    |
| `colorBlendMode`             | `BlendMode?`            | 着色混合模式                      |
| `opacity`                    | `Animation<double>?`    | 透明度动画                       |
| `filterQuality`              | `FilterQuality`         | 渲染质量（`low`/`medium`/`high`） |
| `cacheWidth` / `cacheHeight` | `int?`                  | 解码时缩放（节省内存）                 |
| `errorBuilder`               | `Widget Function(...)?` | 加载失败时的回调                    |
| `frameBuilder`               | `Widget Function(...)?` | 帧构建回调                       |
| `loadingBuilder`             | `Widget Function(...)?` | 加载进度回调                      |

###  `BoxFit` 填充模式详解

| 模式        | 效果                             |
| ----------- | -------------------------------- |
| `fill`      | 拉伸填满，可能变形               |
| `contain`   | 等比缩放，完整显示（可能有黑边） |
| `cover`     | 等比缩放，填满容器（可能裁剪）   |
| `fitWidth`  | 宽度适配，高度可能溢出或不足     |
| `fitHeight` | 高度适配，宽度可能溢出或不足     |
| `scaleDown` | 效果同 `contain`，但禁止放大     |
| `none`      | 原图大小，超出裁剪               |

```dart
// 示例：封面图常用 cover
Image.network(
  url,
  width: double.infinity,
  height: 200,
  fit: BoxFit.cover,
)
```

## 加载状态处理

### 基础加载指示器
```dart
Image.network(
  url,
  loadingBuilder: (context, child, loadingProgress) {
    if (loadingProgress == null) return child;
    return Center(
      child: CircularProgressIndicator(
        value: loadingProgress.expectedTotalBytes != null
            ? loadingProgress.cumulativeBytesLoaded / 
              loadingProgress.expectedTotalBytes!
            : null,
      ),
    );
  },
  errorBuilder: (context, error, stackTrace) {
    return Icon(Icons.error, color: Colors.red);
  },
)
```

### 使用 `FadeInImage`（推荐）
```dart
FadeInImage.network(
  url,
  placeholder: AssetImage('assets/loading.gif'),
  imageErrorBuilder: (context, error, stackTrace) {
    return Icon(Icons.broken_image);
  },
  fit: BoxFit.cover,
)
```


## 性能优化

### 解码时缩放（省内存）
```dart
// 原图 4000x3000，但只需要显示 200x200
Image.network(
  url,
  cacheWidth: 200,   // 解码时直接缩放到 200px 宽
  cacheHeight: 200,  // 解码时直接缩放到 200px 高
)
```

### 使用 `precacheImage` 预加载
```dart
class MyPage extends StatefulWidget {
  @override
  State<MyPage> createState() => _MyPageState();
}

class _MyPageState extends State<MyPage> {
  @override
  void initState() {
    super.initState();
    // 预加载图片到缓存
    precacheImage(NetworkImage(url), context);
  }
  
  @override
  Widget build(BuildContext context) {
    return Image.network(url); // 直接从缓存读取
  }
}
```


## 缓存策略

### 默认缓存
- **内存缓存**：最多 100MB，最近使用的图片
- **磁盘缓存**：Flutter 不提供，系统 ImageCache 仅内存

### 清除缓存
```dart
import 'package:flutter/widgets.dart';

// 清空内存缓存
imageCache.clear();
imageCache.clearLiveImages();

// 或按需清除单张
imageCache.evict(key);
```

### 推荐第三方缓存方案
| 包                      | 特点                             |
| ----------------------- | -------------------------------- |
| `cached_network_image`  | 自动磁盘缓存，加载占位，错误处理 |
| `flutter_cache_manager` | 自定义缓存策略                   |

```dart
CachedNetworkImage(
  imageUrl: url,
  placeholder: (context, url) => CircularProgressIndicator(),
  errorWidget: (context, url, error) => Icon(Icons.error),
)
```

## 常用场景示例

### 圆角图片
```dart
ClipRRect(
  borderRadius: BorderRadius.circular(16),
  child: Image.network(url, width: 100, height: 100),
)

// 或使用 DecorationImage
Container(
  width: 100,
  height: 100,
  decoration: BoxDecoration(
    borderRadius: BorderRadius.circular(16),
    image: DecorationImage(
      image: NetworkImage(url),
      fit: BoxFit.cover,
    ),
  ),
)
```

### 圆形头像
```dart
CircleAvatar(
  radius: 50,
  backgroundImage: NetworkImage(url),
)

// 或
ClipOval(
  child: Image.network(url, width: 100, height: 100),
)
```

### 本地 SVG
```dart
// 需要 flutter_svg 包
SvgPicture.asset(
  'assets/icons/icon.svg',
  width: 24,
  height: 24,
  color: Colors.blue, // 着色
)
```
