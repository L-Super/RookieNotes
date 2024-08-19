CMake 中的策略（POLICY）用于保留跨多个版本的向后兼容行为。
当引入新策略时，较新的 CMake 版本将开始警告向后兼容行为。可以通过使用 [`cmake_policy()`](https://runebook.dev/zh/docs/cmake/command/cmake_policy#command:cmake_policy) 命令显式请求 OLD 或向后兼容行为来禁用警告。还可以请求 `NEW` 或策略的非向后兼容行为，也可以避免警告。每个策略还可以使用 `CMAKE_POLICY_DEFAULT_CMP<NNNN>` 变量在命令行上显式设置为 `NEW` 或 `OLD` 行为。

策略是一种弃用机制，而不是可靠的功能切换。策略几乎不应该设置为 `OLD` ，除非在冻结或稳定的代码库中消除警告，或者暂时作为更大迁移路径的一部分。每个策略的 `OLD` 行为都是不受欢迎的，将在未来版本中替换为错误条件。

示例：
```cmake
if(POLICY CMP0990)
  cmake_policy(SET CMP0990 NEW)
endif()
```
这具有将 `NEW` 行为与用户可能正在使用的较新 CMake 版本一起使用的效果，并且不会发出兼容性警告。