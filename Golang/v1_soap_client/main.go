package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"regexp"
	"strings"
)

func main() {
	number := "10"
	if len(os.Args) > 1 {
		number = os.Args[1]
	}

	soapBody := `<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
  <soap:Body>
    <NumberToWords xmlns="http://www.dataaccess.com/webservicesserver/">
      <ubiNum>` + number + `</ubiNum>
    </NumberToWords>
  </soap:Body>
</soap:Envelope>`

	req, _ := http.NewRequest("POST",
		"https://www.dataaccess.com/webservicesserver/NumberConversion.wso",
		strings.NewReader(soapBody))
	req.Header.Set("Content-Type", "text/xml; charset=utf-8")
	req.Header.Set("SOAPAction", "\"\"")

	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println("Error:", err)
		os.Exit(1)
	}
	defer resp.Body.Close()

	body, _ := io.ReadAll(resp.Body)
	re := regexp.MustCompile(`<m:NumberToWordsResult>(.*?)</m:NumberToWordsResult>`)
	match := re.FindStringSubmatch(string(body))

	if len(match) > 1 {
		fmt.Println("Resultado:", match[1])
	} else {
		fmt.Println("No se encontró resultado")
	}
}
