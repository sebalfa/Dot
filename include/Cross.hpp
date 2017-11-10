//
//  Cross.hpp
//  Dots
//
//  Created by sebalfa on 09/11/2017.
//  Copyright © 2017 sebalfa. All rights reserved.
//

#ifndef Cross_hpp
#define Cross_hpp

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>

int Validate(std::string txt, std::string sig, std::string pubkey);
std::string hex_to_string(const std::string& input);
std::string string_to_hex(const std::string& input);
void clearTerminal();

#endif /* Cross_hpp */
