添加自定义命令
`add_custom_command` 有两个主要签名。
### 使用自定义命令作为生成器
```
add_custom_command(OUTPUT output1 [output2 ...]
                   COMMAND command1 [ARGS] [args1...]
                   [COMMAND command2 [ARGS] [args2...] ...]
                   [MAIN_DEPENDENCY depend]
                   [DEPENDS [depends...]]
                   [BYPRODUCTS [files...]]
                   [IMPLICIT_DEPENDS <lang1> depend1
                                    [<lang2> depend2] ...]
                   [WORKING_DIRECTORY dir]
                   [COMMENT comment]
                   [DEPFILE depfile]
                   [JOB_POOL job_pool]
                   [VERBATIM] [APPEND] [USES_TERMINAL]
                   [COMMAND_EXPAND_LISTS])
```

### 使用自定义命令作为目标钩子
```
add_custom_command(TARGET <target>
                   PRE_BUILD | PRE_LINK | POST_BUILD
                   COMMAND command1 [ARGS] [args1...]
                   [COMMAND command2 [ARGS] [args2...] ...]
                   [BYPRODUCTS [files...]]
                   [WORKING_DIRECTORY dir]
                   [COMMENT comment]
                   [VERBATIM] [USES_TERMINAL]
                   [COMMAND_EXPAND_LISTS])
```
- PRE_BUILD 将在此目标的任何其他规则之前运行 (仅 Visual Studio 生成器; 对于其他的生成器，其行为像 PRE_LINK)。
- PRE_LINK 绑定要在编译所有源之后但在链接 (或存档) 目标之前运行的命令。不适用于自定义目标。
- POST_BUILD 在执行完目标中的所有其他规则后运​​行。