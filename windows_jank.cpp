#include <windows.h>
#include <ntsecapi.h>
#include <iostream>
#include <Lmcons.h>

#define UNLEN       256

std::string GetFQName(std::string username) {
    char domainname[UNLEN + 1];
    DWORD hstSize = sizeof(domainname);
    GetComputerNameEx(ComputerNameDnsHostname, domainname, &hstSize);

    std::string FQName = domainname + std::string("\\") + username;
    return FQName;
}

void AddSeTcbPrivilege(const char* FQname) {
    bool bInstallPrivilegeSuccess = false;
    bool m_bInstallPrivileges = true;
    bool bMustReboot = false;

    if (m_bInstallPrivileges)
    {
        LSA_HANDLE hLsaHandle = 0;
        LSA_OBJECT_ATTRIBUTES ObjectAttributes;

        ZeroMemory(&ObjectAttributes, sizeof(ObjectAttributes));

        NTSTATUS result1 = LsaOpenPolicy(NULL, &ObjectAttributes, POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, &hLsaHandle);

        ULONG uWinerr1 = LsaNtStatusToWinError(result1);

        std::cout << "uWinerr1 " << uWinerr1;

        {
            SID Sid;
            TCHAR lpszDomainName[256];
            DWORD dwSizeOfSid, dwDomainNameLength = 256;
            SID_NAME_USE SidNameUse;

            dwSizeOfSid = 256;
            BOOL bSuccess = LookupAccountName(NULL, FQname, &Sid, &dwSizeOfSid, lpszDomainName, &dwDomainNameLength, &SidNameUse);

            if (bSuccess)
            {
                LSA_UNICODE_STRING strPrivilegeStrings[1];
                strPrivilegeStrings[0].Buffer = (const PWSTR)L"SeTcbPrivilege";
                strPrivilegeStrings[0].Length = wcslen(strPrivilegeStrings[0].Buffer) * sizeof(wchar_t);
                strPrivilegeStrings[0].MaximumLength = wcslen(strPrivilegeStrings[0].Buffer) * sizeof(wchar_t);

                NTSTATUS sResult = LsaAddAccountRights(hLsaHandle, &Sid, strPrivilegeStrings, 1);

                ULONG uWinerr = LsaNtStatusToWinError(sResult);

                bInstallPrivilegeSuccess = (sResult == ERROR_SUCCESS);
            } else
            {
                DWORD dwLastError = GetLastError();
            }
        }

        LsaClose(hLsaHandle);

        if (bInstallPrivilegeSuccess)
        {
            bMustReboot = true;
        }
    }
}