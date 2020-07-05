


#include <iostream>

#include <windows.h>

#include "./SignalCallback.h"

#pragma comment(lib, "user32.lib")



void test(void* pArg)
{
    std::cout << __FUNCTION__ << std::endl;
}



int main(int argc, char* argv[])
{
    SignalCallback Callback(test);
    
    bool Backup = false;
    while (!(GetAsyncKeyState(VK_ESCAPE) < 0)){
        bool Direct = (GetAsyncKeyState(VK_RCONTROL) < 0);
        bool Edge = (Direct ^ Backup) & Direct;
        Backup = Direct;
        if (Edge) Callback.Signal();
    }
    
    return 0;
}
