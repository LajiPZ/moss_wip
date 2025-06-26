#include <asm/asm.h>
#include <mmu.h>
#include <trap.h>

// clang-format off
.macro SAVE_ALL
	csrrd   x, csr_prmd
	andi    x, x, 0x3
	beq     x, zero, 1f
	move    x, sp
	li.w    sp, (KSTACKTOP - TF_SIZE)
	st.w    x, sp, TF_REG3
	b       2f
1:
	li.w    x, TF_SIZE
	st.w    sp, sp, TF_REG3 - TF_SIZE
	sub.w   sp, sp, x
2:
	csrrd   x, csr_prmd
	st.w    x, sp, TF_CRMD
	csrrd   x, csr_estat
	st.w    x, sp, TF_ESTAT
	csrrd   x, csr_era
	st.w    x, sp, TF_ERA
	csrrd   x, csr_badv
	st.w    x, sp, TF_BADV
	csrrd   x, csr_ecfg
	st.w    x, sp, TF_ECFG
	st.w    $r0, sp, TF_REG0
	st.w    $r1, sp, TF_REG1
	st.w    $r2, sp, TF_REG2
	st.w    $r4, sp, TF_REG4
	st.w    $r5, sp, TF_REG5
	st.w    $r6, sp, TF_REG6
	st.w    $r7, sp, TF_REG7
	st.w    $r8, sp, TF_REG8
	st.w    $r9, sp, TF_REG9
	st.w    $r10, sp, TF_REG10
	st.w    $r11, sp, TF_REG11
	st.w    $r12, sp, TF_REG12
	st.w    $r13, sp, TF_REG13
	st.w    $r14, sp, TF_REG14
	st.w    $r15, sp, TF_REG15
	st.w    $r16, sp, TF_REG16
	st.w    $r17, sp, TF_REG17
	st.w    $r18, sp, TF_REG18
	st.w    $r19, sp, TF_REG19
	st.w    $r20, sp, TF_REG20
	st.w    $r21, sp, TF_REG21
	st.w    $r22, sp, TF_REG22
	st.w    $r23, sp, TF_REG23
	st.w    $r24, sp, TF_REG24
	st.w    $r25, sp, TF_REG25
	st.w    $r26, sp, TF_REG26
	st.w    $r27, sp, TF_REG27
	st.w    $r28, sp, TF_REG28
	st.w    $r29, sp, TF_REG29
	st.w    $r30, sp, TF_REG30
	st.w    $r31, sp, TF_REG31
.endm
/*
 * Note that we restore the IE flags from stack. This means
 * that a modified IE mask will be nullified.
 */
.macro RESTORE_SOME
	ld.w    v0, sp, TF_CRMD
	csrwr   v0, csr_prmd
	ld.w    v1, sp, TF_ERA
	csrwr   v1, csr_era
	ld.w    v1, sp, TF_ECFG
	csrwr   v1, csr_ecfg
	ld.w    v1, sp, TF_BADV
	csrwr   v1, csr_badv
	ld.w    $r31, sp, TF_REG31
	ld.w    $r30, sp, TF_REG30
	ld.w    $r29, sp, TF_REG29
	ld.w    $r28, sp, TF_REG28
	ld.w    $r27, sp, TF_REG27
	ld.w    $r26, sp, TF_REG26
	ld.w    $r25, sp, TF_REG25
	ld.w    $r24, sp, TF_REG24
	ld.w    $r23, sp, TF_REG23
	ld.w    $r22, sp, TF_REG22
	ld.w    $r21, sp, TF_REG21
	ld.w    $r20, sp, TF_REG20
	ld.w    $r19, sp, TF_REG19
	ld.w    $r18, sp, TF_REG18
	ld.w    $r17, sp, TF_REG17
	ld.w    $r16, sp, TF_REG16
	ld.w    $r15, sp, TF_REG15
	ld.w    $r14, sp, TF_REG14
	ld.w    $r13, sp, TF_REG13
	ld.w    $r12, sp, TF_REG12
	ld.w    $r11, sp, TF_REG11
	ld.w    $r10, sp, TF_REG10
	ld.w    $r9, sp, TF_REG9
	ld.w    $r8, sp, TF_REG8
	ld.w    $r7, sp, TF_REG7
	ld.w    $r6, sp, TF_REG6
	ld.w    $r5, sp, TF_REG5
	ld.w    $r4, sp, TF_REG4
	ld.w    $r2, sp, TF_REG2
	ld.w    $r1, sp, TF_REG1
.endm
