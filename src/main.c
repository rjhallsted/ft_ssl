/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:19:52 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/04 15:35:00 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

/*
 * block_size is size in bytes of amount to pass each function
 * E.g. base64 accepts multiples of 3 (24 bits)
 * DES accepts 8 (64 bits)
 */

static int	stop_condition(unsigned char *block, ftssl_args_t args)
{
	static int stop_next_flag;

	if (stop_next_flag)
		return (1);
	if (args.base64_mode == FTSSL_B64ON && args.mode == FTSSL_MODE_DEC
			&&ft_strchr((char *)block, '='))
		stop_next_flag = 1;
	return (0);
}

static int	get_block(int fd, unsigned char *block, int block_size)
{
	int		ret;
	int		prog;

	prog = 0;
	while (prog < block_size &&
		   (ret = read(fd, block + prog, block_size - prog)))
		prog += ret;
	return (prog);
}

const ftssl_command_t commandList[] = {
	{"undefined", 0, NULL, NULL},
	{FTSSL_B64_TXT, FTSSL_BLCKSZ_B64, NULL, ftssl_base64},
	{FTSSL_DES_TXT, FTSSL_BLCKSZ_DES, ftssl_padblock_ecb, ftssl_des_ecb},
	{FTSSL_DESECB_TXT, FTSSL_BLCKSZ_DES, ftssl_padblock_ecb, ftssl_des_ecb}
};

static void	do_blocks(ftssl_args_t args, int commKey, int in_fd, int out_fd)
{
	unsigned char		*block;
	char				*output;
	int					len;
	ftssl_command_t		command;
	int					padded;

	command = commandList[commKey];
	block = ft_memalloc(command.blocksize);
	output = ft_memalloc(command.blocksize);
	padded = 0;
	while ((len = get_block(in_fd, block, command.blocksize))
		&& !stop_condition(block, args))
	{
		if (len < (int)command.blocksize && command.padFunc)
		{
			command.padFunc(block, len, command.blocksize);
			padded = 1;
		}
		len = command.func(args, block, output, len);
		write(out_fd, output, len);
		ft_bzero(block, command.blocksize);
		ft_bzero(block, command.blocksize);
	}
	if (!padded && command.padFunc)
	{
		command.padFunc(block, 0, command.blocksize);
		len = command.func(args, block, output, len);
		write(out_fd, output, len);
	}
	free(block);
	free(output);
	if (args.base64_mode == FTSSL_B64ON && args.mode == FTSSL_MODE_ENC)
		ft_printf_fd(out_fd, "\n");
}

static int	find_commandKey(char *commandName)
{
	int i;
	int command_count;

	i = 1;
	command_count = sizeof(commandList) / sizeof(ftssl_command_t);
	while (i < command_count)
	{
		if (!ft_strcmp(commandList[i].name, commandName))
			return (i);
	}
	return (0);
}

int			main(int argc, char **argv)
{
	int				input_fd;
	int				output_fd;
	int				com_key;
	ftssl_args_t	args;
	
	if (argc < 2)
	{
		ftssl_nocommand_error();
		return (0);
	}
	args = ftssl_get_args(argc, argv);
	if (!(com_key = find_commandKey(args.command)))
		ftssl_invalid_command_error(args.command);
	if (args.input_file && ft_strcmp(args.input_file, "-"))
	{
		if ((input_fd = open(args.input_file, O_RDONLY)) == -1)
			ftssl_file_open_error(args.input_file, O_RDONLY);
	}
	else
		input_fd = STDIN_FILENO;
	if (args.output_file && ft_strcmp(args.output_file, "-"))
	{
		if ((output_fd = open(args.output_file, O_RDONLY)) == -1)
			ftssl_file_open_error(args.output_file, O_RDONLY);
	}
	else
		output_fd = STDOUT_FILENO;
	do_blocks(args, com_key, input_fd, output_fd);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
	return (0);
}
