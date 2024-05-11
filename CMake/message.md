语法格式：
```cmake
 message([<mode>] "message text" ...)
```

指定消息的类型：

+ (none) or NOTICE：将向 stderr 输出一条消息，以吸引用户的注意。
+ STATUS：建议用于用户的主要消息
+ WARNING：警告，会继续执行
+ AUTHOR_WARNING: CMake 警告。继续处理。
+ AUTHOR_WARNING：警告 (dev),，会继续执行
+ SEND_ERROR：错误,，继续处理，但跳过生成的步骤
+ FATAL_ERROR：错误，停止处理和生成
+ DEPRECATION: 若启用了 CMAKE_ERROR_DEPRECATED 或 CMAKE_WARN_DEPRECATED 变量，将做出相应处理。
+ VERBOSE: 将继续处理，用于通常不是很有必要的更详细的信息。
+ DEBUG: 将继续处理，并包含在项目出现问题时可能有用的详细信息。
+ TRACE:将继续处理，并建议在项目开发期间打印消息。通常，在发布项目之前，将这些类型的消息删除。

```cmake
message("Important message")
message(NOTICE "Important message")#与上句等效
message(STATUS "STATUS message")
```

