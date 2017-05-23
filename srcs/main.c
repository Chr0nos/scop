/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/23 14:33:03 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void				error_handler(int id, const char *str)
{
	ft_dprintf(2, "[%2d] error: %s\n", id, str);
}

t_vertex_pack		*get_pack(t_vertex_pack *pack)
{
	static t_vertex_pack	*stored_pack = NULL;

	if (pack)
	{
		stored_pack = pack;
		ft_printf("storing pack address %p\n", pack);
	}
	return (stored_pack);
}

int					main(int ac, char **av)
{
	if (ac < 2)
		return (1);
	if (!glfwInit())
		return (11);
	ft_printf("Init ok\nGlfw version: %s\n", glfwGetVersionString());
	return (run_parse(av[1], (ac > 2) ? av[2] : NULL));
}
