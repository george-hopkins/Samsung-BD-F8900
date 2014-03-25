#ifndef CSR_BT_OBEX_UTIL_H__
#define CSR_BT_OBEX_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_synergy.h"
#include "csr_unicode.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_obex_extraction_lib.h"
#include "csr_bt_obex_auth.h"

#ifdef __cplusplus
extern "C" { 
#endif
     
#define CSR_BT_OBEX_UTIL_STATUS_ACCEPTED                    0x00
#define CSR_BT_OBEX_UTIL_STATUS_EXCEPTION                   0x01
                                                            
#define CSR_BT_OBEX_UTIL_CONNECT_REQUEST_OPERATION          0x00
#define CSR_BT_OBEX_UTIL_CONNECT_RESPONSE_OPERATION         0x01
#define CSR_BT_OBEX_UTIL_DISCONNECT_REQUEST_OPERATION       0x02
#define CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION              0x03
#define CSR_BT_OBEX_UTIL_PUT_FINAL_REQUEST_OPERATION        0x04    
#define CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION              0x05
#define CSR_BT_OBEX_UTIL_GET_FINAL_REQUEST_OPERATION        0x06    
#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
#define CSR_BT_OBEX_UTIL_SETPATH_REQUEST_OPERATION          0x07
#endif
#define CSR_BT_OBEX_UTIL_ABORT_REQUEST_OPERATION            0x08
#define CSR_BT_OBEX_UTIL_ACTION_REQUEST_OPERATION           0x09
#define CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION          0x0A
#define CSR_BT_OBEX_UTIL_NAME_HEADER                        0x0B
#define CSR_BT_OBEX_UTIL_EMPTY_NAME_HEADER                  0x0C
#define CSR_BT_OBEX_UTIL_TYPE_HEADER                        0x0D
#define CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER                 0x0E
#define CSR_BT_OBEX_UTIL_LENGTH_HEADER                      0x0F
#define CSR_BT_OBEX_UTIL_TARGET_HEADER                      0x10
#define CSR_BT_OBEX_UTIL_WHO_HEADER                         0x11
#define CSR_BT_OBEX_UTIL_BODY_HEADER                        0x12
#define CSR_BT_OBEX_UTIL_END_BODY_HEADER                    0x13
#define CSR_BT_OBEX_UTIL_CONNECTIONID_HEADER                0x14
#define CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER              0x15
#define CSR_BT_OBEX_UTIL_AUTH_CHALLENGE_HEADER              0x16
#define CSR_BT_OBEX_UTIL_AUTH_RESPONSE_HEADER               0x17
#define CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER                0x18
#define CSR_BT_OBEX_UTIL_COUNT_HEADER                       0x19
#define CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_HEADER        0x1A
#define CSR_BT_OBEX_UTIL_DESTNAME_HEADER                    0x1B
#define CSR_BT_OBEX_UTIL_PERMISSIONS_HEADER                 0x1C
#define CSR_BT_OBEX_UTIL_ACTION_ID_HEADER                   0x1D
#define CSR_BT_OBEX_UTIL_SRMP_HEADER                        0x1E
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
#define CSR_BT_OBEX_UTIL_NUM_OF_HEADERS                     0x1F
#else
#define CSR_BT_OBEX_UTIL_NUM_OF_HEADERS                     0x1A
#endif

#define CSR_BT_OBEX_UTIL_UNDEFINE_HEADER                    0x00
                                                            
#define CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX          0xFF
#define CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX           0xFFFF
                                                            
#define CSR_BT_OBEX_L2CAP_PSM_ATTRIBUTE                     0x0200
#define CSR_BT_OBEX_UTIL_ACTION_ID_COPY_OBJECT              0x00
#define CSR_BT_OBEX_UTIL_ACTION_ID_MOVE_RENAME_OBJECT       0x01
#define CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS   0x02
#define CSR_BT_OBEX_UTIL_ACTION_ID_INVALID                  0xFF

#define CSR_BT_OBEX_UTIL_ACTION_READING_PERMISSION_USER     0x00010000
#define CSR_BT_OBEX_UTIL_ACTION_WRITING_PERMISSION_USER     0x00020000
#define CSR_BT_OBEX_UTIL_ACTION_DELETING_PERMISSION_USER    0x00040000
#define CSR_BT_OBEX_UTIL_ACTION_MODIFY_PERMISSION_USER      0x00800000
#define CSR_BT_OBEX_UTIL_ACTION_READING_PERMISSION_GROUP    0x00000100
#define CSR_BT_OBEX_UTIL_ACTION_WRITING_PERMISSION_GROUP    0x00000200
#define CSR_BT_OBEX_UTIL_ACTION_DELETING_PERMISSION_GROUP   0x00000400
#define CSR_BT_OBEX_UTIL_ACTION_MODIFY_PERMISSION_GROUP     0x00008000
#define CSR_BT_OBEX_UTIL_ACTION_READING_PERMISSION_OTHER    0x00000001
#define CSR_BT_OBEX_UTIL_ACTION_WRITING_PERMISSION_OTHER    0x00000002
#define CSR_BT_OBEX_UTIL_ACTION_DELETING_PERMISSION_OTHER   0x00000004
#define CSR_BT_OBEX_UTIL_ACTION_MODIFY_PERMISSION_OTHER     0x00000080
#define CSR_BT_OBEX_UTIL_ACTION_ALL_RESET_PERMISSION        0x00000000

#define CSR_BT_OBEX_SRMP_WAIT                               0x01
#define CSR_BT_OBEX_SRMP_INVALID                            0xFF

typedef struct
{
    CsrUint8        nonce[CSR_BT_NONCE_LENGTH];
    CsrUint16       passwordLength;
    CsrUint8        *password;
    CsrUint8        options;
    CsrUint8        *realm;
    CsrUint16       realmLength;
    CsrCharString       *userId;
} ObexUtilDigestChallengeType;

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilInit
 *
 *  DESCRIPTION
 *      This function initialize one instance of the OBEX util lib.
 *
 *    PARAMETERS
 *      pHandle                 : application queue handle
 *
 *      *instData               : The overloaded void pointer to the
 *                                profiles own instance data
 *
 *      instIdentifier          : Identifier given by the application.
 *                                When the OBEX profiles support more than 
 *                                one OBEX connection, this parameter is used 
 *                                to identify which OBEX connection the CM
 *                                upstream message belongs to
 *----------------------------------------------------------------------------*/
extern void * ObexUtilInit(CsrSchedQid pHandle, void * instData, CsrUint8 instIdentifier);

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilCmMsgHandler
 *
 *  DESCRIPTION
 *      This function handles upstream messages received from the Connection 
 *      Manager (CM).
 *
 *    PARAMETERS
 *      *instData               : The overloaded void pointer to the
 *                                profiles own instance data
 *      **msg                   : The message received from the scheduler
 *----------------------------------------------------------------------------*/     
extern CsrUint8 ObexUtilCmMsgHandler(void * obexUtilInstData, void **msg);

/* Function which CsrPmemFree this library private instance data and sets the 
   pointer to NULL. Note should only be used in the profiles deinit functions   */
extern void ObexUtilDeinit(void ** obexUtilInstData);

/* Defines the called back function and its arguments which is called when the 
   Cm Rfc Connect procedure demands the profile to select a service handle or 
   a list of service handles. When this function is called the profile must 
   either called the function ObexUtilSetServiceHandleIndexListResponse 
   to select one a more service handle indices or a ObexUtilDisconnectRequest to 
   cancel the CM RFC connect procedure. Please note the sdpTagList pointer 
   is NOT own by this library and MUST be CsrPmemFree by the profile               */                      
typedef void(*ObexUtilSelectSdpRecordFuncType)(void                     *instData,
                                               CsrUint16                entriesInSdpTaglist,
                                               CmnCsrBtLinkedListStruct *sdpTagList);


/* Defines the called back function and its arguments which is called 
   when the OBEX connect procedure is finish. Note that the sdpTag 
   and the obexPacket pointer is NOT own by this library and MUST 
   be CsrPmemFree by the profile                                                   */
typedef void(*ObexUtilConnectCfmFuncType)(void                      *instData,
                                          CsrBtResultCode           resultCode,
                                          CsrBtSupplier             resultSupplier,
                                          CsrBtDeviceAddr           deviceAddr,
                                          CsrBtConnId               cid,
                                          CsrUint16                 maxPeerObexPacketLength,
                                          CmnCsrBtLinkedListStruct  *sdpTag,
                                          CsrUint16                 obexPacketLength,
                                          CsrUint8                *obexPacket);



/* Defines the called back function and its arguments which is called when the 
   OBEX DISCONNECT procedure is finish. 
   Please note that the callback function for DISCONNECT can be call anytime 
   after the OBEX connection have been establish due to abnormal disconnect.
   Please note that the obexPacket pointer is NOT own by the obex util lib
   function and MUST therefore be CsrPmemFree                                      */
typedef void (*ObexUtilDisconnectIndFuncType)(void                  *instData,
                                              CsrBtReasonCode       reasonCode,
                                              CsrBtSupplier         reasonSupplier,
                                              CsrUint8            *obexPacket,
                                              CsrUint16             obexPacketLength);


/* Defines the called back function and its arguments which is called when 
   OBEX demands that the profile must respond to a OBEX authentification 
   message. 
   
   The 'options' parameter is defined as follows:
      Bit 0 controls the responding of a valid user Id. 
      If bit 0 is set it means that the application must response with a user 
      Id in an XXXX function. If bit 0 is not set the application can just 
      zero terminated the first character of the user Id string.
    
      Bit 1 indicates the access mode being offered by the sender. 
      If bit 1 is set the access mode is read only. If bit 1 is not set the 
      sender gives full access, e.g. both read and write.
    
      Bit 2 - 7 is reserved.

   The '* realm' parameter represent a displayable string indicating for the user 
   which userid and/or password to use. The first byte of the string is the 
   character set of the string. Note that the realm pointer is NOT own by this 
   library and MUST be CsrPmemFree by the profile                                     */
typedef void(*ObexUtilAuthenticateIndFuncType)(void             *instData,
                                               CsrBtDeviceAddr  deviceAddr,
                                               CsrUint8         options,
                                               CsrUint16        realmLength,
                                               CsrUint8       *realm);


typedef void(*ObexUtilPutContinueRequestFuncType)(void          *instData,
                                                  CsrUint16     obexPacketLength);

/* Defines the called back function and its arguments which is called when OBEX
   GET operation is finish. Regarding the Body/End Of Body header:
   
        If bodyHeader is FALSE then there is no Body or End Of Body header 
        present in the OBEX response packet

        If bodyHeader is TRUE then there is a Body or End Of Body header 
        present in the OBEX response packet

        If a body header is present and bodyLength and bodyOffset is 0 the
        header is empty, e.g no body payload is present.

        If a body header is present and bodyLength and bodyOffset is > 0 the
        header also includes body 'payload'                                     */
typedef void (*ObexUtilGetRequestFuncType)(void                   *instData,
                                           CsrBtObexResponseCode  responseCode,
                                           CsrBool                bodyHeader,
                                           CsrUint16              bodyLength,
                                           CsrUint16              bodyOffset,
                                           CsrUint8             *obexPacket,
                                           CsrUint16              obexPacketLength);

/* Defines the called back function and its arguments which is called when 
   either of the OBEX operations: PUT, SETPATH, or ABORT is finish. 
   Please note that the obexPacket pointer is NOT own by the obex util lib
   function and MUST therefore be CsrPmemFree                                      */
typedef void (*ObexUtilCommonRequestFuncType)(void                  *instData,
                                              CsrBtObexResponseCode responseCode,
                                              CsrUint8            *obexPacket,
                                              CsrUint16             obexPacketLength);

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilConnectRequest
 *
 *  DESCRIPTION
 *      This function will start a OBEX CONNECT operation, e.g make a Bluetooth
 *      service level connection and a OBEX connection. Please note that the 
 *      peer device implementation of the OBEX connect procedure are not require 
 *      to recognize more than the first 7 bytes, though this may restrict their 
 *      implementation
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      **sdpTag                : The info that the profile must read from the 
 *                                peer device service record
 *
 *      deviceAddr              : The Bluetooth address of the device to 
 *                                connect to
 *
 *      outgoingSecurityLevel   : Sets up the security level for new outgoing 
 *                                connection
 *
 *      maxLocalObexPacketLength: The maximum size OBEX packet that 
 *                                the local device can receive
 *
 *      rfcommMtu               : The MTU size of the underlying RFCOMM connection 
 *
 *      **target                : The target header. This header identifies the intended
 *                                target of the operation. Must be a allocated pointer 
 *                                which will be own by this library. Note if set to NULL 
 *                                this header will not be included in the OBEX CONNECT packet.
 *
 *      targetHeaderLength      : The length of the target header
 *
 *      numOfHeaders            : The number of headers in the headerPriorityTable. 
 *
 *      *headerPriorityTable    : The headerPriorityTable identifies the sequence
 *                                of how optionals headers can be included in the OBEX Connect 
 *                                Request packet. If the Count, Length, Description 
 *                                and the user defined header(s) must be included and the 
 *                                headers must be send in the same order as just describe above,
 *                                the headerPriorityTable must be set to 
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_COUNT_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_LENGTH_HEADER
 *                                headerPriorityTable[2] = CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[3] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER
 *                                and numOfHeaders must be set to 4. 
 *                                If only the Length and Count header shall be included, and Length
 *                                must be send before Count, then the headerPriorityTable is set to  
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_LENGTH_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_COUNT_HEADER
 *                                and numOfHeaders must be set to 2.
 *                                In the case that no headers must be included in the OBEX Connect 
 *                                Request packet then numOfHeaders must be set to 0
 *
 *      lpSuperVisionTimeout    : Number of micro seconds without data before 
 *                                entering sniff (if possible). Note setting
 *                                this parameter to CSR_BT_INFINITE_TIME (0) 
 *                                sniff will not be initiated
 *
 *      **digestChallenge       : Used to initiate the OBEX authentication of
 *                                the remote device. Note if set to NULL this
 *                                header will not be included in the OBEX
 *                                CONNECT packet
 *
 *      length                  : Length is used to express the approximate total length of 
 *                                the bodies of all the objects in that the client plans to send
 *
 *      count                   : Count is used to indicate the number of objects that will be
 *                                sent during this connection
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX CONNECT packet
 *
 *      **userDefinedHeader     : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX CONNECT packet.
 *                                
 *      userDefinedHeaderLength : The length of the userDefinedHeader
 *
 *      connectResultHandler    : The callback function that must be called
 *                                when the OBEX CONNECT operation is done.
 *                                E.g. connected, failed to established the 
 *                                connection or cancelled
 *
 *      authenticateIndHandler  : The callback function that must be called 
 *                                when the peer device has initiated the OBEX
 *                                authentication procedure. Note that if it is 
 *                                set to NULL the OBEX authentication procedure
 *                                is not supported
 *
 *      disconnectIndHandler    : The callback function that must be called
 *                                when the OBEX Connection is released normal
 *                                or abnormal
 *
 *      selectSdpRecordIndHandler : The callback function that must be called 
 *                                  if the profile want to select a service 
 *                                  handle or a list of service handles. Note
 *                                  if set to NULL this library will auto select
 *                                  one
 *
 *      windowSize              : Local server Rx window size for ERTM
 *
 *      localSrmEnable          : If TRUE Single Response Mode is enabled (if possible)
 *                                otherwise NOT. Information regarding Single Response
 *                                Mode can be found in IrDA spec Version 1.4 section 3.3

 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilConnectRequestEx(void                               *obexUtilInstData,
                                       CmnCsrBtLinkedListStruct           **sdpTag,
                                       CsrBtDeviceAddr                    deviceAddr,
                                       dm_security_level_t                outgoingSecurityLevel,
                                       CsrUint16                          maxLocalObexPacketLength,
                                       CsrUint16                          rfcommMtu,
                                       CsrUint8                           **target,
                                       CsrUint8                           targetHeaderLength,
                                       CsrUint8                           numOfHeaders,
                                       const CsrUint8                     *headerPriorityTable,
                                       CsrTime                            lpSuperVisionTimeout,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                       ObexUtilDigestChallengeType        **digestChallenge,
#endif
                                       CsrUint32                          length,
                                       CsrUint32                          count,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                       CsrUcs2String                      **description,
#endif                                       
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                       CsrUint16                          userDefinedHeaderLength,
                                       CsrUint8                           **userDefinedHeader,
#endif                                       
                                       ObexUtilConnectCfmFuncType         connectResultHandler,
                                       ObexUtilAuthenticateIndFuncType    authenticateIndHandler,
                                       ObexUtilDisconnectIndFuncType      disconnectIndHandler,
                                       ObexUtilSelectSdpRecordFuncType    selectSdpRecordIndHandler,
                                       CsrUint16                          windowSize,
                                       CsrBool                            localSrmEnable);

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
#define OBEX_CONNECT_PARAM_DIGEST_CHALLENGE(_x) _x,
#else
#define OBEX_CONNECT_PARAM_DIGEST_CHALLENGE(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_CONNECT_PARAM_DESCRIPTION(_x) _x,
#else
#define OBEX_CONNECT_PARAM_DESCRIPTION(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_CONNECT_PARAM_USER_DEFINE(_x) _x,
#else
#define OBEX_CONNECT_PARAM_USER_DEFINE(_x) 
#endif

#define ObexUtilConnectRequest(_obexUtilInstData, _sdpTag, _deviceAddr, _outgoingSecurityLevel,\
                               _maxLocalObexPacketLength, _rfcommMtu,\
                               _target, _targetHeaderLength,\
                               _numOfHeaders, _headerPriorityTable,\
                               _lpSuperVisionTimeout, _digestChallenge,\
                               _length, _count, _description,\
                               _userDefinedHeaderLength, _userDefinedHeader,\
                               _connectResultHandler, _authenticateIndHandler,\
                               _disconnectIndHandler, _selectSdpRecordIndHandler,\
                               _windowSize, _localSrmEnable) \
      ObexUtilConnectRequestEx(_obexUtilInstData,\
                               _sdpTag,\
                               _deviceAddr,\
                               _outgoingSecurityLevel,\
                               _maxLocalObexPacketLength,\
                               _rfcommMtu,\
                               _target,\
                               _targetHeaderLength,\
                               _numOfHeaders,\
                               _headerPriorityTable,\
                               _lpSuperVisionTimeout,\
                               OBEX_CONNECT_PARAM_DIGEST_CHALLENGE(_digestChallenge)\
                               _length,\
                               _count,\
                               OBEX_CONNECT_PARAM_DESCRIPTION(_description)\
                               OBEX_CONNECT_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                               OBEX_CONNECT_PARAM_USER_DEFINE(_userDefinedHeader)\
                               _connectResultHandler,\
                               _authenticateIndHandler,\
                               _disconnectIndHandler,\
                               _selectSdpRecordIndHandler,\
                               _windowSize,\
                               _localSrmEnable) 

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilSetServiceHandleIndexListResponse
 *
 *  DESCRIPTION
 *      This function is a response to the callback function selectSdpRecordIndHandler
 *      which were registered in the ObexUtilConnectRequest. In the response the profile 
 *      must have selected one or more service record handle(s).
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *     **serviceHandleIndexList : A list of service handle indices. 
 *                                Please note in the case where this 
 *                                parameter in NULL the connect
 *                                procedure will be cancel
 *
 *      nofServiceHandleIndicis : The number of indicis
 *                                in the serviceHandleIndexList.
 *                                Please note that in the case that 
 *                                nofServiceHandleIndicis > 1 then 
 *                                will this library try to connect 
 *                                to serviceHandleIndexList[0] first,
 *                                if this fails then serviceHandleIndexList[1] 
 *                                and so on. In this way the profile gives 
 *                                priority of which service record handle 
 *                                it perfers        
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_UTIL_SET_SERVICE_HANDLE_RES
extern CsrUint8 ObexUtilSetServiceHandleIndexListResponse(void            *obexUtilInstData,
                                                          CsrUint16       **serviceHandleIndexList,
                                                          CsrUint16       nofServiceHandleIndicis);
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilDisconnectRequest
 *
 *  DESCRIPTION
 *      This function will either cancel an ongoing OBEX CONNECT operation 
 *      or release the OBEX connection if it is allready connected. Please note 
 *      that an OBEX Disconnect Request packet must fit in one OBEX packet 
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      normalDisconnect        : If TRUE the OBEX DISCONNECT operation is issued
 *                                otherwise not. Please note it is permissible to 
 *                                terminate the transport connection (In this case
 *                                the Bluetooth connection) without issuing the 
 *                                OBEX DISCONNECT operation
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX DISCONNECT packet
 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilDisconnectRequestExt(void               *obexUtilInstData,
                                          CsrBool            normalDisconnect
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                          , CsrUcs2String      **description
#endif
                                          );

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define ObexUtilDisconnectRequest(_obexUtilInstData, _normalDisconnect, _description) \
            ObexUtilDisconnectRequestExt(_obexUtilInstData, _normalDisconnect, _description)
