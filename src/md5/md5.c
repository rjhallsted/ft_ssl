/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/03 22:29:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/03 23:26:28 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

static t_ftssl_md5_args		*get_args(int argc, char **argv)
{
	t_ftssl_md5_args *args;

	args = ft_memalloc(sizeof(t_ftssl_md5_args));
	args->print_input = ft_findopt(argc, argv, 'p', NULL);
	args->quiet_mode = ft_findopt(argc, argv, 'q', NULL);
	args->reverse_mode = ft_findopt(argc, argv, 'r', NULL);
	args->string_mode = ft_findopt(argc, argv, 's', &(args->input_string));

	//get filepaths
	//error for invalid filenames
	//if no file, use stdin
	//open it
}

void						ftssl_md5_wrapper(char *command_name, int argc, char **argv)
{
	t_ftssl_md5_args *args;

	args = get_args(argc, argv);
	//build args struct
	//do algo
	//print output
}
