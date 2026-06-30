require 'sinatra'

UNIDADES = %w[cero uno dos tres cuatro cinco seis siete ocho nueve diez
              once doce trece catorce quince dieciséis diecisiete dieciocho diecinueve]
DECENAS  = %w[_ _ veinte treinta cuarenta cincuenta sesenta setenta ochenta noventa]
CENTENAS = %w[_ ciento doscientos trescientos cuatrocientos quinientos
              seiscientos setecientos ochocientos novecientos]

def numero_a_letras(n)
  n = n.to_i
  return UNIDADES[n] if n < 20
  return n == 20 ? 'veinte' : "veinti#{UNIDADES[n - 20]}" if n < 30
  if n < 100
    d = DECENAS[n / 10]
    u = n % 10 == 0 ? '' : " y #{UNIDADES[n % 10]}"
    return "#{d}#{u}"
  end
  if n < 1000
    return 'cien' if n == 100
    c = CENTENAS[n / 100]
    r = n % 100
    return r == 0 ? c : "#{c} #{numero_a_letras(r)}"
  end
  if n < 1_000_000
    miles = n / 1000
    r = n % 1000
    pre = miles == 1 ? 'mil' : "#{numero_a_letras(miles)} mil"
    return r == 0 ? pre : "#{pre} #{numero_a_letras(r)}"
  end
  'fuera de rango'
end

get '/' do
  n = params[:n] || '10'
  resultado = numero_a_letras(n.to_i)
  resultado = resultado[0].upcase + resultado[1..]

  <<~HTML
    <!DOCTYPE html>
    <html lang="es">
    <head><meta charset="UTF-8"><title>Número a Letras</title>
    <style>
      * { margin:0; padding:0; box-sizing:border-box; }
      body { font-family:'Segoe UI',sans-serif; background:linear-gradient(135deg,#1a1a2e,#16213e,#0f3460); min-height:100vh; display:flex; align-items:center; justify-content:center; }
      .card { background:white; border-radius:16px; padding:2.5rem 3rem; box-shadow:0 20px 60px rgba(0,0,0,0.4); text-align:center; max-width:500px; width:90%; }
      .titulo { font-size:1.5rem; font-weight:700; color:#0f3460; margin-bottom:1.5rem; }
      .numero { font-size:4rem; font-weight:800; color:#e94560; margin-bottom:0.5rem; }
      .etiqueta { font-size:0.85rem; color:#999; text-transform:uppercase; letter-spacing:1px; margin-bottom:1.5rem; }
      .resultado { background:#f0f4ff; border-left:4px solid #0f3460; border-radius:8px; padding:1rem 1.5rem; font-size:1.4rem; font-weight:600; color:#1a1a2e; }
      .pie { margin-top:1.5rem; font-size:0.8rem; color:#bbb; }
    </style></head>
    <body>
      <div class="card">
        <div class="titulo">🔢 Número a Letras en Español</div>
        <div class="numero">#{n}</div>
        <div class="etiqueta">Número ingresado</div>
        <div class="resultado">#{resultado}</div>
        <div class="pie">Ruby · Caso de Estudio SOAP · UTP Puebla</div>
      </div>
    </body></html>
  HTML
end

set :port, 4567