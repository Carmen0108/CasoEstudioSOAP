const soap = require('soap');
const http = require('http');
const url = require('url');

const WSDL = 'https://www.dataaccess.com/webservicesserver/NumberConversion.wso?WSDL';

http.createServer((req, res) => {
  const n = url.parse(req.url, true).query.n || '10';
  soap.createClient(WSDL, (err, client) => {
    client.NumberToWords({ ubiNum: n }, (err, result) => {
      res.writeHead(200, {'Content-Type': 'text/plain; charset=utf-8'});
      res.end(result.NumberToWordsResult);
    });
  });
}).listen(3000, () => console.log('Servidor en http://localhost:3000'));