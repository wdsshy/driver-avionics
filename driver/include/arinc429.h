/*
 * linux/arinc429.h
 *
 * Definitions for ARINC429 network layer
 * (socket addr / ARINC429 frame / ARINC429 filter)
 *
 * Copyright (C) 2015 Marek Vasut <marex@denx.de>
 * Updates Copyright (C) 2019 CCX Technologies Inc. <charles@ccxtechnologies.com>
 *
 * Based on the SocketCAN stack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __UAPI_ARINC429_H__
#define __UAPI_ARINC429_H__

#include <linux/types.h>
#include <linux/socket.h>

/************************************************************************
 * CCX: This is an ugly hack so that we can build this out of tree without
 * patching the kernel, delete it and put these definions in the propper
 * place if this is ever pulled into your kernel. */

/* should be in include/linux/socket.h, and should have it's own
 * index, not stealing from Ash (which is unused) */
#ifndef AF_ARINC429
#define AF_ARINC429		AF_ASH
#endif

/* should be in include/linux/socket.h, and should have it's own
 * index, not stealing from Ash (which is unused) */
#ifndef PF_ARINC429
#define PF_ARINC429		AF_ARINC429
#endif

/* should be in include/uapi/linux/if_arp.h */
#ifndef ARPHRD_ARINC429
#define ARPHRD_ARINC429	281
#endif

/* should be in include/uapi/linux/if_ether.h */
#ifndef ETH_P_ARINC429
#define ETH_P_ARINC429	0x001D
#endif

/************************************************************************/

/* ARINC429 kernel definitions */

/*
 * ARINC packet:
 *
 * .-.---.------.---.-----.
 * |P|SSM| Data |SDI|Label|
 * '-'---'------'---'-----'
 *  3 3 2 2....1 1 9 8...0
 *  1 0 9 8    1 0
 */

/**
 * struct arinc429_word - basic ARINC429 word structure
 * @label:	ARINC429 label
 * @data:	ARINC429 P, SSM, DATA and SDI
 */
union arinc429_word {
	__u32 raw;
	struct {
		__u32 label:8;
		__u32 sdi:2;
		__u32 data:21;
		__u32 parity:1;
	} fmt;
};

#define ARINC429_WORD_SIZE	(sizeof(union arinc429_word))

/* particular protocols of the protocol family PF_ARINC429 */
#define ARINC429_RAW		1 /* RAW sockets */
#define ARINC429_NPROTO		2

#define SOL_ARINC429_BASE	100

/**
 * struct sockaddr_arinc429 - the sockaddr structure for ARINC429 sockets
 * @arinc429_family:	address family number AF_ARINC429.
 * @arinc429_ifindex:	ARINC429 network interface index.
 * @arinc429_addr:	protocol specific address information
 */
struct sockaddr_arinc429 {
	__kernel_sa_family_t arinc429_family;
	int         arinc429_ifindex;
	union {
		/* reserved for future ARINC429 protocols address information */
	} arinc429_addr;
};

/**
 * struct arinc429_filter - ARINC429 ID based filter in arinc429_register().
 * @arinc429_label: relevant bits of ARINC429 ID which are not masked out.
 * @arinc429_mask:  ARINC429 mask (see description)
 *
 * Description:
 * A filter matches, when
 *
 *          <received_arinc429_id> & mask == arinc429_id & mask
 */
struct arinc429_filter {
	__u8	label;		/* 8 bit label */
	__u8	mask;		/* 8 bit label mask */
#define ARINC429_INV_FILTER	0x00000001
	__u32	flags;		/* Flags */
};

#endif /* __UAPI_ARINC429_H__ */
