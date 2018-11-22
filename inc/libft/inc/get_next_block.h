/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_block.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 20:37:02 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 21:05:17 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_BLOCK_H
# define GET_NEXT_BLOCK_H

# define BLOCK_BUFF_SIZE 10000

typedef struct		s_gnb
{
	int				fd;
	int				len;
	char			buff[BLOCK_BUFF_SIZE + 1];
	struct s_gnb	*next;
}					t_gnb;

int					get_next_block(const int fd, char **block,
								   size_t block_size);

#endif
