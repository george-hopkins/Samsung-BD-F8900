#ifndef CSR_AC97_H__
#define CSR_AC97_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Return values */
#define AC97_RESULT_SUCCESS          0
#define AC97_RESULT_FAILURE         -1
#define AC97_RESULT_NO_MORE_SLOTS   -2


/*****************************************/
/********* AC97 Power Management *********/
/*****************************************/

/* Reset types applicable to AC97Reset */
#define AC97_RESET_TYPE_COLD            0   /* Resets registers, activates AC97 link and full power up */
#define AC97_RESET_TYPE_WARM            1   /* Activates AC97 link and full power up */
#define AC97_RESET_TYPE_REGISTER        2   /* Resets registers and full power up (requires active AC97 link) */

/* Reset and power up. Initial state is powered down, a cold or warm reset is mandatory to enable communication */
CsrInt32 AC97Reset(CsrInt32 type);

/* Power down the entire chip. To wake it up again, a cold or warm reset is required. */
CsrInt32 AC97PowerDown(void);


/*********************************/
/********* AC97 Commands *********/
/*********************************/

/* Write a value to an AC97 register */
CsrInt32 AC97CmdWrite(CsrUint8 address, CsrUint16 data);

/* Write a value from an AC97 register */
CsrInt32 AC97CmdRead(CsrUint8 address, CsrUint16 *data);

/* Read/Modify/Write a value to an AC97 register. Sets the value of address[msb:lsb] to value of data and preserves other bits. */
CsrInt32 AC97CmdReadModifyWrite(CsrUint8 address, CsrUint8 msb, CsrUint8 lsb, CsrUint16 data);


/****************************************************/
/********* GSM Input Configuration (MONOIN) *********/
/****************************************************/

#define AC97_GSM_INPUT_GAIN(gain)   (gain & 0x1F)   /* 0: +12.0dB - 1.5dB Steps - 31: -34.5dB */

/* Set the gain of the GSM PGA (Programmable Gain Amplifier) (Registers: 0x08) */
CsrInt32 AC97GsmInputGainSet(CsrUint8 gain);


/*****************************************************/
/********* CSR Input Configuration (LINEL/R) *********/
/*****************************************************/

/* Channels */
#define AC97_CSR_INPUT_LEFT         1
#define AC97_CSR_INPUT_RIGHT        2
#define AC97_CSR_INPUT_BOTH         (AC97_CSR_INPUT_LEFT | AC97_CSR_INPUT_RIGHT)

#define AC97_GSR_INPUT_GAIN(gain)   (gain & 0x1F)   /* 0: +12.0dB - 1.5dB Steps - 31: -34.5dB */

/* Set the gain of the CSR PGAs (Programmable Gain Amplifier) (Registers: 0x0A) */
CsrInt32 AC97CsrInputGainSet(CsrInt32 channel, CsrUint8 gain);


/*******************************************/
/********* DAC Input Configuration *********/
/*******************************************/

/* Channels */
#define AC97_DAC_LEFT                   1
#define AC97_DAC_RIGHT                  2
#define AC97_DAC_BOTH                   (AC97_DAC_LEFT | AC97_DAC_RIGHT)

/* DAC sample rates */
#define AC97_DAC_SAMPLERATE_8000HZ      0x1F40
#define AC97_DAC_SAMPLERATE_11025HZ     0x2B11
#define AC97_DAC_SAMPLERATE_12000HZ     0x2EE0
#define AC97_DAC_SAMPLERATE_16000HZ     0x3E80
#define AC97_DAC_SAMPLERATE_22050HZ     0x5622
#define AC97_DAC_SAMPLERATE_24000HZ     0x5DC0
#define AC97_DAC_SAMPLERATE_32000HZ     0x7D00
#define AC97_DAC_SAMPLERATE_44100HZ     0xAC44
#define AC97_DAC_SAMPLERATE_48000HZ     0xBB80     /* Default */

#define AC97_DAC_GAIN_UNITY             8
#define AC97_DAC_GAIN(gain)             (gain & 0x1F)   /* 0: +12.0dB - 1.5dB Steps - 31: -34.5dB */

