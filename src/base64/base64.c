/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suedadam <suedadam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 14:41:41 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/16 19:51:21 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_ssl.h>

/*
** base64_encode_block accepts 3 octets and returns 4 encoded characters.
** Those characters are referenced using the 4 hextets in the input as indices
** in the table below.
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

/*
** Returns the number of characters expected in the encoded text
*/

int			ftssl_base64_encode(const unsigned char *in,
								unsigned char *out, int len)
{
	int	res_len;

	res_len = (len / 3);
	res_len += (len % 3 > 0) ? 1 : 0;
	res_len *= 4;
	while (len > 0)
	{
		out[0] = (unsigned char)g_b64e[in[0] >> 2];
		if (len > 1)
			out[1] = (unsigned char)g_b64e[((in[0] & 0x3) << 4) | (in[1] >> 4)];
		else
			out[1] = (unsigned char)g_b64e[(in[0] & 0x3) << 4];
		if (len > 2)
			out[2] = (unsigned char)g_b64e[((in[1] & 0xf) << 2) | (in[2] >> 6)];
		else if (len == 2)
			out[2] = (unsigned char)g_b64e[((in[1] & 0xf) << 2)];
		else
			out[2] = (unsigned char)'=';
		out[3] = (unsigned char)(len > 2) ? g_b64e[in[2] & 0x3f] : '=';
		len -= 3;
		in += 3;
		out += 4;
	}
	*out = '\n';
	return (res_len + 1);
}

static int	find_in_table(char c)
{
	int i;

	i = 0;
	while (i < 64 && g_b64e[i] != c)
		i++;
	return (i);
}

static int	base64_decode_block(const unsigned char *input,
								unsigned char *out)
{
	unsigned char	inu[4];
	int				ret;

	inu[0] = (unsigned char)find_in_table(input[0]);
	inu[1] = (unsigned char)find_in_table(input[1]);
	inu[2] = (unsigned char)find_in_table(input[2]);
	inu[3] = (unsigned char)find_in_table(input[3]);
	out[0] = (unsigned char)((inu[0] << 2) & 0xff) | ((inu[1] >> 4) & 0x3);
	if (input[2] == '=')
		out[1] = 0;
	else
		out[1] = (unsigned char)((inu[1] & 0xf) << 4) | ((inu[2] >> 2) & 0xf);
	if (input[3] == '=')
		out[2] = 0;
	else
		out[2] = ((inu[2] & 0x3) << 6) | (inu[3] & 0x3f);
	ret = 3;
	while (input[ret] == '=' && ret > 1)
		ret--;
	return (ret);
}

/*
** Returns the number of characters expected in the decoded text
*/

int			ftssl_base64_decode(const unsigned char *input,
								unsigned char *output, int len)
{
	unsigned char	*start;
	int				res_len;
	int				ret;

	start = output;
	res_len = len * 3 / 4;
	while (len >= 4 && !ft_memchr(input, '=', 4))
	{
		ret = base64_decode_block(input, output);
		len -= 4;
		input += 4;
		output += 3;
	}
	if (len > 0)
	{
		ret = base64_decode_block(input, output);
		output += ret;
	}
	if (ret < 3)
		res_len -= (3 - ret);
	return (res_len);
}

int			ftssl_base64(t_ftssl_args args, const unsigned char *input,
						unsigned char *output, int len)
{
	if (args.mode == FTSSL_MODE_DEC)
		return (ftssl_base64_decode(input, output, len));
	else
		return (ftssl_base64_encode(input, output, len));
}
