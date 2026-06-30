using System;
using System.Net;
using System.Text;

string[] unidades = {"cero","uno","dos","tres","cuatro","cinco","seis","siete",
    "ocho","nueve","diez","once","doce","trece","catorce","quince","dieciséis",
    "diecisiete","dieciocho","diecinueve"};
string[] decenas = {"","","veinte","treinta","cuarenta","cincuenta",
    "sesenta","setenta","ochenta","noventa"};
string[] centenas = {"","ciento","doscientos","trescientos","cuatrocientos",
    "quinientos","seiscientos","setecientos","ochocientos","novecientos"};

string NumeroALetras(long n) {
    if (n < 20)  return unidades[n];
    if (n < 30)  return n == 20 ? "veinte" : "veinti" + unidades[n - 20];
    if (n < 100) return decenas[n/10] + (n%10 != 0 ? " y " + unidades[n%10] : "");
    if (n < 1000) {
        if (n == 100) return "cien";
        return centenas[n/100] + (n%100 != 0 ? " " + NumeroALetras(n%100) : "");
    }
    if (n < 1_000_000) {
        long miles = n/1000, r = n%1000;
        string pre = miles == 1 ? "mil" : NumeroALetras(miles) + " mil";
        return r > 0 ? pre + " " + NumeroALetras(r) : pre;
    }
    return "fuera de rango";
}

HttpListener listener = new HttpListener();
listener.Prefixes.Add("http://localhost:8081/");
listener.Start();
Console.WriteLine("Servidor en http://localhost:8081");

while (true) {
    var context = listener.GetContext();
    var request = context.Request;
    var responseObj = context.Response;

    string n = request.QueryString["n"] ?? "10";
    string resultado = NumeroALetras(long.Parse(n));
    resultado = char.ToUpper(resultado[0]) + resultado.Substring(1);

    string html = $@"<!DOCTYPE html>
<html lang='es'>
<head><meta charset='UTF-8'><title>Número a Letras</title>
<style>
* {{ margin:0; padding:0; box-sizing:border-box; }}
body {{ font-family:'Segoe UI',sans-serif; background:linear-gradient(135deg,#1a1a2e,#16213e,#0f3460); min-height:100vh; display:flex; align-items:center; justify-content:center; }}
.card {{ background:white; border-radius:16px; padding:2.5rem 3rem; box-shadow:0 20px 60px rgba(0,0,0,0.4); text-align:center; max-width:500px; width:90%; }}
.titulo {{ font-size:1.5rem; font-weight:700; color:#0f3460; margin-bottom:1.5rem; }}
.numero {{ font-size:4rem; font-weight:800; color:#e94560; margin-bottom:0.5rem; }}
.etiqueta {{ font-size:0.85rem; color:#999; text-transform:uppercase; letter-spacing:1px; margin-bottom:1.5rem; }}
.resultado {{ background:#f0f4ff; border-left:4px solid #0f3460; border-radius:8px; padding:1rem 1.5rem; font-size:1.4rem; font-weight:600; color:#1a1a2e; }}
.pie {{ margin-top:1.5rem; font-size:0.8rem; color:#bbb; }}
</style></head>
<body>
<div class='card'>
  <div class='titulo'>🔢 Número a Letras en Español</div>
  <div class='numero'>{n}</div>
  <div class='etiqueta'>Número ingresado</div>
  <div class='resultado'>{resultado}</div>
  <div class='pie'>.NET · Caso de Estudio SOAP · UTP Puebla</div>
</div>
</body></html>";

    byte[] buffer = Encoding.UTF8.GetBytes(html);
    responseObj.ContentType = "text/html; charset=utf-8";
    responseObj.ContentLength64 = buffer.Length;
    responseObj.OutputStream.Write(buffer, 0, buffer.Length);
    responseObj.OutputStream.Close();
}