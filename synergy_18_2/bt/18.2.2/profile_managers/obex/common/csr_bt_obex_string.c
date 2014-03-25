/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_bt_obex.h"
#include "csr_bt_util.h"
#include "csr_pmem.h"
#include "csr_bt_obex_common.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_private_lib.h"

#define CAPITAL(x)    ((x >= 'a') && (x <= 'z') ? ((x) & 0xDF) : (x))

#ifdef CSR_BT_INSTALL_OBEX_STRING_DESCRIPTION
CsrUint8 * CsrBtBuildImgDescriptorHeader(char * encoding, char * pixel, char * size,
                                      char * maxSize, char * transformation, CsrUint16 descriptorLength)
{
    CsrUint16 length;
    CsrUint8    * obexPacket;
    CsrUint8    * buffer;

    buffer         = (CsrUint8 *) CsrPmemAlloc(descriptorLength);
    obexPacket    = buffer;
    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_DESCRIPTOR_HEADER);
    CsrMemCpy(buffer, CSR_BT_IMAGE_DESCRIPTOR_HEADER, length);
    buffer += length;

    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_HEADER);
    CsrMemCpy(buffer, CSR_BT_IMAGE_HEADER, length);
    buffer += length;

    if (encoding && CsrStrLen(encoding))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_ENCODING_HEADER);
        CsrMemCpy(buffer, CSR_BT_ENCODING_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(encoding);
        CsrMemCpy(buffer, encoding, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (pixel &&  CsrStrLen(pixel))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_PIXEL_HEADER);
        CsrMemCpy(buffer, CSR_BT_PIXEL_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(pixel);
        CsrMemCpy(buffer, pixel, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }

    if (size && CsrStrLen(size))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_SIZE_HEADER);
        CsrMemCpy(buffer, CSR_BT_SIZE_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(size);
        CsrMemCpy(buffer, size, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (maxSize && CsrStrLen(maxSize))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_MAX_SIZE_HEADER);
        CsrMemCpy(buffer, CSR_BT_MAX_SIZE_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(maxSize);
        CsrMemCpy(buffer, maxSize, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (transformation && CsrStrLen(transformation))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_TRANSFORMATION_HEADER);
        CsrMemCpy(buffer, CSR_BT_TRANSFORMATION_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(transformation);
        CsrMemCpy(buffer, transformation, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_TERMINATOR);
    CsrMemCpy(buffer, CSR_BT_IMAGE_TERMINATOR, length);
    buffer += length;
    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_DESCRIPTOR_TERMINATOR);
    CsrMemCpy(buffer, CSR_BT_IMAGE_DESCRIPTOR_TERMINATOR, length);
    return obexPacket;
}

CsrUint8 * CsrBtBuildImgAttachmentDescriptorHeader(char * contentType, char * charset, char * name,
                                                char * size, char * created, CsrUint16 descriptorLength)
{
    CsrUint16 length;
    CsrUint8    * obexPacket;
    CsrUint8    * buffer;

    buffer         = (CsrUint8 *) CsrPmemAlloc(descriptorLength);
    obexPacket    = buffer;

    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_ATTACHMENT_DESCRIPTOR_HEADER);
    CsrMemCpy(buffer, CSR_BT_IMAGE_ATTACHMENT_DESCRIPTOR_HEADER, length);
    buffer += length;

    length        = (CsrUint16) CsrStrLen(CSR_BT_ATTACHMENT_HEADER);
    CsrMemCpy(buffer, CSR_BT_ATTACHMENT_HEADER, length);
    buffer += length;

    if (contentType && CsrStrLen(contentType))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_CONTENT_TYPE_HEADER);
        CsrMemCpy(buffer, CSR_BT_CONTENT_TYPE_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(contentType);
        CsrMemCpy(buffer, contentType, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (charset && CsrStrLen(charset))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_CHARSET_HEADER);
        CsrMemCpy(buffer, CSR_BT_CHARSET_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(charset);
        CsrMemCpy(buffer, charset, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (name && CsrStrLen(name))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_NAME_HEADER);
        CsrMemCpy(buffer, CSR_BT_NAME_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(name);
        CsrMemCpy(buffer, name, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (size && CsrStrLen(size))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_SIZE_HEADER);
        CsrMemCpy(buffer, CSR_BT_SIZE_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(size);
        CsrMemCpy(buffer, size, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (created && CsrStrLen(created))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_CREATED_HEADER);
        CsrMemCpy(buffer, CSR_BT_CREATED_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(created);
        CsrMemCpy(buffer, created, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    length        = (CsrUint16) CsrStrLen(CSR_BT_ATTACHMENT_TERMINATOR);
    CsrMemCpy(buffer, CSR_BT_ATTACHMENT_TERMINATOR, length);
    buffer += length;
    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_ATTACHMENT_DESCRIPTOR_TERMINATOR);
    CsrMemCpy(buffer, CSR_BT_IMAGE_ATTACHMENT_DESCRIPTOR_TERMINATOR, length);
    return obexPacket;
}

CsrUint16 CsrBtReturnImgDescriptionLength(char * encoding, char * pixel, char * size, char * maxSize, char * transformation)
{
    CsrUint16 imgDescriptorLength;

    imgDescriptorLength   = (CsrUint16)CsrStrLen(CSR_BT_IMAGE_DESCRIPTOR_HEADER);
    imgDescriptorLength   = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_IMAGE_DESCRIPTOR_TERMINATOR));
    imgDescriptorLength   = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_IMAGE_HEADER));
    imgDescriptorLength   = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_IMAGE_TERMINATOR));

    if (encoding && CsrStrLen(encoding))
    {
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_ENCODING_HEADER));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(encoding));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }
    if (pixel && CsrStrLen(pixel))
    {
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_PIXEL_HEADER));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(pixel));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }
    if (size && CsrStrLen(size))
    {
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_SIZE_HEADER));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(size));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }

    if (maxSize && CsrStrLen(maxSize))
    {
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_MAX_SIZE_HEADER));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(maxSize));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }

    if (transformation && CsrStrLen(transformation))
    {
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_TRANSFORMATION_HEADER));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(transformation));
        imgDescriptorLength  = (CsrUint16) (imgDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }
    return imgDescriptorLength;
}