/* Set the sample rate used by the DAC (Registers: 0x2A, 0x2C) */
CsrInt32 AC97DacSampleRateSet(CsrUint16 sampleRate);

/* Set the gain of the DAC PGA's (Programmable Gain Amplifier) (Registers: 0x0C) */
CsrInt32 AC97DacGainSet(CsrInt32 channel, CsrUint8 gain);


/***************************************************************/
/********* Microphone Input Configuration (MICA, MICB) *********/
/***************************************************************/

/* Microphones */
#define AC97_MICROPHONE_A               1
#define AC97_MICROPHONE_B               2
#define AC97_MICROPHONE_BOTH            (AC97_MICROPHONE_A | AC97_MICROPHONE_A)

/* Valid boosts for AC97MicrophoneInputBoostSet */
#define AC97_MICROPHONE_BOOST_12DB      00
#define AC97_MICROPHONE_BOOST_18DB      01
#define AC97_MICROPHONE_BOOST_24DB      10
#define AC97_MICROPHONE_BOOST_30DB      11

/* Valid gains for AC97MicrophoneGainSet */
#define AC97_MICROPHONE_GAIN_UNITY      8
#define AC97_MICROPHONE_GAIN(gain)      (gain & 0x1F)   /* 0: +12.0dB - 1.5dB Steps - 31: -34.5dB */

/* Set the boost of the microphone input amplifier (Registers: 0x22) */
CsrInt32 AC97MicrophoneInputBoostSet(CsrInt32 microphone, CsrUint8 boost);

/* Set the gain of the microphone PGA (Programmable Gain Amplifier) (Registers: 0x0E) */
CsrInt32 AC97MicrophoneGainSet(CsrInt32 microphone, CsrUint8 gain);


/*******************************************/
/********* ADC Configuration (ADC) *********/
/*******************************************/

/* ADC Channels */
#define AC97_ADC_LEFT                    1
#define AC97_ADC_RIGHT                   2
#define AC97_ADC_BOTH                    (AC97_ADC_LEFT | AC97_ADC_RIGHT)

/* Valid sources for AC97AdcSourceSet */
#define AC97_ADC_SOURCE_MICA             0       /* From MICA */
#define AC97_ADC_SOURCE_MICB             1       /* From MICB */
#define AC97_ADC_SOURCE_CSR              2       /* From CSR module (LINEL/R) */
#define AC97_ADC_SOURCE_GSM              3       /* From GSM module (MONOIN)*/
#define AC97_ADC_SOURCE_HPMIX            4       /* From Headphone Mixer Output (stereo) */
#define AC97_ADC_SOURCE_SPKMIX           5       /* From Speaker Mixer Output (mono) */
#define AC97_ADC_SOURCE_GSMMIX           6       /* From MONO Mixer Output (mono) */

/* Valid gains for AC97AdcGainSet */
#define AC97_ADC_GAIN_MUTE               (0x80)                      /* Mutes both channels */
#define AC97_ADC_GAIN_AUTOMATIC(maxgain) (0x40 | (maxgain & 0x7))    /* 0: -12dB - 6dB Steps - 7: +30dB (maxgain is common to both channels, unmutes both channels) */
#define AC97_ADC_GAIN_FIXED(gain)        (gain & 0x3F)               /* 0: -17.25dB - 0.75dB Steps - 63: +30dB (unmutes both channels) */

/* ADC sample rates */
#define AC97_ADC_SAMPLERATE_8000HZ       0x1F40
#define AC97_ADC_SAMPLERATE_11025HZ      0x2B11
#define AC97_ADC_SAMPLERATE_12000HZ      0x2EE0
#define AC97_ADC_SAMPLERATE_16000HZ      0x3E80
#define AC97_ADC_SAMPLERATE_22050HZ      0x5622
#define AC97_ADC_SAMPLERATE_24000HZ      0x5DC0
#define AC97_ADC_SAMPLERATE_32000HZ      0x7D00
#define AC97_ADC_SAMPLERATE_44100HZ      0xAC44
#define AC97_ADC_SAMPLERATE_48000HZ      0xBB80      /* Default */

