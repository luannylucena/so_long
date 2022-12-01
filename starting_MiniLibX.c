#include <stdlib.h>
#include "minilibx-linux/mlx.h"
#include <X11/keysym.h>

//gcc main.c minilibx-linux/libmlx.a minilibx-linux/libmlx_Linux.a -lX11 -lXext -Imlx -lXext


//int main(void)
//{
//	void	*mlx_ptr = mlx_init();
//	void	*win_ptr = mlx_new_window(mlx_ptr, 200, 200, "aaaa");
//	mlx_loop(mlx_ptr);
//}

# define WINDOW_WIDTH 600 //largura
# define WINDOW_HEIGHT 300 //altura
# define MLX_ERROR 1 

int main(void)
{
	void	*mlx_ptr;
	void	*win_ptr;

	mlx_ptr = mlx_init();
	if (mlx_ptr == NULL)
		return (MLX_ERROR);
	win_ptr = mlx_new_window(mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "My first window!");
	if (win_ptr == NULL)
	{
		free(win_ptr);
		return (MLX_ERROR);
	}
	mlx_loop(mlx_ptr); // p/ manter a janela aberta
	mlx_destroy_window(mlx_ptr, win_ptr); //essa função evita vazamentos, precisa ser usada antes do 'free'.
	mlx_destroy_display(mlx_ptr);
	free(mlx_ptr);
}

