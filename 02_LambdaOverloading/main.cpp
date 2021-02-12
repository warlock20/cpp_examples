/**
 * REFERENCES:
 * https://www.youtube.com/watch?v=1gNzhE-Tn40&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=50
 *
 */

#include <iostream>
#include <utility>


template<typename... B>
struct lambdaOverload : B ...
{
    template<typename ... T>
    lambdaOverload(T&&... t) : B(std::forward<T>(t))...
    {

    }
    using B::operator() ...;
};

template<typename ...T>
lambdaOverload(T ...)-> lambdaOverload<std::decay_t<T> ...>;

int main() {
    auto sumIntegers = [](uint32_t x, uint32_t y){return x+y;};
    auto sumFloat = [](float one){return one*0.2;};

    lambdaOverload l(sumIntegers,sumFloat);
    return l(10);
}