CsrUint16 CsrBtReturnImgAttachDescriptionLength(char * contentType, char * charset, char * name, char * size, char * created)
{
    CsrUint16 imgAttachDescriptorLength;

    imgAttachDescriptorLength   = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_ATTACHMENT_DESCRIPTOR_HEADER);
    imgAttachDescriptorLength   = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_IMAGE_ATTACHMENT_DESCRIPTOR_TERMINATOR));
    imgAttachDescriptorLength   = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_ATTACHMENT_HEADER));
    imgAttachDescriptorLength   = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_ATTACHMENT_TERMINATOR));

    if (contentType && CsrStrLen(contentType))
    {
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_CONTENT_TYPE_HEADER));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(contentType));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }
    if (charset && CsrStrLen(charset))
    {
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_CHARSET_HEADER));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(charset));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }
    if (name && CsrStrLen(name))
    {
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_NAME_HEADER));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(name));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }
    if (size && CsrStrLen(size))
    {
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_SIZE_HEADER));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(size));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }
    if (created && CsrStrLen(created))
    {
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_CREATED_HEADER));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(created));
        imgAttachDescriptorLength  = (CsrUint16) (imgAttachDescriptorLength + CsrStrLen(CSR_BT_COMMON_TERMINATOR));
    }
    return imgAttachDescriptorLength;
}
#endif

int CsrBtStrICmp(char * string1, char * string2)
{
    while (*string1 || *string2)
    {
        if (CAPITAL(*string1) != CAPITAL(*string2))
        {
            return (*string1 - *string2);
        }
        string1++;
        string2++;
    }

    return 0;
}

