/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/03 15:36:46 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/03 19:04:11 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

extern const unsigned int expanPerm[48];
extern const unsigned int pboxPerm[32];

static void swap_ul(unsigned int *a, unsigned int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

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

int	ftssl_des_encrypt(ftssl_args_t args, char *input, char *output, int len)
{
	int i;
	unsigned long left;
	unsigned long right;
	
	//init permutation
	//key permutation
	//split to left and right
	i = 0;
	while (i < 16)
		des_round(&left, &right, args.keyval, i++);
	//join left and right
	//final permutation
}

int	ftssl_des_decrypt(ftssl_args_t args, char *input, char *output, int len)
{

}

int ftssl_des_ecb(ftssl_args_t args, const unsigned char *input,
					char *output, int len)
{
	int		reslen;

	if (args.mode == FTSSL_MODE_DEC)
		reslen = ftssl_des_decrypt(args, input, output, len);
	else
		reslen = ftssl_des_encrypt(args, input, output, len);
	return (reslen);
}

/* int ftssl_des_cbc(ftssl_args_t args, const unsigned char *input, char *output, int len) */
/* { */

/* } */

