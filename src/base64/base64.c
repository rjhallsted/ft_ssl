/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suedadam <suedadam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:41:41 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/15 21:10:59 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft/inc/libft.h"

/*
 * base64_encode_block accepts 3 octets and returns 4 encoded characters.
 * Those characters are referenced using the 4 hextets in the input as indices
 * in the table below.
*/

const static unsigned char g_b64e[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/' };

static int base64_encode_block(const unsigned char *in, char *out, int len)
{
	out[0] = (unsigned char) g_b64e[in[0] >> 2];
	out[1] = (unsigned char) g_b64e[((in[0] & 0x3) << 4) | (in[1] >> 4)];
	out[2] = (unsigned char) (len > 1) ? g_b64e[((in[1] & 0xf) << 2) | (in[2] >> 6)] : '=';
	out[3] = (unsigned char) (len > 2) ? g_b64e[in[2] & 0x3f] : '=';

	return (4);
}

/*
 * Returns the number of characters expected in the encoded text
 */
int ft_ssl_base64_encode(const unsigned char *input, char *output, int len)
{
	int	res_len;

	res_len = (len / 3);
	if (len % 3 > 0)
		res_len++;
	res_len *= 4;
//	ft_printf("%c%c%c\n", input[0], input[1], input[2]);
	base64_encode_block(input, output, len);
//	ft_printf("%c%c%c%c\n", input[0], input[1], input[2], input[3]);
	len -= 3;
	input += 3;
	output += 4;
	while (len >= 3)
	{
//		ft_printf("%c%c%c\n", input[0], input[1], input[2]);
		base64_encode_block(input, output, len);
//		ft_printf("%c%c%c%c\n", input[0], input[1], input[2], input[3]);
		len -= 3;
		input += 3;
		output += 4;
	}
	return (res_len);
}

static int	find_in_table(char c)
{
	int i;

	i = 0;
	while (i < 64 && g_b64e[i] != c)
		i++;
	return (i);
}

static int base64_decode_block(const unsigned char *input, char *out)
{
	unsigned char	inu[4];
	int				ret;
	
	inu[0] = (unsigned char)find_in_table(input[0]);
	inu[1] = (unsigned char)find_in_table(input[1]);
	inu[2] = (unsigned char)find_in_table(input[2]);
	inu[3] = (unsigned char)find_in_table(input[3]);

//	ft_printf("\n\ninput: %s\n", input);
//	ft_printf("%hhu, %hhu, %hhu, %hhu\n", inu[0], inu[1], inu[2], inu[3]);
		
	out[0] = (unsigned char) ((inu[0] << 2) & 0xff) | ((inu[1] >> 4) & 0x3);
	out[1] = (unsigned char) (input[2] == '=') ? 0 : ((inu[1] & 0xf) << 4) | ((inu[2] >> 2) & 0xf);
	out[2] = (unsigned char) (input[3] == '=') ? 0 : ((inu[2] & 0x3) << 6) | (inu[3] & 0x3f);

	ret = 3;
	if (input[3] == '=')
		ret--;
	if (input[2] == '=')
		ret--;
//	ft_printf("%hhu, %hhu, %hhu\n", out[0], out[1], out[2]);
	return (ret);
}

/*
 * Returns the number of characters expected in the decoded text
 */
int	ft_ssl_base64_decode(const unsigned char *input, char *output, int len)
{
	char 	*start;
	int		res_len;
	int		ret;
	
	start = output;
//	ft_printf("len: %d\n", len);
	res_len = (len / 4 * 3);
	while (len >= 4 && !ft_strchr((char *)input, '='))
	{
		ret = base64_decode_block(input, output);
		len -= 4;
		input += 4;
		output += 3;
	}
	if (len > 0)
		ret = base64_decode_block(input, output);
//	ft_printf("ret: %d\n", ret);
	if (ret < 3)
		res_len = ret;
//	ft_printf("res_len: %d\n", res_len);	
	return (res_len);
}
