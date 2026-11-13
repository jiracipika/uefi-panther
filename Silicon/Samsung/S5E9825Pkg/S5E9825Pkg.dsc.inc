##
#  Samsung gs201 (Google Tensor G2, S5E9825) silicon include.
#
#  Library classes default to the SamsungPkg null implementations; only
#  silicon-observed overrides live here. Hardware constants are evidence-
#  tagged in S5E9825Pkg/README.md (DTB / HW / TBD).
#
#  This include does NOT chain SamsungPkg/SamsungPkg.dsc.inc: that base
#  builds SamsungPkg/Drivers/MshcDxe, whose DmaMap calls carry two
#  alignment arguments the current EmbeddedPkg DmaLib prototype does not
#  take (upstream drift, compile error 203/527 of MshcHost.c), and pulls
#  storage drivers a gs201 platform cannot drive through the null libraries
#  anyway. The wanted Samsung pieces are repeated here.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  #
  # Timer
  #
  USE_PHYSICAL_TIMER                         = 0

  #
  # KEP
  #
  HAS_CNTFRQ_EL0_EMPTY_ERRATA                = 1

!include SiliciumPkg/SiliciumPkg.dsc.inc

[BuildOptions]
  *_CLANGPDB_AARCH64_CC_FLAGS = -D HAS_CNTFRQ_EL0_EMPTY_ERRATA=$(HAS_CNTFRQ_EL0_EMPTY_ERRATA)

[PcdsFixedAtBuild]
  #
  # ACPI
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemId|"GOOGLE"
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x3130323545355335
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x201

  #
  # CPU - 3 clusters: 4x Cortex-A55, 2x Cortex-A78, 2x Cortex-X1
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3

  #
  # GICv3 - DTB: GICD 0x5880000/0x10000, GICR 0x5900000/0x200000
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x05880000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x05900000

  #
  # Timer - TBD: 24.576 MHz assumed from the gs101 family; verify CNTFRQ_EL0
  # at first light before trusting any timer-derived timeout.
  #
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|24576000

  #
  # Monitor conduit - the factory DTB's PSCI node carries method "smc" and
  # the ABL enters the payload at EL2 (DTB + HW).
  #
  gArmTokenSpaceGuid.PcdMonitorConduitHvc|FALSE

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Google"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Pixel 7"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"panther"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Pixel_7_panther"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"Pixel 7"

  #
  # Simple Frame Buffer - the ABL splash: 0xfac00000, XRGB8888, stride 4320
  # (HW; the address is consumed by the platform's display bring-up, not here)
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2400
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

[LibraryClasses]
  #
  # Memory Libraries
  #
  RamManagerLib|SamsungPkg/Library/RamManagerLib/RamManagerLib.inf

  #
  # IO Libraries - Samsung null implementations until real gs201 code lands
  # (see S5E9825Pkg/README.md for the evidence-tagged register map).
  #
  GpioBankLib|SamsungNullPkg/Library/GpioBankNullLib/GpioBankNullLib.inf
  HsI2cLib|SamsungNullPkg/Library/HsI2cNullLib/HsI2cNullLib.inf

  #
  # Power Management Libraries
  #
  PmicLib|SamsungNullPkg/Library/PmicNullLib/PmicNullLib.inf

  #
  # Storage Libraries - gs201 storage is UFS (0x3c400000, DTB); both stay
  # null until the Samsung UfsCal adapter is adapted to gs201.
  #
  PlatformUfsLib|SamsungNullPkg/Library/PlatformUfsLibNull/PlatformUfsLibNull.inf
  PlatformMshcLib|SamsungNullPkg/Library/PlatformMshcLibNull/PlatformMshcLibNull.inf
  UfsCalAdapterLib|SamsungPkg/Library/UfsCalAdapterLib/UfsCalAdapterLib.inf

  #
  # USB Libraries
  #
  PlatformUsbfnLib|SamsungNullPkg/Library/PlatformUsbfnLibNull/PlatformUsbfnLibNull.inf

  #
  # Samsung Libraries
  #
  UsiLib|SamsungNullPkg/Library/UsiNullLib/UsiNullLib.inf

  #
  # BDS Libraries
  #
  ErrataPatchesLib|SamsungPkg/Library/ErrataPatchesLib/ErrataPatchesLib.inf

  #
  # Reset - PSCI SYSTEM_RESET silently no-ops in the Pixel ABL context (HW);
  # use the SMC fast-reset the HakiOS campaign verified end to end.
  #
  ResetSystemLib|S5E9825Pkg/Library/ResetSystemLib/ResetSystemLib.inf

[Components]
  #
  # IO Drivers (null-library backed until the gs201 register maps land)
  #
  SamsungPkg/Drivers/GpioDxe/GpioDxe.inf
  SamsungPkg/Drivers/SpeedyDxe/SpeedyDxe.inf
  SamsungPkg/Drivers/HsI2cDxe/HsI2cDxe.inf

  #
  # Power Management Drivers
  #
  SamsungPkg/Drivers/PmicDxe/PmicDxe.inf

  #
  # RNG Drivers
  #
  SamsungPkg/Drivers/RngDxe/RngDxe.inf

  #
  # Samsung Drivers
  #
  SamsungPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  SamsungPkg/Drivers/UsiDxe/UsiDxe.inf
