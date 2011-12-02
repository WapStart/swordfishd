#!/usr/bin/perl

use IO::Socket;

use constant REPETITIONS_NUMBER => 50;
 
my $get_cmd  = "get key";

for(my $x = 0; $x < REPETITIONS_NUMBER; ++$x) {
 
  my $sock = new IO::Socket::INET(
                                    PeerAddr => 'localhost',
                                    PeerPort => '9696',
                                    Proto => 'tcp',
                                 );
 
  die "Could not create socket: $!\n" unless $sock;
 
  #print "begin write!\n";
  print $get_cmd.$x."\r\n";
  print $sock $get_cmd.$x."\r\n";
  #print "end write!\n";
  my $var = <$sock>;
  my $var2 = "empty";
  if (substr($var,0,3) eq "VAL")
  {
    $var2 = <$sock>;
  }
    
  print $var, $var2, "\n"; 
  close($sock);
}
