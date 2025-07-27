#include <iostream>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "database.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;

namespace asio = boost::asio;

class connection{
private:
    database db;
    boost::system::error_code error;
    asio::io_context io;
    const int port = 12345;
    tcp::endpoint endpoint;
    tcp::acceptor acceptor;
public:
    connection();
    int accept();
    int send(const std::string* data, tcp::socket* socket);
    int sendMessages(tcp::socket* socket);
};
