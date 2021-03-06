/*
 * Copyright (C) 2016 Broadcom
 *	Author: Jayachandran C <jchandra@broadcom.com>
 * Copyright (C) 2016 Semihalf
 * 	Author: Tomasz Nowicki <tn@semihalf.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation (the "GPL").
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License version 2 (GPLv2) for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 (GPLv2) along with this source code.
 */

#define pr_fmt(fmt) "ACPI: " fmt

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/pci-acpi.h>
#include <linux/pci-ecam.h>

/* Structure to hold entries from the MCFG table */
struct mcfg_entry {
	struct list_head	list;
	phys_addr_t		addr;
	u16			segment;
	u8			bus_start;
	u8			bus_end;
};
struct mcfg_fixup {
	char oem_id[ACPI_OEM_ID_SIZE + 1];
	char oem_table_id[ACPI_OEM_TABLE_ID_SIZE + 1];
	u32 oem_revision;
	u16 seg;
	struct resource bus_range;
	struct pci_ecam_ops *ops;
	struct resource cfgres;
};

#define MCFG_DOM_ANY			(-1)
#define MCFG_BUS_RANGE(start, end)	DEFINE_RES_NAMED((start),	\
						((end) - (start) + 1),	\
						NULL, IORESOURCE_BUS)
#define MCFG_BUS_ANY		MCFG_BUS_RANGE(0x0, 0xff)
#define MCFG_RES_EMPTY		DEFINE_RES_NAMED(0, 0, NULL, 0)

static struct mcfg_fixup mcfg_quirks[] = {
/*	{ OEM_ID, OEM_TABLE_ID, REV, DOMAIN, BUS_RANGE, cfgres, ops }, */
#ifdef CONFIG_PCI_HOST_THUNDER_PEM
	/* SoC pass2.x */
	{ "CAVIUM", "THUNDERX", 1, 4, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x88001f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 5, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x884057000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 6, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x88808f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 7, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x89001f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 8, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x894057000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 9, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x89808f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 14, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x98001f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 15, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x984057000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 16, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x98808f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 17, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x99001f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 18, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x994057000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 1, 19, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x99808f000000UL, 0x39 * SZ_16M) },

	/* SoC pass1.x */
	{ "CAVIUM", "THUNDERX", 2, 4, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x88001f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 5, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x884057000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 6, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x88808f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 7, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x89001f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 8, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x894057000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 9, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x89808f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 14, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x98001f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 15, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x984057000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 16, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x98808f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 17, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x99001f000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 18, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x994057000000UL, 0x39 * SZ_16M) },
	{ "CAVIUM", "THUNDERX", 2, 19, MCFG_BUS_ANY, &pci_thunder_pem_ops,
	  DEFINE_RES_MEM(0x99808f000000UL, 0x39 * SZ_16M) },
#endif
#ifdef CONFIG_PCI_HOST_THUNDER_ECAM
	/* SoC pass1.x */
	{ "CAVIUM", "THUNDERX", 2, 0, MCFG_BUS_ANY, &pci_thunder_ecam_ops,
	  MCFG_RES_EMPTY},
	{ "CAVIUM", "THUNDERX", 2, 1, MCFG_BUS_ANY, &pci_thunder_ecam_ops,
	  MCFG_RES_EMPTY},
	{ "CAVIUM", "THUNDERX", 2, 2, MCFG_BUS_ANY, &pci_thunder_ecam_ops,
	  MCFG_RES_EMPTY},
	{ "CAVIUM", "THUNDERX", 2, 3, MCFG_BUS_ANY, &pci_thunder_ecam_ops,
	  MCFG_RES_EMPTY},
	{ "CAVIUM", "THUNDERX", 2, 10, MCFG_BUS_ANY, &pci_thunder_ecam_ops,
	  MCFG_RES_EMPTY},
	{ "CAVIUM", "THUNDERX", 2, 11, MCFG_BUS_ANY, &pci_thunder_ecam_ops,
	  MCFG_RES_EMPTY},
	{ "CAVIUM", "THUNDERX", 2, 12, MCFG_BUS_ANY, &pci_thunder_ecam_ops,
	  MCFG_RES_EMPTY},
	{ "CAVIUM", "THUNDERX", 2, 13, MCFG_BUS_ANY, &pci_thunder_ecam_ops,
	  MCFG_RES_EMPTY},
