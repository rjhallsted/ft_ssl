/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 02:37:48 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/11 19:09:59 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../inc/libft/inc/libft.h"
#include "../../inc/ft_ssl.h"

#include <unistd.h>
int	ftssl_des3_ecb(t_ftssl_args args, const unsigned char *input,
				   unsigned char *output, int len)
{
	int				ret;
	unsigned char	*tmp;
	unsigned char	*tmp2;

	tmp = ft_memalloc(len);
	ret = ftssl_des_ecb(args, input, tmp, len);
 	args.mode = (args.mode == FTSSL_MODE_ENC) ? FTSSL_MODE_DEC : FTSSL_MODE_ENC;
	args.keyval = args.keyval2;
	tmp2 = ft_memalloc(len);
	ret = ftssl_des_ecb(args, tmp, tmp2, ret);
	free(tmp);
 	args.mode = (args.mode == FTSSL_MODE_ENC) ? FTSSL_MODE_DEC : FTSSL_MODE_ENC;
	args.keyval = args.keyval3;
	ret = ftssl_des_ecb(args, tmp2, output, ret);
	free(tmp2);
	return (ret);
}

int	ftssl_des3_cbc(t_ftssl_args args, const unsigned char *input,
					unsigned char *output, int len)
{
	int						reslen;
	unsigned long			in_val;
	unsigned long			out_val;
	unsigned long			*keys[3];

	keys[0] = ftssl_des_genkeys(args.keyval, (args.mode == FTSSL_MODE_DEC));
	keys[1] = ftssl_des_genkeys(args.keyval2, (args.mode != FTSSL_MODE_DEC));	
	keys[2] = ftssl_des_genkeys(args.keyval3, (args.mode == FTSSL_MODE_DEC));
	reslen = 0;
	while (reslen < len)
	{
		in_val = ftssl_get_inputval(args, input, len, reslen);
		if (args.mode == FTSSL_MODE_ENC)
			in_val ^= args.init_vector;
		out_val = ftssl_des_algo(keys[0], in_val);
		out_val = ftssl_des_algo(keys[1], out_val);
		out_val = ftssl_des_algo(keys[2], out_val);
		if (args.mode == FTSSL_MODE_DEC)
			out_val ^= args.init_vector;
		args.init_vector = (args.mode == FTSSL_MODE_ENC) ? out_val : in_val;
		ft_memcpy(output + reslen, &out_val, FTSSL_BLCKSZ_DES);
		ft_reverse_bytes((void *)(output + reslen), 8);
		reslen += FTSSL_BLCKSZ_DES;
	}
	if (args.mode == FTSSL_MODE_DEC && len % 8 != 0)
		reslen -= *(output + reslen - 1);
	free(keys[0]);
	free(keys[1]);
	free(keys[2]);
	return (reslen);
}
