/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "platform/csr_s3c24a0.h"
#include "platform/csr_ac97.h"
#include "platform/csr_delay.h"
#include "platform/csr_interrupt.h"


#define CMD_WRITE(address, data)            {rAC_CODEC_CMD = (address << 16) | data;}
#define CMD_READ(address)                   {rAC_CODEC_CMD = (1 << 23) | (address << 16);}
#define CMD_READ_DATA_GET()                 (rAC_CODEC_STAT)

#define COLD_RESET_ASSERT()                 BIT_SET(rAC_GLBCTRL, 0)
#define COLD_RESET_NEGATE()                 BIT_CLEAR(rAC_GLBCTRL, 0)

#define WARM_RESET_ASSERT()                 BIT_SET(rAC_GLBCTRL, 1)
#define WARM_RESET_NEGATE()                 BIT_CLEAR(rAC_GLBCTRL, 1)

#define AC_LINK_ON()                        BIT_SET(rAC_GLBCTRL, 2)
#define AC_LINK_OFF()                       BIT_CLEAR(rAC_GLBCTRL, 2)

#define AC_LINK_DATA_TRANSFER_ON()          BIT_SET(rAC_GLBCTRL, 3)
#define AC_LINK_DATA_TRANSFER_OFF()         BIT_CLEAR(rAC_GLBCTRL, 3)

#define AC_LINK_IS_ACTIVE()                 (REG_CHECK(rAC_GLBCTRL, 3, 2) == 0x3)

/* Maximum number of retries when polling */
#define MAX_POLL        1024


/*****************************************/
/********* AC97 Power Management *********/
/*****************************************/

CsrInt32 AC97Reset(CsrInt32 type)
{
    CsrInt32 i;

    if (type == AC97_RESET_TYPE_REGISTER)
    {
        if (AC_LINK_IS_ACTIVE())
        {
            AC97CmdWrite(0x00, 0x0000);
        }
        else
        {
            return AC97_RESULT_FAILURE;
        }
    }

    /* Reset the control register */
    rAC_GLBCTRL = 0;

    if (type == AC97_RESET_TYPE_COLD)
    {
        /* Cold Reset */
        COLD_RESET_ASSERT();
        COLD_RESET_NEGATE();
    }

    /* Warm Reset (to wake it up) */
    WARM_RESET_ASSERT();
    DelayUs(100);
    WARM_RESET_NEGATE();

    /* Set the chip in read-mode to prevent reseting it again when the AC-link is turned on */
    CMD_READ(0);

    /* Enable Codec Ready Detection */
    BIT_SET(rAC_GLBCTRL, 22);

    /* AC-link On */
    AC_LINK_ON();
    DelayUs(100);

    /* Data Transfer Enable */
    AC_LINK_DATA_TRANSFER_ON()

    /* Wait for Codec Ready */
    for (i = 0; i < MAX_POLL; i++)
    {
        if (BIT_CHECK(rAC_GLBSTAT, 22))
        {
            break;
        }
    }

    if (i == MAX_POLL)
    {
        rAC_GLBCTRL = 0;
        return AC97_RESULT_FAILURE;
    }

    /* Disable Codec Ready Detection */
    BIT_CLEAR(rAC_GLBCTRL, 22);

    AC97CmdWrite(0x26, 0x0000); /* Power-on AC97 codec components */
    AC97CmdWrite(0x3C, 0x8800); /* Codec specific power bits (keep AUXADC/AUXDAC in power down) */
    AC97CmdWrite(0x3E, 0x0000); /* Codec specific power bits */
    AC97CmdWrite(0x44, 0x0082); /* CLXAX2 Clock Double (AC97 BITCLK = 12.288 MHz) */

    return AC97_RESULT_SUCCESS;
}

CsrInt32 AC97PowerDown(void)
{
    if (AC_LINK_IS_ACTIVE())
    {
        AC97CmdWrite(0x26, 0xFFFF);
        AC97CmdWrite(0x3C, 0xFFFF);
        AC97CmdWrite(0x3E, 0xFFFF);
    }
    rAC_GLBCTRL = 0;

    return AC97_RESULT_SUCCESS;
}

/*********************************/
/********* AC97 Commands *********/
/*********************************/

