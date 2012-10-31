
#include	"Orbe.h"
#include	"OrbeConfig.h"
#include	"LED.h"


int main(int argc, char *argv[])
{
	OrbeConfig_setup();
	
	struct corba_object LED_value;
	LED object = &LED_value;
	
	Orbe_setObjectKey(object, "test_object");
	Orbe_startServer();
	return 0;
}
