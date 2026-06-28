const express = require('express');
const app = express();

const UNIDADES = ['cero','uno','dos','tres','cuatro','cinco','seis','siete','ocho','nueve',
  'diez','once','doce','trece','catorce','quince','dieciséis','diecisiete','dieciocho','diecinueve'];
const DECENAS = ['','','veinte','treinta','cuarenta','cincuenta','sesenta','setenta','ochenta','noventa'];
const CENTENAS = ['','ciento','doscientos','trescientos','cuatrocientos','quinientos',
  'seiscientos','setecientos','ochocientos','novecientos'];

function numeroALetras(n) {
  n = parseInt(n);
  if (isNaN(n) || n < 0) return 'Número inválido';
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
  return 'Número fuera de rango';
}

app.get('/', (req, res) => {
  const n = req.query.n || '10';
  const resultado = numeroALetras(n);
  res.send(`
    <!DOCTYPE html>
    <html lang="es">
    <head>
      <meta charset="UTF-8">
      <title>Número a Letras</title>
      <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
          font-family: 'Segoe UI', sans-serif;
          background: linear-gradient(135deg, #1a1a2e, #16213e, #0f3460);
          min-height: 100vh;
          display: flex;
          align-items: center;
          justify-content: center;
        }
        .card {
          background: white;
          border-radius: 16px;
          padding: 2.5rem 3rem;
          box-shadow: 0 20px 60px rgba(0,0,0,0.4);
          text-align: center;
          max-width: 500px;
          width: 90%;
        }
        .titulo {
          font-size: 1.5rem;
          font-weight: 700;
          color: #0f3460;
          margin-bottom: 1.5rem;
        }
        .numero {
          font-size: 4rem;
          font-weight: 800;
          color: #e94560;
          margin-bottom: 0.5rem;
        }
        .etiqueta {
          font-size: 0.85rem;
          color: #999;
          text-transform: uppercase;
          letter-spacing: 1px;
          margin-bottom: 1.5rem;
        }
        .resultado {
          background: #f0f4ff;
          border-left: 4px solid #0f3460;
          border-radius: 8px;
          padding: 1rem 1.5rem;
          font-size: 1.4rem;
          font-weight: 600;
          color: #1a1a2e;
          text-transform: capitalize;
        }
        .pie {
          margin-top: 1.5rem;
          font-size: 0.8rem;
          color: #bbb;
        }
      </style>
    </head>
    <body>
      <div class="card">
        <div class="titulo">🔢 Número a Letras en Español</div>
        <div class="numero">${n}</div>
        <div class="etiqueta">Número ingresado</div>
        <div class="resultado">${resultado}</div>
        <div class="pie">Node.js · Caso de Estudio SOAP · UTP Puebla</div>
      </div>
    </body>
    </html>
  `);
});

app.listen(3000, () => console.log('Servidor en http://localhost:3000'));