/* Set the source for the ADC (Registers: 0x14) */
CsrInt32 AC97AdcSourceSet(CsrInt32 channel, CsrUint8 source);

/* Set the gain of the ADC (Registers: 0x62, 0x12) */
CsrInt32 AC97AdcGainSet(CsrInt32 channel, CsrUint8 gain);

/* Enable/disable the +20dB boost on ADC inputs (both channels) (Registers: 0x14) */
CsrInt32 AC97AdcBoostSet(CsrUint8 enable);

/* Set the sample rate used by the ADC (Registers: 0x2A, 0x32) */
CsrInt32 AC97AdcSampleRateSet(CsrUint16 sampleRate);


/*******************************************************/
/********* PCM Interface Configuration (VXDAC) *********/
/*******************************************************/

#define AC_PCMDATA                      ((volatile CsrUint32 *) 0x7F001018)
#define AC_MICDATA                      ((volatile CsrUint32 *) 0x7F00101C)

#define AC97_PCM_MODE_DISABLE           0   /* PCMCLK, PCMFS in Hi-Z - Default */
#define AC97_PCM_MODE_SLAVE             1   /* PCMCLK, PCMFS is input */
#define AC97_PCM_MODE_MASTER            2   /* PCMCLK, PCMFS is output */
#define AC97_PCM_MODE_PARTIALMASTER     3   /* PCMCLK is output, PCMFS is input */

#define AC97_PCM_FORMAT_RIGHT           0   /* Right justified */
#define AC97_PCM_FORMAT_RIGHT_INV       4   /* Right justified, PCMFS inverted */
#define AC97_PCM_FORMAT_LEFT            1   /* Left justified */
#define AC97_PCM_FORMAT_LEFT_INV        5   /* Left justified, PCMFS inverted */
#define AC97_PCM_FORMAT_I2S             2   /* I2S */
#define AC97_PCM_FORMAT_I2S_INV         6   /* I2S, PCMFS inverted */
#define AC97_PCM_FORMAT_DSP_B           7   /* DSP Format, MSB available on 1st rising edge of PCMCLK */
#define AC97_PCM_FORMAT_DSP_A           3   /* DSP Format, MSB available on 2nd rising edge of PCMCLK */

#define AC97_PCM_WORDLENGTH_16          0   /* 16bit samples */
#define AC97_PCM_WORDLENGTH_20          1   /* 20bit samples */
#define AC97_PCM_WORDLENGTH_24          2   /* 24bit samples */
#define AC97_PCM_WORDLENGTH_32          3   /* 32bit samples. Not compatible with AC97_PCM_FORMAT_RIGHT/AC97_PCM_FORMAT_RIGHT_INV */

#define AC97_PCM_SOURCE_LEFTRIGHT       0   /* Stereo - Output left and right ADC data */
#define AC97_PCM_SOURCE_SWAP            1   /* Stereo - Output left and right ADC data swapped */
#define AC97_PCM_SOURCE_LEFT            2   /* Mono - Output left ADC data only (must use AC97_PCM_FORMAT_DSP_A/AC97_PCM_FORMAT_DSP_B) */
#define AC97_PCM_SOURCE_RIGHT           3   /* Mono - Output right ADC data only (must use AC97_PCM_FORMAT_DSP_A/AC97_PCM_FORMAT_DSP_B) */

#define AC97_PCM_POLARITY_NORMAL        0   /* Normal PCMCLK polarity - Default */
#define AC97_PCM_POLARITY_INVERT        1   /* Inverted PCMCLK polarity */

#define AC97_PCM_OSR_128FS              0   /* 128x oversampling on VXDAC */
#define AC97_PCM_OSR_64FS               1   /* 64x oversampling on VXDAC - Default */

