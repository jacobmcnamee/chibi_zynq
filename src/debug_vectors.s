/*
 * debug_vectors.s
 *
 *  Created on: Feb 26, 2016
 *      Author: jacob
 */

    .text

    .global Und_Handler
Und_Handler:
    b Und_Handler

    .global Swi_Handler
Swi_Handler:
    b Swi_Handler

    .global Prefetch_Handler
Prefetch_Handler:
    b Prefetch_Handler

    .global Abort_Handler
Abort_Handler:
    b Abort_Handler

    .global Fiq_Handler
Fiq_Handler:
    b Fiq_Handler
