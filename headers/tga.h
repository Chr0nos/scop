/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 18:18:32 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/07 00:33:17 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TGA_H
# include <GL/glew.h>
# define TGA_H
# define TGA_RED				0x0000ff00
# define TGA_GREEN				0x00ff0000
# define TGA_BLUE				0xff000000
# define TGA_ALPHA				0x000000ff
# define TGA_TYPE_TC_RAW		2

#pragma pack(push, 1)

typedef struct		s_tga
{
	char			id;
	char			color_map;
	char			type;
	unsigned short	first_entry_index;
	unsigned short	color_map_len;
	unsigned char	bpp;
	unsigned short	x_offset;
	unsigned short	y_offset;
	unsigned short	width;
	unsigned short	height;
	unsigned char	depth;
	unsigned char	descriptor;
	char			drop[3];
}					t_tga;

#pragma pack(pop)

#define TGA_SIZE	sizeof(t_tga)

unsigned int	*load_tga(const char *filepath, t_tga *specs);
GLuint			load_ogl_tga(const char *filepath);

#endif
