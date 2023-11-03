C++ 17后，可以直接在if语句里直接初始化一个临时变量，它可以声明任意多个变量，或是一条结构化绑定声明

作用：将作用域限制于if里，而避免临时变量在if作用域外

```cpp
if (auto it = m.find(10); it != m.end()) { 
    return it->second.size(); 
}

if (std::lock_guard lock(mutex); shared_flag) { 
    unsafe_ping(); shared_flag = false; 
}
```

