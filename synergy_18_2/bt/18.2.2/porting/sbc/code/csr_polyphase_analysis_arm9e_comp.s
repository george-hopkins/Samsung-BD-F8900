;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;      (c) Cambridge Silicon Radio Limited 2009
;;  
;;               All rights reserved
;;
;;FILE:          csr_polyphase_analysis_arm9e_comp.s
;;
;;DESCRIPTION:   Sub-Band Codec (decoder and encoder). This file is
;;       specially designed for the ARM9E DSP-processors
;;       with compressed code (no unroled for loops)
;;  
;;REVISION:      #1 $hn02 $This file is and optimized version of
;;             "csr_polyphase_analysis.c" rev. 4 
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    CODE32  
    AREA polyPhaseAnalysisSB4Asm, CODE, READONLY 

polyPhaseAnalysisSB4 PROC
; arg0 = r0 = pointer to hdl  which is a struct SbcHandle_t and it has the pointer to &sb_sample[0][0][0] which is now a ***sb_sample pointer
; arg1 = r1 = nrof_channels
; arg2 = r2 = current_block
; arg3 = r3 = pointer to "&audio_window[0]"
    STMFD   sp!,{r4-r11,lr} ; Store the registers r4-r8 plus reg lr (r14)

    ;; Set-up the pointers
    LDR r4,C_SB4_FilterLabel ; Stores the pointer to C_SB4 in r4
    LDR r0,[r0] ; r0 = &sb_sample[0]
    LDR r5,[r0,r2,LSL #2] ; r5 = &sb_sample[current_block][0]

    MOV r0,r1 ; r0 = nrof_channels;
    LDR r1,[r5] ; r1 = &sb_sample[current_block][0][0]
    LDR r2,[r3] ; r2=&audio_window[o][0]

chan_SB4
    ;; Stores &sb_sample[current_block][0] and &audio_windows[0] to the stack
    STMFD   sp!,{r5,r3}
    MOV r12,#6      ; counter var "i"
    
loop_SB4
    ;; First we calculates y[i] = r7 (for "i" = {0,1,...,5})
    LDRSH   r5,[r4,#0x0]    ; r5 = C_SB[i+0]
    LDRSH   r6,[r2,#0x0]    ; r6 = audio_window[0][i+0]
    SMULBB  r7,r5,r6    ; r7 = C_SB[i+0]*audio_window[0][i+0]
    LDRSH   r5,[r4,#0x10]!  ; r5 = C_SB[i+8]
    LDRSH   r6,[r2,#0x10]!  ; r6 = audio_window[0][i+8]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+8]*audio_window[0][i+8]
    LDRSH   r5,[r4,#0x10]!  ; r5 = C_SB[i+16]
    LDRSH   r6,[r2,#0x10]!  ; r6 = audio_window[0][i+16]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+16]*audio_window[0][i+16]
    LDRSH   r5,[r4,#0x10]!  ; r5 = C_SB[i+24]
    LDRSH   r6,[r2,#0x10]!  ; r6 = audio_window[0][i+24]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+24]*audio_window[0][i+24]
    LDRSH   r5,[r4,#0x10]!  ; r5 = C_SB[i+32]
    LDRSH   r6,[r2,#0x10]!  ; r6 = audio_window[0][i+32]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+32]*audio_window[0][i+32]
    MOV r7,r7,ASR #1    ; y[i] >>= 1
    
    STMFD   sp!,{r7}    ; Stores the result in r7 on the stack
    SUB r4,r4,#0x3e ; Sets the C_SB4 pointer to "i+1"
    SUB r2,r2,#0x3e ; Sets the audio_window pointer to "i+1"
    SUBS    r12,r12,#1  ; Decrements "i"
    BGT loop_SB4    ; If not jump it takes another iteration

    ;; Calculates y[7]
    LDRSH   r5,[r4,#0x02]!  ; r5 = C_SB[i+0]
    LDRSH   r6,[r2,#0x02]!  ; r6 = audio_window[0][i+0]
    SMULBB  r10,r5,r6   ; r10 = C_SB[i+0]*audio_window[0][i+0]
    LDRSH   r5,[r4,#0x10]!  ; r5 = C_SB[i+8]
    LDRSH   r6,[r2,#0x10]!  ; r6 = audio_window[0][i+8]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB[i+8]*audio_window[0][i+8]
    LDRSH   r5,[r4,#0x10]!  ; r5 = C_SB[i+16]
    LDRSH   r6,[r2,#0x10]!  ; r6 = audio_window[0][i+16]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB[i+16]*audio_window[0][i+16]
    LDRSH   r5,[r4,#0x10]!  ; r5 = C_SB[i+24]
    LDRSH   r6,[r2,#0x10]!  ; r6 = audio_window[0][i+24]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB[i+24]*audio_window[0][i+24]
    LDRSH   r5,[r4,#0x10]!  ; r5 = C_SB[i+32]
    LDRSH   r6,[r2,#0x10]!  ; r6 = audio_window[0][i+32]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB[i+32]*audio_window[0][i+32]
    MOV r10,r10,ASR #1  ; y[7] >>= 1
    
    ;; We reset the pointers so they're ready for the next iteration
    SUB r4,r4,#0x4e ; r4 = &C_SB4[0]
    ADD r2,r2,#0x52 ; r2 = &audio_window[ch+1][0]
    
    ;; Now we calculate y[5] = r10 = y[5] - y[7]
    SUB r10,r7,r10

    ADD sp,sp,#4
    LDMFD   sp!,{r5-r9} ; r5=y[4],r6=y[3],r7=y[2],r8=y[1],r9=y[0]
    
    ADD r5,r5,r9    ; r5 = y[0] = y[0] + y[4]
    ADD r8,r6,r8    ; r8 = y[1] = y[1] + y[3]

    MOV r9,r7,ASR #1    ; r9 = y[2]
    MOV r7,r5       ; r7 = y[0]
    ;; 52 Instructions so far (48(50))

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
    STMIA   r1, {r7-r10}    ; store sb_sample[current_block][ch][0-3]

    ;; Set-up the loop for the next channel or ends
    LDMFD   sp!,{r5,r3} ; loads r5 = &sb_sample[current_block][0]  and r3 = &audio_sample[0]  back into our internal registers
    LDR r1,[r5,#4]   ; r1 = &sb_sample[current_block][ch][0] (update for next iteration)
    LDR r2,[r3,#4] ; r2 = &audio_sample[ch] (update for next iteration)
    SUBS    r0,r0,#1    ; decrease number of channels by one
    BGT chan_SB4    ; If ch counter < nr_of_channels run one more time
    
    ;; Total of 80 instructions between STMFD and LDMFD (this is 16 less than the C implementation (96))
    ;; but this is with unrolled for loops
    ;; 
    ;; So in total this yields 4+132*2+52 = 320 linies of instructions with 2 channels which should be 
    ;; compared to (7+2+4*2+26*8*2+18*2+40*2+1) = 550 linies of instructions for the C implementation
    ;;
    ;; In short Assembly    = 320 instructions
    ;;      C       = 550 instructions
    ;;      Reduction   = 41 % (counted in number of instructions)

    LDMFD sp!,{r4-r11,pc}   ; POP's the registers that where stored in the beginning

;========================================================================================================

A_SB4_cos2          DCD     0x00007642      ; cos2 = cos(2*pi/16) [NB: Remember here that cos4 = cos(4*pi/16) = cos2 - 0x1bc0]
A_SB4_cos6          DCD     0x000030FC      ; cos6 = cos(6*pi/16)

;========================================================================================================

C_SB4_FilterLabel       DCD     C_SB4           ; Address of C_SB4[0]

;========================================================================================================

polyPhaseAnalysisSB4_End                       ; End of function body useful when it needs to be moved

;========================================================================================================

A_SB4_CodeSize          DCD     (polyPhaseAnalysisSB4_End - polyPhaseAnalysisSB4)
A_SB4_FilterAddrOffset  DCD     (C_SB4_FilterLabel - polyPhaseAnalysisSB4)

;========================================================================================================

    ENDP                                    ; End of function definition

;========================================================================================================

    CODE32  
    AREA polyPhaseAnalysisSB8Asm, CODE, READONLY 
    
polyPhaseAnalysisSB8 PROC
; arg0 = r0 = pointer to hdl  which is a struct SbcHandle_t and it has the pointer to &sb_sample[0][0][0] which is now a ***sb_sample pointer
; arg1 = r1 = nrof_channels
; arg2 = r2 = current_block
; arg3 = r3 = pointer to "&audio_window[0]"
    STMFD   sp!,{r4-r11,lr} ; Store the registers r4-r8 plus reg lr (r14)

    ;; Set-up the pointers
    LDR r0,[r0] ; r0 = &sb_sample[0]
    LDR r4,[r0,r2,LSL #2] ; r4 = &sb_sample[current_block][0]

    LDR r2,[r3] ; r2=&audio_window[o][0]

    
chan_SB8
    ;; Stores nrof_channels, &sb_sample[current_block][0] and &audio_windows[0] to the stack
    STMFD   sp!,{r1,r4,r3}
    LDR r4,C_SB8_FilterLabel; Stores the pointer to C_SB8 in r4
    MOV r12,#12     ; counter var "i" = 12
    
loop1_SB8
    ;; First we calculates y[i] = r7 (for "i" = {0,1,...,11})
    LDRSH   r5,[r4,#0x0]    ; r5 = C_SB[i+0]
    LDRSH   r6,[r2,#0x0]    ; r6 = audio_window[0][i+0]
    SMULBB  r7,r5,r6    ; r7 = C_SB[i+0]*audio_window[0][i+0]
    LDRSH   r5,[r4,#0x20]!  ; r5 = C_SB[i+8]
    LDRSH   r6,[r2,#0x20]!  ; r6 = audio_window[0][i+8]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+8]*audio_window[0][i+8]
    LDRSH   r5,[r4,#0x20]!  ; r5 = C_SB[i+16]
    LDRSH   r6,[r2,#0x20]!  ; r6 = audio_window[0][i+16]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+16]*audio_window[0][i+16]
    LDRSH   r5,[r4,#0x20]!  ; r5 = C_SB[i+24]
    LDRSH   r6,[r2,#0x20]!  ; r6 = audio_window[0][i+24]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+24]*audio_window[0][i+24]
    LDRSH   r5,[r4,#0x20]!  ; r5 = C_SB[i+32]
    LDRSH   r6,[r2,#0x20]!  ; r6 = audio_window[0][i+32]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+32]*audio_window[0][i+32]
    MOV r7,r7,ASR #1    ; y[i] >>= 1

    STMFD   sp!,{r7}
    SUB r4,r4,#0x7e ; Sets the C_SB4 pointer to "i+1"
    SUB r2,r2,#0x7e ; Sets the audio_window pointer to "i+1"
    SUBS    r12,r12,#1
    BGT loop1_SB8   ; If not 0 it takes another iteration

    MOV r12,#3      ; put "i" = 3
    
loop2_SB8
    ;; First we calculates y[i] = r7 (for "i" = {13,14,15})
    LDRSH   r5,[r4,#0x02]!  ; r5 = C_SB[i+0]
    LDRSH   r6,[r2,#0x02]!  ; r6 = audio_window[0][i+0]
    SMULBB  r7,r5,r6    ; r7 = C_SB[i+0]*audio_window[0][i+0]
    LDRSH   r5,[r4,#0x20]!  ; r5 = C_SB[i+8]
    LDRSH   r6,[r2,#0x20]!  ; r6 = audio_window[0][i+8]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+8]*audio_window[0][i+8]
    LDRSH   r5,[r4,#0x20]!  ; r5 = C_SB[i+16]
    LDRSH   r6,[r2,#0x20]!  ; r6 = audio_window[0][i+16]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+16]*audio_window[0][i+16]
    LDRSH   r5,[r4,#0x20]!  ; r5 = C_SB[i+24]
    LDRSH   r6,[r2,#0x20]!  ; r6 = audio_window[0][i+24]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+24]*audio_window[0][i+24]
    LDRSH   r5,[r4,#0x20]!  ; r5 = C_SB[i+32]
    LDRSH   r6,[r2,#0x20]!  ; r6 = audio_window[0][i+32]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[i+32]*audio_window[0][i+32]
    MOV r7,r7,ASR #1    ; y[i] >>= 1

    STMFD   sp!,{r7}
    
    SUB r4,r4,#0x80 ; Sets the C_SB4 pointer to "i+1"
    SUB r2,r2,#0x80 ; Sets the audio_window pointer to "i+1"
    SUBS    r12,r12,#1
    BGT loop2_SB8   ; If not 0 it takes another iteration

    LDMFD   sp!,{r6-r11}    ; loads y[15-9] into r6-r11
    SUB r3,r11,r6   ; r3 = (y[9] = (y[9] - y[15]))
    SUB r4,r9,r8    ; r4 = (y[11] = (y[11] - y[13]))
    SUB r8,r10,r7   ; r8 = (y[10] = (y[10] - y[14]))
    
    LDMFD   sp!,{r1,r5-r7,r9-r12,r14}   ; loads r1=y[8],r5=y[7],r6=y[6],r7=y[5],r9=y[4],r10=y[3],r11=y[2],r12=y[1],r14=y[0] respectively
    ADD r0,r12,r5   ; r0 = (y[1] = (y[1] + y[7]))
    ADD r5,r14,r1   ; r5 = (y[0] = (y[0] + y[8]))
    ADD r1,r10,r7   ; r1 = (y[3] = (y[3] + y[5]))
    ADD r7,r11,r6   ; r7 = (y[2] = (y[2] + y[6]))
    MOV r6,r9,ASR #1    ; r6 = y[4] >> 1

    STMFD   sp!,{r5-r8} ; stores y[0,4,2,10] on the stack

    ;; Now loads the cos values 3 and 7
    LDR r11,A_SB8_cos1  ; r11 = cos(1*pi/16) = cos1
    LDR r12,A_SB8_cos7  ; r12 = cos(7*pi/16) = cos7
    LDR r5,A_SB8_cos3   ; r5 = cos(3*pi/16) = cos3
    LDR r6,A_SB8_cos5   ; r6 = cos(5*pi/16) = cos5
    
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

    ;; 84 - 12 - 2 = 70

    LDMFD   sp!,{r1,r4,r3} ; loads r1 = ch and r4 = &sb_sample[current_block][0] r3 = &audio_window[0] back into our internal registers
    LDR r0,[r4] ; r0 = &sb_sample[current_block][0][0]
    
    STMIA   r0!,{r5-r12}    ; store sptr[0-7]
    
    ;; Set-up the loop for the next channel or ends
    ADD r4,r4,#4   ; r4 = &sb_sample[current_block][ch][0] (update for next iteration)
    LDR r2,[r3,#4] ; r2 = &audio_sample[ch] (update for next iteration)
    SUBS    r1,r1,#1    ; decrease number of channels by 1
    BGT chan_SB8    ; If ch counter < nr_of_channels run one more time
    
    ;; Total of 144 instructions between STMFD and LDMFD (this is 120 less than the C implementation (264))
    ;; but this is with unrolled for loops
    ;; 
    ;; So in total this yields 4+305*2+1+212 = 827 linies of instructions with 2 channels which should be 
    ;; compared to (5+3*2+5*2+27*16*2+(33+64+64+61)*2+1 = 1331 linies of instructions for the C implementation
    ;;
    ;; In short Assembly    = 827 instructions
    ;;      C       = 1331 instructions
    ;;      Reduction   = 38 % (counted in number of instructions)
    
    LDMFD sp!,{r4-r11,pc}   ; POP's the registers that where stored in the beginning

;========================================================================================================

A_SB8_cos1          DCD     0x00007D8A      ; cos1 = cos(pi/16)   = 32138 = 0x7D8A
A_SB8_cos2          DCD     0x00007642      ; cos2 = cos(2*pi/16) [NB: Remember here that cos4 = cos(4*pi/16) = cos2 - 0x1bc0]
A_SB8_cos3          DCD     0x00006A6E      ; cos3 = cos(3*pi/16) = 27246 = 0x6A6E
A_SB8_cos5          DCD     0x0000471D      ; cos5 = cos(5*pi/16) = 18205 = 0x471D
A_SB8_cos6          DCD     0x000030FC      ; cos6 = cos(6*pi/16)
A_SB8_cos7          DCD     0x000018F9      ; cos7 = cos(7*pi/16) =  6393 = 0x18F9

;========================================================================================================

C_SB8_FilterLabel       DCD     C_SB8           ; Address of C_SB8[0]

;========================================================================================================

polyPhaseAnalysisSB8_End                       ; End of function body useful when it needs to be moved

;========================================================================================================

A_SB8_CodeSize          DCD     (polyPhaseAnalysisSB8_End - polyPhaseAnalysisSB8)
A_SB8_FilterAddrOffset  DCD     (C_SB8_FilterLabel - polyPhaseAnalysisSB8)

;========================================================================================================

    ENDP                                    ; End of function definition

;========================================================================================================

    EXPORT polyPhaseAnalysisSB4 ; Makes polyPhaseAnalysisSB4 globally accessible by other C functions
        EXPORT A_SB4_CodeSize
        EXPORT A_SB4_FilterAddrOffset

    EXPORT polyPhaseAnalysisSB8 ; Makes polyPhaseAnalysisSB8 globally accessible by other C functions
        EXPORT A_SB8_CodeSize
        EXPORT A_SB8_FilterAddrOffset

    IMPORT C_SB4        ; Makes the external defined global array "C_SB4" accessible for this function
    IMPORT C_SB8        ; Makes the external defined global array "C_SB8" accessible for this function

    END
;========================================================================================================
