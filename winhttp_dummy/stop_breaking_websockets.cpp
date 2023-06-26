#define POINTER_64 __ptr64

#include <stdlib.h>
#include <stdint.h>

#include <Windows.h>
#include <winternl.h>
#include <Rpc.h>
#include <winhttp.h>

#define stdcall __stdcall
#define musttail [[clang::musttail]]

#define jump_to_mem_addr(var) __asm__("jmp* %0"::"m"(var)); __builtin_unreachable()

#if __INTELLISENSE__
#define constinit constexpr
#endif

#define unknown_t uintptr_t

#define countof(array_type) \
(sizeof(array_type) / sizeof(array_type[0]))

#define _MACRO_CAT(arg1, arg2) arg1 ## arg2
#define MACRO_CAT(arg1, arg2) _MACRO_CAT(arg1, arg2)
#define MACRO_CAT2(arg1, arg2, ...) _MACRO_CAT(arg1, arg2) __VA_ARGS__
#define MACRO_CATW_RAW(arg1, arg2, arg3) arg1 ## arg2 ## arg3
#define MACRO_CATW(arg1, arg2, arg3) MACRO_CATW_RAW(arg1, arg2, arg3)
#define _MACRO_STR(arg) #arg
#define MACRO_STR(arg) _MACRO_STR(arg)
#define MACRO_EVAL(...) __VA_ARGS__

#define MACRO_SECOND(arg1, arg2, ...) arg2
#define MACRO_SECOND_EVAL(...) MACRO_EVAL(MACRO_SECOND(__VA_ARGS__))

#define MACROV_0(val) ,val

#define MACRO_EMPTY
#define _EMPTY(...) _EMPTY __VA_ARGS__
#define __MACRO_DEPAREN(arg1, ...) arg1##__VA_ARGS__
#define _MACRO_DEPAREN(...) __MACRO_DEPAREN(MACRO,__VA_ARGS__)
#define MACRO_DEPAREN(...) MACRO_EVAL(_MACRO_DEPAREN(_EMPTY __VA_ARGS__))

#define MACRO_IF(cond, ...) MACRO_DEPAREN(MACRO_SECOND(MACROV_##cond(),(__VA_ARGS__)))

typedef NTSTATUS(WINAPI *LPSTART_RPC_SERVER) (RPC_WSTR IfName, RPC_IF_HANDLE IfSpec);
typedef NTSTATUS(WINAPI *LPSTOP_RPC_SERVER) (RPC_IF_HANDLE IfSpec);
typedef NTSTATUS(WINAPI *LPSTOP_RPC_SERVER_EX) (RPC_IF_HANDLE IfSpec);

typedef VOID(WINAPI *LPNET_BIOS_OPEN) (VOID);
typedef VOID(WINAPI *LPNET_BIOS_CLOSE) (VOID);
typedef DWORD(WINAPI *LPNET_BIOS_RESET) (UCHAR LanaNum);

typedef DWORD(WINAPI *LPREGISTER_STOP_CALLBACK) (HANDLE* phNewWaitObject, PCWSTR pszServiceName, HANDLE hObject, WAITORTIMERCALLBACK Callback, PVOID Context, DWORD dwFlags);

typedef struct _SVCHOST_GLOBAL_DATA {
    PSID NullSid;                               // S-1-0-0
    PSID WorldSid;                              // S-1-1-0
    PSID LocalSid;                              // S-1-2-0
    PSID NetworkSid;                            // S-1-5-2
    PSID LocalSystemSid;                        // S-1-5-18
    PSID LocalServiceSid;                       // S-1-5-19
    PSID NetworkServiceSid;                     // S-1-5-20
    PSID BuiltinDomainSid;                      // S-1-5-32
    PSID AuthenticatedUserSid;                  // S-1-5-11
    PSID AnonymousLogonSid;                     // S-1-5-7
    PSID AliasAdminsSid;                        // S-1-5-32-544
    PSID AliasUsersSid;                         // S-1-5-32-545
    PSID AliasGuestsSid;                        // S-1-5-32-546
    PSID AliasPowerUsersSid;                    // S-1-5-32-547
    PSID AliasAccountOpsSid;                    // S-1-5-32-548
    PSID AliasSystemOpsSid;                     // S-1-5-32-549
    PSID AliasPrintOpsSid;                      // S-1-5-32-550
    PSID AliasBackupOpsSid;                     // S-1-5-32-551
    LPSTART_RPC_SERVER StartRpcServer;
    LPSTOP_RPC_SERVER StopRpcServer;
    LPSTOP_RPC_SERVER_EX StopRpcServerEx;
    LPNET_BIOS_OPEN NetBiosOpen;
    LPNET_BIOS_CLOSE NetBiosClose;
    LPNET_BIOS_RESET NetBiosReset;
#if (_WIN32_WINNT == _WIN32_WINNT_WINXP && NTDDI_VERSION >= NTDDI_WINXPSP2) \
        || (_WIN32_WINNT == _WIN32_WINNT_WS03 && NTDDI_VERSION >= NTDDI_WS03SP1) \
        || (_WIN32_WINNT >= _WIN32_WINNT_LONGHORN)
    LPREGISTER_STOP_CALLBACK RegisterStopCallback;
#endif
} SVCHOST_GLOBAL_DATA;

