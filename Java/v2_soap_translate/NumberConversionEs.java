import java.io.*;
import java.net.*;
import java.util.regex.*;

public class NumberConversionEs {
    public static void main(String[] args) throws Exception {
        String number = args.length > 0 ? args[0] : "10";

        // --- SOAP ---
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

        BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream(), "UTF-8"));
        StringBuilder response = new StringBuilder();
        String line;
        while ((line = br.readLine()) != null) response.append(line);

        Matcher m = Pattern.compile("<m:NumberToWordsResult>(.*?)</m:NumberToWordsResult>")
                          .matcher(response.toString());
        String english = m.find() ? m.group(1) : "";

        // --- Traducción con MyMemory API ---
        String encoded = URLEncoder.encode(english, "UTF-8");
        URL transUrl = new URL("https://api.mymemory.translated.net/get?q=" + encoded + "&langpair=en|es");
        HttpURLConnection transConn = (HttpURLConnection) transUrl.openConnection();
        transConn.setRequestMethod("GET");
        transConn.setConnectTimeout(10000);
        transConn.setReadTimeout(10000);

        BufferedReader br2 = new BufferedReader(new InputStreamReader(transConn.getInputStream(), "UTF-8"));
        StringBuilder transResponse = new StringBuilder();
        String line2;
        while ((line2 = br2.readLine()) != null) transResponse.append(line2);

        Matcher tm = Pattern.compile("\"translatedText\":\"(.*?)\"")
                           .matcher(transResponse.toString());
        String spanish = tm.find() ? tm.group(1) : english;

        System.out.println("Número: " + number);
        System.out.println("En inglés: " + english);
        System.out.println("En español: " + spanish);
    }
}