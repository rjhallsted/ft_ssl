/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 20:24:18 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 12:41:22 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <ft_ssl.h>
#include <libft.h>
#include "args.h"
#include "error.h"
#include "md5.h"

static t_md5_state init_state(void)
{
	t_md5_state state;

	state.state[0] = 0x67452301;
	state.state[1] = 0xefcdab89;
	state.state[2] = 0x98badcfe;
	state.state[3] = 0x10325476;
	return (state);
}

static int		get_block(char *block, int fd)
{
	int r;
	int tmp;

	r = 0;
	while ((tmp = read(fd, block + r, BUFF_SIZE - r)) > 0)
		r += tmp;
	return (r);
}

static char		*build_hash(t_md5_state state)
{
	char	*output;
	char	*tmp;
	int		i;

	output = ft_strnew(32);
	i = 0;
	while (i < 4)
	{
		ft_reverse_bytes(state.state + i, sizeof(unsigned int));
		tmp = ft_uitoa_base(state.state[i], 16);
		tmp = ft_strjoinfree(ft_xstring('0', 8 - ft_strlen(tmp)), tmp, 3);
		ft_strncpy((char *)output + (i * 8), (char *)tmp, 8);
		free(tmp);
		i++;
	}
	return (output);
}

static void	md5_loop(t_hash_args *args, t_md5_state *state, int fd)
{
	char			block[BLOCK_SIZE + 1];
	uint64_t		file_len;
	int				rlen;

	block[BUFF_SIZE] = '\0';
	file_len = 0;
	while ((rlen = get_block(block, fd)) > 0)
	{
		if (args->print_input)
			ft_printf("%s", block);
		file_len += rlen;
		if (rlen != 64)
		{
			block[rlen] = (unsigned char)128;
			ft_bzero(block + rlen + 1, BLOCK_SIZE - rlen - 8);
			file_len *= 8;
			if (rlen < 56)
				ft_memcpy(block + 56, &file_len, 8);
		}
		md5_rounds((unsigned int *)block, state);
	}
	if (ft_memcmp(block + 56, &file_len, 8))
	{
		ft_bzero(block, 56);
		file_len *= 8;
		ft_memcpy(block + 56, &file_len, 8);
		md5_rounds((unsigned int *)block, state);
	}
	args->print_input = 0;
}

static void	md5_string_loop(t_hash_args *args, t_md5_state *state)
{
	char			*input_str;
	uint64_t		str_len;
	unsigned int	final_len;
	unsigned int	i;

	str_len = ft_strlen(args->input_string);
	final_len = ((str_len / 64) + 1) * 64;
	if (str_len % 64 >= 56)
		final_len += 64;
	input_str = ft_strnew(final_len);
	ft_strncpy(input_str, args->input_string, str_len);
	input_str[str_len] = -128;
	str_len *= 8;
	ft_memcpy(input_str + final_len - 8, &str_len, 8);
	i = 0;
	while (i < final_len)
	{
		md5_rounds((unsigned int *)(input_str + i), state);
		i += 64;
	}
}

static void	do_md5(t_hash_args *args, char *filename, int fd)
{
	t_md5_state		state;
	char			*output;
	unsigned int	file_len;

	file_len = 0;
	state = init_state();
	md5_loop(args, &state, fd);
	output = build_hash(state);
	if (filename && !args->quiet_mode && !args->reverse_output)
		ft_printf("MD5 (%s) = %s\n", filename, output);
	else if (filename && !args->quiet_mode && args->reverse_output)
		ft_printf("%s %s\n", output, filename);
	else
		ft_printf("%s\n", output);
	free(output);
}

static void do_md5_string(t_hash_args *args)
{
	t_md5_state		state;
	char			*output;
	unsigned int	file_len;

	file_len = 0;
	state = init_state();
	md5_string_loop(args, &state);
	output = build_hash(state);
	if (!args->quiet_mode && !args->reverse_output)
		ft_printf("MD5 (\"%s\") = %s\n", args->input_string, output);
	else if (!args->quiet_mode && args->reverse_output)
		ft_printf("%s \"%s\"\n", output, args->input_string);
	else
		ft_printf("%s\n", output);
	free(output);
}

void	ftssl_md5(char *command, int argc, char **argv)
{
	t_hash_args		*args;
	int				i;

	args = get_args(argc, argv);
	command = NULL;
	if (args->read_stdin)
		do_md5(args, NULL, STDIN_FILENO);
	if (args->string_mode && args->input_string)
		do_md5_string(args);
	i = 0;
	while (i < args->fd_count)
	{
		do_md5(args, args->filenames[i], args->fds[i]);
		i++;
	}
	handle_file_errors(args, argv);
	free_args(args);
}
