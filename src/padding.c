/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/04 15:00:32 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/04 15:33:44 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "../inc/ft_ssl.h"
#include "../inc/libft/inc/libft.h"

void ftssl_padblock_ecb(unsigned char *block, int cursize, int fullsize)
{
	size_t diff;

	diff = fullsize - cursize;
	ft_bzero((block + cursize), diff - 1);
	block[fullsize - 1] = diff;
}
