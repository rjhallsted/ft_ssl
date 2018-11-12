/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/03 22:29:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/11 17:14:34 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

static const unsigned int		g_r1const[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821
};

static const unsigned int		g_r2const[] = {
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a
};

static const unsigned int		g_r3const[] = {
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665
};

static const unsigned int		g_r4const[] = {
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const unsigned int		g_r1shift[] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22
};

static const unsigned int		g_r2shift[] = {
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20
};

static const unsigned int		g_r3shift[] = {
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23
};

static const unsigned int		g_r4shift[] = {
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

static int					is_flag(const char *input)
{
	int i;

	if (input[0] == '-')
	{
		i = 1;
		while (input[i])
		{
			if (ft_strchr("pqrs", input[i]))
				return (1);
			i++;
		}
	}
	return (0);
}

static t_ftssl_md5_args		*init_args(void)
{
	t_ftssl_md5_args	*args;

	args = ft_memalloc(sizeof(t_ftssl_md5_args));
	args->print_input = 0;
	args->quiet_mode = 0;
	args->reverse_output = 0;
	args->string_mode = 0;
	args->input_string = NULL;
	args->fds = NULL;
	args->filenames = NULL;
	args->fd_count = 0;
	args->read_stdin = 0;
	args->error_indices = NULL;
	args->error_count = 0;
	return (args);
}

static void					free_args(t_ftssl_md5_args *args)
{
	if (args->input_string)
		free(args->input_string);
	if (args->fds)
		free(args->fds);
	if (args->filenames)
		ft_free_2d_array((void ***)&(args->filenames), sizeof(args->filenames));
	if (args->error_indices)
		free(args->error_indices);
	free(args);
}

static void					args_handle_files(t_ftssl_md5_args *args, int argc,
										char **argv, int i)
{
	int fd;

	while (++i < argc)
	{
		if ((fd = open(argv[i], O_RDONLY)) > 0)
		{
			args->fds = ft_memrealloc(args->fds,
					sizeof(int) * (args->fd_count + 1), sizeof(args->fds));
			args->fds[args->fd_count] = fd;
			args->filenames = ft_memrealloc(args->filenames,
				sizeof(char *) * (args->fd_count + 1),
				sizeof(args->filenames));
			args->filenames[args->fd_count++] = ft_strdup(argv[i]);
		}
		else
		{
			args->error_indices = ft_memrealloc(args->error_indices,
				sizeof(int) * (args->error_count + 1),
				sizeof(int) * args->error_count);
			args->error_indices[args->error_count++] = i;
		}
	}
	args->read_stdin = ((args->fd_count == 0 && !args->string_mode
						&& !args->error_count) || args->print_input);
}

static t_ftssl_md5_args		*get_args(int argc, char **argv)
{
	t_ftssl_md5_args	*args;
	int					i;

	args = init_args();
	i = 1;
	while (++i < argc && is_flag(argv[i]))
	{
		if (ft_strchr(argv[i], 'p'))
			args->print_input = 1;
		if (ft_strchr(argv[i], 'q'))
			args->quiet_mode = 1;
		if (ft_strchr(argv[i], 'r'))
			args->reverse_output = 1;
		if (ft_strchr(argv[i], 's'))
		{
			args->string_mode = 1;
			if (++i < argc)
				args->input_string = ft_strdup(argv[i]);
			else
				ft_printf_fd(STDERR_FILENO,
					"md5: option requires an argument -- s\n");
		}
	}
	args_handle_files(args, argc, argv, i - 1);
	return (args);
}

/*
**pad to 64 bits short of a multiple of 512 bits.
**(8 and 64 bytes, respectively)
*/

static size_t				pad_input(unsigned char *input,
								unsigned char **padded)
{
	size_t	len;
	int64_t len_bits;
	size_t	pad_len;

	len = ft_strlen((char *)input);
	pad_len = 0;
	while ((len + pad_len) % 64 != 56)
		pad_len++;
	*padded = ft_memrealloc(input, len + pad_len + 8, len);
	if (pad_len > 0)
	{
		(*padded)[len] = (unsigned char)128;
		ft_bzero(*padded + len + 1, pad_len - 1);
	}
	len_bits = ((long long)len * 8);
	ft_memcpy(*padded + len + pad_len, &len_bits, 8);
	return (len + pad_len + 8);
}

static unsigned int				shift(unsigned int value, unsigned int shiftby)
{
	unsigned int	shifted_off;

	shifted_off = (value >> (32 - shiftby));
	value = (value << shiftby) | shifted_off;
	return (value);
}

static void						rotate_order(unsigned int *order)
{
	order[3] ^= order[2];
	order[2] ^= order[3];
	order[3] ^= order[2];
	order[2] ^= order[1];
	order[1] ^= order[2];
	order[2] ^= order[1];
	order[1] ^= order[0];
	order[0] ^= order[1];
	order[1] ^= order[0];
}

static void						round1_func(unsigned int *c, unsigned int in,
										unsigned int *o, int i)
{
	c[o[0]] = c[o[1]] + shift(c[o[0]]
					+ ((c[o[1]] & c[o[2]]) | ((~c[o[1]]) & c[o[3]]))
					+ in + g_r1const[i], g_r1shift[i]);
}

static void						round2_func(unsigned int *c, unsigned int in,
										unsigned int *o, int i)
{
	c[o[0]] = c[o[1]] + shift(c[o[0]]
					+ ((c[o[1]] & c[o[3]]) | (c[o[2]] & (~c[o[3]])))
					+ in + g_r2const[i], g_r2shift[i]);
}

static void						round3_func(unsigned int *c, unsigned int in,
										unsigned int *o, int i)
{
	c[o[0]] = c[o[1]] + shift(c[o[0]]
					+ (c[o[1]] ^ c[o[2]] ^ c[o[3]])
					+ in + g_r3const[i], g_r3shift[i]);
}

static void						round4_func(unsigned int *c, unsigned int in,
										unsigned int *o, int i)
{
	c[o[0]] = c[o[1]] + shift(c[o[0]]
					+ (c[o[2]] ^ (c[o[1]] | (~c[o[3]])))
					+ in + g_r4const[i], g_r4shift[i]);
}

static void						rounds(unsigned int *c, unsigned int *in,
									unsigned int *order)
{
	int				i;

	i = -1;
	while (++i < 16)
	{
		round1_func(c, in[g_r1mindex[i]], order, i);
		rotate_order(order);
	}
	while (++i < 32)
	{
		round2_func(c, in[g_r2mindex[i - 16]], order, i - 16);
		rotate_order(order);
	}
	while (++i < 48)
	{
		round3_func(c, in[g_r3mindex[i - 32]], order, i - 32);
		rotate_order(order);
	}
	while (++i < 64)
	{
		round4_func(c, in[g_r4mindex[i - 48]], order, i - 48);
		rotate_order(order);
	}
}

static void						init_arrays(unsigned int **chain,
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

static unsigned char			*build_md5_output(unsigned int *chain)
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

static unsigned char			*md5_algorithm(unsigned char *input,
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
		rounds(chain_tmp, (unsigned int *)(input + i), order);
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

static void					do_md5(t_ftssl_md5_args *args, char *filename,
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
	len = pad_input(input, &padded);
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

static void					do_md5_outer(t_ftssl_md5_args *args)
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

void						ftssl_md5_wrapper(char *command_name,
											int argc, char **argv)
{
	t_ftssl_md5_args	*args;
	int					i;

	args = get_args(argc, argv);
	command_name = NULL;
	do_md5_outer(args);
	i = -1;
	while (++i < args->error_count)
		ft_printf_fd(STDERR_FILENO, "ft_ssl: md5: %s%s\n",
			argv[args->error_indices[i]], ": No such file or directory");
	free_args(args);
}
