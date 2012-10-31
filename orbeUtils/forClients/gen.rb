#!/usr/local/bin/ruby
#
# $Id: gen.rb 986 2009-01-21 02:59:13Z takaya $
# Copyright (C) 2009 AIST, National Institute of Advanced Industrial Science and Technology
#
# Description:
# generate a ORBE's client from capture binary file.
# You would be required actual Socket_{initialize,send,recv}.
#
# Usage: gen.rb [-d] [-e] [-w] capture
# 
# Options:
# -d ... debug mode. set Orbe_getDebugFlag() 1.
#        default is unset.
# -e ... exchange endian to get packet length in case of setting -w option.
# -w ... wait reply from server.
#
# Example:
# % gen.rb -d -w ../../StOrbMain/giopData/JavaIDL-rm11-op11-GIOP-Request.bin > main.c
# % gcc -DSOCKET_CHANNEL_USE_STDINOUT -I../../StOrbMain/pc main.c ../../StOrbMain/pc/StdInOut.o -o main 
# % ./main
#

require	"optparse"

OPTS = Hash.new(false) 

opt = OptionParser.new
opt.on('-d') {|v| OPTS[:d] = v }	## debug mode
opt.on('-e') {|v| OPTS[:e] = v }	## need endian exchange
opt.on('-w') {|v| OPTS[:w] = v }	## need to wait reply

opt.parse!(ARGV)

raise "need data binary." unless ARGV[0]
gets nil

print <<EOF
#include	<stdio.h>


#include	"Orbe.h"
#include	"OrbeConfig.h"

#if !defined(SOCKET_CHANNEL_USE_UDP) && !defined(SOCKET_CHANNEL_USE_UART)
#	define SOCKET_CHANNEL_USE_STDINOUT
#endif

#include	"Socket.h"

#undef Orbe_exchangeInt
#define	Orbe_exchangeInt(x) #{OPTS[:e] ? "swapl(x)" : "(x)"}

int Orbe_getDebugFlag()
{
	return #{OPTS[:d] ? 1 : 0};
}

int main(int argc, char *argv[])
{
	u8 *p = #{$_.inspect};
	unsigned int len = #{$_.size};

	Socket_initialize();
	Socket_send(p, len);

#if #{OPTS[:w] ? 1 : 0}
	u8 buf[4096];
	Socket_recv(buf, 12);
	len = *(u32*)(buf+8);
	Socket_recv(buf, Orbe_exchangeInt(len));
#endif

	return 0;
}
EOF
