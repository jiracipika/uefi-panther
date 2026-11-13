/**
  panther (Pixel 7, gs201) memory map.

  Evidence tags, see the INF header:
    DTB - decoded from the factory device tree (panther-lga-base{,-2}.dtb).
    HW  - silicon-observed during the HakiOS campaign.
    TBD - placeholder, verify before relying on it.

  DDR banks (DTB, UniXNU-panther/dts/panther.dts; sizes unverified on HW):
    0x80000000 + 0x80000000, 0x880000000 + 0x80000000,
    0x900000000 + 0x80000000, 0x980000000 + 0x80000000.
  Mu's PrePi discovers usable DDR through this table, so only the first
  bank is described here and only its safe window: the top of the bank
  carries ABL state the firmware must not disturb (HW).

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/MemoryMapLib.h>

STATIC
EFI_MEMORY_REGION_DESCRIPTOR
gMemoryDescriptor[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  // DDR regions (first bank, [0x80000000, 0x100000000))
  //
  // "UEFI FD" is the address the boot shim copies the firmware image to
  // (Resources/Configs/panther.toml uefi_fd). The ABL decompresses the boot
  // image's kernel field at its own fixed address somewhere below this
  // window; the shim relocates the FD before the first byte of it executes
  // from here. TBD: confirm the ABL load address and its scratch windows on
  // hardware before trusting that nothing overlaps.
  { "UEFI FD",           0x90000000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK },
  { "DXE Heap",          0x90400000, 0x08000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK },
  { "UEFI Stack",        0x9F000000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK },

  // Reserved regions of the first bank the firmware must not touch.
  //  - Splash framebuffer: XRGB8888 1080x2400, stride 4320, DECON0/DPP0
  //    still scan it out with TRIG_CON 0x3070 (HW). Kept alive as the boot
  //    console until the display driver takes the panel over.
  { "Splash Framebuffer", 0xFAC00000, 0x01194000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH },
  //  - pstore/ramoops rings the Android side recovers logs from (DTB:
  //    ramoops@fd3ff000 and @fd5ff000; bl_log@fd800000; ABL bl_old_log at
  //    0xfd880000, HW).
  { "ABL Log Region",     0xFD3FF000, 0x00641000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK },

  // Register regions (DTB unless noted)
  { "PMU Syscon",        0x18060000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "ACPM",              0x18210000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "ACPM SRAM",         0x18500000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "DPP0",              0x1C0B0000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "DECON0",            0x1C240000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  //  DSIM0 raises an external abort when touched while its clock is gated
  //  (HW, twice reproduced). Mapped for the display bring-up path only;
  //  clock bring-up must precede any DSIM access.
  { "DSIM0",             0x1C2C0000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "UFS HCI",           0x3C400000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "UFS TOP",           0x3C410000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "UFS PHY SRAM",      0x3C700000, 0x00014000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "Mali CSF",          0x28000000, 0x00480000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  //  Watchdogs: cluster watchdogs the HakiOS reset recipe uses (HW). Key
  //  unlock 0xc0ffee at base+0x04.
  { "Watchdog0",         0x200C4000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "Watchdog1",         0x200C5000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },

  // GICv3 (DTB): distributor 0x5880000/0x10000, redistributors
  // 0x5900000/0x200000.
  { "GICD",              0x05880000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "GICR",              0x05900000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
};

VOID
GetMemoryMap (
  OUT EFI_MEMORY_REGION_DESCRIPTOR  **MemoryDescriptor,
  OUT UINT8                          *MemoryDescriptorCount
  )
{
  *MemoryDescriptor      = gMemoryDescriptor;
  *MemoryDescriptorCount = ARRAY_SIZE (gMemoryDescriptor);
}
