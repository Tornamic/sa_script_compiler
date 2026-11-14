#pragma once

#include <windows.h>

#define REGISTRY_STRING_SIZE 256  // buffer size ReadString expects

// Settings stored under HKEY_LOCAL_MACHINE\SOFTWARE\DMA Design Ltd\GTA3\SC ("path", "file", "target", "compileid").
//
// Every call site in sc.exe loads ecx with the address of g_Registry (0x2B226A8) first, so these are __thiscall
// members of a global object. The class has no data. Its (inline) constructor makes sure the key exists: the static
// initializer for g_Registry just calls CreateKey().
class CRegistry
{
public:
    CRegistry() { CreateKey(); }

    void ReadDWORD(LPCSTR lpValueName, DWORD* pdwData);         // 0x0041B710
    void ReadString(LPCSTR lpValueName, char* pszData);         // 0x0041B7B0, buffer of REGISTRY_STRING_SIZE
    bool DoesValueExist(LPCSTR lpValueName);                    // 0x0041B850
    void WriteDWORD(LPCSTR lpValueName, DWORD dwData);          // 0x0041B900
    void WriteString(LPCSTR lpValueName, const char* pszData);  // 0x0041B980
    void CreateKey();                                           // 0x0041BA10
};

extern CRegistry g_Registry;  // 0x02B226A8
