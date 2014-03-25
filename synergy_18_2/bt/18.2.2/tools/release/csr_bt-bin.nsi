###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision$
###############################################################################

!include "MUI.nsh"

SetCompressor /SOLID lzma

Name "BCHS Binary release Version ${VERSION}"

OutFile "..\..\setup.exe"

InstallDir $PROGRAMFILES\BCHS-bin\${VERSION}

!define MUI_WELCOMEFINISHPAGE_BITMAP ".\csr.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP_NOSTRETCH

!insertmacro MUI_PAGE_WELCOME 
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_SHOWREADME $INSTDIR\readme.txt
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"


;--------------------------------
; The stuff to install
;--------------------------------
Section "" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; Put file there
  File  /r ..\..\stage\bchs_bin\${VERSION}\*

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd ; end the section


;--------------------------------
;Uninstaller Section
;--------------------------------
Section "Uninstall"
  RMDir /r "$INSTDIR"
SectionEnd
