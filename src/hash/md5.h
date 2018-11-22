/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 21:00:07 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 16:13:41 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_HASH_MD5_H
# define FT_SSL_HASH_MD5_H

# include <stdint.h>

/*
**512 bits, so 64 bytes
*/
# define BLOCK_SIZE_MD5 64

typedef struct	s_md5_state {
	uint32_t	state[4];
	uint32_t	working[4];
}				t_md5_state;

void			md5_rounds(const unsigned int *block, t_md5_state *state);
void			ftssl_md5(char *command, int argc, char **argv);

#endif
