#!/usr/bin/perl

use IO::Socket;

use constant REPETITIONS_NUMBER => 10;
 
my $get_cmd  = "get WTF\r\n";

for(my $x = 0; $x < REPETITIONS_NUMBER; ++$x) {
 
  my $sock = new IO::Socket::INET(
                                    PeerAddr => 'localhost',
                                    PeerPort => '9696',
                                    Proto => 'tcp',
                                 );
 
  die "Could not create socket: $!\n" unless $sock;
 
  #print "begin write!\n";
  print $sock $get_cmd;
  #print "end write!\n";
  my $var = <$sock>;
  print $var, "\n"; 
  close($sock);
}
