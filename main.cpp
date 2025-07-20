#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "database.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;

namespace asio = boost::asio;

int addMessage(std::string* message, database* database){ 
    const u_int8_t PLACE_OCCUPIED_SEND = 6;
    return ( database->insert(message->substr(PLACE_OCCUPIED_SEND)) );
}

int sendReply(asio::io_context* io, tcp::socket* socket, bool pasteSuccess){

    std::string reply;
    
    if(pasteSuccess){
        reply = "Ошибка, сообщение не было вставлено";
    } else {
        reply = "сообщение было успешно доставлено";
    }
    uint32_t lenReply= reply.size();

    asio::write(*socket, asio::buffer(&lenReply, 4));
    asio::write(*socket, asio::buffer(reply));

    return 0;
}

int sendMessages(asio::io_context* io, tcp::socket* socket, database* database){
    std::string reply;

    json jsonReply = json::array();

    for(pqxx::row row : database->select()){
        jsonReply.push_back({{"message", row["message"].as<std::string>()}});
    };

    reply = jsonReply.dump(-1);

    uint32_t lenReply = reply.size();

    asio::write(*socket, asio::buffer(&lenReply, 4));
    asio::write(*socket, asio::buffer(reply));

    return 0;
}

int main(){
    database database;
    boost::system::error_code error;
    asio::io_context io;
    tcp::endpoint endpoint(tcp::v4() , 12345);
    tcp::acceptor acceptor(io, endpoint);

    if(acceptor.is_open()){
        std::cout << "сервер запущен" << std::endl;
    }

    while (1){
        try{
            tcp::socket socket(io);

            if(acceptor.is_open()){

                acceptor.accept(socket);
                
                uint32_t lenReadBuffer;
                asio::read(socket, asio::buffer(&lenReadBuffer, 4));

                std::string readBuffer; 
                readBuffer.resize(lenReadBuffer);
                asio::read(socket, asio::buffer(readBuffer));

                if(readBuffer.rfind("<SEND>", 0) == 0){

                    sendReply(&io, &socket, addMessage(&readBuffer, &database));

                } else if(readBuffer.rfind("<GET>", 0) == 0){

                    sendMessages(&io, &socket, &database);
                }

            }
            socket.close();
        }catch(...){}
    }
    
    return 0;

}