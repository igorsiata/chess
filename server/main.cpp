#include "App.h"
#include <iostream>
#include <string>

struct PerSocketData {
    // You can store per-connection data here
    int playerId = 0;
};


int main() {
    uWS::App().ws<PerSocketData>("/*", {
        // When a new connection is opened
        .open = [](auto *ws) {
            std::cout << "Client connected!" << std::endl;
            ws->send("Welcome to the C++ Chess Server!");
        },

        // When a message is received
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            std::cout << "Received: " << message << std::endl;
            
            // Echo back the message
            ws->send(message, opCode);
        },

        // When a connection is closed
        .close = [](auto *ws, int code, std::string_view message) {
            std::cout << "Client disconnected" << std::endl;
        }
    }).listen(9001, [](auto *token) {
        if (token) {
            std::cout << "Server listening on port 9001" << std::endl;
        }
    }).run();
}
