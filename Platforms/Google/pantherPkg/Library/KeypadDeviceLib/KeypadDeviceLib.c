/**
  panther keypad skeleton: the protocol without keys.

  gs201's power and volume keys reach the system through the Google PMIC
  ADC, not a Samsung GPIO bank; the mapping is unmeasured. Until it is,
  this reports no keys - the boot menu remains usable through the serial
  text console only.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>

#include <Library/KeypadDeviceLib.h>
#include <Library/DebugLib.h>

/**
  Resets the keypad hardware.

  @param[in] This        Protocol instance pointer.

  @retval EFI_SUCCESS    Nothing to reset; always succeeds.
**/
EFI_STATUS
KeypadDeviceReset (
  IN KEYPAD_DEVICE_PROTOCOL  *This
  )
{
  DEBUG ((DEBUG_INFO, "%a: no keypad hardware mapped yet\n", __func__));
  return EFI_SUCCESS;
}

/**
  Polls the keypad for key events.

  @param[in]  This             Protocol instance pointer.
  @param[in]  KeypadReturnApi  The API used to report key events.
  @param[in]  Delta            Time since the last poll, in microseconds.

  @retval EFI_SUCCESS          Polled; no keys exist to report yet.
**/
EFI_STATUS
KeypadDeviceGetKeys (
  IN KEYPAD_DEVICE_PROTOCOL  *This,
  IN KEYPAD_RETURN_API       *KeypadReturnApi,
  IN UINT64                   Delta
  )
{
  //
  // TBD: PMIC ADC key mapping. No keys are reported until it is measured.
  //
  return EFI_SUCCESS;
}
