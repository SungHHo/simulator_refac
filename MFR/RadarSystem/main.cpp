#include "Mfr.hpp"
#include <thread>


int main() 
{
    Mfr mfr;
    
    mfr.startTcp();
    mfr.startUdp();

    while (true) 
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}