/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_with_MiniLibX.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luanny <luanny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 12:34:20 by lmedeiro          #+#    #+#             */
/*   Updated: 2022/12/02 13:37:03 by luanny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minilibx-linux/mlx.h"
#include <X11/keysym.h>
#include <X11/X.h>

# define WINDOW_WIDTH 600 //largura
# define WINDOW_HEIGHT 300 //altura

# define MLX_ERROR 1 

typedef struct s_data
{
	void *mlx_ptr;
	void *win_ptr;
}
t_data;
//O objeto 't-data' é um endereço de dados. Contém todas as coisas que precisamos acessar dentro do handle_input.

int handle_no_event(void *data)
{
	return (0);
}

//O 'handle_input' será acionado sempre que uma tecla for liberada. Seu dever é verificar qual tecla foi liberada e fazer as 
//coisas de acordo.
//int handle_input(int keysym, t_data *data)
//{
//	if (keysym == XK_Escape)
//		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
//	return (0);
//}

int handle_keypress(int keysym, t_data *data)
{
    if (keysym == XK_Escape)
        mlx_destroy_window(data->mlx_ptr, data->win_ptr);

    printf("Keypress: %d\n", keysym);
    return (0);
}

int handle_keyrelease(int keysym, void *data)
{
    printf("Keyrelease: %d\n", keysym);
    return (0);
}

int main(void)
{
   
	t_data data;

	data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (MLX_ERROR);
	data.win_ptr = mlx_new_window(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Second Window");
	if (data.win_ptr == NULL)
	{
		free(data.win_ptr);
		return (MLX_ERROR);
	}

	//o 'mlx_loop_hook' é acionado qdo não acontece nenhum evento.
	mlx_loop_hook(data.mlx_ptr, &handle_no_event, &data);
    
    //para obter o evento adeaquado, usamos a função 'mlx_key_hook'.
	//mlx_key_hook(data.win_ptr, &handle_input, &data);
    //( a partir de agora, substituimos por mlx_hook);
    mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &handle_keypress, &data);
    mlx_hook(data.win_ptr, KeyRelease, KeyReleaseMask, &handle_keyrelease, &data);
    //Essa função nunca retorna. É um loop infinito que espera por um evento e, em
    //seguida, chama uma função definida pelo usuário associada a esse evento. Um único parâmetro
    //é necessário: mlx_ptr.
	mlx_loop(data.mlx_ptr);

	//sairemos do loop se não houver nenhuma janela restante e executaremos este código
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
}
