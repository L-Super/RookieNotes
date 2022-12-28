Pyzmq 源自 zeromq

# Request-Reply 模式
REQ-REP 套接字对是同步的。客户端在发出 `zmq_send()` ，然后再发出 `zmq_recv()`。类似地，服务端按照需要的顺序发出 `zmq_recv()` 和 `zmq_send()`。

阻塞式客户端代码：
```preview
path: ../Code/zmq_client.py
start: 1
end: 29
```

非阻塞式客户端：
```preview
path: ../Code/zmq_client.py
start: 32
end: 70
```

服务端：
```preview
path: ../Code/zmq_server.py
```

