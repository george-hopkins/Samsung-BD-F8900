/// <!-- -*- c++ -*- ////////////////////////////////////////////////////// -->
///
/// System   Guggenheim
/// Module   Maple DSMCC Protocol
///
/// @file    DSMCCProtocol.cpp
/// @author  Adam Labuda <a.labuda@samsung.com>
/// @date    04 Mar 2010
///
/// @brief   Class interfacing Maple with DSM protocol handler
///
///////////////////////////////////////////////////////////////////////////////
///
/// Copyrighlst (c) 2010 Samsung Electronics Polska Sp. z o.o.
///
/// All Rights Reserved.
///
/// This  document  may  not, in  whole  or in  part, be  copied,  photocopied,
/// reproduced,  translated,  or  reduced to any  electronic  medium or machine
/// readable form without prior written consent from Samsung Electronics Polska
/// Sp. z o.o.
///
/// Samsung Poland Reasearch Center, DTV Lab, PL.
///
///////////////////////////////////////////////////////////////////////////////

#include "DSMCCProtocolInterface.h"

CDSMCCProtocolInterface* DSMCCProtocol_GetInstance(void)
{
    return NULL;
}

const INSTANCE_POINTER CDSMCCProtocolInterface::GetInstance = NULL;


