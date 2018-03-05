/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 16:25:27 by rhallste          #+#    #+#             */
/*   Updated: 2018/03/04 19:42:21 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# include <string.h>

/* 
 * FTSSL_BLCKSZ_B64 must be a mulitple of 12;
 */

# define FTSSL_BUFFSIZE 1000

enum e_block_sizes {
	FTSSL_BLCKSZ_B64 = 12,
	FTSSL_BLCKSZ_DES = 8
};

# define FTSSL_MODE_ENC 1
# define FTSSL_MODE_DEC 2

# define FTSSL_B64OFF 0
# define FTSSL_B64ON 1

# define FTSSL_KEYNO 0
# define FTSSL_KEYYES 1

# define FTSSL_ERR_TXT "undefined"
# define FTSSL_B64_TXT "base64"
# define FTSSL_DES_TXT "des"
# define FTSSL_DESECB_TXT "des-ecb"
# define FTSSL_DESCBC_TXT "des-cbc"

typedef struct			ftssl_args_s {
	char				*command;
	char				*input_file;
	char				*output_file;
	char				*keystr;
	unsigned long		keyval;
	unsigned long		init_vector;
	int					mode;
	int					base64_mode;
}						ftssl_args_t;

typedef					void ftssl_padFunc_t(unsigned char *, int, int);
typedef					int ftssl_commandFunc_t(ftssl_args_t,
								const unsigned char *, char *, int);

typedef struct 			ftssl_command_s {
	char				*name;
	size_t				blocksize;
	ftssl_commandFunc_t	*func;
	int					need_key;
}						ftssl_command_t;

typedef struct			ftssl_opthelp_s {
	int					argc;
	char				**argv;
	char				flag;
	char				*label;
}						ftssl_opthelp_t;

ftssl_args_t	ftssl_get_args(int argc, char **argv);
void			ftssl_destroy_args(ftssl_args_t args);

ftssl_opthelp_t ftssl_opthelp_init(int argc, char **argv, char flag,
								   char *label);
void			ftssl_opthelp_destroy(ftssl_opthelp_t opt);
char			*ftssl_find_optvalue(ftssl_opthelp_t opt, char *command);

unsigned char	*ftssl_padblock_ecb(unsigned char *block, int cursize, int fullsize);

void			ftssl_flag_arg_error(const char *command, const char *option);
void			ftssl_nocommand_error(void);
void			ftssl_invalid_command_error(const char *command);
void			ftssl_file_open_error(const char *filename, int permissions);
void			ftssl_invalid_hexkey_error(void);

int				ftssl_base64(ftssl_args_t args, const unsigned char *input,
							char *out, int len);
int				ftssl_base64_encode(const unsigned char *input, char *out, int len);
int				ftssl_base64_decode(const unsigned char *input, char *out, int len);

int				ftssl_des_ecb(ftssl_args_t args, const unsigned char *input,
							char *output, int len);
unsigned long	ftssl_des_algo(unsigned long keys[16], unsigned long input);
unsigned long	ftssl_des_permute(unsigned long in, unsigned int *tab,
							size_t tab_size);
unsigned long	ftssl_des_sbox_sub(unsigned long in);
unsigned long	ftssl_des_key_transform(unsigned long *key, int round);
unsigned long	*ftssl_des_genkeys(unsigned long initKey, int reverse);

#endif
