        .data
msg:    .asciz  "*****Input\ ID*****\n"
input1: .asciz  "**\ Please\ Enter\ Member\ 1\ ID:**\n"
input2: .asciz  "**\ Please\ Enter\ Member\ 2\ ID:**\n"
input3: .asciz  "**\ Please\ Enter\ Member\ 3\ ID:**\n"
entercommand:.asciz  "**\ Please\ Enter\ Command\ **\n"
command:.asciz  "\0"
str3:   .asciz  "*****Print\ Team\ Member\ ID\ and\ ID\ Summation*****\n"
str4:   .asciz  "ID\ Summation\ =\ "
end:    .asciz  "*****End\ Print*****\n"
str:    .asciz  "\n"
str1:   .asciz  "%d"
str2:   .asciz  "%s"
n1:     .word   0
n2:     .word   0
n3:     .word   0
total:  .word   0
p:      .word   112

        .text
        .globl  id
        .globl  n1
        .globl  n2
        .globl  n3
        .globl  str4
        .globl  total
        .globl  str1

id:     stmfd   sp!,{lr}
        ldr     r0, =msg
        bl      printf

        ldr     r0, =input1
        bl      printf      @ print "** Please Enter Member 1 ID:**\n"
        ldr     r0, =str1
        ldr     r1, =n1
        bl      scanf       @ input student1 ID

        ldr     r0, =input2
        bl      printf      @ print "** Please Enter Member 2 ID:**\n"
        ldr     r0, =str1
        ldr     r1, =n2
        bl      scanf       @ input student2 ID

        ldr     r0, =input3
        bl      printf      @ print "** Please Enter Member 3 ID:**\n"
        ldr     r0, =str1
        ldr     r1, =n3
        bl      scanf       @ input student3 ID

        ldr     r1, =n1
        ldr     r1, [r1]    @ r1 = id1
        ldr     r2, =n2
        ldr     r2, [r2]    @ r2 = id2
        add     r1, r1, r2  @ r1 = id1 + id2
        ldr     r2, =n3
        ldr     r2, [r2]    @ r2 = id3
        add     r1, r1, r2  @ r1 = id1 + id2 + id3
        ldr     r3, =total
        str     r1, [r3]    @----------2-------------

        ldr     r0, =entercommand
        bl      printf      @ print "** Please Enter Command **\n"
        ldr     r0, =str2
        ldr     r1, =command
        bl      scanf       @ input command

        ldr     r0, =p
        ldr     r1, =command
        ldrb    r2, [r1, #0]! @ ---------3------------
        ldrb    r3, [r0, #0]
        cmp     r2, r3, ror #0 @ ---------22----------
        bne     else        @ if command=p print, else break


        ldr     r0, =str3
        bl      printf      @ print "*****Print Team Member ID and ID Summation*****\n"
        ldr     r0, =str1
        ldr     r6, =n1
        ldr     r1, [r6], r0 @ ----------1-------------
        bl      printf      @ print id1
        ldr     r0, =str
        bl      printf

        ldr     r0, =str1
        ldr     r5, =n2
        ldr     r1, [r5], #3 @ ----------4-------------
        bl      printf      @ print id2
        ldr     r0, =str
        bl      printf

        ldr     r0, =str1
        ldr     r1, =n3
        ldr     r1, [r1]
        bl      printf      @ print id3

        ldr     r0, =str
        bl      printf
        ldr     r0, =str
        bl      printf
        ldr     r0, =str4   @ print "ID Summation = "
        bl      printf
        ldr     r0, =str1
        ldr     r5, =total
        ldr     r1, [r5]
        bl      printf      @ print the sum of id1,id2,id3
        ldr     r0, =str
        bl      printf

        ldr     r0, =end
        bl      printf      @ print "*****End Print*****\n"
        ldr     r0, =str1

        mov     r0, #0      @ -----------33------------
        ldmfd   sp!,{lr}
        mov     pc, lr      @ -----------11------------
else:   ldmfd   sp!,{lr}
        mov     pc, lr
