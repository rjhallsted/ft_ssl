/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/03 15:36:46 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/05 21:40:27 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

static void swap_ul(unsigned long *a, unsigned long *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

extern const unsigned int expanPerm[48];
extern const unsigned int pboxPerm[32];

static void des_round(unsigned long *left, unsigned long *right,
					  unsigned long key)
{
	*right = ftssl_des_permute(*right, 32, (unsigned int *)expanPerm, 48);
	*right ^= key;
	*right = ftssl_des_sbox_sub(*right);
	*right = ftssl_des_permute(*right, 32, (unsigned int *)pboxPerm, 32);
	*right ^= *left;
	swap_ul(left, right);
}

extern const unsigned int initPerm[64];
extern const unsigned int keyPerm[56];
extern const unsigned int finalPerm[64];

unsigned long	ftssl_des_algo(unsigned long keys[16], unsigned long input)
{
	int i;
	unsigned long left;
	unsigned long right;

	input = ftssl_des_permute(input, 64, (unsigned int *)initPerm, 64);
	ft_printf("ip: %lx\n", input);
	left = (input >> 32) & 0xffffffff;
	right = input & 0xffffffff;
	i = 0;
	while (i < 16)
		des_round(&left, &right, keys[i++]);
	input = left << 32;
	input |= right;
	input = ftssl_des_permute(input, 64, (unsigned int *)finalPerm, 64);
	return (input);
}

int ftssl_des_ecb(ftssl_args_t args, const unsigned char *input,
					unsigned char *output, int len)
{
	int				reslen;
	int				i;
	unsigned long	input_val;
	unsigned long	output_val;
	unsigned long	*keys;
	/* unsigned char	*tmp; */

	if (args.mode == FTSSL_MODE_DEC)
		keys = ftssl_des_genkeys(args.keyval, 1);
	else
		keys = ftssl_des_genkeys(args.keyval, 0);
	i = 0;
	reslen = 0;
//	while (i <= len)
	while (i < len)
	{
		/* if (len - i < FTSSL_BLCKSZ_DES) */
		/* { */
		/* 	tmp = ftssl_padblock_ecb((unsigned char *)(input + i), (len - i), FTSSL_BLCKSZ_DES); */
		/* 	input_val = (unsigned long)*tmp & 0xffffffffffffffff; */
		/* 	free(tmp); */
		/* } */
		/* else */
//		ft_printf("in: %s\n", ft_ultoa_base(input + i, 2));
		for (int j = 0; j < 8; j++)
		{
			ft_printf("%hx (%p)\n", input[i + j], input + i + j);
		}
//		ft_memcpy(&input_val, input + i, FTSSL_BLCKSZ_DES);
//		ft_printf("%p\n", input + i);
		input_val = *(unsigned long *)(input + i);
		ft_printf("input: %lx\n", input_val);
		output_val = ftssl_des_algo(keys, input_val);
		ft_memcpy(output + i, &output_val, FTSSL_BLCKSZ_DES);
		i += FTSSL_BLCKSZ_DES;
		reslen += FTSSL_BLCKSZ_DES;
	}
//	if (args.mode == FTSSL_MODE_DEC)
//		reslen -= *(output - 1);
	free(keys);
	return (reslen);
}

