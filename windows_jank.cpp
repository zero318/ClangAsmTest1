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

BOOL PathMatchSpecImpl(LPCWSTR pszFile, LPCWSTR pszSpec, DWORD dwFlags) {
	LPCWSTR spec_str = pszSpec;
	if (spec_str != NULL) {
		LPCWSTR file_str = pszFile;
		if (file_str != NULL) {
			if (dwFlags & PMSF_DONT_STRIP_SPACES) {
				while (*spec_str == L' ') ++spec_str;
			}
			WCHAR file_char = *file_str;
			WCHAR spec_char;
			if (file_char != L'\0') {
				do {
					spec_char = *spec_str;
					if (spec_char ==  L'\0') {
						break;
					}
					if (spec_char == L'*') {
						++spec_str;
						if (*spec_str == L'\0') {
							return TRUE;
						}
						if (*spec_str != L'.') {
							if (*file_str != L'\0') {
								for (;;) {
									if (PathMatchSpecImpl(file_str, spec_str, dwFlags)) {
										return TRUE;
									}
									++file_str;
									if (*file_str == L'\0') {
										return FALSE;
									}
								}
							}
							return FALSE;
						}
						++spec_str;
						if (spec_str[0] == L'*' && spec_str[1] == L'\0') {
							return TRUE;
						}
						WCHAR temp = *file_str;
						if (temp == L'\0') {
							return FALSE;
						}
						do {
							++file_str;
							if (temp == L'.') {
								if (PathMatchSpecImpl(file_str, spec_str, dwFlags)) {
									return TRUE;
								}
							}
							temp = *file_str;
						} while (temp != L'\0');
						return FALSE;
					}
					if (spec_char != L'?') {
						if (CharUpperW(*spec_str) != CharUpperW(file_char)) {
							return FALSE;
						}
					}
					++file_str;
					file_char = *file_str;
					++spec_str;
				} while (file_char != L'\0');
				if (*file_str != L'\0') {
					return FALSE;
				}
			}
			spec_char = spec_str[0];
			if (spec_char == L'\0') {
				return TRUE;
			}
			if (spec_char != L'*' && spec_char != L'?') {
				return FALSE;
			}
			spec_char = spec_str[1];
			if (spec_char == L'\0') {
				return TRUE;
			}
			if (spec_char != L'.') {
				return FALSE;
			}
			spec_char = spec_str[2];
			if (spec_char != L'*' && spec_char != L'?') {
				return FALSE;
			}
			return spec_char == L'\0';
		}
	}
	return FALSE;
}

BOOL PathMatchSpecHelper(LPCWSTR pszFile, LPCWSTR pszSpec, DWORD dwFlags) {
	LPCWSTR spec_str_arg = pszSpec;
	BOOL ret = FALSE;
	if (
		spec_str_arg != NULL &&
		pszFile != NULL &&
		spec_str_arg[0] != L'\0'
	) {
		PWSTR spec_str = StrDupW(spec_str_arg);
		if (spec_str != NULL) {
			PWSTR spec_str_iter = spec_str;
			for (;;) {
				PWSTR spec_str_path = StrChrW(spec_str, L':');
				if (spec_str_path != NULL) {
					*spec_str_path = L'\0';
				}
				if (PathMatchSpecImpl(pszFile, spec_str_iter, dwFlags)) {
					ret = TRUE;
					break;
				}
				if (spec_str_path == NULL) {
					break;
				}
				spec_str_iter = spec_str_path + 1;
			}
		}
		LocalFree(spec_str);
	}
	return ret;
}

HRESULT PathMatchSpecExW(LPCWSTR pszFile, LPCWSTR pszSpec, DWORD dwFlags) {
	if (dwFlags & PMSF_MULTIPLE) {
		return !!PathMatchSpecHelper(pszFile, pszSpec, dwFlags);
	} else {
		// some other bs
	}
}

BOOL PathMatchSpecW(LPCWSTR pszFile, LPCWSTR pszSpec) {
	return PathMatchSpecHelper(pszFile, pszSpec, PMSF_NORMAL);
}