/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:19:52 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 15:25:46 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <libft.h>
#include <ft_ssl.h>
#include "hash/md5.h"
#include "hash/sha256.h"

const t_ftssl_command		g_command_list[] = {
	{FTSSL_MD5_TXT, ftssl_md5},
	{FTSSL_SHA256_TXT, ftssl_sha256},
	{FTSSL_SHA224_TXT, ftssl_md5_family_wrapper},
	{FTSSL_SHA512_TXT, ftssl_md5_family_wrapper},
	{FTSSL_SHA384_TXT, ftssl_md5_family_wrapper},
	{FTSSL_SHA512_224_TXT, ftssl_md5_family_wrapper},
	{FTSSL_SHA512_256_TXT, ftssl_md5_family_wrapper},
	{FTSSL_B64_TXT, ftssl_des_family_wrapper},
	{FTSSL_DES_TXT, ftssl_des_family_wrapper},
	{FTSSL_DESECB_TXT, ftssl_des_family_wrapper},
	{FTSSL_DESCBC_TXT, ftssl_des_family_wrapper},
	{FTSSL_DES3_TXT, ftssl_des_family_wrapper},
	{FTSSL_DES3ECB_TXT, ftssl_des_family_wrapper},
	{FTSSL_DES3CBC_TXT, ftssl_des_family_wrapper}
};

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
	if (command == NULL)
	{
		ftssl_invalid_command_error(argv[1]);
		return (-1);
	}
	else
		command(argv[1], argc, argv);
	return (0);
}
