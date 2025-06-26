#include <asm/asm.h>
#include <mmu.h>

.text
.globl	_start
/* Lab 1 Key Code "enter-kernel" */
_start:
	/* initialize */
	#if !defined(LAB) || LAB >= 3
	la      a0, exc_gen_entry
	csrwr   a0, csr_eentry
	#endif
	#if !defined(LAB) || LAB >= 2
	la      a1, tlb_miss_entry
	li.w    a0, 0x1fffffff
	and     a1, a1, a0
	csrwr   a1, csr_tlbrentry
	#endif

	/* disable interrupts and enable memory map */
	li.w    a0, 0xb0     // PLV = 0, ID = 0, DA = 0, PG = 1, DATF = DATM = 01
	csrwr   a0, csr_crmd

	/* set page size 2^12B=4KB for tlb */
	li.w    a0, 0x0c000000
	csrwr   a0, csr_tlbidx

	/* set up the kernel stack */
	li.w    sp, KSTACKTOP

	/* clear bss segment for C env. */
	la      t0, .bss
	la      t1, bss_end
bss_loop:
	beq     t0, t1, bss_loop_end
	st.b    zero, t0, 0
	addi.w  t0, t0, 1
	b       bss_loop
bss_loop_end:
	// Configure KSEG0/1
	li.w a0, 0x80000001
	csrwr a0, csr_dmw0
	li.w a0, 0xa0000011
	csrwr a0, csr_dmw1

	/* jump to la32r_init */
	b       la32r_init
	nop
