/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maketest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:58:23 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/06 16:59:58 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char message[8] = {0x67, 0x5a, 0x69, 0x67, 0x5e, 0x5a, 0x6b, 0x5a};

	fd = open("test/input.txt", O_WRONLY | O_CREAT);
	write(fd, message, 8);
	close(fd);
}
