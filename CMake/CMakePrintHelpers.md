CMakePrintHelpers 打印相关属性值


示例：
打印链接库的类型和源码路径
```cmake
include(CMakePrintHelpers)  
cmake_print_properties(TARGETS nlohmann_json::nlohmann_json  
        PROPERTIES TYPE SOURCE_DIR)
```
输出：
```bash
Properties for TARGET nlohmann_json::nlohmann_json:
nlohmann_json::nlohmann_json.TYPE = "INTERFACE_LIBRARY"
nlohmann_json::nlohmann_json.SOURCE_DIR = "F:/FlowD/cmake-build-debug/_deps/json-src"
```