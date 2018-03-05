/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/03 15:40:58 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/05 15:53:15 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

unsigned long ftssl_des_permute(unsigned long in, size_t in_size,
								unsigned int *tab, size_t tab_size)
{
	unsigned long	out;
	unsigned long	tmp;
	size_t			i;

	out = 0;
	i = 0;
	while (i < tab_size)
	{
		tmp = (in >> (in_size - tab[i])) & 1;
		tmp <<= (tab_size - 1) - i;
		out |= tmp;
		i++;
	}
	return (out);
}

extern const unsigned int sbox[8][64];

unsigned long ftssl_des_sbox_sub(unsigned long in)
{
	int i;
	int row;
	int tmp;
	int col;
	int out;

	out = 0;
	i = 0;
	while (i < 8)
	{
		tmp = (in >> ((7 - i) * 6)) & 0b111111;
		row = ((tmp >> 4) & 0b10) | (tmp & 1);
		col = (tmp >> 1) & 0b1111;
		tmp = sbox[i][(row * 16) + col];
		tmp <<= ((7 - i) * 4);
		out |= tmp;
		i++;
	}
	return (out);
}

static unsigned long circle_shift_left(unsigned long val, int rotby)
{
	unsigned long	tmp;
	unsigned long	mask;

	mask = (rotby == 2) ? 0b11: 0b1;
	tmp = mask << (28 - rotby);
	tmp &= val;
	tmp >>= 28 - rotby;
	mask = (rotby == 2) ? 0x3ffffff : 0x7ffffff;
	val &= mask;
	val <<= rotby;
	val |= tmp;
	return (val);	
}

extern const int keyShift[16];
extern const unsigned int compPerm[48];

/*
** Rounds are indexed from 0
*/

unsigned long ftssl_des_key_transform(unsigned long *key, int round)
{
	unsigned long	left;
	unsigned long	right;

	left = (*key >> 28) & 0xfffffffL;
	right = *key & 0xfffffffL;
	left = circle_shift_left(left, keyShift[round]);
	right = circle_shift_left(right, keyShift[round]);
	*key = left << 28;
	*key |= right;
	return (ftssl_des_permute(*key, 56, (unsigned int *)compPerm, 48));
}

extern const unsigned int keyPerm[56];

unsigned long *ftssl_des_genkeys(unsigned long key, int reverse)
{
	unsigned long *keys;
	int i;

	key = ftssl_des_permute(key, 64, (unsigned int *)keyPerm, 56);
	keys = ft_memalloc(sizeof(unsigned long) * 16);
	i = 0;
	while (i < 16)
	{
		if (reverse)
			keys[15 - i] = ftssl_des_key_transform(&key, i);
		else
			keys[i] = ftssl_des_key_transform(&key, i);
		i++;
	}
	return (keys);
}
