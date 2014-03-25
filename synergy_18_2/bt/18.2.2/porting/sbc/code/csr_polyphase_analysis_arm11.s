;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;      (c) Cambridge Silicon Radio Limited 2010
;;  
;;               All rights reserved
;;
;;FILE:          csr_polyphase_analysis_arm11.s
;;
;;DESCRIPTION:   Sub-Band Codec (decoder and encoder). This file is
;;       specially designed for the ARM9E DSP-processors
;;
;;REVISION:      #1 $dl09 $This file is and optimized version of
;;             "csr_polyphase_analysis.c" rev. 4 
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    CODE32  
    AREA setendBEAsm, CODE, READONLY 

setendBE PROC
   setend be
   bx lr
   
setendBE_End
setendBE_CodeSize               DCD     (setendBE_End - setendBE)

   ENDP                             ; End of function definition


    CODE32  
    AREA setendLEAsm, CODE, READONLY 
	
setendLE PROC
   setend le
   bx lr
   
setendLE_End
setendLE_CodeSize               DCD     (setendLE_End - setendLE)

   ENDP                              ; End of function definition

    CODE32  
    AREA polyPhaseAnalysisSB4Asm, CODE, READONLY 

polyPhaseAnalysisSB4 PROC
; arg0 = r0 = pointer to hdl  which is a struct SbcHandle_t and it has the pointer to &sb_sample[0][0][0] which is now a ***sb_sample pointer
; arg1 = r1(0:15) = nrof_channels. r1(16:31) = blk
; arg2 = r2 = pointer to "&audio_window1[0]"
; arg3 = r3 = pointer to "&audio_window2[0]"

    STMFD   sp!,{r4-r12,lr} ; Store the registers r4-r12 plus reg lr (r14)
    
    ;; Set-up the pointers
    LDR r0,[r0] ; r0 = &sb_sample[0]
    MOV r5, r1, LSR #8
    LDR r4,[r0,r5,LSL #2] ; r4 = &sb_sample[current_block][0]    
    AND r1, r1, #0xFF

    LDR r5,[r2] ; r5=&audio_window1[o][0]
    LDR r6,[r3] ; r6=&audio_window2[o][0]

loop_SB4
    ;; Stores nrof_channels, &sb_sample[current_block][0], &audio_window1[0] and &audio_window2[0] to the stack
    STMFD   sp!,{r1,r4,r2,r3}
    
    LDR r2,C_SB4_FilterLabel_SIMD; Stores the pointer to the pointer to C_SB4_SIMD in r2
    LDR r4,[r2]                  ; Stores the pointer to C_SB4_SIMD in r4
    
    ;; The coefficients are rearranged in order to speed things up, using the following formula, we get the offset from its base register (r3)
    ; #offset = 2 * {[ index/8 - floor(index/8)] * 40 + floor(index/8)}
	; We multiply by 2 because coefficients are 16 bits wide
    ; Example: The offset for coefficient C_SB4[19] = 2 * {[ 2.375 - 2 ] * 40 + 2} = 34 (refer to the table C_SB4_SIMD[40] in csr_polyphase.c)

	;; First, calculate y[0] = r7 = (y[0] + y[4]) >> 1
    LDRD    r8,r9,[r4]    ; r8[0:15] = C_SB4[0], r8[16:31] = C_SB4[8]
                          ; r9[0:15] = C_SB4[16], r9[16:31] = C_SB4[24]
    LDRD    r0,r1,[r5]    ; r0[0:15] = audio_window[0][0], r0[16:31] = audio_window[0][8]
                          ; r1[0:15] = audio_window[0][16], r1[16:31] = audio_window[0][24]
    SMUAD    r2, r0, r8
    SMLAD    r2, r1, r9, r2
    LDRSH    r0,[r4,#0x8]   ; r0 = C_SB8[32]
    LDRSH    r1,[r5,#0x8]   ; r1 = audio_window[0][32]
    SMLABB    r2,r0,r1,r2   ; r2 += C_SB8[32]*audio_window[0][32]
    MOV    r7,r2,ASR #1     ; r7 = y[0] >> 1
    ;----------    
    LDRD    r8,r9,[r4,#40]      ; r8[0:15] = C_SB8[4], r8[16:31] = C_SB8[12]
                                ; r9[0:15] = C_SB8[20], r9[16:31] = C_SB8[28]
    LDRD    r0,r1,[r6]          ; r0[0:15] = audio_window[0][4], r0[16:31] = audio_window[0][12]
                                ; r1[0:15] = audio_window[0][20], r1[16:31] = audio_window[0][28]
    SMUAD    r2, r0, r8
    SMLAD    r2, r1, r9, r2
    LDRSH    r0,[r4,#48]   ; r5 = C_SB8[36]
    LDRSH    r1,[r6,#8]   ; r6 = audio_window[0][36]
    SMLABB   r2,r0,r1,r2 ; r7 += C_SB8[36]*audio_window[0][36]
    MOV      r11,r2,ASR #1   ; r11 = y[4] >> 1
    QADD    r7,r7,r11   ; r7 = (y[0] + y[4]) >> 1
    ;;r7 locked

	;; Second, calculate y[1] = r8 = (y[1] + y[3]) >> 1
    LDRSH    r0, [r4, #10]       ; r0 = C_SB8[1]
    LDRSH   r1,[r5, #10]        ; r1 = audio_window[0][1]
    
    SMULBB  r2,r0,r1            ; r2 = C_SB8[1]*audio_window[0][1]
    
    LDRD    r8,r9,[r4, #12]     ; r8[0:15] = C_SB8[9], r8[16:31] = C_SB8[17]
                                ; r9[0:15] = C_SB8[25], r9[16:31] = C_SB8[33]
    LDRD    r0,r1,[r5, #12]     ; r0[0:15] = audio_window[0][9], r0[16:31] = audio_window[0][17]
                                ; r1[0:15] = audio_window[0][25], r1[16:31] = audio_window[0][33]
    SMLAD    r2, r0, r8, r2
    SMLAD    r2, r1, r9, r2
    
    MOV    r11,r2,ASR #1   ; r11 = y[1] >> 1
    ; ---------
    LDRSH     r0, [r4, #30]   ; r0 = C_SB8[3]
    LDRSH     r1, [r5, #30]   ; r1 = audio_window[0][3]
    SMULBB    r2, r0, r1      ; r2 += C_SB8[3]*audio_window[0][3]
    
    LDRD    r8,r9,[r4, #32]     ; r8[0:15] = C_SB8[11], r8[16:31] = C_SB8[19]
                                ; r9[0:15] = C_SB8[27], r9[16:31] = C_SB8[35]
    LDRD    r0,r1,[r5, #32]     ; r0[0:15] = audio_window[0][11], r0[16:31] = audio_window[0][19]
                                ; r1[0:15] = audio_window[0][27], r1[16:31] = audio_window[0][35]
    SMLAD    r2, r0, r8, r2
    SMLAD    r2, r1, r9, r2

    MOV    r8,r2,ASR #1   ; r8 = y[3] >> 1
    QADD    r8,r8,r11   ; r8 = (y[1] + y[3]) >> 1
    ; r7, r8 locked
	
	; Third, calculate y[2] = r9 = y[2] >> 2
	LDRD    r0,r1,[r4,#20]    ; r0[0:15] = C_SB8[2], r0[16:31] = C_SB8[10]
                              ; r1[0:15] = C_SB8[18], r1[16:31] = C_SB8[26]
    LDRD    r10,r11,[r5,#20]  ; r10[0:15] = audio_window[0][2], r10[16:31] = audio_window[0][10]
                              ; r11[0:15] = audio_window[0][18], r11[16:31] = audio_window[0][26]
    SMUAD    r2, r10, r0
    SMLAD    r2, r11, r1, r2
    LDRSH    r10,[r4,#28]   ; r10 = C_SB8[34]
    LDRSH    r11,[r5,#28]   ; r11 = audio_window[0][34]
    SMLABB    r2,r10,r11,r2  ; r2 += C_SB8[34]*audio_window[0][34]
    MOVS    r9,r2,ASR #2   ; r9 = y[2] >> 2
    ADC     r9,r9,#0    ; correct rounding (NB: migth be removed for speed optimization)
    ; locked r7, r8, r9
	
	;; Fourth, calculate y[5] = r10 = y[5]
	LDRSH     r10, [r4, #50]   ; r10 = C_SB8[5]
    LDRSH     r11, [r6, #10]   ; r11 = audio_window[0][5]
    SMULBB    r12, r10, r11      ; r12 = C_SB8[5]*audio_window[0][5]
    
    LDRD    r10,r11,[r4, #52]     ; r10[0:15] = C_SB8[13], r10[16:31] = C_SB8[21]
                                ; r11[0:15] = C_SB8[29], r11[16:31] = C_SB8[37]
    LDRD    r2,r3,[r6, #12]      ; r2[0:15] = audio_window[0][13], r2[16:31] = audio_window[0][21]
                                ; r3[0:15] = audio_window[0][29], r3[16:31] = audio_window[0][37]
    SMLAD    r12, r2, r10, r12
    SMLAD    r12, r3, r11, r12
    MOV      r10,r12,ASR #1  ; r10 = y[5] >> 1
	; locked r7, r8, r9, r10

	;; Fifth we calculates y[7] = r11 = y[7]
	LDRSH     r2, [r4, #70]   ; r2 = C_SB8[7]
    LDRSH     r3, [r6, #30]   ; r3 = audio_window[0][7]
    SMULBB    r11, r2, r3      ; r11 = C_SB8[7]*audio_window[0][7]
    
    LDRD    r0,r1,[r4, #72]     ; r0[0:15] = C_SB8[15], r0[16:31] = C_SB8[23]
                                ; r1[0:15] = C_SB8[31], r1[16:31] = C_SB8[39]
    LDRD    r2,r3,[r6, #32]     ; r2[0:15] = audio_window[0][15], r2[16:31] = audio_window[0][23]
                                ; r3[0:15] = audio_window[0][31], r3[16:31] = audio_window[0][39]
    SMLAD    r11, r2, r0, r11
    SMLAD    r11, r3, r1, r11
    
    MOV    r11,r11,ASR #1   ; r11 = y[7] >> 1
	
    
    ;; Sixth, calculate y[5] = r10 = y[5] - y[7]) >> 16
    QSUB    r10,r10,r11
	
	
    ;; 68 Instructions so far

    ;; At this point we have the following registers status
    ;; r0 = nr_of_channels
    ;; r1 = &sb_sample[current_block][ch][0]
    ;; r2 = &audio_sample[0][0]
    ;; r3 = ch counter
    ;; r4 = &C_SB4[0]
    ;; r5 = ---
    ;; r6 = ---
    ;; r7 = y[0]
    ;; r8 = y[1]
    ;; r9 = y[2]
    ;; r10 = y[5]
    ;; r11 = ---
    ;; r12 = intra-procedure-call scratch register (ip) (unused and unsaved)
    ;; r13 = stack pointer (sp)
    ;; r14 = linker register (lr)
    ;; r15 = program counter (pc)
    
    ;; Now we calculate cos4 by means of cos2 and after this we calculate y0_cos4 = y[0] * cos4
    LDR r11,A_SB4_cos2  ; r11 = cos(2*pi/16) = cos2
    SUB r6,r11,#0x1BC0  ; r6 = cos(4*pi/16) = cos4
    SMULWB  r5,r7,r6    ; r5 = y0_cos4
    LDR r12,A_SB4_cos6  ; r12 = cos(6*pi/16) = cos6

    ;; Calculates   sptr[0] = r7 = ((y0_cos4 + y[2])>>1 + (y1_cos2 + y5_cos6)>>1) >> 13
    SMULWB  r6,r8,r11   ; r6 = y1_cos2
    SMLAWB  r6,r10,r12,r6   ; r6 = y1_cos2 + y5_cos6

    ;; Now we're freeing r8, r10 and r12
    SMULWB  r12,r8,r12  ; r12 = y1_cos6 (this is done to free r8)
    SMULWB  r11,r10,r11 ; r11 = y5_cos2 (this is done to free r10)
    SUB r11,r12,r11 ; r11 = (y1_cos6 - y5_cos2)
    
    ;; sprt[0] calculation cont'd
    ADD r8,r5,r9    ; r8 = (y0_cos4 + y[2])
    MOV r8,r8,ASR #1    ; r8 = (y0_cos4 + y[2]) >> 1
    ADD r7,r8,r6,ASR #1 ; r7 = ((y0_cos4 + y[2])>>1 + (y1_cos2 + y5_cos6)>>1)
    
    ;; Calculates   sptr[3] = r10 = ((y[2] + y0_cos4)>>1 - (y1_cos2 + y5_cos6)>>1) >> 13
    SUB r10,r8,r6,ASR #1; r10 = ((y[2] + y0_cos4)>>1 - (y1_cos2 + y5_cos6)>>1)

    ;; Calculates   sptr[1] = r8 = ((y[2] - y0_cos4)>>1 + (y1_cos6 - y5_cos2)>>1) >> 13
    SUB r12,r9,r5   ; r12 = (y[2] - y0_cos4)
    MOV r12,r12,ASR #1  ; r12 = (y[2] - y0_cos4) >> 1
    ADD r8,r12,r11,ASR #1; r8 = ((y[2] - y0_cos4)>>1 + (y1_cos6 - y5_cos2)>>1)
     
    ;; Calculates   sptr[2] = r9 = ((y[2] - y0_cos4)>>1 - (y1_cos6 - y5_cos2)>>1) >> 13 
    SUB r9,r12,r11,ASR #1; r9 = ((y[2] - y0_cos4)>>1 - (y1_cos6 - y5_cos2)>>1)
    
    ;; Current register status
    ;; r0 = nr_of_channels
    ;; r1 = &sb_sample[current_block][ch][0]
    ;; r2 = &audio_sample[ch][0]
    ;; r3 = ch counter
    ;; r4 = &C_SB4[0]
    ;; r5 = ---
    ;; r6 = ---
    ;; r7 = sptr[0]
    ;; r8 = sptr[1]
    ;; r9 = sptr[2]
    ;; r10 = sptr[3]
    ;; r11 = ---
    ;; r12 = intra-procedure-call scratch register (ip) (unused and unsaved)
    ;; r13 = stack pointer (sp)
    ;; r14 = linker register (lr)
    ;; r15 = program counter (pc)

    ;; All we need to do now is to store the result in sb_sample[current_block][ch][0-3]
        
    ;; 122 Instructions until here (151)
	   

    LDMFD   sp!,{r1,r4,r2,r3} ; loads r1 = ch and r4 = &sb_sample[current_block][0] r2 = &audio_window1[0] r3 = &audio_window2[0] back into our internal registers
    LDR r0,[r4] ; r0 = &sb_sample[current_block][0][0]
    STMIA   r0, {r7-r10}    ; store sb_sample[current_block][ch][0-3]
	
    ;; Set-up the loop for the next channel or ends
    ADD r4,r4,#4   ; r4 = &sb_sample[current_block][ch][0] (update for next iteration)
    LDR r5,[r2,#4] ; r5=&audio_window1[1][0]
    LDR r6,[r3,#4] ; r6=&audio_window2[1][0]
    
	SUBS    r1,r1,#1    ; decrease number of channels by 1
    BGT loop_SB4    ; If ch counter < nr_of_channels run one more time
	
    ;; Total of 91 instructions between STMFD and LDMFD
    ;; 
    ;;
    ;;      Reduction   = 30.5 % (counted in number of instructions)

    LDMFD sp!,{r4-r12,pc}   ; POP's the registers that where stored in the beginning

;========================================================================================================

A_SB4_cos2          DCD     0x00007642      ; cos2 = cos(2*pi/16) [NB: Remember here that cos4 = cos(4*pi/16) = cos2 - 0x1bc0]
A_SB4_cos6          DCD     0x000030FC      ; cos6 = cos(6*pi/16)

;========================================================================================================

C_SB4_FilterLabel_SIMD       DCD     aliasC_SB4           ; Address of C_SB4_SIMD[0]

;========================================================================================================

polyPhaseAnalysisSB4_End                       ; End of function body useful when it needs to be moved

;========================================================================================================

A_SB4_CodeSize               DCD     (polyPhaseAnalysisSB4_End - polyPhaseAnalysisSB4)
A_SB4_Filter_SIMDAddrOffset  DCD     (C_SB4_FilterLabel_SIMD - polyPhaseAnalysisSB4)

;========================================================================================================

    ENDP                                    ; End of function definition

;========================================================================================================

    CODE32  
    AREA polyPhaseAnalysisSB8Asm, CODE, READONLY 
    
polyPhaseAnalysisSB8 PROC
; arg0 = r0 = pointer to hdl  which is a struct SbcHandle_t and it has the pointer to &sb_sample[0][0][0] which is now a ***sb_sample pointer
; arg1 = r1(0:15) = nrof_channels. r1(16:31) = blk
; arg2 = r2 = pointer to "&audio_window1[0]"
; arg3 = r3 = pointer to "&audio_window2[0]"

    STMFD   sp!,{r4-r12,lr} ; Store the registers r4-r12 plus reg lr (r14)
    
    ;; Set-up the pointers
    LDR r0,[r0] ; r0 = &sb_sample[0]
    MOV r5, r1, LSR #8
    LDR r4,[r0,r5,LSL #2] ; r4 = &sb_sample[current_block][0]    
    AND r1, r1, #0xFF

	LDR r5,[r2] ; r5=&audio_window1[o][0]
    LDR r6,[r3] ; r6=&audio_window2[o][0]
	
loop_SB8
    	
    ;; Stores nrof_channels, &sb_sample[current_block][0], &audio_window1[0] and &audio_window2[0] to the stack
    STMFD   sp!,{r1,r4,r2,r3}
    
    LDR r2,C_SB8_FilterLabel_SIMD; Stores the pointer to the pointer to C_SB8_SIMD in r2
    LDR r4,[r2]                  ; Stores the pointer to C_SB8_SIMD in r4
    
    ;; The coefficients are rearranged in order to speed things up, using the following formula, we get the offset from its base register (r3)
    ; #offset = 2 * {[ index/16 - floor(index/16)] * 80 + floor(index/16)}
	; We multiply by 2 because coefficients are 16 bits wide
    ; Example: The offset for coefficient C_SB8[49] = 2 * {[ 3.0625 - 3 ] * 80 + 3} = 16 (refer to the table C_SB8_SIMD[80] in csr_polyphase.c)

    ;; First, calculate y[0] = r7 = (y[0] + y[8]) >> 1
    LDRD    r8,r9,[r4]    ; r8[0:15] = C_SB8[0], r8[16:31] = C_SB8[16]
                          ; r9[0:15] = C_SB8[32], r9[16:31] = C_SB8[48]
    LDRD    r0,r1,[r5]    ; r0[0:15] = audio_window[0][0], r0[16:31] = audio_window[0][16]
                          ; r1[0:15] = audio_window[0][32], r1[16:31] = audio_window[0][48]
    SMUAD    r2, r0, r8
    SMLAD    r2, r1, r9, r2
    LDRSH    r0,[r4,#0x8]   ; r0 = C_SB8[64]
    LDRSH    r1,[r5,#0x8]   ; r1 = audio_window[0][64]
    SMLABB    r2,r0,r1,r2   ; r2 += C_SB8[64]*audio_window[0][64]    
    MOV    r7,r2,ASR #1     ; r7 = y[0] >> 1
    ;----------    
    LDRD    r8,r9,[r4,#80]      ; r8[0:15] = C_SB8[8], r8[16:31] = C_SB8[24]
                                ; r9[0:15] = C_SB8[40], r9[16:31] = C_SB8[56]
    LDRD    r0,r1,[r6]          ; r0[0:15] = audio_window[0][8], r0[16:31] = audio_window[0][24]
                                ; r1[0:15] = audio_window[0][40], r1[16:31] = audio_window[0][56]
    SMUAD    r2, r0, r8
    SMLAD    r2, r1, r9, r2
    LDRSH    r0,[r4,#88]   ; r5 = C_SB8[72]
    LDRSH    r1,[r6,#8]   ; r6 = audio_window[0][72]
    SMLABB    r2,r0,r1,r2 ; r7 += C_SB8[72]*audio_window[0][72]    
    MOV    r11,r2,ASR #1   ; r11 = y[8] >> 1
    QADD    r7,r7,r11   ; r7 = (y[0] + y[8]) >> 1
    ;;r7 locked

    ;; Second, calculate y[1] = r0 = (y[1] + y[7]) >> 1
    LDRSH    r0, [r4, #10]       ; r0 = C_SB8[1]
    LDRSH   r1,[r5, #10]        ; r1 = audio_window[0][1]
    
    SMULBB  r2,r0,r1            ; r2 = C_SB8[1]*audio_window[0][1]
    
    LDRD    r8,r9,[r4, #12]     ; r8[0:15] = C_SB8[17], r8[16:31] = C_SB8[33]
                                ; r9[0:15] = C_SB8[49], r9[16:31] = C_SB8[65]
    LDRD    r0,r1,[r5, #12]     ; r0[0:15] = audio_window[0][17], r0[16:31] = audio_window[0][33]
                                ; r1[0:15] = audio_window[0][49], r1[16:31] = audio_window[0][65]
    SMLAD    r2, r0, r8, r2
    SMLAD    r2, r1, r9, r2    
    
    MOV    r11,r2,ASR #1   ; r11 = y[1] >> 1    
    ; ---------    
    LDRSH     r0, [r4, #70]   ; r0 = C_SB8[7]
    LDRSH     r1, [r5, #70]   ; r1 = audio_window[0][7]
    SMULBB    r2, r0, r1      ; r2 += C_SB8[7]*audio_window[0][7]
    
    LDRD    r8,r9,[r4, #72]     ; r8[0:15] = C_SB8[23], r8[16:31] = C_SB8[39]
                                ; r9[0:15] = C_SB8[55], r9[16:31] = C_SB8[71]
    LDRD    r0,r1,[r5, #72]      ; r0[0:15] = audio_window[0][23], r0[16:31] = audio_window[0][39]
                                ; r1[0:15] = audio_window[0][55], r1[16:31] = audio_window[0][71]
    SMLAD    r2, r0, r8, r2
    SMLAD    r2, r1, r9, r2
    
    MOV    r8,r2,ASR #1   ; r8 = y[7] >> 1            
    QADD    r0,r8,r11   ; r0 = (y[1] + y[7]) >> 1
    ; r7, r0 locked    
    
    LDRD    r8,r9,[r4,#20]    ; r8[0:15] = C_SB8[2], r8[16:31] = C_SB8[18]
                              ; r9[0:15] = C_SB8[34], r9[16:31] = C_SB8[50]
    LDRD    r10,r11,[r5,#20]    ; r10[0:15] = audio_window[0][2], r10[16:31] = audio_window[0][18]
                              ; r11[0:15] = audio_window[0][34], r11[16:31] = audio_window[0][50]
    SMUAD    r2, r10, r8
    SMLAD    r2, r11, r9, r2
    LDRSH    r10,[r4,#28]   ; r10 = C_SB8[66]
    LDRSH    r11,[r5,#28]   ; r11 = audio_window[0][66]
    SMLABB    r2,r10,r11,r2  ; r2 += C_SB8[66]*audio_window[0][66]
    MOV    r12,r2,ASR #1   ; r12 = y[2] >> 1
    ; ------
    LDRD    r8,r9,[r4,#60]    ; r8[0:15] = C_SB8[6], r8[16:31] = C_SB8[22]
                              ; r9[0:15] = C_SB8[38], r9[16:31] = C_SB8[54]
    LDRD    r10,r11,[r5,#60]    ; r10[0:15] = audio_window[0][6], r10[16:31] = audio_window[0][22]
                                ; r11[0:15] = audio_window[0][38], r11[16:31] = audio_window[0][54]
    SMUAD    r2, r10, r8
    SMLAD    r2, r11, r9, r2
    LDRSH    r10,[r4,#68]   ; r10 = C_SB8[70]
    LDRSH    r11,[r5,#68]   ; r11 = audio_window[0][70]
    SMLABB    r2,r10,r11,r2 ; r2 += C_SB8[70]*audio_window[0][70]
    MOV    r9,r2,ASR #1   ; r9 = y[6] >> 1
    QADD    r9,r9,r12   ; r9 = (y[2] + y[6]) >> 1
    ; locked r0, r7, r9
    
    ;; Fourth, calculate y[3] = r10 = (y[3] + y[5]) >> 1
    LDRSH     r10, [r4, #30]   ; r10 = C_SB8[3]
    LDRSH     r11, [r5, #30]   ; r11 = audio_window[0][3]
    SMULBB    r1, r10, r11      ; r1 += C_SB8[3]*audio_window[0][3]
    
    LDRD    r10,r11,[r4, #32]     ; r10[0:15] = C_SB8[19], r10[16:31] = C_SB8[35]
                                ; r11[0:15] = C_SB8[51], r11[16:31] = C_SB8[67]
    LDRD    r2,r3,[r5, #32]      ; r2[0:15] = audio_window[0][19], r2[16:31] = audio_window[0][35]
                                ; r3[0:15] = audio_window[0][51], r3[16:31] = audio_window[0][67]
    SMLAD    r1, r2, r10, r1
    SMLAD    r1, r3, r11, r1
    MOV r1,r1,ASR #1  ; r1 = y[3] >> 1    
    ; ---
    LDRSH     r10, [r4, #50]   ; r10 = C_SB8[5]
    LDRSH     r11, [r5, #50]   ; r11 = audio_window[0][5]
    SMULBB    r12, r10, r11      ; r12 = C_SB8[5]*audio_window[0][5]
    
    LDRD    r10,r11,[r4, #52]     ; r10[0:15] = C_SB8[21], r10[16:31] = C_SB8[37]
                                ; r11[0:15] = C_SB8[53], r11[16:31] = C_SB8[69]
    LDRD    r2,r3,[r5, #52]      ; r2[0:15] = audio_window[0][21], r2[16:31] = audio_window[0][37]
                                ; r3[0:15] = audio_window[0][53], r3[16:31] = audio_window[0][69]
    SMLAD    r12, r2, r10, r12
    SMLAD    r12, r3, r11, r12
    MOV r12,r12,ASR #1  ; r12 = y[5] >> 1
    QADD    r1,r1,r12  ; r1 = (y[3] + y[5]) >> 1
    ;locked r0, r1, r7, r9
        
    ;; Fifth, calculate y[4] = r8 = y[4] >> 2
    LDRD    r2,r3,[r4,#40]    ; r2[0:15] = C_SB8[4], r2[16:31] = C_SB8[20]
                              ; r3[0:15] = C_SB8[36], r3[16:31] = C_SB8[52]
    LDRD    r10,r11,[r5,#40]    ; r10[0:15] = audio_window[0][4], r10[16:31] = audio_window[0][20]
                                ; r11[0:15] = audio_window[0][36], r11[16:31] = audio_window[0][52]
    SMUAD    r2, r10, r2
    SMLAD    r2, r11, r3, r2
    LDRSH    r10,[r4,#48]   ; r10 = C_SB8[68]
    LDRSH    r11,[r5,#48]   ; r11 = audio_window[0][68]
    SMLABB    r2,r10,r11,r2 ; r2 += C_SB8[68]*audio_window[0][68]
    MOVS    r8,r2,ASR #2   ; r8 = y[4] >> 2
    ADC r8,r8,#0    ; correct rounding (NB: migth be removed for speed optimization)
    ; locked r0, r1, r7, r8, r9
    ;85
    ;LDR r4,C_SB8_FilterLabel_SIMD2; Stores the pointer to C_SB8_SIMD2 in r4
    
    ;; Sixth we calculates y[9] = r10 = y[9]
    LDRSH     r10, [r4, #90]   ; r10 = C_SB8[9]
    LDRSH     r11, [r6, #10]   ; r11 = audio_window[0][9]
    SMULBB    r12, r10, r11      ; r12 += C_SB8[9]*audio_window[0][9]
    
    LDRD    r10,r11,[r4, #92]     ; r10[0:15] = C_SB8[25], r10[16:31] = C_SB8[41]
                                ; r11[0:15] = C_SB8[57], r11[16:31] = C_SB8[73]
    LDRD    r2,r3,[r6, #12]      ; r2[0:15] = audio_window[0][25], r2[16:31] = audio_window[0][41]
                                ; r3[0:15] = audio_window[0][57], r3[16:31] = audio_window[0][73]
    SMLAD    r12, r2, r10, r12
    SMLAD    r12, r3, r11, r12
    MOV r10,r12,ASR #1  ; r10 = y[9] >> 1
    ; locked r0, r1, r7, r8, r9, r10

    push {r0,r1,r7,r8,r9} ;remember to pop them!!!

    ;; Seventh, calculate y[15] = r11 = y[15]
    LDRSH     r2, [r4, #150]   ; r2 = C_SB8[15]
    LDRSH     r3, [r6, #70]   ; r3 = audio_window[0][15]
    SMULBB    r12, r2, r3      ; r12 += C_SB8[15]*audio_window[0][15]
    
    LDRD    r0,r1,[r4, #152]     ; r0[0:15] = C_SB8[31], r0[16:31] = C_SB8[47]
                                ; r1[0:15] = C_SB8[63], r1[16:31] = C_SB8[79]
    LDRD    r2,r3,[r6, #72]      ; r2[0:15] = audio_window[0][31], r2[16:31] = audio_window[0][47]
                                ; r3[0:15] = audio_window[0][63], r3[16:31] = audio_window[0][79]
    SMLAD    r12, r2, r0, r12
    SMLAD    r12, r3, r1, r12
    MOV r11,r12,ASR #1  ; r11 = y[15] >> 1
    ;locked r11
 
    ;; Eigth, calculate y[9] = r3 = (y[9] - y[15]) >> 1
    QSUB    r3,r10,r11
    ;locked r3,r11
    
    ;; Ninth, calculate y[10] = r10 = y[10] 
    LDRD    r8,r9,[r4,#100]    ; r8[0:15] = C_SB8[10], r8[16:31] = C_SB8[26]
                              ; r9[0:15] = C_SB8[42], r9[16:31] = C_SB8[58]
    LDRD    r0,r1,[r6,#20]    ; r0[0:15] = audio_window[0][10], r0[16:31] = audio_window[0][26]
                                ; r1[0:15] = audio_window[0][42], r1[16:31] = audio_window[0][58]
    SMUAD    r2, r0, r8
    SMLAD    r2, r1, r9, r2
    LDRSH    r0,[r4,#108]   ; r0 = C_SB8[74]
    LDRSH    r1,[r6,#28]   ; r1 = audio_window[0][74]
    SMLABB    r2,r0,r1,r2 ; r2 += C_SB8[74]*audio_window[0][74]
    MOV r10,r2,ASR #1  ; r10 = y[10] >> 1
    ;locked r3,r10, r11
        
    ;; Tenth, calculate y[14] = r11 = y[14]
    LDRD    r8,r9,[r4,#140]    ; r8[0:15] = C_SB8[14], r8[16:31] = C_SB8[30]
                              ; r9[0:15] = C_SB8[46], r9[16:31] = C_SB8[62]
    LDRD    r0,r1,[r6,#60]    ; r0[0:15] = audio_window[0][14], r0[16:31] = audio_window[0][30]
                                ; r1[0:15] = audio_window[0][46], r1[16:31] = audio_window[0][62]
    SMUAD    r2, r0, r8
    SMLAD    r2, r1, r9, r2
    LDRSH    r0,[r4,#148]   ; r0 = C_SB8[78]
    LDRSH    r1,[r6,#68]   ; r1 = audio_window[0][78]
    SMLABB    r2,r0,r1,r2 ; r2 += C_SB8[78]*audio_window[0][78]
    MOV r11,r2,ASR #1  ; r11 = y[14] >> 1
      
    ;; Eleventh we calculate y[10] = r10 = (y[10] - y[14]) >> 1
    QSUB    r10,r10,r11
    ;locked r3,r10, r11
    
    ;; Twelfth, calculate y[11] = r10 = y[11]
    LDRSH     r8, [r4, #110]   ; r8 = C_SB8[11]
    LDRSH     r9, [r6, #30]   ; r9 = audio_window[0][11]
    SMULBB    r12, r8, r9      ; r12 += C_SB8[11]*audio_window[0][11]
    
    LDRD    r0,r1,[r4, #112]     ; r0[0:15] = C_SB8[27], r0[16:31] = C_SB8[43]
                                ; r1[0:15] = C_SB8[59], r1[16:31] = C_SB8[75]
    LDRD    r8,r9,[r6, #32]      ; r8[0:15] = audio_window[0][27], r8[16:31] = audio_window[0][43]
                                ; r9[0:15] = audio_window[0][59], r9[16:31] = audio_window[0][75]
    SMLAD    r12, r8, r0, r12
    SMLAD    r12, r9, r1, r12
    MOV r12,r12,ASR #1  ; r12 = y[11] >> 1    
    
    ;locked r3, r10, r11, r12
    
    ;; Thirteenth, calculates y[13] = r11 = y[13]
    LDRSH     r8, [r4, #130]   ; r8 = C_SB8[13]
    LDRSH     r9, [r6, #50]   ; r9 = audio_window[0][13]
    SMULBB    r11, r8, r9      ; r12 += C_SB8[13]*audio_window[0][13]
    
    LDRD    r0,r1,[r4, #132]     ; r0[0:15] = C_SB8[29], r0[16:31] = C_SB8[45]
                                ; r1[0:15] = C_SB8[61], r1[16:31] = C_SB8[77]
    LDRD    r8,r9,[r6, #52]      ; r8[0:15] = audio_window[0][29], r8[16:31] = audio_window[0][45]
                                ; r9[0:15] = audio_window[0][61], r9[16:31] = audio_window[0][77]
    SMLAD    r11, r8, r0, r11
    SMLAD    r11, r9, r1, r11
    MOV r11,r11,ASR #1  ; r11 = y[13] >> 1
    
    ;; Eleventh we calculate y[11] = r4 = (y[11] - y[13]) >> 1
    QSUB    r4,r12,r11

    pop {r0,r1,r7,r8,r9} ;remember to pop them!!!
    
    ;; This is just a little restructing before status
    ;; Stores y[10] in the temporary buffer
    STMFD   sp!,{r7-r10}    ; stores y[0,2,4,10] to the stack

    ;; Now loads the cos values 3 and 7
    LDR r11,A_SB8_cos1  ; r11 = cos(1*pi/16) = cos1
    LDR r12,A_SB8_cos7  ; r12 = cos(7*pi/16) = cos7
    LDR r5,A_SB8_cos3   ; r5 = cos(3*pi/16) = cos3
    LDR r6,A_SB8_cos5   ; r6 = cos(5*pi/16) = cos5

    ;; 143 Instructions so far
    ;; Status

    ;; At this point we have the following internal registers status
    ;; r0 = y[1]
    ;; r1 = y[3]
    ;; r2 = &audio_sample[0][0]
    ;; r3 = y[9]
    ;; r4 = y[11]
    ;; r5 = cos3
    ;; r6 = cos5
    ;; r7 = ---
    ;; r8 = ---
    ;; r9 = ---
    ;; r10 = ---
    ;; r11 = cos1
    ;; r12 = cos7
    ;; r13 = stack pointer (sp)
    ;; r14 = linker register (lr)
    ;; r15 = program counter (pc)

    ;; In the following we will calculate:
    ;; t11 = (((y1_cos3 + y9_cos5) >> 1) + ((y3_cos1 + y11_cos7) >> 1)) >> 1
    ;; t12 = (((y1_cos5 - y9_cos3) >> 1) - ((y3_cos7 - y11_cos1) >> 1)) >> 1
    ;; t13 = (((y1_cos7 + y9_cos1) >> 1) - ((y3_cos3 - y11_cos5) >> 1)) >> 1
    ;; t14 = (((y1_cos1 - y9_cos7) >> 1) - ((y3_cos5 + y11_cos3) >> 1)) >> 1
    ;;
    ;; t01 = (y2_cos2 + y10_cos6) >> 1
    ;; t02 = (y2_cos6 - y10_cos2) >> 1
    ;;
    ;; s01  = (y[4] + y0_cos4) >> 1
    ;; s02  = (y[4] - y0_cos4) >> 1
    ;;
    ;; sptr[0] = s01 + t01 + t11
    ;; sptr[7] = s01 + t01 - t11
    ;;
    ;; sptr[3] = s01 - t01 - t12
    ;; sptr[4] = s01 - t01 + t12
    ;;
    ;; sptr[1] = s02 + t02 - t13
    ;; sptr[6] = s02 + t02 + t13
    ;;
    ;; sptr[2] = s02 - t02 - t14
    ;; sptr[5] = s02 - t02 + t14
    ;;
    
    ;; First we calculate t11 and t12
    ;; t11 = (((y1_cos3 + y9_cos5) >> 1) + ((y3_cos1 + y11_cos7) >> 1)) >> 1
    ;; t12 = (((y1_cos5 - y9_cos3) >> 1) - ((y3_cos7 - y11_cos1) >> 1)) >> 1
    SMULWB  r9,r1,r11   ; r9 = y[3] * cos1
    SMLAWB  r9,r4,r12,r9    ; r9 = y[3] * cos1 + y[11] * cos7
    MOV r9,r9,ASR #1    ; r9 = (y[3] * cos1 + y[11] * cos7) >> 1

    SMULWB  r10,r1,r12  ; r10 = y[3] * cos7
    SMULWB  r14,r4,r11  ; r14 = y[11] * cos1
    SUB r10,r10,r14 ; r10 = y[3] * cos7 - y[11] * cos1
    MOV r10,r10,ASR #1  ; r10 = (y[3] * cos7 - y[11] * cos1) >> 1

    SMULWB  r8,r0,r5    ; r8 = y[1] * cos3
    SMLAWB  r8,r3,r6,r8 ; r8 = y[1] * cos3 + y[9] * cos5
    MOV r8,r8,ASR #1    ; r8 = (y[1] * cos3 + y[9] * cos5) >> 1
    QADD    r7,r8,r9    ; r7 = ((y[1] * cos3 + y[9] * cos5) >> 1) + ((y3_cos1 + y11_cos7) >> 1)
    MOV r7,r7,ASR #1    ; r7 = t11 = (((y[1] * cos3 + y[9] * cos5) >> 1) + ((y3_cos1 + y11_cos7) >> 1)) >> 1
    
    SMULWB  r9,r0,r6    ; r9 = y[1] * cos5
    SMULWB  r8,r3,r5    ; r8 = y[9] * cos3
    SUB r8,r9,r8    ; r8 = y[1] * cos5 - y[9] * cos3
    MOV r8,r8,ASR #1    ; r8 = (y[1] * cos5 - y[9] * cos3) >> 1
    SUB r8,r8,r10   ; r8 = ((y[1] * cos5 - y[9] * cos3) >> 1) - ((y[3] * cos7 - y[11] * cos1) >> 1)
    MOV r8,r8,ASR #1    ; r8 = t12 = (((y[1] * cos5 - y[9] * cos3) >> 1) - ((y[3] * cos7 - y[11] * cos1) >> 1)) >> 1
    
    ;; Then we calculate t13 and t14
    ;; t13 = (((y1_cos7 + y9_cos1) >> 1) - ((y3_cos3 - y11_cos5) >> 1)) >> 1
    ;; t14 = (((y1_cos1 - y9_cos7) >> 1) - ((y3_cos5 + y11_cos3) >> 1)) >> 1
    SMULWB  r9,r1,r5    ; r9 = y[3] * cos3
    SMULWB  r10,r4,r6   ; r10 = y[11] * cos5
    SUB r9,r9,r10   ; r9 = y[3] * cos3 - y[11] * cos5
    MOV r9,r9,ASR #1    ; r9 = (y[3] * cos3 - y[11] * cos5) >> 1
    
    SMULWB  r10,r0,r12  ; r10 = y[1] * cos7
    SMLAWB  r10,r3,r11,r10  ; r10 = y[1] * cos7 + y[9] * cos1
    MOV r10,r10,ASR #1  ; r10 = (y[1] * cos7 + y[9] * cos1) >> 1
    SUB r9,r10,r9   ; r9 = ((y[1] * cos7 + y[9] * cos1) >> 1) - ((y[3] * cos3 - y[11] * cos5) >> 1)
    MOV r9,r9,ASR #1    ; r9 = t13 = (((y[1] * cos7 + y[9] * cos1) >> 1) - ((y[3] * cos3 - y[11] * cos5) >> 1)) >> 1

    SMULWB  r10,r0,r11  ; r10 = y[1] * cos1
    SMULWB  r12,r3,r12  ; r12 = y[9] * cos7
    SUB r10,r10,r12 ; r10 = y[1] * cos1 - y[9] * cos7
    MOV r10,r10,ASR #1  ; r10 = (y[1] * cos1 - y[9] * cos7) >> 1

    SMULWB  r11,r1,r6   ; r11 = y[3] * cos5
    SMLAWB  r11,r4,r5,r11   ; r11 = y[3] * cos5 + y[11] * cos3
    MOV r11,r11,ASR #1  ; r11 = (y[3] * cos5 + y[11] * cos3) >> 1
    SUB r10,r10,r11 ; r10 = ((y[1] * cos1 - y[9] * cos7) >> 1) - (y[3] * cos5 + y[11] * cos3) >> 1)
    MOV r10,r10,ASR #1  ; r10 = t14 = (((y[1] * cos1 - y[9] * cos7) >> 1) - ((y[3] * cos5 + y[11] * cos3) >> 1)) >> 1

    ;; restucturing
    LDMFD   sp!,{r0,r1,r3,r4} ; loads y[0,4,2,10] back into our internal registers
    LDR r11,A_SB8_cos2  ; r11 = cos(2*pi/16) = cos2
    LDR r12,A_SB8_cos6  ; r12 = cos(6*pi/16) = cos6
    
	;182
    ;; At this point we have the following internal registers status
    ;; r0 = y[0]
    ;; r1 = y[4]
    ;; r2 = &audio_sample[0][0]
    ;; r3 = y[2]
    ;; r4 = y[10]
    ;; r5 = ---
    ;; r6 = ---
    ;; r7 = t11
    ;; r8 = t12
    ;; r9 = t13
    ;; r10 = t14
    ;; r11 = cos2
    ;; r12 = cos6
    ;; r13 = stack pointer (sp)
    ;; r14 = linker register (lr)
    ;; r15 = program counter (pc)
    
    ;; In the following we will calculate t01 and t02:
    ;; t01 = (y2_cos2 + y10_cos6) >> 1
    ;; t02 = (y2_cos6 - y10_cos2) >> 1
    SMULWB  r5,r3,r12   ; r5 = y[2] * cos6
    SMULWB  r6,r4,r11   ; r6 = y[10] * cos2
    SUB r6,r5,r6    ; r6 = y[2] * cos6 - y[10] * cos2
    MOV r6,r6,ASR #1    ; r6 = t02 = (y[2] * cos6 - y[10] * cos2) >> 1

    SMULWB  r5,r3,r11   ; r5 = y[2] * cos2
    SMLAWB  r5,r4,r12,r5    ; r5 = y[2] * cos2 + y[10] * cos6
    MOV r5,r5,ASR #1    ; r5 = t01 = (y[2] * cos2 + y[10] * cos6) >> 1

    ;; now we calculate s01 and s02:
    ;; s01  = (y[4] + y0_cos4) >> 1
    ;; s02  = (y[4] - y0_cos4) >> 1
    SUB r12,r11,#0x1BC0 ; r6 = cos(4*pi/16) = cos4
    SMLAWB  r3,r0,r12,r1    ; r3 = y[4] + y[0] * cos4
    MOV r3,r3,ASR #1    ; r3 = s01 = (y[4] + y[0] * cos4) >> 1

    SMULWB  r4,r0,r12   ; r4 = y[0] * cos4
    SUB r4,r1,r4    ; r4 = y[4] - y[0] * cos4
    MOV r4,r4,ASR #1    ; r4 = s02 = (y[4] - y[0] * cos4) >> 1

    ;; At this point we have the following internal registers status
    ;; r0 = ---
    ;; r1 = ---
    ;; r2 = &audio_sample[0][0]
    ;; r3 = s01
    ;; r4 = s02
    ;; r5 = t01
    ;; r6 = t02
    ;; r7 = t11
    ;; r8 = t12
    ;; r9 = t13
    ;; r10 = t14
    ;; r11 = ---
    ;; r12 = ---
    ;; r13 = stack pointer (sp)
    ;; r14 = linker register (lr)
    ;; r15 = program counter (pc)

    ;; Now all we have to do is calculate sptr[0-7]
    ;; sptr[0] = ((s01 + t01) >> 1) + t11
    ;; sptr[7] = ((s01 + t01) >> 1) - t11
    ;;
    ;; sptr[3] = ((s01 - t01) >> 1) - t12
    ;; sptr[4] = ((s01 - t01) >> 1) + t12
    ;;
    ;; sptr[1] = ((s02 + t02) >> 1) - t13
    ;; sptr[6] = ((s02 + t02) >> 1) + t13
    ;;
    ;; sptr[2] = ((s02 - t02) >> 1) - t14
    ;; sptr[5] = ((s02 - t02) >> 1) + t14
    ;; 
    ADD r11,r3,r5   ; r11 = s01 + t01
    SUB r5,r3,r5    ; r5 = s01 - t01

    ADD r12,r4,r6   ; r12 = s02 + t02
    SUB r6,r4,r6    ; r6 = s02 - t02

    MOV r3,r11,ASR #1   ; r3 = (s01 + t01) >> 1
    MOV r4,r12,ASR #1   ; r4 = (s02 + t02) >> 1
    MOV r5,r5,ASR #1    ; r5 = (s02 - t02) >> 1
    MOV r6,r6,ASR #1    ; r6 = (s01 - t01) >> 1

    ADD r0,r3,r7    ; r0 = sptr[[0]
    SUB r12,r3,r7   ; r12 = sptr[7]

    SUB r1,r4,r9    ; r1 = sptr[1]
    ADD r11,r4,r9   ; r11 = sptr[6]

    ;; At this point we have the following internal registers status
    ;; r0 = sptr[0]
    ;; r1 = sptr[1]
    ;; r2 = &audio_sample[0][0]
    ;; r3 = ---
    ;; r4 = ---
    ;; r5 = (s01 - t01) >> 1
    ;; r6 = (s02 - t02) >> 1
    ;; r7 = ---
    ;; r8 = t12
    ;; r9 = ---
    ;; r10 = t14
    ;; r11 = sptr[6]
    ;; r12 = sptr[7]
    ;; r13 = stack pointer (sp)
    ;; r14 = linker register (lr)
    ;; r15 = program counter (pc)

    ADD r9,r5,r8    ; r9 = sptr[4]
    SUB r8,r5,r8    ; r8 = sptr[3]

    SUB r7,r6,r10   ; r7 = sptr[2]
    ADD r10,r6,r10  ; r10 = sptr[5]

    ;; Now everything has been calculated all we have to do now is scale down and saturate
    ;; At this point we have the following internal registers status
    ;; r0 = sptr[0]
    ;; r1 = sptr[1]
    ;; r2 = &audio_sample[0][0]
    ;; r3 = ---
    ;; r4 = ---
    ;; r5 = ---
    ;; r6 = ---
    ;; r7 = sptr[2]
    ;; r8 = sptr[3]
    ;; r9 = sptr[4]
    ;; r10 = sptr[5]
    ;; r11 = sptr[6]
    ;; r12 = sptr[7]
    ;; r13 = stack pointer (sp)
    ;; r14 = linker register (lr)
    ;; r15 = program counter (pc)

    MOV   r5,r0   ; r5 = sprt[0]
    MOV   r6,r1   ; r6 = sprt[1]

    LDMFD   sp!,{r1,r4,r2,r3} ; loads r1 = ch and r4 = &sb_sample[current_block][0] r2 = &audio_window1[0] r3 = &audio_window2[0] back into our internal registers
    LDR r0,[r4] ; r0 = &sb_sample[current_block][0][0]
    
    STMIA   r0!,{r5-r12}    ; store sptr[0-7]
    
    ;; Set-up the loop for the next channel or ends
    ADD r4,r4,#4   ; r4 = &sb_sample[current_block][ch][0] (update for next iteration)
    LDR r5,[r2,#4] ; r5=&audio_window1[1][0]
    LDR r6,[r3,#4] ; r6=&audio_window2[1][0]
    
	SUBS    r1,r1,#1    ; decrease number of channels by 1
    BGT loop_SB8    ; If ch counter < nr_of_channels run one more time
    
	;; Total of 220 instructions between STMFD and LDMFD.
	;; For 2 channels, there are 220 + 213 = 433 instructions 
	;; Compared to csr_polyphase_analysis_arm9e, we are using 615 - 433 = 182 instructions less
	;; This yields a reduction of 29.6 % (counted in number of instructions)
	
ending
    LDMFD sp!,{r4-r12,pc}   ; POP's the registers that where stored in the beginning

;========================================================================================================

A_SB8_cos1          DCD     0x00007D8A      ; cos1 = cos(pi/16)   = 32138 = 0x7D8A
A_SB8_cos2          DCD     0x00007642      ; cos2 = cos(2*pi/16) [NB: Remember here that cos4 = cos(4*pi/16) = cos2 - 0x1bc0]
A_SB8_cos3          DCD     0x00006A6E      ; cos3 = cos(3*pi/16) = 27246 = 0x6A6E
A_SB8_cos5          DCD     0x0000471D      ; cos5 = cos(5*pi/16) = 18205 = 0x471D
A_SB8_cos6          DCD     0x000030FC      ; cos6 = cos(6*pi/16)
A_SB8_cos7          DCD     0x000018F9      ; cos7 = cos(7*pi/16) =  6393 = 0x18F9

;========================================================================================================

C_SB8_FilterLabel_SIMD       DCD     aliasC_SB8          ; Address of C_SB8_SIMD[0]

;========================================================================================================

polyPhaseAnalysisSB8_End                       ; End of function body useful when it needs to be moved

;========================================================================================================

A_SB8_CodeSize               DCD     (polyPhaseAnalysisSB8_End - polyPhaseAnalysisSB8)
A_SB8_Filter_SIMDAddrOffset  DCD     (C_SB8_FilterLabel_SIMD - polyPhaseAnalysisSB8)

;========================================================================================================

    ENDP                                    ; End of function definition

;========================================================================================================

    EXPORT setendBE
		EXPORT setendBE_CodeSize
	EXPORT setendLE
		EXPORT setendLE_CodeSize
	
	EXPORT polyPhaseAnalysisSB4 ; Makes polyPhaseAnalysisSB4 globally accessible by other C functions
        EXPORT A_SB4_CodeSize
        EXPORT A_SB4_Filter_SIMDAddrOffset

    EXPORT polyPhaseAnalysisSB8 ; Makes polyPhaseAnalysisSB8 globally accessible by other C functions
        EXPORT A_SB8_CodeSize
        EXPORT A_SB8_Filter_SIMDAddrOffset

    ;IMPORT C_SB4_SIMD        ; Makes the external defined global array "C_SB4" accessible for this function
    IMPORT aliasC_SB4        ; Makes the external defined global array "C_SB4" accessible for this function
    IMPORT aliasC_SB8        ; Makes the external defined global array "C_SB8_SIMD1" accessible for this function     
	
    END
;========================================================================================================
