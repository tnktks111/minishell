#include "wildcard.h"

void	ft_int_array_swap(int *a, int *b, size_t size);
void	ft_init_int_array(int *arr, size_t size);
void	init_dp_table(int *dp, int *is_wildcard, size_t size);
int		ft_ismatch(char *str, char *pattern, int *is_wildcard, size_t pat_len);

void	ft_int_array_swap(int *a, int *b, size_t size)
{
	size_t	i;
	int		tmp;

	i = 0;
	while (i < size)
	{
		tmp = a[i];
		a[i] = b[i];
		b[i] = tmp;
		i++;
	}
}

void	ft_init_int_array(int *arr, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		arr[i++] = 0;
}

void	init_dp_table(int *dp, int *is_wildcard, size_t size)
{
	size_t	i;

	dp[0] = 1;
	i = 0;
	while (++i < size)
	{
		if (!is_wildcard[i - 1])
			break ;
		dp[i] = 1;
	}
}

int	ft_ismatch(char *str, char *pattern, int *is_wildcard, size_t pat_len)
{
	int		dp[PATH_MAX + 1];
	int		next_dp[PATH_MAX + 1];
	size_t	i;
	size_t	j;

	ft_init_int_array(dp, PATH_MAX + 1);
	init_dp_table(dp, is_wildcard, pat_len + 1);
	i = 0;
	while (++i < ft_strlen(str) + 1)
	{
		ft_init_int_array(next_dp, pat_len + 1);
		j = 0;
		while (++j < pat_len + 1)
		{
			if (pattern[j - 1] == '*' && is_wildcard[j - 1])
				next_dp[j] = (next_dp[j - 1] || dp[j]);
			else if (str[i - 1] == pattern[j - 1])
				next_dp[j] = dp[j - 1];
		}
		ft_int_array_swap(dp, next_dp, pat_len + 1);
	}
	return (dp[pat_len]);
}
