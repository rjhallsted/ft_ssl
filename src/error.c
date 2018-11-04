/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 14:12:25 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/03 22:32:36 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

void		ftssl_flag_arg_error(const char *command, const char *option)
{
	int e;

	e = STDERR_FILENO;
	ft_printf_fd(e, "%s: option '%s' requires an argument\n", command, option);
	if (command == FTSSL_B64_TXT)
	{
		ft_printf_fd(e, "Usage:\t./ft_ssl base64 [-ed] [-i in_file]");
		ft_printf_fd(e, "[-o out_file]\n");
		ft_printf_fd(e, "\t\t-e, --encode\tencodes input\n");
		ft_printf_fd(e, "\t\t-d, --decode\tdecodes input\n");
		ft_printf_fd(e, "\t\t-i, --input\tinput file (default: \"-\" ");
		ft_printf_fd(e, "for stdin)\n");
		ft_printf_fd(e, "\t\t-o, --output\toutput file (default: \"-\" ");
		ft_printf_fd(e, "for stdout)\n");
	}
	exit(EXIT_FAILURE);
}

void		ftssl_nocommand_error(void)
{
	ft_printf_fd(STDERR_FILENO, "usage: ft_ssl command [command opts] ");
	ft_printf_fd(STDERR_FILENO, "[command args]\n");
	exit(EXIT_FAILURE);
}

void		ftssl_invalid_command_error(const char *command)
{
	char *error_str;

	error_str = "ftssl: Error: '%s' is an invalid command.\n\n";
	ft_printf_fd(STDERR_FILENO, error_str, command);
	ft_printf_fd(STDERR_FILENO, "Standard commands:\n\n");
	ft_printf_fd(STDERR_FILENO, "Message Digest commands:\n\n");
	ft_printf_fd(STDERR_FILENO, "Cipher commands:\n");
	ft_printf_fd(STDERR_FILENO, "%s\n", FTSSL_MD5_TXT);
	ft_printf_fd(STDERR_FILENO, "%s\n", FTSSL_B64_TXT);
	ft_printf_fd(STDERR_FILENO, "%s\n", FTSSL_DES_TXT);
	ft_printf_fd(STDERR_FILENO, "%s\n", FTSSL_DESECB_TXT);
	ft_printf_fd(STDERR_FILENO, "%s\n", FTSSL_DESCBC_TXT);
	ft_printf_fd(STDERR_FILENO, "%s\n", FTSSL_DES3_TXT);
	ft_printf_fd(STDERR_FILENO, "%s\n", FTSSL_DES3ECB_TXT);
	ft_printf_fd(STDERR_FILENO, "%s\n", FTSSL_DES3CBC_TXT);
	exit(EXIT_FAILURE);
}

void		ftssl_file_open_error(const char *filename, int permissions)
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
	ft_printf_fd(STDERR_FILENO, "%s\n", message);
	exit(EXIT_FAILURE);
}

void		ftssl_invalid_hexkey_error(void)
{
	ft_printf_fd(STDERR_FILENO, "non-hex digit\ninvalid hex key value\n");
	exit(EXIT_FAILURE);
}
