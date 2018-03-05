/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/04 15:00:32 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/04 16:45:12 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

unsigned char *ftssl_padblock_ecb(unsigned char *block, int cursize, int fullsize)
{
	size_t			diff;
	unsigned char	*padded;

	diff = fullsize - cursize;
	padded = ft_memalloc(fullsize);
	ft_memcpy(padded, block, cursize);
	padded[fullsize - 1] = diff;
	return (padded);
}
