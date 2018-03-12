/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 02:37:48 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/11 16:48:17 by rhallste         ###   ########.fr       */
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
	int				ret;
	unsigned char	*tmp;
	unsigned char	*tmp2;

	tmp = ft_memalloc(len);
	ret = ftssl_des_cbc(args, input, tmp, len);
 	args.mode = (args.mode == FTSSL_MODE_ENC) ? FTSSL_MODE_DEC : FTSSL_MODE_ENC;
	args.keyval = args.keyval2;
	tmp2 = ft_memalloc(len);
	ret = ftssl_des_cbc(args, tmp, tmp2, ret);
	free(tmp);
 	args.mode = (args.mode == FTSSL_MODE_ENC) ? FTSSL_MODE_DEC : FTSSL_MODE_ENC;
	args.keyval = args.keyval3;
	ret = ftssl_des_cbc(args, tmp2, output, ret);
	free(tmp2);
	return (ret);
}
