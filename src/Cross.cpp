//
//  Cross.cpp
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

#include "../include/Cross.hpp"

int Validate(std::string txt, std::string sig, std::string pubKey)
{
    sig = hex_to_string(sig);
    //Read public key
    CryptoPP::ByteQueue bytes;
    CryptoPP::StringSource ss( pubKey, true, new CryptoPP::Base64Decoder );
    ss.TransferTo(bytes);
    bytes.MessageEnd();
    CryptoPP::RSA::PublicKey publickey;
    publickey.Load(bytes);
    
    CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(publickey);
    
    //Read signed message
    std::string ddtx;
    CryptoPP::StringSink sink( ddtx );
    sink.Put((byte const*) txt.data(), txt.size());
    
    std::string combined(ddtx);
    combined.append(sig);
    
    //Verify signature
    try
    {
        CryptoPP::StringSource(combined, true,
                     new CryptoPP::SignatureVerificationFilter(
                                                     verifier, NULL,
                                                     CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION
                                                     )
                     );
        std::cout << "Signature validated." << std::endl;
        return 1;
    }
    catch(CryptoPP::SignatureVerificationFilter::SignatureVerificationFailed &err)
    {
        std::cout << "Signature invalid." << std::endl;
    }
    
    return 0;
}

#include <string>

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

std::string hex_to_string(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");
    
    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");
        
        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("not a hex digit");
        
        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

void clearTerminal()
{
    std::cout << std::string(100, '\n') << std::endl;
    return;
}
