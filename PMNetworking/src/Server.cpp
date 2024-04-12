#include <PMNetworking/Server.h>
#include <iostream>

namespace PM{
    TCPServer::TCPServer(int port): _port(port), _acceptor(_ioContext, tcp::endpoint(tcp::v4(), _port)) {

    }

    int TCPServer::run(){
        try{
            startAccept();
            _ioContext.run();
        }catch(std::exception& e){
            std::cerr << e.what() << std::endl;
            return -1;
        }
        return 0;
    }

    void TCPServer::startAccept(){
        _socket.emplace(_ioContext);

        _acceptor.async_accept(*_socket, [this](const error_code& error){
            auto conn = TCPConnection::create(std::move(*_socket));

            _connections.insert(conn);

            if (!error){
                conn->start();
            }

            startAccept();
        });
    }



    TCPConnection::TCPConnection(tcp::socket&& socket) : _socket(std::move(socket)){
        error_code ec;
        std::stringstream name;
        name << _socket.remote_endpoint();

        _name = name.str();
    }
    
    void TCPConnection::start(){
        asyncRead();
    }

    void TCPConnection::asyncRead(){
        boost::asio::async_read_until(_socket, _streamBuff, "\n", [self=shared_from_this(), this](error_code ec, size_t bLen){            
            self->onRead(ec, bLen);
        });
    }
    void TCPConnection::onRead(error_code ec, size_t bLen){
        if (ec){
            _socket.close();
            return;
        }

        std::stringstream msg;
        msg << _name << ": " << std::istream(&_streamBuff).rdbuf();

        cout << msg.str();
        asyncRead();
    };

    void TCPConnection::send(const std::string& msg){
        bool queueIdle = _outgoingMsgs.empty();
        _outgoingMsgs.push(msg);

        if (queueIdle){
            asyncWrite();
        }
    }
    
    void TCPConnection::asyncWrite(){
        boost::asio::async_write(_socket, boost::asio::buffer(_outgoingMsgs.front()), [self=shared_from_this(), this](error_code ec, size_t bLen){            
            self->onWrite(ec, bLen);
        });
    }

    void TCPConnection::onWrite(error_code ec, size_t bLen){
        if (ec){
            _socket.close();
            return;
        }

        _outgoingMsgs.pop();
        if (!_outgoingMsgs.empty()){
            asyncWrite();
        }
    }
}