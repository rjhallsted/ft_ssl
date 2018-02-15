/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/14 20:20:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/15 11:55:54 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

static t_flag_data	init_flag_data(void)
{
	t_flag_data data;

	data.command = ERROR_SSLCOM;
	data.mode = ENCRYPT_MODE;
	data.has_input_file = 0;
	data.input_file = NULL;
	data.has_output_file = 0;
	data.output_file = NULL;
	return (data);
}

void				ft_ssl_b64_flag_error(void)
{
	ft_printf("Usage:\t./ft_ssl base64 [-ed] [-i in_file] [-o out_file]\n");
	ft_printf("\t\t-e, --encode\tencodes input\n");	
	ft_printf("\t\t-d, --decode\tdecodes input\n");
	ft_printf("\t\t-i, --input\tinput file (default: \"-\" for stdin)\n");
	ft_printf("\t\t-o, --output\toutput file (default: \"-\" for stdout)\n");
}

void				ft_ssl_nocommand_error(void)
{
	ft_printf("usage: ft_ssl command [command opts] [command args]\n");
}

/* 
 * This table corresponds in order to the e_commands enum in
 * inc/ft_ssl.h
*/
static const char	*g_allowed_commands[FTSSL_COMMAND_COUNT] = {
	"error", "base64", "des"
};

t_flag_data 		ft_ssl_get_flags(int argc, char **argv)
{
	t_flag_data flag_data;
	int			i;

	flag_data = init_flag_data();
	i = 1;
	while (i < FTSSL_COMMAND_COUNT && ft_strcmp(argv[1], g_allowed_commands[i]) != 0)
		i++;
	flag_data.command = (i < FTSSL_COMMAND_COUNT) ? i : ERROR_SSLCOM;
	i = 2;
	while (i < argc)
	{
		if (ft_strcmp("-e", argv[i]) == 0 || ft_strcmp("--encrypt", argv[i]) == 0)
			flag_data.mode = ENCRYPT_MODE;
		else if (ft_strcmp("-d", argv[i]) == 0 || ft_strcmp("--decrypt", argv[i]) == 0)
			flag_data.mode = DECRYPT_MODE;
		else if (ft_strcmp("-i", argv[i]) == 0 || ft_strcmp("--input", argv[i]) == 0)
		{
			flag_data.has_input_file = 1;
			if (++i < argc)
				flag_data.input_file = ft_strdup(argv[i]);
		}
		else if (ft_strcmp("-o", argv[i]) == 0 || ft_strcmp("--output", argv[i]) == 0)
		{
			flag_data.has_output_file = 1;
			if (++i < argc)
				flag_data.output_file = ft_strdup(argv[i]);
		}
		else
		{
			flag_data.mode = ERROR_MODE;
			return (flag_data);
		}
		i++;
	}
	return (flag_data);
}

int					ft_ssl_check_flags(t_flag_data flag_data)
{
	if (flag_data.command == ERROR_SSLCOM)
		return (0);
	if (flag_data.mode == ERROR_MODE)
		return (0);
	if (flag_data.has_input_file && flag_data.input_file == NULL)
		return (0);
	if (flag_data.has_output_file && flag_data.output_file == NULL)
		return (0);
	return (1);
}