#else
#define ObexUtilDisconnectRequest(_obexUtilInstData, _normalDisconnect, _description) \
            ObexUtilDisconnectRequestExt(_obexUtilInstData, _normalDisconnect)
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilAbortRequest
 *
 *  DESCRIPTION
 *      This function is used when the OBEX client decides to terminated a 
 *      multi-packet operation (such as PUT) before it normally end. In the 
 *      case the peer device don't reply to a OBEX ABORT request the Bluetooth
 *      connection will be release. Please note that an OBEX Abort Request
 *      packet must fit in one OBEX packet.
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX ABORT packet
 *
 *      abortResultHandler      : The callback function that must be called
 *                                when the OBEX ABORT operation is done.
 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilAbortRequestEx(void                         *obexUtilInstData,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                    CsrUcs2String                 **description,
#endif
                                    ObexUtilCommonRequestFuncType abortResultHandler);

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define ObexUtilAbortRequest(_obexUtilInstData, _description, _abortResultHandler) \
            ObexUtilAbortRequestEx(_obexUtilInstData, _description, _abortResultHandler)
#else
#define ObexUtilAbortRequest(_obexUtilInstData, _description, _abortResultHandler) \
            ObexUtilAbortRequestEx(_obexUtilInstData, _abortResultHandler)
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilPutRequest
 *
 *  DESCRIPTION
 *      This function will start a OBEX PUT operation.
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      headerPriorityTable[6]  : The headerPriorityTable identifies the sequence
 *                                of how the Length, Type, Name, Description,  
 *                                Application Parameters, and user defined header(s)
 *                                must be sent. If the headers shall be send in the 
 *                                same order as just describe the headerPriorityTable
 *                                is set to 
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_LENGTH_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_TYPE_HEADER
 *                                headerPriorityTable[2] = CSR_BT_OBEX_UTIL_NAME_HEADER
 *                                headerPriorityTable[3] = CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[4] = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER
 *                                headerPriorityTable[5] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER
 *                                Note that all entries in the headerPriorityTable must
 *                                be set. In the case where a header is not surposed to
 *                                be added to the OBEX packet the headerPriorityTable 
 *                                entry is set to OBEX_UTIL_UNDEFINE_HEADER
 *
 *      lengthOfObject          : The total length in bytes of the object.
 *
 *      *type                   : A NULL terminated ASCII text string describing 
 *                                the type of the object. Must be const data, e.g. 
 *                                not an allocated pointer.
 *                                Note if set to NULL this header will 
 *                                not be included in the OBEX PUT packet       
 *
 *      **name                  : A NULL terminated Unicode text string describing
 *                                the name of the object. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX PUT packet
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX PUT packet
 *
 *      **appParametersHeader   : The Application Parameters Header is used by the 
 *                                application (and protocols) layered above OBEX to
 *                                convey additional information in the OBEX PUT
 *                                Request packet. Note this header must be build 
 *                                correct after the OBEX spec (IrOBEX version 1.2 
 *                                section 2.2.12). This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL this header 
 *                                will not be included in the OBEX Put Request packet.
 *                                Note from this library point of view this header
 *                                is the same as the userDefinedHeader, see below
 *
 *      appParametersHeaderLength:The length of the Application parameters header  
 *
 *      **userDefinedHeader     : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX PUT packet.
 *                                
 *      userDefinedHeaderLength : The length of the userDefinedHeader. 
 *
 *      putContinueHandler      : The callback function that must be called 
 *                                when it is a multi-packet OBEX PUT operation.
 *                                In the case where the application 
 *                                don't want to include any body headers this
 *                                callback function must be set to NULL
 *
 *      putResultHandler        : The callback function that must be called
 *                                when the OBEX PUT operation is done.
 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilPutRequestEx(void                                  *obexUtilInstData,
                                   CsrUint8                              headerPriorityTable[6],
                                   CsrUint32                             lengthOfObject,         
                                   const CsrUint8                        *type,                 
                                   CsrUcs2String                         **name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                   CsrUcs2String                         **description,
#endif                                   
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                   CsrUint16                             appParametersHeaderLength,
                                   CsrUint8                              **appParametersHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                   CsrUint16                             userDefinedHeaderLength,  
                                   CsrUint8                              **userDefinedHeader,      
#endif                                   
                                   ObexUtilPutContinueRequestFuncType    putContinueHandler,
                                   ObexUtilCommonRequestFuncType         putResultHandler);  


