use actix_web::{web, App, HttpResponse, HttpServer};

fn numero_a_letras(n: u64) -> String {
    let unidades = ["cero","uno","dos","tres","cuatro","cinco","seis","siete",
        "ocho","nueve","diez","once","doce","trece","catorce","quince",
        "dieciséis","diecisiete","dieciocho","diecinueve"];
    let decenas = ["","","veinte","treinta","cuarenta","cincuenta",
        "sesenta","setenta","ochenta","noventa"];
    let centenas = ["","ciento","doscientos","trescientos","cuatrocientos",
        "quinientos","seiscientos","setecientos","ochocientos","novecientos"];

    if n < 20 { return unidades[n as usize].to_string(); }
    if n < 30 { return if n == 20 { "veinte".to_string() } else { format!("veinti{}", unidades[(n-20) as usize]) }; }
    if n < 100 {
        let d = decenas[(n/10) as usize];
        return if n%10 == 0 { d.to_string() } else { format!("{} y {}", d, unidades[(n%10) as usize]) };
    }
    if n < 1000 {
        if n == 100 { return "cien".to_string(); }
        let c = centenas[(n/100) as usize];
        let r = n % 100;
        return if r == 0 { c.to_string() } else { format!("{} {}", c, numero_a_letras(r)) };
    }
    if n < 1_000_000 {
        let miles = n/1000; let r = n%1000;
        let pre = if miles == 1 { "mil".to_string() } else { format!("{} mil", numero_a_letras(miles)) };
        return if r == 0 { pre } else { format!("{} {}", pre, numero_a_letras(r)) };
    }
    "fuera de rango".to_string()
}

async fn convert(query: web::Query<std::collections::HashMap<String, String>>) -> HttpResponse {
    let n = query.get("n").cloned().unwrap_or_else(|| "10".to_string());
    let num: u64 = n.parse().unwrap_or(10);
    let resultado = numero_a_letras(num);
    let resultado = format!("{}{}", &resultado[..1].to_uppercase(), &resultado[1..]);

    let html = format!(r#"<!DOCTYPE html>
<html lang="es">
<head><meta charset="UTF-8"><title>Número a Letras</title>
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
<div class="card">
  <div class="titulo">🔢 Número a Letras en Español</div>
  <div class="numero">{}</div>
  <div class="etiqueta">Número ingresado</div>
  <div class="resultado">{}</div>
  <div class="pie">Rust · Caso de Estudio SOAP · UTP Puebla</div>
</div>
</body></html>"#, n, resultado);

    HttpResponse::Ok().content_type("text/html; charset=utf-8").body(html)
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    println!("Servidor en http://localhost:8083");
    HttpServer::new(|| App::new().route("/", web::get().to(convert)))
        .bind("0.0.0.0:8083")?
        .run()
        .await
}