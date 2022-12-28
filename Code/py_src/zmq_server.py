import zmq

if __name__ == '__main__':
    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:20777")

    while True:
        #  Wait for next request from client
        message = socket.recv()

        if message == b"DisksOverview":
            # 返回的re是str类型，如果用send_json()发送，会导致客户端解析json失败崩溃
            re = monitor.json_disk_overview()
            socket.send_string(re)
            # socket.send_json(re)

        elif message == b"DisksInfo":
            re = monitor.json_disks_info()
            socket.send_string(re)

        elif message == b"DiskIO":
            re = monitor.json_all_disks_io()
            socket.send_string(re)

        else:
            socket.send(b"invalid request")

    socket.close()
    context.term()

