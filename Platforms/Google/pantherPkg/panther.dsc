##
#  panther (Google Pixel 7, Samsung gs201 / Tensor G2) platform DSC.
#
#  Built by the UniXNU driver: `./unixnu build firmware --uefi ../uefi-panther`.
#  Hardware constants are evidence-tagged (DTB / HW / TBD) in
#  pantherPkg/README.md and UniXNU-panther/docs/GS201-BOOT-DEVIATIONS.md.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  PLATFORM_NAME                  = panther
  PLATFORM_GUID                  = dc29f4b5-b2cb-4213-9c88-2a47e8ef0707
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/pantherPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = pantherPkg/panther.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

!include S5E9825Pkg/S5E9825Pkg.dsc.inc

[PcdsFixedAtBuild]
  #
  # DDR memory - first bank starts at 0x80000000 (DTB).
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI stack - matches the "UEFI Stack" region of this platform's
  # MemoryMapLib (0x9F000000, 0x40000). TBD against the ABL's live windows
  # at first light.
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0x9F000000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # Simple frame buffer - the ABL splash the firmware inherits (HW):
  # 0xfac00000, XRGB8888, 1080x2400, stride 4320.
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2400
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|pantherPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # Input Libraries
  #
  KeypadDeviceLib|pantherPkg/Library/KeypadDeviceLib/KeypadDeviceLib.inf

[Components]
  #
  # Input (listed by the platform's APRIORI/DXE includes).
  #
  SiliciumPkg/Drivers/KeypadDxe/KeypadDxe.inf
  SiliciumPkg/Drivers/KeypadDeviceDxe/KeypadDeviceDxe.inf

  #
  # UniXNU boot manager and its firmware hooks.
  #
  UniXNUPkg/Application/UniXNU/UniXNU.inf
  UniXNUPkg/Driver/UniXNUDxe/UniXNUDxe.inf
  UniXNUPkg/Driver/UniXNURamDiskDxe/UniXNURamDiskDxe.inf
  DarwinBootPkg/Driver/DarwinBoot.inf

  #
  # Payload filesystem chain (FAT volume on the UFS payload partition).
  #
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

[LibraryClasses]
  UniXNULib|UniXNUPkg/Library/UniXNULib/UniXNULib.inf
  FdtLib|MdePkg/Library/BaseFdtLib/BaseFdtLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
