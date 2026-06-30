import com.sun.net.httpserver.*;
import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

public class NumberToWordsServer {
    static String[] UNIDADES = {"cero","uno","dos","tres","cuatro","cinco","seis","siete",
        "ocho","nueve","diez","once","doce","trece","catorce","quince","dieciséis",
        "diecisiete","dieciocho","diecinueve"};
    static String[] DECENAS = {"","","veinte","treinta","cuarenta","cincuenta",
        "sesenta","setenta","ochenta","noventa"};
    static String[] CENTENAS = {"","ciento","doscientos","trescientos","cuatrocientos",
        "quinientos","seiscientos","setecientos","ochocientos","novecientos"};

    static String numeroALetras(long n) {
        if (n < 20)  return UNIDADES[(int)n];
        if (n < 30)  return n == 20 ? "veinte" : "veinti" + UNIDADES[(int)(n-20)];
        if (n < 100) return DECENAS[(int)(n/10)] + (n%10 != 0 ? " y " + UNIDADES[(int)(n%10)] : "");
        if (n < 1000) {
            if (n == 100) return "cien";
            return CENTENAS[(int)(n/100)] + (n%100 != 0 ? " " + numeroALetras(n%100) : "");
        }
        if (n < 1_000_000) {
            long miles = n/1000, r = n%1000;
            String pre = miles == 1 ? "mil" : numeroALetras(miles) + " mil";
            return r > 0 ? pre + " " + numeroALetras(r) : pre;
        }
        return "fuera de rango";
    }

    public static void main(String[] args) throws Exception {
        HttpServer server = HttpServer.create(new InetSocketAddress(8080), 0);
        server.createContext("/", exchange -> {
            String query = exchange.getRequestURI().getQuery();
            String n = "10";
            if (query != null && query.contains("n="))
                n = query.split("n=")[1].split("&")[0];

            String resultado = numeroALetras(Long.parseLong(n));
            String html = "<!DOCTYPE html><html lang='es'>" +
                "<head><meta charset='UTF-8'><title>Número a Letras</title>" +
                "<style>" +
                "* { margin:0; padding:0; box-sizing:border-box; }" +
                "body { font-family: 'Segoe UI', sans-serif; background: linear-gradient(135deg,#1a1a2e,#16213e,#0f3460); min-height:100vh; display:flex; align-items:center; justify-content:center; }" +
                ".card { background:white; border-radius:16px; padding:2.5rem 3rem; box-shadow:0 20px 60px rgba(0,0,0,0.4); text-align:center; max-width:500px; width:90%; }" +
                ".titulo { font-size:1.5rem; font-weight:700; color:#0f3460; margin-bottom:1.5rem; }" +
                ".numero { font-size:4rem; font-weight:800; color:#e94560; margin-bottom:0.5rem; }" +
                ".etiqueta { font-size:0.85rem; color:#999; text-transform:uppercase; letter-spacing:1px; margin-bottom:1.5rem; }" +
                ".resultado { background:#f0f4ff; border-left:4px solid #0f3460; border-radius:8px; padding:1rem 1.5rem; font-size:1.4rem; font-weight:600; color:#1a1a2e; text-transform:capitalize; }" +
                ".pie { margin-top:1.5rem; font-size:0.8rem; color:#bbb; }" +
                "</style></head><body>" +
                "<div class='card'>" +
                "<div class='titulo'>🔢 Número a Letras en Español</div>" +
                "<div class='numero'>" + n + "</div>" +
                "<div class='etiqueta'>Número ingresado</div>" +
                "<div class='resultado'>" + resultado + "</div>" +
                "<div class='pie'>Java · Caso de Estudio SOAP · UTP Puebla</div>" +
                "</div></body></html>";

            byte[] bytes = html.getBytes(StandardCharsets.UTF_8);
            exchange.getResponseHeaders().set("Content-Type", "text/html; charset=utf-8");
            exchange.sendResponseHeaders(200, bytes.length);
            try (OutputStream os = exchange.getResponseBody()) { os.write(bytes); }
        });
        server.start();
        System.out.println("Servidor en http://localhost:8080");
    }
}