/*
 * reg_test.s
 *
 *  Created on: Jan 26, 2016
 *      Author: jacob
 */

/* Load <n> to r<n> */
.macro test_init_r n
    ldr     r\n, =\n
.endm

/* Jump to fail if r<n> does not contain <n> */
.macro test_check_r n
    cmp     r\n, #\n
    bne     fail
.endm

/* Load <n><n+1> to d<n> */
.macro test_init_d n
    ldr     r0, =\n
    ldr     r1, =\n+1
    vmov    d\n, r0, r1
.endm

/* Jump to fail if d<n> does not contain <n><n+1> */
.macro test_check_d n
    vmov    r0, r1, d\n
    cmp     r0, #\n
    bne     fail
    cmp     r1, #\n+1
    bne     fail
.endm


    .data
test_str:
    .asciz "reg test\r\n"
error_str:
    .asciz "reg test ERROR\r\n"


    .text
    .fpu neon

    .global reg_test_thread
reg_test_thread:

    test_init_r 0
    test_init_r 1
    test_init_r 2
    test_init_r 3
    test_init_r 4
    test_init_r 5
    test_init_r 6
    test_init_r 7
    test_init_r 8
    test_init_r 9
    test_init_r 10
    test_init_r 11
    test_init_r 12
    test_init_r 14

    push {r0, r1}

    /* Initialize all 32 d registers */
    test_init_d 0
    test_init_d 1
    test_init_d 2
    test_init_d 3
    test_init_d 4
    test_init_d 5
    test_init_d 6
    test_init_d 7
    test_init_d 8
    test_init_d 9
    test_init_d 10
    test_init_d 11
    test_init_d 12
    test_init_d 13
    test_init_d 14
    test_init_d 15
    test_init_d 16
    test_init_d 17
    test_init_d 18
    test_init_d 19
    test_init_d 20
    test_init_d 21
    test_init_d 22
    test_init_d 23
    test_init_d 24
    test_init_d 25
    test_init_d 26
    test_init_d 27
    test_init_d 28
    test_init_d 29
    test_init_d 30
    test_init_d 31

    /* Thread sleep */
    push {r0-r3, r12, lr}
    ldr     r0, =100
    bl      chThdSleep
    pop {r0-r3, r12, lr}

    /* Verify all 32 d registers */
    test_check_d 0
    test_check_d 1
    test_check_d 2
    test_check_d 3
    test_check_d 4
    test_check_d 5
    test_check_d 6
    test_check_d 7
    test_check_d 8
    test_check_d 9
    test_check_d 10
    test_check_d 11
    test_check_d 12
    test_check_d 13
    test_check_d 14
    test_check_d 15
    test_check_d 16
    test_check_d 17
    test_check_d 18
    test_check_d 19
    test_check_d 20
    test_check_d 21
    test_check_d 22
    test_check_d 23
    test_check_d 24
    test_check_d 25
    test_check_d 26
    test_check_d 27
    test_check_d 28
    test_check_d 29
    test_check_d 30
    test_check_d 31

    pop     {r0, r1}

    test_check_r 0
    test_check_r 1
    test_check_r 2
    test_check_r 3
    test_check_r 4
    test_check_r 5
    test_check_r 6
    test_check_r 7
    test_check_r 8
    test_check_r 9
    test_check_r 10
    test_check_r 11
    test_check_r 12
    test_check_r 14

    b       success

fail:
    ldr     r0, =error_str
    bl      printf
    b       loop

success:
    ldr     r0, =test_str
    bl      printf

loop:
    /* Thread sleep */
    ldr     r0, =10
    bl      chThdSleep

    b       reg_test_thread
