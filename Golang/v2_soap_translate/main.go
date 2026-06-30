package main

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"net/url"
	"os"
	"regexp"
	"strings"
)

type TranslateResponse struct {
	ResponseData struct {
		TranslatedText string `json:"translatedText"`
	} `json:"responseData"`
}

func main() {
	number := "10"
	if len(os.Args) > 1 {
		number = os.Args[1]
	}

	// --- SOAP ---
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

	resp, _ := http.DefaultClient.Do(req)
	defer resp.Body.Close()
	body, _ := io.ReadAll(resp.Body)

	re := regexp.MustCompile(`<m:NumberToWordsResult>(.*?)</m:NumberToWordsResult>`)
	match := re.FindStringSubmatch(string(body))
	english := ""
	if len(match) > 1 {
		english = match[1]
	}

	// --- Traducción MyMemory ---
	transURL := "https://api.mymemory.translated.net/get?q=" +
		url.QueryEscape(english) + "&langpair=en|es"
	transResp, _ := http.Get(transURL)
	defer transResp.Body.Close()
	transBody, _ := io.ReadAll(transResp.Body)

	var result TranslateResponse
	json.Unmarshal(transBody, &result)

	fmt.Println("Número:", number)
	fmt.Println("En inglés:", english)
	fmt.Println("En español:", result.ResponseData.TranslatedText)
}
