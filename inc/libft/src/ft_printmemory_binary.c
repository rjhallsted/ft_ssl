/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printmemory_binary.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 19:55:17 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/11 17:32:32 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/libft.h"

static void	put_binary(int a)
{
	char	*str;
	int		len;

	str = ft_itoa_base(a, 2);
	len = ft_strlen(str);
	write(1, "00000000", 8 - len);
	write(1, str, len);
	free(str);
}

static void	put_spacing(int a)
{
	while (a < 4)
	{
		if (a % 4 == 1)
		{
			write(1, "       ", 7);
			a++;
		}
		write(1, "         ", 9);
		a += 1;
	}
}

void		ft_printmemory_binary(const void *addr, size_t size)
{
	const unsigned char	*mem;
	size_t				i;
	size_t				tmp;
	unsigned char		c;

	i = 0;
	mem = (const unsigned char *)addr;
	while (i < size)
	{
		tmp = i;
		while (tmp < size && (tmp - i) < 4)
		{
			put_binary(mem[tmp++]);
			write(1, " ", 1);
		}
		if (tmp - i < 4)
			put_spacing((int)(tmp - i));
		while (i < tmp)
		{
			c = (ft_isprint(mem[i])) ? *(mem + i) : '.';
			write(1, &c, 1);
			i++;
		}
		write(1, "\n", 1);
	}
}
