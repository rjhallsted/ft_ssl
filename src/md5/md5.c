/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/03 22:29:05 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/08 20:51:09 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

static int					is_flag(const char *input)
{
	int i;

	if (input[0] == '-') {
		i = 1;
		while (input[i])
		{
			if (ft_strchr("pqrs", input[i]))
				return (1);
			i++;
		}
	}
	return (0);
}

static t_ftssl_md5_args		*get_args(int argc, char **argv)
{
	t_ftssl_md5_args	*args;
	int					i;
	int					fd;
	
	args = ft_memalloc(sizeof(t_ftssl_md5_args));
	args->print_input = ft_findopt(argc, argv, 'p', NULL);
	args->quiet_mode = ft_findopt(argc, argv, 'q', NULL);
	args->reverse_output = ft_findopt(argc, argv, 'r', NULL);
	args->string_mode = ft_findopt(argc, argv, 's', &(args->input_string));

	args->input_fds = NULL;
	args->input_fd_count = 0;
	args->has_file_errors = 0;
	i = 2;
	while (i < argc)
	{
		if (!is_flag(argv[i]) && (fd = open(argv[i], O_RDONLY)) > 0) {
			args->input_fds = ft_memrealloc(args->input_fds, sizeof(int) * (args->input_fd_count + 1), sizeof(args->input_fds));
			args->input_fds[args->input_fd_count] = fd;
			args->input_fd_count++;
		} else {
			ft_printf_fd(STDERR_FILENO, "%s%s%s\n", "md5: ", argv[i], ": No such file or directory");
			args->has_file_errors = 1;
		}
		i++;
	}
	if (args->input_fd_count == 0 && !args->string_mode && !args->has_file_errors) {
		args->input_fds = ft_memalloc(sizeof(int));
		args->input_fds[0] = STDIN_FILENO;
		args->input_fd_count = 1;
	}
	return (args);
}

static size_t				pad_input(unsigned char *input, unsigned char **padded)
{
	size_t	len;
	int64_t len_bits;
	size_t	pad_len;

	/* pad to 64 bits short of a multiple of 512 bits. (8 and 64 bytes, respectively) */
	len = ft_strlen((char *)input);
	pad_len = 0;
	while ((len + pad_len) % 64 != 56)
		pad_len++;
	ft_printf("%zd\n", len);
	*padded = ft_memrealloc(input, len + pad_len + 8, len);
	if (pad_len > 0) {
		(*padded)[len] = (unsigned char)128; //add 1 bit followed by 0's (big endian style)
		ft_bzero(*padded + len + 1, pad_len - 1);
	} else
		*padded = input;
	len_bits = ((long long)len * 8);
	ft_reverse_bytes(&len_bits, 8);
	ft_memcpy(*padded + len + pad_len, &len_bits, 8);
	return (len + pad_len + 8);
}

void						ftssl_md5_wrapper(char *command_name, int argc, char **argv)
{
	t_ftssl_md5_args	*args;
	unsigned char		*input;
	size_t				len;
	int					i;
	
	args = get_args(argc, argv);
	command_name = NULL;
	//handle strings here
	i = 0;
	while (i < args->input_fd_count) {
		input = (unsigned char *)ft_get_file_contents(args->input_fds[i]);
		len = pad_input(input, &input);
		ft_printmemory_binary(input, len);
		//do algo
		//output
		i++;
	}
}
