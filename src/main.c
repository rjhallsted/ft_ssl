/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:19:52 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/15 16:09:58 by rhallste         ###   ########.fr       */
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

static int	get_block(int fd, unsigned char **block, int block_size)
{
	char	buffer[block_size + 1];
	int		ret;
	int		prog;

	prog = 0;
	ft_bzero(buffer, block_size + 1);
	while (prog < block_size &&
		   (ret = read(fd, buffer + prog, block_size - prog)))
		prog += ret;
	*block = (unsigned char *)ft_strnew(block_size);
	ft_memcpy(*block, buffer, block_size);
	return (prog);
}

static void file_open_error(char *filename, int permissions)
{
	char *message;
	char *action;

	if (permissions & O_CREAT)
		action = "create";
	else
		action = "open";
	ft_printf_fd(STDERR_FILENO, "Unable to %s '%s': ", action, filename);
	if (errno == EACCES)
		message = "Permission denied";
	else
		message = "No such file or directory";
	ft_printf_fd(STDERR_FILENO, "%s\n", message);
}

static void	do_blocks(t_flag_data flags, int in_fd, int out_fd)
{
	int 			blocksize;
	unsigned char	*block;
	char			*output;
	char			*(*filter)(const unsigned char *, int);
	int				len;

	//if (flags.command = B64_SSLCOM)
	//{
		blocksize = (flags.mode == DECRYPT_MODE) ? B64D_BLOCKSIZE : B64E_BLOCKSIZE;
		filter = (flags.mode == DECRYPT_MODE) ? &ft_ssl_base64_decode : &ft_ssl_base64_encode;
		//}
	block = NULL;
	while ((len = get_block(in_fd, &block, blocksize)))
	{
		output = filter(block, len);
		free(block);
		block = NULL;
		ft_printf_fd(out_fd, "%s", output);
		free(output);
	}
	if (flags.command == B64_SSLCOM && flags.mode != DECRYPT_MODE)
		ft_printf_fd(out_fd, "\n");
}

int			main(int argc, char **argv)
{
	int			input_fd;
	int			output_fd;
	t_flag_data	flag_data;
	
	if (argc < 2)
	{
		ft_ssl_nocommand_error();
		return (0);
	}
	flag_data = ft_ssl_get_flags(argc, argv);
	if (!(ft_ssl_check_flags(flag_data)))
	{
		if (flag_data.command == ERROR_SSLCOM)
			ft_ssl_nocommand_error();
		else
			ft_ssl_b64_flag_error();
		return (-1);
	}
	if (flag_data.has_input_file && ft_strcmp(flag_data.input_file, "-") != 0)
	{
		input_fd = open(flag_data.input_file, O_RDONLY);
		if (input_fd == -1)
		{
			file_open_error(flag_data.input_file, O_RDONLY);
			return (-1);
		}
	}
	else
		input_fd = STDIN_FILENO;
	if (flag_data.has_output_file && ft_strcmp(flag_data.output_file, "-") != 0)
	{
		output_fd = open(flag_data.output_file, O_WRONLY|O_CREAT|O_TRUNC, 0644);
		if (output_fd == -1)
		{
			file_open_error(flag_data.output_file, O_WRONLY|O_CREAT|O_TRUNC);
			close(input_fd);
			return (-1);
		}
	}
	else
		output_fd = STDOUT_FILENO;
	do_blocks(flag_data, input_fd, output_fd);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
	return (0);
}
