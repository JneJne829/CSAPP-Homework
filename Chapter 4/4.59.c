/* 
 * 
 * 4.47 is the better one
 * loop part in 4.47
 * 
 * L4:
 *   mrmovq 8(%rax), %r9
 *   mrmovq (%rax), %r10
 *   rrmovq %r9, %r8
 *   subq %r10, %r8
 *   jge L3
 *   rmmovq %r10, 8(%rax)
 *   rmmovq %r9, (%rax)
 * 
 * 50% jge is right, run 5 instructions; 50% jge is wrong, run 7 instructions and 2 nop bubble. so Cycles Per Loop is 50% * 5 + (7 + 2) * 50% = 7
 * 
 * loop part in 4.48
 * 
 * L4:
 *   mrmovq 8(%rax), %r9
 *   mrmovq (%rax), %r10
 *   rrmovq %r9, %r8
 *   subq %r10, %r8
 *   cmovl %r9, %r11
 *   cmovl %r10, %r9
 *   cmovl %r11, %r10
 *   rmmovq %r9, 8(%rax)
 *   rmmovq %r10, (%rax)
 * 
 * Cycles Per Loop is 9
 * 
 * loop part in 4.49
 * 
 * L4:
 *   mrmovq 8(%rax), %r9
 *   mrmovq (%rax), %r10
 *   rrmovq %r9, %r8
 *   rrmovq %r10, %r11
 *   xorq %r9, %r10
 *   subq %r11, %r8
 *   cmovge %r11, %r9
 *   xorq %r10, %r9
 *   xorq %r9, %r10
 *   rmmovq %r9, 8(%rax)
 *   rmmovq %r10, (%rax)
 * 
 * Cycles Per Loop is 11
 * 
 */