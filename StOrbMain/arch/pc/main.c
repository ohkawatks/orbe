/*
 * $Id: main.c 1016 2009-02-03 09:51:31Z takaya $
 * Copyright (C) 2008,2009 AIST, National Institute of Advanced Industrial Science and Technology 
 */
#include	"Orbe.h"
#include	"OrbeConfig.h"
#include	"rm11.h"


int main(int argc, char *argv[])
{
	OrbeConfig_setup();
	
	struct corba_object rm11_value;
	rm11 object = &rm11_value;

	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-d") == 0) {
			int debug_flag = 1;
			Orbe_setDebugFlag(debug_flag);
		}
	}
//	Orbe_setObjectKey(object, "test_object");
	Orbe_startServer();
	return 0;
}
