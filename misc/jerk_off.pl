#!/usr/bin/perl

use IO::Socket;

use constant REPETITIONS_NUMBER => 30000000;
 
my $get_cmd  = "get key";

my @chars = ('A', 'B', 'C', 'D', 'E', 'F');

sub make_key {
  my $ret = "ttt";
  for(my $x = 0; $x < 7; ++$x) {
    $ret .= $chars[rand 6];  
  }
  return $ret;
}

$key = make_key();

for(my $x = 0; $x < REPETITIONS_NUMBER; ++$x) {
 
  my $sock = new IO::Socket::INET(
                                    PeerAddr => 'localhost',
                                    PeerPort => '9696',
                                    Proto => 'tcp',
                                 );
 
  die "Could not create socket: $!\n" unless $sock;
  
  #print "begin write!\n";
  #print $get_cmd.($x+145)."\r\n";
  $key = make_key();
  #print $sock $get_cmd.$x.$key."\r\n";
  print $sock "get ".$key."\r\n";
  #print "end write!\n";
  my $var = <$sock>;
  my $var2 = "empty";
  if (substr($var,0,3) eq "VAL")
  {
    $var2 = <$sock>;
  }
    
  #print $var, $var2, "\n"; 
  close($sock);
}