int CsrBtStrICmpLength(char * string1, char * string2, CsrUint16 theLength)
{
    CsrUintFast16    n;

    /*    while (*string1 || *string2) */
    for ( n = 0; n < theLength; n++)
    {
        if (CAPITAL(*string1) != CAPITAL(*string2))
        {
            return (*string1 - *string2);
        }
        string1++;
        string2++;
    }
    return 0;
}

#ifdef CSR_BT_INSTALL_OBEX_STRING_DESCRIPTION
CsrUint8 *CsrBtBuildImgListHeader(CsrUint16 nbImageHandles, CsrUint16 imageListLength, CsrUint16 *length)
{
    CsrUint8    * buffer;

    buffer = (CsrUint8 *) CsrPmemAlloc(imageListLength);

    *length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_LISTING_HEADER);
    CsrMemCpy(buffer, CSR_BT_IMAGE_LISTING_HEADER, *length);

    return buffer;
}

CsrUint8 * CsrBtBuildImgListObject(CsrUint8 *imageListString,
                                char imageHandleString[9],
                                char createdFileTimeString[18], CsrUint16 *totalLength)
{
    CsrUint16 length;
    CsrUint8    * buffer = imageListString;

    /* Image header */
    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_HEADER);
    CsrMemCpy(buffer, CSR_BT_IMAGE_HEADER, length);
    buffer += length;
    *totalLength += length;

    /* Handle */
    length        = (CsrUint16) CsrStrLen(CSR_BT_HANDLE_HEADER);
    CsrMemCpy(buffer, CSR_BT_HANDLE_HEADER, length);
    buffer += length;
    *totalLength += length;

    length = 7 + 2;
    CsrMemCpy(buffer, imageHandleString, length);
    buffer += length;
    *totalLength += length;

    length        = (CsrUint16) CsrStrLen(CSR_BT_SPACE);
    CsrMemCpy(buffer, CSR_BT_SPACE, length);
    buffer += length;
    *totalLength += length;

    /* Created */
    length        = (CsrUint16) CsrStrLen(CSR_BT_SIMPLE_CREATED_HEADER);
    CsrMemCpy(buffer, CSR_BT_SIMPLE_CREATED_HEADER, length);
    buffer += length;
    *totalLength += length;

    length = 16 + 2;
    CsrMemCpy(buffer, createdFileTimeString, length);
    buffer += length;
    *totalLength += length;

    /* Image terminator */
    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_TERMINATOR);
    CsrMemCpy(buffer, CSR_BT_IMAGE_TERMINATOR, length);
    buffer += length;
    *totalLength += length;
    return imageListString;
}

CsrUint16 CsrBtReturnImgHandlesDescriptionLength(char * created, char * modified, char * encoding, char * pixel)
{
    CsrUint16 imgHandlesDescriptorLength;

    imgHandlesDescriptorLength  = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_HANDLES_DESCRIPTOR_HEADER);
    imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_IMAGE_HANDLES_DESCRIPTOR_TERMINATOR);

    if ((created && CsrStrLen(created)) || (modified && CsrStrLen(modified)) ||
        (encoding && CsrStrLen(encoding)) || (pixel && CsrStrLen(pixel)))
    {
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_FILTERING_PARAMETERS_HEADER);
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_FILTERING_PARAMETERS_TERMINATOR);
    }

    if (created && CsrStrLen(created))
    {
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_CREATED_HEADER);
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(created);
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
    }
    if (modified && CsrStrLen(modified))
    {
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_MODIFIED_HEADER);
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(modified);
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
    }
    if (encoding && CsrStrLen(encoding))
    {
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_ENCODING_HEADER);
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(encoding);
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
    }
    if (pixel && CsrStrLen(pixel))
    {
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_PIXEL_HEADER);
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(pixel);
        imgHandlesDescriptorLength += (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
    }

    return imgHandlesDescriptorLength;
}

