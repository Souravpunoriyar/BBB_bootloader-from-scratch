#include "common.h"
#include "serial.h"

int load_kernel()
{
  char str[32] ;
  serial_tx("intiating Serial Transfer \r\n");
  serial_rx(str, 32);
  serial_tx("You entered: ");
  serial_tx(str);
  serial_tx("\r\n");
  serial_tx("Kernel Loading done\r\n");
  return 0;
}