#ifndef __x86_64__
static void WINAPI HaltAndCatchFire1V(uintptr_t) {
    __asm__("int3");
}
static uintptr_t WINAPI HaltAndCatchFire0Z() {
    __asm__("int3");
    return FALSE;
}
static uintptr_t WINAPI HaltAndCatchFire1Z(uintptr_t) {
    __asm__("int3");
    return FALSE;
}
static uintptr_t WINAPI HaltAndCatchFire2Z(uintptr_t, uintptr_t) {
    __asm__("int3");
    return FALSE;
}
static uintptr_t WINAPI HaltAndCatchFire3Z(uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return FALSE;
}
static uintptr_t WINAPI HaltAndCatchFire4Z(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return FALSE;
}
static uintptr_t WINAPI HaltAndCatchFire5Z(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return FALSE;
}
static uintptr_t WINAPI HaltAndCatchFire6Z(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return FALSE;
}
static uintptr_t WINAPI HaltAndCatchFire7Z(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return FALSE;
}
static uintptr_t WINAPI HaltAndCatchFire0O() {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire1O(uintptr_t) {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire2O(uintptr_t, uintptr_t) {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire3O(uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire4O(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire5O(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire6O(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire7O(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire8O(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
static uintptr_t WINAPI HaltAndCatchFire9O(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
#else
static void WINAPI HaltAndCatchFireV() {
    __asm__("int3");
}
static uintptr_t WINAPI HaltAndCatchFireZ() {
    __asm__("int3");
    return FALSE;
}
static uintptr_t WINAPI HaltAndCatchFireO() {
    __asm__("int3");
    return ERROR_INVALID_FUNCTION;
}
#define HaltAndCatchFire1V HaltAndCatchFireV
#define HaltAndCatchFire0Z HaltAndCatchFireZ
#define HaltAndCatchFire1Z HaltAndCatchFireZ
#define HaltAndCatchFire2Z HaltAndCatchFireZ
#define HaltAndCatchFire3Z HaltAndCatchFireZ
#define HaltAndCatchFire4Z HaltAndCatchFireZ
#define HaltAndCatchFire5Z HaltAndCatchFireZ
#define HaltAndCatchFire6Z HaltAndCatchFireZ
#define HaltAndCatchFire7Z HaltAndCatchFireZ
#define HaltAndCatchFire0O HaltAndCatchFireO
#define HaltAndCatchFire1O HaltAndCatchFireO
#define HaltAndCatchFire2O HaltAndCatchFireO
#define HaltAndCatchFire3O HaltAndCatchFireO
#define HaltAndCatchFire4O HaltAndCatchFireO
#define HaltAndCatchFire5O HaltAndCatchFireO
#define HaltAndCatchFire6O HaltAndCatchFireO
#define HaltAndCatchFire7O HaltAndCatchFireO
#define HaltAndCatchFire8O HaltAndCatchFireO
#define HaltAndCatchFire9O HaltAndCatchFireO
#endif

#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...)

#define GenerateDelayLoadData \
DelayLoadData(WinHttpPacJsWorkerMain,                2Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2), arg1, arg2) \
DelayLoadData(DllCanUnloadNow,                       0O, 1, 1, 1, HRESULT WINAPI                , (void)) \
DelayLoadData(DllGetClassObject,                     3O, 1, 1, 1, HRESULT WINAPI                , (REFCLSID rclsid, REFIID riid, LPVOID* ppv), rclsid, riid, ppv) \
DelayLoadData(WinHttpIsHostInProxyBypassList,        5O, 1, 1, 1, DWORD WINAPI                  , (const WINHTTP_PROXY_INFO* pProxyInfo, PCWSTR pwszHost, INTERNET_SCHEME tScheme, INTERNET_PORT nPort, BOOL* pfIsInBypassList), pProxyInfo, pwszHost, tScheme, nPort, pfIsInBypassList) \
DelayLoadData(SvchostPushServiceGlobals,             1V, 1, 1, 1, VOID WINAPI                   , (SVCHOST_GLOBAL_DATA* lpGlobalData), lpGlobalData) \
DelayLoadData(WinHttpAddRequestHeaders,              4Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers), hRequest, lpszHeaders, dwHeadersLength, dwModifiers) \
DelayLoadData(WinHttpAddRequestHeadersEx,            8O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hRequest, DWORD dwModifiers, ULONGLONG ullFlags, ULONGLONG ullExtra, DWORD cHeaders, WINHTTP_EXTENDED_HEADER* pHeaders), hRequest, dwModifiers, ullFlags, ullExtra, cHeaders, pHeaders) \
DelayLoadData(WinHttpAutoProxySvcMain,               2Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2), arg1, arg2) \
DelayLoadData(WinHttpCheckPlatform,                  0Z, 1, 1, 1, BOOL WINAPI                   , (void)) \
DelayLoadData(WinHttpCloseHandle,                    1Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hInternet), hInternet) \
DelayLoadData(WinHttpConnect,                        4Z, 1, 1, 1, HINTERNET WINAPI              , (HINTERNET hSession, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD dwReserved), hSession, pswzServerName, nServerPort, dwReserved) \
DelayLoadData(WinHttpConnectionDeletePolicyEntries,  2Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2), arg1, arg2) \
DelayLoadData(WinHttpConnectionDeleteProxyInfo,      2Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2), arg1, arg2) \
DelayLoadData(WinHttpConnectionFreeNameList,         1Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1), arg1) \
DelayLoadData(WinHttpConnectionFreeProxyInfo,        1Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1), arg1) \
DelayLoadData(WinHttpConnectionFreeProxyList,        1Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1), arg1) \
DelayLoadData(WinHttpConnectionGetNameList,          1Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1), arg1) \
DelayLoadData(WinHttpConnectionGetProxyInfo,         3Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2, unknown_t arg3), arg1, arg2, arg3) \
DelayLoadData(WinHttpConnectionGetProxyList,         2Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2), arg1, arg2) \
DelayLoadData(WinHttpConnectionSetPolicyEntries,     3Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2, unknown_t arg3), arg1, arg2, arg3) \
DelayLoadData(WinHttpConnectionSetProxyInfo,         3Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2, unknown_t arg3), arg1, arg2, arg3) \
DelayLoadData(WinHttpConnectionUpdateIfIndexTable,   2Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2), arg1, arg2) \
DelayLoadData(WinHttpCrackUrl,                       4Z, 1, 1, 1, BOOL WINAPI                   , (LPCWSTR pwszUrl, DWORD dwUrlLength, DWORD dwFlags, LPURL_COMPONENTS lpUrlComponents), pwszUrl, dwUrlLength, dwFlags, lpUrlComponents) \
DelayLoadData(WinHttpCreateProxyResolver,            2O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hSession, HINTERNET* phResolver), hSession, phResolver) \
DelayLoadData(WinHttpCreateUrl,                      4Z, 1, 1, 1, BOOL WINAPI                   , (LPURL_COMPONENTS lpUrlComponents, DWORD dwFlags, LPWSTR pwszUrl, LPDWORD pdwUrlLength), lpUrlComponents, dwFlags, pwszUrl, pdwUrlLength) \
DelayLoadData(WinHttpDetectAutoProxyConfigUrl,       2Z, 1, 1, 1, BOOL WINAPI                   , (DWORD dwAutoDetectFlags, LPWSTR* ppwstrAutoConfigUrl), dwAutoDetectFlags, ppwstrAutoConfigUrl) \
DelayLoadData(WinHttpFreeProxyResult,                1V, 1, 1, 1, VOID WINAPI                   , (WINHTTP_PROXY_RESULT* pProxyResult), pProxyResult) \
DelayLoadData(WinHttpFreeProxyResultEx,              1V, 1, 1, 1, VOID WINAPI                   , (WINHTTP_PROXY_RESULT_EX* pProxyResultEx), pProxyResultEx) \
DelayLoadData(WinHttpFreeProxySettings,              1V, 1, 1, 1, VOID WINAPI                   , (WINHTTP_PROXY_SETTINGS* pWinHttpProxySettings), pWinHttpProxySettings) \
DelayLoadData(WinHttpGetDefaultProxyConfiguration,   1Z, 1, 1, 1, BOOL WINAPI                   , (WINHTTP_PROXY_INFO* pProxyInfo), pProxyInfo) \
DelayLoadData(WinHttpGetIEProxyConfigForCurrentUser, 1Z, 1, 1, 1, BOOL WINAPI                   , (WINHTTP_CURRENT_USER_IE_PROXY_CONFIG* pProxyConfig), pProxyConfig) \
DelayLoadData(WinHttpGetProxyForUrl,                 4Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hSession, LPCWSTR lpcwszUrl, WINHTTP_AUTOPROXY_OPTIONS* pAutoProxyOptions, WINHTTP_PROXY_INFO* pProxyInfo), hSession, lpcwszUrl, pAutoProxyOptions, pProxyInfo) \
DelayLoadData(WinHttpGetProxyForUrlEx2,              6O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hResolver, PCWSTR pcwszUrl, WINHTTP_AUTOPROXY_OPTIONS* pAutoProxyOptions, DWORD cbInterfaceSelectionContext, BYTE* pInterfaceSelectionContext, DWORD_PTR pContext), hResolver, pcwszUrl, pAutoProxyOptions, cbInterfaceSelectionContext, pInterfaceSelectionContext, pContext) \
DelayLoadData(WinHttpGetProxyForUrlEx,               4O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hResolver, PCWSTR pcwszUrl, WINHTTP_AUTOPROXY_OPTIONS* pAutoProxyOptions, DWORD_PTR pContext), hResolver, pcwszUrl, pAutoProxyOptions, pContext) \
DelayLoadData(WinHttpGetProxyForUrlHvsi,             9O, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4, unknown_t arg5, unknown_t arg6, unknown_t arg7, unknown_t arg8, unknown_t arg9), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
DelayLoadData(WinHttpGetProxyResult,                 2O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hResolver, WINHTTP_PROXY_RESULT* pProxyResult), hResolver, pProxyResult) \
DelayLoadData(WinHttpGetProxyResultEx,               2O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hResolver, WINHTTP_PROXY_RESULT_EX* pProxyResultEx), hResolver, pProxyResultEx) \
DelayLoadData(WinHttpGetProxySettingsVersion,        2O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hSession, DWORD* pdwProxySettingsVersion), hSession, pdwProxySettingsVersion) \
DelayLoadData(WinHttpGetTunnelSocket,                4Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4), arg1, arg2, arg3, arg4) \
DelayLoadData(WinHttpOpen,                           5Z, 1, 1, 1, HINTERNET WINAPI              , (LPCWSTR pszAgentW, DWORD dwAccessType, LPCWSTR pszProxyW, LPCWSTR pszProxyBypassW, DWORD dwFlags), pszAgentW, dwAccessType, pszProxyW, pszProxyBypassW, dwFlags) \
DelayLoadData(WinHttpOpenRequest,                    7Z, 1, 1, 1, HINTERNET WINAPI              , (HINTERNET hConnect, LPCWSTR pwszVerb, LPCWSTR pwszObjectName, LPCWSTR pwszVersion, LPCWSTR pwszReferrer, LPCWSTR FAR* ppwszAcceptTypes, DWORD dwFlags), hConnect, pwszVerb, pwszObjectName, pwszVersion, pwszReferrer, ppwszAcceptTypes, dwFlags) \
DelayLoadData(WinHttpProbeConnectivity,              6Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4, unknown_t arg5, unknown_t arg6), arg1, arg2, arg3, arg4, arg5, arg6) \
DelayLoadData(WinHttpQueryAuthSchemes,               4Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hRequest, LPDWORD lpdwSupportedSchemes, LPDWORD lpdwFirstScheme, LPDWORD pdwAuthTarget), hRequest, lpdwSupportedSchemes, lpdwFirstScheme, pdwAuthTarget) \
DelayLoadData(WinHttpQueryDataAvailable,             2Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hRequest, LPDWORD lpdwNumberOfBytesAvailable), hRequest, lpdwNumberOfBytesAvailable) \
DelayLoadData(WinHttpQueryHeaders,                   6Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hRequest, DWORD dwInfoLevel, LPCWSTR pwszName, LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex), hRequest, dwInfoLevel, pwszName, lpBuffer, lpdwBufferLength, lpdwIndex) \
DelayLoadData(WinHttpQueryOption,                    4Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, LPDWORD lpdwBufferLength), hInternet, dwOption, lpBuffer, lpdwBufferLength) \
DelayLoadData(WinHttpReadData,                       4Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hRequest, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead), hRequest, lpBuffer, dwNumberOfBytesToRead, lpdwNumberOfBytesRead) \
DelayLoadData(WinHttpReadProxySettings,              7O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hSession, PCWSTR pcwszConnectionName, BOOL fFallBackToDefaultSettings, BOOL fSetAutoDiscoverForDefaultSettings, DWORD* pdwSettingsVersion, BOOL* pfDefaultSettingsAreReturned, WINHTTP_PROXY_SETTINGS* pWinHttpProxySettings), hSession, pcwszConnectionName, fFallBackToDefaultSettings, fSetAutoDiscoverForDefaultSettings, pdwSettingsVersion, pfDefaultSettingsAreReturned, pWinHttpProxySettings) \
DelayLoadData(WinHttpReadProxySettingsHvsi,          8O, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4, unknown_t arg5, unknown_t arg6, unknown_t arg7, unknown_t arg8), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
DelayLoadData(WinHttpReceiveResponse,                2Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hRequest, LPVOID lpReserved), hRequest, lpReserved) \
DelayLoadData(WinHttpResetAutoProxy,                 2O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hSession, DWORD dwFlags), hSession, dwFlags) \
DelayLoadData(WinHttpSaveProxyCredentials,           4Z, 1, 1, 1, unknown_t WINAPI              , (unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4), arg1, arg2, arg3, arg4) \
DelayLoadData(WinHttpSendRequest,                    7Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext), hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength, dwTotalLength, dwContext) \
DelayLoadData(WinHttpSetCredentials,                 6Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hRequest, DWORD AuthTargets, DWORD AuthScheme, LPCWSTR pwszUserName, LPCWSTR pwszPassword, LPVOID pAuthParams), hRequest, AuthTargets, AuthScheme, pwszUserName, pwszPassword, pAuthParams) \
DelayLoadData(WinHttpSetDefaultProxyConfiguration,   1Z, 1, 1, 1, BOOL WINAPI                   , (WINHTTP_PROXY_INFO* pProxyInfo), pProxyInfo) \
DelayLoadData(WinHttpSetOption,                      4Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength), hInternet, dwOption, lpBuffer, dwBufferLength) \
DelayLoadData(WinHttpSetProxySettingsPerUser,        1O, 1, 1, 1, DWORD WINAPI                  , (BOOL fProxySettingsPerUser), fProxySettingsPerUser) \
DelayLoadData(WinHttpSetStatusCallback,              4Z, 1, 1, 1, WINHTTP_STATUS_CALLBACK WINAPI, (HINTERNET hInternet, WINHTTP_STATUS_CALLBACK lpfnInternetCallback, DWORD dwNotificationFlags, DWORD_PTR dwReserved), hInternet, lpfnInternetCallback, dwNotificationFlags, dwReserved) \
DelayLoadData(WinHttpSetTimeouts,                    5Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hInternet, int nResolveTimeout, int nConnectTimeout, int nSendTimeout, int nReceiveTimeout), hInternet, nResolveTimeout, nConnectTimeout, nSendTimeout, nReceiveTimeout) \
DelayLoadData(WinHttpTimeFromSystemTime,             2Z, 1, 1, 1, BOOL WINAPI                   , (CONST SYSTEMTIME* pst, LPWSTR pwszTime), pst, pwszTime) \
DelayLoadData(WinHttpTimeToSystemTime,               2Z, 1, 1, 1, BOOL WINAPI                   , (LPCWSTR pwszTime, SYSTEMTIME* pst), pwszTime, pst) \
DelayLoadData(WinHttpWebSocketClose,                 4O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hWebSocket, USHORT usStatus, PVOID pvReason, DWORD dwReasonLength), hWebSocket, usStatus, pvReason, dwReasonLength) \
DelayLoadData(WinHttpWebSocketCompleteUpgrade,       2Z, 1, 1, 1, HINTERNET WINAPI              , (HINTERNET hRequest, DWORD_PTR pContext), hRequest, pContext) \
DelayLoadData(WinHttpWebSocketQueryCloseStatus,      5O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hWebSocket, USHORT* pusStatus, PVOID pvReason, DWORD dwReasonLength, DWORD* pdwReasonLengthConsumed), hWebSocket, pusStatus, pvReason, dwReasonLength, pdwReasonLengthConsumed) \
DelayLoadData(WinHttpWebSocketReceive,               5O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hWebSocket, PVOID pvBuffer, DWORD dwBufferLength, DWORD* pdwBytesRead, WINHTTP_WEB_SOCKET_BUFFER_TYPE* peBufferType), hWebSocket, pvBuffer, dwBufferLength, pdwBytesRead, peBufferType) \
DelayLoadData(WinHttpWebSocketSend,                  4O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hWebSocket, WINHTTP_WEB_SOCKET_BUFFER_TYPE eBufferType, PVOID pvBuffer, DWORD dwBufferLength), hWebSocket, eBufferType, pvBuffer, dwBufferLength) \
DelayLoadData(WinHttpWebSocketShutdown,              4O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hWebSocket, USHORT usStatus, PVOID pvReason, DWORD dwReasonLength), hWebSocket, usStatus, pvReason, dwReasonLength) \
DelayLoadData(WinHttpWriteData,                      4Z, 1, 1, 1, BOOL WINAPI                   , (HINTERNET hRequest, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten), hRequest, lpBuffer, dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten) \
DelayLoadData(WinHttpWriteProxySettings,             3O, 1, 1, 1, DWORD WINAPI                  , (HINTERNET hSession, BOOL fForceUpdate, WINHTTP_PROXY_SETTINGS* pWinHttpProxySettings), hSession, fForceUpdate, pWinHttpProxySettings)

