 Manipulation in Code
To manipulate and query the clearances field, use bitwise operations:

Set Sword Clearance: task->clearances |= 0x02;
Unset Sword Clearance: task->clearances &= ~0x02;
Check Sword Clearance: (task->clearances & 0x02) != 0
Repeat similar operations for Midnight Clearance (0x04) and Clamp Clearance (0x08).



**/////////////////////////
Bit 1 (always 0 for alignment).
Bit 2 (1 for Sword clearance, 0 otherwise).
Bit 3 (1 for Midnight clearance, 0 otherwise).
Bit 4 (1 for Clamp clearance, 0 otherwise).**
