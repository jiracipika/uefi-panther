/**
  gs201 reset through the SMC the Pixel 7 ABL context accepts.

  PSCI SYSTEM_RESET (0x84000009) silently no-ops after the Android
  bootloader hands the core over; SMC 0x82000504 resets deterministically
  (HW, HakiOS campaign 2026-11, observed-reliability order of section 4 in
  UniXNU-panther/docs/GS201-BOOT-DEVIATIONS.md). Cold and warm reset are the
  same call here: the ABL context offers no deeper control, and a recovery
  preset would land the device in its recovery partition.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/ArmMonitorLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#include <Library/ResetSystemLib.h>

/**
  The gs201 fast reset SMC (HW-verified).

  ABL accepts this call from the payload context; the return value is the
  monitor's status with 0 = success. A second call is harmless: the first
  one that lands resets the core.
**/
#define GS201_FAST_RESET_SMC  0x82000504

STATIC
VOID
Gs201FastReset (
  VOID
  )
{
  ARM_MONITOR_ARGS  Args;

  ZeroMem (&Args, sizeof (Args));
  Args.Arg0 = GS201_FAST_RESET_SMC;
  ArmMonitorCall (&Args);

  //
  // The call above does not return on success. If it did, the monitor
  // refused the reset: say so, then keep the CPU parked in an endless
  // loop rather than pretending the reset happened.
  //
  DEBUG ((DEBUG_ERROR, "%a: SMC %#lx returned %lx\n", __func__, (UINT64)GS201_FAST_RESET_SMC, (UINT64)Args.Arg0));
  while (TRUE) {
    CpuDeadLoop ();
  }
}

/**
  Resets the entire platform.

  @param[in] ResetType      The type of reset to perform.
  @param[in] ResetStatus    The status code for the reset.
  @param[in] DataSize       The size, in bytes, of ResetData.
  @param[in] ResetData      For a ColdReset, WarmReset or
                            ShutdownReset, the data platform passed.

**/
VOID EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  )
{
  DEBUG ((DEBUG_INFO, "%a: type %d\n", __func__, (INT32)ResetType));
  Gs201FastReset ();
}

/**
  Causes the system to enter an execution state at the point where the S3
  resume record was stored.

**/
VOID EFIAPI
EnterS3WithImmediateWake (
  VOID
  )
{
  //
  // gs201 in this context has no S3 record: a reset is the only exit.
  //
  Gs201FastReset ();
}