#define AC97_PCM_PCMCLK_16FS            4   /* 16 PCMCLK periods per PCMFS period */
#define AC97_PCM_PCMCLK_32FS            3   /* 32 PCMCLK periods per PCMFS period */
#define AC97_PCM_PCMCLK_64FS            2   /* 64 PCMCLK periods per PCMFS period */
#define AC97_PCM_PCMCLK_128FS           1   /* 128 PCMCLK periods per PCMFS period */

#define AC97_PCM_SAMPLERATE_6000HZ      15
#define AC97_PCM_SAMPLERATE_6400HZ      14
#define AC97_PCM_SAMPLERATE_8000HZ      11
#define AC97_PCM_SAMPLERATE_9600HZ      9
#define AC97_PCM_SAMPLERATE_12000HZ     7
#define AC97_PCM_SAMPLERATE_16000HZ     5   /* Highest recommended sample rate for VXDAC */
#define AC97_PCM_SAMPLERATE_19200HZ     4
#define AC97_PCM_SAMPLERATE_24000HZ     3
#define AC97_PCM_SAMPLERATE_32000HZ     2
#define AC97_PCM_SAMPLERATE_48000HZ     1

/* Set configuration parameters for PCM interface (Registers: 0x36, 0x44, 0x4C) */
CsrInt32 AC97PcmConfigurationSet(CsrUint8 mode, CsrUint8 format, CsrUint8 wordLength, CsrUint8 source, CsrUint8 polarity, CsrUint8 osr, CsrUint8 pcmClk, CsrUint8 sampleRate);


/********************************************************/
/********* Auxiliary ADC Configuration (AUXADC) *********/
/********************************************************/

/* The AUX ADC is connected to the MIC2B input (middle pin
   on mini-jack), and the output is routed to slot 6 of
   the AC97 link frame.

   The data in the slot will contain:
   Data[15]    - Don't care.
   Data[14:12] - Zero: invalid data, Non-zero: valid data.
   Data[11:0]  - Audio data (12bit). */

#define AC97_AUXADC_DISABLE                 0
#define AC97_AUXADC_ENABLE                  1

#define AC97_AUXADC_SAMPLERATE_8000HZ       0   /* Every six AC-Link Frames */
#define AC97_AUXADC_SAMPLERATE_12000HZ      1   /* Every four AC-Link Frames */
#define AC97_AUXADC_SAMPLERATE_24000HZ      2   /* Every other AC-Link Frames */
#define AC97_AUXADC_SAMPLERATE_48000HZ      3   /* Every AC-Link Frame */

/* Set configuration parameters for AUX ADC (Registers: 0x78, 0x76, 0x74) */
CsrInt32 AC97AuxAdcConfigurationSet(CsrUint8 enable, CsrUint8 sampleRate);


/***************************************/
/********* Mixer Configuration *********/
/***************************************/

/* Configuration options for mixer sources. Can be logically OR'ed together.
   Note that each mixer source only supports a subset of these options. */
#define AC97_MIXER_UNMUTE               0
#define AC97_MIXER_MUTE_RIGHT           1
#define AC97_MIXER_MUTE_LEFT            2
#define AC97_MIXER_MUTE                 (AC97_MIXER_MUTE_RIGHT | AC97_MIXER_MUTE_LEFT)
#define AC97_MIXER_BOOST_20DB           4
#define AC97_MIXER_GAIN_UNITY           0
#define AC97_MIXER_GAIN(gain)           (8 | ((gain & 0x7) << 4))        /* 0: +6dB - 3dB Steps - 7: -15dB */

/*** Mono Mixer (MONOMIX) ***
 Sources:
    CSR (LINEL/R)       - Stereo mute/unmute - (Registers: 0x0A[13])
    DAC (AC97 Link)     - Stereo mute/unmute - (Registers: 0x0C[13])
    MICA/MICB           - Stereo mute/left(MICA)/right(MICB)/unmute, boost - (Registers: 0x10[7:5])
    ADC                 - Stereo mute/left/right/unmute, boost (Registers: 0x14[10:8])
    VXDAC               - Mono mute/unmute, gain - (Registers: 0x18[7:4]) */
