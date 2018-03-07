/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:19:52 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/06 19:12:54 by rhallste         ###   ########.fr       */
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

const ftssl_command_t commandList[] = {
	{"undefined", 0, NULL, FTSSL_KEYNO},
	{FTSSL_B64_TXT, FTSSL_BLCKSZ_B64, ftssl_base64, FTSSL_KEYNO},
	{FTSSL_DES_TXT, FTSSL_BLCKSZ_DES, ftssl_des_ecb, FTSSL_KEYYES},
	{FTSSL_DESECB_TXT, FTSSL_BLCKSZ_DES, ftssl_des_ecb, FTSSL_KEYYES}
};

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
		i++;
	}
	return (0);
}

static int do_func(ftssl_args_t args, unsigned char *in, size_t in_size,
						unsigned char **output)
{
	ftssl_command_t command;
	int				len;

	command = commandList[find_commandKey(args.command)];
	len = ((in_size / command.blocksize) + 1) * command.blocksize;
	*output = ft_memalloc(len);
	len = command.func(args, in, *output, in_size);
	return (len);
}

static unsigned long key_strtoul(char *keystr)
{
	unsigned long	keyval;
	int				i;
	char			*pos;
	char			*hexkeys;

	hexkeys = "0123456789abcdef";
	ft_strtolow(keystr);
	i = 0;
	keyval = 0;
	while (i < 16)
	{
		keyval <<= 4;
		if (!(pos = ft_strchr(hexkeys, keystr[i])))
			ftssl_invalid_hexkey_error();
		keyval |= (pos - hexkeys);
		i++;
	}
	return (keyval);
}

static void prep_args(ftssl_args_t *args)
{
	ftssl_command_t	command;
	int				padlen;
	char			*padding;
	
	command = commandList[find_commandKey(args->command)];
	if (command.need_key)
	{
		if (args->keystr == NULL)
			args->keystr = getpass("enter des key in hex: ");
		padlen = 16 - ft_strlen(args->keystr);
		padding = ft_xstring('0', padlen);
		args->keystr = ft_strjoinfree(args->keystr, padding, 3);
		args->keyval = key_strtoul(args->keystr);
	}
}

static int	b64_wrap(ftssl_args_t *args, unsigned char **input,
					int in_len, unsigned char **output)
{
	char	*tmp;
	int		ret;
	
	tmp = ft_strdup(args->command);
	free(args->command);
	args->command = ft_strdup(FTSSL_B64_TXT);
	ret = do_func(*args, *input, in_len, output);
	free(*input);
	*input = *output;
	free(args->command);
	args->command = tmp;
	return (ret);
}

static void	do_work(ftssl_args_t args, int input_fd, int output_fd)
{
	char			buffer[FTSSL_BUFFSIZE];
	unsigned char	*input;
	unsigned char 	*output;
	int				ret;
	int				prog;

	prog = 0;
	input = NULL;
	while ((ret = read(input_fd, buffer, FTSSL_BUFFSIZE)) > 0)
	{
		input = ft_memrealloc(input, prog + ret, prog);
		ft_memcpy(input + prog, buffer, ret);
		prog += ret;
	}
	prep_args(&args);
	if (args.base64_mode == FTSSL_B64ON && args.mode == FTSSL_MODE_DEC)
		prog = b64_wrap(&args, &input, prog, &output);
	ret = do_func(args, input, prog, &output);
	free(input);
	if (args.base64_mode == FTSSL_B64ON && args.mode == FTSSL_MODE_ENC)
	{
		input = output;
		ret = b64_wrap(&args, &input, ret, &output);
		output = input;
	}
	write(output_fd, output, ret);
	free(output);
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
		if ((output_fd = open(args.output_file, O_WRONLY | O_CREAT, 0644)) == -1)
			ftssl_file_open_error(args.output_file, O_WRONLY | O_CREAT);
	}
	else
		output_fd = STDOUT_FILENO;
	do_work(args, input_fd, output_fd);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
	return (0);
}
