/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/04 15:00:32 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/11 17:40:16 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

unsigned char	*ftssl_padblock_ecb(unsigned char *block, int cursize,
									int fullsize)
{
	int				diff;
	unsigned char	*padded;

	diff = fullsize - cursize;
	padded = ft_memalloc(fullsize);
	ft_memcpy(padded, block, cursize);
	ft_memset(padded + cursize, diff, diff);
	return (padded);
}
