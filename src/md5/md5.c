/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/03 22:29:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/12 17:13:08 by rhallste         ###   ########.fr       */
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

static unsigned char	*build_md5_output(unsigned int *chain)
{
	char	*output;
	char	*tmp;
	int		i;

	output = ft_strnew(32);
	i = 0;
	while (i < 4)
	{
		ft_reverse_bytes(chain + i, sizeof(unsigned int));
		tmp = ft_uitoa_base(chain[i], 16);
		tmp = ft_strjoinfree(ft_xstring('0', 8 - ft_strlen(tmp)), tmp, 3);
		ft_strncpy((char *)output + (i * 8), (char *)tmp, 8);
		free(tmp);
		i++;
	}
	return ((unsigned char *)output);
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
		i += 64;
	}
	free(order);
	free(chain_tmp);
	return (build_md5_output(chain));
}

static void				do_md5(t_ftssl_md5_args *args, char *filename,
								unsigned char *input)
{
	unsigned char		*output;
	unsigned char		*padded;
	size_t				len;

	if (args->print_input)
	{
		ft_printf("%s", (char *)input);
		args->print_input = 0;
	}
	len = ftssl_md5_pad_input(input, &padded);
	output = md5_algorithm(padded, len);
	free(padded);
	if (filename && !args->quiet_mode && !args->reverse_output)
		ft_printf("MD5 (%s) = ", filename);
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
