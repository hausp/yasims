/* created by Vinicius Freitas<vinicius.mctf@grad.ufsc.br> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include "core/Context.hpp"
#include "core/signals.hpp"

int main(int argc, char** argv) {
    Context context;
    signals::set_context(context);
    return signals::run(argc, argv);
}