CsrInt32 AC97CmdWrite(CsrUint8 address, CsrUint16 data)
{
    if (AC_LINK_IS_ACTIVE())
    {
        CMD_WRITE(address, data);
        DelayUs(50); /* Worst case: 41.67us */
        CMD_READ(0);
        return AC97_RESULT_SUCCESS;
    }
    else
    {
        return AC97_RESULT_FAILURE;
    }
}

CsrInt32 AC97CmdRead(CsrUint8 address, CsrUint16 *data)
{
    if (AC_LINK_IS_ACTIVE())
    {
        CsrUint32 tmp;
        CsrInt32 i;

        CMD_READ(address);

        for (i = 0; i < MAX_POLL; i++)
        {
            tmp = CMD_READ_DATA_GET();
            if (((tmp >> 16) & 0xFF) == address)
            {
                *data = tmp & 0xFFFF;
                break;
            }
        }
        if (i < MAX_POLL)
        {
            return AC97_RESULT_SUCCESS;
        }
    }

    return AC97_RESULT_FAILURE;
}

CsrInt32 AC97CmdReadModifyWrite(CsrUint8 address, CsrUint8 msb, CsrUint8 lsb, CsrUint16 data)
{
    CsrUint16 tmp;

    if (AC97CmdRead(address, &tmp) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    tmp = (tmp & ~(((1 << (msb - lsb + 1)) - 1) << lsb)) | ((data & ((1 << (msb - lsb + 1)) - 1)) << lsb);

    return AC97CmdWrite(address, tmp);
}

/****************************************************/
/********* GSM Input Configuration (MONOIN) *********/
/****************************************************/

CsrInt32 AC97GsmInputGainSet(CsrUint8 gain)
{
    CsrUint16 data;

    if (AC97CmdRead(0x08, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    REG_SET(data, 5, 0, gain);

    return AC97CmdWrite(0x08, data);
}

/**************************************************/
/********* CSR Input Configuration (LINE) *********/
/**************************************************/

CsrInt32 AC97CsrInputGainSet(CsrInt32 channel, CsrUint8 gain)
{
    CsrUint16 data;

    if (AC97CmdRead(0x0A, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (channel & AC97_CSR_INPUT_LEFT)
    {
        REG_SET(data, 12, 8, gain);
    }
    if (channel & AC97_CSR_INPUT_RIGHT)
    {
        REG_SET(data, 4, 0, gain);
    }

    return AC97CmdWrite(0x0A, data);
}

/*******************************************/
/********* DAC Input Configuration *********/
/*******************************************/

CsrInt32 AC97DacSampleRateSet(CsrUint16 sampleRate)
{
    CsrUint16 data;

    if (AC97CmdRead(0x2A, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (!(data & 0x1))
    {
        if (AC97CmdWrite(0x2A, data | 0x1) == AC97_RESULT_FAILURE)
        {
            return AC97_RESULT_FAILURE;
        }
    }

    return AC97CmdWrite(0x2C, sampleRate);
}

CsrInt32 AC97DacGainSet(CsrInt32 channel, CsrUint8 gain)
{
    CsrUint16 data;

    if (AC97CmdRead(0x0C, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (channel & AC97_DAC_LEFT)
    {
        REG_SET(data, 12, 8, gain);
    }
    if (channel & AC97_DAC_RIGHT)
    {
        REG_SET(data, 4, 0, gain);
    }

    return AC97CmdWrite(0x0C, data);
}

/***************************************************************/
/********* Microphone Input Configuration (MICA, MICB) *********/
/***************************************************************/

CsrInt32 AC97MicrophoneInputBoostSet(CsrInt32 microphone, CsrUint8 boost)
{
    CsrUint16 data;

    if (AC97CmdRead(0x22, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (microphone & AC97_MICROPHONE_A)
    {
        REG_SET(data, 11, 10, boost);
    }
    if (microphone & AC97_MICROPHONE_B)
    {
        REG_SET(data, 9, 8, boost);
    }

    return AC97CmdWrite(0x22, data);
}

CsrInt32 AC97MicrophoneGainSet(CsrInt32 microphone, CsrUint8 gain)
{
    CsrUint16 data;

    if (AC97CmdRead(0x0E, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (microphone & AC97_MICROPHONE_A)
    {
        REG_SET(data, 12, 8, gain);
    }
    if (microphone & AC97_MICROPHONE_B)
    {
        REG_SET(data, 4, 0, gain);
    }

    return AC97CmdWrite(0x0E, data);
}

/*******************************************/
/********* ADC Configuration (ADC) *********/
/*******************************************/

CsrInt32 AC97AdcSourceSet(CsrInt32 channel, CsrUint8 source)
{
    CsrUint16 data;

    if (source > 6)
    {
        return AC97_RESULT_FAILURE;
    }

    if (AC97CmdRead(0x14, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (channel & AC97_ADC_LEFT)
    {
        REG_SET(data, 5, 3, source);
    }
    if (channel & AC97_ADC_RIGHT)
    {
        REG_SET(data, 2, 0, source);
    }

    return AC97CmdWrite(0x14, data);
}

CsrInt32 AC97AdcGainSet(CsrInt32 channel, CsrUint8 gain)
{
    CsrUint16 data;

    if (gain & AC97_ADC_GAIN_MUTE)
    {
        return AC97CmdWrite(0x12, 0x8000);
    }

    if (gain & AC97_ADC_GAIN_AUTOMATIC(0))
    {
        data = ((gain & 0x7) << 11) | (3 << 9);

        if (channel & AC97_ADC_LEFT)
        {
            BIT_SET(data, 15);
        }
        if (channel & AC97_ADC_RIGHT)
        {
            BIT_SET(data, 14);
        }

        if (AC97CmdWrite(0x62, data) == AC97_RESULT_FAILURE)
        {
            return AC97_RESULT_FAILURE;
        }

        if (AC97CmdRead(0x12, &data) == AC97_RESULT_FAILURE)
        {
            return AC97_RESULT_FAILURE;
        }

        BIT_CLEAR(data, 15);

        return AC97CmdWrite(0x12, data);
    }

    /* Enable zero-crossing detection */
    data = (1 << 7);

    if (channel & AC97_ADC_LEFT)
    {
        REG_SET(data, 14, 8, (1 << 6) | gain);
    }
    if (channel & AC97_ADC_RIGHT)
    {
        REG_SET(data, 6, 0, (1 << 6) | gain);
    }

    return AC97CmdWrite(0x12, data);
}

CsrInt32 AC97AdcBoostSet(CsrUint8 enable)
{
    CsrUint16 data;

    if (AC97CmdRead(0x14, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (enable)
    {
        BIT_SET(data, 6);
    }
    else
    {
        BIT_CLEAR(data, 6);
    }

    return AC97CmdWrite(0x14, data);
}

CsrInt32 AC97AdcSampleRateSet(CsrUint16 sampleRate)
{
    CsrUint16 data;

    if (AC97CmdRead(0x2A, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (!(data & 0x1))
    {
        if (AC97CmdWrite(0x2A, data | 0x1) == AC97_RESULT_FAILURE)
        {
            return AC97_RESULT_FAILURE;
        }
    }

    return AC97CmdWrite(0x32, sampleRate);
}

/*******************************************************/
/********* PCM Interface Configuration (VXDAC) *********/
/*******************************************************/

CsrInt32 AC97PcmConfigurationSet(CsrUint8 mode, CsrUint8 format, CsrUint8 wordLength, CsrUint8 source, CsrUint8 polarity, CsrUint8 osr, CsrUint8 pcmClk, CsrUint8 sampleRate)
{
    CsrUint16 data;

    /* Configure Voice DAC clock to 256Fs */
    if (AC97CmdReadModifyWrite(0x44, 11, 8, sampleRate) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    /* Get GPIO Configuration Register */
    if (AC97CmdRead(0x4C, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    /* Set directions of PCM port pins */
    switch (mode & 0x3)
    {
        case AC97_PCM_MODE_DISABLE:
            BIT_SET(data, 1);   /* PCMCLK */
            BIT_SET(data, 3);   /* PCMFS */
            BIT_SET(data, 4);   /* PCMDAC */
            BIT_SET(data, 5);   /* PCMADC */
            break;
        case AC97_PCM_MODE_SLAVE:
            BIT_SET(data, 1);
            BIT_SET(data, 3);
            BIT_SET(data, 4);
            BIT_CLEAR(data, 5);
            break;
        case AC97_PCM_MODE_MASTER:
            BIT_CLEAR(data, 1);
            BIT_CLEAR(data, 3);
            BIT_SET(data, 4);
            BIT_CLEAR(data, 5);
            break;
        case AC97_PCM_MODE_PARTIALMASTER:
            BIT_CLEAR(data, 1);
            BIT_SET(data, 3);
            BIT_SET(data, 4);
            BIT_CLEAR(data, 5);
            break;
    }

    /* Write back GPIO Configuration Register */
    if (AC97CmdWrite(0x4C, data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    /* Configure VXDAC */
    if (mode != AC97_PCM_MODE_DISABLE)
    {
        data = 0x8000 |
               ((mode & 0x3) << 13) |
               ((pcmClk & 0x7) << 9) | /* Assumes Voice DAC clock is 256Fs */
               ((osr & 0x1) << 8) |
               ((polarity & 0x1) << 7) |
               (((format >> 2) & 0x1) << 6) |
               ((source & 0x3) << 4) |
               ((wordLength & 0x3) << 2) |
               (format & 0x3);
    }
    else
    {
        data = 0x8000;
    }

    return AC97CmdWrite(0x36, data);
}

/********************************************************/
/********* Auxiliary ADC Configuration (AUXADC) *********/
/********************************************************/

CsrInt32 AC97AuxAdcConfigurationSet(CsrUint8 enable, CsrUint8 sampleRate)
{
    if (enable == AC97_AUXADC_ENABLE)
    {
        CsrInt32 ret;

        ret = AC97CmdWrite(0x74, (1 << 8) | (1 << 5));
        ret |= AC97CmdWrite(0x76, ((sampleRate & 0x3) << 8) | (15 << 4) | (1 << 3) | 1); /* DEL = 1111, SLEN = 1, SLT = 001 (Slot 6) */
        ret |= AC97CmdWrite(0x78, (3 << 14));

        return ret;
    }
    else
    {
        return AC97CmdWrite(0x78, 0x0000);
    }
}

/***************************************/
/********* Mixer configuration *********/
/***************************************/

CsrInt32 AC97MonoMixerConfigurationSet(CsrUint8 csr, CsrUint8 dac, CsrUint8 mic, CsrUint8 adc, CsrUint8 vxdac)
{
    CsrInt32 ret;

    ret = AC97CmdReadModifyWrite(0x0A, 13, 13, (csr & AC97_MIXER_MUTE) ? 1 : 0);
    ret |= AC97CmdReadModifyWrite(0x0C, 13, 13, (dac & AC97_MIXER_MUTE) ? 1 : 0);
    ret |= AC97CmdReadModifyWrite(0x10, 7, 5, ((mic & 0x3) << 1) | ((mic & AC97_MIXER_BOOST_20DB) ? 1 : 0));
    ret |= AC97CmdReadModifyWrite(0x14, 10, 8, ((adc & 0x3) << 1) | ((adc & AC97_MIXER_BOOST_20DB) ? 1 : 0));
    ret |= AC97CmdReadModifyWrite(0x18, 7, 4, (((vxdac & AC97_MIXER_MUTE) ? 1 : 0) << 3) | ((vxdac & AC97_MIXER_GAIN(0)) ? (vxdac >> 4) & 0x7 : 2));

    return ret;
}

CsrInt32 AC97HeadphoneMixerConfigurationSet(CsrUint8 gsm, CsrUint8 csr, CsrUint8 dac, CsrUint8 mic, CsrUint8 adc, CsrUint8 vxdac)
{
    CsrInt32 ret;

    ret = AC97CmdReadModifyWrite(0x08, 15, 15, (gsm & AC97_MIXER_MUTE) ? 1 : 0);
    ret |= AC97CmdReadModifyWrite(0x0A, 15, 15, (csr & AC97_MIXER_MUTE) ? 1 : 0);
    ret |= AC97CmdReadModifyWrite(0x0C, 15, 15, (dac & AC97_MIXER_MUTE) ? 1 : 0);
    ret |= AC97CmdReadModifyWrite(0x10, 4, 0, ((mic & 0x3) << 3) | ((mic & AC97_MIXER_GAIN(0)) ? (mic >> 4) & 0x7 : 2));
    ret |= AC97CmdReadModifyWrite(0x14, 15, 11, ((adc & 0x3) << 3) | ((adc & AC97_MIXER_GAIN(0)) ? (adc >> 4) & 0x7 : 2));
    ret |= AC97CmdReadModifyWrite(0x18, 15, 12, (((vxdac & AC97_MIXER_MUTE) ? 1 : 0) << 3) | ((vxdac & AC97_MIXER_GAIN(0)) ? (vxdac >> 4) & 0x7 : 2));

    return ret;
}

CsrInt32 AC97SpeakerMixerConfigurationSet(CsrUint8 gsm, CsrUint8 csr, CsrUint8 dac, CsrUint8 vxdac)
{
    CsrInt32 ret;

    ret = AC97CmdReadModifyWrite(0x08, 14, 14, (gsm & AC97_MIXER_MUTE) ? 1 : 0);
    ret |= AC97CmdReadModifyWrite(0x0A, 14, 14, (csr & AC97_MIXER_MUTE) ? 1 : 0);
    ret |= AC97CmdReadModifyWrite(0x0C, 14, 14, (dac & AC97_MIXER_MUTE) ? 1 : 0);
    ret |= AC97CmdReadModifyWrite(0x18, 11, 8, (((vxdac & AC97_MIXER_MUTE) ? 1 : 0) << 3) | ((vxdac & AC97_MIXER_GAIN(0)) ? (vxdac >> 4) & 0x7 : 2));

    return ret;
}

/*********************************************************************/
/********* Mixer Output Inverters Configuration (INV1, INV2) *********/
/*********************************************************************/

CsrInt32 AC97InverterSourceSet(CsrInt32 inverter, CsrUint8 source)
{
    CsrUint16 data;

    if (AC97CmdRead(0x1E, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (source & AC97_INVERTER_1)
    {
        REG_SET(data, 15, 13, source & 0x7);
    }
    if (source & AC97_INVERTER_2)
    {
        REG_SET(data, 12, 10, source & 0x7);
    }

    return AC97CmdWrite(0x1E, data);
}

/***************************************************/
/********* GSM Output Configuration (MONO) *********/
/***************************************************/

CsrInt32 AC97GsmOutputSourceSet(CsrUint8 source)
{
    CsrUint16 data;

    if (AC97CmdRead(0x1C, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    REG_SET(data, 15, 14, source & 0x3);

    return AC97CmdWrite(0x1C, data);
}

CsrInt32 AC97GsmOutputVolumeSet(CsrUint8 volume)
{
    CsrUint16 data;

    if (AC97CmdRead(0x04, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    REG_SET(data, 7, 0, volume);

    return AC97CmdWrite(0x04, data);
}

/*************************************************************/
/********* Headphone Output Configuration (HPL, HPR) *********/
/*************************************************************/

CsrInt32 AC97HeadphoneSourceSet(CsrInt32 channel, CsrUint8 source)
{
    CsrUint16 data;

    if (AC97CmdRead(0x1C, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (channel & AC97_HEADPHONE_LEFT)
    {
        REG_SET(data, 7, 6, source & 0x3);
    }
    if (channel & AC97_HEADPHONE_RIGHT)
    {
        REG_SET(data, 5, 4, source & 0x3);
    }

    return AC97CmdWrite(0x1C, data);
}

CsrInt32 AC97HeadphoneVolumeSet(CsrInt32 channel, CsrUint8 volume)
{
    CsrUint16 data;

    if (AC97CmdRead(0x04, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (channel & AC97_HEADPHONE_LEFT)
    {
        REG_SET(data, 15, 8, volume);
    }
    if (channel & AC97_HEADPHONE_RIGHT)
    {
        REG_SET(data, 7, 0, volume);
    }

    return AC97CmdWrite(0x04, data);
}

/*********************************************************/
/********* CSR Output Configuration (SPKL, SPKR) *********/
/*********************************************************/

CsrInt32 AC97CsrOutputSourceSet(CsrInt32 channel, CsrUint8 source)
{
    CsrUint16 data;

    if (AC97CmdRead(0x1C, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (channel & AC97_CSR_OUTPUT_LEFT)
    {
        REG_SET(data, 13, 11, source & 0x7);
    }
    if (channel & AC97_CSR_OUTPUT_RIGHT)
    {
        REG_SET(data, 10, 8, source & 0x7);
    }

    return AC97CmdWrite(0x1C, data);
}

CsrInt32 AC97CsrOutputVolumeSet(CsrInt32 channel, CsrUint8 volume)
{
    CsrUint16 data;

    if (AC97CmdRead(0x02, &data) == AC97_RESULT_FAILURE)
    {
        return AC97_RESULT_FAILURE;
    }

    if (channel & AC97_CSR_OUTPUT_LEFT)
    {
        REG_SET(data, 15, 8, volume);
    }
    if (channel & AC97_CSR_OUTPUT_RIGHT)
    {
        REG_SET(data, 7, 0, volume);
    }

    return AC97CmdWrite(0x02, data);
}
