#include "common.h"
#include "serial.h"


typedef void (*execute_kernel)();
execute_kernel Run_Kernel;

int load_kernel()
{
  serial_tx("intiating Serial Transfer \r\n");
  //serial_tx("Enter File size to transfer Transfer \r\n"); TODO
  //serial_rx(str, 32);
  serial_rx_image_xfer(84);
  serial_tx("\r\n");
  serial_tx("Kernel Loading done\r\n");
  dumper(0x80000000,20);
  Run_Kernel = (execute_kernel)BASE_ADRESS;
  Run_Kernel(); 
  return 0;
}
