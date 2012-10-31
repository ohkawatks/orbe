
#include	"Orbe.h"
#include	"OrbeConfig.h"
#include	"LCD.h"

extern void LCD_init();

int main(int argc, char *argv[])
{
	OrbeConfig_setup();
	LCD_init();
	
	struct corba_object LCD_value;
	LCD object = &LCD_value;
	
	Orbe_setObjectKey(object, "test_object");
	Orbe_startServer();
	return 0;
}
