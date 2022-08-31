编写类 String 的构造函数，析构函数，复制构造函数 
需要重载下面的运算符：
1、<、>、==和！=比较运算符
2、+=连接运算符和赋值运算符
 3、<<输出运算符和>>输入运算符

 ```C++
// String.h
#ifndef _STRING_H_
#define _STRING_H_

#include <iostream>
using namespace std;

class String
{
public:
String();//默认构造函数
String(int n,char c);//普通构造函数
String(const char* source);//普通构造函数
String(const String& s);//复制构造函数
~String();//析构函数
String& operator=(char* s);//重载=，实现字符串赋值
String& operator=(const String& s);//重载=，实现对象赋值
char& operator[](int i);//重载[]，实现数组运算
const char& operator[](int i)const;//重载[]，实现数组运算（对象为常量）
String& operator += (const String& s);//重载+=，实现与对象相加
String& operator += (const char* s);//重载+=，实现与字符串相加
friend ostream& operator << (ostream &out, String& s);//重载<<，实现输出流
friend istream& operator >> (istream &in, String& s);//重载>>，实现输入流
friend bool operator < (const String& left, const String& right);//重载<
friend bool operator > (const String& left, const String& right);//重载>
friend bool operator == (const String& left, const String& right);//重载==
friend bool operator != (const String& left, const String& right);//重载！=
//char* getData();//获取data指针
int length(); //获取字符串长度

private:
int size; //data表示的字符长度
char* data; //指向字符串数据
};
#endif
```


```c++
// String.cpp
#include <iostream>
#include <string.h>
#include "String.h"
using namespace std;

String::String(){ //默认构造函数,构造空字符串
data = new char[1]; //空字符串只有'\0'一个元素
*data = '\0';
size = 0;
}
String::String(int n, char c){ //普通构造函数
data = new char[n+1]; //含有N个相同字符的字符串
size = n;
char* temp = data; //保存data
while (n--) //做n次赋值
{
*temp++ = c;
}
*temp = '\0';
}
String::String(const char* source){//普通构造函数
if (source ==NULL) //字符串内容与source相同
{
data = new char[1]; //将data赋为空字符串
*data = '\0';
size = 0;
}else{
size = strlen(source); //复制source字符串
data = new char[size + 1];
strcpy(data, source);
}
}
String::String(const String& s){ //复制构造函数
data = new char[s.size + 1]; //字符串内容与对象s的相同
strcpy(data, s.data);
size = s.size;
}
String::~String(){//析构函数
if (data != NULL) //字符串内容与source相同
{
delete []data; //data不为空，释放堆内存
data = NULL;
size = 0;
}
}
String& String::operator=(char* s){//重载=，实现字符串赋值
if (data != NULL)
{
delete[]data;
}
size = strlen(s);
data = new char[size + 1];
strcpy(data, s); //复制目标字符串
return *this;
}
String& String::operator=(const String& s){//重载=，实现String对象赋值
if (this == &s) //如果对象S就是自己，直接返回*this
{
return *this;
}
if (data != NULL) //释放data堆内存
{
delete[]data;
}
size = strlen(s.data);
data = new char[size + 1]; //分配堆内存
strcpy(data, s.data); //复制对象s的字符串成员
return *this;
}
char& String::operator[](int i){//重载[]，实现数组运算
return data[i]; //取数组下标为i的字符元素
}
const char& String::operator[](int i)const{//重载[]，实现数组运算（对象为常量）
return data[i];
}
String& String::operator += (const String& s){//重载+=，实现与对象相加
int len = size + s.size + 1; //连接对象s的字符串成员
char *temp = data;
data = new char[len]; //申请足够的堆内存来存放连接后的字符串
size = len - 1;
strcpy(data, temp); //复制原来的字符串
strcat(data, s.data); //连接目标对象内的字符串成员
delete[] temp;
return *this;
}
String& String::operator += (const char* s){//重载+=，实现与字符串相加
if (s == NULL) //连接s字符串
{
return *this;
}
int len = size + strlen(s) + 1;
char *temp = data;
data = new char[len]; //申请足够的堆内存来存放连接后的字符串
size = len - 1;
strcpy(data, temp); //复制原来的字符串
strcat(data, s); //连接目标对象内的字符串成员
delete[] temp;
return *this;
}
int String::length(){ //获取字符串长度
return size;
}

ostream& operator << (ostream &out, String& s){//重载<<，实现输出流
for (int i = 0; i < s.length();i++) //打印对象s内字符串成员的所有字符元素
{
out << s[i]<<" "; //输出字符串中每一个字符元素
}
return out;
}
istream& operator >> (istream &in, String& s){//重载>>，实现输入流
char p[50];
in.getline(p, 50); //从输入流接收最多50个字符
s = p; //调用赋值函数
return in;
}
bool operator < (const String& left, const String& right){//重载<
int i = 0;
while (left[i] == right[i] && left[i] !=0&& right[i]!=0)
{
i++;
}
return left[i] - right[i]<0 ? true : false;
}
bool operator > (const String& left, const String& right){//重载>
int i = 0;
while (left[i] == right[i] && left[i] != 0 && right[i] != 0)
{
i++;
}
return left[i] - right[i] > 0 ? true : false;
}
bool operator == (const String& left, const String& right){//重载==
int i = 0;
while (left[i] == right[i] && left[i] != 0 && right[i] != 0)
{
i++;
}
return left[i] - right[i] == 0 ? true : false;
}
bool operator != (const String& left, const String& right){//重载！=
int i = 0;
while (left[i] == right[i] && left[i] != 0 && right[i] != 0)
{
i++;
}
return left[i] - right[i] != 0 ? true : false;
}
```