#endif
	{ "QCOM  ", "QDF2432 ", 1, 0, MCFG_BUS_ANY, &pci_32b_ops },
	{ "QCOM  ", "QDF2432 ", 1, 1, MCFG_BUS_ANY, &pci_32b_ops },
	{ "QCOM  ", "QDF2432 ", 1, 2, MCFG_BUS_ANY, &pci_32b_ops },
	{ "QCOM  ", "QDF2432 ", 1, 3, MCFG_BUS_ANY, &pci_32b_ops },
	{ "QCOM  ", "QDF2432 ", 1, 4, MCFG_BUS_ANY, &pci_32b_ops },
	{ "QCOM  ", "QDF2432 ", 1, 5, MCFG_BUS_ANY, &pci_32b_ops },
	{ "QCOM  ", "QDF2432 ", 1, 6, MCFG_BUS_ANY, &pci_32b_ops },
	{ "QCOM  ", "QDF2432 ", 1, 7, MCFG_BUS_ANY, &pci_32b_ops },
#ifdef CONFIG_PCI_HISI_ACPI
	#define PCI_ACPI_QUIRK_QUAD_DOM(table_id, seg, ops) \
	{ "HISI  ", table_id, 0, seg + 0, MCFG_BUS_ANY, ops }, \
	{ "HISI  ", table_id, 0, seg + 1, MCFG_BUS_ANY, ops }, \
	{ "HISI  ", table_id, 0, seg + 2, MCFG_BUS_ANY, ops }, \
	{ "HISI  ", table_id, 0, seg + 3, MCFG_BUS_ANY, ops }
	PCI_ACPI_QUIRK_QUAD_DOM("HIP05   ", 0, &hisi_pcie_ops),
	PCI_ACPI_QUIRK_QUAD_DOM("HIP06   ", 0, &hisi_pcie_ops),
	PCI_ACPI_QUIRK_QUAD_DOM("HIP07   ", 0, &hisi_pcie_ops),
	PCI_ACPI_QUIRK_QUAD_DOM("HIP07   ", 4, &hisi_pcie_ops),
	PCI_ACPI_QUIRK_QUAD_DOM("HIP07   ", 8, &hisi_pcie_ops),
	PCI_ACPI_QUIRK_QUAD_DOM("HIP07   ", 12, &hisi_pcie_ops),
#endif

#define XGENE_V1_ECAM_MCFG(rev, seg) \
	{"APM   ", "XGENE   ", rev, seg, MCFG_BUS_ANY, \
		&xgene_v1_pcie_ecam_ops }
#define XGENE_V2_ECAM_MCFG(rev, seg) \
	{"APM   ", "XGENE   ", rev, seg, MCFG_BUS_ANY, \
		&xgene_v2_pcie_ecam_ops }
	/* X-Gene SoC with v1 PCIe controller */
	XGENE_V1_ECAM_MCFG(1, 0),
	XGENE_V1_ECAM_MCFG(1, 1),
	XGENE_V1_ECAM_MCFG(1, 2),
	XGENE_V1_ECAM_MCFG(1, 3),
	XGENE_V1_ECAM_MCFG(1, 4),
	XGENE_V1_ECAM_MCFG(2, 0),
	XGENE_V1_ECAM_MCFG(2, 1),
	XGENE_V1_ECAM_MCFG(2, 2),
	XGENE_V1_ECAM_MCFG(2, 3),
	XGENE_V1_ECAM_MCFG(2, 4),
	/* X-Gene SoC with v2.1 PCIe controller */
	XGENE_V2_ECAM_MCFG(3, 0),
	XGENE_V2_ECAM_MCFG(3, 1),
	/* X-Gene SoC with v2.2 PCIe controller */
	XGENE_V2_ECAM_MCFG(4, 0),
	XGENE_V2_ECAM_MCFG(4, 1),
	XGENE_V2_ECAM_MCFG(4, 2),
};

static char mcfg_oem_id[ACPI_OEM_ID_SIZE];
static char mcfg_oem_table_id[ACPI_OEM_TABLE_ID_SIZE];
static u32 mcfg_oem_revision;

static void pci_mcfg_match_quirks(struct acpi_pci_root *root,
				  struct resource *cfgres,
				  struct pci_ecam_ops **ecam_ops)
{
	struct mcfg_fixup *f;
	int i;

