
#ifdef SOCKET_CHANNEL_USE_ECU_CAN

#include "EcuCan.h"

#define ECUCAN_WAITTIME 10000
#define XCAN_MAX_FRAME_SIZE_IN_WORDS (XCAN_MAX_FRAME_SIZE / sizeof(Xuint32))

static int ECUCAN_DEVICE_ID;
static int INTC_DEVICE_ID;
static int INTC_ECUCAN_VEC_ID;

static XCan Can;
static Xuint8 buff[400];
static Xuint32 work_buff[400];
static Xuint32 can_id;
static Xuint32 RxFrame[XCAN_MAX_FRAME_SIZE_IN_WORDS];

int EcuCan_setupDevice(int ecucan_device_id, int intc_device_id, int intc_ecucan_vec_id) {
	ECUCAN_DEVICE_ID = ecucan_device_id;
	INTC_DEVICE_ID = intc_device_id;
	INTC_ECUCAN_VEC_ID = intc_ecucan_vec_id;
	return 0;
}

void EcuCan_initialize(void) 
{
	XStatus Result;
	
	Result = XCan_Initialize(&Can, ECUCAN_DEVICE_ID);
//    if (Result != XST_SUCCESS)
//    {
//        return Result;
//    }
    
    Result = XCan_SelfTest(&Can);
//    if (Result != XST_SUCCESS)
//    {
//        return Result;
//    }
    
    XCan_EnterMode(&Can, XCAN_MODE_CONFIG);
    while(XCan_GetMode(&Can) != XCAN_MODE_CONFIG);
    
    XCan_SetBaudRatePrescaler(&Can, 11); // 11(12)=100Kbps, 23(24)=50kbps
    XCan_SetBitTiming(&Can, 3, 2, 15);
    
    XCan_EnterMode(&Can, XCAN_MODE_NORMAL);
    while(XCan_GetMode(&Can) != XCAN_MODE_NORMAL);
}

size_t EcuCan_send(u8* start, size_t length)
{
	Xuint8 send_length;
	int current_length = 0;
	int i;
	int j;

	do {
		if ((length - current_length) >= 8) {
			send_length = 8;
		} else {
			send_length = (length - current_length);
		}
		
		for ( i = 0; i < ECUCAN_WAITTIME; i++ ) {
			j++;
		}
		
		SendFrame(&Can, send_length, &start[current_length]);
		
		current_length += send_length;
	
	} while (current_length < length);
	
	return current_length;
}

size_t EcuCan_recv(u8* start, size_t length)
{
	XStatus ret = XST_SUCCESS;
	Xuint32	canID, recv_length = 0;
	Xuint32	dlc;
	static Xuint32 current_length = 0, giop_length = 0;
	
	if (giop_length == 0) {
		do {
			ret = RecvFrame(&Can, &canID, &dlc, recv_length);
			if (ret != XST_SUCCESS)
				continue;
			else	
				recv_length += dlc;
			
			if ((giop_length == 0) && (recv_length >= GIOP_HDR)) {
				giop_length = getLength();
			}
//			xil_printf("EcuCan_recv can_id = %x count = %d\r\n", canID, count++);
		} while ((giop_length + GIOP_HDR) > recv_length);
		
		can_id = canID;
	}
	
	memcpy(start, &buff[current_length], length);
	current_length += length;
	
	if (current_length >= (giop_length + GIOP_HDR))
	{
		current_length = 0;
		giop_length = 0;
	}
	
	return length;
}

static XStatus SendFrame(XCan *InstancePtr, Xuint8 send_length, Xuint8 *send_buff)
{
    XStatus Result;
    
    work_buff[0] = XCan_mCreateIdValue(0x101, 0, 0, 0, 0);
    work_buff[1] = XCan_mCreateDlcValue(send_length);

	memcpy(&work_buff[2], send_buff, send_length);

//	print("SendFrame wait\r\n");
    while (XCan_mIsTxFifoFull(InstancePtr) == XTRUE);

//	xil_printf("SendFrame canID = %d\r\n", work_buff[0]);
	
    Result = XCan_Send(InstancePtr, work_buff);

    return Result;
}

static XStatus RecvFrame(XCan *InstancePtr, Xuint32	*canID, Xuint32 *dlc, Xuint32 current_length)
{
    XStatus Result;

//	print("RecvFrame wait\r\n");
    while (XCan_mIsRxEmpty(InstancePtr) == XTRUE);

//	print("RecvFrame\r\n");
	
    Result = XCan_Recv(InstancePtr, RxFrame);
    if (Result == XST_SUCCESS)
    {
    	*canID = RxFrame[0] >> 21;
    	*dlc = RxFrame[1] >> 28;

        memcpy(&buff[current_length], &RxFrame[2], 8);        
    }

//	xil_printf("RecvFrame canID = %x dlc = %d\r\n", *canID, *dlc);
    return Result;
}

Xuint32 getLength()
{
	Xuint32 length = 0;
	Xuint8 tmp;
	Xuint8 endian = buff[6];
	Xuint8 work[4];
	
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

#endif
