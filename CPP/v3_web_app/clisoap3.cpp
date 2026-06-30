#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

std::vector<std::string> UNIDADES = {"cero","uno","dos","tres","cuatro","cinco","seis",
    "siete","ocho","nueve","diez","once","doce","trece","catorce","quince",
    "dieciseis","diecisiete","dieciocho","diecinueve"};
std::vector<std::string> DECENAS = {"","","veinte","treinta","cuarenta","cincuenta",
    "sesenta","setenta","ochenta","noventa"};
std::vector<std::string> CENTENAS = {"","ciento","doscientos","trescientos","cuatrocientos",
    "quinientos","seiscientos","setecientos","ochocientos","novecientos"};

std::string numeroALetras(long long n) {
    if (n < 20)  return UNIDADES[n];
    if (n < 30)  return n == 20 ? "veinte" : "veinti" + UNIDADES[n-20];
    if (n < 100) {
        std::string d = DECENAS[n/10];
        return n%10 == 0 ? d : d + " y " + UNIDADES[n%10];
    }
    if (n < 1000) {
        if (n == 100) return "cien";
        std::string c = CENTENAS[n/100];
        long long r = n%100;
        return r == 0 ? c : c + " " + numeroALetras(r);
    }
    if (n < 1000000) {
        long long miles = n/1000, r = n%1000;
        std::string pre = miles == 1 ? "mil" : numeroALetras(miles) + " mil";
        return r == 0 ? pre : pre + " " + numeroALetras(r);
    }
    return "fuera de rango";
}

std::string getParam(const std::string& query, const std::string& key) {
    std::string search = key + "=";
    size_t pos = query.find(search);
    if (pos == std::string::npos) return "10";
    pos += search.size();
    size_t end = query.find('&', pos);
    return query.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8084);
    addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 5);
    std::cout << "Servidor en http://localhost:8084" << std::endl;

    while (true) {
        SOCKET client = accept(server, NULL, NULL);
        char buf[4096] = {0};
        recv(client, buf, sizeof(buf)-1, 0);

        std::string request(buf);
        std::string query = "";
        size_t qpos = request.find('?');
        if (qpos != std::string::npos) {
            size_t qend = request.find(' ', qpos);
            query = request.substr(qpos+1, qend-qpos-1);
        }

        std::string n = getParam(query, "n");
        long long num = std::stoll(n);
        std::string resultado = numeroALetras(num);
        resultado[0] = toupper(resultado[0]);

        std::string html = "<!DOCTYPE html><html lang='es'>"
            "<head><meta charset='UTF-8'><title>Numero a Letras</title>"
            "<style>"
            "* { margin:0; padding:0; box-sizing:border-box; }"
            "body { font-family:'Segoe UI',sans-serif; background:linear-gradient(135deg,#1a1a2e,#16213e,#0f3460); min-height:100vh; display:flex; align-items:center; justify-content:center; }"
            ".card { background:white; border-radius:16px; padding:2.5rem 3rem; box-shadow:0 20px 60px rgba(0,0,0,0.4); text-align:center; max-width:500px; width:90%; }"
            ".titulo { font-size:1.5rem; font-weight:700; color:#0f3460; margin-bottom:1.5rem; }"
            ".numero { font-size:4rem; font-weight:800; color:#e94560; margin-bottom:0.5rem; }"
            ".etiqueta { font-size:0.85rem; color:#999; text-transform:uppercase; letter-spacing:1px; margin-bottom:1.5rem; }"
            ".resultado { background:#f0f4ff; border-left:4px solid #0f3460; border-radius:8px; padding:1rem 1.5rem; font-size:1.4rem; font-weight:600; color:#1a1a2e; }"
            ".pie { margin-top:1.5rem; font-size:0.8rem; color:#bbb; }"
            "</style></head><body>"
            "<div class='card'>"
            "<div class='titulo'>&#128290; Numero a Letras en Espanol</div>"
            "<div class='numero'>" + n + "</div>"
            "<div class='etiqueta'>Numero ingresado</div>"
            "<div class='resultado'>" + resultado + "</div>"
            "<div class='pie'>C++ &middot; Caso de Estudio SOAP &middot; UTP Puebla</div>"
            "</div></body></html>";

        std::string response = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "Content-Length: " + std::to_string(html.size()) + "\r\n"
            "Connection: close\r\n\r\n" + html;

        send(client, response.c_str(), response.size(), 0);
        closesocket(client);
    }

    WSACleanup();
    return 0;
}