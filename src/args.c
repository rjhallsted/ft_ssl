/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/14 20:20:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/07 02:11:09 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

static t_ftssl_args	init_args(void)
{
	t_ftssl_args	args;

	args.command = NULL;
	args.input_file = NULL;
	args.output_file = NULL;
	args.keystr = NULL;
	args.iv_str = NULL;
	args.keyval = 0;
	args.init_vector = 0;
	args.mode = FTSSL_MODE_ENC;
	args.base64_mode = FTSSL_B64OFF;
	return (args);
}

void				ftssl_destroy_args(t_ftssl_args args)
{
	if (args.command)
		free(args.command);
	if (args.input_file)
		free(args.input_file);
	if (args.output_file)
		free(args.output_file);
	if (args.keystr)
		free(args.keystr);
	if (args.iv_str)
		free(args.iv_str);
}

static void			set_args_with_val(t_ftssl_args *args, int argc, char **argv)
{
	t_ftssl_opthelp opt;

	opt = ftssl_opthelp_init(argc, argv, 'i', "input");
	args->input_file = ftssl_find_optvalue(opt, args->command);
	ftssl_opthelp_destroy(opt);
	opt = ftssl_opthelp_init(argc, argv, 'o', "output");
	args->output_file = ftssl_find_optvalue(opt, args->command);
	ftssl_opthelp_destroy(opt);
	opt = ftssl_opthelp_init(argc, argv, 'k', "key");
	args->keystr = ftssl_find_optvalue(opt, args->command);
	ftssl_opthelp_destroy(opt);
	opt = ftssl_opthelp_init(argc, argv, 'v', "init-vector");
	args->iv_str = ftssl_find_optvalue(opt, args->command);
	ftssl_opthelp_destroy(opt);
}

t_ftssl_args		ftssl_get_args(int argc, char **argv)
{
	t_ftssl_args	args;

	args = init_args();
	if (argc < 2)
		ftssl_nocommand_error();
	args.command = ft_strdup(argv[1]);
	if (ft_findopt(argc, argv, 'd', NULL)
		|| ft_findopt_long(argc, argv, "decode", NULL))
		args.mode = FTSSL_MODE_DEC;
	if (ft_findopt(argc, argv, 'a', NULL)
		|| ft_findopt_long(argc, argv, "base64", NULL))
		args.base64_mode = FTSSL_B64ON;
	set_args_with_val(&args, argc, argv);
	return (args);
}
