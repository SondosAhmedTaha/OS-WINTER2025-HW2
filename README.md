 Manipulation in Code
To manipulate and query the clearances field, use bitwise operations:

Set Sword Clearance: task->clearances |= 0x02;
Unset Sword Clearance: task->clearances &= ~0x02;
Check Sword Clearance: (task->clearances & 0x02) != 0
Repeat similar operations for Midnight Clearance (0x04) and Clamp Clearance (0x08).

