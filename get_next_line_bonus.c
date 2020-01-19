/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablomar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 14:08:40 by pablomar          #+#    #+#             */
/*   Updated: 2019/12/05 19:22:32 by pablomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void			ft_del(char **str)
{
	if (str != NULL && *str != NULL)
	{
		free(*str);
		*str = NULL;
	}
}

static char			*ft_strchr(const char *s, int c)
{
	if (s == NULL)
		return (NULL);
	while (*s)
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (unsigned char)c)
		return ((char *)s);
	return (NULL);
}

static int			create_line(char **sub, char **line, int fd)
{
	int		len;
	char	*temp;

	len = 0;
	while (sub[fd][len] != '\n' && sub[fd][len] != '\0')
		len++;
	*line = ft_substr(sub[fd], 0, len);
	temp = ft_strdup(&sub[fd][len + 1]);
	free(sub[fd]);
	sub[fd] = temp;
	return (1);
}

static int			output(int bytes_read, int fd, char **sub, char **line)
{
	if (bytes_read < 0)
		return (-1);
	else if (bytes_read == 0 && (sub[fd] == NULL || sub[fd][0] == '\0'))
	{
		*line = ft_strdup("");
		ft_del(&sub[fd]);
		return (0);
	}
	else if (ft_strchr(sub[fd], '\n'))
		return (create_line(sub, line, fd));
	else
	{
		*line = ft_strdup(sub[fd]);
		ft_del(&sub[fd]);
		return (0);
	}
}

int					get_next_line(int fd, char **line)
{
	static char	*sub[FD_MAX];
	int			bytes_read;
	char		*buffer;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0 || line == NULL)
		return (-1);
	if (!(buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE + 1)))
		return (-1);
	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[bytes_read] = '\0';
		if (sub[fd] == NULL)
			sub[fd] = ft_strdup(buffer);
		else
		{
			temp = ft_strjoin(sub[fd], buffer);
			free(sub[fd]);
			sub[fd] = temp;
		}
		if (ft_strchr(sub[fd], '\n'))
			break ;
	}
	free(buffer);
	return (output(bytes_read, fd, sub, line));
}