/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_PUT_PARAM_DESCRIPTION(_x) _x,
#else
#define OBEX_PUT_PARAM_DESCRIPTION(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
#define OBEX_PUT_PARAM_APP_PARAMS(_x) _x,
#else
#define OBEX_PUT_PARAM_APP_PARAMS(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_PUT_PARAM_USER_DEFINE(_x) _x,
#else
#define OBEX_PUT_PARAM_USER_DEFINE(_x) 
#endif

#define ObexUtilPutRequest(_obexUtilInstData,\
                           _headerPriorityTable,\
                           _lengthOfObject,\
                           _type, _name, _description,\
                           _appParametersHeaderLength, _appParametersHeader,\
                           _userDefinedHeaderLength, _userDefinedHeader,\
                           _putContinueHandler, _putResultHandler)\
      ObexUtilPutRequestEx(_obexUtilInstData,\
                           _headerPriorityTable,\
                           _lengthOfObject,\
                           _type,\
                           _name,\
                           OBEX_PUT_PARAM_DESCRIPTION(_description)\
                           OBEX_PUT_PARAM_APP_PARAMS(_appParametersHeaderLength)\
                           OBEX_PUT_PARAM_APP_PARAMS(_appParametersHeader)\
                           OBEX_PUT_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                           OBEX_PUT_PARAM_USER_DEFINE(_userDefinedHeader)\
                           _putContinueHandler,\
                           _putResultHandler)

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilPutContinueRequest
 *
 *  DESCRIPTION
 *      This function will 
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      finalFlag               : Use to identify if it is the last chunk of the 
 *                                object or not. 
 *                                If it is the last chunk it must be set to: 
 *                                TRUE.
 *                                If it is not the last chunk it must be set to:
 *                                FALSE.
 *
 *      **body                  : A body chunk of an object. Must be a allocated 
 *                                pointer which will be own by this library. Note 
 *                                that if set to NULL a zero length body object is 
 *                                send
 *
 *      bodyLength              : The length of the body chunk
 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilPutContinueRequest(void                       *obexUtilInstData,
                                           CsrBool                    finalFlag,
                                           CsrUint8                   **body,
                                           CsrUint16                  bodyLength);



/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilGetRequest
 *
 *  DESCRIPTION
 *      This function will start a OBEX GET Request operation.
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *
 *      headerPriorityTable[5]  : The headerPriorityTable identifies the sequence
 *                                of how the Type, Name, Description,  
 *                                Application Parameters, and user defined header(s)
 *                                must be sent. If the headers shall be send in the 
 *                                same order as just describe the headerPriorityTable
 *                                is set to 
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_TYPE_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_NAME_HEADER
 *                                headerPriorityTable[2] = CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[3] = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER
 *                                headerPriorityTable[4] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER
 *                                Note that all entries in the headerPriorityTable must
 *                                be set. In the case where a header is not surposed to
 *                                be added to the OBEX packet the headerPriorityTable 
 *                                entry is set to OBEX_UTIL_UNDEFINE_HEADER
 *
 *      *type                   : A NULL terminated ASCII text string describing 
 *                                the type of the object. Must be const data, e.g. 
 *                                not an allocated pointer.
 *                                Note if set to NULL this header will 
 *                                not be included in the OBEX GET packet       
 *
 *      **name                  : A NULL terminated Unicode text string describing
 *                                the name of the object. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX GET packet
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX GET packet
 *
 *     **appParametersHeader    : The Application Parameters Header is used by the 
 *                                application (and protocols) layered above OBEX to
 *                                convey additional information in the OBEX GET
 *                                Request packet. Note this header must be build 
 *                                correct after the OBEX spec (IrOBEX version 1.2 
 *                                section 2.2.12). This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL this header 
 *                                will not be included in the OBEX Get Request packet.
 *                                Note from this library point of view this header
 *                                is the same as the userDefinedHeader, see below
 *
 *      appParametersHeaderLength:The length of the Application parameters header  
 *
 *      **userDefinedHeader      : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX GET packet.
 *                                
 *      userDefinedHeaderLength : The length of the userDefinedHeader. 
 *
 *
 *      srmpValue               : The Single Response Parameter (SRMP) can be used to
 *                                facilitate additional header exchange, beyond the 
 *                                first response packet exchange used to enable SRM.
 *                                It must be set to either:
 *
 *                                CSR_BT_OBEX_SRMP_WAIT: Client instructs the Server 
 *                                to wait for the next request packet during a GET operation.  
 *                                This case will utilize the 0x01 option for the SRMP header, 
 *                                and may be issued by the Client during a GET request to 
 *                                instruct the Server to wait, after sending its response packet, 
 *                                until the next GET request is issued by the Client.  
 *                                Once the Client issues a GET request including the Final Bit, 
 *                                no further GET requests shall occur during SRM mode, unless 
 *                                the SRMP header is used during this request packet. 
 *                                If used, the SRMP header shall be issued in the first GET request, 
 *                                and may be used in consecutive GET request packets to cause the 
 *                                Server to continue its wait; however, once the SRMP header is not 
 *                                issued in a GET request, the SRMP header shall not be used in 
 *                                another GET request for the duration of the operation.
 *
 *                                CSR_BT_OBEX_SRMP_INVALID, means that this SRMP 
 *                                header will be ignore. E.g. it will not be 
 *                                included in OBEX Response Packet. 
 *
 *      getResultHandler        : The callback function that must be called
 *                                when the OBEX GET operation needs a response
 *                                from the application or the operation is done.
 *
 *      getBodyHandler          : The callback function that must be called 
 *                                when a body header must be included OBEX GET 
 *                                Request operation. In the case where the application 
 *                                don't want to include any body headers this
 *                                callback function must be set to NULL
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
extern CsrUint8 ObexUtilGetRequestEx(void                                 *obexUtilInstData,
                                  CsrUint8                              headerPriorityTable[5],
                                  const CsrUint8                        *type,                 
                                  CsrUcs2String                         **name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                  CsrUcs2String                         **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                  CsrUint16                             appParametersHeaderLength,
                                  CsrUint8                              **appParametersHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                  CsrUint16                             userDefinedHeaderLength,  
                                  CsrUint8                              **userDefinedHeader,  
#endif
                                  CsrUint8                              srmpValue,    
                                  ObexUtilGetRequestFuncType            getResultHandler,
                                  ObexUtilPutContinueRequestFuncType    getBodyHandler);

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_GET_PARAM_DESCRIPTION(_x) _x,
#else
#define OBEX_GET_PARAM_DESCRIPTION(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
#define OBEX_GET_PARAM_APP_PARAMS(_x) _x,
#else
#define OBEX_GET_PARAM_APP_PARAMS(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_GET_PARAM_USER_DEFINE(_x) _x,
#else
#define OBEX_GET_PARAM_USER_DEFINE(_x) 
#endif

#define ObexUtilGetRequest(_obexUtilInstData,\
                                  _headerPriorityTable,\
                                  _type, _name, _description,\
                                  _appParametersHeaderLength, _appParametersHeader,\
                                  _userDefinedHeaderLength, _userDefinedHeader,\
                                  _srmpValue,\
                                  _getResultHandler,\
                                  _getBodyHandler)\
             ObexUtilGetRequestEx(_obexUtilInstData,\
                                  _headerPriorityTable,\
                                  _type,\
                                  _name,\
                                  OBEX_GET_PARAM_DESCRIPTION(_description)\
                                  OBEX_GET_PARAM_APP_PARAMS(_appParametersHeaderLength)\
                                  OBEX_GET_PARAM_APP_PARAMS(_appParametersHeader)\
                                  OBEX_GET_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                                  OBEX_GET_PARAM_USER_DEFINE(_userDefinedHeader)\
                                  _srmpValue,\
                                  _getResultHandler,\
                                  _getBodyHandler)



#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilGetContinueRequest
 *
 *  DESCRIPTION
 *      This function will 
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      srmpValue               : The Single Response Parameter (SRMP) can be used to
 *                                facilitate additional header exchange, beyond the 
 *                                first response packet exchange used to enable SRM.
 *                                It must be set to either:
 *
 *                                CSR_BT_OBEX_SRMP_WAIT: Client instructs the Server 
 *                                to wait for the next request packet during a GET operation.  
 *                                This case will utilize the 0x01 option for the SRMP header, 
 *                                and may be issued by the Client during a GET request to 
 *                                instruct the Server to wait, after sending its response packet, 
 *                                until the next GET request is issued by the Client.  
 *                                Once the Client issues a GET request including the Final Bit, 
 *                                no further GET requests shall occur during SRM mode, unless 
 *                                the SRMP header is used during this request packet. 
 *                                If used, the SRMP header shall be issued in the first GET request, 
 *                                and may be used in consecutive GET request packets to cause the 
 *                                Server to continue its wait; however, once the SRMP header is not 
 *                                issued in a GET request, the SRMP header shall not be used in 
 *                                another GET request for the duration of the operation.
 *
 *                                CSR_BT_OBEX_SRMP_INVALID, means that this SRMP 
 *                                header will be ignore. E.g. it will not be 
 *                                included in OBEX Response Packet. 
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
extern CsrUint8 ObexUtilGetContinueRequest(void     *obexUtilInstData,
                                           CsrUint8 srmpValue);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilGetBodyRequest
 *
 *  DESCRIPTION
 *      This function will add a body header in an OBEX GET Request packet
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      finalFlag               : Use to identify if it is the last chunk of the 
 *                                object or not. 
 *                                If it is the last chunk it must be set to: 
 *                                TRUE.
 *                                If it is not the last chunk it must be set to:
 *                                FALSE.
 *
 *      **body                  : A body chunk of an object. Must be a allocated 
 *                                pointer which will be own by this library. Note 
 *                                that if set to NULL a zero length body object is 
 *                                send
 *
 *      bodyLength              : The length of the body chunk
 *
 *      srmpValue               : The Single Response Parameter (SRMP) can be used to
 *                                facilitate additional header exchange, beyond the 
 *                                first response packet exchange used to enable SRM.
 *                                It must be set to either:
 *
 *                                CSR_BT_OBEX_SRMP_WAIT: Client instructs the Server 
 *                                to wait for the next request packet during a GET operation.  
 *                                This case will utilize the 0x01 option for the SRMP header, 
 *                                and may be issued by the Client during a GET request to 
 *                                instruct the Server to wait, after sending its response packet, 
 *                                until the next GET request is issued by the Client.  
 *                                Once the Client issues a GET request including the Final Bit, 
 *                                no further GET requests shall occur during SRM mode, unless 
 *                                the SRMP header is used during this request packet. 
 *                                If used, the SRMP header shall be issued in the first GET request, 
 *                                and may be used in consecutive GET request packets to cause the 
 *                                Server to continue its wait; however, once the SRMP header is not 
 *                                issued in a GET request, the SRMP header shall not be used in 
 *                                another GET request for the duration of the operation.
 *
 *                                CSR_BT_OBEX_SRMP_INVALID, means that this SRMP 
 *                                header will be ignore. E.g. it will not be 
 *                                included in OBEX Response Packet. 
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
extern CsrUint8 ObexUtilGetBodyRequest(void        *obexUtilInstData,
                                       CsrBool     finalFlag,
                                       CsrUint8    **body,
                                       CsrUint16   bodyLength,
                                       CsrUint8    srmpValue);
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilSetPathRequest
 *
 *  DESCRIPTION
 *      This function is used when the OBEX client to set the "current
 *      directory" on the receiving side in order to enable transfers
 *      that need additional path information. Please note that an OBEX
 *      Setpath Request packet must fit in one OBEX packet 
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      flags                   : The flags have the following meanings:
 *
 *                                bit 0: Backup a level before applying name 
 *                                bit 1: Don't create directory if it does not exist 
 *                                bit 2: Reserved 
 *                                bit 3: Reserved 
 *                                bit 4: Reserved 
 *                                bit 5: Reserved
 *                                bit 6: Reserved
 *                                bit 7: Reserved
 *                                                            
 *                                Note the CSR_UNUSED flag bits must be set to zero.
 *
 *      headerPriorityTable[3]  : The headerPriorityTable identifies the sequence
 *                                of how the name header, description header and 
 *                                user defined header(s) must be sent. If the name header 
 *                                must be sent first followed by the description header 
 *                                and the user defined header(s) as the last one the 
 *                                headerPriorityTable is set to 
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_NAME_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[2] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER
 *                                Note that all entries in the headerPriorityTable 
 *                                must be set. In the case where a header is not surposed
 *                                to be added to the OBEX packet the headerPriorityTable 
 *                                entry is set to OBEX_UTIL_UNDEFINE_HEADER. E.g. if only
 *                                the name header must be included in the OBEX
 *                                packet then the headerPriorityTable must be set to
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_NAME_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER
 *                                headerPriorityTable[2] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER
 *                                and the *userDefinedHeader and description pointers 
 *                                must be NULL. Note in the special case where an
 *                                empty name header (defined as a Name header of length 3
 *                                one byte opcode + two byte length) must be sent, the *name 
 *                                pointer must be set to NULL and the headerPriorityTable[x] 
 *                                must be set to OBEX_UTIL_EMPTY_NAME_HEADER
 *
 *      **name                  : A NULL terminated Unicode text string describing
 *                                the path name. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX SETPATH packet
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX SETPATH packet
 *
 *      **userDefinedHeader     : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX SETPATH packet
 *                                
 *      userDefinedHeaderLength : The length of the userDefinedHeader
 *
 *      setPathResultHandler    : The callback function that must be called
 *                                when the OBEX SETPATH operation is done.
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
extern CsrUint8 ObexUtilSetPathRequestEx(void                          *obexUtilInstData,
                                      CsrUint8                       flags,
                                      CsrUint8                       headerPriorityTable[3],
                                      CsrUcs2String                  **name,   
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                      CsrUcs2String                  **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                      CsrUint16                      userDefinedHeaderLength,
                                      CsrUint8                       **userDefinedHeader,
#endif
                                      ObexUtilCommonRequestFuncType  setPathResultHandler);

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_SET_PATH_PARAM_DESCRIPTION(_x) _x,
#else
#define OBEX_SET_PATH_PARAM_DESCRIPTION(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_SET_PATH_PARAM_USER_DEFINE(_x) _x,
#else
#define OBEX_SET_PATH_PARAM_USER_DEFINE(_x) 
#endif
#define ObexUtilSetPathRequest(_obexUtilInstData,\
                               _flags,\
                               _headerPriorityTable,\
                               _name,\
                               _description,\
                               _userDefinedHeaderLength,\
                               _userDefinedHeader,\
                               _setPathResultHandler)\
      ObexUtilSetPathRequestEx(_obexUtilInstData,\
                               _flags,\
                               _headerPriorityTable,\
                               _name,\
                               OBEX_SET_PATH_PARAM_DESCRIPTION(_description)\
                               OBEX_SET_PATH_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                               OBEX_SET_PATH_PARAM_USER_DEFINE(_userDefinedHeader)\
                               _setPathResultHandler)

