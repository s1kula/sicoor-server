#include <pqxx/pqxx>
#include <string>

class database {
    private:
    const std::string setting = "host=localhost port=5432 dbname=sicoor user=postgres password=pass";
    pqxx::connection connect;

    public:
    database();
    pqxx::result select();
    int insert(std::string message);
};          