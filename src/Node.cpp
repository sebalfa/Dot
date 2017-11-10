//
//  Node.cpp
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
#include "../include/Node.hpp"
#include "../include/Cross.hpp"

int NODE::parseCmd()
{
    //Parse cmd
    std::stringstream iss(cmd);
    std::string sl;
    while(iss.good())
    {
        getline(iss,sl,'\n');
        v.push_back(sl);
    }
    
    //Check query
    if( v[0] == "t" ) //Transaction
    {
        std::cout << "Query: transaction request." << std::endl;
        
        if( Validate(v[0]+"\n"+v[1]+"\n"+v[2]+"\n", v[4], v[3]) == 1 ){
            response = "Signature validated.";
            return 0;
        }else{
            response = "Signature invalid.";
            return 1;
        }
        
    }else{
        response = cmd;
    }
    
    return 1;
}

int NODE::Listen()
{
    try
    {
        //Create acceptor:
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 9393));
        
        //Listen for incoming messages ----------
        while(true)
        {
            std::cout << "Listening.." << std::endl;
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            
            std::string s = socket.remote_endpoint().address().to_string(); //IP ADDRESS
            std::cout << "\nClient (" << s << ") connected." << std::endl;
            
            while(true){
                socket.read_some(boost::asio::buffer(buf), error);
                
                if (error == boost::asio::error::eof)
                {
                    std::cout << "Client (" << s << ") disconnected." << std::endl;
                    break;
                }
                
                cmd = buf.data();
                
                if( cmd != "" ){
                    parseCmd();
                    boost::asio::write(socket, boost::asio::buffer(response), error);
                    v.clear();
                    cmd = "";
                    buf.assign(0);
                }
                
            }
        }
        //--------------
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}
