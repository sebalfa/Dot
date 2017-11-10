//
//  Client.cpp
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

#include <stdio.h>
#include "../include/Client.hpp"
#include "../include/Cross.hpp"

std::string CLIENT::Sign(std::string tx)
{
    //Read private key --------
    CryptoPP::StringSource ss( private_key, true, new CryptoPP::Base64Decoder( ) );
    ss.TransferTo(bytes);
    bytes.MessageEnd();
    CryptoPP::RSA::PrivateKey privateKey;
    privateKey.Load(bytes);
    CryptoPP::RSASSA_PKCS1v15_SHA_Signer privkey(privateKey);
    //-------------/

    
    //Sign message -----------
    CryptoPP::SecByteBlock sbbSignature(privkey.SignatureLength());
    privkey.SignMessage(rng, (byte const*) tx.data(), tx.size(), sbbSignature);
    //------------/
    

    //Prepare string for node ------
    std::string dd, ddd;
    
    CryptoPP::StringSink sink( dd );
    sink.Put((byte const*) tx.data(), tx.size());
    CryptoPP::StringSink sinksig( ddd );
    
    sinksig.Put(sbbSignature, sbbSignature.size());
    //-------------/
    
    //Return signature: ----
    return string_to_hex(ddd);
}

int CLIENT::parseKeyPair(std::string keyPair)
{
    //Read file: ------
    std::ifstream file(keyPair);
    std::string keys;
    std::string line;
    //------------/
    
    //Parse keys: ------------
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        if( private_key == "" ){
            private_key = line;
        }else{
            public_key = line;
        }
        
    }
    // ------------/
    
    return 0;
}

int CLIENT::Transfer(std::string c, std::string keyPair)
{
    parseKeyPair(keyPair);
    sign = Sign(c);
    c = c + public_key +"\n"+ sign;
    
    std::cout << "\nConnecting to node.." << std::endl;
    try
    {
        //Connect to Node: --------
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("10.0.2.26", "9393");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);
        //Connected to Node ---------------/
        
        //Prepare and send message -------
        std::string message = c;
        std::copy(message.begin(), message.end(), bufa.begin());
        
        boost::asio::write(socket, boost::asio::buffer(message), error);
        //-----------
        
        //Wait for response -----------
        while(true)
        {
           
            len = socket.read_some(boost::asio::buffer(buf), error);
            
            if (error == boost::asio::error::eof)
            {
                
                break;
            }
            else if (error)
            {
                throw boost::system::system_error(error); // Some other error.
            }
            if( std::strlen(buf.data()) > 0 ){
                std::cout << buf.data() << "\nDisconnecting from node." << std::endl;
                break;
            }
        }
        //----------------/
        
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}
