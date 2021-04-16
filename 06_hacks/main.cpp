#include <iostream>

using namespace std;
struct RunOnce {
    template <typename T>
    RunOnce(T &&f) { f(); }
};

int main() {
    /**
     * https://stackoverflow.com/questions/17701197/how-to-run-code-inside-a-loop-only-once-without-external-flag
     */
     uint32_t i = 10;
    while(i>0){
        static RunOnce a([]() { std::cout<<"ONE RUN\n";});
        static RunOnce b([]() { std::cout<<"ANOTHER ONE RUN\n";});
        for(static bool first = true;first;first=false)
        {
            std::cout<<"Yet ANOTHER ONE RUN\n";
        }
        std::cout<<i<<"\n";
        i--;
    }
    return 0;
}
