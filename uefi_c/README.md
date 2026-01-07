## README

Usage:
```bash
./build.sh
```

Loading Kernel Image:
```
dev_path_merged: PciRoot(0x0)/Pci(0x1,0x0)/HD(1,MBR,0xBE1AFDFA,0x3F,0xFBFC1)/Image
LoadImage: Success!
EFI stub: Booting Linux Kernel...
EFI stub: EFI_RNG_PROTOCOL unavailable
EFI stub: Generating empty DTB
EFI stub: Exiting boot services...
[    0.000000] Booting Linux on physical CPU 0x0000000000 [0x411fd070]
[    0.000000] Linux version 5.15.179 (zyp@hp) (Ubuntu clang version 14.0.0-1ubuntu1.1, Ubuntu LLD 14.0.0) #7 SMP PREEMPT Tue Jan 6 16:15:07 CST 2026
[    0.000000] efi: EFI v2.70 by EDK II
[    0.000000] efi: SMBIOS 3.0=0x7bed0000 MEMATTR=0x792ee698 ACPI 2.0=0x78430018 MEMRESERVE=0x7842ff18
[    0.000000] ACPI: Early table checksum verification disabled
[    0.000000] ACPI: RSDP 0x0000000078430018 000024 (v02 BOCHS )
[    0.000000] ACPI: XSDT 0x000000007843FE98 000064 (v01 BOCHS  BXPC     00000001      01000013)
[    0.000000] ACPI: FACP 0x000000007843FA98 000114 (v06 BOCHS  BXPC     00000001 BXPC 00000001)
[    0.000000] ACPI: DSDT 0x0000000078437518 0014BE (v02 BOCHS  BXPC     00000001 BXPC 00000001)
[    0.000000] ACPI: APIC 0x000000007843FC18 0000AC (v04 BOCHS  BXPC     00000001 BXPC 00000001)
[    0.000000] ACPI: PPTT 0x000000007843FD18 000060 (v02 BOCHS  BXPC     00000001 BXPC 00000001)
[    0.000000] ACPI: GTDT 0x000000007843D898 000068 (v03 BOCHS  BXPC     00000001 BXPC 00000001)
[    0.000000] ACPI: MCFG 0x000000007843FE18 00003C (v01 BOCHS  BXPC     00000001 BXPC 00000001)
[    0.000000] ACPI: SPCR 0x000000007843FF98 000050 (v02 BOCHS  BXPC     00000001 BXPC 00000001)
[    0.000000] ACPI: DBG2 0x000000007843E818 000057 (v00 BOCHS  BXPC     00000001 BXPC 00000001)
[    0.000000] ACPI: IORT 0x000000007843E898 000080 (v03 BOCHS  BXPC     00000001 BXPC 00000001)
[    0.000000] ACPI: SPCR: console: pl011,mmio32,0x9000000,9600
[    0.000000] NUMA: Failed to initialise from firmware
[    0.000000] NUMA: Faking a node at [mem 0x0000000040000000-0x000000007fffffff]
[    0.000000] NUMA: NODE_DATA [mem 0x7fe00c00-0x7fe02fff]
[    0.000000] Zone ranges:
[    0.000000]   DMA      [mem 0x0000000040000000-0x000000007fffffff]
[    0.000000]   DMA32    empty
[    0.000000]   Normal   empty
[    0.000000] Movable zone start for each node
[    0.000000] Early memory node ranges
[    0.000000]   node   0: [mem 0x0000000040000000-0x000000007843ffff]
[    0.000000]   node   0: [mem 0x0000000078440000-0x000000007872ffff]
[    0.000000]   node   0: [mem 0x0000000078730000-0x000000007bbfffff]
[    0.000000]   node   0: [mem 0x000000007bc00000-0x000000007bfdffff]
[    0.000000]   node   0: [mem 0x000000007bfe0000-0x000000007fffffff]
[    0.000000] Initmem setup node 0 [mem 0x0000000040000000-0x000000007fffffff]
[    0.000000] cma: Reserved 32 MiB at 0x000000007cc00000
[    0.000000] psci: probing for conduit method from ACPI.
[    0.000000] psci: PSCIv1.1 detected in firmware.
[    0.000000] psci: Using standard PSCI v0.2 function IDs
[    0.000000] psci: Trusted OS migration not required
[    0.000000] psci: SMC Calling Convention v1.0
[    0.000000] ACPI: SRAT not present
[    0.000000] percpu: Embedded 20 pages/cpu s41496 r8192 d32232 u81920
[    0.000000] Detected PIPT I-cache on CPU0
[    0.000000] CPU features: detected: Spectre-v2
[    0.000000] CPU features: detected: Spectre-v3a
[    0.000000] CPU features: detected: Spectre-v4
[    0.000000] CPU features: detected: Spectre-BHB
[    0.000000] CPU features: kernel page table isolation forced ON by KASLR
[    0.000000] CPU features: detected: Kernel page table isolation (KPTI)
[    0.000000] CPU features: detected: ARM erratum 834220
[    0.000000] CPU features: detected: ARM erratum 1742098
[    0.000000] CPU features: detected: ARM erratum 832075
[    0.000000] CPU features: detected: ARM errata 1165522, 1319367, or 1530923
[    0.000000] Built 1 zonelists, mobility grouping on.  Total pages: 258048
[    0.000000] Policy zone: DMA
[    0.000000] Kernel command line: root=/dev/vdb rw init=/init
[    0.000000] Dentry cache hash table entries: 131072 (order: 8, 1048576 bytes, linear)
[    0.000000] Inode-cache hash table entries: 65536 (order: 7, 524288 bytes, linear)
[    0.000000] mem auto-init: stack:all(zero), heap alloc:off, heap free:off
[    0.000000] Memory: 959656K/1048576K available (15488K kernel code, 3110K rwdata, 8220K rodata, 1600K init, 498K bss, 56152K reserved, 32768K cma-reserved)
[    0.000000] SLUB: HWalign=64, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
[    0.000000] rcu: Preemptible hierarchical RCU implementation.
[    0.000000] rcu:     RCU event tracing is enabled.
[    0.000000] rcu:     RCU restricting CPUs from NR_CPUS=256 to nr_cpu_ids=1.
[    0.000000]  Trampoline variant of Tasks RCU enabled.
[    0.000000] rcu: RCU calculated value of scheduler-enlistment delay is 25 jiffies.
[    0.000000] rcu: Adjusting geometry for rcu_fanout_leaf=16, nr_cpu_ids=1
[    0.000000] NR_IRQS: 64, nr_irqs: 64, preallocated irqs: 0
[    0.000000] Root IRQ handler: gic_handle_irq
[    0.000000] GICv2m: ACPI overriding V2M MSI_TYPER (base:80, num:64)
[    0.000000] GICv2m: range[mem 0x08020000-0x08020fff], SPI[80:143]
[    0.000000] arch_timer: cp15 timer(s) running at 62.50MHz (virt).
[    0.000000] clocksource: arch_sys_counter: mask: 0xffffffffffffff max_cycles: 0x1cd42e208c, max_idle_ns: 881590405314 ns
[    0.000047] sched_clock: 56 bits at 62MHz, resolution 16ns, wraps every 4398046511096ns
[    0.007376] Console: colour dummy device 80x25
[    0.008573] ACPI: Core revision 20210730
[    0.011615] Calibrating delay loop (skipped), value calculated using timer frequency.. 125.00 BogoMIPS (lpj=250000)
[    0.011736] pid_max: default: 32768 minimum: 301
[    0.012251] LSM: Security Framework initializing
[    0.014395] Mount-cache hash table entries: 2048 (order: 2, 16384 bytes, linear)
[    0.014434] Mountpoint-cache hash table entries: 2048 (order: 2, 16384 bytes, linear)
[    0.040359] rcu: Hierarchical SRCU implementation.
[    0.043019] Remapping and enabling EFI services.
[    0.044689] smp: Bringing up secondary CPUs ...
[    0.044782] smp: Brought up 1 node, 1 CPU
[    0.044812] SMP: Total of 1 processors activated.
[    0.044890] CPU features: detected: 32-bit EL0 Support
[    0.044910] CPU features: detected: 32-bit EL1 Support
[    0.044973] CPU features: detected: CRC32 instructions
[    0.065702] CPU: All CPU(s) started at EL1
[    0.066073] alternatives: patching kernel code
[    0.081776] devtmpfs: initialized
[    0.086538] KASLR disabled due to lack of seed
[    0.087387] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 7645041785100000 ns
[    0.087506] futex hash table entries: 256 (order: 2, 16384 bytes, linear)
[    0.091103] pinctrl core: initialized pinctrl subsystem
[    0.100083] SMBIOS 3.0.0 present.
[    0.100268] DMI: QEMU QEMU Virtual Machine, BIOS 0.0.0 02/06/2015
[    0.106508] NET: Registered PF_NETLINK/PF_ROUTE protocol family
[    0.114326] DMA: preallocated 128 KiB GFP_KERNEL pool for atomic allocations
[    0.114619] DMA: preallocated 128 KiB GFP_KERNEL|GFP_DMA pool for atomic allocations
[    0.114781] DMA: preallocated 128 KiB GFP_KERNEL|GFP_DMA32 pool for atomic allocations
[    0.114956] audit: initializing netlink subsys (disabled)
[    0.119213] thermal_sys: Registered thermal governor 'step_wise'
[    0.119263] thermal_sys: Registered thermal governor 'power_allocator'
[    0.120519] audit: type=2000 audit(0.104:1): state=initialized audit_enabled=0 res=1
[    0.120823] cpuidle: using governor menu
[    0.122040] hw-breakpoint: found 6 breakpoint and 4 watchpoint registers.
[    0.122350] ASID allocator initialised with 32768 entries
[    0.123874] ACPI: bus type PCI registered
[    0.123949] acpiphp: ACPI Hot Plug PCI Controller Driver version: 0.5
[    0.125055] Serial: AMBA PL011 UART driver
[    0.142886] HugeTLB registered 1.00 GiB page size, pre-allocated 0 pages
[    0.142921] HugeTLB registered 32.0 MiB page size, pre-allocated 0 pages
[    0.142933] HugeTLB registered 2.00 MiB page size, pre-allocated 0 pages
[    0.142943] HugeTLB registered 64.0 KiB page size, pre-allocated 0 pages
[    0.151153] cryptd: max_cpu_qlen set to 1000
[    0.156164] ACPI: Added _OSI(Module Device)
[    0.156193] ACPI: Added _OSI(Processor Device)
[    0.156204] ACPI: Added _OSI(3.0 _SCP Extensions)
[    0.156212] ACPI: Added _OSI(Processor Aggregator Device)
[    0.156277] ACPI: Added _OSI(Linux-Dell-Video)
[    0.156290] ACPI: Added _OSI(Linux-Lenovo-NV-HDMI-Audio)
[    0.156300] ACPI: Added _OSI(Linux-HPI-Hybrid-Graphics)
[    0.168928] ACPI: 1 ACPI AML tables successfully acquired and loaded
[    0.174863] ACPI: Interpreter enabled
[    0.174895] ACPI: Using GIC for interrupt routing
[    0.175188] ACPI: MCFG table detected, 1 entries
[    0.197481] ARMH0011:00: ttyAMA0 at MMIO 0x9000000 (irq = 12, base_baud = 0) is a SBSA
[    0.215060] printk: console [ttyAMA0] enabled
[    0.228977] ACPI: PCI: Interrupt link L000 configured for IRQ 35
[    0.229258] ACPI: PCI: Interrupt link L001 configured for IRQ 36
[    0.229479] ACPI: PCI: Interrupt link L002 configured for IRQ 37
[    0.229694] ACPI: PCI: Interrupt link L003 configured for IRQ 38
[    0.230167] ACPI: PCI Root Bridge [PCI0] (domain 0000 [bus 00-ff])
[    0.231627] acpi PNP0A08:00: _OSC: OS supports [ExtendedConfig ASPM ClockPM Segments MSI HPX-Type3]
[    0.236369] acpi PNP0A08:00: _OSC: platform does not support [LTR]
[    0.237172] acpi PNP0A08:00: _OSC: OS now controls [PME PCIeCapability]
[    0.238964] acpi PNP0A08:00: ECAM area [mem 0x4010000000-0x401fffffff] reserved by PNP0C02:00
[    0.239478] acpi PNP0A08:00: ECAM at [mem 0x4010000000-0x401fffffff] for [bus 00-ff]
[    0.240612] ACPI: Remapped I/O 0x000000003eff0000 to [io  0x0000-0xffff window]
[    0.243182] PCI host bridge to bus 0000:00
[    0.243417] pci_bus 0000:00: root bus resource [mem 0x10000000-0x3efeffff window]
[    0.243672] pci_bus 0000:00: root bus resource [io  0x0000-0xffff window]
[    0.243890] pci_bus 0000:00: root bus resource [mem 0x8000000000-0xffffffffff window]
[    0.244193] pci_bus 0000:00: root bus resource [bus 00-ff]
[    0.246375] pci 0000:00:00.0: [1b36:0008] type 00 class 0x060000
[    0.250252] pci 0000:00:01.0: [1af4:1001] type 00 class 0x010000
[    0.250980] pci 0000:00:01.0: reg 0x10: [io  0x0080-0x00ff]
[    0.251417] pci 0000:00:01.0: reg 0x14: [mem 0x10001000-0x10001fff]
[    0.252257] pci 0000:00:01.0: reg 0x20: [mem 0x8000000000-0x8000003fff 64bit pref]
[    0.253607] pci 0000:00:02.0: [1af4:1001] type 00 class 0x010000
[    0.254044] pci 0000:00:02.0: reg 0x10: [io  0x0000-0x007f]
[    0.254419] pci 0000:00:02.0: reg 0x14: [mem 0x10000000-0x10000fff]
[    0.255176] pci 0000:00:02.0: reg 0x20: [mem 0x8000004000-0x8000007fff 64bit pref]
[    0.257428] pci 0000:00:01.0: BAR 4: assigned [mem 0x8000000000-0x8000003fff 64bit pref]
[    0.257926] pci 0000:00:02.0: BAR 4: assigned [mem 0x8000004000-0x8000007fff 64bit pref]
[    0.258199] pci 0000:00:01.0: BAR 1: assigned [mem 0x10000000-0x10000fff]
[    0.258423] pci 0000:00:02.0: BAR 1: assigned [mem 0x10001000-0x10001fff]
[    0.258640] pci 0000:00:01.0: BAR 0: assigned [io  0x1000-0x107f]
[    0.259305] pci 0000:00:02.0: BAR 0: assigned [io  0x1080-0x10ff]
[    0.259984] pci_bus 0000:00: resource 4 [mem 0x10000000-0x3efeffff window]
[    0.260227] pci_bus 0000:00: resource 5 [io  0x0000-0xffff window]
[    0.260416] pci_bus 0000:00: resource 6 [mem 0x8000000000-0xffffffffff window]
[    0.265860] iommu: Default domain type: Translated
[    0.266060] iommu: DMA domain TLB invalidation policy: strict mode
[    0.266965] vgaarb: loaded
[    0.267940] SCSI subsystem initialized
[    0.269173] ACPI: bus type USB registered
[    0.269606] usbcore: registered new interface driver usbfs
[    0.270022] usbcore: registered new interface driver hub
[    0.270297] usbcore: registered new device driver usb
[    0.271223] pps_core: LinuxPPS API ver. 1 registered
[    0.271380] pps_core: Software ver. 5.3.6 - Copyright 2005-2007 Rodolfo Giometti <giometti@linux.it>
[    0.272265] PTP clock support registered
[    0.272651] EDAC MC: Ver: 3.0.0
[    0.274510] Registered efivars operations
[    0.276766] FPGA manager framework
[    0.277427] Advanced Linux Sound Architecture Driver Initialized.
[    0.287426] clocksource: Switched to clocksource arch_sys_counter
[    0.288375] VFS: Disk quotas dquot_6.6.0
[    0.288605] VFS: Dquot-cache hash table entries: 512 (order 0, 4096 bytes)
[    0.289984] pnp: PnP ACPI init
[    0.293244] system 00:00: [mem 0x4010000000-0x401fffffff window] could not be reserved
[    0.293776] pnp: PnP ACPI: found 1 devices
[    0.303712] NET: Registered PF_INET protocol family
[    0.305035] IP idents hash table entries: 16384 (order: 5, 131072 bytes, linear)
[    0.308697] tcp_listen_portaddr_hash hash table entries: 512 (order: 1, 8192 bytes, linear)
[    0.309018] Table-perturb hash table entries: 65536 (order: 6, 262144 bytes, linear)
[    0.309295] TCP established hash table entries: 8192 (order: 4, 65536 bytes, linear)
[    0.309728] TCP bind hash table entries: 8192 (order: 5, 131072 bytes, linear)
[    0.310114] TCP: Hash tables configured (established 8192 bind 8192)
[    0.312062] UDP hash table entries: 512 (order: 2, 16384 bytes, linear)
[    0.312492] UDP-Lite hash table entries: 512 (order: 2, 16384 bytes, linear)
[    0.313727] NET: Registered PF_UNIX/PF_LOCAL protocol family
[    0.315814] RPC: Registered named UNIX socket transport module.
[    0.316039] RPC: Registered udp transport module.
[    0.316187] RPC: Registered tcp transport module.
[    0.316325] RPC: Registered tcp NFSv4.1 backchannel transport module.
[    0.316647] PCI: CLS 0 bytes, default 64
[    0.321733] hw perfevents: enabled with armv8_pmuv3_0 PMU driver, 7 counters available
[    0.322187] kvm [1]: HYP mode not available
[    0.330084] Initialise system trusted keyrings
[    0.332411] workingset: timestamp_bits=42 max_order=18 bucket_order=0
[    0.341802] squashfs: version 4.0 (2009/01/31) Phillip Lougher
[    0.343669] NFS: Registering the id_resolver key type
[    0.344083] Key type id_resolver registered
[    0.344255] Key type id_legacy registered
[    0.344700] nfs4filelayout_init: NFSv4 File Layout Driver Registering...
[    0.344988] nfs4flexfilelayout_init: NFSv4 Flexfile Layout Driver Registering...
[    0.345678] 9p: Installing v9fs 9p2000 file system support
[    0.362702] Key type asymmetric registered
[    0.362889] Asymmetric key parser 'x509' registered
[    0.363341] Block layer SCSI generic (bsg) driver version 0.4 loaded (major 245)
[    0.363579] io scheduler mq-deadline registered
[    0.363716] io scheduler kyber registered
[    0.377435] input: Power Button as /devices/LNXSYSTM:00/LNXSYBUS:00/PNP0C0C:00/input/input0
[    0.378397] ACPI: button: Power Button [PWRB]
[    0.380072] EINJ: EINJ table not found.
[    0.414623] ACPI: \_SB_.L001: Enabled at IRQ 36
[    0.415453] virtio-pci 0000:00:01.0: enabling device (0005 -> 0007)
[    0.418563] ACPI: \_SB_.L002: Enabled at IRQ 37
[    0.418728] virtio-pci 0000:00:02.0: enabling device (0005 -> 0007)
[    0.425194] Serial: 8250/16550 driver, 4 ports, IRQ sharing enabled
[    0.428170] SuperH (H)SCI(F) driver initialized
[    0.428714] msm_serial: driver initialized
[    0.442859] loop: module loaded
[    0.449137] virtio_blk virtio0: [vda] 1032192 512-byte logical blocks (528 MB/504 MiB)
[    0.461882]  vda: vda1
[    0.466368] virtio_blk virtio1: [vdb] 131072 512-byte logical blocks (67.1 MB/64.0 MiB)
[    0.469965] megasas: 07.717.02.00-rc1
[    0.478086] tun: Universal TUN/TAP device driver, 1.6
[    0.479456] thunder_xcv, ver 1.0
[    0.479607] thunder_bgx, ver 1.0
[    0.479753] nicpf, ver 1.0
[    0.481052] hns3: Hisilicon Ethernet Network Driver for Hip08 Family - version
[    0.481202] hns3: Copyright (c) 2017 Huawei Corporation.
[    0.481538] hclge is initializing
[    0.481709] e1000: Intel(R) PRO/1000 Network Driver
[    0.481837] e1000: Copyright (c) 1999-2006 Intel Corporation.
[    0.482046] e1000e: Intel(R) PRO/1000 Network Driver
[    0.482161] e1000e: Copyright(c) 1999 - 2015 Intel Corporation.
[    0.482354] igb: Intel(R) Gigabit Ethernet Network Driver
[    0.482475] igb: Copyright (c) 2007-2014 Intel Corporation.
[    0.482628] igbvf: Intel(R) Gigabit Virtual Function Network Driver
[    0.482774] igbvf: Copyright (c) 2009 - 2012 Intel Corporation.
[    0.483320] sky2: driver version 1.30
[    0.484648] VFIO - User Level meta-driver version: 0.3
[    0.487094] ehci_hcd: USB 2.0 'Enhanced' Host Controller (EHCI) Driver
[    0.487460] ehci-pci: EHCI PCI platform driver
[    0.487700] ehci-platform: EHCI generic platform driver
[    0.487932] ehci-orion: EHCI orion driver
[    0.488120] ehci-exynos: EHCI Exynos driver
[    0.488326] ohci_hcd: USB 1.1 'Open' Host Controller (OHCI) Driver
[    0.488525] ohci-pci: OHCI PCI platform driver
[    0.488742] ohci-platform: OHCI generic platform driver
[    0.489043] ohci-exynos: OHCI Exynos driver
[    0.490194] usbcore: registered new interface driver usb-storage
[    0.496981] rtc-efi rtc-efi.0: registered as rtc0
[    0.497644] rtc-efi rtc-efi.0: setting system clock to 2026-01-07T06:02:33 UTC (1767765753)
[    0.498795] i2c_dev: i2c /dev entries driver
[    0.504446] sdhci: Secure Digital Host Controller Interface driver
[    0.504593] sdhci: Copyright(c) Pierre Ossman
[    0.505189] Synopsys Designware Multimedia Card Interface Driver
[    0.505955] sdhci-pltfm: SDHCI platform and OF driver helper
[    0.507297] ledtrig-cpu: registered to indicate activity on CPUs
[    0.512794] pstore: Registered efi as persistent store backend
[    0.514836] usbcore: registered new interface driver usbhid
[    0.515008] usbhid: USB HID core driver
[    0.521274] NET: Registered PF_PACKET protocol family
[    0.522469] 9pnet: Installing 9P2000 support
[    0.522769] Key type dns_resolver registered
[    0.523629] Loading compiled-in X.509 certificates
[    0.525723] pstore: Using crash dump compression: deflate
[    0.529970] clk: Disabling unused clocks
[    0.530839] ALSA device list:
[    0.531096]   No soundcards found.
[    0.561399] EXT4-fs (vdb): recovery complete
[    0.562151] EXT4-fs (vdb): mounted filesystem with ordered data mode. Opts: (null). Quota mode: none.
[    0.562668] VFS: Mounted root (ext4 filesystem) on device 254:16.
[    0.564774] devtmpfs: mounted
[    0.598564] Freeing unused kernel memory: 1600K
[    0.599398] Run /init as init process
Booted into minimal rootfs
/bin/sh: can't access tty; job control turned off
~ #
~ # busybox ls
bin         etc         lost+found  sbin        tmp
dev         init        proc        sys
```

