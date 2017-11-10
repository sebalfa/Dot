//
//  main.cpp
//  Dots
//
//  Created by sebalfa on 09/11/2017.
//  Copyright © 2017 sebalfa. All rights reserved.
//

#include <iostream>
#include "include/Node.hpp"
#include "include/Client.hpp"
#include "include/Cross.hpp"

std::string const copy = "Copyright (c) 2017 sebalfa\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
"of this software and associated documentation files (the \"Software\"), to deal\n"
"in the Software without restriction, including without limitation the rights\n"
"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
"copies of the Software, and to permit persons to whom the Software is\n"
"furnished to do so, subject to the following conditions:\n"

"\nThe above copyright notice and this permission notice shall be included in all\n"
"copies or substantial portions of the Software.\n"

"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
"SOFTWARE.\n";

void startScreen(std::string s = "") {
    clearTerminal();
    std::cout << "FINGERPRINT COMMAND LINE TOOL.\n" << copy << "\n" << s << std::endl;
    return;
}

int argHandler(int argc, const char * argv[]){
    
    if(argc < 2){
        //no arguments parsed, terminate.
        std::cout << "No valid arguments parsed. See manual (-h)." << std::endl;
        return 1;
    }
    
    std::string arg = argv[1];
    
    startScreen();
    
    //Check if client or node:
    if( arg == "-c" || arg == "-client" || arg == "-clientmode" ){
        //initiate client mode.
        CLIENT c;
        std::cout << "Client mode initiated.\nCommand:" << std::endl;
        std::string cmd;
        
        //Client command handling: --------
        while(std::cin >> cmd){
            std::cin.ignore();
            
            if( cmd == "t" || cmd == "transfer" || cmd == "send"){ //Transfer funds
                startScreen("CLIENT MODE");
                
                std::string m;
                std::string msg = "t\n";
                std::cout << "TRANSFER\nDot ID: (-a = any and all):" << std::endl;
                std::getline(std::cin, m);
                
                msg += m + "\n";
                std::cout << "Amount:" << std::endl;
                std::getline(std::cin, m);
                msg += m + "\n";
                std::cout << "Key-pair (file/.w):" << std::endl;
                std::getline(std::cin, m);
                
                /* Before transfering, the client signs his/her command.
                 * The signature and creation of public key is done client-side.
                 * Private key is not sent to node.
                 * [public_key][cmd][signature]
                 */
                
                c.Transfer(msg, m);
                
                break;
            }else if( cmd == "b" || cmd == "balance" || cmd == "bal" ){
                //Check balance
            }else if( cmd == "x" || cmd == "exit" ){
                std::cout << "Terminating session.." << std::endl;
                return 0;
            }else if( cmd == "h" || cmd == "help" || cmd == "manual" ){
                //Manual
                std::cout << "Manual here.." << std::endl;
            }else{
                //Invalid command
                std::cout << "Invalid command parsed.\n\nCommand: " << std::endl;
            }
        }
       //--------------
        
        
        
        
        return 0;
    }else if( arg == "-n" || arg == "-node" || arg == "-nodemode" ){
        //initiate node mode.
        NODE a;
        std::cout << "Node mode initiated.\n" << std::endl;
        a.Listen();
        
        return 0;
    }else if (arg == "-h" || arg == "-help" || arg == "-manual" ){
        //show manual
        std::cout << "Manual here.." << std::endl;
        return 0;
    }else{
        //invalid arguments, terminate.
        std::cout << "Invalid arguments parsed. See manual (-h)." << std::endl;
        return 1;
    }
    
    return 0;
}

int main(int argc, const char * argv[]) {
    
    argHandler(argc, argv);
    
    return 0;
}
