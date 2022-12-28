#include "zmq.hpp"
int main () {
    //  Prepare our context and socket
    zmq::context_t context{2};
    zmq::socket_t socket (context, zmq::socket_type::rep);
    socket.bind ("tcp://*:5555");

    while (true) {
        zmq::message_t request;

        //  Wait for next request from client
        socket.recv (request, zmq::recv_flags::none);
        std::cout << "Received:" << request.to_string() << std::endl;

        //  Do some 'work'
        sleep(1);

        //  Send reply back to client
        zmq::message_t reply{"world"};
        socket.send (reply, zmq::send_flags::none);
    }
    return 0;
}