CsrInt32 AC97MonoMixerConfigurationSet(CsrUint8 csr, CsrUint8 dac, CsrUint8 mic, CsrUint8 adc, CsrUint8 vxdac);

/*** Headphone Mixers (HPMIXL, HPMIXR) ***
 Sources:
    GSM (MONOIN)        - Mono mute/unmute - (Registers: 0x08[15])
    CSR (LINEL/R)       - Stereo mute/unmute - (Registers: 0x0A[15])
    DAC (AC97 Link)     - Stereo mute/unmute - (Registers: 0x0C[15])
    MICA/MICB           - Stereo mute/left(MICA)/right(MICB)/unmute, gain - (Registers: 0x10[4:0])
    ADC                 - Stereo mute/left/right/unmute, gain - (Registers: 0x14[15:11])
    VXDAC               - Mono mute/unmute, gain - (Registers: 0x18[15:12]) */
CsrInt32 AC97HeadphoneMixerConfigurationSet(CsrUint8 gsm, CsrUint8 csr, CsrUint8 dac, CsrUint8 mic, CsrUint8 adc, CsrUint8 vxdac);

/*** Speaker Mixer (SPKMIX) ***
 Sources:
    GSM (MONOIN)        - Mono mute/unmute - (Registers: 0x08[14])
    CSR (LINEL/R)       - Stereo mute/unmute - (Registers: 0x0A[14])
    DAC (AC97 Link)     - Stereo mute/unmute - (Registers: 0x0C[14])
    VXDAC               - Mono mute/unmute, gain - (Registers: 0x18[11:8]) */
CsrInt32 AC97SpeakerMixerConfigurationSet(CsrUint8 gsm, CsrUint8 csr, CsrUint8 dac, CsrUint8 vxdac);


/*********************************************************************/
/********* Mixer Output Inverters Configuration (INV1, INV2) *********/
/*********************************************************************/

#define AC97_INVERTER_1                     1
#define AC97_INVERTER_2                     2
#define AC97_INTERTER_BOTH                  (AC97_INVERTER_1 | AC97_INVERTER_2)

#define AC97_INVERTER_SOURCE_NONE           0   /* No sources connected - Default */
#define AC97_INVERTER_SOURCE_MONOMIX        1   /* Output from Mono Mixer (MONOMIX) */
#define AC97_INVERTER_SOURCE_SPKMIX         2   /* Output from Speaker Mixer (SPKMIX) */
#define AC97_INVERTER_SOURCE_HPMIXL         3   /* Output from Headphone Mixer Left (HPMIXL) */
#define AC97_INVERTER_SOURCE_HPMIXR         4   /* Output from Headphone Mixer Right (HPMIXR) */
#define AC97_INVERTER_SOURCE_HPMIXMONO      5   /* Mono mix of Headphone Mixers ((HPMIXL + HPMIXR) / 2) */

/* Set the source for the Mixer Output Inverters (Registers: 0x1E) */
CsrInt32 AC97InverterSourceSet(CsrInt32 inverter, CsrUint8 source);


/***************************************************/
/********* GSM Output Configuration (MONO) *********/
/***************************************************/

#define AC97_GSM_OUTPUT_SOURCE_VMID         0   /* Recommended before power-down - Default */
#define AC97_GSM_OUTPUT_SOURCE_MONOMIX      2   /* Output from Mono Mixer (MONOMIX) */
#define AC97_GSM_OUTPUT_SOURCE_INV1         3   /* Output from Mixer Output Inverter 1 (INV1) */

#define AC97_GSM_OUTPUT_VOLUME_MUTE         0xC0
#define AC97_GSM_OUTPUT_VOLUME_UNITY        0x40
#define AC97_GSM_OUTPUT_VOLUME(gain)        (0x40 | (gain & 0x1F))      /* 0: 0dB - 1.5dB Steps - 31: -46.5dB */

/* Set the source for the GSM Output (Registers: 0x1C) */
CsrInt32 AC97GsmOutputSourceSet(CsrUint8 source);

/* Set the volume for the GSM Output (Registers: 0x08) */
CsrInt32 AC97GsmOutputVolumeSet(CsrUint8 volume);


