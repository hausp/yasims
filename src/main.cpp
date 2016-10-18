/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "core/Context.hpp"
#include <iostream>

int main(int argc, char** argv) {
    Context context;
    auto shit = context.run(argc, argv);
    std::cout << "my old friend" << std::endl;
    return shit;
}
