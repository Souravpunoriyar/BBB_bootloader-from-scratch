#include <stdbool.h>
#include "led.h"
#include "serial.h"
#include "dumper.h"

//unsigned long int tlb_l1_base = 0x402FCC00;

typedef unsigned int  uint;
static uint section_table[ 0x1000 ] __attribute__(( aligned( 0x4000 ) ));


enum {
	// device type (asynchronous / "posted" writes)
	type_device	= 1 << 2 | 0 << 12 | 0 << 14,

	// device type with synchronous ("non-posted") writes
	type_sync	= 0 << 2 | 0 << 12 | 0 << 14,
	// ARM calls this "strongly ordered", although in ARMv8 they changed
	// terminology to "Device-nGnRnE" ... not sure that's an improvement.
};

// normal memory type with specified cache policy
enum CachePolicy {
	nc = 0,  // non-cacheable
	wt = 2,  // write-through, read-allocate, no write-allocate
	wb = 3,  // write-back,    read-allocate, no write-allocate
	wa = 1,  // write-back,    read-allocate, write-allocate
};
static inline uint type_normal( enum CachePolicy l1, enum CachePolicy l2 )
{
	bool shared = false;
	return l1 << 2 | l2 << 12 | 1 << 14 | shared << 16;
}

//-------------- Permissions -------------------------------------------------//

enum {
	sect_access	= 1 << 10,
	sect_user	= 1 << 11,
	sect_readonly	= 1 << 15,
	sect_noexec	= 1 <<  4,

	// section permissions:
	______	= 0,
	rwx___	= sect_access,
	rwxrwx	= sect_access | sect_user,

	r_x___	= rwx___ | sect_readonly,
	r_xr_x	= rwxrwx | sect_readonly,

	r_____	= r_x___ | sect_noexec,
	rw____	= rwx___ | sect_noexec,
	r__r__	= r_xr_x | sect_noexec,
	rw_rw_	= rwxrwx | sect_noexec,

	// these are only valid if not using simplified permissions model:
	rwxr_x	= sect_user,
	rw_r__	= rwxr_x | sect_noexec,
};

//-------------- Section translation primitives ------------------------------//
//
// Beware that these routines do not perform any sanity-checking at all.
//
// Beware also that these routines are just meant to initialize the section
// table *before* enabling the MMU.  Making changes while the MMU is enabled is
// of course possible, but the procedure is more involved and delicate.
//
// Translation via page tables not supported by this file (currently).

enum {
	sect_fault	= 0,  // translation fault
	sect_pgtbl	= 1,  // translation via page table
	sect_map	= 2,  // section translation

	// section translation descriptor flags
	sect_super	= 1 << 18,  // supersection (16 MB)
	sect_proc	= 1 << 17,  // process-specific (non-global)
};



static void map_section( uint desc ) {
	desc |= sect_map;
	section_table[ desc >> 20 ] = desc;
}

// create 16 MB flat mapping
static void map_supersection( uint desc ) {
	uint i = 0;
	desc |= sect_map | sect_super;
	for( i = 0; i < 16; i++ )
		section_table[ ( desc >> 20 ) + i ] = desc;
}

// get L2 cache policy (assumes the address is mapped as normal memory)
static enum CachePolicy get_L2_policy( uint va ) {
	return (enum CachePolicy)( section_table[ va >> 20 ] >> 12 & 3 );
}

//-------------- Init translation table (am335x) -----------------------------//

void init_translation()
{
	uint addr = 0;
	// peripherals
	for( addr = 0x44000000; addr != 0x57000000; addr += 0x01000000 )
		map_supersection( addr | rw_rw_ | type_device );

	// external RAM
//	for( uint addr = 0x80000000; addr != 0xc0000000; addr += 0x01000000 )
//		map_supersection( addr | rwxrwx | type_normal( wb, wa ) );

	// local memories
	map_section( 0x40000000 | r_xr_x | type_normal( wt, nc ) ); // mpuss rom
	map_section( 0x40200000 | rwxrwx | type_normal( wt, wt ) ); // mpuss ram
	map_section( 0x40300000 | rwxrwx | type_normal( wt, wt ) ); // ocmc ram

	asm( "dsb" ::: "memory" );

	// configure the section table address and cache policy into the MMU
	uint tr_base = (uint) section_table;
	tr_base |= get_L2_policy( tr_base ) << 3;
	asm( "mcr p15, 0, %0, c2, c0, 0" :: "r"( tr_base ) );
}


