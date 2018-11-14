/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_file_contents.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 18:28:47 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/11 17:31:58 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../inc/libft.h"

char	*ft_get_file_contents(int fd)
{
	char	*contents;
	int		rv;
	int		prog;
	char	buff[BUFF_SIZE + 1];

	contents = ft_strnew(0);
	prog = 0;
	while ((rv = read(fd, buff, BUFF_SIZE)))
	{
		prog += rv;
		buff[rv] = '\0';
		contents = ft_strjoinfree(contents, buff, 1);
	}
	return (contents);
}