//static unknown_t WINAPI WinHttpPacJsWorkerMain(unknown_t arg1, unknown_t arg2);
//static HRESULT WINAPI DllCanUnloadNow();
//static HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
//static DWORD WINAPI WinHttpIsHostInProxyBypassList(const WINHTTP_PROXY_INFO* pProxyInfo, PCWSTR pwszHost, INTERNET_SCHEME tScheme, INTERNET_PORT nPort, BOOL* pfIsInBypassList); // Private1
//static VOID WINAPI SvchostPushServiceGlobals(SVCHOST_GLOBAL_DATA* lpGlobalData);
//static BOOL WINAPI WinHttpAddRequestHeaders(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwModifiers);
//static DWORD WINAPI WinHttpAddRequestHeadersEx(HINTERNET hRequest, DWORD dwModifiers, ULONGLONG ullFlags, ULONGLONG ullExtra, DWORD cHeaders, WINHTTP_EXTENDED_HEADER* pHeaders);
//static unknown_t WINAPI WinHttpAutoProxySvcMain(unknown_t arg1, unknown_t arg2);
//static BOOL WINAPI WinHttpCheckPlatform(void);
//static BOOL WINAPI WinHttpCloseHandle(HINTERNET hInternet);
//static HINTERNET WINAPI WinHttpConnect(HINTERNET hSession, LPCWSTR pswzServerName, INTERNET_PORT nServerPort, DWORD dwReserved);
//static unknown_t WINAPI WinHttpConnectionDeletePolicyEntries(unknown_t arg1, unknown_t arg2);
//static unknown_t WINAPI WinHttpConnectionDeleteProxyInfo(unknown_t arg1, unknown_t arg2);
//static unknown_t WINAPI WinHttpConnectionFreeNameList(unknown_t arg1);
//static unknown_t WINAPI WinHttpConnectionFreeProxyInfo(unknown_t arg1);
//static unknown_t WINAPI WinHttpConnectionFreeProxyList(unknown_t arg1);
//static unknown_t WINAPI WinHttpConnectionGetNameList(unknown_t arg1);
//static unknown_t WINAPI WinHttpConnectionGetProxyInfo(unknown_t arg1, unknown_t arg2, unknown_t arg3);
//static unknown_t WINAPI WinHttpConnectionGetProxyList(unknown_t arg1, unknown_t arg2);
//static unknown_t WINAPI WinHttpConnectionSetPolicyEntries(unknown_t arg1, unknown_t arg2, unknown_t arg3);
//static unknown_t WINAPI WinHttpConnectionSetProxyInfo(unknown_t arg1, unknown_t arg2, unknown_t arg3);
//static unknown_t WINAPI WinHttpConnectionUpdateIfIndexTable(unknown_t arg1, unknown_t arg2);
//static BOOL WINAPI WinHttpCrackUrl(LPCWSTR pwszUrl, DWORD dwUrlLength, DWORD dwFlags, LPURL_COMPONENTS lpUrlComponents);
//static DWORD WINAPI WinHttpCreateProxyResolver(HINTERNET hSession, HINTERNET* phResolver);
//static BOOL WINAPI WinHttpCreateUrl(LPURL_COMPONENTS lpUrlComponents, DWORD dwFlags, LPWSTR pwszUrl, LPDWORD pdwUrlLength);
//static BOOL WINAPI WinHttpDetectAutoProxyConfigUrl(DWORD dwAutoDetectFlags, LPWSTR* ppwstrAutoConfigUrl);
//static VOID WINAPI WinHttpFreeProxyResult(WINHTTP_PROXY_RESULT* pProxyResult);
//static VOID WINAPI WinHttpFreeProxyResultEx(WINHTTP_PROXY_RESULT_EX* pProxyResultEx);
//static VOID WINAPI WinHttpFreeProxySettings(WINHTTP_PROXY_SETTINGS* pWinHttpProxySettings);
//static BOOL WINAPI WinHttpGetDefaultProxyConfiguration(WINHTTP_PROXY_INFO* pProxyInfo);
//static BOOL WINAPI WinHttpGetIEProxyConfigForCurrentUser(WINHTTP_CURRENT_USER_IE_PROXY_CONFIG* pProxyConfig);
//static BOOL WINAPI WinHttpGetProxyForUrl(HINTERNET hSession, LPCWSTR lpcwszUrl, WINHTTP_AUTOPROXY_OPTIONS* pAutoProxyOptions, WINHTTP_PROXY_INFO* pProxyInfo);
//static DWORD WINAPI WinHttpGetProxyForUrlEx2(HINTERNET hResolver, PCWSTR pcwszUrl, WINHTTP_AUTOPROXY_OPTIONS* pAutoProxyOptions, DWORD cbInterfaceSelectionContext, BYTE* pInterfaceSelectionContext, DWORD_PTR pContext);
//static DWORD WINAPI WinHttpGetProxyForUrlEx(HINTERNET hResolver, PCWSTR pcwszUrl, WINHTTP_AUTOPROXY_OPTIONS* pAutoProxyOptions, DWORD_PTR pContext);
//static unknown_t WINAPI WinHttpGetProxyForUrlHvsi(unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4, unknown_t arg5, unknown_t arg6, unknown_t arg7, unknown_t arg8, unknown_t arg9);
//static DWORD WINAPI WinHttpGetProxyResult(HINTERNET hResolver, WINHTTP_PROXY_RESULT* pProxyResult);
//static DWORD WINAPI WinHttpGetProxyResultEx(HINTERNET hResolver, WINHTTP_PROXY_RESULT_EX* pProxyResultEx);
//static DWORD WINAPI WinHttpGetProxySettingsVersion(HINTERNET hSession, DWORD* pdwProxySettingsVersion);
//static unknown_t WINAPI WinHttpGetTunnelSocket(unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4);
//static HINTERNET WINAPI WinHttpOpen(LPCWSTR pszAgentW, DWORD dwAccessType, LPCWSTR pszProxyW, LPCWSTR pszProxyBypassW, DWORD dwFlags);
//static HINTERNET WINAPI WinHttpOpenRequest(HINTERNET hConnect, LPCWSTR pwszVerb, LPCWSTR pwszObjectName, LPCWSTR pwszVersion, LPCWSTR pwszReferrer, LPCWSTR FAR* ppwszAcceptTypes, DWORD dwFlags);
//static unknown_t WINAPI WinHttpProbeConnectivity(unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4, unknown_t arg5, unknown_t arg6);
//static BOOL WINAPI WinHttpQueryAuthSchemes(HINTERNET hRequest, LPDWORD lpdwSupportedSchemes, LPDWORD lpdwFirstScheme, LPDWORD pdwAuthTarget);
//static BOOL WINAPI WinHttpQueryDataAvailable(HINTERNET hRequest, LPDWORD lpdwNumberOfBytesAvailable);
//static BOOL WINAPI WinHttpQueryHeaders(HINTERNET hRequest, DWORD dwInfoLevel, LPCWSTR pwszName, LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);
//static BOOL WINAPI WinHttpQueryOption(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, LPDWORD lpdwBufferLength);
//static BOOL WINAPI WinHttpReadData(HINTERNET hRequest, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead);
//static DWORD WINAPI WinHttpReadProxySettings(HINTERNET hSession, PCWSTR pcwszConnectionName, BOOL fFallBackToDefaultSettings, BOOL fSetAutoDiscoverForDefaultSettings, DWORD* pdwSettingsVersion, BOOL* pfDefaultSettingsAreReturned, WINHTTP_PROXY_SETTINGS* pWinHttpProxySettings);
//static unknown_t WINAPI WinHttpReadProxySettingsHvsi(unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4, unknown_t arg5, unknown_t arg6, unknown_t arg7, unknown_t arg8);
//static BOOL WINAPI WinHttpReceiveResponse(HINTERNET hRequest, LPVOID lpReserved);
//static DWORD WINAPI WinHttpResetAutoProxy(HINTERNET hSession, DWORD dwFlags);
//static unknown_t WINAPI WinHttpSaveProxyCredentials(unknown_t arg1, unknown_t arg2, unknown_t arg3, unknown_t arg4);
//static BOOL WINAPI WinHttpSendRequest(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext);
//static BOOL WINAPI WinHttpSetCredentials(HINTERNET hRequest, DWORD AuthTargets, DWORD AuthScheme, LPCWSTR pwszUserName, LPCWSTR pwszPassword, LPVOID pAuthParams);
//static BOOL WINAPI WinHttpSetDefaultProxyConfiguration(WINHTTP_PROXY_INFO* pProxyInfo);
//static BOOL WINAPI WinHttpSetOption(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength);
//static DWORD WINAPI WinHttpSetProxySettingsPerUser(BOOL fProxySettingsPerUser);
//static WINHTTP_STATUS_CALLBACK WINAPI WinHttpSetStatusCallback(HINTERNET hInternet, WINHTTP_STATUS_CALLBACK lpfnInternetCallback, DWORD dwNotificationFlags, DWORD_PTR dwReserved);
//static BOOL WINAPI WinHttpSetTimeouts(HINTERNET hInternet, int nResolveTimeout, int nConnectTimeout, int nSendTimeout, int nReceiveTimeout);
//static BOOL WINAPI WinHttpTimeFromSystemTime(CONST SYSTEMTIME* pst, LPWSTR pwszTime);
//static BOOL WINAPI WinHttpTimeToSystemTime(LPCWSTR pwszTime, SYSTEMTIME* pst);
//static DWORD WINAPI WinHttpWebSocketClose(HINTERNET hWebSocket, USHORT usStatus, PVOID pvReason, DWORD dwReasonLength);
//static HINTERNET WINAPI WinHttpWebSocketCompleteUpgrade(HINTERNET hRequest, DWORD_PTR pContext);
//static DWORD WINAPI WinHttpWebSocketQueryCloseStatus(HINTERNET hWebSocket, USHORT* pusStatus, PVOID pvReason, DWORD dwReasonLength, DWORD* pdwReasonLengthConsumed);
//static DWORD WINAPI WinHttpWebSocketReceive(HINTERNET hWebSocket, PVOID pvBuffer, DWORD dwBufferLength, DWORD* pdwBytesRead, WINHTTP_WEB_SOCKET_BUFFER_TYPE* peBufferType);
//static DWORD WINAPI WinHttpWebSocketSend(HINTERNET hWebSocket, WINHTTP_WEB_SOCKET_BUFFER_TYPE eBufferType, PVOID pvBuffer, DWORD dwBufferLength);
//static DWORD WINAPI WinHttpWebSocketShutdown(HINTERNET hWebSocket, USHORT usStatus, PVOID pvReason, DWORD dwReasonLength);
//static BOOL WINAPI WinHttpWriteData(HINTERNET hRequest, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpdwNumberOfBytesWritten);
//static DWORD WINAPI WinHttpWriteProxySettings(HINTERNET hSession, BOOL fForceUpdate, WINHTTP_PROXY_SETTINGS* pWinHttpProxySettings);