/*************************************************************/
/********* Headphone Output Configuration (HPL, HPR) *********/
/*************************************************************/

#define AC97_HEADPHONE_LEFT                 1
#define AC97_HEADPHONE_RIGHT                2
#define AC97_HEADPHONE_BOTH                 (AC97_HEADPHONE_LEFT | AC97_HEADPHONE_RIGHT)

#define AC97_HEADPHONE_SOURCE_VMID          0   /* Recommended before power-down - Default */
#define AC97_HEADPHONE_SOURCE_HPMIX         2   /* Output from Headphone Mixers (HPMIXL, HPMIXR) */

#define AC97_HEADPHONE_VOLUME_MUTE          0xC0
#define AC97_HEADPHONE_VOLUME_UNITY         0x40
#define AC97_HEADPHONE_VOLUME(gain)         (0x40 | (gain & 0x1F))      /* 0: 0dB - 1.5dB Steps - 31: -46.5dB */

/* Set the source for the Headphone Output (Registers: 0x1C) */
CsrInt32 AC97HeadphoneSourceSet(CsrInt32 channel, CsrUint8 source);

/* Set the volume for the Headphone Output (Registers: 0x04) */
CsrInt32 AC97HeadphoneVolumeSet(CsrInt32 channel, CsrUint8 volume);


/*********************************************************/
/********* CSR Output Configuration (SPKL, SPKR) *********/
/*********************************************************/

#define AC97_CSR_OUTPUT_LEFT                 1
#define AC97_CSR_OUTPUT_RIGHT                2
#define AC97_CSR_OUTPUT_BOTH                 (AC97_CSR_OUTPUT_LEFT | AC97_CSR_OUTPUT_RIGHT)

#define AC97_CSR_OUTPUT_SOURCE_VMID          0   /* Recommended before power-down - Default */
#define AC97_CSR_OUTPUT_SOURCE_HPMIX         2   /* Output from Headphone Mixers (HPMIXL, HPMIXR) */
#define AC97_CSR_OUTPUT_SOURCE_SPKMIX        3   /* Output from Speaker Mixer (SPLMIX) */
#define AC97_CSR_OUTPUT_SOURCE_INV           4   /* Output from Mixer Output Inverters (INV1 to SPKL, INV2 to SPKR) */

#define AC97_CSR_OUTPUT_VOLUME_MUTE          0xC0
#define AC97_CSR_OUTPUT_VOLUME_UNITY         0x40
#define AC97_CSR_OUTPUT_VOLUME(gain)         (0x40 | (gain & 0x1F))      /* 0: 0dB - 1.5dB Steps - 31: -46.5dB */

/* Set the source for the CSR Output (Registers: 0x1C) */
CsrInt32 AC97CsrOutputSourceSet(CsrInt32 channel, CsrUint8 source);

/* Set the volume for the CSR Output (Registers: 0x02) */
CsrInt32 AC97CsrOutputVolumeSet(CsrInt32 channel, CsrUint8 volume);


/***************************************************/
/********* Audio Data I/O (DMA controlled) *********/
/***************************************************/

