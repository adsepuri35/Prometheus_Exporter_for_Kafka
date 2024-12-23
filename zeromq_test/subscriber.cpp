#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

int main() {
    // Prepare our context and socket
    zmq::context_t context(2);
    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect("tcp://localhost:5555");

    for (int request_nbr = 0; request_nbr != 10; ++request_nbr) {
        zmq::message_t request(5);
        memcpy(request.data(), "Hello", 5);
        std::cout << "Sending Hello " << request_nbr << "..." << std::endl;
        socket.send(request, zmq::send_flags::none);

        // Get the reply
        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);
        std::cout << "Received World " << request_nbr << std::endl;
    }
    return 0;
}