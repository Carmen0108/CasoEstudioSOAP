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

    // --- SOAP ---
    let response = client
        .post("https://www.dataaccess.com/webservicesserver/NumberConversion.wso")
        .header("Content-Type", "text/xml; charset=utf-8")
        .header("SOAPAction", "\"\"")
        .body(soap_body)
        .send()
        .expect("Error SOAP");

    let body = response.text().unwrap();
    let re = regex::Regex::new(r"<m:NumberToWordsResult>(.*?)</m:NumberToWordsResult>").unwrap();
    let english = re.captures(&body)
        .map(|c| c[1].to_string())
        .unwrap_or_default();

    // --- Traducción MyMemory ---
    let encoded = urlencoding::encode(&english).to_string();
    let trans_url = format!("https://api.mymemory.translated.net/get?q={}&langpair=en|es", encoded);
    let trans_response = client.get(&trans_url).send().expect("Error traducción");
    let trans_json: serde_json::Value = trans_response.json().unwrap();
    let spanish = trans_json["responseData"]["translatedText"]
        .as_str()
        .unwrap_or(&english)
        .to_string();

    println!("Número: {}", number);
    println!("En inglés: {}", english);
    println!("En español: {}", spanish);
}