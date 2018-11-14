/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/03 22:29:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/14 14:14:37 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

/*
**pad to 64 bits short of a multiple of 512 bits.
**(8 and 64 bytes, respectively)
*/

static void				init_arrays(unsigned int **chain,
									unsigned int **chain_tmp,
									unsigned int **order)
{
	*chain = ft_memalloc(sizeof(unsigned int) * 4);
	(*chain)[0] = 0x67452301;
	(*chain)[1] = 0xefcdab89;
	(*chain)[2] = 0x98badcfe;
	(*chain)[3] = 0x10325476;
	*chain_tmp = ft_memalloc(sizeof(unsigned int) * 4);
	(*chain_tmp)[0] = (*chain)[0];
	(*chain_tmp)[1] = (*chain)[1];
	(*chain_tmp)[2] = (*chain)[2];
	(*chain_tmp)[3] = (*chain)[3];
	*order = ft_memalloc(sizeof(unsigned int) * 4);
	(*order)[0] = 0;
	(*order)[1] = 1;
	(*order)[2] = 2;
	(*order)[3] = 3;
}

static unsigned char	*md5_algorithm(unsigned char *input,
										size_t input_len)
{
	unsigned int	*chain;
	unsigned int	*chain_tmp;
	unsigned int	*order;
	size_t			i;

	init_arrays(&chain, &chain_tmp, &order);
	i = 0;
	while (i < input_len)
	{
		ftssl_md5_rounds(chain_tmp, (unsigned int *)(input + i), order);
		chain[0] += chain_tmp[0];
		chain[1] += chain_tmp[1];
		chain[2] += chain_tmp[2];
		chain[3] += chain_tmp[3];
		chain_tmp[0] = chain[0];
		chain_tmp[1] = chain[1];
		chain_tmp[2] = chain[2];
		chain_tmp[3] = chain[3];
		i += 64;
	}
	free(order);
	free(chain_tmp);
	return (ftssl_return_hash_output(chain, 4, 1));
}

static unsigned char	*get_output(char *command, unsigned char *input,
									unsigned int sha_version)
{
	unsigned char	*output;
	unsigned char	*padded;
	size_t			len;

	if (ft_strcmp(command, FTSSL_MD5_TXT) == 0)
	{
		len = ftssl_md5_pad_input(input, &padded, 0);
		output = md5_algorithm(padded, len);
	}
	else if (ft_strcmp(command, FTSSL_SHA256_TXT) == 0
			|| ft_strcmp(command, FTSSL_SHA224_TXT) == 0)
	{
		len = ftssl_md5_pad_input(input, &padded, 1);
		output = ftssl_sha256_algorithm(padded, len, sha_version);
	}
	else
	{
		len = ftssl_md5_pad_input_512(input, &padded);
		output = ftssl_sha512_algorithm(padded, len, sha_version);
	}
	free(padded);
	return (output);
}

static void				do_md5(t_ftssl_md5_args *args, char *filename,
								unsigned char *input)
{
	unsigned char		*output;
	unsigned int		sha_version;

	if (args->print_input)
	{
		ft_printf("%s", (char *)input);
		args->print_input = 0;
	}
	if (ft_strequ(args->command, FTSSL_SHA224_TXT)
		|| ft_strequ(args->command, FTSSL_SHA512_TXT)
		|| ft_strequ(args->command, FTSSL_SHA256_TXT)
		|| ft_strequ(args->command, FTSSL_SHA384_TXT))
		sha_version = (unsigned int)ft_atoi(args->command + 3);
	else if (ft_strequ(args->command, FTSSL_SHA512_224_TXT))
		sha_version = 5224;
	else
		sha_version = 5256;
	output = get_output(args->command, input, sha_version);
	if (filename && !args->quiet_mode && !args->reverse_output)
		ft_printf("%s (%s) = ", ft_strtoup(args->command), filename);
	ft_printf("%s", output);
	if (filename && !args->quiet_mode && args->reverse_output)
		ft_printf(" %s", filename);
	ft_printf("\n");
	free(output);
}

void					ftssl_md5_outer(t_ftssl_md5_args *args)
{
	unsigned char		*input;
	char				*str_name;
	int					i;

	if (args->read_stdin)
		do_md5(args, NULL, (unsigned char *)ft_get_file_contents(STDIN_FILENO));
	if (args->string_mode && args->input_string)
	{
		str_name = ft_strjoinfree(
						ft_strjoin("\"", args->input_string), "\"", 1);
		do_md5(args, str_name, (unsigned char *)args->input_string);
		free(str_name);
	}
	i = -1;
	while (++i < args->fd_count)
	{
		input = (unsigned char *)ft_get_file_contents(args->fds[i]);
		if (args->filenames)
			do_md5(args, args->filenames[i], input);
		else
			do_md5(args, NULL, input);
	}
}
