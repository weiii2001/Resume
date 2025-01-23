        .data
msg:    .asciz  "*****Print\ Name*****\n"
team:   .asciz  "Team\ 52\n"
name1:  .asciz  "Ryan\ Hsu\n"
name2:  .asciz  "Stan\ Lu\n"
name3:  .asciz  "Chain\ Lee\n"
end:    .asciz  "*****End\ Print*****\n"
        .text
        .globl  name
        .globl  team
        .globl  name1
        .globl  name2
        .globl  name3

name:   stmfd   sp!,{lr}
        ldrvc   r0, =msg    @ -----------one----------
        bl      printf      @ print "*****Print Name*****\n"
        ldr     r0, =team
        bl      printf      @ print "Team 52\n"
        ldr     r0, =name1
        bl      printf
        ldrpl   r0, =name2  @ -----------two----------
        bl      printf
        ldreq   r0, =name3  @ ----------three---------
        bl      printf
        ldr     r0, =end
        bl      printf

        subs    lr, pc, sp  @ r14 = r15 - r13
        adds    pc, sp, lr  @ r15 = r13 + r14
        mov     r0, #0
        ldmfd   sp!,{lr}
        mov     pc, lr
