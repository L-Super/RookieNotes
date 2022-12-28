#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <zmq.hpp>

using namespace std::chrono_literals;

// 阻塞
void block_socket()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
    // zmq::socket_t socket{ context, ZMQ_REQ };
    socket.connect("tcp://localhost:5555");

    // set up some static data to send
    const std::string data{"Hello"};

    for (auto request_num = 0; request_num < 10; ++request_num)
    {
        // send the request message
        std::cout << "Sending Hello " << request_num << "..." << std::endl;
        socket.send(zmq::buffer(data), zmq::send_flags::none);

        // wait for reply from server
        zmq::message_t reply{};
        socket.recv(reply, zmq::recv_flags::none);

        std::cout << "Received " << reply.to_string() << " (" << request_num << ")";
        std::cout << std::endl;
    }
}

//非阻塞
void no_block_socket()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};
    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, ZMQ_REQ};

    std::string addr = {"tcp://127.0.0.1:20777"};
    socket.connect(addr);

    zmq::pollitem_t items[] = {
        {socket, 0, ZMQ_POLLIN, 0}};

    // set up some static data to send
    const std::string data{"hello"};

    for (auto request_num = 0; request_num < 10; ++request_num)
    {
        // initialize a request message
        //		zmq::message_t request{ data.cbegin(), data.cend() };
        zmq::message_t request{data};
        // send the request message
        std::cout << "Sending Hello " << request_num << "..." << std::endl;

        auto op = socket.send(request, zmq::send_flags::none);
        // C++11 optional
        if (op.has_value())
        {
            std::cout << "option value: " << op.value() << " data size: " << data.size() << std::endl;
        }

        // 1s timeout in milliseconds
        zmq::poll(&items[0], 1, 1000);
        zmq::message_t reply;
        if (items[0].revents & ZMQ_POLLIN)
        {
            // wait for reply from server
            socket.recv(reply);
            std::cout << "Received Reply " << request_num << " [" << reply.to_string() << "]" << std::endl;
        }
        else
        {
            std::cout << "Receive timeout!" << std::endl;
            break;
        }
        std::this_thread::sleep_for(1s);
    }
}

int main(int argc, char *argv[])
{
    block_socket();
    no_block_socket();

    return 0;
}
