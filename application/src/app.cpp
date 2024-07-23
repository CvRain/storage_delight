#include <iostream>
#include <exception>
#include "hello.hpp"

int main() {
    try {
        storage_delight::core::Hello::say_hello().wait();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}