	/*
	 * First match against PCI topology <domain:bus> then use OEM ID, OEM
	 * table ID, and OEM revision from MCFG table standard header.
	 */
	for (i = 0, f = mcfg_quirks; i < ARRAY_SIZE(mcfg_quirks); i++, f++) {
		if (f->seg == root->segment &&
		    resource_contains(&f->bus_range, &root->secondary) &&
		    !memcmp(f->oem_id, mcfg_oem_id, ACPI_OEM_ID_SIZE) &&
		    !memcmp(f->oem_table_id, mcfg_oem_table_id,
		            ACPI_OEM_TABLE_ID_SIZE) &&
		    f->oem_revision == mcfg_oem_revision) {
			if (f->cfgres.start)
				*cfgres = f->cfgres;
			if (f->ops)
				*ecam_ops =  f->ops;
			dev_info(&root->device->dev, "Applying PCI MCFG quirks for %s %s rev: %d\n",
				 f->oem_id, f->oem_table_id, f->oem_revision);
			return;
		}
	}
}

/* List to save MCFG entries */
static LIST_HEAD(pci_mcfg_list);

int pci_mcfg_lookup(struct acpi_pci_root *root, struct resource *cfgres,
		    struct pci_ecam_ops **ecam_ops)
{
	struct pci_ecam_ops *ops = &pci_generic_ecam_ops;
	struct resource *bus_res = &root->secondary;
	u16 seg = root->segment;
	struct mcfg_entry *e;
	struct resource res;

	/* Use address from _CBA if present, otherwise lookup MCFG */
	if (root->mcfg_addr)
		goto skip_lookup;

	/*
	 * We expect exact match, unless MCFG entry end bus covers more than
	 * specified by caller.
	 */
	list_for_each_entry(e, &pci_mcfg_list, list) {
		if (e->segment == seg && e->bus_start == bus_res->start &&
		    e->bus_end >= bus_res->end) {
			root->mcfg_addr = e->addr;
		}

	}

skip_lookup:
	memset(&res, 0, sizeof(res));
	if (root->mcfg_addr) {
		res.start = root->mcfg_addr + (bus_res->start << 20);
		res.end = res.start + (resource_size(bus_res) << 20) - 1;
		res.flags = IORESOURCE_MEM;
	}

	/*
	 * Let to override default ECAM ops and CFG resource range.
	 * Also, this might even retrieve CFG resource range in case MCFG
	 * does not have it. Invalid CFG start address means MCFG firmware bug
	 * or we need another quirk in array.
	 */
	pci_mcfg_match_quirks(root, &res, &ops);
	if (!res.start)
		return -ENXIO;

	*cfgres = res;
	*ecam_ops = ops;
	return 0;
}

static __init int pci_mcfg_parse(struct acpi_table_header *header)
{
	struct acpi_table_mcfg *mcfg;
	struct acpi_mcfg_allocation *mptr;
	struct mcfg_entry *e, *arr;
	int i, n;

	if (header->length < sizeof(struct acpi_table_mcfg))
		return -EINVAL;

	n = (header->length - sizeof(struct acpi_table_mcfg)) /
					sizeof(struct acpi_mcfg_allocation);
	mcfg = (struct acpi_table_mcfg *)header;
	mptr = (struct acpi_mcfg_allocation *) &mcfg[1];

	arr = kcalloc(n, sizeof(*arr), GFP_KERNEL);
	if (!arr)
		return -ENOMEM;

	for (i = 0, e = arr; i < n; i++, mptr++, e++) {
		e->segment = mptr->pci_segment;
		e->addr =  mptr->address;
		e->bus_start = mptr->start_bus_number;
		e->bus_end = mptr->end_bus_number;
		list_add(&e->list, &pci_mcfg_list);
	}

	/* Save MCFG IDs and revision for quirks matching */
	memcpy(mcfg_oem_id, header->oem_id, ACPI_OEM_ID_SIZE);
	memcpy(mcfg_oem_table_id, header->oem_table_id, ACPI_OEM_TABLE_ID_SIZE);
	mcfg_oem_revision = header->oem_revision;

	pr_info("MCFG table detected, %d entries\n", n);
	return 0;
}

/* Interface called by ACPI - parse and save MCFG table */
void __init pci_mmcfg_late_init(void)
{
	int err = acpi_table_parse(ACPI_SIG_MCFG, pci_mcfg_parse);
	if (err)
		pr_err("Failed to parse MCFG (%d)\n", err);
}
