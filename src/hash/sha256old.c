/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256old.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:22:19 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 14:52:00 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include <libft.h>
#include <ft_ssl.h>

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

static const unsigned int	g_sha256_init[] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

static const unsigned int	g_sha224_init[] = {
	0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
	0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
};

static void			sha_init_arrays(unsigned int **chain,
								unsigned int **chain_tmp,
								unsigned int sha_version)
{
	int					i;
	const unsigned int	*init_vars;

	*chain = ft_memalloc(sizeof(unsigned int) * 8);
	if (sha_version == 256)
		init_vars = g_sha256_init;
	else
		init_vars = g_sha224_init;
	i = 0;
	while (i < 8)
	{
		(*chain)[i] = init_vars[i];
		i++;
	}
	*chain_tmp = ft_memalloc(sizeof(unsigned int) * 8);
}

static unsigned int	*generate_words(unsigned int *input)
{
	unsigned int	*w;
	int				i;

	w = ft_memalloc(sizeof(unsigned int) * 64);
	i = 0;
	while (i < 16)
	{
		w[i] = input[i];
		ft_reverse_bytes(w + i, sizeof(unsigned int));
		i++;
	}
	while (i < 64)
	{
		w[i] = SHA_SSIG1(w[i - 2]) + w[i - 7]
				+ SHA_SSIG0(w[i - 15]) + w[i - 16];
		i++;
	}
	return (w);
}

static void			sha256_hash_func(unsigned int *ct,
									unsigned int *words)
{
	unsigned int	tmp1;
	unsigned int	tmp2;
	int				i;

	i = 0;
	while (i < 64)
	{
		tmp1 = ct[7] + SHA_BSIG1(ct[4])
			+ SHA_CH(ct[4], ct[5], ct[6]) + g_sha256_k[i] + words[i];
		tmp2 = SHA_BSIG0(ct[0]) + SHA_MAJ(ct[0], ct[1], ct[2]);
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

unsigned char		*ftssl_sha256_algorithm(unsigned char *input,
											size_t input_len,
											unsigned int sha_version)
{
	unsigned int	*chain;
	unsigned int	*ct;
	unsigned int	*w;
	size_t			iter[2];
	int				pieces;

	sha_init_arrays(&chain, &ct, sha_version);
	iter[0] = 0;
	while (iter[0] < input_len)
	{
		iter[1] = 0;
		while (++iter[1] < 9)
			ct[iter[1] - 1] = chain[iter[1] - 1];
		w = generate_words((unsigned int *)(input + iter[0]));
		sha256_hash_func(ct, w);
		free(w);
		iter[1] = 0;
		while (++iter[1] < 9)
			chain[iter[1] - 1] += ct[iter[1] - 1];
		iter[0] += 64;
	}
	free(ct);
	pieces = (sha_version == 256) ? 8 : 7;
	return (ftssl_return_hash_output(chain, pieces, 0));
}
