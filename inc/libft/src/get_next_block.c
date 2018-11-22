/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_block.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 20:32:02 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 21:04:52 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/libft.h"
#include "../inc/get_next_block.h"

static int		copy(char **block, t_gnb *data, size_t rv, size_t block_size)
{
	int		copy_len;
	int		i;
	size_t	old_len;
	
	copy_len = (rv > block_size) ? block_size : rv;
	old_len = (size_t)data->len;
	data->len += copy_len;
	if (!(*block = ft_memrealloc(*block, (size_t)data->len + 1, old_len)))
		return (-1);
	ft_strncat(*block, data->buff, (size_t)copy_len);
	i = 0;
	copy_len++;
	while (data->buff[i] && data->buff[i + copy_len])
	{
		data->buff[i] = data->buff[i + copy_len];
		i++;
	}
	ft_bzero(data->buff + i, (size_t)(BLOCK_BUFF_SIZE - i));
	return ((rv >= block_size));
}

static t_gnb	*new_data_item(int fd)
{
	t_gnb	*new;

	if (!(new = ft_memalloc(sizeof(t_gnb))))
		return (NULL);
	new->fd = fd;
	new->len = 0;
	ft_bzero(new->buff, BLOCK_BUFF_SIZE + 1);
	new->next = NULL;
	return (new);
}

static t_gnb	*find_fdb(t_gnb **first, int fd)
{
	t_gnb	*item;

	if (!first)
		return (NULL);
	item = NULL;
	if (!(*first))
	{
		*first = new_data_item(fd);
		return (*first);
	}
	item = *first;
	if (item->fd == fd)
		return (item);
	while (item->next)
	{
		if (item->next->fd == fd)
			return (item->next);
		item = item->next;
	}
	item = *first;
	*first = new_data_item(fd);
	(*first)->next = item;
	return (*first);
}

static void		free_fdb(t_gnb **first, t_gnb *data)
{
	t_gnb *tmp;
	t_gnb *item;

	if (!data || !first)
		return ;
	if (*first == data)
	{
		tmp = data->next;
		free(data);
		*first = tmp;
	}
	else
	{
		item = *first;
		while (item->next != data)
			item = item->next;
		tmp = data->next;
		free(data);
		item->next = tmp;
	}
}

int				get_next_block(const int fd, char **block, size_t block_size)
{
	static t_gnb	*first;
	t_gnb			*d;
	ssize_t			rv;

	if (!(block) || fd < 0 || (!(d = find_fdb(&first, fd))))
		return (-1);
	*block = NULL;
	if (ft_strlen(d->buff) > 0 && (rv = copy(block, d, (int)ft_strlen(d->buff), block_size)))
		return ((int)rv);
	if ((rv = read(fd, d->buff, BLOCK_BUFF_SIZE)) == -1 || (rv == 0 && *block == NULL))
	{
		free_fdb(&first, d);
		return ((int)rv);
	}
	while (rv)
	{
		if (copy(block, d, (int)rv, block_size))
			return (1);
		if ((rv = (int)read(fd, d->buff, BLOCK_BUFF_SIZE)) == -1)
		{
			free_fdb(&first, d);
			return (-1);
		}
	}
	return ((rv == 0 && *block != NULL));
}
