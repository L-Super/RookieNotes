语法格式：
```cmake
 message([<mode>] "message text" ...)
```

指定消息的类型：

+ (none) or NOTICE：重要消息
+ STATUS：可能感兴趣的主要消息
+ WARNING：警告，会继续执行
+ AUTHOR_WARNING：警告 (dev),，会继续执行
+ SEND_ERROR：错误,，继续处理，但跳过生成的步骤
+ FATAL_ERROR：错误，停止处理和生成

```cmake
message("Important message")
message(NOTICE "Important message")#与上句等效
message(STATUS "STATUS message")
```

