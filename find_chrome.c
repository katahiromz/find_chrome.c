/* find_chrome by katahiromz --- Find the location of Google Chrome web browser
 * License: MIT
 */
#include <windows.h>
#include <stdio.h>

#ifndef _countof
    #define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif

BOOL get_chrome_path(HKEY hBaseKey, LPTSTR pszPath, DWORD cchPathMax)
{
    HKEY hKey;
    LONG error;
    DWORD cbPath;

    pszPath[0] = 0;
    error = RegOpenKeyEx(hBaseKey,
                         TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe"),
                         0, KEY_READ, &hKey);
    if (error != ERROR_SUCCESS)
        return FALSE;

    cbPath = cchPathMax * sizeof(TCHAR);
    error = RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)pszPath, &cbPath);
    pszPath[cchPathMax - 1] = 0;

    RegCloseKey(hKey);
    return (error == ERROR_SUCCESS) && (GetFileAttributes(pszPath) != 0xFFFFFFFF);
}

BOOL find_chrome(LPTSTR pszPath, DWORD cchPathMax)
{
    return get_chrome_path(HKEY_CURRENT_USER, pszPath, cchPathMax) ||
           get_chrome_path(HKEY_LOCAL_MACHINE, pszPath, cchPathMax);
}

int main(void)
{
    TCHAR szPath[MAX_PATH];
    BOOL found = find_chrome(szPath, _countof(szPath));
    if (found)
    {
#ifdef UNICODE
        printf("found: %ls\n", szPath);
#else
        printf("found: %s\n", szPath);
#endif
    }
    else
    {
        puts("not found");
    }

    return 0;
}