/* This part of the driver is used for controlling the input and output of
   audio data to the AC97 audio interface using DMA. This driver is divided
   in three completely independent and concurrent interfaces, relating to
   the supported audio data slots of the AC97 link:

   1) PcmOutput - Output to slot 3/4 of AC97 link.
   2) PcmInput  - Input from slot 3/4 of AC97 link.
   3) MicInput  - Input from slot 6 of AC97 link.

   PcmOutput is used for sending 16bit signed integer stereo audio data to
   the DAC of the AC97 audio codec. Use the configuration functions above
   to configure the AC97 codec to route the analog audio to the desired
   endpoint, and to set the appropriate parameters for the DAC. The data
   transmitted must be encoded as 32bit unsigned integers as follows:

   Data[31:16] - Left channel (16bit signed integer - 2's complement)
   Data[15:0]  - Right channel (16bit signed integer - 2's complement)

   PcmInput is used for receiving 16bit signed integer stereo audio data from
   the ADC of the AC97 audio codec. Use the configuration functions above
   to configure the AC97 codec for the desired source of analog audio, and
   to set the appropriate parameters for the ADC. The data received is encoded
   as 32bit unsigned integers using the same format as PcmOutput.

   MicInput is used for receiving 12bit signed integer mono audio from
   the AUXDAC. Note that the AUXDAC can only be connected to MIC2B input
   (middle pin of microphone mini-jack). The data received is packed as
   follows:

   Data[15]    - Don't care.
   Data[14:12] - Zero: invalid data, Non-zero: valid data.
   Data[11:0]  - Audio data (12bit signed integer - 2's complement).

   General operational guidelines:

   1) Call AC97*Start to initialise and start the corresponding interface.
   2) Call AC97*Enqueue to provide buffers containing data (for output), or
      free memory for data (for input). The supplied callback function will
      be called when the DMA completes the transfer to/from the supplied
      memory region to/from the hardware FIFO. The depth of the hardware FIFO
      is 16 (stereo) samples. This function can be called several times to
      enqueue additional buffers for transmission/reception.
   3) Optionally call AC97*Pause/AC97*Resume to pause/resume
      transmission/reception.
   4) Call AC97*Stop to stop and deinitialise/disable the corresponding
      interface.

   AC97*Pause/AC97*Resume can be called anytime, but will have no effect if
   the corresponding interface is not started or if the requested stance is
   already the active stance. AC97*Stop can be called regardles of whether
   the interface is paused or not. Note that calling AC97*Stop will cause all
   pending callbacks to be triggered (with a FALSE result code), even if
   callbacks have been masked (using AC97*CallbackMask). AC97*Pause preserves
   the enqueued buffers and transmission/reception is resumed when AC97*Resume
   is called.

   Note that for PcmOutput, if AC97*Enqueue is not called, the hardware FIFO
   will wrap around as it drains, playing back the same 16 samples continuously,
   usually causing monotonical distorted tones. To avoid this, always call
   AC97*Pause when there are no more data to supply to the interface.

   The three interfaces are completely independent and can be started and
   stopped individually concurrently (from different threads). However,
   within the scope of a single interface only one thread may call any of
   the defined functions at a time.

   Callbacks are always called with interrupts disabled, and consequently their
   execution cannot be interrupted by anything. */

/* Callback Function for signalling operation completion */
typedef void (*AC97PcmCallbackFunction)(CsrUint32 length, CsrUint32 *data, CsrUint8 result);
typedef void (*AC97MicCallbackFunction)(CsrUint32 length, CsrUint16 *data, CsrUint8 result);

/********* AC97*Start *********

    Initialise and start the driver. Can be called again to reinitialise.
    Reinitialisation will stop any active transfers, cancel any pending
    operations, and all pending callbacks will be triggered with a FALSE
    result code before returning from this function.

    Return values:
    AC97_RESULT_SUCCESS - Driver successfully initialised.
    AC97_RESULT_FAILURE - Invalid parameter specified. */

CsrInt32 AC97PcmOutputStart(CsrUint8 dmaChannel);
CsrInt32 AC97PcmInputStart(CsrUint8 dmaChannel);
CsrInt32 AC97MicInputStart(CsrUint8 dmaChannel);


/********* AC97Stop *********

    Stop the driver. This will stop any active transfers, cancel any pending
    operations, and all pending callbacks will be triggered with a FALSE result
    code before returning from this function.

    Return values:
    AC97_RESULT_SUCCESS - Driver successfully stopped. */

CsrInt32 AC97PcmOutputStop(void);
CsrInt32 AC97PcmInputStop(void);
CsrInt32 AC97MicInputStop(void);


/********* AC97*Pause *********

    Pause the driver. This will put the driver on hold. Any pending operations
    and associated buffers will remain queued in the driver until the
    corresponding AC97*Resume function is called, or alternatively the
    corresponding AC97*Stop function is called to stop the driver completely.

    This function does not result in any side-effects if called while the
    driver is already paused, or if the interface is not started.

    Return values:
    AC97_RESULT_SUCCESS - Driver successfully paused. */

