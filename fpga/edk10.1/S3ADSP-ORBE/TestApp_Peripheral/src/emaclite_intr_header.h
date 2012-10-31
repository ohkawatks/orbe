#define TESTAPP_GEN

/* $Id: emaclite_intr_header.h,v 1.1.2.1 2008/02/13 10:39:06 svemula Exp $ */


#include "xbasic_types.h"
#include "xstatus.h"

XStatus EmacLiteExample(XIntc* IntcInstancePtr,
                        XEmacLite* EmacLiteInstPtr,
                        Xuint16 EmacLiteDeviceId,
                        Xuint16 EmacLiteIntrId);


