/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 21:00:07 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/16 21:07:47 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_HASH_MD5_H
# define FT_SSL_HASH_MD5_H

/*
**512 bits, so 64 bytes
*/
# define BLOCK_SIZE 64

struct	s_md5_state {
	uint32_t state[4];
	uint32_t working[4];
}		t_md5_state;

#endif
