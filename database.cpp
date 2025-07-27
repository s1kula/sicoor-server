#include "database.hpp"

database::database() : connect(setting){};

pqxx::result database::select(){
    try{
        pqxx::work transaction(connect);

        pqxx::result result = transaction.exec("SELECT message FROM messages");

        transaction.commit();

        return result;
    } catch(...){
        return pqxx::result();
    }
}

int database::insert(std::string message){
    try{
        pqxx::work transaction(connect);
    
        transaction.exec_params("INSERT INTO messages (message) VALUES ($1);", message);

        transaction.commit();

        return 0;
    } catch(...){
        return 1;
    }
};
