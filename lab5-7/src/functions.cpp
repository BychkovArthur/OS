#include "functions.hpp"

std::string getAddres(size_t port) {
    return std::string("tcp://127.0.0.1:") + std::to_string(port);
}
