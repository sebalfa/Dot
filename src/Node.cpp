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

/*
 TODO: threading sv.t. sockets.
 Windows server + compiling
 Fingerprint
 Dot-block
 Rewards
 */


int NODE::Broadcast(std::string message) {
    
    int i = 0;
    while( i < 1 ) {
        std::cout << "\nBroadcasting to node.." << std::endl;
        try
        {
            //Connect to Node: --------
            tcp::resolver resolver(io_service);
            tcp::resolver::query query("10.0.2.15", "9393");
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
            
            tcp::socket socket(io_service);
            boost::asio::connect(socket, endpoint_iterator);
            //Connected to Node ---------------/
            
            //Prepare and send message -------
            boost::array<char, 8192> buf;
            std::copy(message.begin(), message.end(), buf.begin());
            
            boost::asio::write(socket, boost::asio::buffer(message), error);
            std::cout << "Message broadcasted." << std::endl;
            //-----------
            
            
            
            //Wait for response -----------
            socket.close();
            return 0;
            //----------------/
            
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        ++i;
    }
    return 0;
}


int NODE::parseCmd(std::string cmd)
{
    //Parse cmd
    std::stringstream iss(cmd);
    std::string sl;
    v.clear();
    while(iss.good())
    {
        getline(iss,sl,'\n');
        v.push_back(sl);
    }
    
    // Check query
    if( v[0] == "t" ) // tx broadcast from client
    {
        std::cout << "Query: transaction request." << std::endl;
        
        if( Validate(v[0]+"\n"+v[1]+"\n"+v[2]+"\n", v[4], v[3]) == 1 ){
            // Signature is valid. Now, broadcast this tx to the network, then add to Dot-block.
            
            return 0;
        }else{
            response = "Signature invalid.";
            return 1;
        }
        
    }else if( v[0] == "b" ){ // tx broadcast from node
        
        std::cout << "Query: node tx broadcast." << std::endl;
        
        if( Validate("t\n"+v[1]+"\n"+v[2]+"\n", v[4], v[3]) == 1 ){
            // Signature is valid. Now, broadcast this tx to the network, then add to Dot-block.
            
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
        boost::array<char, 8192> buf;
        std::string cmd;
        
        //Listen for incoming messages ----------
        while(true)
        {
            std::cout << "Listening.." << std::endl;
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            
            std::string s = socket.remote_endpoint().address().to_string(); //IP ADDRESS
            std::cout << "\nPeer (" << s << ") connected." << std::endl;
            
            while(true){
                socket.read_some(boost::asio::buffer(buf), error);
                
                if (error == boost::asio::error::eof)
                {
                    std::cout << "Peer (" << s << ") disconnected." << std::endl;
                    break;
                }
                
                cmd = buf.data();
                
                if( cmd != "" ){
                    int prsd = parseCmd(cmd);
                    
                    if( prsd == 0 ){
                        
                        response = "1";
                        boost::asio::write(socket, boost::asio::buffer(response), error);
                        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
                        //Broadcast("b\n"+v[1]+"\n"+v[2]+"\n"+v[3]+"\n"+v[4]);
                    }else{
                        boost::asio::write(socket, boost::asio::buffer(response), error);
                        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
                    }
                    
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
