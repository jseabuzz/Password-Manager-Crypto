#include <iostream>
#include <boost/asio.hpp>
#include <array>

using boost::asio::io_context;
using boost::asio::ip::tcp;
using boost::asio::connect;
using boost::system::error_code;
using std::array;
using boost::asio::buffer;
using boost::asio::error::eof;
using std::endl;
using std::cout;
using std::cerr;

int main(int argc, char* argv[]){
    try{
        io_context ioContext;

        tcp::resolver resolver { ioContext };
        auto endpoints = resolver.resolve("localhost", "6969");

        tcp::socket socket {ioContext};
        connect(socket, endpoints);

        while(true){
            array<char, 128> buf {};
            error_code error;

            size_t len = socket.read_some(buffer(buf), error);

            if (error == eof){
                break;
            } else if (error){
                throw boost::system::system_error(error);
            }

            cout.write(buf.data(), len);
        }

    } catch (std::exception& e){
        cerr << e.what() << endl;
    }



    return 0;
}
