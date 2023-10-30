#pragma once
#include <iostream>

#ifdef RF_DEBUG
#define ASSERT(cond, ...) { if(!(cond)) { std::cout<<"Assertion Failed: "<< __VA_ARGS__ << std::endl; __debugbreak(); } }
#endif // RF_DEBUG

