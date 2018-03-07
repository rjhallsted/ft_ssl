/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/03 15:36:46 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/06 23:36:53 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

/*
** Note that rounds are generally indexed from 0
*/

extern const unsigned int g_expan_perm[48];
extern const unsigned int g_pbox_perm[32];

static unsigned long	des_feistel(unsigned long block, unsigned long key)
{
	block = ftssl_des_permute(block, 32, (unsigned int *)g_expan_perm, 48);
	block ^= key;
	block = ftssl_des_sbox_sub(block);
	block = ftssl_des_permute(block, 32, (unsigned int *)g_pbox_perm, 32);
	return (block);
}

extern const unsigned int g_init_perm[64];
extern const unsigned int g_final_perm[64];

static unsigned long	ftssl_des_algo(unsigned long keys[16],
									   unsigned long input)
{
	int						i;
	unsigned long			left;
	unsigned long			right;
	unsigned long			tmp;

	input = ftssl_des_permute(input, 64, (unsigned int *)g_init_perm, 64);
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
	input = ftssl_des_permute(input, 64, (unsigned int *)g_final_perm, 64);
	return (input);
}

/*
** The calls to ft_reverse_bytes are to account for little-endianess
*/

static unsigned long	get_inputval(t_ftssl_args args,
								const unsigned char *input,
								int len, int i)
{
	unsigned char *tmp;
	unsigned long input_val;

	if (len - i < FTSSL_BLCKSZ_DES && args.mode == FTSSL_MODE_ENC)
	{
		tmp = ftssl_padblock_ecb((unsigned char *)(input + i),
								(len - i), FTSSL_BLCKSZ_DES);
		ft_reverse_bytes((void *)tmp, 8);
		input_val = *(unsigned long *)tmp;
		free(tmp);
	}
	else
	{
		ft_reverse_bytes((void *)(input + i), 8);
		input_val = *(unsigned long *)(input + i);
	}
	return (input_val);
}

int						ftssl_des_ecb(t_ftssl_args args,
								const unsigned char *input,
								unsigned char *output, int len)
{
	int				reslen;
	unsigned long	input_val;
	unsigned long	output_val;
	unsigned long	*keys;
	int				checklen;

	if (args.mode == FTSSL_MODE_DEC)
		keys = ftssl_des_genkeys(args.keyval, 1);
	else
		keys = ftssl_des_genkeys(args.keyval, 0);
	reslen = 0;
	checklen = (args.mode == FTSSL_MODE_DEC) ? len - 1 : len;
	while (reslen <= checklen)
	{
		input_val = get_inputval(args, input, len, reslen);
		output_val = ftssl_des_algo(keys, input_val);
		ft_memcpy(output + reslen, &output_val, FTSSL_BLCKSZ_DES);
		ft_reverse_bytes((void *)(output + reslen), 8);
		reslen += FTSSL_BLCKSZ_DES;
	}
	if (args.mode == FTSSL_MODE_DEC)
		reslen -= *(output + reslen - 1);
	free(keys);
	return (reslen);
}

int						ftssl_des_cbc(t_ftssl_args args,
								const unsigned char *input,
								unsigned char *output, int len)
{
	int						reslen;
	unsigned long			input_val;
	unsigned long			output_val;
	unsigned long			*keys;
	int						checklen;
	
	if (args.mode == FTSSL_MODE_DEC)
		keys = ftssl_des_genkeys(args.keyval, 1);
	else
	{
		keys = ftssl_des_genkeys(args.keyval, 0);
		output_val = args.init_vector;
	}
	reslen = 0;
	checklen = (args.mode == FTSSL_MODE_DEC) ? len - 1 : len;
	while (reslen <= checklen)
	{
		input_val = get_inputval(args, input, len, reslen);
		if (args.mode == FTSSL_MODE_ENC)
			input_val ^= output_val;
		output_val = ftssl_des_algo(keys, input_val);
		ft_memcpy(output + reslen, &output_val, FTSSL_BLCKSZ_DES);
		ft_reverse_bytes((void *)(output + reslen), 8);
		reslen += FTSSL_BLCKSZ_DES;
	}
	if (args.mode == FTSSL_MODE_DEC)
		reslen -= *(output + reslen - 1);
	free(keys);
	return (reslen);
}
