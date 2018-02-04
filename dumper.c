
#include "common.h"
#include "serial.h"

void reverse_str(char *str, int len)
{
 int start =0;
 int end = len -1;
 char temp = '0';
 while(start < len)
 {
   temp = str[start];
   str[start] = str[end];
   str[end] = temp;
   start++;
   end--;
 } 

}

char* itoa_new(int num, char *str, int base)
{
int i = 0;
int isNeg = 0;
int rem = 0;
if(num == 0)
{
  str[i++] = '0';
  str[i]='\0';
  return str;
}

if(num < 0 && base == 10)
{
  isNeg = 1;
  num = -num;
}

while(num != 0)
{
  rem = num % base;
  str[i++]= (rem > 9)?(rem -10) + 'a' : rem + '0';
  num = num/base;

}

if(isNeg)
  str[i++] = '-';

str[i] = '\0';  

reverse_str(str, i);
return str;
}


void dump_regs(void)
{
  
  unsigned long int reg_val;
  char hex_str[50];
  __asm__ __volatile__("mov %0, r0":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r0 = 0x"); serial_tx(hex_str); serial_tx("\r\n");
 
  __asm__ __volatile__("mov %0, r1":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r1 = 0x"); serial_tx(hex_str); serial_tx("\r\n");

 __asm__ __volatile__("mov %0, r2":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r2 = 0x"); serial_tx(hex_str); serial_tx("\r\n");

  __asm__ __volatile__("mov %0, r3":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r3 = 0x"); serial_tx(hex_str); serial_tx("\r\n");

__asm__ __volatile__("mov %0, r4":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r4 = 0x"); serial_tx(hex_str); serial_tx("\r\n");


  __asm__ __volatile__("mov %0, r5":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r5 = 0x"); serial_tx(hex_str); serial_tx("\r\n");


  __asm__ __volatile__("mov %0, r6":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r6 = 0x"); serial_tx(hex_str); serial_tx("\r\n");


  __asm__ __volatile__("mov %0, r7":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r7 = 0x"); serial_tx(hex_str); serial_tx("\r\n");


  __asm__ __volatile__("mov %0, r8":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r8 = 0x"); serial_tx(hex_str); serial_tx("\r\n");


  __asm__ __volatile__("mov %0, r9":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r9 = 0x"); serial_tx(hex_str); serial_tx("\r\n");


  __asm__ __volatile__("mov %0, r10":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r10 = 0x"); serial_tx(hex_str); serial_tx("\r\n");

  __asm__ __volatile__("mov %0, r11":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r11 = 0x"); serial_tx(hex_str); serial_tx("\r\n");



  __asm__ __volatile__("mov %0, r12":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r12 = 0x"); serial_tx(hex_str); serial_tx("\r\n");


  __asm__ __volatile__("mov %0, r13":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r13 = 0x"); serial_tx(hex_str); serial_tx("\r\n");

  __asm__ __volatile__("mov %0, r14":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r14 = 0x"); serial_tx(hex_str); serial_tx("\r\n");

 __asm__ __volatile__("mov %0, r15":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("r15 = 0x"); serial_tx(hex_str); serial_tx("\r\n");

__asm__ __volatile__("mov %0, sp":"=r" (reg_val) : : );
  set_string_zero(hex_str,50);
  get_hex_string( reg_val , hex_str, 50);
  serial_tx("sp = 0x"); serial_tx(hex_str); serial_tx("\r\n");


}



void dump_loc(unsigned long int address, unsigned int size)
{
  int i = 0, k = 4;
  //void * loc = address;
  int val = 0;
  char hex_str[50];
  int *p = NULL; 
  char num_buff[10] = {0};
    while(i< (size))
    {
      address = address+4;
      p = (int *) (address);
      val = *p;
      itoa_new(i, num_buff, 10);
      serial_tx("[");
      serial_tx("(i= ");
      serial_tx(num_buff);
      serial_tx(") ");
      set_string_zero(hex_str, 50);
      get_hex_string(val, hex_str, 50); 
      serial_tx(hex_str);
      serial_tx("] ");
      serial_tx("\r\t");
      i++;  
   }
 
}



void dumper(int location, int size)
{
  dump_regs();
  dump_loc(location,size);

}
