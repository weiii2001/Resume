        .data
num1:   .word   1500
num3:   .word   4000000

        .text
        .globl  drawJuliaSet

drawJuliaSet:
        mov     r4, sp          @-------------------1-----------------
        stmfd   sp!, {r4-r10,fp,lr}
        sub     sp, sp, #16     @ tmp, color, zx, zy
        mov     r4, r0          @ r4 = r0 = cX
        mov     r5, r1          @ r5 = r1 = cY
        mov     r6, r2          @ r6 = r2 = width
        mov     r7, r3          @ r7 = r3 = height
        mov     r8, #0          @ r8 = i = 0--------------2-----------
        mov     r9, #0          @ r9 = x = 0
        mov     r10, #0         @ r10 = y = 0

for1:   cmp     r9, r6
        bge     end             @ if x >= width, end

for2:   cmp     r10, r7
        bge     backtofor1      @ if y >= height, back to for1
        movlt   r1, r6          @ -----------------one----------------
        mov     r1, r1, lsr #1  @ r1 = width>>1-----------3-----------
        mov     r2, r9, lsl #0  @ r2 = x--------------4---------------
        sub     r2, r2, r1      @ r2 = x - (width>>1)
        ldr     r0, =num1
        ldr     r0, [r0]        @ r0 = 1500
        mul     r0, r0, r2      @ r0 = 1500 * (x-(width>>1))
        bl      __aeabi_idiv    @ r0 = r0/r1
        str     r0, [sp, #4]    @ zx = 1500 * (x-(width>>1))

        mov     r1, r7
        mov     r1, r1, lsr #1  @ r1 = height>>1
        mov     r2, r10         @ r2 = y
        sub     r2, r2, r1      @ r2 = y - (height>>1)
        mov     r0, #1000       @ r0 = 1000
        mul     r0, r0, r2      @ r0 = 1000 * (y-(height>>1))
        bl      __aeabi_idiv    @ r0 = r0/r1
        str     r0, [sp]        @ zy = 1000 * (y-(height>>1))

        mov     r8, #255        @ i = 255

while:  ldr     r0, [sp, #4]    @ r0 = zx
        mul     r0, r0, r0      @ r0 = zx*zx
        ldr     r1, [sp]        @ r1 = zy
        mul     r1, r1, r1      @ r1 = zy*zy
        add     r0, r0, r1      @ r0 = zx*zx + zy*zy
        ldr     r1, =num3
        ldr     r1, [r1]        @ r1 = 4000000
        cmp     r0, r1
        bge     endwhile        @ zx*zx + zy*zy >= 4000000
        mov     r1, #0
        cmp     r8, r1
        ble     endwhile        @ i <= 0

        ldrgt   r0, [sp, #4]    @ r0 = zx-------------two-------------
        mul     r0, r0, r0      @ r0 = zx*zx
        ldr     r1, [sp]        @ r1 = zy
        mul     r1, r1, r1      @ r1 = zy*zy
        sub     r0, r0, r1      @ r0 = zx*zx - zy*zy
        mov     r1, #1000       @ r1 = 1000
        bl      __aeabi_idiv    @ r0 = (zx*zx - zy*zy)/1000

        mov     r1, r4          @ r1 = cX
        add     r1, r0, r1      @ r1 = (zx*zx - zy*zy)/1000 + cX
        str     r1, [sp, #12]   @ tmp = (zx*zx - zy*zy)/1000 + cX

        ldr     r0, [sp, #4]
        ldr     r1, [sp]
        mul     r0, r0, r1      @ r0 = zx*zy
        add     r0, r0, r0      @ r0 = 2*zx*zy
        mov     r1, #1000       @ r1 = 1000
        bl      __aeabi_idiv    @ r0 = 2*zx*zy/1000
        mov     r1, r5
        add     r0, r0, r1      @ r0 = 2*zx*zy/1000 + cY
        str     r0, [sp]        @ zy = 2*zx*zy/1000 + cY
        ldr     r0, [sp, #12]   @ r0 = tmp
        str     r0, [sp, #4]    @ zx = tmp

        sub     r8, r8, #1      @ i--
        b       while

endwhile:
        mov     r0, r8          @ r0 = i
        ldr     r1, =0xff
        and     r0, r0, r1      @ r0 = i&0xff
        mov     r1, r0
        mov     r1, r1, lsl #8  @ r1 = (i&0xff)<<8
        orr     r0, r0, r1      @ r0 = ((i&0xff)<<8)||(i&0xff)
        str     r0, [sp, #8]    @ color = ((i&0xff)<<8)||(i&0xff)
        mvn     r0, r0          @ color = ~color
        ldr     r1, =0xffff
        and     r0, r0, r1
        str     r0, [sp, #8]    @ color = (~color)&0xffff
        mov     r2, r10         @ r2 = y
        mov     r1, r6          @ r1 = width
        mul     r2, r2, r1      @ r2 = y*width
        mov     r1, r9          @ r1 = x
        add     r2, r2, r1      @ r2 = y*width + x
        add     r2, r2, r2
        add     r2, r2, #52     @ r2 = r2 + 52
        str     r0, [sp, r2]
        add     r10, r10, #1    @ y = y+1
        bl      for2

backtofor1:
        mov     r10, #0         @ y = 0
        add     r9, r9, #1      @ x = x+1
        bl      for1

end:    subs    r0, r14, r13    @************************************
        adds    r0, r14, r13
        mov     r0, #0
        add     sp, sp, #16
        ldmfd   sp!,{r4-r10,fp,lr}
        mov     pc, lr
