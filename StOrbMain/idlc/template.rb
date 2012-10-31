#
# $Id: template.rb 1014 2009-02-03 09:45:21Z takaya $
# Copyright (C) 2008,2009 AIST, National Institute of Advanced Industrial Science and Technology
#
module IDL
HEADER = <<'EOF'
/*
 * $FILE$
 * input file   : $INPUT$
 * generated at : $TIME$
 */

#ifndef $MACRO$
#define $MACRO$
#include "CORBA_Object.h"

%s
extern void dispatch(Req *request, Reply *reply, Giop_Hdr *header);

#endif /* $MACRO$ */
EOF

IMPL = <<'EOF'
/*
 * $FILE$
 * input file	: $INPUT$
 * generated at : $TIME$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CORBA_Object.h"
#include "Giop.h"
#include "Orbe.h"
#include "$INCLUDE$"

%s
EOF

BODY = <<'EOF'
/*
 * $FILE$
 * input file	: $INPUT$
 * generated at : $TIME$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CORBA_Object.h"
#include "Giop.h"
#include "Orbe.h"
#include "$INCLUDE$"

%s
EOF
end