#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilCliAuthenticateResponse
 *
 *  DESCRIPTION
 *      This function is used
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      passwordLength          : The length of the password. 
 *
 *      **password              : The password is the secret known by both server 
 *                                and client. An MD5  hashing function is applied 
 *                                to the password string to produce a 128 bit 
 *                                string. This resulting string is the request-digest 
 *                                in the OBEX Digest Response header. Must be a 
 *                                allocated pointer which will be own by this 
 *                                library. This pointer must not be NULL  
 *
 *      userIdLength            : The length of the user id. Maximum size is 
 *                                20 bytes 
 *
 *      **userId                : A userId is required if bit 0 is set in 'options' 
 *                                parameter in the ObexUtilAuthenticateIndFuncType 
 *                                structure. The userId is used by the peer device
 *                                to identify the proper password. Must be a 
 *                                allocated pointer which will be own by this 
 *                                library. Note if set to NULL it will not be 
 *                                included in the OBEX Digest Response header 
 *
 *      **chal                  : Used to initiate the OBEX authentication of
 *                                the remote device. Note if set to NULL this
 *                                header will not be included in the OBEX
 *                                CONNECT Response packet. Note if the header
 *                                is set the responseCode MUST be set to 
 *                                CSR_BT_OBEX_UNAUTHORIZED_CODE. If not this 
 *                                header will NOT be included
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
extern CsrUint8 ObexUtilCliAuthenticateResponse(void                        *obexUtilInstData,
                                                CsrUint16                   passwordLength,
                                                CsrUint8                    **password,
                                                CsrCharString               **userId,
                                                ObexUtilDigestChallengeType **chal);
#endif
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilActionRequest
 *
 *  DESCRIPTION
 *
 *      This function will start a OBEX Action Request operation which can be
 *      used to:
 *          Copy an object from one location to another.
 *          Move an object from one location to another
 *          Sets the access permissions of an object or folder.
 *          
 *      Please note that an OBEX Action Request packet must fit in one OBEX packet.
 *          
 *              
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      actionId                : The actionId is a 1-byte quantity for specifying 
 *                                the action of the ACTION operation. The actionId
 *                                must be set to one of the following values:
 *
 *                                CSR_BT_OBEX_UTIL_ACTION_ID_COPY_OBJECT.
 *                                CSR_BT_OBEX_UTIL_ACTION_ID_MOVE_RENAME_OBJECT.
 *                                CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS.
 *                                
 *
 *      headerPriorityTable[2]  : The headerPriorityTable identifies the sequence
 *                                of how the description header and user defined header(s) 
 *                                must be sent. If the description header 
 *                                must be sent first followed by the user defined header(s)
 *                                the headerPriorityTable is set to 
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER
 *                                Note that all entries in the headerPriorityTable 
 *                                must be set. In the case where a header is not surposed
 *                                to be added to the OBEX packet the headerPriorityTable 
 *                                entry is set to OBEX_UTIL_UNDEFINE_HEADER. E.g. if only
 *                                the description header must be included in the OBEX
 *                                packet then the headerPriorityTable must be set to
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER
 *                                and the *userDefinedHeader pointer must be NULL. 
 *
 *      **name                  : A NULL terminated Unicode text string which indicates
 *                                the name of the object to perform the action upon. 
 *                                Must be a allocated pointer which will be own by 
 *                                this library. Note this header is mandatory to use.
 *
 *      **destName              : A NULL terminated Unicode text string which indicates the
 *                                destination name for a Copy or Move/Rename action. This name
 *                                shall be express as relative path names to the current folder and
 *                                must be a allocated pointer which will be own by this library. 
 *                                Note this header is only used for Copy and Move/Rename actions,
 *                                and is mandatory for these actions. This means that if 'actionId'  
 *                                is set to CSR_BT_OBEX_UTIL_ACTION_ID_COPY_OBJECT or   
 *                                CSR_BT_OBEX_UTIL_ACTION_ID_MOVE_RENAME_OBJECT 'destName' must be set.
 *                                Otherwise it must be set to NULL.
 *
 *      permission              : A 4-byte unsigned integer where the 4 bytes describe bit marks representing
 *                                the various permission values. It is used for setting "Read", "Write", 
 *                                "Delete" and "Modify" permissions for files and folders. The permission
 *                                are applied to three different permissions levels, which are "User", 
 *                                "Group" and "Other". In order to set the parameter 'permission' the 
 *                                following mask values are defined
 *
 *                                CSR_BT_OBEX_UTIL_ACTION_READING_PERMISSION_USER   (If used Reading permissions is granted for user)
 *                                CSR_BT_OBEX_UTIL_ACTION_WRITING_PERMISSION_USER   (If used Writing permissions is granted for user)
 *                                CSR_BT_OBEX_UTIL_ACTION_DELETING_PERMISSION_USER  (If used Deleting permissions is granted for user)
 *                                CSR_BT_OBEX_UTIL_ACTION_MODIFY_PERMISSION_USER    (If used file access permissions can be changed by user)
 *                                CSR_BT_OBEX_UTIL_ACTION_READING_PERMISSION_GROUP  (If used Reading permissions is granted for group)
 *                                CSR_BT_OBEX_UTIL_ACTION_WRITING_PERMISSION_GROUP  (If used Writing permissions is granted for group)
 *                                CSR_BT_OBEX_UTIL_ACTION_DELETING_PERMISSION_GROUP (If used Deleting permissions is granted for group)
 *                                CSR_BT_OBEX_UTIL_ACTION_MODIFY_PERMISSION_GROUP   (If used file access permissions can be changed by group)
 *                                CSR_BT_OBEX_UTIL_ACTION_READING_PERMISSION_OTHER  (If used Reading permissions is granted for other)
 *                                CSR_BT_OBEX_UTIL_ACTION_WRITING_PERMISSION_OTHER  (If used Writing permissions is granted for other)
 *                                CSR_BT_OBEX_UTIL_ACTION_DELETING_PERMISSION_OTHER (If used Deleting permissions is granted for other)
 *                                CSR_BT_OBEX_UTIL_ACTION_MODIFY_PERMISSION_OTHER   (If used file access permissions can be changed by other)
 *                                CSR_BT_OBEX_UTIL_ACTION_ALL_RESET_PERMISSION      (If used all bit are reset, e.g. set to 0)  
 *  
 *                                Please note that this header shall only be used if the 'actionId' 
 *                                is set to CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS.
 *                                Also note that in order to do the Move Action operation the Client needs to
 *                                have both "Read" and "Delete" permission to the source file/folder.                      
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX Action packet
 *
 *      **userDefinedHeader     : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX SETPATH packet
 *                                
 *      userDefinedHeaderLength : The length of the userDefinedHeader
 *
 *      actionResultHandler     : The callback function that must be called
 *                                when the OBEX Action operation is done.
 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilActionRequestEx(void                          *obexUtilInstData,
                                      CsrUint8                      actionId,
                                      CsrUint8                      headerPriorityTable[2],
                                      CsrUcs2String                 **name,   
                                      CsrUcs2String                 **destName,
                                      CsrUint32                     permission,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                      CsrUcs2String                 **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                      CsrUint16                     userDefinedHeaderLength,
                                      CsrUint8                      **userDefinedHeader,
#endif
                                      ObexUtilCommonRequestFuncType actionResultHandler);
#endif


/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_ACTION_PARAM_DESCRIPTION(_x) _x,
#else
#define OBEX_ACTION_PARAM_DESCRIPTION(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_ACTION_PARAM_USER_DEFINE(_x) _x,
#else
#define OBEX_ACTION_PARAM_USER_DEFINE(_x) 
#endif

#define ObexUtilActionRequest(_obexUtilInstData,\
                              _actionId,\
                              _headerPriorityTable,\
                              _name, _destName,\
                              _permission,\
                              _description,\
                              _userDefinedHeaderLength,\
                              _userDefinedHeader,\
                              _actionResultHandler)\
      ObexUtilActionRequestEx(_obexUtilInstData,\
                              _actionId,\
                              _headerPriorityTable,\
                              _name,\
                              _destName,\
                              _permission,\
                              OBEX_ACTION_PARAM_DESCRIPTION(_description)\
                              OBEX_ACTION_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                              OBEX_ACTION_PARAM_USER_DEFINE(_userDefinedHeader)\
                              _actionResultHandler)


/* Defines the called back function and its arguments which is called 
   when an incoming OBEX connect request packet is received. Note that 
   the target pointer and the obexPacket pointer is NOT own by this 
   library and MUST be CsrPmemFree by the profile                                 */
typedef void(*ObexUtilServConnectFuncType)(void                    *instData,
                                           CsrBtDeviceAddr         deviceAddr,
                                           CsrBtConnId             cid,
                                           CsrUint16               maxPeerObexPacketLength,
                                           CsrUint16               obexResponsePacketLength,
                                           CsrUint32               length,
                                           CsrUint32               count,
                                           CsrUint16               targetHeaderLength,
                                           CsrUint8                *target,
                                           CsrBool                 authenticated,
                                           CsrBool                 challenged,
                                           CsrUint16               obexPacketLength,
                                           CsrUint8                *obexPacket);

/* Defines the called back function and its arguments which is called when 
   either an OBEX Disconnect request packet is received or the transport
   connection is release direct. Please note that the obexPacket pointer 
   is NOT own by this library and MUST be CsrPmemFree by the profile. In the case
   where the transport connection is released direct the obexPacket pointer
   will be NULL                                                             */
typedef void (*ObexUtilServDisconnectFuncType)(void                *instData,
                                               CsrBtDeviceAddr     deviceAddr,
                                               CsrBtReasonCode     reasonCode,
                                               CsrBtSupplier       reasonSupplier,
                                               CsrUint16           obexPacketLength,
                                               CsrUint8            *obexPacket);

/* Defines the called back function and its arguments which is called when 
   an SETPATH request packet is received. Please note that the obexPacket 
   pointer is NOT own by this library and MUST be CsrPmemFree by the profile      */
typedef void (*ObexUtilServSetpathFuncType)(void                    *instData,
                                            CsrUint16               obexResponsePacketLength,
                                            CsrUint8                flags,
                                            CsrUint8                constants,
                                            CsrBool                 authenticated,
                                            CsrUint16               obexPacketLength,
                                            CsrUint8                *obexPacket);

/* Defines the called back function and its arguments which is called when 
   an PUT request packet is received. Please note that the obexPacket 
   pointer is NOT own by this library and MUST be CsrPmemFree by the profile      */
typedef void (*ObexUtilServPutFuncType)(void                    *instData,
                                        CsrUint16               obexResponsePacketLength,
                                        CsrBool                 authenticated,
                                        CsrBool                 finalBitSet,
                                        CsrUint16               bodyLength,
                                        CsrUint16               bodyOffset,
                                        CsrUint16               obexPacketLength,
                                        CsrUint8                *obexPacket);

