/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/03 15:36:46 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/06 20:04:37 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

/*
** Note that rounds are generally indexed from 0
*/

extern const unsigned int expanPerm[48];
extern const unsigned int pboxPerm[32];

static unsigned long des_feistel(unsigned long block, unsigned long key)
{
	block = ftssl_des_permute(block, 32, (unsigned int *)expanPerm, 48);
	block ^= key;
	block = ftssl_des_sbox_sub(block);
	block = ftssl_des_permute(block, 32, (unsigned int *)pboxPerm, 32);
	return (block);
}

extern const unsigned int initPerm[64];
extern const unsigned int keyPerm[56];
extern const unsigned int finalPerm[64];

unsigned long	ftssl_des_algo(unsigned long keys[16], unsigned long input)
{
	int i;
	unsigned long left;
	unsigned long right;
	unsigned long tmp;
	
	input = ftssl_des_permute(input, 64, (unsigned int *)initPerm, 64);
	left = (input >> 32) & 0xffffffff;
	right = input & 0xffffffff;
	i = 0;
	while (i < 16)
	{
		tmp = left;
		left = right;
		right = des_feistel(right, keys[i]);
		right ^= tmp;
		i++;
	}
	input = (right << 32) | left;
	input = ftssl_des_permute(input, 64, (unsigned int *)finalPerm, 64);
	return (input);
}

/*
** The calls to ft_reverse_bytes are to account for little-endianess
*/

int ftssl_des_ecb(t_ftssl_args args, const unsigned char *input,
					unsigned char *output, int len)
{
	int				reslen;
	int				i;
	unsigned long	input_val;
	unsigned long	output_val;
	unsigned long	*keys;
	unsigned char	*tmp;
	int				checklen;

	if (args.mode == FTSSL_MODE_DEC)
		keys = ftssl_des_genkeys(args.keyval, 1);
	else
		keys = ftssl_des_genkeys(args.keyval, 0);
	i = 0;
	reslen = 0;
	checklen = (args.mode == FTSSL_MODE_DEC) ? len - 1 : len;
	while (i <= checklen)
	{
		if (len - i < FTSSL_BLCKSZ_DES && args.mode == FTSSL_MODE_ENC)
		{
			tmp = ftssl_padblock_ecb((unsigned char *)(input + i), (len - i), FTSSL_BLCKSZ_DES);
			ft_reverse_bytes((void *)tmp, 8);
			input_val = *(unsigned long *)tmp;
			free(tmp);
		}
		else
		{
			ft_reverse_bytes((void *)(input + i), 8);
			input_val = *(unsigned long *)(input + i);
		}
		output_val = ftssl_des_algo(keys, input_val);
		ft_memcpy(output + i, &output_val, FTSSL_BLCKSZ_DES);
		ft_reverse_bytes((void *)(output + i), 8);
		i += FTSSL_BLCKSZ_DES;
		reslen += FTSSL_BLCKSZ_DES;
	}
	if (args.mode == FTSSL_MODE_DEC)
		reslen -= *(output + i - 1);
	free(keys);
	return (reslen);
}

