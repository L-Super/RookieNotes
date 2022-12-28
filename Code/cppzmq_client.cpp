#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <zmq.hpp>

using namespace std::chrono_literals;

void no_block_socket(){
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

int main(int argc, char* argv[])
{
	std::string addr;
	std::string msg;

	// initialize the zmq context with a single IO thread
	zmq::context_t context{ 1 };
	// construct a REQ (request) socket and connect to interface
	zmq::socket_t socket{ context, ZMQ_REQ };

	addr = {"tcp://127.0.0.1:20777"};
	socket.connect(addr);
	/*
	 * void *poller = zmq_poller_new ();
	 * zmq_poller_event_t events [2];
	 * //First item refers to ØMQ socket 'socket'
	 * zmq_poller_add (poller, socket, ZMQ_POLLIN, NULL);
	 * //Second item refers to standard socket 'fd'
	 * zmq_poller_add_fd (poller, fd, ZMQ_POLLIN, NULL);
	 * //Poll for events indefinitely
	 * int rc = zmq_poller_wait_all (items, events, 2, -1);
	 * assert (rc >= 0);
	 * // Returned events will be stored in 'events'
	 * zmq_poller_destroy (&poller);
	 */
//	zmq::pollitem_t pollitem;
//	pollitem.socket = socket;
//	pollitem.events = ZMQ_POLLIN;
	zmq::pollitem_t items [] = {
		{ socket, 0, ZMQ_POLLIN, 0 }
	};

	// set up some static data to send
	const std::string data{ msg };

	for (auto request_num = 0; request_num < 10; ++request_num)
	{
		// initialize a request message
//		zmq::message_t request{ data.cbegin(), data.cend() };
		zmq::message_t request{ data };
		// send the request message
		std::cout << "Sending Hello " << request_num << "..." << std::endl;

		auto op = socket.send(request, zmq::send_flags::none);
		if(op.has_value())
		{
			std::cout<<"option value: "<<op.value()<<" data size: "<<data.size()<<std::endl;
		}

		zmq::poll (&items[0], 1, 3000); // 1s timeout in milliseconds
		zmq::message_t reply;
		if (items [0].revents & ZMQ_POLLIN) {
			socket.recv(reply);
			std::cout << "Received Reply " << request_num << " [" << reply.to_string() << "]" << std::endl;
		} else {
			std::cout << "Receive timeout!" << std::endl;
			break;
		}
//		// wait for reply from server
//		zmq::message_t reply{};
//		op = socket.recv(reply, zmq::recv_flags::none);
//		if(op.has_value())
//		{
//			std::cout<<"option value: "<<op.value()<<"data size: "<<reply.size()<<std::endl;
//		}
//		else
//		{
//			std::cout<<"EAGAIN \n";
//		}
//
//		std::cout << "Received " << reply.to_string() << " (" << request_num << ")" << std::endl;
		std::string re = reply.to_string();
		if (!re.empty()){
			nlohmann::json json = nlohmann::json::parse(re);
//		std::cout<<"dump: "<<json.dump()<<std::endl;
//		auto total = json.at("data");

			double total = json["data"]["lastCount"];
//		auto total = json["data"]["lastCount"].get<double>();
//		auto used = json["data"][1].get<double>();
			std::cout<<total<<" "<<"used"<<"\n";
		}


		std::this_thread::sleep_for(1s);
	}

//    std::cout<<"receive"<<request_data_from_raid("RequestBaseInfo")<<std::endl;
	return 0;
}
