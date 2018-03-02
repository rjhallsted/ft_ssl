/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 16:25:27 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/01 23:31:20 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

/* 
 * FTSSL_BLCKSZ_B64 must be a mulitple of 12;
 */

enum e_block_sizes {
	FTSSL_BLCKSZ_B64 = 48,
	FTSSL_BLCKSZ_DES = 8
};

# define FTSSL_MODE_ENC 1
# define FTSSL_MODE_DEC 2

# define FTSSL_B64OFF 0
# define FTSSL_B64ON 1

# define FTSSL_ERR_TXT "undefined"
# define FTSSL_B64_TXT "base64"

typedef struct			ftssl_args_t {
	char				*command;
	char				*input_file;
	char				*output_file;
	char				*key;
	int					mode;
	int					base64_mode;
	int					init_vector;
}

typedef					int ftssl_commandFunc_t(ftssl_args_t, char *, char *, int);

typedef struct 			ftssl_command_s {
	char				*name;
	size_t				blocksize;
	ftssl_commandFunc_t	*func;
}						ftssl_commant_t;

typedef struct			ftssl_opthelp_t {
	int					argc;
	char				**argv;
	char				flag;
	char				*label;
}

ftssl_args_t	ftssl_get_args(int argc, char **argv);
void			ftssl_destroy_args(ftssl_args_t args);
ftssl_opthelp_t ftssl_opthelp_init(int argc, char **argv, char flag,
								   char *label);
void			ftssl_opthelp_destroy(ftssl_opthelp_t opt);
char			*ftssl_find_optvalue(ftssl_opthelp_t opt, char *command);
void			ftssl_flag_arg_error(const char *command, const char *option);
void			ftssl_b64_usage(void);
void			ftssl_nocommand_error(void);
int				ftssl_base64(ftssl_args_t args, const unsigned char *input,
							char *out, int len);
int				ftssl_base64_encode(const unsigned char *input, char *out, int len);
int				ftssl_base64_decode(const unsigned char *input, char *out, int len);

#endif
