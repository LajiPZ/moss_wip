#include <asm/asm.h>
#include <env.h>
#include <pmap.h>
#include <printk.h>
#include <sched.h>
#include <trap.h>
#include <sd.h>

/*
 * Note:
 * When build with 'make test lab=?_?', we will replace your 'la32r_init' with a generated one from
 * 'tests/lab?_?'.
 */

#ifdef MOS_INIT_OVERRIDDEN
#include <generated/init_override.h>
#else

void la32r_init() {
	printk("init.c:\tla32r_init() is called\n");
	// lab2:
	la32r_detect_memory();
	la32r_vm_init();
	page_init();

	// lab3:
	env_init();
	sd_init();
	// lab3:
	// ENV_CREATE_PRIORITY(user_bare_loop, 1);
	// ENV_CREATE_PRIORITY(user_bare_loop, 2);

	// lab4:
	// ENV_CREATE(user_tltest);
	 ENV_CREATE(user_fktest);
	// ENV_CREATE(user_pingpong);

	// lab6:
//	ENV_CREATE(user_icode); // This must be the first env!

	// lab5:
	// ENV_CREATE(user_fstest);
//	ENV_CREATE(fs_serv); // This must be the second env!
	// ENV_CREATE(user_devtst);

	// lab3:
	schedule(0);
	halt();
}

#endif
