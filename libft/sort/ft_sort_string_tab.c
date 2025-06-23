/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_string_tab.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:57:25 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/23 13:43:45 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

static int ft_str_tab_len(char **strarr);
static void ft_swap(char **a, char **b);
static int partition(char *array[], int left, int right, int(*cmp)(char *, char *));
static void quick_sort(char *array[], int left, int right, int(*cmp)(char *, char *));
void ft_sort_string_tab(char **tab, int(*cmp)(char *, char *));

static int ft_str_tab_len(char **strarr)
{
	int i;

	i = 0;
	while (strarr[i])
		i++;
	return (i);
}

static void ft_swap(char **a, char **b)
{
    char *tmp = *a;
    *a = *b;
    *b = tmp;
}

static int partition(char *array[], int left, int right, int(*cmp)(char *, char *))
{
	int i = left;
	int j = right + 1;
	char *pivot;
	
	pivot = array[left];
	while (i < j)
	{
		while (++i <= right && cmp(array[i], pivot) < 0);
		while (--j >= left && cmp(array[j], pivot) > 0);
		if (i >= j) break;
		ft_swap(&array[i], &array[j]);
	}
	ft_swap(&array[left], &array[j]);
	return j;
}

static void quick_sort(char *array[], int left, int right, int(*cmp)(char *, char *)) {
    if (left < right) {
        int q = partition(array, left, right, cmp);
        quick_sort(array, left, q - 1, cmp);
        quick_sort(array, q + 1, right, cmp);
    }
}

void ft_sort_string_tab(char **tab, int(*cmp)(char *, char *))
{
	int size;

	size = ft_str_tab_len(tab);
	quick_sort(tab, 0, size - 1, cmp);
}
