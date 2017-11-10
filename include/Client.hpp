#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <boost/algorithm/string/replace.hpp>

using boost::asio::ip::tcp;

class CLIENT {
    boost::array<char, 8192> buf, bufa;
    size_t len;
    boost::system::error_code error;
    boost::asio::io_service io_service;
    std::string sign, private_key, public_key;
    std::string Sign(std::string cmd);
    int parseKeyPair(std::string kp);
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::ByteQueue bytes;
public:
    int Transfer(std::string c, std::string prk);
};