#undef DelayLoadData
#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...) \


#undef DelayLoadData
#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...) \
MACRO_IF(auto_generate_shim, static ret_convention MACRO_CAT(func, Shim) declare_args;)

GenerateDelayLoadData

#undef DelayLoadData
#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...) \
MACRO_IF(auto_generate_ptr, typedef decltype(&MACRO_CAT(func, Shim)) MACRO_CAT(func, Ptr); static MACRO_CAT(func, Ptr) MACRO_CAT(func, Resolved) = &MACRO_CAT(func, Shim);)

GenerateDelayLoadData

#undef DelayLoadData
#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...) \
MACRO_IF(include_in_lookup, MACRO_IF(auto_generate_ptr, (const void**const)&MACRO_CAT(func, Resolved),))

static inline const void* *const func_ptrs[] = {
    GenerateDelayLoadData
};

#undef DelayLoadData
#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...) \
MACRO_IF(include_in_lookup, MACRO_STR(func),)

static inline constinit const char *const func_names[] = {
    GenerateDelayLoadData
};

#undef DelayLoadData
#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...) \
MACRO_IF(include_in_lookup, (void*)&MACRO_CAT(HaltAndCatchFire, halt_type),)

static inline constinit const void *const halt_funcs[] = {
    GenerateDelayLoadData
};

