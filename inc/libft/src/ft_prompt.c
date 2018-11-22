/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 22:56:58 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/21 21:34:34 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <libft.h>
#include <ft_printf.h>
#include <get_next_line.h>

char	*ft_prompt(const char *prompt)
{
	char *response;

	ft_printf("%s", prompt);
	get_next_line(STDIN_FILENO, &response);
	return (response);
}
