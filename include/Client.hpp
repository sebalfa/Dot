//
//  Client.hpp
//  Dots
//
//  Created by sebalfa on 09/11/2017.
//
//  Copyright (c) 2017 sebalfa
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

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
