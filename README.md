измените port на строке 16(connection.hpp), передайте ip и port вашим пользователям. 
создайте базу данных и таблицу messages (запрос ниже create table query).
измените строку "setting" в database.hpp на данные вашей бд.

change the port on line 16(connection.hpp), provide the ip and port to your users.
create a database and the messages table (query below create table query).
change the string "setting" in database.hpp to your database credentials.


create table query:
CREATE TABLE messages (
    id SERIAL PRIMARY KEY,
    message VARCHAR(1000) NOT NULL
);


сборка/build

g++ main.cpp database.cpp connection.cpp -o main -lpqxx -lboost_system


зависимости/dependencies

boost/asio              (boost.org)
libpqxx                 (github.com/jtv/libpqxx)
nlohmann-json           (github.com/nlohmann/json)