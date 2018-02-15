/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/14 20:20:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/14 20:24:44 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

static t_flag_data	init_flag_data(void)
{
	t_flag_data data;

	data.command = NULL;
	data.mode = MODE_ENCRYPT;
	data.has_input_file = 0;
	data.input_file = NULL;
	data.has_output_file = 0;
	data.output_file = NULL;
	return (data);
}

void				ft_ssl_flag_error(void)
{
	ft_printf("Usage:\t./ft_ssl base64 [-ed] [-i in_file] [-o out_file]\n");
	ft_printf("\t\t-e, --encode\tencodes input\n");	
	ft_printf("\t\t-d, --decode\tdecodes input\n");
	ft_printf("\t\t-i, --input\tinput file (default: \"-\" for stdin)\n");
	ft_printf("\t\t-o, --output\toutput file (default: \"-\" for stdout)\n");
}

t_flag_data 		ft_ssl_get_flags(int argc, char **argv)
{
	t_flag_data flag_data;
	int			i;

	flag_data = init_flag_data();
	i = 2;
	while (i < argc)
	{
		if (ft_strcmp("-e", argv[i]) == 0 || ft_strcmp("--encrypt", argv[i]) == 0)
			flag_data.mode = MODE_ENCRYPT;
		else if (ft_strcmp("-d", argv[i]) == 0 || ft_strcmp("--decrypt", argv[i]) == 0)
			flag_data.mode = MODE_DECRYPT;
		else if (ft_strcmp("-i", argv[i]))
		{
			flag_data.has_input_file = 1;
			if (++i < argc)
				flag_data.input_file = ft_strdup(argv[i]);
		}
		else if (ft_strcmp("-o", argv[i]))
		{
			flag_data.has_output_file = 1;
			if (++i < argc)
				flag_data.output_file = ft_strdup(argv[i]);
		}
		else
		{
			ft_ssl_flag_error();
			flag_data.mode = MODE_ERROR;
			return (flag_data);
		}
		i++;
	}
	return (flag_data);
}

int					ft_ssl_check_flags(t_flag_data flag_data)
{
	if (flag_data.mode == MODE_ERROR)
		return (0);
	if (flag_data.has_input_file && flag_data.input_file == NULL)
		return (0);
	if (flag_data.has_output_file && flag_data.output_file == NULL)
		return (0);
	return (1);
}
