/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 20:24:18 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/16 21:18:52 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
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
	state.working[0] = state.state[0];
	state.working[1] = state.state[1];
	state.working[2] = state.state[2];
	state.working[3] = state.state[3];
	return (state);
}

static int		get_block(char *block, int fd)
{

}

static void	do_md5(t_hash_args *args, char *filename, int fd)
{
	char			block[BLOCK_SIZE + 1];
	uint64_t		file_len;
	int				read_len;
	t_md5_state		state;

	file_len = 0;
	state = init_state();
	while ((read_len = get_block(&block, fd)) > 0)
	{
		block[read_len] = '\0';
		if (args->print_input)
			ft_printf("%s", block);
		file_len = += read_len;
		//pad if necessary
		//add len block at end if necessary;
		md5_rounds(block, &state);
	}
	if (args->print_input)
		args->print_input = 0;
	//get output hash
	//do output printing
}

static void do_md5_string(t_hash_args *args)
{
	//str_name = ft_strjoinfree(ft_strjoin("\"", args->input_string), "\"", 1);
	//do_md5(args, str_name, (unsigned char *)args->input_string);
	//free(str_name);
}

void	ftssl_md5(char *command, int argc, char **argv)
{
	t_hash_args		*args;
	int				i;

	args = get_args(argc, argv);
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
