use strict;
use warnings;
use LWP::UserAgent;

my $number = $ARGV[0] // '10';

my $soap_body = qq{<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
  <soap:Body>
    <NumberToWords xmlns="http://www.dataaccess.com/webservicesserver/">
      <ubiNum>$number</ubiNum>
    </NumberToWords>
  </soap:Body>
</soap:Envelope>};

my $ua = LWP::UserAgent->new(timeout => 30);
my $response = $ua->post(
    'https://www.dataaccess.com/webservicesserver/NumberConversion.wso',
    'Content-Type' => 'text/xml; charset=utf-8',
    'SOAPAction'   => '""',
    Content        => $soap_body
);

if ($response->is_success) {
    my $body = $response->decoded_content;
    if ($body =~ /<m:NumberToWordsResult>(.*?)<\/m:NumberToWordsResult>/) {
        print "Resultado: $1\n";
    }
} else {
    print "Error: " . $response->status_line . "\n";
}