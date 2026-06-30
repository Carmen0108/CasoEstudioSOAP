using System;
using System.IO;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;

string number = args.Length > 0 ? args[0] : "10";

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
if (match.Success)
    Console.WriteLine("Resultado: " + match.Groups[1].Value);
else
    Console.WriteLine("No se encontró resultado");