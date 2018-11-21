/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5rounds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 20:09:33 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 13:15:16 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.h"

static const unsigned int	g_md5_k[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

#define ROTL(x,by,ws) ((x << by) | (x >> (ws - by)))
#define R1F(a,b,c,d) a + ((b & c) | ((~b) & d))
#define R2F(a,b,c,d) a + ((b & d) | (c & (~d)))
#define R3F(a,b,c,d) a + (b ^ c ^ d)
#define R4F(a,b,c,d) a + (c ^ (b | (~d)))
#define R1(a,b,c,d,i,k,s) a = b + ROTL((R1F(a,b,c,d) + i + k), s, 32)
#define R2(a,b,c,d,i,k,s) a = b + ROTL((R2F(a,b,c,d) + i + k), s, 32)
#define R3(a,b,c,d,i,k,s) a = b + ROTL((R3F(a,b,c,d) + i + k), s, 32)
#define R4(a,b,c,d,i,k,s) a = b + ROTL((R4F(a,b,c,d) + i + k), s, 32)
#define SA state->working[0]
#define SB state->working[1]
#define SC state->working[2]
#define SD state->working[3]

static void	round1(const unsigned int *block, t_md5_state *state)
{
	R1(SA, SB, SC, SD, block[0], g_md5_k[0], 7);
	R1(SD, SA, SB, SC, block[1], g_md5_k[1], 12);
	R1(SC, SD, SA, SB, block[2], g_md5_k[2], 17);
	R1(SB, SC, SD, SA, block[3], g_md5_k[3], 22);
	R1(SA, SB, SC, SD, block[4], g_md5_k[4], 7);
	R1(SD, SA, SB, SC, block[5], g_md5_k[5], 12);
	R1(SC, SD, SA, SB, block[6], g_md5_k[6], 17);
	R1(SB, SC, SD, SA, block[7], g_md5_k[7], 22);
	R1(SA, SB, SC, SD, block[8], g_md5_k[8], 7);
	R1(SD, SA, SB, SC, block[9], g_md5_k[9], 12);
	R1(SC, SD, SA, SB, block[10], g_md5_k[10], 17);
	R1(SB, SC, SD, SA, block[11], g_md5_k[11], 22);
	R1(SA, SB, SC, SD, block[12], g_md5_k[12], 7);
	R1(SD, SA, SB, SC, block[13], g_md5_k[13], 12);
	R1(SC, SD, SA, SB, block[14], g_md5_k[14], 17);
	R1(SB, SC, SD, SA, block[15], g_md5_k[15], 22);
}

static void	round2(const unsigned int *block, t_md5_state *state)
{
	R2(SA, SB, SC, SD, block[1], g_md5_k[16], 5);
	R2(SD, SA, SB, SC, block[6], g_md5_k[17], 9);
	R2(SC, SD, SA, SB, block[11], g_md5_k[18], 14);
	R2(SB, SC, SD, SA, block[0], g_md5_k[19], 20);
	R2(SA, SB, SC, SD, block[5], g_md5_k[20], 5);
	R2(SD, SA, SB, SC, block[10], g_md5_k[21], 9);
	R2(SC, SD, SA, SB, block[15], g_md5_k[22], 14);
	R2(SB, SC, SD, SA, block[4], g_md5_k[23], 20);
	R2(SA, SB, SC, SD, block[9], g_md5_k[24], 5);
	R2(SD, SA, SB, SC, block[14], g_md5_k[25], 9);
	R2(SC, SD, SA, SB, block[3], g_md5_k[26], 14);
	R2(SB, SC, SD, SA, block[8], g_md5_k[27], 20);
	R2(SA, SB, SC, SD, block[13], g_md5_k[28], 5);
	R2(SD, SA, SB, SC, block[2], g_md5_k[29], 9);
	R2(SC, SD, SA, SB, block[7], g_md5_k[30], 14);
	R2(SB, SC, SD, SA, block[12], g_md5_k[31], 20);
}

static void	round3(const unsigned int *block, t_md5_state *state)
{
	R3(SA, SB, SC, SD, block[5], g_md5_k[32], 4);
	R3(SD, SA, SB, SC, block[8], g_md5_k[33], 11);
	R3(SC, SD, SA, SB, block[11], g_md5_k[34], 16);
	R3(SB, SC, SD, SA, block[14], g_md5_k[35], 23);
	R3(SA, SB, SC, SD, block[1], g_md5_k[36], 4);
	R3(SD, SA, SB, SC, block[4], g_md5_k[37], 11);
	R3(SC, SD, SA, SB, block[7], g_md5_k[38], 16);
	R3(SB, SC, SD, SA, block[10], g_md5_k[39], 23);
	R3(SA, SB, SC, SD, block[13], g_md5_k[40], 4);
	R3(SD, SA, SB, SC, block[0], g_md5_k[41], 11);
	R3(SC, SD, SA, SB, block[3], g_md5_k[42], 16);
	R3(SB, SC, SD, SA, block[6], g_md5_k[43], 23);
	R3(SA, SB, SC, SD, block[9], g_md5_k[44], 4);
	R3(SD, SA, SB, SC, block[12], g_md5_k[45], 11);
	R3(SC, SD, SA, SB, block[15], g_md5_k[46], 16);
	R3(SB, SC, SD, SA, block[2], g_md5_k[47], 23);
}

static void	round4(const unsigned int *block, t_md5_state *state)
{
	R4(SA, SB, SC, SD, block[0], g_md5_k[48], 6);
	R4(SD, SA, SB, SC, block[7], g_md5_k[49], 10);
	R4(SC, SD, SA, SB, block[14], g_md5_k[50], 15);
	R4(SB, SC, SD, SA, block[5], g_md5_k[51], 21);
	R4(SA, SB, SC, SD, block[12], g_md5_k[52], 6);
	R4(SD, SA, SB, SC, block[3], g_md5_k[53], 10);
	R4(SC, SD, SA, SB, block[10], g_md5_k[54], 15);
	R4(SB, SC, SD, SA, block[1], g_md5_k[55], 21);
	R4(SA, SB, SC, SD, block[8], g_md5_k[56], 6);
	R4(SD, SA, SB, SC, block[15], g_md5_k[57], 10);
	R4(SC, SD, SA, SB, block[6], g_md5_k[58], 15);
	R4(SB, SC, SD, SA, block[13], g_md5_k[59], 21);
	R4(SA, SB, SC, SD, block[4], g_md5_k[60], 6);
	R4(SD, SA, SB, SC, block[11], g_md5_k[61], 10);
	R4(SC, SD, SA, SB, block[2], g_md5_k[62], 15);
	R4(SB, SC, SD, SA, block[9], g_md5_k[63], 21);
}

void		md5_rounds(const unsigned int *block, t_md5_state *state)
{
	state->working[0] = state->state[0];
	state->working[1] = state->state[1];
	state->working[2] = state->state[2];
	state->working[3] = state->state[3];
	round1(block, state);
	round2(block, state);
	round3(block, state);
	round4(block, state);
	state->state[0] += state->working[0];
	state->state[1] += state->working[1];
	state->state[2] += state->working[2];
	state->state[3] += state->working[3];
}
