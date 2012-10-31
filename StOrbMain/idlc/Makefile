#
# $Id: Makefile 972 2008-12-16 09:53:34Z takaya $
# Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
#
all:	idlParser.rb main.rb

main.rb:	parse2.y
	racc parse2.y -o main.rb

idlParser.rb:	parse.y
	racc parse.y -o idlParser.rb

clean:
	-rm -f *~ *.o idlParser.rb main.rb
