/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suedadam <suedadam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:41:41 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/13 00:05:49 by suedadam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft/inc/libft.h"

/*
 * base64_encode_block accepts 3 octets and returns 4 encoded characters.
 * Those characters are referenced using the 4 hextets in the input as indices
 * in the table below.
*/

const static char g_base64_table[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/' };

static char *base64_encode_block(const char *input)
{
	unsigned int	sum;
	char			output[5];
	int				len;
	int				i;

	len = 0;
	while (len < 3 && *(input + len))
		len++;
	sum = (len > 0) ? input[0] : 0;
	sum <<= 8;
	sum |= (len > 1) ? input[1] : 0;
	sum <<= 8;
	sum |= (len > 2) ? input[2] : 0;

	output[4] = '\0';
	i = 4;
	while (i-- > 0)
	{
		output[i] = (sum & 0x3f) ? g_base64_table[sum & 0x3f] : '=';
		sum >>= 6;
	}
	return (ft_strdup(output));
}

char *base64_encode(const char *input)
{
	char	*output;
	int		len;

	len = ft_strlen(input);
	output = base64_encode_block(input);
	if (len >= 3)
	{
		len -= 3;
		input += 3;
		while (len > 0)
		{
			output = ft_strjoinfree(output, base64_encode_block(input), 3);
			len -= 3;
			input += 3;
		}	
	}
	return (output);
}

/* static char *base64_decode_block(const char *input) */
/* { */

/* } */

/* char *base64_decode(const char *input) */
/* { */

/* } */
