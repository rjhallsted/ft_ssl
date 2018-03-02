/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/01 23:25:39 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/02 12:01:36 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/libft/inc/libft.h"
#include "../inc/ft_ssl.h"

ftssl_opthelp_t ftssl_opthelp_init(int argc, char **argv, char flag,
						  char *label)
{
	ftssl_opthelp_t opt;

	opt.argc = argc;
	opt.argv = argv;
	opt.flag = flag;
	opt.label = (label) ? ft_strdup(label) : NULL;
	return (opt);
}

void ftssl_opthelp_destroy(ftssl_opthelp_t opt)
{
	if (opt.label)
		free(opt.label);
}

char *ftssl_find_optvalue(ftssl_opthelp_t opt, char *command)
{
	int		res;
	char	*value;
	char	flagstr[50];

	value = NULL;
	res = ft_findopt(opt.argc, opt.argv, opt.flag, value);
	if (res && !value)
	{
		flagstr[0] = opt.flag;
		flagstr[1] = '\0';
		ftssl_flag_arg_error(command, flagstr);
	}
	if (!res)
		res = ft_findopt_long(opt.argc, opt.argv, opt.label, value);
	if (res && !value)
	{
		ft_memset(flagstr, '-', 2);
		flagstr[2] = '\0';
		ft_strncat(flagstr, opt.label, 50);
		ftssl_flag_arg_error(command, flagstr);
	}
	return (value);
}
