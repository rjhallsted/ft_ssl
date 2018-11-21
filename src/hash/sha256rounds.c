/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256rounds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 14:40:33 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 15:35:32 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "sha256.h"

static const unsigned int	g_sha256_k[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define SHR(x,by) (x >> by)
#define SHL(x,by) (x << by)
#define ROTR(x,by,w) ((x >> by) | (x << (w - by)))
#define SHA_CH(x,y,z) ((x & y) ^ ((~x) & z))
#define SHA_MAJ(x,y,z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA_BSIG0(x) (ROTR(x,2,32) ^ ROTR(x,13,32) ^ ROTR(x,22,32))
#define SHA_BSIG1(x) (ROTR(x,6,32) ^ ROTR(x,11,32) ^ ROTR(x,25,32))

#define SA state->working[0]
#define SB state->working[1]
#define SC state->working[2]
#define SD state->working[3]
#define SE state->working[4]
#define SF state->working[5]
#define SG state->working[6]
#define SH state->working[7]

void	sha256_rounds(t_sha256_state *state)
{
	unsigned int	tmp1;
	unsigned int	tmp2;
	int				i;

	i = -1;
	while (++i < 8)
		state->working[i] = state->state[i];
	i = -1;
	while (++i < 64)
	{
		tmp1 = SH + SHA_BSIG1(SE)
			+ SHA_CH(SE, SF, SG) + g_sha256_k[i] + state->words[i];
		tmp2 = SHA_BSIG0(SA) + SHA_MAJ(SA, SB, SC);
		SH = SG;
		SG = SF;
		SF = SE;
		SE = SD + tmp1;
		SD = SC;
		SC = SB;
		SB = SA;
		SA = tmp1 + tmp2;
	}
	i = -1;
	while (++i < 8)
		state->state[i] += state->working[i];
}
