// registry.cpp: 0x41B710-0x41BA6E in sc.exe
//
// Settings of the compiler live in HKEY_LOCAL_MACHINE\SOFTWARE\DMA Design Ltd\GTA3\SC.

#include "common.h"
#include "registry.h"

#define SC_REGISTRY_KEY "SOFTWARE\\DMA Design Ltd\\GTA3\\SC"

// 0x02B226A8
CRegistry g_Registry;

// 0x0041B710
void CRegistry::ReadDWORD(LPCSTR lpValueName, DWORD* pdwData)
{
    HKEY hKey;
    DWORD dwDisposition;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        RegCreateKeyExA(
            HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, (LPSTR) "", 0, KEY_ALL_ACCESS, nullptr, &hKey, &dwDisposition);
        *pdwData = 0;
        return;
    }

    DWORD cbData = sizeof(DWORD);
    LSTATUS status = RegQueryValueExA(hKey, lpValueName, nullptr, nullptr, reinterpret_cast<LPBYTE>(pdwData), &cbData);
    RegCloseKey(hKey);
    if (status != ERROR_SUCCESS)
    {
        *pdwData = 0;
    }
}

// 0x0041B7B0
void CRegistry::ReadString(LPCSTR lpValueName, char* pszData)
{
    HKEY hKey;
    DWORD dwDisposition;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        RegCreateKeyExA(
            HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, (LPSTR) "", 0, KEY_ALL_ACCESS, nullptr, &hKey, &dwDisposition);
        *pszData = '\0';
        return;
    }

    DWORD cbData = REGISTRY_STRING_SIZE;
    LSTATUS status = RegQueryValueExA(hKey, lpValueName, nullptr, nullptr, reinterpret_cast<LPBYTE>(pszData), &cbData);
    RegCloseKey(hKey);
    if (status != ERROR_SUCCESS)
    {
        *pszData = '\0';
    }
}

// 0x0041B850
bool CRegistry::DoesValueExist(LPCSTR lpValueName)
{
    HKEY hKey;
    DWORD cbData;
    BYTE data[REGISTRY_STRING_SIZE];

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
    {
        cbData = sizeof(data);
        memset(data, 0, sizeof(data));
        LSTATUS status = RegQueryValueExA(hKey, lpValueName, nullptr, nullptr, data, &cbData);
        RegCloseKey(hKey);
        return status == ERROR_SUCCESS;
    }
    return false;
}

// 0x0041B900
void CRegistry::WriteDWORD(LPCSTR lpValueName, DWORD dwData)
{
    HKEY hKey;
    DWORD dwDisposition;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        RegCreateKeyExA(
            HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, (LPSTR) "", 0, KEY_ALL_ACCESS, nullptr, &hKey, &dwDisposition);
    }
    RegSetValueExA(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dwData), sizeof(DWORD));
    RegCloseKey(hKey);
}

// 0x0041B980
void CRegistry::WriteString(LPCSTR lpValueName, const char* pszData)
{
    HKEY hKey;
    DWORD dwDisposition;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        RegCreateKeyExA(
            HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, (LPSTR) "", 0, KEY_ALL_ACCESS, nullptr, &hKey, &dwDisposition);
    }
    RegSetValueExA(hKey, lpValueName, 0, REG_SZ, reinterpret_cast<const BYTE*>(pszData), strlen(pszData));
    RegCloseKey(hKey);
}

// 0x0041BA10
void CRegistry::CreateKey()
{
    HKEY hKey;
    DWORD dwDisposition;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        RegCreateKeyExA(
            HKEY_LOCAL_MACHINE, SC_REGISTRY_KEY, 0, (LPSTR) "", 0, KEY_ALL_ACCESS, nullptr, &hKey, &dwDisposition);
    }
    else
    {
        RegCloseKey(hKey);
    }
}
