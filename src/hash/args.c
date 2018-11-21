/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/11 17:35:22 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 10:57:26 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <libft.h>
#include <ft_ssl.h>
#include "args.h"

static int				is_flag(const char *input)
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

static t_hash_args		*init_args(char *command)
{
	t_hash_args	*args;

	args = ft_memalloc(sizeof(t_hash_args));
	args->command = ft_strtolow(ft_strdup(command));
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

void					free_args(t_hash_args *args)
{
//should make it so that args->input_string gets freed here as well.
	free(args->command);
	if (args->fds)
		free(args->fds);
	if (args->filenames)
		ft_free_2d_array((void ***)&(args->filenames), args->fd_count);
	if (args->error_indices)
		free(args->error_indices);
	free(args);
}

void					ftssl_md5_free_args(t_ftssl_md5_args *args)
{
	free_args((t_hash_args *)args);
}

static t_hash_args		*args_handle_files(t_hash_args *args,
										int argc, char **argv, int i)
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
	return (args);
}

t_hash_args				*get_args(int argc, char **argv)
{
	t_hash_args	*args;
	int			i;

	args = init_args(argv[1]);
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
					"%s: option requires an argument -- s\n",
					args->command);
		}
	}
	return (args_handle_files(args, argc, argv, i - 1));
}

t_ftssl_md5_args		*ftssl_md5_get_args(int argc, char **argv)
{
	return ((t_ftssl_md5_args *)get_args(argc, argv));
}