/* Defines the called back function and its arguments which is called when 
   an GET request packet is received. Please note that the obexPacket 
   pointer is NOT own by this library and MUST be CsrPmemFree by the profile      */
typedef void (*ObexUtilServGetFuncType)(void                    *instData,
                                        CsrUint16               obexResponsePacketLength,
                                        CsrBool                 authenticated,
                                        CsrBool                 finalBitSet,
                                        CsrUint16               obexPacketLength,
                                        CsrUint8                *obexPacket);


/* Defines the called back function and its arguments which is called when 
   either an OBEX ABORT request packet is 
   received. Please note that the obexPacket pointer is NOT own by this 
   library and MUST be CsrPmemFree by the profile                                 */
typedef void (*ObexUtilServAbortFuncType)(void                    *instData,
                                          CsrUint16               descriptionOffset,
                                          CsrUint16               descriptionLength,
                                          CsrUint16               obexPacketLength,
                                          CsrUint8                *obexPacket);

/* Defines the called back function and its arguments which is called when
   the OBEX server has been activated                                       */
typedef void (*ObexUtilServActivateFuncType)(void                  *instData,
                                             CsrUint8              serverChannel,
                                             psm_t                 psm,
                                             CsrBtResultCode       resultCode,
                                             CsrBtSupplier         resultSupplier);

/* Defines the called back function and its arguments which is called when
   the OBEX server has been deactivated or set non-connectable              */
typedef void (*ObexUtilServDeactivateFuncType)(void                  *instData,
                                               CsrUint8              serverChannel,
                                               psm_t                 psm);

/* Defines the called back function and its arguments which is called when 
   an ACTION request packet is received. Please note that the obexPacket 
   pointer is NOT own by this library and MUST be CsrPmemFree by the profile.
   Note that the 'permissions' parameter only is valid if and only if the 
   'actionId' is CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS. Likewise
   the 'destNameOffset' is only valid if the 'actionId' is different from
   CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS                        */
typedef void (*ObexUtilServActionFuncType)(void                    *instData,
                                           CsrUint16               obexResponsePacketLength,
                                           CsrUint8                actionId,    
                                           CsrUint16               nameOffset,
                                           CsrUint16               destNameOffset,
                                           CsrUint32               permissions,
                                           CsrBool                 authenticated,
                                           CsrUint16               obexPacketLength,
                                           CsrUint8                *obexPacket);

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServActivateStart
 *
 *  DESCRIPTION
 *      This function will activate the OBEX server, e.g. register some callback 
 *      functions and register a SDP record and obtaining a local server channel
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      secIncoming             : Sets up the security level for new incoming 
 *                                connection
 *  
 *      profileUuid             : The profiles UUID. This UUID is used by the
 *                                security manager whenever it request for 
 *                                authorisation
 *
 *      lpSuperVisionTimeout    : Number of micro seconds without data before 
 *                                entering sniff (if possible). Note setting
 *                                this parameter to CSR_BT_INFINITE_TIME (0) 
 *                                sniff will not be initiated
 *
 *      serverChannel           : The local serverChannel. Note if the serverChannel
 *                                is set to CSR_BT_NO_SERVER and the parameter
 *                                servChServiceRecordIndex is not set to 
 *                                CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX
 *                                this library will obtain one.
 *
 *      psm                     : The local psm. Note if the psm is set to 
 *                                L2CA_PSM_INVALID and the parameter
 *                                psmServiceRecordIndex is not set to
 *                                CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX
 *                                this library will obtain one
 *
 *      serviceRecordLength     : The number of bytes in the service record.
 *
 *      servChServiceRecordIndex: The index of where the local server channel must 
 *                                be place in the service record. If OBEX over 
 *                                RFCOMM is not supported this parameter must be 
 *                                set to CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX
 *
 *      psmServiceRecordIndex   : The index of where the local psm must be place 
 *                                in the service record. If OBEX over L2CAP
 *                                is not supported this parameter must be 
 *                                set to CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX.
 *
 *   servChServiceRecordAltIndex: If OBEX is supported both over L2CAP and RFCOMM, 
 *                                and if the local serverChannel must be inserted 
 *                                twice this parameter must be used. Otherwise it
 *                                must be set to CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX
 *
 *      **serviceRecord         : The service record which is a list of attribute IDs 
 *                                and values. Must be a allocated pointer which will 
 *                                be own by this library. Note if set to NULL no Service
 *                                Record will be register to the SDS layer
 *
 *      classOfDevice           : The Bluetooth class of device of the peer device. 
 *                                For more information refers to the Bluetooth SIG
 *                                Assigned Numbers web page. Note only bit no 13 to 
 *                                bit no 23 must be set.
 *
 *      connectIndHandler       : The callback function that must be called
 *                                when a OBEX CONNECT Request packet is received.
 *                                Note the application must used the function
 *                                ObexUtilServConnectResponse to accept or reject 
 *                                the OBEX connection.
 *
 *      putIndHandler           : The callback function that must be called when
 *                                the first OBEX PUT Request packet is received.
 *                                Note the application must used the function
 *                                ObexUtilServPutResponse to accept or reject 
 *                                the OBEX PUT operation.
 *                                
 *      getIndHandler           : The callback function that must be called when
 *                                the first OBEX GET Request packet is received.
 *                                Note the application must used the function
 *                                ObexUtilServGetResponse to accept or reject 
 *                                the OBEX GET operation.
 *
 *      setpathIndHandler       : The callback function that must be called when
 *                                a OBEX SETPATH Request packet is received.
 *                                Note the application must used the function
 *                                ObexUtilServSetpathResponse to accept or reject 
 *                                the OBEX SETPATH operation.
 *
 *      abortIndHandler         : The callback function that must be called
 *                                when a OBEX ABORT Request packet is received. 
 *
 *      disconnectIndHandler    : The callback function that must be called
 *                                when the OBEX Connection is released normal
 *                                or abnormal
 *
 *      authenticateIndHandler  : The callback function that must be called 
 *                                when the peer device has initiated the OBEX
 *                                authentication procedure. Note that if it is 
 *                                set to NULL the OBEX authentication procedure
 *                                is not supported
 *
 *      activateCfmHandler      : The callback function that must be called 
 *                                when the OBEX Server has been activated
 *
 *      actionIndHandler        : The callback function that must be called when
 *                                a OBEX ACTION Request packet is received.
 *                                Note the application must used the function
 *                                ObexUtilServActionResponse to accept or reject 
 *                                the OBEX ACTION operation.
 *
 *      localMaxPacketSize      : Maximum OBEX packet size we accept
 *
 *      windowSize              : Local server Rx window size for ERTM
 *
 *      localSrmEnable          : If TRUE Single Response Mode is enabled (if possible)
 *                                otherwise NOT. Information regarding Single Response
 *                                Mode can be found in IrDA spec Version 1.4 section 3.3
 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilServActivateStart(void                             *obexUtilInstData,
                                          dm_security_level_t              secIncoming,
                                          uuid16_t                         profileUuid,
                                          CsrTime                          lpSuperVisionTimeout,
                                          CsrUint8                         serverChannel,
                                          psm_t                            psm, 
                                          CsrUint16                        serviceRecordLength,
                                          CsrUint16                        servChServiceRecordIndex,
                                          CsrUint16                        psmServiceRecordIndex,
                                          CsrUint16                        servChServiceRecordAltIndex,
                                          CsrUint8                         **serviceRecord,
                                          CsrUint24                        classOfDevice,
                                          ObexUtilServConnectFuncType      connectIndHandler,
                                          ObexUtilServPutFuncType          putIndHandler,
                                          ObexUtilServGetFuncType          getIndHandler,
                                          ObexUtilServSetpathFuncType      setpathIndHandler,
                                          ObexUtilServAbortFuncType        abortIndHandler,
                                          ObexUtilServDisconnectFuncType   disconnectIndHandler,
                                          ObexUtilAuthenticateIndFuncType  authenticateIndHandler,
                                          ObexUtilServActivateFuncType     activateCfmHandler,
                                          ObexUtilServActionFuncType       actionIndHandler,
                                          CsrUint16                        localMaxPacketSize,
                                          CsrUint16                        windowSize,
                                          CsrBool                          localSrmEnable);

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServDeactivateStart
 *
 *  DESCRIPTION
 *      This function will deactivate the OBEX server, e.g. unregister some 
 *      callback functions and unregister the SDP record and releaseing the 
 *      obtained local server channel. Note if the local server is 
 *      connectable it will be set non-connectable and if a transport 
 *      connection is present it will be release. Also note that if
 *      ObexUtilServDeactivateStart is called while 
 *      ObexUtilServNonConnectableStart procedure is running the deactivate
 *      will overwrite the non-connectable procedure which mean that the
 *      callback deactivateCfmHandler will be called instead of the 
 *      nonConnectableCfmHandler 
 *      
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      serverChannel           : The local serverChannel. Note if the 
 *                                serverChannel is set to CSR_BT_NO_SERVER this 
 *                                library will release the local server channel
 *                                In the case where the server channel is not
 *                                released the profile must keep the server channel 
 *                                and reused it later in a ObexUtilServActivateStart
 *
 *      psm                     : The local psm. Note if the psm is set to 
 *                                L2CA_PSM_INVALID this library will release
 *                                the local psm. In the case where the psm is 
 *                                not released the profile must keep the psm
 *                                and reused it later in a ObexUtilServActivateStart
 *
 *      deactivateCfmHandler    : The callback function that must be called 
 *                                when the OBEX Server has been deactivated
 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilServDeactivateStart(void                           *obexUtilInstData,
                                            CsrUint8                       serverChannel,
                                            psm_t                          psm, 
                                            ObexUtilServDeactivateFuncType deactivateCfmHandler);

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServConnectableStart
 *
 *  DESCRIPTION
 *      This function will set the OBEX server connectable, e.g the library will 
 *      make sure that the server always are connectable until it received a 
 *      ObexUtilServNonConnectableStart. Note that the server must be activeted
 *      this function can be called
 *
 *    PARAMETERS
 *      *obexUtilInstData        : The overloaded void pointer to this 
 *                                 library private instance data
 *
 *      rfcommMtu                : The MTU size of the underlying RFCOMM connection 
 *
 *      l2caMtu                  : The MTU size of the underlying l2CAP connection
 *                                 Note it is very IMPORTENT that l2caMtu is set
 *                                 to the same as maxLocalObexPacketLength in the
 *                                 ObexUtilServConnectResponse because an incoming
 *                                 OBEX packet must fit in a single L2CAP packet 
 *
 *      *sdpTag                  : The info that the profile must read from the 
 *                                 peer device service record
 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilServConnectableStart(void       *obexUtilInstData, 
                                             CsrUint16  rfcommMtu,
                                             CsrUint16  l2caMtu);

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServNonConnectableStart
 *
 *  DESCRIPTION
 *      This function will set the OBEX server non-connectable. In the case that
 *      a transport connection is present it will be release
 *
 *    PARAMETERS
 *      *obexUtilInstData        : The overloaded void pointer to this 
 *                                 library private instance data
 *
 *      nonConnectableCfmHandler : The callback function that must be called 
 *                                 when the OBEX Server is non-connectable 
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_UTIL_SERV_NON_CONNECTABLE
extern CsrUint8 ObexUtilServNonConnectableStart(void                           *obexUtilInstData,
                                                ObexUtilServDeactivateFuncType nonConnectableCfmHandler);
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServConnectResponse
 *
 *  DESCRIPTION
 *      This function will send an OBEX CONNECT response packet. Note that this
 *      response packet must fit in a single OBEX packet. In order to ensure 
 *      this,the server must never response with more data than specified by
 *      the obexResponsePacketLength parameter, which is defined in the  
 *      ObexUtilServConnectFuncType. I.e.the following calculation must be TRUE:
 *
 *      obexResponsePacketLength >= (whoHeaderLength + userDefinedHeaderLength + 
 *                                   CsrUcs2ByteStrLen(description) + digestChallenge->realmLength)
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      responseCode            : The one byte responseCode. The list of valid 
 *                                response code values are defined in the IrOBEX 
 *                                specification  version 1.2 section 3.2 and 3.2.1
 *                                and can be found in csr_bt_obex.h
 *
 *      whoHeaderLength         : The length of the who header
 *
 *      **who                   : The who header. The value of the WHO header matches the
 *                                value of the of the target header received in the 
 *                                OBEX Connect Command. The WHO haeder must be a allocated 
 *                                pointer which will be own by this library. Note if set to 
 *                                NULL this header will not be include in the OBEX CONNECT 
 *                                RESPONSE packet. If a target header is present a who header
 *                                must be returned otherwise not. 
 *
 *      connectionId            : The Connection Identifier header. The server can identify 
 *                                the need for a connection to be assigned, by the presence 
 *                                of a Target header in the OBEX CONNECT REQUEST packet. Note
 *                                the connectionId will not be include in the OBEX CONNECT 
 *                                RESPONSE if it is set to CSR_BT_UNUSED_CONNECTION_ID
 *
 *      numOfHeaders            : The number of headers in the headerPriorityTable. 
 *
 *      *headerPriorityTable    : The headerPriorityTable identifies the sequence
 *                                of how optionals headers can be included in the OBEX Connect 
 *                                Response packet. If the Count, Length, Description 
 *                                and the user defined header(s) must be included and the 
 *                                headers must be send in the same order as just describe above,
 *                                the headerPriorityTable must be set to 
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_COUNT_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_LENGTH_HEADER
 *                                headerPriorityTable[2] = CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[3] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER
 *                                and numOfHeaders must be set to 4. 
 *                                If only the Length and Count header shall be included, and Length
 *                                must be send before Count, then the headerPriorityTable is set to  
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_LENGTH_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_COUNT_HEADER
 *                                and numOfHeaders must be set to 2.
 *                                In the case that no headers must be included in the OBEX Connect 
 *                                Response packet then numOfHeaders must be set to 0
 *
 *      maxLocalObexPacketLength: The maximum size OBEX packet that the local 
 *                                OBEX server can receive
 *
 *      **digestChallenge       : Used to initiate the OBEX authentication of
 *                                the remote device. Note if set to NULL this
 *                                header will not be included in the OBEX
 *                                CONNECT Response packet. Note if the header
 *                                is set the responseCode MUST be set to 
 *                                CSR_BT_OBEX_UNAUTHORIZED_CODE. If not this 
 *                                header will NOT be included
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header will not
 *                                be included in the OBEX CONNECT Response packet
 *                                
 *      userDefinedHeaderLength : The length of the userDefinedHeader
 *
 *      **userDefinedHeader     : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX CONNECT Response packet.
 *
 * directAuthenticateResponse   : According to the IrDA spec this parameter must be set to
 *                                FALSE. However some customers have demanded that the 
 *                                local server is able to send both the OBEX Authenticate
 *                                challenge header and the OBEX Authenticate Response 
 *                                header in the same OBEX CONNECT RESPONSE packet. E.g.
 *                                setting this parameter TRUE will break the Irda spec
 *----------------------------------------------------------------------------*/                                            
