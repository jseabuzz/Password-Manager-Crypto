#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::system::error_code;
using std::cout;
using boost::asio::io_context;
using boost::asio::write;
using boost::asio::buffer;
using std::string;
using std::cerr;
using std::endl;

int main(int, char**){
    try {
        io_context ioContext;

        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 6969));

        while(true) {
            cout << "Accepting connections on port 6969." << endl;

            tcp::socket socket(ioContext);
            acceptor.accept(socket);

            cout << "client connected! Sending message." << endl;
            error_code errorCode;

            write(socket, buffer("Hello client.\n"), errorCode);
        }
    } catch (std::exception& e){
        cerr << e.what() << endl;
    }


    return 0;
}
