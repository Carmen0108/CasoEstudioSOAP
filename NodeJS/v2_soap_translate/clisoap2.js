const soap = require('soap');
const http = require('http');
const url = require('url');
const { translate } = require('@vitalets/google-translate-api');

const WSDL = 'https://www.dataaccess.com/webservicesserver/NumberConversion.wso?WSDL';

http.createServer((req, res) => {
  const n = url.parse(req.url, true).query.n || '10';
  soap.createClient(WSDL, async (err, client) => {
    client.NumberToWords({ ubiNum: n }, async (err, result) => {
      const english = result.NumberToWordsResult;
      const translated = await translate(english, { from: 'en', to: 'es' });
      res.writeHead(200, {'Content-Type': 'text/plain; charset=utf-8'});
      res.end(translated.text);
    });
  });
}).listen(3000, () => console.log('Servidor en http://localhost:3000'));