extern CsrUint8 ObexUtilServConnectResponseEx(void                           *obexUtilInstData,
                                            CsrBtObexResponseCode          responseCode,
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
                                            CsrUint16                      whoHeaderLength,
                                            CsrUint8                       **who,
#endif
                                            CsrUint32                      connectionId, 
                                            CsrUint8                       numOfHeaders,
                                            const CsrUint8                 *headerPriorityTable
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                            ,ObexUtilDigestChallengeType    **digestChallenge
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                            ,CsrUcs2String                  **description
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                            ,CsrUint16                      userDefinedHeaderLength
                                            ,CsrUint8                       **userDefinedHeader
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                            ,CsrBool                        directAuthenticateResponse
#endif
                                            );

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
#define OBEX_SRV_CONNECT_PARAM_WHO(_x) _x,
#else
#define OBEX_SRV_CONNECT_PARAM_WHO(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
#define OBEX_SRV_CONNECT_PARAM_CHALLENGE(_x) ,_x
#else
#define OBEX_SRV_CONNECT_PARAM_CHALLENGE(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_SRV_CONNECT_PARAM_DESCRIPTION(_x) ,_x
#else
#define OBEX_SRV_CONNECT_PARAM_DESCRIPTION(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_SRV_CONNECT_PARAM_USER_DEFINE(_x) ,_x
#else
#define OBEX_SRV_CONNECT_PARAM_USER_DEFINE(_x) 
#endif


#define ObexUtilServConnectResponse(_obexUtilInstData, _responseCode,\
                                    _whoHeaderLength, _who,\
                                    _connectionId, \
                                    _numOfHeaders, _headerPriorityTable,\
                                    _digestChallenge,\
                                    _description,\
                                    _userDefinedHeaderLength, _userDefinedHeader,\
                                    _directAuthenticateResponse)\
      ObexUtilServConnectResponseEx(_obexUtilInstData,\
                                    _responseCode,\
                                    OBEX_SRV_CONNECT_PARAM_WHO(_whoHeaderLength)\
                                    OBEX_SRV_CONNECT_PARAM_WHO(_who)\
                                    _connectionId, \
                                    _numOfHeaders,\
                                    _headerPriorityTable\
                                    OBEX_SRV_CONNECT_PARAM_CHALLENGE(_digestChallenge)\
                                    OBEX_SRV_CONNECT_PARAM_DESCRIPTION(_description)\
                                    OBEX_SRV_CONNECT_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                                    OBEX_SRV_CONNECT_PARAM_USER_DEFINE(_userDefinedHeader)\
                                    OBEX_SRV_CONNECT_PARAM_CHALLENGE(_directAuthenticateResponse))
                                    

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServSetPutContinueFunc
 *
 *  DESCRIPTION
 *      This function will register a temporary callback function that must be 
 *      called every time the next OBEX PUT Request packet is received. Note
 *      it only makes sense to register this callback function if it is a multi
 *      PUT operation, as the callback function register with this function call
 *      is delete as soon as the PUT operation is done and the callback function 
 *      registered ObexUtilServActivateStart will be use again
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      putContinueIndHandler   : The callback function that must be called when
 *                                the next OBEX PUT Request packet is received.
 *                                Note the application must used the function
 *                                ObexUtilServPutResponse to accept or reject 
 *                                the OBEX PUT operation. Please also note that
 *                                when the PUT operation is done this callback 
 *                                function is delete and the callback function 
 *                                that was register in the ObexUtilServActivateStart
 *                                is used again. E.g. calling this functions 
 *                                gives only meaning if ObexUtilServPutResponse
 *                                or ObexUtilServPutWithHeaderResponse is called 
 *                                with the CSR_BT_OBEX_CONTINUE_RESPONSE_CODE 
 *                                response code  
 *----------------------------------------------------------------------------*/  
extern CsrUint8 ObexUtilServSetPutContinueFunc(void                        *obexUtilInstData,
                                               ObexUtilServPutFuncType     putContinueIndHandler);


/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServPutWithHeaderResponse
 *
 *  DESCRIPTION
 *      This function will send a OBEX PUT response packet and allow the profile
 *      to defines some optionals response headers. Note that this response packet 
 *      must fit in a single OBEX packet. In order to ensure this,the server must 
 *      never response with more data than specified by the obexResponsePacketLength 
 *      parameter, which is defined in the ObexUtilServPutFuncType. I.e.the following 
 *      calculation must be TRUE:
 *
 *      obexResponsePacketLength >= (CsrUcs2ByteStrLen(name) + userDefinedHeaderLength + 
 *                                   CsrUcs2ByteStrLen(description) + appParametersHeaderLength +
 *                                   digestChallenge->realmLength)
 *      
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      responseCode            : The one byte responseCode. The list of valid 
 *                                response code values are defined in the IrOBEX 
 *                                specification  version 1.2 section 3.2 and 3.2.1 
 *
 *      **digestChallenge       : Used to initiate the OBEX authentication of
 *                                the remote device. Note if set to NULL this
 *                                header will not be included in the OBEX
 *                                CONNECT Response packet. Note if the header
 *                                is set the responseCode MUST be set to 
 *                                CSR_BT_OBEX_UNAUTHORIZED_CODE and it must be set
 *                                in the first response Packet. If not this 
 *                                header will NOT be included
 *
 *      headerPriorityTable[4]  : The headerPriorityTable identifies the sequence
 *                                of how the name header, description header, 
 *                                application parameters header and the  
 *                                user defined header(s) must be sent. If the name header 
 *                                must be sent first followed by the description header 
 *                                and the user defined header(s) as the last one the 
 *                                headerPriorityTable is set to 
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_NAME_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[2] = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER
 *                                headerPriorityTable[3] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER
 *                                Note that all entries in the headerPriorityTable 
 *                                must be set. In the case where a header is not surposed
 *                                to be added to the OBEX packet the headerPriorityTable 
 *                                entry is set to OBEX_UTIL_UNDEFINE_HEADER. E.g. if only
 *                                the name header must be included in the OBEX
 *                                packet then the headerPriorityTable must be set to
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_NAME_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER
 *                                headerPriorityTable[2] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER
 *                                headerPriorityTable[3] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER
 *                                and the *userDefinedHeader and description pointers 
 *                                must be NULL. Note in the special case where an
 *                                empty name header (defined as a Name header of length 3
 *                                one byte opcode + two byte length) must be sent, the *name 
 *                                pointer must be set to NULL and the headerPriorityTable[x] 
 *                                must be set to OBEX_UTIL_EMPTY_NAME_HEADER
 *
 *      **name                  : A NULL terminated Unicode text string describing
 *                                the name. Must be a allocated pointer which will
 *                                be own by this library. Note if set to NULL this 
 *                                header will not be included in the OBEX Put 
 *                                response packet
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX Put response packet
 *
 *      **appParametersHeader   : The Application Parameters Header is used by the 
 *                                application (and protocols) layered above OBEX to
 *                                convey additional information in the OBEX PUT
 *                                Request packet. Note this header must be build 
 *                                correct after the OBEX spec (IrOBEX version 1.2 
 *                                section 2.2.12). This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL this header 
 *                                will not be included in the OBEX Put Request packet.
 *                                Note from this library point of view this header
 *                                is the same as the userDefinedHeader, see below
 *
 *      appParametersHeaderLength:The length of the Application parameters header  
 *
 *      **userDefinedHeader     : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX Put response packet
 *                                
 *      userDefinedHeaderLength : The length of the userDefinedHeader
 *
 *      srmpValue               : The Single Response Parameter (SRMP) can be used to
 *                                facilitate additional header exchange, beyond the 
 *                                first response packet exchange used to enable SRM.
 *                                It must be set to either:
 *
 *                                CSR_BT_OBEX_SRMP_WAIT. E.g. If another response 
 *                                packet is not expected by the peer Client, this 
 *                                parameter value will additionally inform the 
 *                                Client that an additional response shall occur 
 *                                from the local server. Please note that this value 
 *                                will only be valid if SRM is Enabled. If SRM is 
 *                                not enable the library will 
 *                                consider it as CSR_BT_OBEX_SRMP_INVALID
 *
 *                                CSR_BT_OBEX_SRMP_INVALID, means that this SRMP 
 *                                header will be ignore. E.g. it will not be 
 *                                included in OBEX Response Packet. 
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_UTIL_SERV_PUT_GET_WITH_HEADER_EXT
extern CsrUint8 ObexUtilServPutWithHeaderResponseEx(void                      *obexUtilInstData,
                                                  CsrBtObexResponseCode       responseCode,
                                                  ObexUtilDigestChallengeType **digestChallenge,
                                                  CsrUint8                    headerPriorityTable[4],
                                                  CsrUcs2String               **name,   
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                                  CsrUcs2String               **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                                  CsrUint16                   appParametersHeaderLength,
                                                  CsrUint8                    **appParametersHeader,
#endif                                                  
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                                  CsrUint16                   userDefinedHeaderLength,
                                                  CsrUint8                    **userDefinedHeader,
#endif
                                                  CsrUint8                    srmpValue);

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
#define OBEX_SRV_PUT_PARAM_CHALLENGE(_x) _x,
#else
#define OBEX_SRV_PUT_PARAM_CHALLENGE(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_SRV_PUT_PARAM_DESCRIPTION(_x) _x,
#else
#define OBEX_SRV_PUT_PARAM_DESCRIPTION(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
#define OBEX_SRV_PUT_PARAM_APP_PARAMS(_x) _x,
#else
#define OBEX_SRV_PUT_PARAM_APP_PARAMS(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_SRV_PUT_PARAM_USER_DEFINE(_x) _x,
#else
#define OBEX_SRV_PUT_PARAM_USER_DEFINE(_x) 
#endif

