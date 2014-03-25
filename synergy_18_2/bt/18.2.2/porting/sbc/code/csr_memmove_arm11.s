    CODE32
    AREA CsrSbcMemMoveAsm, CODE, READONLY

CsrSbcMemMove PROC
; Important! Pointer addresses must be word-aligned!! Otherwise a runtime error could occur!
; arg0 = r0 = void * source
; arg1 = r1 = number of bands

   STMFD   sp!,{r2-r12,lr} ; Store the registers r2-r12 plus reg lr (r14)

   CMP    r1, #4
   BEQ    bands_4
   
bands_8
    ADD  r1, r0, #40    ; r1 = Source address
    ADD  r0, r0, #44    ; r0 = Destination address    
    LDMIA r1!,{r3-r12}  ; Load a bunch of samples
    
	;Shift them 16 bits and pack them into registers
	MOV   r12, r12, LSL #16
	PKHTB r12, r12, r11, ASR #16
	
	MOV   r11, r11, LSL #16
	PKHTB r11, r11, r10, ASR #16
	
	MOV   r10, r10, LSL #16
	PKHTB r10, r10, r9, ASR #16

	MOV   r9, r9, LSL #16
	PKHTB r9, r9, r8, ASR #16

	MOV   r8, r8, LSL #16
	PKHTB r8, r8, r7, ASR #16
	
	MOV   r7, r7, LSL #16
	PKHTB r7, r7, r6, ASR #16
	
	MOV   r6, r6, LSL #16
	PKHTB r6, r6, r5, ASR #16
	
	MOV   r5, r5, LSL #16
	PKHTB r5, r5, r4, ASR #16
	
	MOV   r4, r4, LSL #16
	PKHTB r4, r4, r3, ASR #16
	
	MOV   r1, r3, LSL #16  ; This sample needs data from next reading
	
	STMIA  r0!,{r4-r12}    ; Store multiple registers

	ADD    r0, r0, #-80    ; r0 = Source/Destination address
    LDMIA  r0,{r2-r11}     

	PKHTB r12, r1, r11, ASR #16  ; Complete sample with previous reading (r1)
	
	MOV   r11, r11, LSL #16
	PKHTB r11, r11, r10, ASR #16
	
	MOV   r10, r10, LSL #16
	PKHTB r10, r10, r9, ASR #16

	MOV   r9, r9, LSL #16
	PKHTB r9, r9, r8, ASR #16
	
	MOV   r8, r8, LSL #16
	PKHTB r8, r8, r7, ASR #16
	
	MOV   r7, r7, LSL #16
	PKHTB r7, r7, r6, ASR #16
	
	MOV   r6, r6, LSL #16
	PKHTB r6, r6, r5, ASR #16
	
	MOV   r5, r5, LSL #16
	PKHTB r5, r5, r4, ASR #16
	
	MOV   r4, r4, LSL #16
	PKHTB r4, r4, r3, ASR #16
	
	MOV   r3, r3, LSL #16
	PKHTB r3, r3, r2, ASR #16

	MOV   r2, r2, LSL #16
	
	STMIA  r0,{r2-r12}
	

END
    LDMFD sp!,{r2-r12,pc}   ; POP's the registers that were stored in the beginning

    ;; The C library memmove implementation required 154 instructions
    ;; This one requires 50 (we only need half of the data thanks to data rearrangement) and just 6 memory access instructions
    ;; -> 67.5% reduction in number of instructions

bands_4

    LDMIA r0,{r3-r12}
    
	;Shift them 16 bits and pack them into registers
	MOV   r12, r12, LSL #16
	PKHTB r12, r12, r11, ASR #16
	
	MOV   r11, r11, LSL #16
	PKHTB r11, r11, r10, ASR #16
	
	MOV   r10, r10, LSL #16
	PKHTB r10, r10, r9, ASR #16

	MOV   r9, r9, LSL #16
	PKHTB r9, r9, r8, ASR #16

	MOV   r8, r8, LSL #16
	PKHTB r8, r8, r7, ASR #16
	
	MOV   r7, r7, LSL #16
	PKHTB r7, r7, r6, ASR #16
	
	MOV   r6, r6, LSL #16
	PKHTB r6, r6, r5, ASR #16
	
	MOV   r5, r5, LSL #16
	PKHTB r5, r5, r4, ASR #16
	
	MOV   r4, r4, LSL #16
	PKHTB r4, r4, r3, ASR #16
	
	MOV   r3, r3, LSL #16
	
	STMIA  r0,{r3-r12}    ; Store multiple registers
  
	
    LDMFD sp!,{r2-r12,pc}   ; POP's the registers that where stored in the beginning

;========================================================================================================


;========================================================================================================

    ENDP                                    ; End of function definition

;========================================================================================================

   EXPORT CsrSbcMemMove ; Makes CsrSbcMemMove globally accessible by other C functions

   END
;========================================================================================================
