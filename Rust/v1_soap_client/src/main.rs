use std::env;

fn main() {
    let number = env::args().nth(1).unwrap_or_else(|| "10".to_string());

    let soap_body = format!(r#"<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
  <soap:Body>
    <NumberToWords xmlns="http://www.dataaccess.com/webservicesserver/">
      <ubiNum>{}</ubiNum>
    </NumberToWords>
  </soap:Body>
</soap:Envelope>"#, number);

    let client = reqwest::blocking::Client::new();
    let response = client
        .post("https://www.dataaccess.com/webservicesserver/NumberConversion.wso")
        .header("Content-Type", "text/xml; charset=utf-8")
        .header("SOAPAction", "\"\"")
        .body(soap_body)
        .send()
        .expect("Error en la petición");

    let body = response.text().expect("Error leyendo respuesta");
    let re = regex::Regex::new(r"<m:NumberToWordsResult>(.*?)</m:NumberToWordsResult>").unwrap();

    if let Some(cap) = re.captures(&body) {
        println!("Resultado: {}", &cap[1]);
    } else {
        println!("No se encontró resultado");
    }
}