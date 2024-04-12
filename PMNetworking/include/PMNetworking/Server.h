#pragma once
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <unordered_set>
#include <boost/asio.hpp>

namespace PM{
    using boost::asio::ip::tcp;
    using boost::system::error_code;
    using std::cout;
    using std::endl;

    class TCPConnection : public std::enable_shared_from_this<TCPConnection>{
        public:
            using TCPPointer = std::shared_ptr<TCPConnection>;

            static TCPPointer create(tcp::socket&& socket){
                return TCPPointer(new TCPConnection(std::move(socket)));
            }
            
            tcp::socket& socket(){
                return _socket;
            }
            void start();
            void send(const std::string& msg);
        private:
            tcp::socket _socket;
            std::string _name; 
            std::queue<std::string> _outgoingMsgs;
            boost::asio::streambuf _streamBuff {65536};

            explicit TCPConnection(tcp::socket&& socket);
            void asyncRead();
            void onRead(error_code ec, size_t bLen);
            void asyncWrite();
            void onWrite(error_code ec, size_t bLen);
    };

    class TCPServer{
        public:
            TCPServer(int port);
            int run();
        private:
            int _port;
            boost::asio::io_context _ioContext;
            tcp::acceptor _acceptor; 
            std::optional<tcp::socket> _socket;
            std::unordered_set<TCPConnection::TCPPointer> _connections {};

            void startAccept();
    };

}