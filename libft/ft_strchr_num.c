/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <abouabra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 13:26:06 by abouabra          #+#    #+#             */
/*   Updated: 2023/06/01 17:39:55 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strchr_num(char *s, int c)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (*(unsigned char *)s && *(unsigned char *)s != (unsigned char)c)
	{
		s++;
		i++;
	}
	if (*(unsigned char *)s == (unsigned char)c)
		return (i);
	return (0);
}
