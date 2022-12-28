#
#   Hello World client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends "Hello" to server, expects "World" back
#
import json
import time

import zmq

def block_socket():
    """
    阻塞socket通信
    """
    context = zmq.Context()

    #  Socket to talk to server
    print("Connecting to hello world server…")
    socket = context.socket(zmq.REQ)
    socket.connect("tcp://localhost:20777")

    #  Do 10 requests, waiting each time for a response
    for request in range(2):
        print(f"Sending request {request} …")

        socket.send_string("DisksInfo")

        #  Get the reply.
        message = socket.recv()

        data = json.loads(message)
        print(f"Received reply {request}  {data}")

def no_block_socket():
    """
    非阻塞socket
    主要针对socket.recv()设置非阻塞，当服务端不在线时，可以避免无限等待。
    """
    context = zmq.Context()

    # Socket to talk to server
    print("Connecting to hello world server…")
    socket = context.socket(zmq.REQ)
    socket.connect("tcp://localhost:20777")

    # 轮询接口
    poller = zmq.Poller()
    # 为 I/O 监视注册一个 zmq 套接字，要监视的事件。可以是POLLIN、POLLOUT或POLLIN|POLLOUT。
    poller.register(socket, zmq.POLLIN)
    # 设置 2s 超时
    TIMEOUT = 2000

    # Do 10 requests, waiting each time for a response
    for i in range(10):
        socket.send(b"Hello")
        # 方式一
        # socks = dict(poller.poll(TIMEOUT))
        # # 如果socket在socks里 且 socks[socket]的事件等于zmq.POLLIN
        # if socket in socks and socks[socket] == zmq.POLLIN:
        #     reply = socket.recv()
        #     print("Received reply ", i, "[", reply, "]")

        # 方式二
        # 轮询已注册的 zmq 以获取I/O。
        # 如果当前有待处理的事件，此函数将立即返回。否则，该函数将在第一个事件可用或超时毫秒过后立即返回。
        if poller.poll(TIMEOUT):
            reply = socket.recv()
            print("Received reply ", i, "[", reply, "]")
        else:
            print("timeout")
            break
        time.sleep(2)

if __name__ == '__main__':
    block_socket()
    no_block_socket()