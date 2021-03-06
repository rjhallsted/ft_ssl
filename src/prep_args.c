/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 20:49:34 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/16 19:59:59 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <libft.h>
#include <ft_ssl.h>

/*
** g_command_list is defined in main.c
*/
extern const t_ftssl_des_command g_command_list[];

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

static char				*pad_input(char *input, int len)
{
	int		padlen;
	char	*padding;

	padlen = len - ft_strlen(input);
	if (padlen > 0)
	{
		padding = ft_xstring('0', padlen);
		input = ft_strjoinfree(input, padding, 3);
	}
	return (input);
}

void					ftssl_prep_args(t_ftssl_args *args)
{
	t_ftssl_des_command	command;

	command = g_command_list[ftssl_find_comm_key(args->command)];
	if (command.need_key)
	{
		if (args->keystr == NULL)
			args->keystr = getpass("enter des key in hex: ");
		args->keystr = pad_input(args->keystr, 16 * command.keys_needed);
		args->keyval = hextoul(args->keystr);
		if (command.keys_needed > 1)
			args->keyval2 = hextoul(args->keystr + 16);
		if (command.keys_needed > 2)
			args->keyval3 = hextoul(args->keystr + 32);
	}
	if (command.need_iv)
	{
		if (args->iv_str == NULL)
			args->iv_str = getpass("enter initial vector: ");
		args->iv_str = pad_input(args->iv_str, 16);
		args->init_vector = hextoul(args->iv_str);
	}
}
