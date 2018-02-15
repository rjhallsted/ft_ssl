/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suedadam <suedadam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:41:41 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/15 13:05:59 by rhallste         ###   ########.fr       */
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
	unsigned int	bits;
	char			output[5];
	int				len;
	int				i;

	len = 0;
	while (len < 3 && *(input + len))
		len++;
	bits = (len > 0) ? (unsigned char)input[0] : 0;
	bits <<= 4;
	if (len > 1)
	{
		bits <<= 4;
		bits |= (len > 1) ? (unsigned char)input[1] : 0;
		bits <<= 2;
		if (len > 2)
		{
			bits <<= 6;
			bits |= (len > 2) ? (unsigned char)input[2] : 0;
		}
	}

	output[4] = '\0';
	i = 3;
	while (i > len)
		output[i--] = '=';
	while (i >= 0)
	{
		output[i] = g_base64_table[bits & 0x3f];
		bits >>= 6;
		i--;
	}
	return (ft_strdup(output));
}

char *ft_ssl_base64_encode(const char *input)
{
	char	*output;
	int		len;

	len = ft_strlen(input);
	output = base64_encode_block(input);
	len -= 3;
	input += 3;
	while (len >= 3)
	{
		output = ft_strjoinfree(output, base64_encode_block(input), 3);
		len -= 3;
		input += 3;
	}
	return (output);
}

static int	find_in_table(char c)
{
	int i;

	i = 0;
	while (i < 64 && g_base64_table[i] != c)
		i++;
	return (i);
}

static char *base64_decode_block(const char *input)
{
	int				i;
	unsigned int	bits;
	char			output[4];
	int				len;

	i = 0;
	bits = 0;
	len = 0;
	while (i < 4 && *(input + i) && input[i] != '=')
	{
		bits <<= 6;
		len += 6;
		bits |= find_in_table(input[i]);
		i++;
	}
	if (i < 4)
	{
		bits <<= i * 2;
		len += i * 2;
	}
	i = len / 8;
	ft_bzero(output + i, 4 - i);
	while (i > 0)
	{
		output[--i] = (unsigned char)(bits & 0xff);
		bits >>= 8;
	}
	return (ft_strdup(output));
}

char *ft_ssl_base64_decode(const char *input)
{
	char	*output;
	int		len;

	len = ft_strlen(input);
	output = base64_decode_block(input);
	len -= 4;
	input += 4;
	while (len >= 4 && !ft_strchr(input, '='))
	{
		output = ft_strjoinfree(output, base64_decode_block(input), 3);
		len -= 4;
		input += 4;
	}
	if (len > 0)
		output = ft_strjoinfree(output, base64_decode_block(input), 3);
	return (output);	
}
