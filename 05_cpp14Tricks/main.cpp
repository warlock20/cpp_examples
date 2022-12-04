#include <iostream>
#include <iomanip>

int main() {

    //Feature 1: std::quoted
    std::cout<<std::quoted("A quoted \"string\" using std::quoted")<<"\n";
    std::cout<<std::quoted("A quoted \"string\" using std::quoted but delimiter & ",{'&'})<<"\n";
    std::cout<<std::quoted("A quoted \"string\" using std::quoted but delimiter & and escape %",{'&'},{'%'})<<"\n";

    return 0;
}
