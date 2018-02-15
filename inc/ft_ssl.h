/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 16:25:27 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/14 20:44:40 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

enum e_modes {
	ERROR_MODE,
	ENCRYPT_MODE,
	DECRYPT_MODE
};

typedef struct	s_flag_data {
	char		*command;
	int			has_input_file;
	char		*input_file;
	int			has_output_file;
	char		*output_file;
	int			mode;
}				t_flag_data;

enum e_block_sizes {
	B64_BLOCKSIZE = 3,
	DES_BLOCKSIZE = 64
};

t_flag_data	ft_ssl_get_flags(int argc, char **argv);
void		ft_ssl_flag_error(void);
int			ft_ssl_check_flags(t_flag_data flag_data);

char *base64_encode(const char *input);

#endif
