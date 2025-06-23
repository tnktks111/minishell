/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_tmpfile.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:45:34 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/22 20:45:51 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	file_iswdir(char *filepath);
static int	get_random_string(char *buffer);
static char	*get_sys_tmpdir(void);
static char	*get_tmpdir(void);
static char	*sh_mktmpname(void);
int			sh_mktmpfd(char **file_path_ptr);

int	file_iswdir(char *filepath)
{
	struct stat	sb;

	if (stat(filepath, &sb))
		return (0);
	return (S_ISDIR(sb.st_mode) && access(filepath, W_OK) == 0);
}

static int	get_random_string(char *buffer)
{
	static const char	charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	static const int	charset_size = sizeof(charset) - 1;
	char				random_bytes[RANDOM_SIZE];
	int					urandom_fd;
	int					i;

	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd == -1)
	{
		perror("Error opening /dev/urandom");
		return (EXIT_FAILURE);
	}
	if (read(urandom_fd, random_bytes, RANDOM_SIZE) != RANDOM_SIZE)
	{
		perror("Error reading from /dev/urandom");
		close(urandom_fd);
		return (EXIT_FAILURE);
	}
	close(urandom_fd);
	i = -1;
	while (++i < RANDOM_SIZE)
		buffer[i] = charset[(unsigned char)random_bytes[i] % charset_size];
	buffer[RANDOM_SIZE] = 0;
	return (EXIT_SUCCESS);
}

static char	*get_sys_tmpdir(void)
{
	static char	*sys_tmpdir = NULL;

	if (sys_tmpdir)
		return (sys_tmpdir);
	sys_tmpdir = "/tmp";
	if (file_iswdir(sys_tmpdir))
		return (sys_tmpdir);
	sys_tmpdir = "/var/tmp";
	if (file_iswdir(sys_tmpdir))
		return (sys_tmpdir);
	sys_tmpdir = "/usr/tmp";
	if (file_iswdir(sys_tmpdir))
		return (sys_tmpdir);
	sys_tmpdir = DEFAULT_TMPDIR;
	return (sys_tmpdir);
}

static char	*get_tmpdir(void)
{
	char	*tmpdir;

	tmpdir = getenv("TMPDIR");
	if (!tmpdir || (file_iswdir(tmpdir) == 0 || ft_strlen(tmpdir) > PATH_MAX
			- RANDOM_SIZE))
		tmpdir = get_sys_tmpdir();
	return (tmpdir);
}

static char	*sh_mktmpname(void)
{
	char	*filepath;
	char	*tmp;
	char	buffer[RANDOM_SIZE + 1];
	char	*tmpdir;

	if (get_random_string(buffer) == EXIT_FAILURE)
		return (NULL);
	tmpdir = get_tmpdir();
	tmp = ft_strjoin(tmpdir, "/");
	if (!tmp)
	{
		perror("malloc :");
		return (NULL);
	}
	filepath = ft_strjoin(tmp, buffer);
	if (!filepath)
	{
		perror("malloc :");
		return (free(tmp), NULL);
	}
	free(tmp);
	return (filepath);
}

int	sh_mktmpfd(char **file_path_ptr)
{
	char	*file_path;
	int		fd;

	file_path = sh_mktmpname();
	fd = open(file_path, O_RDWR | O_CREAT | O_EXCL, 0644);
	while (fd < 0 && errno == EEXIST)
	{
		free(file_path);
		file_path = sh_mktmpname();
		if (!file_path)
		{
			perror("malloc :");
			return (-1);
		}
		fd = open(file_path, O_RDWR | O_CREAT | O_EXCL, 0644);
	}
	if (fd < 0)
	{
		perror("open :");
		free(file_path);
		return (-1);
	}
	printf("%s\n", file_path);
	*file_path_ptr = file_path;
	return (fd);
}
