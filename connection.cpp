#include "connection.hpp"

connection::connection() : 
    endpoint(tcp::v4() , port), 
    acceptor(io, endpoint)
    {
        if(acceptor.is_open()){
            std::cout << "сервер запущен" << std::endl;
        }
    }

int connection::accept(){
    try{
        tcp::socket socket(io);

        if(acceptor.is_open()){

            acceptor.accept(socket);
                    
            uint32_t lenReadBuffer;
            asio::read(socket, asio::buffer(&lenReadBuffer, 4));

            std::string readBuffer; 
            readBuffer.resize(lenReadBuffer);
            asio::read(socket, asio::buffer(readBuffer));

            json query = json::parse(readBuffer);

            if(query["code"] == 1001){

                std::string reply;
                json jsonReply;

                if (db.insert(query["message"]) == 0){
                    jsonReply["code"] = 200; 
                    reply = jsonReply.dump(-1);
                } else {
                    jsonReply["code"] = 500; 
                    reply = jsonReply.dump(-1);
                }
                    send(&reply, &socket);

            } else if(query["code"] == 1002){

                sendMessages(&socket);}
        }
        socket.close();
    }catch(...){
        return 1;
    }

    return 0;
}

int connection::send(const std::string* data, tcp::socket* socket){

    uint32_t lenWriteBuffer = data->size();

    asio::write(*socket, asio::buffer(&lenWriteBuffer, 4));
    asio::write(*socket, asio::buffer(*data));

    uint32_t lenReadBuffer;
    asio::read(*socket, asio::buffer(&lenReadBuffer, 4));

    std::string readBuffer; 
    readBuffer.resize(lenReadBuffer);
    asio::read(*socket, asio::buffer(readBuffer));

    socket->close();

    json reply = json::parse(readBuffer);

    return reply["code"];
} 

int connection::sendMessages(tcp::socket* socket){
    std::string reply;

    json jsonReply = json::array();

    for(pqxx::row row : db.select()){
        jsonReply.push_back({{"message", row["message"].as<std::string>()}});
    };

    reply = jsonReply.dump(-1);

    send(&reply, socket);

    return 0;
}