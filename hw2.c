#include <linux/kernel.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}


asmlinkage long sys_set_sec(int sword, int midnight, int clamp){
}

asmlinkage long sys_get_sec(char clr){
}

asmlinkage long sys_check_sec(pid_t pid, char clr){
	
	
	
}

asmlinkage long sys_set_sec_branch(int height, char clr){
	
	
}




