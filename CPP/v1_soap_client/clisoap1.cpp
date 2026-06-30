#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winhttp.h>
#include <regex>

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "ws2_32.lib")

std::string wstrToStr(const std::wstring& wstr) {
    std::string str(wstr.begin(), wstr.end());
    return str;
}

int main(int argc, char* argv[]) {
    std::string number = argc > 1 ? argv[1] : "10";
    std::wstring wnumber(number.begin(), number.end());

    std::wstring soapBody = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        L"<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
        L"<soap:Body><NumberToWords xmlns=\"http://www.dataaccess.com/webservicesserver/\">"
        L"<ubiNum>" + wnumber + L"</ubiNum>"
        L"</NumberToWords></soap:Body></soap:Envelope>";

    HINTERNET hSession = WinHttpOpen(L"SOAP Client/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    HINTERNET hConnect = WinHttpConnect(hSession, L"www.dataaccess.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST",
        L"/webservicesserver/NumberConversion.wso",
        NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    WinHttpAddRequestHeaders(hRequest, L"Content-Type: text/xml; charset=utf-8", -1, WINHTTP_ADDREQ_FLAG_ADD);
    WinHttpAddRequestHeaders(hRequest, L"SOAPAction: \"\"", -1, WINHTTP_ADDREQ_FLAG_ADD);

    std::string body(soapBody.begin(), soapBody.end());
    WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        (LPVOID)body.c_str(), body.size(), body.size(), 0);
    WinHttpReceiveResponse(hRequest, NULL);

    std::string response;
    DWORD size = 0;
    do {
        WinHttpQueryDataAvailable(hRequest, &size);
        if (size == 0) break;
        char* buffer = new char[size + 1];
        DWORD downloaded = 0;
        WinHttpReadData(hRequest, buffer, size, &downloaded);
        buffer[downloaded] = 0;
        response += buffer;
        delete[] buffer;
    } while (size > 0);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    std::regex re("<m:NumberToWordsResult>(.*?)</m:NumberToWordsResult>");
    std::smatch match;
    if (std::regex_search(response, match, re)) {
        std::cout << "Resultado: " << match[1] << std::endl;
    } else {
        std::cout << "No se encontro resultado" << std::endl;
    }

    return 0;
}