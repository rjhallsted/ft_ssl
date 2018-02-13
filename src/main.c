/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:19:52 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/13 11:37:51 by rhallste         ###   ########.fr       */
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

char *get_block(int fd, int block_size)
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

int main(int argc, char **argv)
{
	int		block_size;
	char	*block;
	char	*output;
	int		fd;
	
	if (argc < 2)
	{
		ft_printf("usage: ft_ssl command [command opts] [command args]\n");
		return (0);
	}
	fd = 0;
	if (ft_strcmp(argv[1], "base64") == 0)
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
