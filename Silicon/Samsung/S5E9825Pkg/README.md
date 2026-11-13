# S5E9825Pkg — Samsung gs201 (Google Tensor G2) silicon package

Samsung gs201, the SoC of the Google Pixel 7 (panther) and Pixel 7 Pro
(cheetah/panther-pro-folks). Samsung's part number S5E9825 is **inferred**
from Samsung's part numbering; the factory DTB identifies the SoC only as
`google,gs201` (evidence tag: DTB, `UniXNU-gs201-common/dts/gs201.dtsi`).

The package is a skeleton: library classes default to the `SamsungPkg.dsc.inc`
null implementations and only what is silicon-observed is overridden. Every
hardware constant below is evidence-tagged:

- **DTB** — decoded from the factory device tree
  (`jiracipika/hakios-gs201/dtb/panther-lga-base{,-2}.dtb`).
- **HW** — observed on the silicon during the HakiOS campaign (2026-11).
- **TBD** — placeholder; verify before relying on it.

## What is overridden here

| Library | Implementation | Evidence |
| --- | --- | --- |
| `ResetSystemLib` | SMC 0x82000504, watchdog recipe as fallback | HW (SMC verified; PSCI SYSTEM_RESET silently no-ops in the ABL context) |

## Platform constants (gs201, DTB unless noted)

| Item | Value | Evidence |
| --- | --- | --- |
| CPU clusters | 3: 4x Cortex-A55 (cpu0-3), 2x Cortex-A78 (cpu4-5), 2x Cortex-X1 (cpu6-7) | DTB + HW |
| GICv3 distributor | 0x5880000, size 0x10000 | DTB |
| GICv3 redistributors | 0x5900000, size 0x200000 | DTB |
| Architectural timer | 24.576 MHz | **TBD** (assumed from gs101 family; verify CNTFRQ_EL0 at first light) |
| PMU syscon | 0x18060000 | DTB |
| Watchdogs | 0x200c4000, 0x200c5000 (key unlock 0xc0ffee at base+0x04) | HW |
| UFS host controller | 0x3c400000 | DTB |
| Mali G710 CSF interface | 0x28000000 | DTB |
| ACPM | 0x18210000 + SRAM 0x18500000 | DTB |
| DECON0 / DPP0 / DSIM0 | 0x1c240000 / 0x1c0b0000 / 0x1c2c0000 | DTB |

**DSIM warning (HW):** touching DSIM0 while its clock is gated raises an
external abort, twice reproduced regardless of MMU state. Display code must
bring the DSIM clock up before any DSIM register access.

## Entry context on this board (HW, Pixel ABL)

The Android bootloader (ABL) enters the payload at its PE32+ entry point with
x0 = x1 = 0, MMU on (identity-limited mappings), EL2, and CNTPCT/CNTFRQ
trapped. See `UniXNU-panther/docs/GS201-BOOT-DEVIATIONS.md` for the full
measured contract; the `Platforms/Google/pantherPkg` platform and the boot
image packing carry that responsibility, not this package.
