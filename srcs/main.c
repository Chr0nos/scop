/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 01:58:22 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void				error_handler(int id, const char *str)
{
	ft_dprintf(2, "[%2d] error: %s\n", id, str);
}

int					main(int ac, char **av)
{
	if (ac < 2)
	{
		ft_dprintf(2, "error: missing parameter(s)\n");
		return (1);
	}
	ft_bzero(&g_pack, sizeof(t_vertex_pack));
	if (!glfwInit())
		return (11);
	ft_printf("Init ok\nGlfw version: %s\n", glfwGetVersionString());
	return (run_parse(ac - 1, &av[1]));
}
