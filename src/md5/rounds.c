/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/11 17:41:00 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/14 13:08:45 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_ssl.h"

static const unsigned int	g_r1const[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821
};

static const unsigned int	g_r2const[] = {
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a
};

static const unsigned int	g_r3const[] = {
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665
};

static const unsigned int	g_r4const[] = {
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const unsigned int	g_r1shift[] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22
};

static const unsigned int	g_r2shift[] = {
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20
};

static const unsigned int	g_r3shift[] = {
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23
};

static const unsigned int	g_r4shift[] = {
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static const unsigned int		g_r1mindex[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

static const unsigned int		g_r2mindex[] = {
	1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12
};

static const unsigned int		g_r3mindex[] = {
	5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2
};

static const unsigned int		g_r4mindex[] = {
	0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9
};

static void					round1_func(unsigned int *c, unsigned int in,
										unsigned int *o, int i)
{
	unsigned int val;

	val = c[o[0]] + ((c[o[1]] & c[o[2]]) | ((~c[o[1]]) & c[o[3]]));
	val += in + g_r1const[i];
	val = ROTL(val, g_r1shift[i], 32);
	c[o[0]] = c[o[1]] + val;
	o[3] ^= o[2];
	o[2] ^= o[3];
	o[3] ^= o[2];
	o[2] ^= o[1];
	o[1] ^= o[2];
	o[2] ^= o[1];
	o[1] ^= o[0];
	o[0] ^= o[1];
	o[1] ^= o[0];
}

static void					round2_func(unsigned int *c, unsigned int in,
										unsigned int *o, int i)
{
	unsigned int val;

	val = c[o[0]] + ((c[o[1]] & c[o[3]]) | (c[o[2]] & (~c[o[3]])));
	val += in + g_r2const[i];
	val = ROTL(val, g_r2shift[i], 32);
	c[o[0]] = c[o[1]] + val;
	o[3] ^= o[2];
	o[2] ^= o[3];
	o[3] ^= o[2];
	o[2] ^= o[1];
	o[1] ^= o[2];
	o[2] ^= o[1];
	o[1] ^= o[0];
	o[0] ^= o[1];
	o[1] ^= o[0];
}

static void					round3_func(unsigned int *c, unsigned int in,
										unsigned int *o, int i)
{
	unsigned int val;

	val = c[o[0]] + (c[o[1]] ^ c[o[2]] ^ c[o[3]]);
	val += in + g_r3const[i];
	val = ROTL(val, g_r3shift[i], 32);
	c[o[0]] = c[o[1]] + val;
	o[3] ^= o[2];
	o[2] ^= o[3];
	o[3] ^= o[2];
	o[2] ^= o[1];
	o[1] ^= o[2];
	o[2] ^= o[1];
	o[1] ^= o[0];
	o[0] ^= o[1];
	o[1] ^= o[0];
}

static void					round4_func(unsigned int *c, unsigned int in,
										unsigned int *o, int i)
{
	unsigned int val;

	val = c[o[0]] + (c[o[2]] ^ (c[o[1]] | (~c[o[3]])));
	val += in + g_r4const[i];
	val = ROTL(val, g_r4shift[i], 32);
	c[o[0]] = c[o[1]] + val;
	o[3] ^= o[2];
	o[2] ^= o[3];
	o[3] ^= o[2];
	o[2] ^= o[1];
	o[1] ^= o[2];
	o[2] ^= o[1];
	o[1] ^= o[0];
	o[0] ^= o[1];
	o[1] ^= o[0];
}

void						ftssl_md5_rounds(unsigned int *c, unsigned int *in,
									unsigned int *order)
{
	int				i;

	i = 0;
	while (i < 16)
	{
		round1_func(c, in[g_r1mindex[i]], order, i);
		i++;
	}
	while (i < 32)
	{
		round2_func(c, in[g_r2mindex[i - 16]], order, i - 16);
		i++;
	}
	while (i < 48)
	{
		round3_func(c, in[g_r3mindex[i - 32]], order, i - 32);
		i++;
	}
	while (i < 64)
	{
		round4_func(c, in[g_r4mindex[i - 48]], order, i - 48);
		i++;
	}
}
