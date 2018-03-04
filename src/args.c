/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/14 20:20:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/04 15:07:47 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

static ftssl_args_t init_args(void)
{
	ftssl_args_t	args;

	args.command = NULL;
	args.input_file = NULL;
	args.output_file = NULL;
	args.keystr = NULL;
	args.keyval = 0;
	args.init_vector = 0;
	args.mode = FTSSL_MODE_ENC;
	args.base64_mode = FTSSL_B64OFF;
	return (args);
}

void ftssl_destroy_args(ftssl_args_t args)
{
	if (args.command)
		free(args.command);
	if (args.input_file)
		free(args.input_file);
	if (args.output_file)
		free(args.output_file);
	if (args.keystr)
		free(args.keystr);
}

ftssl_args_t ftssl_get_args(int argc, char **argv)
{
	ftssl_args_t	args;
	ftssl_opthelp_t opt;
	
	args = init_args();
	if (argc < 2)
		args.command = ft_strdup(FTSSL_ERR_TXT);
	args.command = ft_strdup(argv[1]);
	if (ft_strcmp(argv[1], FTSSL_B64_TXT) == 0)
		args.base64_mode = FTSSL_B64ON;
	if (ft_findopt(argc, argv, 'd', NULL)
		|| ft_findopt_long(argc, argv, "decode", NULL))
		args.mode = FTSSL_MODE_DEC;
	opt = ftssl_opthelp_init(argc, argv, 'i', "input");
	args.input_file = ftssl_find_optvalue(opt, args.command);
	ftssl_opthelp_destroy(opt);
	opt = ftssl_opthelp_init(argc, argv, 'o', "output");
	args.output_file = ftssl_find_optvalue(opt, args.command);
	ftssl_opthelp_destroy(opt);
	opt = ftssl_opthelp_init(argc, argv, 'k', "key");
	args.keystr = ftssl_find_optvalue(opt, args.command);
	ftssl_opthelp_destroy(opt);
	if (ft_findopt(argc, argv, 'a', NULL)
		|| ft_findopt_long(argc, argv, "base64", NULL))
		args.base64_mode = FTSSL_B64ON;
	return (args);
}
