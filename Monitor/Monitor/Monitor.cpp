
#include "Monitor.h"
#include<iostream>  
#include<Windows.h>  
#include "atlconv.h"
#include <list>
#include <string>
#include <tchar.h>
#include "tlhelp32.h"

#define  UNICODE
using namespace std;


void    ForceRunProcess();
DWORD GetProcessID(LPCTSTR lpProcessName);

int main()
{

    while (true)
    {
        if (GetProcessID("FM_Server.exe") == 0)
        {
            cout << "Not found fmserver" << endl;
            ForceRunProcess();
        }

        Sleep(5000);
    }

    ForceRunProcess();
    return 0;
}



DWORD GetProcessID(LPCTSTR lpProcessName)
{
    std::list<std::string> processList;

    //创建进程快照
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe32 = { 0 };
        pe32.dwSize = sizeof(pe32);
        //拿到第一个进程信息
        if (!Process32First(hSnapshot, &pe32))
        {
            CloseHandle(hSnapshot);
            return FALSE;
        }
        do
        {
            int nLen = _tcslen(lpProcessName);
            int nLen2 = _tcslen(pe32.szExeFile);
            if (nLen != nLen2)
            {
                continue;
            }
            if (_tcscmp(lpProcessName, pe32.szExeFile) == 0)
            {
                return pe32.th32ProcessID;
            }
            processList.push_back(pe32.szExeFile);
        } while (Process32Next(hSnapshot, &pe32));
    }
    return 0;
}
void    ForceRunProcess()
{
    USES_CONVERSION;

    TCHAR szCurPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, szCurPath);

    BOOL bRet = TRUE;
    CHAR cPath[1024];
  //  sprintf(cPath, "%s\\..\\FM_Server\\FM_Server.exe", szCurPath);
    sprintf(cPath, "%s\\..\\FMServer\\FM_Server.exe", szCurPath);
    const WCHAR * cLineChar = A2W(A2T(cPath));

    cout << cPath << endl;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    //创建一个新进程  
    if (CreateProcess(
        NULL,   //  指向一个NULL结尾的、用来指定可执行模块的宽字节字符串  
        cPath,  // 命令行字符串  
        NULL, //    指向一个SECURITY_ATTRIBUTES结构体，这个结构体决定是否返回的句柄可以被子进程继承。  
        NULL, //    如果lpProcessAttributes参数为空（NULL），那么句柄不能被继承。<同上>  
        false,//    指示新进程是否从调用进程处继承了句柄。   
        0,  //  指定附加的、用来控制优先类和进程的创建的标  
        //  CREATE_NEW_CONSOLE  新控制台打开子进程  
        //  CREATE_SUSPENDED    子进程创建后挂起，直到调用ResumeThread函数  
        NULL, //    指向一个新进程的环境块。如果此参数为空，新进程使用调用进程的环境  
        NULL, //    指定子进程的工作路径  
        &si, // 决定新进程的主窗体如何显示的STARTUPINFO结构体  
        &pi  // 接收新进程的识别信息的PROCESS_INFORMATION结构体  
        ))
    {
        cout << "start FM_Server success." << endl;

        //下面两行关闭句柄，解除本进程和新进程的关系，不然有可能不小心调用TerminateProcess函数关掉子进程  
        //      CloseHandle(pi.hProcess);  
        //      CloseHandle(pi.hThread);  
        //终止子进程  
        //   TerminateProcess(pi.hProcess, 300);

        //终止本进程，状态码  
        //   ExitProcess(1001);
    }
    else{
        cerr << "start FM_Server fail." << endl;
    }

    Sleep(100);

}