#include <linux/kernel.h>

#include <linux/errno.h>       // For error codes: EINVAL, EPERM, ESRCH
#include <linux/sched.h>       // For current, task_struct
#include <linux/pid.h>         // For pid_task, find_vpid
#include <linux/capability.h>  // For capable()

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}


asmlinkage long sys_set_sec(int sword, int midnight, int clamp){
	
	// Get the current task's task_struct
    struct task_struct *curr_taskStruct = current;
    
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


asmlinkage long sys_check_sec(pid_t pid, char clr) {
	
    unsigned char mask = 0x00;
    // Retrieve the target process by PID
    struct task_struct *the_process = pid_task(find_vpid(pid), PIDTYPE_PID);

    // Determine the clearance bitmask based on `clr`
    switch (clr) {
        case 's': mask = 0x02; break; // sword
        case 'm': mask = 0x04; break; // midnight
        case 'c': mask = 0x08; break; // clamps
        default: return -EINVAL; // Invalid clearance argument
    }

    // Check if the target process exists
    if (!the_process) {
        return -ESRCH; // Target process does not exist
    }

    // Check if the calling process has the required clearance
    if (!(current->clearances & mask)) {
        return -EPERM; // Permission denied
    }

    // Check if the target process has the required clearance
    if (the_process->clearances & mask) {
        return 1; // Target process has the clearance
    }
    return 0; // Target process does not have the clearance
}

asmlinkage long sys_set_sec_branch(int height, char clr){

	unsigned char mask = 0x00;
	struct task_struct* current_parent = current->real_parent;
    int counter = 0;
    int i = 0;
	
    //check input
    if (height <= 0) { return -EINVAL; }
    
    // Determine the clearance bitmask based on `clr`
    switch (clr) {
        case 's': mask = 0x02; break; // sword
        case 'm': mask = 0x04; break; // midnight
        case 'c': mask = 0x08; break; // clamps
        default: return -EINVAL; // Invalid clearance argument
    }

    // Check if the calling process has the required clearance
    if (!(current->clearances & mask)) {
        return -EPERM; // Permission denied
    }

    // Traverse up the parent hierarchy up to the specified height
    for (i = 0; i < height; i++) {
        // Check if the parent already has the clearance
        if (!(current_parent->clearances & mask)) {
            // Add the clearance
            current_parent->clearances |= mask;
            counter++;
        }

        // Move to the next parent in the hierarchy
        current_parent = current_parent->real_parent;
    }

    return counter;
}