static inline constexpr const wchar_t *const lib_name = L"\\winhttp.dll";

static inline constexpr size_t lib_name_length = []() constexpr {
    size_t length = 0;
    while (lib_name[length]) ++length;
    return length;
}();

static wchar_t path_buffer[MAX_PATH + lib_name_length];
//static BOOL is_wow_64 = false;

static void resolve_funcs() {
    //IsWow64Process(GetCurrentProcess(), &is_wow_64);
    //size_t system_dir_size = (is_wow_64 ? GetSystemWow64DirectoryW : GetSystemDirectoryW)(path_buffer, countof(path_buffer));
    size_t system_dir_size = GetSystemDirectoryW(path_buffer, countof(path_buffer));
    if (system_dir_size <= MAX_PATH) {
        for (
            size_t i = 0;
            (path_buffer[system_dir_size + i] = lib_name[i]);
            ++i
        );
        if (HMODULE lib = LoadLibraryW(path_buffer)) {
            for (size_t i = 0; i < countof(func_names); ++i) {
                *func_ptrs[i] = (void*)GetProcAddress(lib, func_names[i]) ? : halt_funcs[i];
            }
            return;
        }
    }
    for (size_t i = 0; i < countof(func_names); ++i) {
        *func_ptrs[i] = halt_funcs[i];
    }
}

#undef DelayLoadData

#ifndef __x86_64__
#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...) \
MACRO_IF(auto_generate_shim, static ret_convention MACRO_CAT(func, Shim) declare_args { resolve_funcs(); jump_to_mem_addr(MACRO_CAT(func, Resolved)); })
#else
#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...) \
MACRO_IF(auto_generate_shim, static ret_convention MACRO_CAT(func, Shim) declare_args { resolve_funcs(); musttail return MACRO_CAT(func, Resolved)(__VA_ARGS__); })
#endif

GenerateDelayLoadData

#undef DelayLoadData
#define DelayLoadData(func, halt_type, auto_generate_shim, auto_generate_ptr, include_in_lookup, ret_convention, declare_args, ...) \
MACRO_IF(auto_generate_shim, ret_convention MACRO_CAT(func,Export) declare_args asm(MACRO_STR(MACRO_CAT(_,func))); ret_convention MACRO_CAT(func,Export) declare_args { jump_to_mem_addr(MACRO_CAT(func, Resolved)); })

extern "C" {

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    return TRUE;
}

GenerateDelayLoadData

}