/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 20:55:29 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 15:52:32 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "args.h"
#include "error.h"
#include <libft.h>

void	handle_file_errors(t_hash_args *args, char **argv)
{
	int		i;
	char	*command;

	command = ft_strtolow(ft_strdup(args->command));
	i = 0;
	while (i < args->error_count)
	{
		ft_printf_fd(STDERR_FILENO,
			"ft_ssl: %s: %s: No such file or directory\n",
			command, argv[args->error_indices[i]]);
		i++;
	}
	free(command);
}
