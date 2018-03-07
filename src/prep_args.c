/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 20:49:34 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/07 02:07:35 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../inc/libft/inc/libft.h"
#include "../inc/ft_ssl.h"

/*
** g_command_list is defined in main.c
*/
extern const t_ftssl_command g_command_list[];

static unsigned long	hextoul(char *keystr)
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

static char				*pad_input(char *input)
{
	int		padlen;
	char	*padding;

	padlen = 16 - ft_strlen(input);
	if (padlen > 0)
	{
		padding = ft_xstring('0', padlen);
		input = ft_strjoinfree(input, padding, 3);
	}
	return (input);
}

void					ftssl_prep_args(t_ftssl_args *args)
{
	t_ftssl_command	command;

	command = g_command_list[ftssl_find_comm_key(args->command)];
	if (command.need_key)
	{
		if (args->keystr == NULL)
			args->keystr = getpass("enter des key in hex: ");
		args->keystr = pad_input(args->keystr);
		args->keyval = hextoul(args->keystr);
	}
	if (command.need_iv)
	{
		if (args->iv_str == NULL)
			args->iv_str = getpass("enter initial vector: ");
		args->iv_str = pad_input(args->iv_str);
		args->init_vector = hextoul(args->iv_str);
	}
}
