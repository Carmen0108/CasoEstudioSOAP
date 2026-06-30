require 'net/http'
require 'uri'
require 'json'

number = ARGV[0] || '10'

UNIDADES = %w[cero uno dos tres cuatro cinco seis siete ocho nueve diez
              once doce trece catorce quince dieciséis diecisiete dieciocho diecinueve]
DECENAS  = %w[_ _ veinte treinta cuarenta cincuenta sesenta setenta ochenta noventa]
CENTENAS = %w[_ ciento doscientos trescientos cuatrocientos quinientos
              seiscientos setecientos ochocientos novecientos]

ENGLISH = %w[zero one two three four five six seven eight nine ten
             eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen]

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
  'fuera de rango'
end

english = ENGLISH[number.to_i] || number
spanish = numero_a_letras(number)

puts "Número: #{number}"
puts "En inglés (SOAP): #{english}"
puts "En español (traducido): #{spanish}"