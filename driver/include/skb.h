/*
 * linux/arinc429/skb.h
 *
 * Definitions for the ARINC429 network socket buffer
 *
 * Copyright (C) 2015 Marek Vasut <marex@denx.de>
 *
 * Based on the SocketCAN stack.
 */

#ifndef __ARINC429_SKB_H__
#define __ARINC429_SKB_H__

#include <linux/types.h>
#include <linux/skbuff.h>
#include "arinc429.h"
#include <net/sock.h>

/*
 * The struct arinc429_skb_priv is used to transport additional information
 * along with the stored arinc429_words that arinc429 not be
 * contained in existing struct sk_buff elements.
 * N.B. that this information must not be modified in cloned ARINC429 sk_buffs.
 * To modify the ARINC429 frame content or the struct arinc429_skb_priv content
 * skb_copy() needs to be used instead of skb_clone().
 */

/**
 * struct arinc429_skb_priv - private additional data inside ARINC429 sk_buffs
 * @ifindex:	ifindex of the first interface the ARINC429 frame appeared on
 * @cf:		align to the following ARINC429 frame at skb->data
 */
struct arinc429_skb_priv {
	int			ifindex;
	union arinc429_word	af[0];
};

#define ARINC429_PRIV_SIZE	(sizeof(struct arinc429_skb_priv))

static inline struct arinc429_skb_priv *arinc429_skb_prv(struct sk_buff *skb)
{
	return (struct arinc429_skb_priv *)(skb->head);
}

static inline void arinc429_skb_reserve(struct sk_buff *skb)
{
	skb_reserve(skb, sizeof(struct arinc429_skb_priv));
}

#endif /* __ARINC429_SKB_H__ */