CsrInt32 AC97PcmOutputPause(void);
CsrInt32 AC97PcmInputPause(void);
CsrInt32 AC97MicInputPause(void);


/********* AC97*Resume *********

    Resume the driver. This will resume the pending operations paused by the
    corresponding AC97*Pause function call.

    This function does not result in any side-effects if the driver has not
    previously been paused, or if the driver has already been resumed.

    Return values:
    AC97_RESULT_SUCCESS - Driver successfully stopped. */

CsrInt32 AC97PcmOutputResume(void);
CsrInt32 AC97PcmInputResume(void);
CsrInt32 AC97MicInputResume(void);


/********* AC97PcmOutputEnqueue *********

    Enqueue a buffer containing audio data for playback. The length specified
    is the number of elements in the specified array, corresponding to the
    number of stereo samples.

    The function will return immediatly, and the specified callback function
    will be called when all data has been transfered to the hardware, and the
    buffer can be deallocated. The result code will be set to TRUE, to indicate
    that the data in the buffer has been successfully transfered.

    If AC97PcmOutputStop is called while buffers are queued in the driver, the
    associated callback functions will be called for each unprocessed buffer
    in arbitrary sequence with the result parameter set to FALSE, to allow
    deallocation of the buffers.

    Return values:
    AC97_RESULT_SUCCESS - Operation succesfully started.
    AC97_RESULT_FAILURE - Invalid parameter specified.
    AC97_RESULT_NO_MORE_SLOTS - No free buffer slots, wait for callback, or
                                retry later. */

CsrInt32 AC97PcmOutputEnqueue(CsrUint32 length, CsrUint32 *data, AC97PcmCallbackFunction callback);


/********* AC97PcmInput, AC97MicInput *********

    Enqueue a buffer containing free memory for receiving data. The length
    specified is the number of elements in the specified array, corresponding
    to the number of (stereo) samples.

    The function will return immediatly, and the specified callback function
    will be called when all data has been transfered from the hardware, and the
    buffer can be processed. The result code will be set to TRUE, to indicate
    that the data in the buffer has been successfully transfered.

    If AC97PcmOutputStop is called while buffers are queued in the driver, the
    associated callback functions will be called for each unfilled buffer
    in arbitrary sequence with the result parameter set to FALSE, to indicate
    that they contain arbitrary data.

    Return values:
    AC97_RESULT_SUCCESS - Operation succesfully started.
    AC97_RESULT_FAILURE - Invalid parameter specified.
    AC97_RESULT_NO_MORE_SLOTS - No free buffer slots, wait for callback, or
                                retry later. */

CsrInt32 AC97PcmInputEnqueue(CsrUint32 length, CsrUint32 *data, AC97PcmCallbackFunction callback);
CsrInt32 AC97MicInputEnqueue(CsrUint32 length, CsrUint16 *data, AC97MicCallbackFunction callback);


/********* AC97*CallbackMask, AC97*CallbackUnmask *********

    Functions for temporarily masking and unmasking the callback
    function. This is used for protecting data structures that are
    accessed from normal context as well as from the callback functions.
    Note that interrupts are always completely disabled in callbacks, and
    only normal context accesses to shared data structures need to be
    protected by masking the callback.

    Note that this will, as a side-effect, stall all further data transfers
    when the currently transfering buffer is completely transferred.
    Transmission/reception will resume when the callbacks are unmasked, however,
    as this might drain the hardware FIFO's, leading to missed samples, it is
    advisable to only mask the callbacks for very short periods of time.

    Note: AC97*Stop will trigger all pending callbacks, regardless of the
    state of the callback mask. */

void AC97PcmOutputCallbackMask(void);
void AC97PcmInputCallbackMask(void);
void AC97MicInputCallbackMask(void);

void AC97PcmOutputCallbackUnmask(void);
void AC97PcmInputCallbackUnmask(void);
void AC97MicInputCallbackUnmask(void);


#ifdef __cplusplus
}
#endif

#endif
