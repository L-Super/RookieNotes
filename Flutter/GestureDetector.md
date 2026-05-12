`GestureDetector` 是 Flutter 中处理手势的核心组件，它可以检测各种用户交互手势并执行相应回调。

## 核心概念

**作用**：为子组件添加手势识别能力，无需修改子组件本身。

**特点**：
- 不可见组件（只负责检测手势）
- 支持多种手势同时检测
- 可控制手势冲突解决策略

```dart
GestureDetector(
  onTap: () => print('点击'),
  child: Container(width: 100, height: 100, color: Colors.blue),
)
```

## 手势类型总览

| 手势类型        | 回调                       | 触发时机     |
| ----------- | ------------------------ | -------- |
| **点击类**     |                          |          |
| 点击          | `onTap`                  | 轻触后抬起    |
| 双击          | `onDoubleTap`            | 快速两次点击   |
| 长按          | `onLongPress`            | 长按 500ms |
| 长按开始        | `onLongPressStart`       | 长按开始瞬间   |
| 长按结束        | `onLongPressEnd`         | 长按结束     |
| **按下与抬起**   |                          |          |
| 按下          | `onTapDown`              | 手指刚接触屏幕  |
| 抬起          | `onTapUp`                | 手指抬起     |
| 取消          | `onTapCancel`            | 手势被中断    |
| **拖拽类**     |                          |          |
| 拖拽开始        | `onPanStart`             | 开始拖拽     |
| 拖拽更新        | `onPanUpdate`            | 拖拽过程中    |
| 拖拽结束        | `onPanEnd`               | 拖拽结束     |
| **水平/垂直拖拽** |                          |          |
| 水平拖拽        | `onHorizontalDragUpdate` | 水平方向拖拽   |
| 垂直拖拽        | `onVerticalDragUpdate`   | 垂直方向拖拽   |
| **缩放类**     |                          |          |
| 缩放开始        | `onScaleStart`           | 双指触摸开始   |
| 缩放更新        | `onScaleUpdate`          | 双指缩放/旋转中 |
| 缩放结束        | `onScaleEnd`             | 双指离开     |


## 常用手势示例

### 点击事件
```dart
GestureDetector(
  onTap: () {
    print('单击');
  },
  onDoubleTap: () {
    print('双击');
  },
  onLongPress: () {
    print('长按');
  },
  child: Container(
    width: 200,
    height: 200,
    color: Colors.blue,
    child: Center(child: Text('点击我')),
  ),
)
```

### 获取点击位置
```dart
GestureDetector(
  onTapDown: (TapDownDetails details) {
    print('点击位置: ${details.globalPosition}');
    print('相对组件位置: ${details.localPosition}');
  },
  onTapUp: (TapUpDetails details) {
    print('抬起位置: ${details.globalPosition}');
  },
  child: Container(width: 200, height: 200, color: Colors.green),
)
```

### 拖拽移动
```dart
class DraggableBox extends StatefulWidget {
  @override
  State<DraggableBox> createState() => _DraggableBoxState();
}

class _DraggableBoxState extends State<DraggableBox> {
  Offset _position = Offset(0, 0);

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        Positioned(
          left: _position.dx,
          top: _position.dy,
          child: GestureDetector(
            onPanUpdate: (DragUpdateDetails details) {
              setState(() {
                _position += details.delta;
              });
            },
            child: Container(
              width: 100,
              height: 100,
              color: Colors.orange,
            ),
          ),
        ),
      ],
    );
  }
}
```

### 缩放和旋转
```dart
class ScaleRotateWidget extends StatefulWidget {
  @override
  State<ScaleRotateWidget> createState() => _ScaleRotateWidgetState();
}

class _ScaleRotateWidgetState extends State<ScaleRotateWidget> {
  double _scale = 1.0;
  double _rotation = 0.0;

  @override
  Widget build(BuildContext context) {
    return Center(
      child: GestureDetector(
        onScaleUpdate: (ScaleUpdateDetails details) {
          setState(() {
            _scale = details.scale;
            _rotation = details.rotation;
          });
        },
        child: Transform(
          alignment: Alignment.center,
          transform: Matrix4.identity()
            ..scale(_scale)
            ..rotateZ(_rotation),
          child: Container(
            width: 200,
            height: 200,
            color: Colors.purple,
            child: Center(
              child: Text(
                '缩放: ${_scale.toStringAsFixed(2)}\n旋转: ${_rotation.toStringAsFixed(2)}',
                textAlign: TextAlign.center,
                style: TextStyle(color: Colors.white),
              ),
            ),
          ),
        ),
      ),
    );
  }
}
```

## 手势冲突处理

### behavior 属性
控制 GestureDetector 如何响应空白区域的触摸：

```dart
GestureDetector(
  // 默认：仅在子组件范围内响应
  behavior: HitTestBehavior.deferToChild,
  onTap: () => print('点击'),
  child: Container(width: 100, height: 100, color: Colors.blue),
)

GestureDetector(
  // 透明区域也能响应（整个组件区域）
  behavior: HitTestBehavior.opaque,
  onTap: () => print('整个区域都响应'),
  child: Container(width: 100, height: 100, color: Colors.transparent),
)

GestureDetector(
  // 穿透子组件，父级也能响应
  behavior: HitTestBehavior.translucent,
  onTap: () => print('自己和父组件都响应'),
  child: Container(width: 100, height: 100),
)
```

### 手势消除竞争
```dart
GestureDetector(
  // 竞争失败时仍会触发 onTap
  onTap: () => print('点击'),
  
  // 水平拖拽优先于垂直拖拽
  onHorizontalDragUpdate: (details) {
    print('水平拖拽');
  },
  onVerticalDragUpdate: (details) {
    print('垂直拖拽');
  },
  child: Container(width: 200, height: 200, color: Colors.red),
)
```


## 高级用法

### 组合手势判断
```dart
GestureDetector(
  onLongPress: () => print('长按'),
  onLongPressStart: (details) => print('长按开始'),
  onLongPressEnd: (details) => print('长按结束'),
  child: Container(width: 100, height: 100, color: Colors.teal),
)

// 获取长按位置
onLongPressStart: (LongPressStartDetails details) {
  print('长按位置: ${details.globalPosition}');
}
```

### 控制手势识别方向
```dart
// 仅允许水平拖拽
GestureDetector(
  onHorizontalDragUpdate: (details) {
    // 水平拖拽处理
  },
  // 不设置 onVerticalDragUpdate，垂直拖拽不响应
  child: Container(width: 200, height: 200, color: Colors.amber),
)
```

### RawGestureDetector（自定义手势）
```dart
import 'package:flutter/widgets.dart';

class CustomTapGestureRecognizer extends TapGestureRecognizer {
  @override
  void handleTapUp({required TapUpDetails details}) {
    print('自定义手势处理');
    super.handleTapUp(details: details);
  }
}

RawGestureDetector(
  gestures: {
    CustomTapGestureRecognizer:
        GestureRecognizerFactoryWithHandlers<CustomTapGestureRecognizer>(
      () => CustomTapGestureRecognizer(),
      (instance) => instance.onTap = () => print('点击'),
    ),
  },
  child: Container(width: 100, height: 100, color: Colors.pink),
)
```


