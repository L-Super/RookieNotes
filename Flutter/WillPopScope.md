导航返回拦截（WillPopScope）用于返回按钮拦截，让你可以自定义返回行为（如：二次确认、阻止返回、执行业务逻辑）。

为了避免用户误触返回按钮而导致 App 退出，很多 App 会拦截用户点击返回键的按钮，进行一些防误触判断，比如当用户在某一个时间段内点击两次时，才会认为用户是要退出（而非误触）。

`WillPopScope` 的默认构造函数：

```dart
const WillPopScope({
  ...
  required WillPopCallback onWillPop,
  required Widget child
})
```

`onWillPop` 是一个回调函数，当用户点击返回按钮时被调用（包括导航返回按钮及 Android 物理返回按钮）。该回调需要返回一个 `Future` 对象，如果返回的 `Future` 最终值为 `false` 时，则当前路由不出栈 (不会返回)；最终值为 `true` 时，当前路由出栈退出。

### 示例

为了防止用户误触返回键退出，我们拦截返回事件。当用户在 1 秒内点击两次返回按钮时，则退出；如果间隔超过 1 秒则不退出，并重新记时。代码如下：

```dart
import 'package:flutter/material.dart';

class WillPopScopeTestRoute extends StatefulWidget {
  @override
  WillPopScopeTestRouteState createState() {
    return WillPopScopeTestRouteState();
  }
}

class WillPopScopeTestRouteState extends State<WillPopScopeTestRoute> {
  DateTime? _lastPressedAt; //上次点击时间

  @override
  Widget build(BuildContext context) {
    return WillPopScope(
      onWillPop: () async {
        if (_lastPressedAt == null ||
            DateTime.now().difference(_lastPressedAt!) > Duration(seconds: 1)) {
          //两次点击间隔超过1秒则重新计时
          _lastPressedAt = DateTime.now();
          return false;
        }
        return true;
      },
      child: Container(
        alignment: Alignment.center,
        child: Text("1秒内连续按两次返回键退出"),
      ),
    );
  }
}
```

退出确认：
```dart
class ConfirmExitPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return WillPopScope(
      onWillPop: () async {
        // 显示确认对话框
        final shouldPop = await showDialog<bool>(
          context: context,
          builder: (context) => AlertDialog(
            title: Text('确认退出'),
            content: Text('确定要离开吗？'),
            actions: [
              TextButton(
                onPressed: () => Navigator.pop(context, false),
                child: Text('取消'),
              ),
              TextButton(
                onPressed: () => Navigator.pop(context, true),
                child: Text('确定'),
              ),
            ],
          ),
        ) ?? false;
        
        return shouldPop;  // true=退出, false=留下
      },
      child: Scaffold(
        appBar: AppBar(title: Text('需要确认的页面')),
        body: Center(child: Text('按返回键会弹出确认框')),
      ),
    );
  }
}
```