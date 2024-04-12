#include <iostream>
#include <boost/asio.hpp>
#include <PMNetworking/Server.h>

int main(int, char**){
    PM::TCPServer server(6969);
    server.run();
    
    return 0;
}






















/*  try {
        io_context ioContext;

        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 6969));

        while(true) {
            cout << "Accepting connections on port 6969." << endl;

            tcp::socket socket(ioContext);
            acceptor.accept(socket);

            cout << "client connected! Sending message." << endl;
            error_code errorCode;

            write(socket, buffer("Hello client. Change to something.\n"), errorCode);
        }
    } catch (std::exception& e){
        cerr << e.what() << endl;
    }*/