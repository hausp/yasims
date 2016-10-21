
#include "random/Types.hpp"

std::random_device dist::Global::RD;
size_t dist::Global::SEED = dist::Global::RD();
