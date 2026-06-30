using System;
using System.IO;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;

string number = args.Length > 0 ? args[0] : "10";

// --- SOAP ---
string soapBody = $@"<?xml version=""1.0"" encoding=""utf-8""?>
<soap:Envelope xmlns:soap=""http://schemas.xmlsoap.org/soap/envelope/"">
  <soap:Body>
    <NumberToWords xmlns=""http://www.dataaccess.com/webservicesserver/"">
      <ubiNum>{number}</ubiNum>
    </NumberToWords>
  </soap:Body>
</soap:Envelope>";

HttpWebRequest request = (HttpWebRequest)WebRequest.Create(
    "https://www.dataaccess.com/webservicesserver/NumberConversion.wso");
request.Method = "POST";
request.ContentType = "text/xml; charset=utf-8";
request.Headers.Add("SOAPAction", "\"\"");

byte[] bodyBytes = Encoding.UTF8.GetBytes(soapBody);
request.ContentLength = bodyBytes.Length;
using (Stream stream = request.GetRequestStream())
    stream.Write(bodyBytes, 0, bodyBytes.Length);

using HttpWebResponse response = (HttpWebResponse)request.GetResponse();
using StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
string responseBody = reader.ReadToEnd();

Match match = Regex.Match(responseBody, @"<m:NumberToWordsResult>(.*?)</m:NumberToWordsResult>");
string english = match.Success ? match.Groups[1].Value : "";

// --- Traducción MyMemory ---
string encoded = Uri.EscapeDataString(english);
HttpWebRequest transRequest = (HttpWebRequest)WebRequest.Create(
    $"https://api.mymemory.translated.net/get?q={encoded}&langpair=en|es");
transRequest.Method = "GET";

using HttpWebResponse transResponse = (HttpWebResponse)transRequest.GetResponse();
using StreamReader transReader = new StreamReader(transResponse.GetResponseStream(), Encoding.UTF8);
string transBody = transReader.ReadToEnd();

Match transMatch = Regex.Match(transBody, @"""translatedText"":""(.*?)""");
string spanish = transMatch.Success ? transMatch.Groups[1].Value : english;

Console.WriteLine($"Número: {number}");
Console.WriteLine($"En inglés: {english}");
Console.WriteLine($"En español: {spanish}");