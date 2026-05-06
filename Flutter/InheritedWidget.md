InheritedWidget（数据共享）提供了一种在 widget 树中从上到下共享数据的方式，也是 `Provider` 等状态管理库的基础。

它能让子树中的任意 Widget 通过 `context.dependOnInheritedWidgetOfExactType<T>()` 获取它，并在数据变化时**自动重建**依赖它的 Widget。

当 `InheritedWidget` 的数据发生变化时，Flutter 会创建一个新的 `InheritedWidget` 实例，并调用其 `updateShouldNotify` 方法，用于判断新旧数据是否有变化。


示例：

```dart
class CounterProvider extends InheritedWidget {
  final int count;
  final VoidCallback onIncrement;

  const CounterProvider({
    super.key,
    required this.count,
    required this.onIncrement,
    required super.child,
  });

  // 核心方法：告诉框架是否需要重建依赖此 Widget 的组件
  @override
  bool updateShouldNotify(CounterProvider oldWidget) {
    return count != oldWidget.count; // count 变了才通知
  }

  // 便捷方法：方便子 Widget 获取最近实例
  static CounterProvider? of(BuildContext context) {
    return context.dependOnInheritedWidgetOfExactType<CounterProvider>();
  }
}

class CounterApp extends StatefulWidget {
  @override
  State<CounterApp> createState() => _CounterAppState();
}

class _CounterAppState extends State<CounterApp> {
  int _count = 0;

  void _increment() => setState(() => _count++);

  @override
  Widget build(BuildContext context) {
    return CounterProvider(
      count: _count,
      onIncrement: _increment,
      child: MaterialApp(home: CounterDisplay()),
    );
  }
    
  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
    // 父 widget 中的 InheritedWidget 改变(updateShouldNotify 返回 true)时会被调用。
    // 如果 build() 中没有依赖 InheritedWidget，则此回调不会被调用。
    print("CounterApp didChangeDependencies");
  }
}

// 子 Widget 获取数据
class CounterDisplay extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    // 获取并提供依赖关系
    final provider = CounterProvider.of(context)!;
    
    return Column(
      children: [
        Text('Count: ${provider.count}'), // count 变化会重建
        ElevatedButton(
          onPressed: provider.onIncrement,
          child: Text('Increment'),
        ),
      ],
    );
  }
}
```

## `of` 方法的两种写法对比

| 方法                                      | 行为                                           | 使用场景                         |
| :---------------------------------------- | :--------------------------------------------- | :------------------------------- |
| `dependOnInheritedWidgetOfExactType`      | 注册依赖关系，数据变化时**重建**当前 Widget    | 需要响应数据变化的场景           |
| `getElementForInheritedWidgetOfExactType` | 只获取实例，不注册依赖，数据变化时**不会重建** | 只读一次，或不想被重建的静态配置 |

```dart
// 重建版本（常用）
static CounterProvider? of(BuildContext context) {
  return context.dependOnInheritedWidgetOfExactType<CounterProvider>();
}

// 非重建版本（少用）
static CounterProvider? read(BuildContext context) {
  final element = context.getElementForInheritedWidgetOfExactType<CounterProvider>();
  return element?.widget as CounterProvider?;
}
```

## 生命周期与更新机制

```dart
class MyInherited extends InheritedWidget {
  final int data;
  
  const MyInherited({required this.data, required super.child});
  
  @override
  bool updateShouldNotify(MyInherited old) => data != old.data;
}

// 父组件重建时...
parent rebuild → 调用 MyInherited 构造函数 → 传入新 data
                                    ↓
                      updateShouldNotify 返回 true
                                    ↓
                    框架遍历 element 的依赖表
                                    ↓
                  所有 dependOn... 过的子 Widget 重建
```

**关键点**：

- 不是 `InheritedWidget` 主动推送，而是**标记依赖节点待重建**
- 重建发生在**下一帧**，由 Flutter 框架统一调度