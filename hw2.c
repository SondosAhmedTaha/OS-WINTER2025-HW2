#include <linux/kernel.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}


asmlinkage long sys_set_sec(int sword, int midnight, int clamp){
    // Validate inputs (only 0 or 1 are allowed for sword, midnight, and clamp)
    if (!(sword == 0 || sword == 1) ||
        !(midnight == 0 || midnight == 1) ||
        !(clamp == 0 || clamp == 1)) {
        return -EINVAL; // Invalid arguments
    }

    // Check if the calling process has the necessary permissions (root privileges)
    if (!capable(CAP_SYS_ADMIN)) {
        return -EPERM; // Permission denied
    }

    // Get the current task's task_struct
    struct task_struct *curr_taskStruct = current;

    // Initialize clearances to 0 before setting new values
    curr_taskStruct->clearances = 0;

    // Set or clear the corresponding bits in the `clearances` field
    if (sword == 1) {
        curr_taskStruct->clearances |= 0x02; // Set the 2nd bit (Sword clearance)
    }

    if (midnight == 1) {
        curr_taskStruct->clearances |= 0x04; // Set the 3rd bit (Midnight clearance)
    }

    if (clamp == 1) {
        curr_taskStruct->clearances |= 0x08; // Set the 4th bit (Clamp clearance)
    }

    return 0; // Success
}



asmlinkage long sys_get_sec(char clr){
    struct task_struct *curr_taskStruct = current;

    if(clr == 's'){
        if((curr_taskStruct->clearances & 0x02) != 0)
            return 1; // Sword clearance is present
        return 0; // Sword clearance is absent
    }

    if(clr == 'm'){
        if((curr_taskStruct->clearances & 0x04) != 0)
            return 1; // Midnight clearance is present
        return 0; // Midnight clearance is absent
    }

    if(clr == 'c'){
        if((curr_taskStruct->clearances & 0x08) != 0)
            return 1; // Clamp clearance is present
        return 0; // Clamp clearance is absent
    }

    return -EINVAL; // Invalid input character
}


asmlinkage long sys_check_sec(pid_t pid, char clr){
	
	
	
}

asmlinkage long sys_set_sec_branch(int height, char clr){
	
	
}




