import java.io.*;
import java.net.*;
import java.util.regex.*;
import javax.net.ssl.*;

public class NumberConversion {
    public static void main(String[] args) throws Exception {
        String number = args.length > 0 ? args[0] : "10";

        String soapBody = "<?xml version=\"1.0\" encoding=\"utf-8\"?>" +
            "<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">" +
            "<soap:Body><NumberToWords xmlns=\"http://www.dataaccess.com/webservicesserver/\">" +
            "<ubiNum>" + number + "</ubiNum>" +
            "</NumberToWords></soap:Body></soap:Envelope>";

        URL url = new URL("https://www.dataaccess.com/webservicesserver/NumberConversion.wso");
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setDoOutput(true);
        conn.setDoInput(true);
        conn.setRequestMethod("POST");
        conn.setRequestProperty("Content-Type", "text/xml; charset=utf-8");
        conn.setRequestProperty("SOAPAction", "\"\"");
        conn.setRequestProperty("User-Agent", "Java/1.8");
        conn.setConnectTimeout(10000);
        conn.setReadTimeout(10000);

        byte[] bodyBytes = soapBody.getBytes("UTF-8");
        conn.setRequestProperty("Content-Length", String.valueOf(bodyBytes.length));

        try (OutputStream os = conn.getOutputStream()) {
            os.write(bodyBytes);
            os.flush();
        }

        InputStream is = conn.getResponseCode() == 200 
            ? conn.getInputStream() 
            : conn.getErrorStream();

        BufferedReader br = new BufferedReader(new InputStreamReader(is, "UTF-8"));
        StringBuilder response = new StringBuilder();
        String line;
        while ((line = br.readLine()) != null) response.append(line);

        System.out.println("Respuesta: " + response.toString());

       Matcher m = Pattern.compile("<m:NumberToWordsResult>(.*?)</m:NumberToWordsResult>")
                          .matcher(response.toString());
        if (m.find()) {
            System.out.println("Resultado: " + m.group(1));
        } else {
            System.out.println("No se encontró resultado");
        }
    }
}