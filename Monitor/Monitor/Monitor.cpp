
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

    //�������̿���
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe32 = { 0 };
        pe32.dwSize = sizeof(pe32);
        //�õ���һ��������Ϣ
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

    //����һ���½���  
    if (CreateProcess(
        NULL,   //  ָ��һ��NULL��β�ġ�����ָ����ִ��ģ��Ŀ��ֽ��ַ���  
        cPath,  // �������ַ���  
        NULL, //    ָ��һ��SECURITY_ATTRIBUTES�ṹ�壬����ṹ������Ƿ񷵻صľ�����Ա��ӽ��̼̳С�  
        NULL, //    ���lpProcessAttributes����Ϊ�գ�NULL������ô������ܱ��̳С�<ͬ��>  
        false,//    ָʾ�½����Ƿ�ӵ��ý��̴��̳��˾����   
        0,  //  ָ�����ӵġ���������������ͽ��̵Ĵ����ı�  
        //  CREATE_NEW_CONSOLE  �¿���̨���ӽ���  
        //  CREATE_SUSPENDED    �ӽ��̴��������ֱ������ResumeThread����  
        NULL, //    ָ��һ���½��̵Ļ����顣����˲���Ϊ�գ��½���ʹ�õ��ý��̵Ļ���  
        NULL, //    ָ���ӽ��̵Ĺ���·��  
        &si, // �����½��̵������������ʾ��STARTUPINFO�ṹ��  
        &pi  // �����½��̵�ʶ����Ϣ��PROCESS_INFORMATION�ṹ��  
        ))
    {
        cout << "start FM_Server success." << endl;

        //�������йرվ������������̺��½��̵Ĺ�ϵ����Ȼ�п��ܲ�С�ĵ���TerminateProcess�����ص��ӽ���  
        //      CloseHandle(pi.hProcess);  
        //      CloseHandle(pi.hThread);  
        //��ֹ�ӽ���  
        //   TerminateProcess(pi.hProcess, 300);

        //��ֹ�����̣�״̬��  
        //   ExitProcess(1001);
    }
    else{
        cerr << "start FM_Server fail." << endl;
    }

    Sleep(100);

}