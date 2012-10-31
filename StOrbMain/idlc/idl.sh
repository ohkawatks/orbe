#!/bin/sh
# $Id: idl.sh 982 2008-12-17 09:22:08Z takaya $
# Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
#

RUBY=/usr/bin/ruby
MAIN=./main.rb
base=`basename $1 .idl`

$RUBY $MAIN $1 && ( indent -kr $base.h; indent -kr $base.c; indent -kr ${base}impl.c )
