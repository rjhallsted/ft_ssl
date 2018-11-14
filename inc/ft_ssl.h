/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/12 16:25:27 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/14 13:58:53 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# include <string.h>
# include <stdint.h>

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
# define FTSSL_SHA256_TXT "sha256"
# define FTSSL_SHA224_TXT "sha224"
# define FTSSL_SHA512_TXT "sha512"
# define FTSSL_SHA384_TXT "sha384"
# define FTSSL_SHA512_224_TXT "sha512_224"
# define FTSSL_SHA512_256_TXT "sha512_256"

/*
**MD5/Hashing-related stuff
*/

typedef struct			s_ftssl_md5_args {
	char				*command;
	int					print_input;
	int					quiet_mode;
	int					reverse_output;
	int					string_mode;
	char				*input_string;
	char				**filenames;
	int					*fds;
	int					fd_count;
	int					read_stdin;
	int					*error_indices;
	int					error_count;
}						t_ftssl_md5_args;

typedef void			t_ftssl_comm_wrap(char *command_name, int argc,
										char **argv);

typedef struct			s_ftssl_command {
	char				*name;
	t_ftssl_comm_wrap	*wrapper_func;
}						t_ftssl_command;

# define SHR(x,by) (x >> by)
# define SHL(x,by) (x << by)
# define ROTR(x,by,w) ((x >> by) | (x << (w - by)))
# define ROTL(x,by,w) ((x << by) | (x >> (w - by)))
# define SHA_CH(x,y,z) ((x & y) ^ ((~x) & z))
# define SHA_MAJ(x,y,z) ((x & y) ^ (x & z) ^ (y & z))
# define SHA_BSIG0(x) (ROTR(x,2,32) ^ ROTR(x,13,32) ^ ROTR(x,22,32))
# define SHA_BSIG1(x) (ROTR(x,6,32) ^ ROTR(x,11,32) ^ ROTR(x,25,32))
# define SHA_SSIG0(x) (ROTR(x,7,32) ^ ROTR(x,18,32) ^ SHR(x,3))
# define SHA_SSIG1(x) (ROTR(x,17,32) ^ ROTR(x,19,32) ^ SHR(x,10))

# define SHA512_BSIG0(x) (ROTR(x,28,64) ^ ROTR(x,34,64) ^ ROTR(x,39,64))
# define SHA512_BSIG1(x) (ROTR(x,14,64) ^ ROTR(x,18,64) ^ ROTR(x,41,64))
# define SHA512_SSIG0(x) (ROTR(x,1,64) ^ ROTR(x,8,64) ^ SHR(x,7))
# define SHA512_SSIG1(x) (ROTR(x,19,64) ^ ROTR(x,61,64) ^ SHR(x,6))

/*
**wrappers
*/

void					ftssl_md5_family_wrapper(char *command_name, int argc,
										char **argv);
unsigned char			*ftssl_return_hash_output(unsigned int *chain,
											int pieces, int reverse_bytes);
unsigned char			*ftssl_return_hash_output_512(uint64_t *chain, int pieces,
												int reverse_bytes, int last_half);
size_t					ftssl_md5_pad_input(unsigned char *input,
											unsigned char **padded,
											int sha_mode);
size_t					ftssl_md5_pad_input_512(unsigned char *input,
											unsigned char **padded);

void					ftssl_des_family_wrapper(char *command_name,
												int argc, char **argv);

void					ftssl_md5_free_args(t_ftssl_md5_args *args);
t_ftssl_md5_args		*ftssl_md5_get_args(int argc, char **argv);

void					ftssl_md5_rounds(unsigned int *c, unsigned int *in,
										unsigned int *order);

void					ftssl_md5_outer(t_ftssl_md5_args *args);

unsigned char			*ftssl_sha256_algorithm(unsigned char *inut,
												size_t input_len,
												unsigned int sha_version);
unsigned char			*ftssl_sha512_algorithm(unsigned char *inut,
												size_t input_len,
												unsigned int sha_version);

/*
**DES/Encryption-related stuff
*/

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
typedef	int				t_ftssl_comm_func(t_ftssl_args args,
									const unsigned char *input,
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

/*
**Args
*/

t_ftssl_args			ftssl_get_args(int argc, char **argv);
void					ftssl_destroy_args(t_ftssl_args args);

t_ftssl_opthelp			ftssl_opthelp_init(int argc, char **argv, char flag,
											char *label);
void					ftssl_opthelp_destroy(t_ftssl_opthelp opt);
char					*ftssl_find_optvalue(t_ftssl_opthelp opt,
											char *command);

int						ftssl_find_comm_key(char *command_name);
void					ftssl_prep_args(t_ftssl_args *args);

/*
**padding?
*/

unsigned char			*ftssl_padblock_ecb(unsigned char *block,
											int cursize, int fullsize);
/*
**error handling
*/

void					ftssl_flag_arg_error(const char *command,
											const char *option);
void					ftssl_nocommand_error(void);
void					ftssl_invalid_command_error(const char *command);
void					ftssl_file_open_error(const char *filename,
											int permissions);
void					ftssl_invalid_hexkey_error(void);

/*
**base64
*/

int						ftssl_base64(t_ftssl_args args,
									const unsigned char *input,
									unsigned char *out, int len);
int						ftssl_base64_encode(const unsigned char *input,
											unsigned char *out, int len);
int						ftssl_base64_decode(const unsigned char *input,
											unsigned char *out, int len);
/*
**DES
*/

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
/*
**Triple-DES
*/

int						ftssl_des3_ecb(t_ftssl_args args,
										const unsigned char *input,
										unsigned char *output, int len);
int						ftssl_des3_cbc(t_ftssl_args args,
										const unsigned char *input,
										unsigned char *output, int len);

#endif
