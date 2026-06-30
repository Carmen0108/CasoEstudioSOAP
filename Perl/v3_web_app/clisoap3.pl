use strict;
use warnings;
use HTTP::Daemon;
use HTTP::Status;

my @unidades = qw(cero uno dos tres cuatro cinco seis siete ocho nueve diez
                  once doce trece catorce quince dieciséis diecisiete dieciocho diecinueve);
my @decenas  = ('', '', 'veinte', 'treinta', 'cuarenta', 'cincuenta',
                'sesenta', 'setenta', 'ochenta', 'noventa');
my @centenas = ('', 'ciento', 'doscientos', 'trescientos', 'cuatrocientos',
                'quinientos', 'seiscientos', 'setecientos', 'ochocientos', 'novecientos');

sub numero_a_letras {
    my ($n) = @_;
    return $unidades[$n] if $n < 20;
    return $n == 20 ? 'veinte' : 'veinti' . $unidades[$n - 20] if $n < 30;
    if ($n < 100) {
        my $d = $decenas[int($n / 10)];
        my $u = $n % 10 ? ' y ' . $unidades[$n % 10] : '';
        return "$d$u";
    }
    if ($n < 1000) {
        return 'cien' if $n == 100;
        my $c = $centenas[int($n / 100)];
        my $r = $n % 100;
        return $r ? "$c " . numero_a_letras($r) : $c;
    }
    if ($n < 1_000_000) {
        my $miles = int($n / 1000);
        my $r = $n % 1000;
        my $pre = $miles == 1 ? 'mil' : numero_a_letras($miles) . ' mil';
        return $r ? "$pre " . numero_a_letras($r) : $pre;
    }
    return 'fuera de rango';
}

my $daemon = HTTP::Daemon->new(LocalPort => 8082, ReuseAddr => 1)
    or die "No se pudo iniciar el servidor: $!";

print "Servidor en http://localhost:8082\n";

while (my $client = $daemon->accept) {
    while (my $request = $client->get_request) {
        my $uri = $request->uri;
        my %params;
        if ($uri->query) {
            %params = map { split /=/, $_ } split /&/, $uri->query;
        }
        my $n = $params{n} // '10';
        my $resultado = numero_a_letras(int($n));
        $resultado = uc(substr($resultado, 0, 1)) . substr($resultado, 1);

        my $html = <<HTML;
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
  <div class="numero">$n</div>
  <div class="etiqueta">Número ingresado</div>
  <div class="resultado">$resultado</div>
  <div class="pie">Perl · Caso de Estudio SOAP · UTP Puebla</div>
</div>
</body></html>
HTML

        $client->send_response(HTTP::Response->new(
            200, 'OK',
            ['Content-Type' => 'text/html; charset=utf-8'],
            $html
        ));
    }
    $client->close;
}