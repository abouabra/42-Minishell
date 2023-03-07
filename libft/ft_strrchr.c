/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhayri <ykhayri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 13:26:21 by abouabra          #+#    #+#             */
/*   Updated: 2023/03/05 18:21:20 by ykhayri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(char *str, int c)
{
	int	i;

	if (!str)
		return (NULL);
	i = ft_strlen(str);
	while (i != 0 && ((unsigned char *)str)[i] != (unsigned char)c)
		i--;
	if (((unsigned char *)str)[i] == (unsigned char)c)
		return ((char *)&str[i]);
	return (0);
}
