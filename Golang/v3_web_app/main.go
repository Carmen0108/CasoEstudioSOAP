package main

import (
	"fmt"
	"net/http"
	"strconv"
	"strings"
)

var unidades = []string{"cero", "uno", "dos", "tres", "cuatro", "cinco", "seis",
	"siete", "ocho", "nueve", "diez", "once", "doce", "trece", "catorce",
	"quince", "dieciséis", "diecisiete", "dieciocho", "diecinueve"}
var decenas = []string{"", "", "veinte", "treinta", "cuarenta", "cincuenta",
	"sesenta", "setenta", "ochenta", "noventa"}
var centenas = []string{"", "ciento", "doscientos", "trescientos", "cuatrocientos",
	"quinientos", "seiscientos", "setecientos", "ochocientos", "novecientos"}

func numeroALetras(n int64) string {
	if n < 20 {
		return unidades[n]
	}
	if n < 30 {
		if n == 20 {
			return "veinte"
		}
		return "veinti" + unidades[n-20]
	}
	if n < 100 {
		d := decenas[n/10]
		if n%10 == 0 {
			return d
		}
		return d + " y " + unidades[n%10]
	}
	if n < 1000 {
		if n == 100 {
			return "cien"
		}
		c := centenas[n/100]
		r := n % 100
		if r == 0 {
			return c
		}
		return c + " " + numeroALetras(r)
	}
	if n < 1_000_000 {
		miles := n / 1000
		r := n % 1000
		pre := "mil"
		if miles > 1 {
			pre = numeroALetras(miles) + " mil"
		}
		if r == 0 {
			return pre
		}
		return pre + " " + numeroALetras(r)
	}
	return "fuera de rango"
}

func handler(w http.ResponseWriter, r *http.Request) {
	nStr := r.URL.Query().Get("n")
	if nStr == "" {
		nStr = "10"
	}
	n, _ := strconv.ParseInt(nStr, 10, 64)
	resultado := numeroALetras(n)
	resultado = strings.ToUpper(resultado[:1]) + resultado[1:]

	html := fmt.Sprintf(`<!DOCTYPE html>
<html lang="es">
<head><meta charset="UTF-8"><title>Número a Letras</title>
<style>
* { margin:0; padding:0; box-sizing:border-box; }
body { font-family:'Segoe UI',sans-serif; background:linear-gradient(135deg,#1a1a2e,#16213e,#0f3460); min-height:100vh; display:flex; align-items:center; justify-content:center; }
.card { background:white; border-radius:16px; padding:2.5rem 3rem; box-shadow:0 20px 60px rgba(0,0,0,0.4); text-align:center; max-width:500px; width:90%%; }
.titulo { font-size:1.5rem; font-weight:700; color:#0f3460; margin-bottom:1.5rem; }
.numero { font-size:4rem; font-weight:800; color:#e94560; margin-bottom:0.5rem; }
.etiqueta { font-size:0.85rem; color:#999; text-transform:uppercase; letter-spacing:1px; margin-bottom:1.5rem; }
.resultado { background:#f0f4ff; border-left:4px solid #0f3460; border-radius:8px; padding:1rem 1.5rem; font-size:1.4rem; font-weight:600; color:#1a1a2e; }
.pie { margin-top:1.5rem; font-size:0.8rem; color:#bbb; }
</style></head>
<body>
<div class="card">
  <div class="titulo">🔢 Número a Letras en Español</div>
  <div class="numero">%s</div>
  <div class="etiqueta">Número ingresado</div>
  <div class="resultado">%s</div>
  <div class="pie">Go · Caso de Estudio SOAP · UTP Puebla</div>
</div>
</body></html>`, nStr, resultado)

	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	fmt.Fprint(w, html)
}

func main() {
	http.HandleFunc("/", handler)
	fmt.Println("Servidor en http://localhost:8080")
	http.ListenAndServe(":8080", nil)
}
