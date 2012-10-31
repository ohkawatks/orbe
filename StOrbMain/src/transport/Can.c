#include	"Can.h"

static CAN_U8 buff[400];
static CAN_U32 can_id;

void Can_initialize(void)
{
	if (CAN_DEBUG)
		UART_printf("call Can_initialize\n\r");
	canInitialize();
}

size_t Can_send(CAN_U8* start, size_t length)
{
	CAN_U8 send_length;
	int current_length = 0;
	int i, j;

	if (CAN_DEBUG) {
		UART_printf("call Can_send\n\r"); 
		CanDebug(start, length);
	}
		
	do {
	
		if ((length - current_length) >= 8)
			send_length = 8;
		else
			send_length = (length - current_length);
		
		canSendData(4, send_length, &start[current_length]);
		
		for(i = 0; i < 25000; i++) // 2500 ‚­‚ç‚¢‚ÅOK
			j += i;
//		if (CAN_DEBUG) {
//			CanDebug(&start[current_length], send_length);
//		}
		
		current_length += send_length;
	
	} while (current_length < length);
	
	if (CAN_DEBUG)
		UART_printf("Can_send current_length = %d\n\r", current_length);
		
	return current_length;
}

size_t Can_recv(CAN_U8* start, size_t length)
{
	CAN_U8 	ret = CAN_RECV_OK;
	CAN_U32	canID, recv_length = 0;
	CAN_U8	dlc;
	static CAN_U32 current_length = 0, giop_length = 0;
	
	if (CAN_DEBUG)
		UART_printf("call Can_recv\n\r"); 
	
	if (giop_length == 0) {
		do {
			ret = canRecvData(1, &canID, &dlc, &buff[recv_length]);
			if (ret != CAN_RECV_OK)
				continue;
			else	
				recv_length += dlc;
			
			if ((giop_length == 0) && (recv_length >= GIOP_HDR)) {
				giop_length = getLength();
			}
		} while ((giop_length + GIOP_HDR) > recv_length);
		
		can_id = canID;
	}
	
	memcpy(start, &buff[current_length], length);
	current_length += length;
	
	if (CAN_DEBUG)
		CanDebug(start, length);
	
	if (current_length >= (giop_length + GIOP_HDR))
	{
		current_length = 0;
		giop_length = 0;
	}
	
	return length;
}

CAN_U32 getLength() 
{
	CAN_U32 length = 0;
	CAN_U8 tmp;
	CAN_U8 endian = buff[6];
	CAN_U8 work[4];
	
	memcpy(work, &buff[8], 4);
	
	if (endian == 1) 
	{
		tmp = work[0];
		work[0] = work[3];
		work[3] = tmp;
		tmp = work[1];
		work[1] = work[2];
		work[2] = tmp;
	}
	
	length += (work[0] << 24);
	length += (work[1] << 16);
	length += (work[2] << 8);
	length += work[3];
	
	return length;
}

void CanDebug(CAN_U8* data, size_t length)
{
	int i;
	
	UART_printf("data = ");
	for (i = 0; i < length; i++)
		UART_printf("%02x ", data[i]);
	UART_printf("\n\r");
}

