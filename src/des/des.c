/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/03 15:36:46 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/03 19:47:20 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

static void swap_ul(unsigned int *a, unsigned int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

extern const unsigned int expanPerm[48];
extern const unsigned int pboxPerm[32];

static void des_round(unsigned long *left, unsigned long *right,
					  unsigned long *key, int round)
{
	unsigned long compKey;
	
	compKey = ftssl_des_key_transform(key, round);
	*right = ftssl_des_permute(*right, expanPerm, 48);
	*right ^= compKey;
	*right = ftssl_des_sbox_sub(*right);
	*right = ftssl_des_permute(*right, pboxPerm, 32);
	*right ^= *left;
	swap_ul(left, right);
}

extern const unsigned int initPerm[64];
extern const unsigned int keyPerm[56];
extern const unsigned int finalPerm[64];

unsigned long	ftssl_des_algo(unsigned long keys[16], unsigned long input);
{
	int i;
	unsigned long left;
	unsigned long right;

	input = ftssl_des_permute(input, initPerm, 64);
	left = (input >> 32) & 0xffffffff;
	right = input & 0xffffffff;
	i = 0;
	while (i < 16)
		des_round(&left, &right, key[i], i++);
	input = left << 32;
	input |= right;
	input = ftssl_des_permute(input, finalPerm, 64);
	return (input);
}

int ftssl_des_ecb(ftssl_args_t args, const unsigned char *input,
					char *output, int len)
{
	int				reslen;
	unsigned long	input_val;
	unsigned long	output_val;
	char			output_str[9];
	unsigned long	*keys;

	input_val = (unsigned long)*input & 0xffffffffffffffff;
	if (args.mode == FTSSL_MODE_DEC)
	{
		keys = ftssl_des_genkeys(args.keyval, 1);
		output_val = ftssl_des_decrypt(keys, input_val);
	}
	else
	{
		keys = ftssl_des_genkeys(args.keyval, 0);
		output_val = ftssl_des_encrypt(keys, input_val);
	}
	free(keys);
	output_str = (char *)&output_val;
	output_str[8] = '\0';
	ft_strncpy(output, output_str, 9);
	return (reslen);
}

/* int ftssl_des_cbc(ftssl_args_t args, const unsigned char *input, char *output, int len) */
/* { */

/* } */

