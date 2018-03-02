/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 14:12:25 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/01 23:42:12 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

void ftssl_flag_arg_error(const char *command, char *option)
{
	ft_printf_fd(stderr, "%s: option '%s' requires an argument\n");
	if (command == FTSSL_B64_TXT)
		ftssl_b64_usage();
	exit(EXIT_FAILURE);
}

void ftssl_b64_usage(void)
{
	int e = STDERR_FILENO;
	
	ft_printf_fd(e, "Usage:\t./ft_ssl base64 [-ed] [-i in_file] [-o out_file]\n");
	ft_printf_fd(e, "\t\t-e, --encode\tencodes input\n");	
	ft_printf_fd(e, "\t\t-d, --decode\tdecodes input\n");
	ft_printf_fd(e, "\t\t-i, --input\tinput file (default: \"-\" for stdin)\n");
	ft_printf_fd(e, "\t\t-o, --output\toutput file (default: \"-\" for stdout)\n");
}

void ftssl_nocommand_error(void)
{
	ft_printf("usage: ft_ssl command [command opts] [command args]\n");
}

void ftssl_file_open_error(char *filename, int permissions)
{
	char *message;
	char *action;

	if (permissions & O_CREAT)
		action = "create";
	else
		action = "open";
	ft_printf_fd(STDERR_FILENO, "Unable to %s '%s': ", action, filename);
	if (errno == EACCES)
		message = "Permission denied";
	else
		message = "No such file or directory";
	ft_printf_fd(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}

