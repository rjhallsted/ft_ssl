/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 16:25:27 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/07 18:36:16 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# include <string.h>

/*
**FTSSL_BLCKSZ_B64 must be a mulitple of 12
*/

# define FTSSL_BUFFSIZE 1000

enum					e_block_sizes {
	FTSSL_BLCKSZ_B64 = 12,
	FTSSL_BLCKSZ_DES = 8
};

# define FTSSL_MODE_ENC 1
# define FTSSL_MODE_DEC 2

# define FTSSL_B64OFF 0
# define FTSSL_B64ON 1

# define FTSSL_KEYNO 0
# define FTSSL_KEYYES 1

# define FTSSL_IVNO 0
# define FTSSL_IVYES 1

# define FTSSL_ERR_TXT "undefined"
# define FTSSL_B64_TXT "base64"
# define FTSSL_DES_TXT "des"
# define FTSSL_DESECB_TXT "des-ecb"
# define FTSSL_DESCBC_TXT "des-cbc"
# define FTSSL_DES3_TXT "des3"
# define FTSSL_DES3ECB_TXT "des3-ecb"
# define FTSSL_DES3CBC_TXT "des3-cbc"
# define FTSSL_MD5_TXT "md5"

/* MD5/Hashing-related stuff */
typedef struct			s_ftssl_md5_args {
	int					print_input;
	int					quiet_mode;
	int					reverse_output;
	int					string_mode;
	char				*input_string;
	int					*input_fds;
	int					input_fd_count;
	int					has_file_errors;
}						t_ftssl_md5_args;

typedef void			t_ftssl_comm_wrap(char *command_name, int argc, char **argv);

typedef struct			s_ftssl_command {
	char				*name;
	t_ftssl_comm_wrap	*wrapper_func;
}						t_ftssl_command;

/* wrappers */

void					ftssl_md5_wrapper(char *command_name, int argc, char **argv);
void					ftssl_des_family_wrapper(char *command_name, int argc, char **argv);

/* DES/Encryption-related stuff */

typedef struct			s_ftssl_args {
	char				*command;
	char				*input_file;
	char				*output_file;
	char				*keystr;
	char				*iv_str;
	unsigned long		keyval;
	unsigned long		keyval2;
	unsigned long		keyval3;
	unsigned long		init_vector;
	int					mode;
	int					base64_mode;
}						t_ftssl_args;

typedef void			t_ftssl_pad_func(unsigned char *input,
										int cursize, int fullsize);
typedef	int				t_ftssl_comm_func(t_ftssl_args args, const unsigned char *input,
										unsigned char *output, int len);

typedef struct			s_ftssl_des_command {
	char				*name;
	size_t				blocksize;
	t_ftssl_comm_func	*func;
	int					need_key;
	int					need_iv;
	int					keys_needed;
}						t_ftssl_des_command;

typedef struct			s_ftssl_opthelp {
	int					argc;
	char				**argv;
	char				flag;
	char				*label;
}						t_ftssl_opthelp;

/* Args */

t_ftssl_args			ftssl_get_args(int argc, char **argv);
void					ftssl_destroy_args(t_ftssl_args args);

t_ftssl_opthelp			ftssl_opthelp_init(int argc, char **argv, char flag,
											char *label);
void					ftssl_opthelp_destroy(t_ftssl_opthelp opt);
char					*ftssl_find_optvalue(t_ftssl_opthelp opt,
											char *command);

int						ftssl_find_comm_key(char *command_name);
void					ftssl_prep_args(t_ftssl_args *args);

/* padding? */

unsigned char			*ftssl_padblock_ecb(unsigned char *block,
											int cursize, int fullsize);
/* error handling */

void					ftssl_flag_arg_error(const char *command,
											const char *option);
void					ftssl_nocommand_error(void);
void					ftssl_invalid_command_error(const char *command);
void					ftssl_file_open_error(const char *filename,
											int permissions);
void					ftssl_invalid_hexkey_error(void);

/* base64 */

int						ftssl_base64(t_ftssl_args args,
									const unsigned char *input,
									unsigned char *out, int len);
int						ftssl_base64_encode(const unsigned char *input,
											unsigned char *out, int len);
int						ftssl_base64_decode(const unsigned char *input,
											unsigned char *out, int len);
/* DES */

int						ftssl_des_ecb(t_ftssl_args args,
										const unsigned char *input,
										unsigned char *output, int len);
int						ftssl_des_cbc(t_ftssl_args args,
										const unsigned char *input,
										unsigned char *output, int len);
unsigned long			ftssl_des_permute(unsigned long in, size_t in_size,
										unsigned int *tab, size_t tab_size);
unsigned long			ftssl_des_sbox_sub(unsigned long in);
unsigned long			ftssl_des_key_transform(unsigned long *key, int round);
unsigned long			*ftssl_des_genkeys(unsigned long init_key, int reverse);
unsigned long			ftssl_get_inputval(t_ftssl_args args,
											const unsigned char *input, int len,
											int offset);
unsigned long			ftssl_des_algo(unsigned long keys[16],
											unsigned long input);
/* Triple-DES */

int						ftssl_des3_ecb(t_ftssl_args args,
										const unsigned char *input,
										unsigned char *output, int len);
int						ftssl_des3_cbc(t_ftssl_args args,
										const unsigned char *input,
										unsigned char *output, int len);

#endif
