/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:19:52 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/14 22:01:42 by rhallste         ###   ########.fr       */
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

static char		*get_block(int fd, int block_size)
{
	char	buffer[block_size + 1];
	int		ret;
	int		prog;

	prog = 0;
	ft_bzero(buffer, block_size + 1);
	while (prog < block_size &&
		   (ret = read(fd, buffer + prog, block_size - prog)))
		prog += ret;
	return ((prog > 0) ? ft_strdup(buffer) : NULL);
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

int			main(int argc, char **argv)
{
	char		*block;
	char		*output;
	int			input_fd;
	int			output_fd;
	t_flag_data	flag_data;
	
	if (argc < 2)
	{
		ft_printf("usage: ft_ssl command [command opts] [command args]\n");
		return (0);
	}
	flag_data = ft_ssl_get_flags(argc, argv);
	if (!(ft_ssl_check_flags(flag_data)))
	{
		ft_ssl_flag_error();
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
	if (flag_data.has_output_file)
	{
		output_fd = open(flag_data.output_file, O_WRONLY|O_CREAT, 0644);
		if (output_fd == -1)
		{
			file_open_error(flag_data.output_file, O_WRONLY|O_CREAT);
			close(input_fd);
			return (-1);
		}
	}
	else
		output_fd = STDOUT_FILENO;
	if (ft_strcmp(flag_data.command, "base64") == 0)
	{
		while ((block = get_block(input_fd, B64_BLOCKSIZE)))
		{
			output = base64_encode(block);
			free(block);
			block = NULL;
			ft_printf_fd(output_fd, "%s", output);
			free(output);
		}
		ft_printf_fd(output_fd, "\n");
	}
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
	return (0);
}
