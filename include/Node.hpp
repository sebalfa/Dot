#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

class NODE {
    std::vector<std::string> v;
    boost::asio::io_service io_service;
    std::string cmd, response;
    boost::system::error_code error;
    boost::array<char, 8192> buf;
    int parseCmd();
   
public:
    int Listen();

};