#define ObexUtilServPutWithHeaderResponse(_obexUtilInstData,\
                                          _responseCode,\
                                          _digestChallenge,\
                                          _headerPriorityTable,\
                                          _name, _description,\
                                          _appParametersHeaderLength, _appParametersHeader,\
                                          _userDefinedHeaderLength, _userDefinedHeader,\
                                          _srmpValue)\
      ObexUtilServPutWithHeaderResponseEx(_obexUtilInstData,\
                                          _responseCode,\
                                          _digestChallenge,\
                                          _headerPriorityTable,\
                                          _name,\
                                          OBEX_SRV_PUT_PARAM_DESCRIPTION(_description)\
                                          OBEX_SRV_PUT_PARAM_APP_PARAMS(_appParametersHeaderLength)\
                                          OBEX_SRV_PUT_PARAM_APP_PARAMS(_appParametersHeader)\
                                          OBEX_SRV_PUT_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                                          OBEX_SRV_PUT_PARAM_USER_DEFINE(_userDefinedHeader)\
                                          _srmpValue)
                                                  
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServPutResponse
 *
 *  DESCRIPTION
 *      This function will send a OBEX PUT response packet without any optionals 
 *      response headers. Note this response will always fit into a single OBEX
 *      Response Packet
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      responseCode            : The one byte responseCode. The list of valid 
 *                                response code values are defined in the IrOBEX 
 *                                specification  version 1.2 section 3.2 and 3.2.1 
 *
 *      srmpValue               : The Single Response Parameter (SRMP) can be used to
 *                                facilitate additional header exchange, beyond the 
 *                                first response packet exchange used to enable SRM.
 *                                It must be set to either:
 *
 *                                CSR_BT_OBEX_SRMP_WAIT. E.g. If another response 
 *                                packet is not expected by the peer Client, this 
 *                                parameter value will additionally inform the 
 *                                Client that an additional response shall occur 
 *                                from the local server. Please note that this value 
 *                                will only be valid if SRM is Enabled. If SRM is 
 *                                not enable the library will 
 *                                consider it as CSR_BT_OBEX_SRMP_INVALID
 *
 *                                CSR_BT_OBEX_SRMP_INVALID, means that this SRMP 
 *                                header will be ignore. E.g. it will not be 
 *                                included in OBEX Response Packet. 
 *----------------------------------------------------------------------------*/ 
extern CsrUint8 ObexUtilServPutResponse(void                  *obexUtilInstData,
                                        CsrBtObexResponseCode responseCode,
                                        CsrUint8              srmpValue);


/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServSetGetContinueFunc
 *
 *  DESCRIPTION
 *      This function will register a temporary callback function that must be 
 *      called every time the next OBEX GET Request packet is received. Note
 *      it only makes sense to register this callback function if it is a multi
 *      GET operation, as the callback function register with this function call
 *      is delete as soon as the GET operation is done and the callback function 
 *      registered ObexUtilServActivateStart will be use again
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      getContinueIndHandler   : The callback function that must be called when
 *                                the next OBEX GET Request packet is received.
 *                                Note the application must used the function
 *                                ObexUtilServGetResponse to accept or reject 
 *                                the OBEX GET operation. Please also note that
 *                                when the GET operation is done this callback 
 *                                function is delete and the callback function 
 *                                that was register in the ObexUtilServActivateStart
 *                                is used again. E.g. calling this functions 
 *                                gives only meaning if ObexUtilServGetResponse
 *                                or ObexUtilServGetWithHeadersResponse 
 *                                is called with the  
 *                                CSR_BT_OBEX_CONTINUE_RESPONSE_CODE response code  
 *----------------------------------------------------------------------------*/  
extern CsrUint8 ObexUtilServSetGetContinueFunc(void                        *obexUtilInstData,
                                               ObexUtilServGetFuncType     getContinueIndHandler);

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServGetResponse
 *
 *  DESCRIPTION
 *      This function will send a OBEX GET response packet without any optionals 
 *      response headers. Note that this response packet must fit in a single 
 *      OBEX packet. In order to ensure this,the server must never response with
 *      more data than specified by the obexResponsePacketLength parameter, which
 *      is defined in the ObexUtilServGetFuncType. I.e.the following calculation
 *      must be TRUE:
 *
 *      obexResponsePacketLength >= bodyLength
 *      
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      responseCode            : The one byte responseCode. The list of valid 
 *                                response code values are defined in the IrOBEX 
 *                                specification  version 1.2 section 3.2 and 3.2.1 
 *
 *      **body                  : A body chunk of an object. Must be a allocated 
 *                                pointer which will be own by this library. Note 
 *                                that if set to NULL a zero length body object is 
 *                                send
 *
 *      bodyLength              : The length of the body chunk
 *
 *      srmpValue               : The Single Response Parameter (SRMP) can be used to
 *                                facilitate additional header exchange, beyond the 
 *                                first response packet exchange used to enable SRM.
 *                                It must be set to either:
 *
 *                                CSR_BT_OBEX_SRMP_WAIT. E.g. If another response 
 *                                packet is not expected by the peer Client, this 
 *                                parameter value will additionally inform the 
 *                                Client that an additional response shall occur 
 *                                from the local server. Please note that this value 
 *                                will only be valid if SRM is Enabled. If SRM is 
 *                                not enable the library will 
 *                                consider it as CSR_BT_OBEX_SRMP_INVALID
 *
 *                                CSR_BT_OBEX_SRMP_INVALID, means that this SRMP 
 *                                header will be ignore. E.g. it will not be 
 *                                included in OBEX Response Packet. 
 *----------------------------------------------------------------------------*/ 
extern CsrUint8 ObexUtilServGetResponse(void                  *obexUtilInstData,
                                        CsrBtObexResponseCode responseCode,
                                        CsrUint8              **body,
                                        CsrUint16             bodyLength,
                                        CsrUint8              srmpValue);

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServGetWithHeaderResponse
 *
 *  DESCRIPTION
 *      This function will send a OBEX GET response packet and allow the profile
 *      to defines some optionals response headers. Note that this response packet 
 *      must fit in a single OBEX packet. In order to ensure this,the server must 
 *      never response with more data than specified by the obexResponsePacketLength 
 *      parameter, which is defined in the ObexUtilServGetFuncType. I.e.the following 
 *      calculation must be TRUE:
 *
 *      obexResponsePacketLength >= (CsrUcs2ByteStrLen(name) + userDefinedHeaderLength +
 *                                   appParametersHeaderLength + bodyLength
 *                                   CsrUcs2ByteStrLen(description) + digestChallenge->realmLength)
 *      
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      responseCode            : The one byte responseCode. The list of valid 
 *                                response code values are defined in the IrOBEX 
 *                                specification  version 1.2 section 3.2 and 3.2.1 
 *
 *      **digestChallenge       : Used to initiate the OBEX authentication of
 *                                the remote device. Note if set to NULL this
 *                                header will not be included in the OBEX
 *                                CONNECT Response packet. Note if the header
 *                                is set the responseCode MUST be set to 
 *                                CSR_BT_OBEX_UNAUTHORIZED_CODE and it must be set
 *                                in the first response Packet. If not this 
 *                                header will NOT be included
 *
 *      headerPriorityTable[6]  : The headerPriorityTable identifies the sequence
 *                                of how the Length, Name, Description,  
 *                                Application Parameters, user defined header(s) and
 *                                the body header must be sent. If the headers shall 
 *                                be send in the same order as just describe
 *                                the headerPriorityTable is set to
 *                                headerPriorityTable[0] = CSR_BT_OBEX_UTIL_LENGTH_HEADER
 *                                headerPriorityTable[1] = CSR_BT_OBEX_UTIL_NAME_HEADER
 *                                headerPriorityTable[2] = CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[3] = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER
 *                                headerPriorityTable[4] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER
 *                                headerPriorityTable[5] = CSR_BT_OBEX_UTIL_BODY_HEADER
 *                                Note that all entries in the headerPriorityTable must
 *                                be set. In the case where a header is not surposed to
 *                                be added to the OBEX packet the headerPriorityTable 
 *                                entry is set to OBEX_UTIL_UNDEFINE_HEADER
 *
 *      bodyHeaderPriorityIndex : The index in the headerPriorityTable where the body
 *                                header is located. Note in the case that the body
 *                                header is not located in the headerPriorityTable 
 *                                this parameter must be set to
 *                                CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX and the 
 *                                *body parameter must be set to NULL
 *
 *      lengthOfObject          : The total length in bytes of the object.
 *
 *      **name                  : A NULL terminated Unicode text string describing
 *                                the name. Must be a allocated pointer which will
 *                                be own by this library. Note if set to NULL this 
 *                                header will not be included in the OBEX Put 
 *                                response packet
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header 
 *                                will not be included in the OBEX Put response packet
 *
 *      **appParametersHeader   : The Application Parameters Header is used by the 
 *                                application (and protocols) layered above OBEX to
 *                                convey additional information in the OBEX PUT
 *                                Request packet. Note this header must be build 
 *                                correct after the OBEX spec (IrOBEX version 1.2 
 *                                section 2.2.12). This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL this header 
 *                                will not be included in the OBEX Put Request packet.
 *                                Note from this library point of view this header
 *                                is the same as the userDefinedHeader, see below
 *
 *      appParametersHeaderLength:The length of the Application parameters header  
 *
 *      **userDefinedHeader     : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX Put response packet
 *                                
 *      userDefinedHeaderLength : The length of the userDefinedHeader
 *
 *      **body                  : A body chunk of an object. Must be a allocated 
 *                                pointer which will be own by this library. Note 
 *                                that if set to NULL a zero length body object is 
 *                                only send if one of the values in the 
 *                                headerPriorityTable is set to 
 *                                CSR_BT_OBEX_UTIL_BODY_HEADER.
 *
 *      bodyLength              : The length of the body chunk
 *
 *      srmpValue               : The Single Response Parameter (SRMP) can be used to
 *                                facilitate additional header exchange, beyond the 
 *                                first response packet exchange used to enable SRM.
 *                                It must be set to either:
 *
 *                                CSR_BT_OBEX_SRMP_WAIT. E.g. If another response 
 *                                packet is not expected by the peer Client, this 
 *                                parameter value will additionally inform the 
 *                                Client that an additional response shall occur 
 *                                from the local server. Please note that this value 
 *                                will only be valid if SRM is Enabled. If SRM is 
 *                                not enable the library will 
 *                                consider it as CSR_BT_OBEX_SRMP_INVALID
 *
 *                                CSR_BT_OBEX_SRMP_INVALID, means that this SRMP 
 *                                header will be ignore. E.g. it will not be 
 *                                included in OBEX Response Packet. 
 *----------------------------------------------------------------------------*/        
#ifdef CSR_BT_INSTALL_OBEX_UTIL_SERV_PUT_GET_WITH_HEADER_EXT
extern CsrUint8 ObexUtilServGetWithHeaderResponseEx(void                      *obexUtilInstData,
                                                  CsrBtObexResponseCode       responseCode,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                                  ObexUtilDigestChallengeType **digestChallenge,
#endif
                                                  CsrUint8                    headerPriorityTable[6],
                                                  CsrUint8                    bodyHeaderPriorityIndex,  
                                                  CsrUint32                   lengthOfObject,
                                                  CsrUcs2String               **name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                                  CsrUcs2String               **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                                  CsrUint16                   appParametersHeaderLength,
                                                  CsrUint8                    **appParametersHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                                  CsrUint16                   userDefinedHeaderLength,  
                                                  CsrUint8                    **userDefinedHeader,
#endif
                                                  CsrUint8                    **body,
                                                  CsrUint16                   bodyLength,
                                                  CsrUint8                    srmpValue);

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
#define OBEX_SRV_GET_W_HDR_PARAM_CHALLENGE(_x) _x,
#else
#define OBEX_SRV_GET_W_HDR_PARAM_CHALLENGE(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_SRV_GET_W_HDR_PARAM_DESCRIPTION(_x) _x,
#else
#define OBEX_SRV_GET_W_HDR_PARAM_DESCRIPTION(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
#define OBEX_SRV_GET_W_HDR_PARAM_APP_PARAMS(_x) _x,
#else
#define OBEX_SRV_GET_W_HDR_PARAM_APP_PARAMS(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_SRV_GET_W_HDR_PARAM_USER_DEFINE(_x) _x,
#else
#define OBEX_SRV_GET_W_HDR_PARAM_USER_DEFINE(_x) 
#endif