CsrUint16 CsrBtReturnImgListLength(CsrUint16 nofImagesInList)
{
    CsrUint16 imgListingLength;

    imgListingLength  = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_HEADER);

    imgListingLength += (CsrUint16) CsrStrLen(CSR_BT_HANDLE_HEADER);
    imgListingLength += (CsrUint16) (7 + 2);

    imgListingLength += (CsrUint16) CsrStrLen(CSR_BT_SPACE);

    imgListingLength += (CsrUint16) CsrStrLen(CSR_BT_SIMPLE_CREATED_HEADER);
    imgListingLength += (CsrUint16) (16 + 2);

    imgListingLength += (CsrUint16) CsrStrLen(CSR_BT_IMAGE_TERMINATOR);


    imgListingLength *= nofImagesInList;

    imgListingLength += (CsrUint16) CsrStrLen(CSR_BT_IMAGE_LISTING_HEADER);
    imgListingLength += (CsrUint16) CsrStrLen(CSR_BT_IMAGE_LISTING_TERMINATOR);


    return imgListingLength;
}

CsrUint8 *  CsrBtBuildImgListTerminator(CsrUint8 *imageListString)

{
    CsrUint16 length;
    CsrUint8    * buffer = imageListString;

    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_LISTING_TERMINATOR);
    CsrMemCpy(buffer, CSR_BT_IMAGE_LISTING_TERMINATOR, length);

    return imageListString;
}

CsrUint8 * CsrBtBuildImgHandlesDescriptorHeader(char * created, char * modified, char * encoding, char * pixel, CsrUint16 descriptorLength)
{
    CsrUint16 length;
    CsrUint8    * obexPacket;
    CsrUint8    * buffer;

    buffer         = (CsrUint8 *) CsrPmemAlloc(descriptorLength);
    obexPacket    = buffer;

    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_HANDLES_DESCRIPTOR_HEADER);
    CsrMemCpy(buffer, CSR_BT_IMAGE_HANDLES_DESCRIPTOR_HEADER, length);
    buffer += length;


    if ((created && CsrStrLen(created)) || (modified && CsrStrLen(modified)) ||
        (encoding && CsrStrLen(encoding)) || (pixel && CsrStrLen(pixel)))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_FILTERING_PARAMETERS_HEADER);
        CsrMemCpy(buffer, CSR_BT_FILTERING_PARAMETERS_HEADER, length);
        buffer += length;
    }

    if (created && CsrStrLen(created))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_CREATED_HEADER);
        CsrMemCpy(buffer, CSR_BT_CREATED_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(created);
        CsrMemCpy(buffer, created, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (modified && CsrStrLen(modified))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_MODIFIED_HEADER);
        CsrMemCpy(buffer, CSR_BT_MODIFIED_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(modified);
        CsrMemCpy(buffer, modified, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (encoding && CsrStrLen(encoding))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_ENCODING_HEADER);
        CsrMemCpy(buffer, CSR_BT_ENCODING_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(encoding);
        CsrMemCpy(buffer, encoding, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }
    if (pixel && CsrStrLen(pixel))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_PIXEL_HEADER);
        CsrMemCpy(buffer, CSR_BT_PIXEL_HEADER, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(pixel);
        CsrMemCpy(buffer, pixel, length);
        buffer += length;
        length        = (CsrUint16) CsrStrLen(CSR_BT_COMMON_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_COMMON_TERMINATOR, length);
        buffer += length;
    }

    if ((created && CsrStrLen(created)) || (modified && CsrStrLen(modified)) ||
        (encoding && CsrStrLen(encoding)) || (pixel && CsrStrLen(pixel)))
    {
        length        = (CsrUint16) CsrStrLen(CSR_BT_FILTERING_PARAMETERS_TERMINATOR);
        CsrMemCpy(buffer, CSR_BT_FILTERING_PARAMETERS_TERMINATOR, length);
        buffer += length;
    }
    length        = (CsrUint16) CsrStrLen(CSR_BT_IMAGE_HANDLES_DESCRIPTOR_TERMINATOR);
    CsrMemCpy(buffer, CSR_BT_IMAGE_HANDLES_DESCRIPTOR_TERMINATOR, length);
    buffer += length;

    return obexPacket;
}
#endif

