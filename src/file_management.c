/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/03 23:12:21 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/03 23:16:07 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>

int	ftssl_open_file(const char *filepath)
{
	int fd;
	
	fd = (filepath && ft_strcmp(filepath, "-")) ? -5 : STDIN_FILENO;
	if (fd == -5 && (fd = open(filepath, O_RDONLY)) == -1)
		ftssl_file_open_error(filepath, O_RDONLY);
	return (fd);
}
