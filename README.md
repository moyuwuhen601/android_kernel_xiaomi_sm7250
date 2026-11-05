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

The base port is branch **`lineage-24.0-port-fix`**. Commit `f420e7401ddc` imports the merged
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

## SukiSU Ultra root variant

Branch **`lineage-24.0-sukisu`** adds **SukiSU Ultra v4.3.0-r1** (a KernelSU
fork) to the kernel above, built in as `CONFIG_KSU=y`:

```
970f65e picasso: build SukiSU Ultra into the kernel
e5f1e14 picasso: port SukiSU Ultra to the 4.19 kernel interfaces
dd32ff1 picasso: add path_mount/path_umount and seccomp_filter_release for KernelSU
5a051b7 picasso: fix SukiSU syscall table patch on 4.19 block mappings
421946e picasso: hand the manager's ksud a driver fd after exec
```

The port adds the missing `path_mount`/`path_umount` and
`seccomp_filter_release()` interfaces, teaches the driver about 4.19's
SELinux policydb, fsnotify, VFS and task_work APIs, and handles the 2 MB block
mappings used for `.rodata` on this kernel. The last commit installs an
`O_CLOEXEC` driver fd into the manager's freshly exec'd ksud, because 4.19 has
no seccomp action cache and the manager's `reboot`-magic fd request would
otherwise be trapped by the app's seccomp filter. The kernel release string
stays `4.19.325-cip136-st20-perf-gbfb9dd7d6863` so the vendor `rmnet_perf.ko` /
`rmnet_shs.ko` vermagic still matches when only boot is flashed.

SukiSU Ultra: <https://github.com/SukiSU-Ultra/SukiSU-Ultra>. It is based on
KernelSU (<https://github.com/tiann/KernelSU>), licensed under GPL-2.0.

## Attribution

The LineageOS 24.0 port of this tree — the conflict resolution described above,
the device-tree/property fixes, and the two boot fixes — was done by
**[@moyuwuhen601](https://github.com/moyuwuhen601) working together with an AI
assistant ([Claude Code](https://claude.com/claude-code))**, 2026-11. The
SukiSU Ultra integration and its 4.19 port on branch `lineage-24.0-sukisu`
were done the same way. The AI assistance is disclosed in the commit trailers
(`Co-Authored-By: Claude Code`). Any mistakes are ours; the credit for the base
kernel belongs to the LineageOS and Qualcomm upstream authors, and SukiSU
Ultra/KernelSU to their respective authors.
