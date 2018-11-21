/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:22:11 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 15:36:20 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <libft.h>
#include <ft_ssl.h>
#include "args.h"
#include "sha256.h"
#include "error.h"

static t_sha256_state	init_state(void)
{
	t_sha256_state state;

	state.state[0] = 0x6a09e667;
	state.state[1] = 0xbb67ae85;
	state.state[2] = 0x3c6ef372;
	state.state[3] = 0xa54ff53a;
	state.state[4] = 0x510e527f;
	state.state[5] = 0x9b05688c;
	state.state[6] = 0x1f83d9ab;
	state.state[7] = 0x5be0cd19;
	return (state);
}

static int			get_block(char *block, int fd)
{
	int r;
	int tmp;

	r = 0;
	while ((tmp = read(fd, block + r, BUFF_SIZE - r)) > 0)
		r += tmp;
	return (r);
}

# define SHR(x,by) (x >> by)
# define ROTR(x,by,w) ((x >> by) | (x << (w - by)))
# define SHA_SSIG0(x) (ROTR(x,7,32) ^ ROTR(x,18,32) ^ SHR(x,3))
# define SHA_SSIG1(x) (ROTR(x,17,32) ^ ROTR(x,19,32) ^ SHR(x,10))

static uint32_t		*generate_words(uint32_t *block)
{
	uint32_t		*w;
	int				i;

	w = ft_memalloc(sizeof(uint32_t) * 64);
	i = 0;
	while (i < 16)
	{
		w[i] = block[i];
		ft_reverse_bytes(w + i, sizeof(uint32_t));
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

static char			*build_hash(t_sha256_state state)
{
	char	*output;
	char	*tmp;
	int		i;

	output = ft_strnew(64);
	i = 0;
	while (i < 8)
	{
		tmp = ft_uitoa_base(state.state[i], 16);
		tmp = ft_strjoinfree(ft_xstring('0', 8 - ft_strlen(tmp)), tmp, 3);
		ft_strncpy((char *)output + (i * 8), (char *)tmp, 8);
		free(tmp);
		i++;
	}
	return (output);
}

static void			sha256_last_block(char *block, uint64_t file_len,
								t_sha256_state *state)
{
	ft_bzero(block, 56);
	file_len *= 8;
	ft_reverse_bytes(&file_len, 8);
	ft_memcpy(block + 56, &file_len, 8);
	state->words = generate_words((uint32_t *)block);
	sha256_rounds(state);
	free(state->words);
}

static void			sha256_loop(t_hash_args *args, t_sha256_state *state, int fd)
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
			ft_reverse_bytes(&file_len, 8);
			if (rlen < 56)
				ft_memcpy(block + 56, &file_len, 8);
		}
		state->words = generate_words((uint32_t *)block);
		sha256_rounds(state);
		free(state->words);
	}
	if (ft_memcmp(block + 56, &file_len, 8))
		sha256_last_block(block, file_len, state);
	args->print_input = 0;
}

static void			sha256_string_loop(t_hash_args *args, t_sha256_state *state)
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
	ft_reverse_bytes(&str_len, 8);
	ft_memcpy(input_str + final_len - 8, &str_len, 8);
	i = 0;
	while (i < final_len)
	{
		state->words = generate_words((uint32_t *)(input_str + i));
		sha256_rounds(state);
		free(state->words);
		i += 64;
	}
}

static void			do_sha256(t_hash_args *args, char *filename, int fd)
{
	t_sha256_state	state;
	char			*output;
	unsigned int	file_len;

	file_len = 0;
	state = init_state();
	sha256_loop(args, &state, fd);
	output = build_hash(state);
	if (filename && !args->quiet_mode && !args->reverse_output)
		ft_printf("MD5 (%s) = %s\n", filename, output);
	else if (filename && !args->quiet_mode && args->reverse_output)
		ft_printf("%s %s\n", output, filename);
	else
		ft_printf("%s\n", output);
	free(output);
}

static void			do_sha256_string(t_hash_args *args)
{
	t_sha256_state	state;
	char			*output;
	unsigned int	file_len;

	file_len = 0;
	state = init_state();
	sha256_string_loop(args, &state);
	output = build_hash(state);
	if (!args->quiet_mode && !args->reverse_output)
		ft_printf("MD5 (\"%s\") = %s\n", args->input_string, output);
	else if (!args->quiet_mode && args->reverse_output)
		ft_printf("%s \"%s\"\n", output, args->input_string);
	else
		ft_printf("%s\n", output);
	free(output);
}

void				ftssl_sha256(char *command, int argc, char **argv)
{
	t_hash_args		*args;
	int				i;

	args = get_args(argc, argv);
	command = NULL;
	if (args->read_stdin)
		do_sha256(args, NULL, STDIN_FILENO);
	if (args->string_mode && args->input_string)
		do_sha256_string(args);
	i = 0;
	while (i < args->fd_count)
	{
		do_sha256(args, args->filenames[i], args->fds[i]);
		i++;
	}
	handle_file_errors(args, argv);
	free_args(args);
}
