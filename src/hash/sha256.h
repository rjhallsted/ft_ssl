/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:25:10 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 16:11:33 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_HASH_SHA256_H
# define FT_SSL_HASH_SHA256_H

# include <stdint.h>

/*
**512 bits, so 64 bytes
*/
# define BLOCK_SIZE_SHA256 64

typedef struct	s_sha256_state {
	uint32_t	state[8];
	uint32_t	working[8];
	uint32_t	*words;
}				t_sha256_state;

void			ftssl_sha256(char *command, int argc, char **argv);
void			sha256_rounds(t_sha256_state *state);
void			ftssl_sha224(char *comman, int argc, char **argv);

#endif
