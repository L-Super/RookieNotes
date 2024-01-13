因为 OpenGL 只是一个标准/规范，具体的实现是由驱动开发商针对特定显卡实现的。由于 OpenGL 驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询。

GLAD 是一个[开源](https://github.com/Dav1dde/glad)的库，它能解决我们上面提到的那个繁琐的问题。GLAD 的配置与大多数的开源库有些许的不同，GLAD 使用了一个[在线服务](http://glad.dav1d.de/)。在这里我们能够告诉 GLAD 需要定义的 OpenGL 版本，并且根据这个版本加载所有相关的 OpenGL 函数。

打开 GLAD 的[在线服务](http://glad.dav1d.de/)，将语言 (Language)设置为**C/C++**，在 API 选项中，选择**3.3**以上的 OpenGL (gl)版本（我们的教程中将使用 3.3 版本，但更新的版本也能用）。之后将模式 (Profile)设置为**Core**，并且保证选中了**生成加载器**(Generate a loader)选项。现在可以先（暂时）忽略扩展 (Extensions)中的内容。都选择完之后，点击**生成**(Generate)按钮来生成库文件。

GLAD 现在应该提供给你了一个 zip 压缩文件，包含两个头文件目录，和一个**glad. c**文件。