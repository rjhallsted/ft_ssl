/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:19:52 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/03 23:14:33 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"


const t_ftssl_command g_command_list[] = {
	{FTSSL_MD5_TXT, NULL},
	{FTSSL_B64_TXT, ftssl_des_family_wrapper},
	{FTSSL_DES_TXT, ftssl_des_family_wrapper},
	{FTSSL_DESECB_TXT, ftssl_des_family_wrapper},
	{FTSSL_DESCBC_TXT, ftssl_des_family_wrapper},
	{FTSSL_DES3_TXT, ftssl_des_family_wrapper},
	{FTSSL_DES3ECB_TXT, ftssl_des_family_wrapper},
	{FTSSL_DES3CBC_TXT, ftssl_des_family_wrapper}
};


/*
** block_size is size in bytes of amount to pass each function
** E.g. base64 accepts multiples of 3 (24 bits)
** DES accepts 8 (64 bits)
*/

const t_ftssl_des_command g_des_command_list[] = {
	{"undefined", 0, NULL, FTSSL_KEYNO, FTSSL_IVNO, 0},
	{FTSSL_B64_TXT, FTSSL_BLCKSZ_B64, ftssl_base64, FTSSL_KEYNO,
		FTSSL_IVNO, 0},
	{FTSSL_DES_TXT, FTSSL_BLCKSZ_DES, ftssl_des_ecb, FTSSL_KEYYES,
		FTSSL_IVNO, 1},
	{FTSSL_DESECB_TXT, FTSSL_BLCKSZ_DES, ftssl_des_ecb, FTSSL_KEYYES,
		FTSSL_IVNO, 1},
	{FTSSL_DESCBC_TXT, FTSSL_BLCKSZ_DES, ftssl_des_cbc, FTSSL_KEYYES,
		FTSSL_IVYES, 1},
	{FTSSL_DES3_TXT, FTSSL_BLCKSZ_DES, ftssl_des3_cbc, FTSSL_KEYYES,
		FTSSL_IVYES, 3},
	{FTSSL_DES3ECB_TXT, FTSSL_BLCKSZ_DES, ftssl_des3_ecb, FTSSL_KEYYES,
		FTSSL_IVNO, 3},
	{FTSSL_DES3CBC_TXT, FTSSL_BLCKSZ_DES, ftssl_des3_cbc, FTSSL_KEYYES,
		FTSSL_IVYES, 3}
};

int						ftssl_find_comm_key(char *command_name)
{
	int i;
	int command_count;

	i = 1;
	command_count = sizeof(g_des_command_list) / sizeof(t_ftssl_des_command);
	while (i < command_count)
	{
		if (!ft_strcmp(g_des_command_list[i].name, command_name))
			return (i);
		i++;
	}
	return (0);
}

static int				do_func(t_ftssl_args args, unsigned char *in,
								size_t in_size, unsigned char **output)
{
	t_ftssl_des_command command;
	int				len;

	command = g_des_command_list[ftssl_find_comm_key(args.command)];
	len = ((in_size / command.blocksize) + 1) * command.blocksize;
	if (ft_strcmp(args.command, FTSSL_B64_TXT) == 0
		&& args.mode == FTSSL_MODE_ENC)
		len = len * 4 / 3;
	*output = ft_memalloc(len);
	len = command.func(args, in, *output, in_size);
	return (len);
}

static int				b64_wrap(t_ftssl_args *args, unsigned char **input,
					int in_len, unsigned char **output)
{
	char	*tmp;
	int		ret;

	tmp = args->command;
	args->command = ft_strdup(FTSSL_B64_TXT);
	ret = do_func(*args, *input, in_len, output);
	free(*input);
	*input = *output;
	free(args->command);
	args->command = tmp;
	return (ret);
}

static void				do_work(t_ftssl_args args, int input_fd, int output_fd)
{
	char			buffer[FTSSL_BUFFSIZE + 1];
	unsigned char	*input;
	unsigned char	*output;
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
	ftssl_prep_args(&args);
	if (args.base64_mode == FTSSL_B64ON && args.mode == FTSSL_MODE_DEC)
		prog = b64_wrap(&args, &input, prog, &output);
	ret = do_func(args, input, prog, &output);
	free(input);
	input = output;
	if (args.base64_mode == FTSSL_B64ON && args.mode == FTSSL_MODE_ENC)
		ret = b64_wrap(&args, &input, ret, &output);
	output = input;
	write(output_fd, output, ret);
	free(output);
}

void						ftssl_des_family_wrapper(char *command_name, int argc, char **argv)
{
	int				in_fd;
	int				out_fd;
	int				com_key;
	int				perms;
	t_ftssl_args	args;

	args = ftssl_get_args(argc, argv);
	if (!(com_key = ftssl_find_comm_key(command_name)))
		ftssl_invalid_command_error(command_name);
	in_fd = (args.input_file
		&& ft_strcmp(args.input_file, "-")) ? -5 : STDIN_FILENO;
	if (in_fd == -5 && (in_fd = open(args.input_file, O_RDONLY)) == -1)
		ftssl_file_open_error(args.input_file, O_RDONLY);
	out_fd = (args.output_file && ft_strcmp(args.output_file, "-")) ? -5 : 1;
	perms = O_WRONLY | O_CREAT | O_TRUNC;
	if (out_fd == -5 && (out_fd = open(args.output_file, perms, 0644)) == -1)
		ftssl_file_open_error(args.output_file, perms);
	do_work(args, in_fd, out_fd);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
}

static t_ftssl_comm_wrap	*get_wrapper_func(char *command_name)
{
	int	i;
	int	command_count;

	i = 0;
	command_count = sizeof(g_command_list) / sizeof(t_ftssl_command);
	while (i < command_count)
	{
		if (!ft_strcmp(command_name, g_command_list[i].name))
			return (g_command_list[i].wrapper_func);
		i++;
	}
	return (NULL);
}

int							main(int argc, char **argv)
{
	t_ftssl_comm_wrap *command;
	
	if (argc < 2)
		ftssl_nocommand_error();
	command = get_wrapper_func(argv[1]);
	if (command == NULL) {
		ftssl_invalid_command_error(argv[1]);
		return (-1);
	} else
		command(argv[1], argc, argv);
	return (0);
}
