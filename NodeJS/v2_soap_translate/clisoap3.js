const express = require('express');
const app = express();

const UNIDADES = ['cero','uno','dos','tres','cuatro','cinco','seis','siete','ocho','nueve',
  'diez','once','doce','trece','catorce','quince','dieciséis','diecisiete','dieciocho','diecinueve'];
const DECENAS = ['','','veinte','treinta','cuarenta','cincuenta','sesenta','setenta','ochenta','noventa'];
const CENTENAS = ['','ciento','doscientos','trescientos','cuatrocientos','quinientos',
  'seiscientos','setecientos','ochocientos','novecientos'];

function numeroALetras(n) {
  n = parseInt(n);
  if (n < 20)  return UNIDADES[n];
  if (n < 30)  return n === 20 ? 'veinte' : 'veinti' + UNIDADES[n - 20];
  if (n < 100) return DECENAS[Math.floor(n/10)] + (n%10 ? ' y ' + UNIDADES[n%10] : '');
  if (n < 1000) {
    if (n === 100) return 'cien';
    return CENTENAS[Math.floor(n/100)] + (n%100 ? ' ' + numeroALetras(n%100) : '');
  }
  if (n < 1000000) {
    const miles = Math.floor(n/1000);
    const resto = n % 1000;
    const pre = miles === 1 ? 'mil' : numeroALetras(miles) + ' mil';
    return resto ? pre + ' ' + numeroALetras(resto) : pre;
  }
  return 'número fuera de rango';
}

app.get('/', (req, res) => {
  const n = req.query.n || '10';
  const resultado = numeroALetras(n);
  res.send(`
    <html><body style="font-family:Arial; padding:2rem">
      <h2>Número a Letras en Español</h2>
      <p><b>Número:</b> ${n}</p>
      <p><b>En español:</b> ${resultado}</p>
    </body></html>
  `);
});

app.listen(3000, () => console.log('Servidor en http://localhost:3000'));