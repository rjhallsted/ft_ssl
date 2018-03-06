/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maketest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:58:23 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/05 22:48:50 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	unsigned long message;
	unsigned char first[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

	message = 0x23456789abcdef;
	fd = open("test/input.txt", O_WRONLY | O_CREAT);
	write(fd, first, 8);
	close(fd);
}
