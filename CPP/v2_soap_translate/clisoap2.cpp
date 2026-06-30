#include <iostream>
#include <string>
#include <regex>
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

std::string httpRequest(const std::wstring& host, const std::wstring& path, 
                         const std::string& body, bool isPost) {
    HINTERNET hSession = WinHttpOpen(L"Client/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, isPost ? L"POST" : L"GET",
        path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    if (isPost) {
        WinHttpAddRequestHeaders(hRequest, L"Content-Type: text/xml; charset=utf-8", -1, WINHTTP_ADDREQ_FLAG_ADD);
        WinHttpAddRequestHeaders(hRequest, L"SOAPAction: \"\"", -1, WINHTTP_ADDREQ_FLAG_ADD);
    }

    WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        body.empty() ? NULL : (LPVOID)body.c_str(),
        body.size(), body.size(), 0);
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
    return response;
}

std::string urlEncode(const std::string& str) {
    std::string encoded;
    for (char c : str) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded += c;
        } else if (c == ' ') {
            encoded += '+';
        } else {
            char hex[4];
            sprintf(hex, "%%%02X", (unsigned char)c);
            encoded += hex;
        }
    }
    return encoded;
}

int main(int argc, char* argv[]) {
    std::string number = argc > 1 ? argv[1] : "10";
    std::wstring wnumber(number.begin(), number.end());

    // --- SOAP ---
    std::string soapBody = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
        "<soap:Body><NumberToWords xmlns=\"http://www.dataaccess.com/webservicesserver/\">"
        "<ubiNum>" + number + "</ubiNum>"
        "</NumberToWords></soap:Body></soap:Envelope>";

    std::string soapResponse = httpRequest(
        L"www.dataaccess.com",
        L"/webservicesserver/NumberConversion.wso",
        soapBody, true);

    std::regex re("<m:NumberToWordsResult>(.*?)</m:NumberToWordsResult>");
    std::smatch match;
    std::string english = "";
    if (std::regex_search(soapResponse, match, re)) english = match[1];

    // --- Traducción MyMemory ---
    std::string encoded = urlEncode(english);
    std::wstring transPath = L"/get?q=" + std::wstring(encoded.begin(), encoded.end()) + L"&langpair=en|es";
    std::string transResponse = httpRequest(L"api.mymemory.translated.net", transPath, "", false);

    std::regex reSpanish("\"translatedText\":\"(.*?)\"");
    std::smatch matchSpanish;
    std::string spanish = english;
    if (std::regex_search(transResponse, matchSpanish, reSpanish)) spanish = matchSpanish[1];

    std::cout << "Numero: " << number << std::endl;
    std::cout << "En ingles: " << english << std::endl;
    std::cout << "En espanol: " << spanish << std::endl;

    return 0;
}