/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 16:25:27 by rhallste          #+#    #+#             */
/*   Updated: 2018/02/15 21:47:54 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

enum e_modes {
	ERROR_MODE,
	ENCRYPT_MODE,
	DECRYPT_MODE
};

#define FTSSL_COMMAND_COUNT 3
enum e_commands {
	ERROR_SSLCOM,
	B64_SSLCOM,
	DES_SSLCOM
};

typedef struct	s_flag_data {
	int			command;
	int			has_input_file;
	char		*input_file;
	int			has_output_file;
	char		*output_file;
	int			mode;
}				t_flag_data;

/* B64E_BLOCKSIZE must be a multiple of 3
 * B64D_BLOCKSIZE must be a mulitple of 4
 */

enum e_block_sizes {
	B64E_BLOCKSIZE = 60,
	B64D_BLOCKSIZE = 16,
	DES_BLOCKSIZE = 64
};

t_flag_data	ft_ssl_get_flags(int argc, char **argv);
void		ft_ssl_b64_flag_error(void);
void		ft_ssl_nocommand_error(void);
int			ft_ssl_check_flags(t_flag_data flag_data);

int			ft_ssl_base64_encode(const unsigned char *input, char *out, int len);
int			ft_ssl_base64_decode(const unsigned char *input, char *out, int len);

#endif
