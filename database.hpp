#include <pqxx/pqxx>
#include <string>

class database {
    private:
    pqxx::connection connect;
    const std::string setting = "host=localhost dbname=sicoor user=postgres password=pass";

    public:
    database();
    pqxx::result select();
    int insert(std::string message);
};          