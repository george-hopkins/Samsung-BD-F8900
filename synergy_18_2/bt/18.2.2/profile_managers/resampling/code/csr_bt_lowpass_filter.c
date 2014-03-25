/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
/* */
#include "csr_bt_lowpass_filter.h"


 /*
 Filter coeff matrix. Total number of pre.defined filters is 42. Must be possible to calculate the filter one run-time in future versions

*8000*, 11025, 16000, 22050, 32000, 44100, 48000                        [8000][11025,16000,22050,32000,44100,48000][Coeff]
 8000 ,*11025*,16000, 22050, 32000, 44100, 48000                        [11025][8000,16000,22050,32000,44100,48000][Coeff]
 8000,  11025,*16000*,22050, 32000, 44100, 48000                        [16000][8000,11025,22050,32000,44100,48000][Coeff]
 8000,  11025, 16000,*22050*,32000, 44100, 48000 -> matrix[][][Coeff] ->[22050][8000,11025,16000,32000,44100,48000][Coeff]
 8000,  11025, 16000, 22050,*32000*,44100, 48000                        [32000][8000,11025,16000,22050,44100,48000][Coeff]
 8000,  11025, 16000, 22050, 32000,*44100*,48000                        [44100][8000,11025,16000,22050,32000,48000][Coeff]
 8000,  11025, 16000, 22050, 32000, 44100,*48000*                       [48000][8000,11025,16000,22050,32000,44100][Coeff]
*/

static CsrUint8 firFilterJumpMatrix[7][7] = { { 0, 2, 5, 7,10,12,13},
                                      { 2, 0, 3, 5, 8,10,11},
                                      { 5, 3, 0, 2, 5, 7, 9},
                                      { 7, 5, 2, 0, 3, 5, 6},
                                      {10, 8, 5, 3, 0, 2, 4},
                                      {12,10, 7, 5, 2, 0, 1},
                                      {13,11, 9, 6, 4, 1, 0} };


/*CsrInt16 filterCoeffFirLow[13][15]   = { {   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        { -99, -177, -203, 271, 1669, 3836, 5890,10016,5890,3836,1669,271,-203,-177,-99},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                                        {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0} };
*/

static CsrInt16 filterCoeffFirLow[13][15]   =   {   {   -78,   -53,   133,   770,  1979,  3519,  4837,  8634,  4837,  3519,  1979,   770,   133,   -53,   -78},
                                            {   -99,  -177,  -203,   271,  1669,  3836,  5890, 10016,  5890,  3836,  1669,   271,  -203,  -177,   -99},
                                            {   -74,  -192,  -331,     0,  1413,  3871,  6332, 10650,  6332,  3871,  1413,     0,  -331,  -192,   -74},
                                            {    91,     2,  -403,  -885,   -12,  3344,  7764, 13126,  7764,  3344,   -12,  -885,  -403,     2,    91},
                                            {   100,    37,  -362,  -939,  -196,  3219,  7897, 13414,  7897,  3219,  -196,  -939,  -362,    37,   100},
                                            {   103,   103,  -255, -1006,  -557,  2931,  8141, 13990,  8141,  2931,  -557, -1006,  -255,   103,   103},
                                            {   -65,   134,   374,  -501, -1851,   987,  8880, 16813,  8880,   987, -1851,  -501,   374,   134,   -65},
                                            {  -104,     0,   468,     0, -1999,     0,  8954, 18022,  8954,     0, -1999,     0,   468,     0,  -104},
                                            {    89,     5,  -410,   872,    25, -3368,  7736, 22976,  7736, -3368,    25,   872,  -410,     5,    89},
                                            {    54,    80,  -462,   701,   426, -3594,  7414, 23610,  7414, -3594,   426,   701,  -462,    80,    54},
                                            {   -28,   174,  -428,   295,  1069, -3829,  6780, 24704,  6780, -3829,  1069,   295,  -428,    174,  -28},
{   102,  -192,   445,  -859,  1369, -1867,  2229, 30406,  2229, -1867,  1369,  -859,   445,   -192,  102} };



/*CsrInt16 filterCoeffFirMedium[FREQ_INPUT][FREQ_OUTPUT][]   = {}; */
/*CsrInt16 filterCoeffFirHigh[FREQ_INPUT][FREQ_OUTPUT][]     = {}; */

/*CsrInt16 filterCoeffIirLow[FREQ_INPUT][FREQ_OUTPUT][]      = {}; */
/*CsrInt16 filterCoeffIirMedium[FREQ_INPUT][FREQ_OUTPUT][]   = {}; */
/*CsrInt16 filterCoeffIirHigh[FREQ_INPUT][FREQ_OUTPUT][]     = {}; */


/*
 *  filterInit
 **/
filter_t* filterInit(CsrInt32 fsIn, CsrInt32 fsOut, CsrUint16 lengthData)
{
    filter_t *inst  = NULL;
    CsrUint8 j       = 0;
    CsrUint8 index1  = 0;
    CsrUint8 index2  = 0;
    CsrUint16 freq[] = {8000, 11025, 16000, 22050, 32000, 44100, 48000};

    /* Find filter coeff */
    inst = CsrPmemAlloc(sizeof(filter_t));

    for( j=0; j<7; j++ )
    {
        if( freq[j] == fsIn )
        {
            index1 = j;
        }
    }
    for( j=0; j<7; j++ )
    {
        if( freq[j] == fsOut )
        {
            index2 = j;
        }
    }


    inst->lengthData    = lengthData;
    inst->c             = &filterCoeffFirLow[firFilterJumpMatrix[index1][index2]][0];
    inst->lengthF       = 15;

    for( j=0; j<15; j++ )
    {
        inst->d[j] = 0;

    }
    return inst;
}

/*
 * filterDeInit
 */
void filterDeInit(filter_t *instanceData)
{
    CsrPmemFree(instanceData);
}

/*
 * filter
 */
void filterSimpleFir(filter_t *inst, CsrInt16 *data)
{
    CsrUint16 i,j;
    CsrInt32 reg;
    /*NOTE THAT THE FILTER COEFF ARE SYTMETRIC THUS CON*/


    for(i=0; i<inst->lengthData; i++)
    {
        for(j=inst->lengthF-2; j>0; j--)
        {
            inst->d[j+1] =  inst->d[j];
        }
        inst->d[1] =  inst->d[0];
        inst->d[0] = data[i];

        reg =  0;
        for(j=0; j<inst->lengthF; j++)
        {
            reg =  reg + (CsrInt32)inst->d[j]*inst->c[j];
        }

        data[i] = (CsrInt16)(reg >> 16);
    }
}
