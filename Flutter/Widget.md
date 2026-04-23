在 Flutter 中几乎所有的对象都是一个 widget，Flutter 中的 widget 的概念更广泛，它不仅可以表示 UI 元素，也可以表示一些功能性的组件如：用于手势检测的 `GestureDetector` 、用于 APP 主题数据传递的 `Theme` 等等。

Flutter 中是通过 Widget 嵌套 Widget 的方式来构建 UI 和进行事件处理。一个 widget 可以对应多个 `Element`

## Widget 接口
Widget 类声明：
```dart
@immutable // 不可变的
abstract class Widget extends DiagnosticableTree {
  const Widget({ this.key });

  final Key? key;

  @protected
  @factory
  Element createElement();

  @override
  String toStringShort() {
    final String type = objectRuntimeType(this, 'Widget');
    return key == null ? type : '$type-$key';
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.defaultDiagnosticsTreeStyle = DiagnosticsTreeStyle.dense;
  }

  @override
  @nonVirtual
  bool operator ==(Object other) => super == other;

  @override
  @nonVirtual
  int get hashCode => super.hashCode;

  static bool canUpdate(Widget oldWidget, Widget newWidget) {
    return oldWidget.runtimeType == newWidget.runtimeType
        && oldWidget.key == newWidget.key;
  }
  ...
}
```

- `@immutable` 代表 Widget 是不可变的，这会限制 Widget 中定义的属性必须是不可变的（final）。为什么不允许 Widget 中定义的属性变化呢？因为，如果属性发生变化则会重新构建 Widget 树，即重新创建新的 Widget 实例来替换旧的 Widget 实例，所以允许 Widget 的属性变化是没有意义的，一旦 Widget 自己的属性变了自己就会被替换。这也是为什么 Widget 中定义的属性必须是 final 的原因。
- `Widget` 类继承自 `DiagnosticableTree`，`DiagnosticableTree` 即“诊断树”，主要作用是提供调试信息。
- `Key`:  `key` 属性类似于 React/Vue 中的 `key`，主要的作用是决定是否在下一次 `build` 时复用旧的 widget ，决定的条件在 `canUpdate()` 方法中。
- `createElement()`：在构建 UI 树时，会先调用此方法生成对应节点的 `Element` 对象。此方法是隐式调用的。
- `debugFillProperties(...)` 复写父类的方法，主要是设置诊断树的一些特性。
- `canUpdate(...)` 是一个静态方法，它主要用于在 widget 树重新 `build` 时复用旧的 widget ，其实具体来说，应该是：是否用新的 widget 对象去更新旧 UI 树上所对应的 `Element` 对象的配置；通过其源码可以看到，只要 `newWidget` 与 `oldWidget` 的 `runtimeType` 和 `key` 同时相等时就会用 `new widget` 去更新 `Element` 对象的配置，否则就会创建新的 `Element`。