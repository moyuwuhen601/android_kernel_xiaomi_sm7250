# LineageOS 24.0 kernel — Xiaomi Redmi K30i 5G (picasso / SM7250, lito)

Downstream port of the Qualcomm SM7250 (lito) 4.19 kernel to **LineageOS 24.0
(Android 17)** for the Xiaomi Redmi K30i 5G (`picasso_48m`) and its K30 5G
siblings.

This repository is **not** our work from scratch — it is built on the work of
others, and the credits below are deliberate. See also the *Attribution* section.

## Upstream sources

| Repository | Branch | What it provides |
|---|---|---|
| [LineageOS/android_kernel_xiaomi_sm7250](https://github.com/LineageOS/android_kernel_xiaomi_sm7250) | `lineage-23.0` | Base tree: SM7250 device, vendor, display/audio/power/touch/fingerprint drivers |
| [LineageOS/android_kernel_xiaomi_sm8250](https://github.com/LineageOS/android_kernel_xiaomi_sm8250) | `lineage-24.0` | Merged in for the 24.0 core: CIP `cip125..136`, the eBPF backports Android 17 requires, the new dma-fence layout, and other core changes |

Every commit inherited from those trees keeps its original author. The Linux
kernel is licensed under **GPL-2.0** (see `COPYING`).

## What this port adds

Branch **`lineage-24.0-port-fix`**. Commit `f420e7401ddc` imports the merged
24.0 tree as a single base commit (the merge of the two upstream trees was
done as `fe2522ebfa60` in the build tree; this repository starts its history
at the imported tree so that it stays small — see *Upstream sources*), and the
7 port commits sit on top:

```
c50e09db127e picasso: enable K30i drivers in milito.config; add ramoops region
36f2f2ce4b6d picasso 24.0 port: fix tfa98xx include path (no addtree), take sm8250 rmnet techpack, drop AW8624
1a83fe3a3dc8 picasso 24.0 port: keep sm7250-23.0 audio/display/power/dts/touch/fp subsystems whole
4680a3c5d3f5 power: supply: silence pointer-to-int cast errors on the 24.0 toolchain flags
84567e11e64c usb: dwc3-msm: only call the sm8250 PS5169 redriver when it is built
075a3904da97 techpack: display: fix sde fence panic with the 24.0 dma-fence layout
c3b8166a5388 arm64: dts: lito-sde: stop advertising DSPP LTM to userspace
```

The two boot-critical fixes are `075a3904da97` (the 24.0 dma-fence union
layout made the kept 23.0 `sde_fence.c` overwrite `cb_list.next`, panicking at
the first display commit) and `c3b8166a5388` (vendor DPPS crashed in
`GetLtmXmlParser()` because the DTB advertised DSPP LTM v1 but the vendor image
ships no `ltm_config_*.xml`; the properties are no longer advertised).

The resulting kernel is `4.19.325-cip136-st20-perf`. Built and tested on a
Redmi K30i 5G running LineageOS 24.0 (release-keys).

## Attribution

The LineageOS 24.0 port of this tree — the conflict resolution described above,
the device-tree/property fixes, and the two boot fixes — was done by
**[@moyuwuhen601](https://github.com/moyuwuhen601) working together with an AI
assistant ([Claude Code](https://claude.com/claude-code))**, 2026-11. The AI
assistance is disclosed in the commit trailers (`Co-Authored-By: Claude Code`).
Any mistakes are ours; the credit for the base kernel belongs to the LineageOS
and Qualcomm upstream authors.
