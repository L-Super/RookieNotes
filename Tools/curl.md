curl 是常用的命令行工具，用来请求 Web 服务器。它的名字就是客户端（client）的 URL 工具的意思。

它的功能非常强大，命令行参数多达几十种。如果熟练的话，完全可以取代 Postman 这一类的图形界面工具。


---

常用curl命令
## -i

`-i`参数打印出服务器回应的 HTTP 标头。

收到服务器回应后，先输出服务器回应的标头，然后空一行，再输出网页的源码

```bash
$ curl -i limuran.top
HTTP/1.1 301 Moved Permanently
Server: nginx/1.25.3
Date: Tue, 12 Dec 2023 01:45:58 GMT
Content-Type: text/html
Content-Length: 169
Connection: keep-alive
Location: https://limuran.top/

<html>
<head><title>301 Moved Permanently</title></head>
<body>
<center><h1>301 Moved Permanently</h1></center>
<hr><center>nginx/1.25.3</center>
</body>
</html>
```

## -I

`-I`参数向服务器发出 HEAD 请求，然会将服务器返回的 HTTP 标头打印出来。

输出服务器对 HEAD 请求的回应

```bash
$ curl -I limuran.top
HTTP/1.1 301 Moved Permanently
Server: nginx/1.25.3
Date: Tue, 12 Dec 2023 02:02:20 GMT
Content-Type: text/html
Content-Length: 169
Connection: keep-alive
Location: https://limuran.top/
```

>  `--head`参数等同于`-I`。



## -o

`-o`参数将服务器的回应保存成文件，等同于`wget`命令。

```bash
$ curl -o example.html https://www.example.com
```

上面命令将`www.example.com`保存成`example.html`。

## -O

`-O`参数将服务器回应保存成文件，并将 URL 的最后部分当作文件名。

```bash
$ curl -O https://www.example.com/foo/bar.html
```

上面命令将服务器回应保存成文件，文件名为`bar.html`。

## **-v**

`-v`参数输出通信的整个过程，用于调试。

```bash
$ curl -v limuran.top
*   Trying 119.23.238.106:80...
* Connected to limuran.top (119.23.238.106) port 80 (#0)
> GET / HTTP/1.1
> Host: limuran.top
> User-Agent: curl/7.81.0
> Accept: */*
>
* Mark bundle as not supporting multiuse
< HTTP/1.1 301 Moved Permanently
< Server: nginx/1.25.3
< Date: Tue, 12 Dec 2023 02:07:02 GMT
< Content-Type: text/html
< Content-Length: 169
< Connection: keep-alive
< Location: https://limuran.top/
<
<html>
<head><title>301 Moved Permanently</title></head>
<body>
<center><h1>301 Moved Permanently</h1></center>
<hr><center>nginx/1.25.3</center>
</body>
</html>
* Connection #0 to host limuran.top left intact
```

`--trace`参数也可以用于调试，还会输出原始的二进制数据。

```bash
$ curl --trace - https://www.example.com
```





> [curl 的用法指南](https://www.ruanyifeng.com/blog/2019/09/curl-reference.html)
>
> [Curl Cookbook](https://catonmat.net/cookbooks/curl)