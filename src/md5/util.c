/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/11 19:27:56 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/14 11:30:35 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

size_t			ftssl_md5_pad_input(unsigned char *input,
							unsigned char **padded, int sha_mode)
{
	size_t		len;
	uint64_t	len_bits;
	size_t		pad_len;

	len = ft_strlen((char *)input);
	pad_len = 1;
	while ((len + pad_len) % 64 != 56)
		pad_len++;
	*padded = ft_memrealloc(input, len + pad_len + 8, len);
	(*padded)[len] = (unsigned char)128;
	ft_bzero(*padded + len + 1, pad_len - 1);
	len_bits = (uint64_t)len * 8;
	if (sha_mode)
		ft_reverse_bytes(&len_bits, sizeof(uint64_t));
	ft_memcpy(*padded + len + pad_len, &len_bits, 8);
	return (len + pad_len + 8);
}

size_t			ftssl_md5_pad_input_512(unsigned char *input,
							unsigned char **padded, int sha_mode)
{
	size_t		len;
	uint128_t	len_bits;
	size_t		pad_len;

	len = ft_strlen((char *)input);
	pad_len = 1;
	while ((len + pad_len) % 128 != 112)
		pad_len++;
	*padded = ft_memrealloc(input, len + pad_len + 16, len);
	(*padded)[len] = (unsigned char)128;
	len_bits = 1634
	ft_memcpy(*padded + len, 
	ft_bzero(*padded + len + 1, pad_len - 1);
	len_bits = (uint64_t)len * 8;
	if (sha_mode)
		ft_reverse_bytes(&len_bits, sizeof(uint64_t));
	ft_memcpy(*padded + len + pad_len, &len_bits, 8);
	return (len + pad_len + 8);
}

void			ftssl_md5_family_wrapper(char *command_name, int argc,
										char **argv)
{
	t_ftssl_md5_args	*args;
	int					i;

	args = ftssl_md5_get_args(argc, argv);
	ftssl_md5_outer(args);
	i = -1;
	while (++i < args->error_count)
		ft_printf_fd(STDERR_FILENO,
			"ft_ssl: %s: %s: No such file or directory\n",
			ft_strtolow(command_name), argv[args->error_indices[i]]);
	ftssl_md5_free_args(args);
}

unsigned char	*ftssl_return_hash_output(unsigned int *chain, int pieces,
											int reverse_bytes)
{
	char	*output;
	char	*tmp;
	int		i;

	output = ft_strnew(pieces * 8);
	i = 0;
	while (i < pieces)
	{
		if (reverse_bytes)
			ft_reverse_bytes(chain + i, sizeof(unsigned int));
		tmp = ft_uitoa_base(chain[i], 16);
		tmp = ft_strjoinfree(ft_xstring('0', 8 - ft_strlen(tmp)), tmp, 3);
		ft_strncpy((char *)output + (i * 8), (char *)tmp, 8);
		free(tmp);
		i++;
	}
	free(chain);
	return ((unsigned char *)output);
}

unsigned char	*ftssl_return_hash_output_512(uint128_t *chain, int pieces,
											int reverse_bytes)
{
	char	*output;
	char	*tmp;
	int		i;

	output = ft_strnew(pieces * 16);
	i = 0;
	while (i < pieces)
	{
		if (reverse_bytes)
			ft_reverse_bytes(chain + i, sizeof(uint128_t));
		tmp = ft_uintmaxtoa_base((uint128_t)chain[i], 16);
		tmp = ft_strjoinfree(ft_xstring('0', 16 - ft_strlen(tmp)), tmp, 3);
		ft_strncpy((char *)output + (i * 16), (char *)tmp, 16);
		free(tmp);
		i++;
	}
	free(chain);
	return ((unsigned char *)output);
}
