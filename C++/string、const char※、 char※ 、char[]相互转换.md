# string、const char*、 char* 、char[ ]相互转换
使用时，要对源格式和目标格式进行初始化。源格式赋值为具体的内容，目标格式赋值为空。

| 目标格式 \ 源格式 | string               | const char*                 | char*                                                        | char[ ]                                                 |
| ----------------- | -------------------- | --------------------------- | ------------------------------------------------------------ | ------------------------------------------------------- |
| string            | NULL                 | const char*=string.c_str(); | const char\*=string.c_str();char\*=<const_cast><char\*>(const char\*); | for(int i=0;i< string.length();i++){ char[i]=string[];} |
| const char*       | string =const char*; | NULL                        | char\*=<const_cast><char\*>(const char\*);                      | strncpy_s(char,const char\*,n);                          |
| char*             | string =char*;       | const char\* = char\*;         | NULL                                                         | strncpy_s(char,char\*,n);                                |
| char[ ]           | string =char;        | const char*=char;           | char*=char;                                                  | NULL                                                    |

