# pantherPkg — Google Pixel 7 (Samsung gs201 / Tensor G2)

Mu-Silicium platform for the Pixel 7. Built through the UniXNU driver:

```sh
./unixnu build firmware --uefi ../uefi-panther
```

with `board.toml` in `UniXNU-panther` pointing at
`Platforms/Google/pantherPkg/panther.dsc` and
`Build/pantherPkg/{target}_CLANGPDB/FV/SILICIUM_UEFI.fd`.

## Evidence tags

Every hardware value is tagged: **DTB** (decoded from the factory device
tree `panther-lga-base{,-2}.dtb`), **HW** (silicon-observed during the
HakiOS campaign, 2026-11), **TBD** (placeholder). The full record lives in
`UniXNU-panther/porting.toml`; the boot contract in
`UniXNU-panther/docs/GS201-BOOT-DEVIATIONS.md`.

## Board state

| Area | Value | Tag |
| --- | --- | --- |
| DDR first bank | 0x80000000 + 0x80000000 (3 more banks above) | DTB |
| UEFI FD / heap / stack | 0x90000000 / 0x90400000 / 0x9F000000 | TBD |
| Splash framebuffer | 0xfac00000, XRGB8888, 1080x2400, stride 4320, DECON0+DPP0 still scanning it | HW |
| GICv3 | GICD 0x5880000, GICR 0x5900000 | DTB |
| CPUs | 8 cores, 3 clusters (4xA55, 2xA78, 2xX1), PSCI smc | DTB |
| Reset | SMC 0x82000504 (S5E9825Pkg ResetSystemLib) | HW |
| Timer | 24.576 MHz | TBD |
| Keys | PMIC ADC path, unmeasured (KeypadDeviceLib stub) | TBD |

## Board deviations the generic flow does not cover

1. **The ABL enters the PE32+ entry point**, not an ARM64 image header, with
   x0 = x1 = 0, MMU on, EL2, timer reads trapped. `SiliciumPkg/Sec/Sec.inf`
   expects its platform handoff from a boot shim; on this board that shim is
   the PE32+ vehicle in the boot image. Building that shim (BootShim-PE) is
   the open Stage-2 work item — until it exists, the FD alone cannot boot.
2. **No initrd, no FDT handoff** (measured): `UniXNURamDiskDxe`'s HOB has no
   first-boot input here. Payload discovery is by UFS partition (dual-system
   procedure) once storage works, or an embedded test payload.
3. **The boot image must be the full 64 MiB AVB-partition container**
   (v4 header + lz4-legacy stream + stock vbmeta + AVBf footer). The
   generic `boot_image` packer does not produce this shape; the firmware
   preflight in `UniXNU-panther` carries it (see GS201-BOOT-DEVIATIONS.md
   section 2).
4. **DSIM aborts on gated-clock access** (HW, twice reproduced). Display
   code must bring the DSIM clock up before touching DSIM0 (0x1c2c0000).

## Pending (not blocking the build)

- BootShim-PE (PE32+ vehicle assembly, MMU handoff to Mu SEC, FDT install).
- AdvancedLogger + `AdvancedLogDxe` + Tools menu (Stage 4 of
  docs/UNIXNU-BRINGUP-PLAN.md; observability without serial depends on it).
- Real display driver (DECON0/DPP0 scan-out takeover, S6E3FC3-P10 panel
  modes; `linux-omarchy` gs-decon family as the code base).
- UFS bring-up (gs201 UFS HCI 0x3c400000, Samsung UfsCal adapter TBD).
- PMIC ADC key mapping for KeypadDeviceLib.
