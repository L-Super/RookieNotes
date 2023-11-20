## session
Session 对象允许您在请求之间保留某些参数。最常见的应用是它可以让 cookie 保持，在后续的一串请求中不需要再带上cookie。

```python
# 实例化session对象，后面用法和直接使用requests一样
s = requests.Session()
# 第一步：发送一个请求，用于设置请求中的cookies
# tips: http://httpbin.org能够用于测试http请求和响应
s.get('http://httpbin.org/cookies/set/sessioncookie/123456789')
# 第二步：再发送一个请求，用于查看当前请求中的cookies
r = s.get("http://httpbin.org/cookies")
print(r.text)
```
第二次请求已经携带上了第一次请求所设置的cookie，即通过session达到了保持cookie的目的。