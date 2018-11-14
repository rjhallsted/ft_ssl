/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 11:03:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/14 13:04:31 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdlib.h>
#include <stdint.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

static const uint64_t	g_sha512_k[] = {
	0x428a2f98d728ae22, 0x7137449123ef65cd,
	0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
	0x3956c25bf348b538, 0x59f111f1b605d019,
	0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
	0xd807aa98a3030242, 0x12835b0145706fbe,
	0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
	0x72be5d74f27b896f, 0x80deb1fe3b1696b1,
	0x9bdc06a725c71235, 0xc19bf174cf692694,
	0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
	0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
	0x2de92c6f592b0275, 0x4a7484aa6ea6e483,
	0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
	0x983e5152ee66dfab, 0xa831c66d2db43210,
	0xb00327c898fb213f, 0xbf597fc7beef0ee4,
	0xc6e00bf33da88fc2, 0xd5a79147930aa725,
	0x06ca6351e003826f, 0x142929670a0e6e70,
	0x27b70a8546d22ffc, 0x2e1b21385c26c926,
	0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
	0x650a73548baf63de, 0x766a0abb3c77b2a8,
	0x81c2c92e47edaee6, 0x92722c851482353b,
	0xa2bfe8a14cf10364, 0xa81a664bbc423001,
	0xc24b8b70d0f89791, 0xc76c51a30654be30,
	0xd192e819d6ef5218, 0xd69906245565a910,
	0xf40e35855771202a, 0x106aa07032bbd1b8,
	0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
	0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
	0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
	0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
	0x748f82ee5defb2fc, 0x78a5636f43172f60,
	0x84c87814a1f0ab72, 0x8cc702081a6439ec,
	0x90befffa23631e28, 0xa4506cebde82bde9,
	0xbef9a3f7b2c67915, 0xc67178f2e372532b,
	0xca273eceea26619c, 0xd186b8c721c0c207,
	0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
	0x06f067aa72176fba, 0x0a637dc5a2c898a6,
	0x113f9804bef90dae, 0x1b710b35131c471b,
	0x28db77f523047d84, 0x32caab7b40c72493,
	0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
	0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
	0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

static const uint64_t	g_sha512_init[] = {
	0x6a09e667f3bcc908, 0xbb67ae8584caa73b,
	0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
	0x510e527fade682d1, 0x9b05688c2b3e6c1f,
	0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
};

static void			sha_init_arrays(uint64_t **chain,
								uint64_t **chain_tmp,
								uint64_t sha_version)
{
	int				i;
	const uint64_t	*init_vars;

	*chain = ft_memalloc(sizeof(uint64_t) * 8);
	if (sha_version == 512)
		init_vars = g_sha512_init;
	i = 0;
	while (i < 8)
	{
		(*chain)[i] = init_vars[i];
		i++;
	}
	*chain_tmp = ft_memalloc(sizeof(uint64_t) * 8);
}

static uint64_t	*generate_words(uint64_t *input)
{
	uint64_t	*w;
	int			i;

	w = ft_memalloc(sizeof(uint64_t) * 64);
	i = 0;
	while (i < 16)
	{
		w[i] = input[i];
		ft_reverse_bytes(w + i, sizeof(uint64_t));
		i++;
	}
	while (i < 80)
	{
		w[i] = SHA512_SSIG1(w[i - 2]) + w[i - 7]
				+ SHA512_SSIG0(w[i - 15]) + w[i - 16];
		i++;
	}
	return (w);
}

static void			sha512_hash_func(uint64_t *ct,
									uint64_t *words)
{
	uint64_t	tmp1;
	uint64_t	tmp2;
	int				i;

	i = 0;
	while (i < 80)
	{
		tmp1 = ct[7] + SHA512_BSIG1(ct[4])
			+ SHA_CH(ct[4], ct[5], ct[6]) + g_sha512_k[i] + words[i];
		tmp2 = SHA512_BSIG0(ct[0]) + SHA_MAJ(ct[0], ct[1], ct[2]);
		ct[7] = ct[6];
		ct[6] = ct[5];
		ct[5] = ct[4];
		ct[4] = ct[3] + tmp1;
		ct[3] = ct[2];
		ct[2] = ct[1];
		ct[1] = ct[0];
		ct[0] = tmp1 + tmp2;
		i++;
	}
}

/*
**Accepts 256 or 224 for sha_version
*/

unsigned char		*ftssl_sha512_algorithm(unsigned char *input,
											size_t input_len,
											unsigned int sha_version)
{
	uint64_t	*chain;
	uint64_t	*ct;
	uint64_t	*w;
	size_t		iter[2];
	int			pieces;

	sha_init_arrays(&chain, &ct, sha_version);
	iter[0] = 0;
	while (iter[0] < input_len)
	{
		iter[1] = 0;
		while (++iter[1] < 9)
			ct[iter[1] - 1] = chain[iter[1] - 1];
		w = generate_words((uint64_t *)(input + iter[0]));
		sha512_hash_func(ct, w);
		free(w);
		iter[1] = 0;
		while (++iter[1] < 9)
			chain[iter[1] - 1] += ct[iter[1] - 1];
		iter[0] += 128;
	}
	free(ct);
	pieces = 8;
	return (ftssl_return_hash_output_512(chain, pieces, 0));
}
