/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:19:52 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/14 19:34:47 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

/*
 * block_size is size in bytes of amount to pass each function
 * E.g. base64 accepts multiples of 3 (24 bits)
 * DES accepts 8 (64 bits)
 */

char		*get_block(int fd, int block_size)
{
	char	buffer[block_size + 1];
	int		ret;
	int		prog;

	prog = 0;
	ft_bzero(buffer, block_size + 1);
	while (prog < block_size &&
		   (ret = read(fd, buffer + prog, block_size - prog)))
		prog += ret;
	return ((prog > 0) ? ft_strdup(buffer) : NULL);
}

void		flag_error(void)
{
	ft_printf("Usage:\t./ft_ssl base64 [-ed] [-i in_file] [-o out_file]\n");
	ft_printf("\t\t-e, --encode\tencodes input\n");	
	ft_printf("\t\t-d, --decode\tdecodes input\n");
	ft_printf("\t\t-i, --input\tinput file (default: \"-\" for stdin)\n");
	ft_printf("\t\t-o, --output\toutput file (default: \"-\" for stdout)\n");
}

t_flag_data get_flags(int argc, char **argv)
{
	t_flag_data flag_data;
	int			i;

	flag_data.command = ft_strdup(argv[1]);
	flag_data.mode = MODE_ENCRYPT;
	flag_data.has_input_file = 0;
	flag_data.input_file = NULL;
	flag_data.has_output_file = 0;
	flag_data.output_file = NULL;
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
			flag_data.input_file = ft_strdup(argv[++i]);
		}
		else if (ft_strcmp("-o", argv[i]))
		{
			flag_data.has_output_file = 1;
			flag_data.output_file = ft_strdup(argv[++i]);
		}
		else
		{
			flag_error();
			flag_data.mode = MODE_ERROR;
			return (flag_data);
		}
		i++;
	}
	return (flag_data);
}

int			main(int argc, char **argv)
{
	int			block_size;
	char		*block;
	char		*output;
	int			fd;
	t_flag_data	flag_data;
	
	if (argc < 2)
	{
		ft_printf("usage: ft_ssl command [command opts] [command args]\n");
		return (0);
	}
	fd = 0;
	flag_data = get_flags(argc, argv);
	if (flag_data.mode == MODE_ERROR)
		return (-1);
	if (ft_strcmp(flag_data.command, "base64") == 0)
	{
		block_size = 3;
//		if ((fd = open(STDIN_FILENO, O_RDONLY)) == -1)
//		{
//			ft_printf_fd(2, "Error: Failed to open file\n");
//			return (-1);
//		}
		while ((block = get_block(fd, block_size)))
		{
			output = base64_encode(block);
			free(block);
			block = NULL;
			ft_printf("%s", output);
			free(output);
		}
		ft_printf("\n");
		close(fd);
	}
	return (0);
}
