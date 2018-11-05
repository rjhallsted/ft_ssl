/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/03 22:29:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/04 21:23:48 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

static int					is_flag(const char *input)
{
	int i;

	if (input[0] == '-') {
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

static t_ftssl_md5_args		*get_args(int argc, char **argv)
{
	t_ftssl_md5_args	*args;
	int					i;
	int					fd;
	
	args = ft_memalloc(sizeof(t_ftssl_md5_args));
	args->print_input = ft_findopt(argc, argv, 'p', NULL);
	args->quiet_mode = ft_findopt(argc, argv, 'q', NULL);
	args->reverse_output = ft_findopt(argc, argv, 'r', NULL);
	args->string_mode = ft_findopt(argc, argv, 's', &(args->input_string));

	args->input_fds = NULL;
	args->input_fd_count = 0;
	i = 2;
	while (i < argc)
	{
		if (!is_flag(argv[i]) && (fd = open(argv[i], O_RDONLY)) > 0) {
			args->input_fds = ft_memrealloc(args->input_fds, sizeof(int) * (args->input_fd_count + 1), sizeof(args->input_fds));
			args->input_fds[args->input_fd_count] = fd;
			args->input_fd_count++;
		} else
			ft_printf_fd(STDERR_FILENO, "%s\n", "md5: it: No such file or directory");
		i++;
	}
	if (args->input_fd_count == 0) {
		args->input_fds = ft_memalloc(sizeof(int));
		args->input_fds[0] = STDIN_FILENO;
		args->input_fd_count = 1;
	}
	return (args);
}

void						ftssl_md5_wrapper(char *command_name, int argc, char **argv)
{
	t_ftssl_md5_args *args;

	args = get_args(argc, argv);
	command_name = NULL;
	for (int i = 0; i < args->input_fd_count; i++)
		ft_printf("%s\n", args->input_fds[i]);
	//do algo
	//print output
}
