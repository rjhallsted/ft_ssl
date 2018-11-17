/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 20:34:18 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/16 20:52:56 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_HASH_ARGS_H
# define FT_SSL_HASH_ARGS_H

typedef struct	s_hash_args {
	char		*command;
	int			print_input;
	int			quiet_mode;
	int			reverse_output;
	int			string_mode;
	char		*input_string;
	char		**filenames;
	int			*fds;
	int			fd_count;
	int			read_stdin;
	int			*error_indices;
	int			error_count;
}				t_hash_args;

void			free_args(t_hash_args *args);
t_hash_args		*get_args(int argc, char **argv);

#endif
