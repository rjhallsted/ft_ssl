/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maketest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:58:23 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/05 20:23:03 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	unsigned long message;
	unsigned char first[1];

	message = 0x23456789abcdefL;
	fd = open("test/input.txt", O_WRONLY | O_CREAT);
	first[0] = 1;
	write(fd, first, 1);
	write(fd, (unsigned char *)message, 7);
	close(fd);
}
