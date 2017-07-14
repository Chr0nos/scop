/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/18 14:24:21 by snicolet          #+#    #+#             */
/*   Updated: 2015/12/15 10:49:08 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int				ft_atoi(const char *str)
{
	int		nb;
	char	negative;

	nb = 0;
	negative = 0;
	while ((ft_strany(*str, " \t\n\v\f\r")) && (*str))
		str++;
	if (*str == '+')
		str++;
	else if ((*str == '-') && (*(str++)))
		negative = 1;
	while (ft_isdigit(*str))
		nb = (nb * 10) + (*(str++) - '0');
	return ((negative) ? (nb * -1) : nb);
}
