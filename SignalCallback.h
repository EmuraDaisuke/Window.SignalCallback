#pragma once



#include <cstddef>
#include <array>

#include <windows.h>



class SignalCallback {
    public:
        using Func = void(*)(void*);
    
    
    
    public:
        ~SignalCallback() noexcept
        {
            if (mhShutdown) SetEvent(mhShutdown);
            if (mhThread) WaitForSingleObject(mhThread, INFINITE);
            
            if (mhThread) CloseHandle(mhThread);
            if (mhShutdown) CloseHandle(mhShutdown);
            if (mhWait) CloseHandle(mhWait);
        }
        
        
        
        SignalCallback()
        :mpFunc(nullptr)
        ,mpArg(nullptr)
        ,mhWait(NULL)
        ,mhShutdown(NULL)
        ,mhThread(NULL)
        {
        }
        
        
        
        SignalCallback(Func pFunc, void* pArg = nullptr)
        :mpFunc(pFunc)
        ,mpArg(pArg)
        ,mhWait(CreateEvent(NULL, FALSE, FALSE, NULL))
        ,mhShutdown(CreateEvent(NULL, FALSE, FALSE, NULL))
        ,mhThread(CreateThread(NULL, 0, Thread, this, 0, NULL))
        {
        }
        
        
        
        SignalCallback(SignalCallback&& v)
        :mpFunc(v.mpFunc)
        ,mpArg(v.mpArg)
        ,mhWait(v.mhWait)
        ,mhShutdown(v.mhShutdown)
        ,mhThread(v.mhThread)
        {
            v.mpFunc = nullptr;
            v.mpArg = nullptr;
            v.mhWait = NULL;
            v.mhShutdown = NULL;
            v.mhThread = NULL;
        }
        
        
        
        SignalCallback& operator =(SignalCallback&& v)
        {
            new(this) SignalCallback(std::move(v)); return *this;
        }
        
        
        
        void Signal() const noexcept
        {
            SetEvent(mhWait);
        }
    
    
    
    public:
        SignalCallback(const SignalCallback&)               = delete;
        
        SignalCallback& operator =(const SignalCallback&)   = delete;
    
    
    
    private:
        DWORD Thread() const noexcept
        {
            std::array<HANDLE, 2> aWait = {mhShutdown, mhWait};
            while (WaitForMultipleObjects(aWait.size(), aWait.data(), FALSE, INFINITE) != WAIT_OBJECT_0) (*mpFunc)(mpArg);
            return 0;
        }
        
        
        
        static DWORD Thread(void* pVoid) noexcept
        {
            auto pThis = reinterpret_cast<SignalCallback*>(pVoid);
            return pThis->Thread();
        }
    
    
    
    private:
        Func mpFunc;
        void* mpArg;
        
        HANDLE mhWait;
        HANDLE mhShutdown;
        HANDLE mhThread;
};