#define ObexUtilServGetWithHeaderResponse(_obexUtilInstData, _responseCode,\
                                          _digestChallenge,\
                                          _headerPriorityTable,\
                                          _bodyHeaderPriorityIndex,\
                                          _lengthOfObject,\
                                          _name, _description,\
                                          _appParametersHeaderLength, _appParametersHeader,\
                                          _userDefinedHeaderLength, _userDefinedHeader,\
                                          _body, _bodyLength,\
                                          _srmpValue)\
      ObexUtilServGetWithHeaderResponseEx(_obexUtilInstData,\
                                          _responseCode,\
                                          OBEX_SRV_GET_W_HDR_PARAM_CHALLENGE(_digestChallenge)\
                                          _headerPriorityTable,\
                                          _bodyHeaderPriorityIndex,\
                                          _lengthOfObject,\
                                          _name,\
                                          OBEX_SRV_GET_W_HDR_PARAM_DESCRIPTION(_description)\
                                          OBEX_SRV_GET_W_HDR_PARAM_APP_PARAMS(_appParametersHeaderLength)\
                                          OBEX_SRV_GET_W_HDR_PARAM_APP_PARAMS(_appParametersHeader)\
                                          OBEX_SRV_GET_W_HDR_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                                          OBEX_SRV_GET_W_HDR_PARAM_USER_DEFINE(_userDefinedHeader)\
                                          _body,\
                                          _bodyLength,\
                                          _srmpValue)
                                                  
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServSetpathResponse
 *
 *  DESCRIPTION
 *      This function will send an OBEX SETPATH response packet. Note that this
 *      response packet must fit in a single OBEX packet. In order to ensure 
 *      this, the server must never response with more data than specified by
 *      the obexResponsePacketLength parameter, which is defined in the  
 *      ObexUtilServSetpathFuncType. I.e.the following calculation must be TRUE:
 *
 *      obexResponsePacketLength >= (CsrUcs2ByteStrLen(name) + userDefinedHeaderLength + 
 *                                   CsrUcs2ByteStrLen(description) + digestChallenge->realmLength)
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      responseCode            : The one byte responseCode. The list of valid 
 *                                response code values are defined in the IrOBEX 
 *                                specification  version 1.2 section 3.2 and 3.2.1 
 *
 *      **digestChallenge       : Used to initiate the OBEX authentication of
 *                                the remote device. Note if set to NULL this
 *                                header will not be included in the OBEX
 *                                Setpath Response packet. Note if the header
 *                                is set the responseCode MUST be set to 
 *                                CSR_BT_OBEX_UNAUTHORIZED_CODE. If not this 
 *                                header will NOT be included
 *
 *      headerPriorityTable[3]  : The headerPriorityTable identifies the sequence
 *                                of how the name header, description header and 
 *                                user defined header(s) must be sent. If the name header 
 *                                must be sent first followed by the description header 
 *                                and the user defined header(s) as the last one the 
 *                                headerPriorityTable is set to 
 *                                headerPriorityTable[0] = OBEX_UTIL_NAME_HEADER
 *                                headerPriorityTable[1] = OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[2] = OBEX_UTIL_USER_DEFINED_HEADER
 *                                Note that all entries in the headerPriorityTable 
 *                                must be set. In the case where a header is not surposed
 *                                to be added to the OBEX packet the headerPriorityTable 
 *                                entry is set to OBEX_UTIL_UNDEFINE_HEADER. E.g. if only
 *                                the name header must be included in the OBEX
 *                                packet then the headerPriorityTable must be set to
 *                                headerPriorityTable[0] = OBEX_UTIL_NAME_HEADER
 *                                headerPriorityTable[1] = OBEX_UTIL_UNDEFINE_HEADER
 *                                headerPriorityTable[2] = OBEX_UTIL_UNDEFINE_HEADER
 *                                and the *userDefinedHeader and description pointers 
 *                                must be NULL. Note in the special case where an
 *                                empty name header (defined as a Name header of length 3
 *                                one byte opcode + two byte length) must be sent, the *name 
 *                                pointer must be set to NULL and the headerPriorityTable[x] 
 *                                must be set to OBEX_UTIL_EMPTY_NAME_HEADER
 *
 *      **name                  : A NULL terminated Unicode text string . Must be a 
 *                                allocated pointer which will be own by this library. 
 *                                Note if set to NULL this header will not be included 
 *                                in the OBEX SETPATH Response packet
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header will not be 
 *                                included in the OBEX SETPATH Response packet
 *
 *      **userDefinedHeader     : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX SETPATH Response packet
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_SRV_SET_PATH
extern CsrUint8 ObexUtilServSetpathResponseEx(void                        *obexUtilInstData,
                                            CsrBtObexResponseCode       responseCode,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                            ObexUtilDigestChallengeType **digestChallenge,
#endif
                                            CsrUint8                    headerPriorityTable[3],
                                            CsrUcs2String               **name   
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                            ,CsrUcs2String               **description
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                            ,CsrUint16                   userDefinedHeaderLength
                                            ,CsrUint8                    **userDefinedHeader
#endif                                            
                                            );

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
#define OBEX_SRV_SET_PATH_PARAM_CHALLENGE(_x) _x,
#else
#define OBEX_SRV_SET_PATH_PARAM_CHALLENGE(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_SRV_SET_PATH_PARAM_DESCRIPTION(_x) ,_x
#else
#define OBEX_SRV_SET_PATH_PARAM_DESCRIPTION(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_SRV_SET_PATH_PARAM_USER_DEFINE(_x) ,_x
#else
#define OBEX_SRV_SET_PATH_PARAM_USER_DEFINE(_x) 
#endif

#define ObexUtilServSetpathResponse(_obexUtilInstData,\
                                    _responseCode,\
                                    _digestChallenge,\
                                    _headerPriorityTable,\
                                    _name,\
                                    _description,\
                                    _userDefinedHeaderLength,\
                                    _userDefinedHeader)\
      ObexUtilServSetpathResponseEx(_obexUtilInstData,\
                                    _responseCode,\
                                    OBEX_SRV_SET_PATH_PARAM_CHALLENGE(_digestChallenge)\
                                    _headerPriorityTable,\
                                    _name\
                                    OBEX_SRV_SET_PATH_PARAM_DESCRIPTION(_description)\
                                    OBEX_SRV_SET_PATH_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                                    OBEX_SRV_SET_PATH_PARAM_USER_DEFINE(_userDefinedHeader))
                                            


#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServAuthenticateResponse
 *
 *  DESCRIPTION
 *      This function is used
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      passwordLength          : The length of the password. 
 *
 *      **password              : The password is the secret known by both server 
 *                                and client. An MD5  hashing function is applied 
 *                                to the password string to produce a 128 bit 
 *                                string. This resulting string is the request-digest 
 *                                in the OBEX Digest Response header. Must be an 
 *                                allocated pointer which will be owned by this 
 *                                library. This pointer must not be NULL  
 *
 *      userIdLength            : The length of the user id. Maximum size is 
 *                                20 bytes 
 *
 *      **userId                : A userId is required if bit 0 is set in 'options' 
 *                                parameter in the ObexUtilAuthenticateIndFuncType 
 *                                structure. The userId is used by the peer device
 *                                to identify the proper password. Must be an 
 *                                allocated pointer which will be own by this 
 *                                library. Note if set to NULL it will not be 
 *                                included in the OBEX Digest Response header 
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
extern CsrUint8 ObexUtilServAuthenticateResponse(void               *obexUtilInstData,
                                                 CsrUint16          passwordLength,
                                                 CsrUint8           **password,
                                                 CsrCharString      **userId);
#endif

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServActionResponse
 *
 *  DESCRIPTION
 *      This function will send an OBEX ACTION response packet. Note that this
 *      response packet must fit in a single OBEX packet. In order to ensure 
 *      this, the server must never response with more data than specified by
 *      the obexResponsePacketLength parameter, which is defined in the  
 *      ObexUtilServActionFuncType. I.e.the following calculation must be TRUE:
 *
 *      obexResponsePacketLength >= (userDefinedHeaderLength + 
 *                                   CsrUcs2ByteStrLen(description) + digestChallenge->realmLength)
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      responseCode            : The one byte responseCode. The list of valid 
 *                                response code values are defined in the IrOBEX 
 *                                specification  version 1.2 section 3.2 and 3.2.1 
 *
 *      **digestChallenge       : Used to initiate the OBEX authentication of
 *                                the remote device. Note if set to NULL this
 *                                header will not be included in the OBEX
 *                                Action Response packet. Note if the header
 *                                is set the responseCode MUST be set to 
 *                                CSR_BT_OBEX_UNAUTHORIZED_CODE. If not this 
 *                                header will NOT be included
 *
 *      headerPriorityTable[2]  : The headerPriorityTable identifies the sequence
 *                                of how the description header and user defined header(s) 
 *                                must be sent. If the description header must be sent 
 *                                first followed by the user defined header(s) the 
 *                                headerPriorityTable is set to 
 *                                headerPriorityTable[0] = OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[1] = OBEX_UTIL_USER_DEFINED_HEADER
 *                                Note that all entries in the headerPriorityTable 
 *                                must be set. In the case where a header is not surposed
 *                                to be added to the OBEX packet the headerPriorityTable 
 *                                entry is set to OBEX_UTIL_UNDEFINE_HEADER. E.g. if only
 *                                the namdescription header must be included in the OBEX
 *                                packet then the headerPriorityTable must be set to
 *                                headerPriorityTable[0] = OBEX_UTIL_DESCRIPTION_HEADER
 *                                headerPriorityTable[1] = OBEX_UTIL_UNDEFINE_HEADER
 *                                and the *userDefinedHeader must be NULL. 
 *
 *      **description           : A NULL terminated Unicode text string used to 
 *                                provide additional description of the object 
 *                                or the operation. Must be a allocated 
 *                                pointer which will be own by this library. 
 *                                Note if set to NULL this header will not be 
 *                                included in the OBEX ACTION Response packet
 *
 *      **userDefinedHeader     : User defined header(s) which allow complete 
 *                                flexibility. Note these headers must be build 
 *                                correct after OBEX spec. This library don't make any
 *                                checks. Must be a allocated pointer which will be own
 *                                by this library. Note if set to NULL these headers 
 *                                will not be included in the OBEX ACTION Response packet
 *----------------------------------------------------------------------------*/
extern CsrUint8 ObexUtilServActionResponseEx(void                        *obexUtilInstData,
                                           CsrBtObexResponseCode       responseCode,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                           ObexUtilDigestChallengeType **digestChallenge,
#endif
                                           CsrUint8                    headerPriorityTable[2]
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                          ,CsrUcs2String               **description
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                          ,CsrUint16                   userDefinedHeaderLength
                                          ,CsrUint8                    **userDefinedHeader
#endif
                                            );

/* Macroes and defines used to adjust the number of parameters to the function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
#define OBEX_SRV_ACTION_PARAM_CHALLENGE(_x) _x,
#else
#define OBEX_SRV_ACTION_PARAM_CHALLENGE(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_SRV_ACTION_PARAM_DESCRIPTION(_x) ,_x
#else
#define OBEX_SRV_ACTION_PARAM_DESCRIPTION(_x) 
#endif
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_SRV_ACTION_PARAM_USER_DEFINE(_x) ,_x
#else
#define OBEX_SRV_ACTION_PARAM_USER_DEFINE(_x) 
#endif

#define ObexUtilServActionResponse(_obexUtilInstData,\
                                   _responseCode,\
                                   _digestChallenge,\
                                   _headerPriorityTable,\
                                   _description,\
                                   _userDefinedHeaderLength,\
                                   _userDefinedHeader)\
        ObexUtilServActionResponseEx(_obexUtilInstData,\
                                     _responseCode,\
                                     OBEX_SRV_ACTION_PARAM_CHALLENGE(_digestChallenge)\
                                     _headerPriorityTable\
                                     OBEX_SRV_ACTION_PARAM_DESCRIPTION(_description)\
                                     OBEX_SRV_ACTION_PARAM_USER_DEFINE(_userDefinedHeaderLength)\
                                     OBEX_SRV_ACTION_PARAM_USER_DEFINE(_userDefinedHeader))\
   
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      ObexUtilServChangeIncomingSecurity
 *
 *  DESCRIPTION
 *      This function is used is to change the security level for new incoming 
 *      connection after the server has been activated.
 *
 *    PARAMETERS
 *      *obexUtilInstData       : The overloaded void pointer to this 
 *                                library private instance data
 *
 *      secIncoming             : Change the security level for new incoming 
 *                                connection
 *----------------------------------------------------------------------------*/
extern void ObexUtilServChangeIncomingSecurity(void                 *obexUtilInstData,
                                               dm_security_level_t  secIncoming); 

/*------------------------------------------------------------------------------*
 * Public Obex Util functions                                                   *
 *------------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
ObexUtilDigestChallengeType * ObexUtilReturnDigestChallengePointer(CsrBool       authtorize, 
                                                                   CsrUint8      **password, 
                                                                   CsrUint16     passwordLength, 
                                                                   CsrCharString **userId, 
                                                                   CsrUint8      **realm, 
                                                                   CsrUint16     realmLength);

#endif

#define ObexUtilReturnSrmpValue(srmpOn) ((CsrUint8) ((srmpOn) ? CSR_BT_OBEX_SRMP_WAIT : CSR_BT_OBEX_SRMP_INVALID))

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
extern void ObexUtilPfreeDigestChallengePointer(ObexUtilDigestChallengeType ** chal);
#endif

#ifdef CSR_BT_INSTALL_OBEX_CONTEXT_ID
extern CsrBool ObexUtilGetInstIdentifierFromCmUpstreamMessage(void * msg, CsrUint8 *id);
#endif
#ifdef __cplusplus
}
#endif

#endif /*OBEX_UTIL_H__*/

