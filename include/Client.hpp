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
#include "../include/Cross.hpp"

using boost::asio::ip::tcp;

class CLIENT {
    
    boost::system::error_code error;
    boost::asio::io_service io_service;
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::ByteQueue bytes;
    size_t len;
    std::string sign, private_key, public_key;
    CONSENT consensus;
    
    std::string Sign(std::string cmd);
    int parseKeyPair(std::string kp);
    
public:
    int Transfer(std::string c, std::string prk);
};
