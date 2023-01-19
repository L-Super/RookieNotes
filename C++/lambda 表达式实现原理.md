lambda 表达式实现，其实是编译器为我们创建了一个类，这个类重载了 `()运算符，让我们可以像调用函数一样使用。所以，你写的 lambda 表达式，真正的实现是这个样子的：  
![](https://i1.wp.com/blog.feabhas.com/wp-content/uploads/2014/08/image_thumb.png?zoom=2&resize=365,332)  
  
而对于捕获变量的lambda表达式来说，编译器在创建类的时候，通过成员函数的形式保存了需要捕获的变量，所以看起来是这个样子：  
![](https://i2.wp.com/blog.feabhas.com/wp-content/uploads/2014/08/image_thumb4.png?zoom=2&resize=529,380)

似乎也没有什么神奇的地方。但正是由于编译器帮我们实现了细节，使我们的代码变得优雅和简洁了许多。