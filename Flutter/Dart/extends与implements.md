对比：

| 维度       | `extends` (继承) | `implements` (实现) |
| -------- | -------------- | ----------------- |
| **本质**   | 复用父类的**实现**    | 遵守接口的**契约**       |
| **方法体**  | 继承父类的方法体       | 必须重新实现所有方法        |
| **成员变量** | 继承父类的变量        | 必须重新声明所有变量        |
| **构造方法** | 必须调用父类构造       | 不继承，也不需调用         |
| **多重继承** | 只能继承一个类        | 可以实现多个接口          |
| **抽象方法** | 可以不实现（留给子类）    | 必须实现所有抽象成员        |

## `extends` - 继承实现

**目的：复用代码，建立 `is-a` 关系**

```dart
class Animal {
  String name = 'animal';
  
  void eat() {
    print('Animal eating');
  }
  
  void sleep() {
    print('Animal sleeping');
  }
}

class Dog extends Animal {
  // 继承了 name 变量
  // 继承了 eat() 和 sleep() 的实现
  
  @override
  void eat() {
    print('Dog eating');  // 可选：覆盖父类实现
  }
  
  @override
  void sleep() {
    super.sleep(); // 可以调用父类
  }
  
  void bark() {
    print('Dog barking');  // 新增方法
  }
}

void main() {
  var dog = Dog();
  print(dog.name);   // 'animal' - 继承了变量
  dog.eat();         // 'Dog eating' - 覆盖后的方法
  dog.sleep();       // 'Animal sleeping' - 继承来的方法
  dog.bark();        // 'Dog barking' - 新增方法
}
```

**特点：**
- 自动获得父类的所有非私有成员（变量、方法）
- 可以选择覆盖（`@override`）父类方法
- 必须调用父类构造器（如果父类没有默认构造）

## `implements` - 实现接口

**目的：遵守契约，建立 `as-a` 关系**

```dart
class Animal {
  String name = 'animal';
  
  void eat() {
    print('Animal eating');
  }
  
  void sleep() {
    print('Animal sleeping');
  }
}

class Dog implements Animal {
  // 必须重新声明所有变量
  @override
  String name = 'dog';
  
  // 必须重新实现所有方法
  @override
  void eat() {
    print('Dog eating');
  }
  
  @override
  void sleep() {
    print('Dog sleeping');  // 不能复用 Animal 的实现
  }
  
  void bark() {
    print('Dog barking');
  }
}
```

**特点：**
- Animal 只是类型约束，不提供任何实现
- Dog 必须提供所有接口成员的实现
- 不继承任何代码，只是"承诺"具有相同的接口

`implements` 可以实现多个接口，`extends` 只能继承一个类：

```dart
class Flyable {
  void fly() => print('Flying');
}

class Swimmable {
  void swim() => print('Swimming');
}

class Duck implements Flyable, Swimmable {
  @override
  void fly() => print('Duck flying');
  
  @override
  void swim() => print('Duck swimming');
}

// extends 只能一个
// class Duck extends Flyable, Swimmable {} // ❌ 错误
```

抽象类与接口
```dart
// 抽象类作为接口
abstract class Drawable {
  void draw();  // 抽象方法
}

class Circle implements Drawable {
  @override
  void draw() {
    print('Drawing circle');
  }
}

// 普通类也可以作为接口
class Logger {
  void log(String msg) {
    print(msg);  // 有实现，但 implements 不会继承它
  }
}

class FileLogger implements Logger {
  @override
  void log(String msg) {
    // 必须重新实现，不能复用父类的 print
    writeToFile(msg);
  }
  
  void writeToFile(String msg) {}
}
```
