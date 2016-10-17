// MOTORImpl.c.template
// generated by ORBE Idl Compiler
// InputFile: MOTOR.idl
// @Tue Nov 11 16:28:37 2008

#include    "CORBA_Object.h"
#include    "MOTOR.h"
#include	"motor_drv.h"
#include	"MB91265.h"
#include	"usr_def.h"

unsigned char turnDirection = 0;
extern unsigned char gDirection;
unsigned int LED_reg = 0;

void startMotor(void)
{
	OCSH1_OTE0 = 1;
	OCSH1_OTE1 = 1;
	OCSH3_OTE0 = 1;
	OCSH3_OTE1 = 1;
	OCSH5_OTE0 = 1;
	OCSH5_OTE1 = 1;
	TCCSL0_STOP = 0;
}

void stopMotor(void)
{
	OCSH1_OTE0 = 0;
	OCSH1_OTE1 = 0;
	OCSH3_OTE0 = 0;
	OCSH3_OTE1 = 0;
	OCSH5_OTE0 = 0;
	OCSH5_OTE1 = 0;
	PDR3 = 0x00;
	
	TCCSL0_STOP = 1;	/* FreeRunTimer 0 Start */
}

void MOTOR_stepRight(
	MOTOR o,
	CORBA_Environment* ev)
{
	mtPWMControllerFWD();
	turnDirection = 0;
}

void MOTOR_stepLeft(
	MOTOR o,
	CORBA_Environment* ev)
{
	mtPWMControllerBWD();
	turnDirection = 1;
}

void MOTOR_startSpin(
	MOTOR o,
	CORBA_octet turnD,
	CORBA_ushort speed,
	CORBA_Environment* ev)
{
	gDirection = turnD;
//	mtSetDuty(speed, 0xFFFF);
	//startMotor();
	Motor_drv_setSpinning(1);
}

void MOTOR_stopSpin(
	MOTOR o,
	CORBA_Environment* ev)
{
	//stopMotor();
	Motor_drv_setSpinning(0);
}

CORBA_octet MOTOR_getPosition(
	MOTOR o,
	CORBA_Environment* ev)
{
	if (turnDirection == 0)
		return mtGetHallPositionFWD();
	else
		return mtGetHallPositionBWD();
}

void MOTOR_turnOn(
	MOTOR o,
	CORBA_long ID,
	CORBA_Environment* ev)
{
	LED_reg |= (1 << ID);

	switch(ID) {
	case 0:
		LED6_ON; 
		break;
	case 1:
		LED7_ON; 
		break;
	case 2:
		LED8_ON; 
		break;
	case 3:
		LED9_ON; 
		break;
	case 4:
		LED10_ON; 
		break;
	case 5:
		LED11_ON; 
		break;
	case 6:
		LED12_ON; 
		break;
	case 7:
		LED13_ON; 
		break;
	case 8:
		LED14_ON; 
		break;
	}
}

void MOTOR_turnOff(
	MOTOR o,
	CORBA_long ID,
	CORBA_Environment* ev)
{
	LED_reg &= (0xfffffffe << ID);

	switch(ID) {
	case 0:
		LED6_OFF; 
		break;
	case 1:
		LED7_OFF; 
		break;
	case 2:
		LED8_OFF; 
		break;
	case 3:
		LED9_OFF; 
		break;
	case 4:
		LED10_OFF; 
		break;
	case 5:
		LED11_OFF; 
		break;
	case 6:
		LED12_OFF; 
		break;
	case 7:
		LED13_OFF; 
		break;
	case 8:
		LED14_OFF; 
		break;
	}
}

CORBA_boolean MOTOR_getState(
	MOTOR o,
	CORBA_long ID,
	CORBA_Environment* ev)
{
	return (LED_reg & (1 << ID)) != 0;
}
