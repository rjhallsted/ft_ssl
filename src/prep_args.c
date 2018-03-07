/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 20:49:34 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/06 20:57:54 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../inc/libft/inc/libft.h"
#include "../inc/ft_ssl.h"

/*
** g_command_list is defined in main.c
*/
extern const t_ftssl_command g_command_list[];

static unsigned long	key_strtoul(char *keystr)
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

void					ftssl_prep_args(t_ftssl_args *args)
{
	t_ftssl_command	command;
	char			*padding;
	int				padlen;

	command = g_command_list[ftssl_find_comm_key(args->command)];
	if (command.need_key)
	{
		if (args->keystr == NULL)
			args->keystr = getpass("enter des key in hex: ");
		padlen = 16 - ft_strlen(args->keystr);
		if (padlen > 0)
		{
			padding = ft_xstring('0', padlen);
			args->keystr = ft_strjoinfree(args->keystr, padding, 3);
		}
		args->keyval = key_strtoul(args->keystr);
	}
}
