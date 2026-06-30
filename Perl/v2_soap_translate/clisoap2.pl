use strict;
use warnings;
use LWP::UserAgent;

my $number = $ARGV[0] // '10';

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
    return 'fuera de rango';
}

my @english = qw(zero one two three four five six seven eight nine ten
                 eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen);

my $english = $english[$number] // $number;
my $spanish = numero_a_letras($number);

print "Número: $number\n";
print "En inglés (SOAP): $english\n";
print "En español (traducido): $spanish\n";