
#include "common.h"
#include "interrupt.h"
#include "bbb.h"
#include "arm.h"
#include "serial.h"
#include "core_init.h"

/*to enable debugs #define CORE_INIT_DEBUG*/
#define LED_INIT_DEBUG

#ifdef LED_INIT_DEBUG
  #define SERIAL_DEBUG(a) serial_tx(a)
#else
  #define SERIAL_DEBUG(a)
#endif

void led_on(void)
{
 

     SERIAL_DEBUG("glow_led \r\n ");
      asm(
	 "ldr r0, =0x44e000AC\n\t"
	 "ldr r1, =0x40002\n\t" 
         "str r1, [r0]\n\t"
     );
     
     asm(
      "ldr r0, =0x4804C134\n\t"
      "ldr r1, [r0]\n\t"
      "bic r1, r1, #(1<<21)\n\t"
      "str r1, [r0]\n\t"
     );

    asm(
	"ldr r0, =0x4804C194\n\t"
	"ldr r2, [r0]\n\t"
	"orr r2, r2, 0x200000\n\t"
	"str r2, [r0]\n\t"
	);
     	  
   
}

void led_off(void)
{

SERIAL_DEBUG("switch off led \r\n");

asm(
	"ldr r0, =0x4804C190\n\t"
	"ldr r2, [r0]\n\t"
	"orr r2, r0, #0x200000\n\t"
	"str r2, [r0]\n\t"
    );	
}
