/*
 * $Id: StdInOut.c 622 2008-08-15 01:03:13Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology 
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"xbasic_types.h"
#include	"Orbe.h"

static FILE *in;
static FILE *out;

void StdInOut_initialize()
{
        in = stdin;
        out = stdout;
        return;
}

unsigned int StdInOut_recv(u8* start, unsigned int length)
{
        size_t len;
        len = fread(start, sizeof(u8), length, in);

		int i;
		DEBUG("RECV: size=%d\n", len);
		for (i = 0; i < len; i++) {
		DEBUG("%02x", start[i]);
			if ((i + 1) % 16 == 0) {
				DEBUG("\n");
			} else if (i % 2 == 1) {
				DEBUG(" ");
			}
		}
		DEBUG("\n");
        return len;
}

int Socket_eof() {
	return feof(in);
}

unsigned int StdInOut_send(u8* start, unsigned int length)
{
        unsigned int len;
        len = fwrite(start, sizeof(u8), length, out);

		int i;
		DEBUG("SEND: size=%d\n", len);
		for (i = 0; i < len; i++) {
			DEBUG("%02x", start[i]);
			if ((i + 1) % 16 == 0) {
				DEBUG("\n");
			} else if (i % 2 == 1) {
			DEBUG(" ");
			}
		}
		DEBUG("\n");
        return len;
}

/*
int main(int argc, char *argv[], char *env[]) {

        char *buf = (char *)malloc(sizeof(char) * 10);
        int i = 0;

        StdInOut_initilize();
        StdInOut_recv(buf, 10);

        for (i = 0; i < 10; i++) {
                printf("%02x ", buf[i]);
        }

	buf[0] = 'x';
	buf[1] = 'y';
	buf[2] = 'z';
	buf[3] = '\n';
	buf[4] = '\0';

	printf("\n");
	StdInOut_send(buf, strlen(buf));
	return 0;
}
*/
