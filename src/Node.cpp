//
//  Node.cpp
//  Dots
//
//  Created by sebalfa on 09/11/2017.
//  Copyright © 2017 sebalfa. All rights reserved.
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