void mmu_init(void)
{
//unsigned long int *sect_dump;
init_translation();
//sect_dump = section_table
char hex_str[50]={0};
get_hex_string((int)section_table, hex_str, 50);
serial_tx(hex_str);
serial_tx("\r\t");
led_on();
dumper((int)section_table, 4096);
#if 0
asm(
	"MRRC P15, 1, R0, R1, C15\n\t" // Read CPUECTLR.
	"ORR R0, R0, #(0x1 << 6)\n\t" // Set SMPEN.
	"MCRR P15, 1, R0, R1, C15\n\t" // Write CPUECTLR.
// Enable caches and the MMU.
	"MRC P15, 0, R1, C1, C0, 0\n\t" // Read SCTLR.
	"ORR R1, R1, #(0x1 << 2)\n\t" // The C bit (data cache).
	"ORR R1, R1, #(0x1 << 12)\n\t" // The I bit (instruction cache).
	"ORR R1, R1, #0x1\n\t" // The M bit (MMU).
	"MCR P15, 0, R1, C1, C0, 0\n\t" // Write SCTLR.
	"DSB\n\t"
	"ISB\n\t"	
		);
#endif
led_off();

}




#if 0
void mmu_init(void)
{

asm(	"MOV R0, #0\n\t" // Use short descriptor.
	"MCR P15, 0, R0, C2, C0, 2\n\t" // Base address is 16KB aligned.
				 // Perform translation table walk for TTBR0
	"ldr r0, =0x40300000\n\t"
	"MOV R1, #0x2B\n\t" // The translation table walk is normal, inner
	"ORR R1, R0, R1\n\t" // and outer cacheable, WB WA, and inner
	"mcr p15, 0, r1, c2, c0, 0\n\t"
	"ldr r4, =0x00100000\n\t"
  	"ldr r2, =0x40010c02\n\t"
	"ldr r3, =1000\n\t"
	".loop_mmu:\n\t"
	"str r2, [r0], #4\n\t" /*0th 0x40000000 so for 80th index for 0x48000000*/
	"add r2, r2, r4\n\t"   /*GPIO should be at 0x080*/
	"subs r3, #1\n\t"
	"bne .loop_mmu\n\t"

	"MRRC P15, 1, R0, R1, C15\n\t" // Read CPUECTLR.
	"ORR R0, R0, #(0x1 << 6)\n\t" // Set SMPEN.
	"MCRR P15, 1, R0, R1, C15\n\t" // Write CPUECTLR.
// Enable caches and the MMU.
	"MRC P15, 0, R1, C1, C0, 0\n\t" // Read SCTLR.
	"ORR R1, R1, #(0x1 << 2)\n\t" // The C bit (data cache).
	"ORR R1, R1, #(0x1 << 12)\n\t" // The I bit (instruction cache).
	"ORR R1, R1, #0x1\n\t" // The M bit (MMU).
	"MCR P15, 0, R1, C1, C0, 0\n\t" // Write SCTLR.
	"DSB\n\t"
	"ISB\n\t"

   );
}


void add_pagetable_entries(void)
{

asm(
	"ldr r4, =0x00100000\n\t"
  	"ldr r2, =0x40010c02\n\t"
	"ldr r3, =1000\n\t"
	".loop_mmu:\n\t"
	"str r2, [r0], #4\n\t" /*0th 0x40000000 so for 80th index for 0x48000000*/
	"add r2, r2, r4\n\t"   /*GPIO should be at 0x080*/
	"subs r3, #1\n\t"
	"bne .loop_mmu\n\t"
   );


}
#endif
