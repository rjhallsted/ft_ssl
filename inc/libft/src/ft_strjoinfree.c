/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinfree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/15 22:28:44 by rhallste          #+#    #+#             */
/*   Updated: 2018/11/07 18:44:23 by rhallste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/libft.h"

char	*ft_strjoinfree(char *s1, char *s2, int option)
{
	char *new;

	new = ft_strjoin(s1, s2);
	if (option == 1 || option == 3)
		free(s1);
	if (option == 2 || option == 3)
		free(s2);
	return (new);
}
