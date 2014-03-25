;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;      (c) Cambridge Silicon Radio Limited 2009
;;  
;;               All rights reserved
;;
;;FILE:          csr_polyphase_analysis_arm9e.s
;;
;;DESCRIPTION:   Sub-Band Codec (decoder and encoder). This file is
;;       specially designed for the ARM9E DSP-processors
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

loop_SB4
    ;; Stores &sb_sample[current_block][0] and &audio_windows[0] to the stack
    STMFD   sp!,{r5,r3}
    
    ;; First we calculates y[0] = r7 = (y[0] + y[4]) >> 16
    LDRSH   r5,[r4,#0x10]   ; r5 = C_SB[8]
    LDRSH   r6,[r2,#0x10]   ; r6 = audio_window[0][8]
    SMULBB  r7,r5,r6    ; r7 = C_SB[8]*audio_window[0][8]
    LDRSH   r5,[r4,#0x20]   ; r5 = C_SB[16]
    LDRSH   r6,[r2,#0x20]   ; r6 = audio_window[0][16]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[16]*audio_window[0][16]
    LDRSH   r5,[r4,#0x30]   ; r5 = C_SB[24]
    LDRSH   r6,[r2,#0x30]   ; r6 = audio_window[0][24]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[24]*audio_window[0][24]
    LDRSH   r5,[r4,#0x40]   ; r5 = C_SB[32]
    LDRSH   r6,[r2,#0x40]   ; r6 = audio_window[0][32]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB[32]*audio_window[0][32]
    MOV r7,r7,ASR #1    ; r11 = y[0] >> 1

    LDRSH   r5,[r4,#0x08]   ; r5 = C_SB[4]
    LDRSH   r6,[r2,#0x08]   ; r6 = audio_window[0][4]
    SMULBB  r11,r5,r6   ; r7 += C_SB[4]*audio_window[0][4]
    LDRSH   r5,[r4,#0x18]   ; r5 = C_SB[12]
    LDRSH   r6,[r2,#0x18]   ; r6 = audio_window[0][12]
    SMLABB  r11,r5,r6,r11   ; r7 += C_SB[12]*audio_window[0][12]
    LDRSH   r5,[r4,#0x28]   ; r5 = C_SB[20]
    LDRSH   r6,[r2,#0x28]   ; r6 = audio_window[0][20]
    SMLABB  r11,r5,r6,r11   ; r7 += C_SB[20]*audio_window[0][20]
    LDRSH   r5,[r4,#0x38]   ; r5 = C_SB[28]
    LDRSH   r6,[r2,#0x38]   ; r6 = audio_window[0][28]
    SMLABB  r11,r5,r6,r11   ; r7 += C_SB[28]*audio_window[0][28]
    LDRSH   r5,[r4,#0x48]   ; r5 = C_SB[36]
    LDRSH   r6,[r2,#0x48]   ; r6 = audio_window[0][36]
    SMLABB  r11,r5,r6,r11   ; r7 += C_SB[36]*audio_window[0][36]
    MOV r11,r11,ASR #1  ; r11 = y[4] >> 1
    QADD    r7,r7,r11   ; r7 += Y[4]
    
    ;; Second we calculates y[1] = r8 = (y[1] + y[3]) >> 16
    LDRSH   r5,[r4,#0x02]   ; r5 = C_SB[1]
    LDRSH   r6,[r2,#0x02]   ; r6 = audio_window[0][1]
    SMULBB  r8,r5,r6    ; r8 = C_SB[1]*audio_window[0][1]
    LDRSH   r5,[r4,#0x12]   ; r5 = C_SB[9]
    LDRSH   r6,[r2,#0x12]   ; r6 = audio_window[0][9]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB[9]*audio_window[0][9]
    LDRSH   r5,[r4,#0x22]   ; r5 = C_SB[17]
    LDRSH   r6,[r2,#0x22]   ; r6 = audio_window[0][17]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB[17]*audio_window[0][17]
    LDRSH   r5,[r4,#0x32]   ; r5 = C_SB[25]
    LDRSH   r6,[r2,#0x32]   ; r6 = audio_window[0][25]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB[25]*audio_window[0][25]
    LDRSH   r5,[r4,#0x42]   ; r5 = C_SB[33]
    LDRSH   r6,[r2,#0x42]   ; r6 = audio_window[0][33]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB[33]*audio_window[0][33]
    MOV r11,r8,ASR #1   ; r11 = y[1] >> 1

    LDRSH   r5,[r4,#0x06]   ; r5 = C_SB[3]
    LDRSH   r6,[r2,#0x06]   ; r6 = audio_window[0][3]
    SMULBB  r8,r5,r6    ; r8 += C_SB[3]*audio_window[0][3]
    LDRSH   r5,[r4,#0x16]   ; r5 = C_SB[11]
    LDRSH   r6,[r2,#0x16]   ; r6 = audio_window[0][11]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB[11]*audio_window[0][11]
    LDRSH   r5,[r4,#0x26]   ; r5 = C_SB[19]
    LDRSH   r6,[r2,#0x26]   ; r6 = audio_window[0][19]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB[19]*audio_window[0][19]
    LDRSH   r5,[r4,#0x36]   ; r5 = C_SB[27]
    LDRSH   r6,[r2,#0x36]   ; r6 = audio_window[0][27]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB[27]*audio_window[0][27]
    LDRSH   r5,[r4,#0x46]   ; r5 = C_SB[35]
    LDRSH   r6,[r2,#0x46]   ; r6 = audio_window[0][35]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB[35]*audio_window[0][35]
    MOV r8,r8,ASR #1    ; r8 = y[3] >> 1
    QADD    r8,r8,r11
        
    ;; Thierd we calculates y[2] = r9 = y[2] >> 15
    LDRSH   r5,[r4,#0x04]   ; r5 = C_SB[2]
    LDRSH   r6,[r2,#0x04]   ; r6 = audio_window[0][2]
    SMULBB  r9,r5,r6    ; r9 = C_SB[2]*audio_window[0][2]
    LDRSH   r5,[r4,#0x14]   ; r5 = C_SB[10]
    LDRSH   r6,[r2,#0x14]   ; r6 = audio_window[0][10 ]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB[10]*audio_window[0][10]
    LDRSH   r5,[r4,#0x24]   ; r5 = C_SB[18]
    LDRSH   r6,[r2,#0x24]   ; r6 = audio_window[0][18]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB[18]*audio_window[0][18]
    LDRSH   r5,[r4,#0x34]   ; r5 = C_SB[26]
    LDRSH   r6,[r2,#0x34]   ; r6 = audio_window[0][26]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB[26]*audio_window[0][26]
    LDRSH   r5,[r4,#0x44]   ; r5 = C_SB[34]
    LDRSH   r6,[r2,#0x44]   ; r6 = audio_window[0][34]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB[34]*audio_window[0][34]
    MOVS    r9,r9,ASR #2    ; r9 = y[2] >> 2
    ADC r9,r9,#0    ; correct rounding (NB: migth be removed for speed optimization)

    ;; Fourth we calculates y[5] = r10 = y[5] 
    LDRSH   r5,[r4,#0x0a]   ; r5 = C_SB[5]
    LDRSH   r6,[r2,#0x0a]   ; r6 = audio_window[0][5]
    SMULBB  r10,r5,r6   ; r10 = C_SB[5]*audio_window[0][5]
    LDRSH   r5,[r4,#0x1a]   ; r5 = C_SB[13]
    LDRSH   r6,[r2,#0x1a]   ; r6 = audio_window[0][13]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB[13]*audio_window[0][13]
    LDRSH   r5,[r4,#0x2a]   ; r5 = C_SB[21]
    LDRSH   r6,[r2,#0x2a]   ; r6 = audio_window[0][21]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB[21]*audio_window[0][21]
    LDRSH   r5,[r4,#0x3a]   ; r5 = C_SB[29]
    LDRSH   r6,[r2,#0x3a]   ; r6 = audio_window[0][29]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB[29]*audio_window[0][29]
    LDRSH   r5,[r4,#0x4a]   ; r5 = C_SB[37]
    LDRSH   r6,[r2,#0x4a]   ; r6 = audio_window[0][37]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB[37]*audio_window[0][37]
    MOV r10,r10,ASR #1  ; r10 = y[5] >> 1      (100 instructions to here)

    ;; Fifth we calculates y[7] = r11 = y[7]
    LDRSH   r5,[r4,#0x0e]   ; r5 = C_SB[7]
    LDRSH   r6,[r2,#0x0e]   ; r6 = audio_window[0][7]
    SMULBB  r11,r5,r6   ; r11 = C_SB[7]*audio_window[0][7]
    LDRSH   r5,[r4,#0x1e]   ; r5 = C_SB[15]
    LDRSH   r6,[r2,#0x1e]   ; r6 = audio_window[0][15]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB[15]*audio_window[0][15]
    LDRSH   r5,[r4,#0x2e]   ; r5 = C_SB[23]
    LDRSH   r6,[r2,#0x2e]   ; r6 = audio_window[0][23]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB[23]*audio_window[0][23]
    LDRSH   r5,[r4,#0x3e]   ; r5 = C_SB[31]
    LDRSH   r6,[r2,#0x3e]   ; r6 = audio_window[0][31]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB[31]*audio_window[0][31]
    LDRSH   r5,[r4,#0x4e]   ; r5 = C_SB[39]
    LDRSH   r6,[r2,#0x4e]   ; r6 = audio_window[0][39]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB[39]*audio_window[0][39]
    MOV r11,r11,ASR #1  ; r11 = y[7] >> 1
    
    ;; Sixth we calculate y[5] = r10 = y[5] - y[7]) >> 16
    QSUB    r10,r10,r11

    ;; 103 Instructions so far (117)

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
    BGT loop_SB4    ; If ch counter < nr_of_channels run one more time
    
    ;; Total of 131 instructions between STMFD and LDMFD (this is 35 more than the C implementation (96))
    ;; but this is with unrolled for loops
    ;; 
    ;; So in total this yields 4+132*2 = 268 linies of instructions with 2 channels which should be 
    ;; compared to (7+2+4*2+26*8*2+18*2+40*2+1) = 550 linies of instructions for the C implementation
    ;;
    ;; In short Assembly    = 268 instructions
    ;;      C       = 550 instructions
    ;;      Reduction   = 51 % (counted in number of instructions)

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

loop_SB8
    ;; Stores nrof_channels, &sb_sample[current_block][0] and &audio_windows[0] to the stack
    STMFD   sp!,{r1,r4,r3}
    
    LDR r4,C_SB8_FilterLabel; Stores the pointer to C_SB8 in r4
    
    ;; First we calculates y[0] = r7 = (y[0] + y[8]) >> 1
    LDRSH   r5,[r4,#0x20]   ; r5 = C_SB8[16]
    LDRSH   r6,[r2,#0x20]   ; r6 = audio_window[0][16]
    SMULBB  r7,r5,r6    ; r7 = C_SB8[16]*audio_window[0][16]
    LDRSH   r5,[r4,#0x40]   ; r5 = C_SB8[32]
    LDRSH   r6,[r2,#0x40]   ; r6 = audio_window[0][32]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB8[32]*audio_window[0][32]
    LDRSH   r5,[r4,#0x60]   ; r5 = C_SB8[48]
    LDRSH   r6,[r2,#0x60]   ; r6 = audio_window[0][48]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB8[48]*audio_window[0][48]
    LDRSH   r5,[r4,#0x80]   ; r5 = C_SB8[64]
    LDRSH   r6,[r2,#0x80]   ; r6 = audio_window[0][64]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB8[64]*audio_window[0][64]
    MOV r11,r7,ASR #1   ; r11 = y[0] >> 1
    
    LDRSH   r5,[r4,#0x10]   ; r5 = C_SB8[8]
    LDRSH   r6,[r2,#0x10]   ; r6 = audio_window[0][8]
    SMULBB  r7,r5,r6    ; r7 += C_SB8[8]*audio_window[0][8]
    LDRSH   r5,[r4,#0x30]   ; r5 = C_SB8[24]
    LDRSH   r6,[r2,#0x30]   ; r6 = audio_window[0][24]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB8[24]*audio_window[0][24]
    LDRSH   r5,[r4,#0x50]   ; r5 = C_SB8[40]
    LDRSH   r6,[r2,#0x50]   ; r6 = audio_window[0][40]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB8[40]*audio_window[0][40]
    LDRSH   r5,[r4,#0x70]   ; r5 = C_SB8[54]
    LDRSH   r6,[r2,#0x70]   ; r6 = audio_window[0][54]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB8[54]*audio_window[0][54]
    LDRSH   r5,[r4,#0x90]   ; r5 = C_SB8[72]
    LDRSH   r6,[r2,#0x90]   ; r6 = audio_window[0][72]
    SMLABB  r7,r5,r6,r7 ; r7 += C_SB8[72]*audio_window[0][72]
    MOV r7,r7,ASR #1    ; r7 = y[8] >> 1
    QADD    r7,r7,r11   ; r7 = (y[0] + y[8]) >> 1
    
    ;; Second we calculates y[1] = r8 = (y[1] + y[7]) >> 1
    LDRSH   r5,[r4,#0x02]   ; r5 = C_SB8[1]
    LDRSH   r6,[r2,#0x02]   ; r6 = audio_window[0][1]
    SMULBB  r8,r5,r6    ; r8 = C_SB8[1]*audio_window[0][1]
    LDRSH   r5,[r4,#0x22]   ; r5 = C_SB8[17]
    LDRSH   r6,[r2,#0x22]   ; r6 = audio_window[0][17]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB8[17]*audio_window[0][17]
    LDRSH   r5,[r4,#0x42]   ; r5 = C_SB8[33]
    LDRSH   r6,[r2,#0x42]   ; r6 = audio_window[0][33]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB8[33]*audio_window[0][33]
    LDRSH   r5,[r4,#0x62]   ; r5 = C_SB8[49]
    LDRSH   r6,[r2,#0x62]   ; r6 = audio_window[0][49]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB8[49]*audio_window[0][49]
    LDRSH   r5,[r4,#0x82]   ; r5 = C_SB8[65]
    LDRSH   r6,[r2,#0x82]   ; r6 = audio_window[0][65]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB8[65]*audio_window[0][65]
    MOV r11,r8,ASR #1   ; r11 = y[1] >> 1

    LDRSH   r5,[r4,#0x0e]   ; r5 = C_SB8[7]
    LDRSH   r6,[r2,#0x0e]   ; r6 = audio_window[0][7]
    SMULBB  r8,r5,r6    ; r8 += C_SB8[7]*audio_window[0][7]
    LDRSH   r5,[r4,#0x2e]   ; r5 = C_SB8[23]
    LDRSH   r6,[r2,#0x2e]   ; r6 = audio_window[0][23]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB8[23]*audio_window[0][23]
    LDRSH   r5,[r4,#0x4e]   ; r5 = C_SB8[39]
    LDRSH   r6,[r2,#0x4e]   ; r6 = audio_window[0][39]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB8[39]*audio_window[0][39]
    LDRSH   r5,[r4,#0x6e]   ; r5 = C_SB8[55]
    LDRSH   r6,[r2,#0x6e]   ; r6 = audio_window[0][55]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB8[55]*audio_window[0][55]
    LDRSH   r5,[r4,#0x8e]   ; r5 = C_SB8[71]
    LDRSH   r6,[r2,#0x8e]   ; r6 = audio_window[0][71]
    SMLABB  r8,r5,r6,r8 ; r8 += C_SB8[71]*audio_window[0][71]
    MOV r8,r8,ASR #1    ; r8 = y[7] >> 1
    QADD    r0,r8,r11   ; r0 = (y[1] + y[7]) >> 1

    ;; Theird we calculates y[2] = r9 = (y[2] + y[6]) >> 1
    LDRSH   r5,[r4,#0x04]   ; r5 = C_SB8[2]
    LDRSH   r6,[r2,#0x04]   ; r6 = audio_window[0][2]
    SMULBB  r9,r5,r6    ; r9 = C_SB8[2]*audio_window[0][2]
    LDRSH   r5,[r4,#0x24]   ; r5 = C_SB8[18]
    LDRSH   r6,[r2,#0x24]   ; r6 = audio_window[0][18]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB8[18]*audio_window[0][18]
    LDRSH   r5,[r4,#0x44]   ; r5 = C_SB8[34]
    LDRSH   r6,[r2,#0x44]   ; r6 = audio_window[0][34]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB8[33]*audio_window[0][34]
    LDRSH   r5,[r4,#0x64]   ; r5 = C_SB8[50]
    LDRSH   r6,[r2,#0x64]   ; r6 = audio_window[0][50]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB8[50]*audio_window[0][50]
    LDRSH   r5,[r4,#0x84]   ; r5 = C_SB8[66]
    LDRSH   r6,[r2,#0x84]   ; r6 = audio_window[0][66]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB8[66]*audio_window[0][66]
    MOV r11,r9,ASR #1   ; r11 = y[2] >> 1

    LDRSH   r5,[r4,#0x0c]   ; r5 = C_SB8[6]
    LDRSH   r6,[r2,#0x0c]   ; r6 = audio_window[0][6]
    SMULBB  r9,r5,r6    ; r9 += C_SB8[6]*audio_window[0][6]
    LDRSH   r5,[r4,#0x2c]   ; r5 = C_SB8[22]
    LDRSH   r6,[r2,#0x2c]   ; r6 = audio_window[0][22]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB8[22]*audio_window[0][22]
    LDRSH   r5,[r4,#0x4c]   ; r5 = C_SB8[38]
    LDRSH   r6,[r2,#0x4c]   ; r6 = audio_window[0][38]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB8[38]*audio_window[0][38]
    LDRSH   r5,[r4,#0x6c]   ; r5 = C_SB8[54]
    LDRSH   r6,[r2,#0x6c]   ; r6 = audio_window[0][54]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB8[54]*audio_window[0][54]
    LDRSH   r5,[r4,#0x8c]   ; r5 = C_SB8[70]
    LDRSH   r6,[r2,#0x8c]   ; r6 = audio_window[0][70]
    SMLABB  r9,r5,r6,r9 ; r9 += C_SB8[70]*audio_window[0][70]
    MOV r9,r9,ASR #1    ; r9 = y[6] >> 1
    QADD    r9,r9,r11   ; r9 = (y[2] + y[6]) >> 1
    ;; 100
    ;; Fourth we calculates y[3] = r10 = (y[3] + y[5]) >> 1
    LDRSH   r5,[r4,#0x06]   ; r5 = C_SB8[3]
    LDRSH   r6,[r2,#0x06]   ; r6 = audio_window[0][3]
    SMULBB  r10,r5,r6   ; r10 = C_SB8[3]*audio_window[0][3]
    LDRSH   r5,[r4,#0x26]   ; r5 = C_SB8[19]
    LDRSH   r6,[r2,#0x26]   ; r6 = audio_window[0][19]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[19]*audio_window[0][19]
    LDRSH   r5,[r4,#0x46]   ; r5 = C_SB8[35]
    LDRSH   r6,[r2,#0x46]   ; r6 = audio_window[0][35]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[35]*audio_window[0][35]
    LDRSH   r5,[r4,#0x66]   ; r5 = C_SB8[51]
    LDRSH   r6,[r2,#0x66]   ; r6 = audio_window[0][51]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[51]*audio_window[0][51]
    LDRSH   r5,[r4,#0x86]   ; r5 = C_SB8[67]
    LDRSH   r6,[r2,#0x86]   ; r6 = audio_window[0][67]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[67]*audio_window[0][67]
    MOV r11,r10,ASR #1  ; r11 = y[3] >> 1

    LDRSH   r5,[r4,#0x0a]   ; r5 = C_SB8[5]
    LDRSH   r6,[r2,#0x0a]   ; r6 = audio_window[0][5]
    SMULBB  r10,r5,r6   ; r10 += C_SB8[5]*audio_window[0][5]
    LDRSH   r5,[r4,#0x2a]   ; r5 = C_SB8[21]
    LDRSH   r6,[r2,#0x2a]   ; r6 = audio_window[0][21]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[21]*audio_window[0][21]
    LDRSH   r5,[r4,#0x4a]   ; r5 = C_SB8[37]
    LDRSH   r6,[r2,#0x4a]   ; r6 = audio_window[0][37]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[37]*audio_window[0][37]
    LDRSH   r5,[r4,#0x6a]   ; r5 = C_SB8[53]
    LDRSH   r6,[r2,#0x6a]   ; r6 = audio_window[0][53]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[53]*audio_window[0][53]
    LDRSH   r5,[r4,#0x8a]   ; r5 = C_SB8[69]
    LDRSH   r6,[r2,#0x8a]   ; r6 = audio_window[0][69]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[69]*audio_window[0][69]
    MOV r10,r10,ASR #1  ; r10 = y[5] >> 1
    QADD    r1,r10,r11  ; r1 = (y[3] + y[5]) >> 1
    
    ;; Fifth we calculates y[4] = r11 = y[4] >> 2
    LDRSH   r5,[r4,#0x08]   ; r5 = C_SB8[4]
    LDRSH   r6,[r2,#0x08]   ; r6 = audio_window[0][4]
    SMULBB  r11,r5,r6   ; r11 = C_SB8[4]*audio_window[0][4]
    LDRSH   r5,[r4,#0x28]   ; r5 = C_SB8[20]
    LDRSH   r6,[r2,#0x28]   ; r6 = audio_window[0][20]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[20]*audio_window[0][20]
    LDRSH   r5,[r4,#0x48]   ; r5 = C_SB8[36]
    LDRSH   r6,[r2,#0x48]   ; r6 = audio_window[0][36]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[36]*audio_window[0][36]
    LDRSH   r5,[r4,#0x68]   ; r5 = C_SB8[52]
    LDRSH   r6,[r2,#0x68]   ; r6 = audio_window[0][52]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[52]*audio_window[0][52]
    LDRSH   r5,[r4,#0x88]   ; r5 = C_SB8[68]
    LDRSH   r6,[r2,#0x88]   ; r6 = audio_window[0][68]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[68]*audio_window[0][68]
    MOVS    r8,r11,ASR #2   ; r8 = y[4] >> 2
    ADC r8,r8,#0    ; correct rounding (NB: migth be removed for speed optimization)

    ;; Sixth we calculates y[9] = r10 = y[9] 
    LDRSH   r5,[r4,#0x12]   ; r5 = C_SB8[9]
    LDRSH   r6,[r2,#0x12]   ; r6 = audio_window[0][9]
    SMULBB  r10,r5,r6   ; r10 = C_SB8[9]*audio_window[0][9]
    LDRSH   r5,[r4,#0x32]   ; r5 = C_SB8[25]
    LDRSH   r6,[r2,#0x32]   ; r6 = audio_window[0][25]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[25]*audio_window[0][25]
    LDRSH   r5,[r4,#0x52]   ; r5 = C_SB8[41]
    LDRSH   r6,[r2,#0x52]   ; r6 = audio_window[0][41]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[41]*audio_window[0][41]
    LDRSH   r5,[r4,#0x72]   ; r5 = C_SB8[57]
    LDRSH   r6,[r2,#0x72]   ; r6 = audio_window[0][57]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[57]*audio_window[0][57]
    LDRSH   r5,[r4,#0x92]   ; r5 = C_SB8[73]
    LDRSH   r6,[r2,#0x92]   ; r6 = audio_window[0][73]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[73]*audio_window[0][73]
    MOV r10,r10,ASR #1  ; r10 = y[9] >> 1

    ;; Seventh we calculates y[15] = r11 = y[15]
    LDRSH   r5,[r4,#0x1e]   ; r5 = C_SB8[15]
    LDRSH   r6,[r2,#0x1e]   ; r6 = audio_window[0][15]
    SMULBB  r11,r5,r6   ; r11 = C_SB8[15]*audio_window[0][15]
    LDRSH   r5,[r4,#0x3e]   ; r5 = C_SB8[31]
    LDRSH   r6,[r2,#0x3e]   ; r6 = audio_window[0][31]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[31]*audio_window[0][31]
    LDRSH   r5,[r4,#0x5e]   ; r5 = C_SB8[47]
    LDRSH   r6,[r2,#0x5e]   ; r6 = audio_window[0][47]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[47]*audio_window[0][47]
    LDRSH   r5,[r4,#0x7e]   ; r5 = C_SB8[63]
    LDRSH   r6,[r2,#0x7e]   ; r6 = audio_window[0][63]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[63]*audio_window[0][63]
    LDRSH   r5,[r4,#0x9e]   ; r5 = C_SB8[79]
    LDRSH   r6,[r2,#0x9e]   ; r6 = audio_window[0][79]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[79]*audio_window[0][79]
    MOV r11,r11,ASR #1  ; r11 = y[15] >> 1
    
    ;; Eigth we calculate y[9] = r3 = (y[9] - y[15]) >> 1
    QSUB    r3,r10,r11

    ;; Nineth we calculates y[10] = r10 = y[10] 
    LDRSH   r5,[r4,#0x14]   ; r5 = C_SB8[10]
    LDRSH   r6,[r2,#0x14]   ; r6 = audio_window[0][10]
    SMULBB  r10,r5,r6   ; r10 = C_SB8[10]*audio_window[0][10]
    LDRSH   r5,[r4,#0x34]   ; r5 = C_SB8[26]
    LDRSH   r6,[r2,#0x34]   ; r6 = audio_window[0][26]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[26]*audio_window[0][26]
    LDRSH   r5,[r4,#0x54]   ; r5 = C_SB8[42]
    LDRSH   r6,[r2,#0x54]   ; r6 = audio_window[0][42]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[42]*audio_window[0][42]
    LDRSH   r5,[r4,#0x74]   ; r5 = C_SB8[58]
    LDRSH   r6,[r2,#0x74]   ; r6 = audio_window[0][58]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[58]*audio_window[0][58]
    LDRSH   r5,[r4,#0x94]   ; r5 = C_SB8[74]
    LDRSH   r6,[r2,#0x94]   ; r6 = audio_window[0][74]
    SMLABB  r10,r5,r6,r10   ; r10 += C_SB8[74]*audio_window[0][74]
    MOV r10,r10,ASR #1  ; r10 = y[10] >> 1

    ;; Tenth we calculates y[14] = r11 = y[14]
    LDRSH   r5,[r4,#0x1c]   ; r5 = C_SB8[14]
    LDRSH   r6,[r2,#0x1c]   ; r6 = audio_window[0][14]
    SMULBB  r11,r5,r6   ; r11 = C_SB8[14]*audio_window[0][14]
    LDRSH   r5,[r4,#0x3c]   ; r5 = C_SB8[30]
    LDRSH   r6,[r2,#0x3c]   ; r6 = audio_window[0][30]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[30]*audio_window[0][30]
    LDRSH   r5,[r4,#0x5c]   ; r5 = C_SB8[46]
    LDRSH   r6,[r2,#0x5c]   ; r6 = audio_window[0][46]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[46]*audio_window[0][46]
    LDRSH   r5,[r4,#0x7c]   ; r5 = C_SB8[62]
    LDRSH   r6,[r2,#0x7c]   ; r6 = audio_window[0][62]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[62]*audio_window[0][62]
    LDRSH   r5,[r4,#0x9c]   ; r5 = C_SB8[78]
    LDRSH   r6,[r2,#0x9c]   ; r6 = audio_window[0][78]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[78]*audio_window[0][78]
    MOV r11,r11,ASR #1  ; r11 = y[14] >> 1
    
    ;; Eleventh we calculate y[10] = r10 = (y[10] - y[14]) >> 1
    QSUB    r10,r10,r11
    ;; 215
    ;; Tvelveth we calculates y[11] = r10 = y[11] 
    LDRSH   r5,[r4,#0x16]   ; r5 = C_SB8[11]
    LDRSH   r6,[r2,#0x16]   ; r6 = audio_window[0][11]
    SMULBB  r12,r5,r6   ; r12 = C_SB8[11]*audio_window[0][11]
    LDRSH   r5,[r4,#0x36]   ; r5 = C_SB8[27]
    LDRSH   r6,[r2,#0x36]   ; r6 = audio_window[0][27]
    SMLABB  r12,r5,r6,r12   ; r12 += C_SB8[27]*audio_window[0][27]
    LDRSH   r5,[r4,#0x56]   ; r5 = C_SB8[43]
    LDRSH   r6,[r2,#0x56]   ; r6 = audio_window[0][43]
    SMLABB  r12,r5,r6,r12   ; r12 += C_SB8[43]*audio_window[0][43]
    LDRSH   r5,[r4,#0x76]   ; r5 = C_SB8[59]
    LDRSH   r6,[r2,#0x76]   ; r6 = audio_window[0][59]
    SMLABB  r12,r5,r6,r12   ; r12 += C_SB8[59]*audio_window[0][59]
    LDRSH   r5,[r4,#0x96]   ; r5 = C_SB8[75]
    LDRSH   r6,[r2,#0x96]   ; r6 = audio_window[0][75]
    SMLABB  r12,r5,r6,r12   ; r12 += C_SB8[75]*audio_window[0][75]
    MOV r12,r12,ASR #1  ; r12 = y[11] >> 1

    ;; Tenth we calculates y[13] = r11 = y[13]
    LDRSH   r5,[r4,#0x1a]   ; r5 = C_SB8[13]
    LDRSH   r6,[r2,#0x1a]   ; r6 = audio_window[0][13]
    SMULBB  r11,r5,r6   ; r11 = C_SB8[13]*audio_window[0][13]
    LDRSH   r5,[r4,#0x3a]   ; r5 = C_SB8[29]
    LDRSH   r6,[r2,#0x3a]   ; r6 = audio_window[0][29]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[29]*audio_window[0][29]
    LDRSH   r5,[r4,#0x5a]   ; r5 = C_SB8[45]
    LDRSH   r6,[r2,#0x5a]   ; r6 = audio_window[0][45]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[45]*audio_window[0][45]
    LDRSH   r5,[r4,#0x7a]   ; r5 = C_SB8[61]
    LDRSH   r6,[r2,#0x7a]   ; r6 = audio_window[0][61]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[61]*audio_window[0][61]
    LDRSH   r5,[r4,#0x9a]   ; r5 = C_SB8[77]
    LDRSH   r6,[r2,#0x9a]   ; r6 = audio_window[0][77]
    SMLABB  r11,r5,r6,r11   ; r11 += C_SB8[77]*audio_window[0][77]
    MOV r11,r11,ASR #1  ; r11 = y[13] >> 1
    
    ;; Eleventh we calculate y[11] = r12 = (y[11] - y[13]) >> 1
    QSUB    r4,r12,r11

    ;; This is just a little restructing before status
    ;; Stores y[10] in the temporary buffer
    STMFD   sp!,{r7-r10}    ; stores y[0,2,4,10] to the stack

    ;; Now loads the cos values 3 and 7
    LDR r11,A_SB8_cos1  ; r11 = cos(1*pi/16) = cos1
    LDR r12,A_SB8_cos7  ; r12 = cos(7*pi/16) = cos7
    LDR r5,A_SB8_cos3   ; r5 = cos(3*pi/16) = cos3
    LDR r6,A_SB8_cos5   ; r6 = cos(5*pi/16) = cos5

    ;; 253 (new version)
    ;; Status
    ;; 234 Instructions so far 

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
    BGT loop_SB8    ; If ch counter < nr_of_channels run one more time
    ;; 115 + 253 = 368
    ;; Total of 310 instructions between STMFD and LDMFD (this is 46 more than the C implementation (264))
    ;; but this is with unrolled for loops
    ;; 
    ;; So in total this yields 4+305*2+1 = 615 linies of instructions with 2 channels which should be 
    ;; compared to (5+3*2+5*2+27*16*2+(33+64+64+61)*2+1 = 1331 linies of instructions for the C implementation
    ;;
    ;; In short Assembly    = 615 instructions
    ;;      C       = 1331 instructions
    ;;      Reduction   = 54 % (counted in number of instructions)
    
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
