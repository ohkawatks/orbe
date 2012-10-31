
#include	"Orbe.h"
#include	"MOTOR.h"
#include	"usr_def.h"
#include	"MB91265.h"
#include	"motor_drv.h"

unsigned char	gMotorStartFlag;
unsigned char	gMtBrake;


void myptInitialize(void)
{
	DDR0 = 0xFF;	/* LED port output				*/
	PDR0 = 0x07;	/* LED port						*/
	PCR0 = 0x00;	/* LED port pull down			*/
	DDR5 = 0xFF;	/* LED port output				*/
	PDR5 = 0xFF;	/* LED port						*/
	DDR4 = 0xBC;	/* switch&A/D port input		*/
	PDR4 = 0x30;	/* switch&A/D port				*/
	PCR4 = 0x00;	/* switch&A/D port pull down	*/

}

void sysInit()
{
	InitIrqLevels();
	__EI();
	
	UART0init();
	myptInitialize();
	mtInitialize();
}

int main(int argc, char *argv[])
{
	struct corba_object MOTOR_value;
	MOTOR object = &MOTOR_value;

	sysInit();
	
	startMotor();
	
	Orbe_setAlive(1);	
	Orbe_setDebugFlag(0);
//	Orbe_setObjectKey(object, "test_object");
	Orbe_startServer();
	return 0;
}
