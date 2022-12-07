#include	<stdlib.h> 
#include	<mlx.h>
#include	<fcntl.h>
#include	<X11/keysym.h>
#include	"GNL/get_next_line.h"
#include	"Defines.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define PIXEL_SIZE 32

struct	s_mlx
{
	void	*mlx_ptr;
	void	*win_ptr;
	char	**map;
	void	*player;
	void	*player_left;
	void	*player_right;
	void	*player_down;
	void	*player_up;
	void	*collec;
	void	*exit_c;
	void	*exit_o;
	void	*wall;
	void	*floor;
	int		stars;
	int		movements;
	int		col;
	int		line;
	int		player_line;
	int		player_col;
};
typedef struct s_mlx	t_mlx;

int		ft_show_map(t_mlx *window);
int		ft_close(t_mlx *window);
char	*ft_itoa(int x);
void	ft_locate_player(t_mlx *window);
void	ft_free_matrix(char **matrix);
int		ft_check_exit(int exit, int collec);
int		ft_check_characters(t_mlx *window, int e, int p, int invalid);

// Função pra printar erros =D
void	ft_print_error(char *error)
{
	write (1, error, ft_strlen(error)); // Sem segredos
}

// Função pra contar a quantidade de colunas da linha (ou a largura total, se preferir)
int	count_col(char *temp)
{
	int	i;

	i = 0;
	while (temp != NULL && temp[i] != 0 && temp[i] != '\n' && temp[i] != '\r') // Conto enquanto ele for diferente dos caracteres inválidos
		i++;
	return (i);
}

// Função pra fazer nosso amigo Homarinho andar na tela
void	ft_event_arrow(t_mlx *window, int col, int line)
{
	ft_locate_player(window); // Localizo a pocição do jogador
	if (window->map[window->player_line + line][window->player_col + col] == '0') // Pergunto se a próxima posição é um 0
	{
		window->map[window->player_line + line][window->player_col + col] = 'P'; // Se for, atualizo a posição do jogador
		window->map[window->player_line][window->player_col] = '0'; // E faço onde ele estava antes ser igual a 0
		window->movements ++; // Incremento a quantidade de movimentos
	}
	if (window->map[window->player_line + line][window->player_col + col] == 'C') // Pergunto se a próxima posição é um C
	{
		window->map[window->player_line + line][window->player_col + col] = 'P'; // Atualizar P
		window->map[window->player_line][window->player_col] = '0'; // E faço onde ele estava antes ser igual a 0
		window->stars --;
		window->movements ++; // Incremento a quantidade de movimentos
	}
	if (window->map[window->player_line + line][window->player_col + col] == 'E' \
		&& window->stars == 0) // Pergunto se a próxima posição é um E e todos os coletáveis foram pegos
	{
		window->map[window->player_line][window->player_col] = '0'; // Apago o jogador da tela
		window->movements ++; // Incremento a quantidade de movimentos
		ft_print_error("\n***You are the champion!***\n\n"); // YOU WIN!!!
		ft_close(window); // Finalizo o game
	}
}

// Função pra contar coletáveis
int	ft_count_collectibles(t_mlx *window)
{
	int	x;
	int	y;

	window->stars = 0; // Inicializo a variável dos coletáveis
	y = 0;
	while (window->map[y] != NULL) // Vou percorrer todas as linhas
	{
		x = 0;
		while (window->map[y][x] != '\0') // E todas as colunas
		{
			if (window->map[y][x] == 'C') // Se um coletável for encontrado
			{
				window->stars = window->stars + 1; // Atualizo meu contador de coletáveis
			}
			x++;
		}
		y++;
	}
	if (window->stars == 0) // Se não existe coletáveis, retorna erro
		return (1);
	else
		return (0); // Se não, tá com 0 erros
}

// Função para colocar as imagens na janela (Criada pra evitar a linha longa com mais de 80 caracteres)
void	ft_put_image_to_window(t_mlx *window, void *image, int x, int y)
{
	mlx_put_image_to_window(window->mlx_ptr, window->win_ptr, \
		image, PIXEL_SIZE * x, PIXEL_SIZE * y); // Coloca a imagem que a gente mandar como argumento na posição x e y certas na tela. (pixel size é pra ajustar ao tamanho da imagem)
}

int	ft_show_moves(t_mlx *window)
{
	char	*movements;

	mlx_string_put(window->mlx_ptr, window->win_ptr, 12, 16, 0x0100ff00, "Moves: ");
	movements = ft_itoa(window->movements);
	mlx_string_put(window->mlx_ptr, window->win_ptr, 60, 16, 0x0100ff00, movements);
	free(movements);
	return (0);
}

// Função pra colocar todas as imagens na tela, na ordem certa
int	ft_show_map(t_mlx *window)
{
	int	y; // linhas
	int	x; // colunas

	y = -1;
	while (window->map[++y] != NULL) // Percorremos todas as linhas ++y ficou aqui por conta das 25 linhas
	{
		x = 0;
		while (x < window->col) // Percorremos todas as colunas
		{
			if (window->map[y][x] == '1') // Se a posição atual for 1
				ft_put_image_to_window(window, window->wall, x, y); // Coloca a imagem da parede
			else if (window->map[y][x] == 'P') // Se a posição atual for P
				ft_put_image_to_window(window, window->player, x, y); // Coloca a imagem do Homarinho
			else if (window->map[y][x] == 'C') // Se a posição atual for C
				ft_put_image_to_window(window, window->collec, x, y); // Coloca a imagem da estrela
			else if (window->map[y][x] == 'E' && window->stars != 0) // Aqui você verifica se tem coletáveis
				ft_put_image_to_window(window, window->exit_c, x, y); // E assim por diante
			else if (window->map[y][x] == 'E' && window->stars == 0) // Aqui você verifica se NÃO TEM coletáveis
				ft_put_image_to_window(window, window->exit_o, x, y);
			else if (window->map[y][x] == '0')
				ft_put_image_to_window(window, window->floor, x, y);
			x++;
		}
	}
	return (ft_show_moves(window));
}

// Função que cria o mapa na variável Map (da struct)
int	ft_create_map(t_mlx *window, char *path)
{
	int		y; // contador pra percorrer as linhas
	int		fd; // file descriptor (id do arquivo)
	char	*temp; 

	y = 0;
	fd = open(path, O_RDONLY); // Abro o arquivo com o caminho que foi informado pelo usuário
	if (fd < 0) // Se ele não existe, erro
		ft_print_error(error_2"\n");
	temp = get_next_line(fd); // Leio a primeira linha
	if (temp == NULL) // Verifico se não é um arquivo vazio, se for, retorna erro
		return (1);
	window->map = malloc((window->line + 1) * sizeof(char *)); // Se deu bão, malloco as linhas que preciso pra ter meu mapa
	if (window->map == NULL) // se não tem memória no sistema, return 1
		return (1);
	while (y < window->line) // Percorro o tamanho de linhas que possuo
	{
		window->map[y] = temp; // Coloco a linha lida na matriz
		y++; // incremento a linha
		temp = get_next_line(fd); // Leio a próxima linha
		if (y < window->line && temp == NULL) // Verifico se deu algo errado. Se sim, return 1
			return (1);
	}
	close (fd); // Fecho o arquivo, pra não dar B.O.
	window->map[y] = NULL; // Coloco o NULL na última linha
	return (0);
}

// Função pra verificar se há caracteres inválidos no mapa
int	ft_valid_character(t_mlx *window)
{
	int	y; // linhas
	int	x; // colunas
	int	p; // Conta players
	int	e; // Conta saídas
	int	invalid; // Conta caracteres inválidos

	p = 0;
	e = 0;
	invalid = 0;
	y = -1; // Vamos percorrer 100% da matriz (pois dá pra mostrar todos os erros de uma vez)
	while (window->map[++y] != NULL) // percorreremos todas as linhas
	{
		x = -1;
		while (++x < window->col) // percorreremos até a coluna máxima válida (ainda tem o \r e o \n na linha toda)
		{
			if (window->map[y][x] == 'P')
				p++;
			else if (window->map[y][x] == 'E')
				e++;
			else if (window->map[y][x] != 'C' && window->map[y][x] != '0' \
				&& window->map[y][x] != '1')
				invalid++;
		}
	}
	return (ft_check_characters(window, e, p, invalid));
}

int	ft_check_characters(t_mlx *window, int e, int p, int invalid)
{
	if (ft_count_collectibles(window) == 1) // Conto os coletáveis e se der ruim, mostro a mensagem de erro
		ft_print_error(error_9"\n");
	if (e != 1) // Verificando se a saída está com valor inválido
		ft_print_error(error_8"\n");
	if (p != 1) // Verificando se o player está com valor inválido 
		ft_print_error(error_7"\n");
	if (invalid > 0) // Se tiver caracter inválido, é game over
		ft_print_error(error_10"\n");
	if (window->stars == 0 || e != 1 || p != 1 || invalid > 0) // Se houve algum dos erros anteriores, return 1
		return (1);
	return (0);
}

// Função pra checar se tem alguma saída ou coletável que não pode ser alcançado
int	ft_check_path(char **copy, t_mlx *window)
{
	int	line;
	int	col;
	int	exit; // Checa se um 'E' foi encontrado
	int	collec; // Checa se um 'C' foi encontrado

	line = 0;
	exit = 0;
	collec = 0;
	col = 0;
	while (copy[line] != NULL) // Percorreremos todas as linhas
	{
		col = 0;
		while (col < window->col) // Percorreremos todas as colunas
		{
			if (copy[line][col] == 'E' && exit == 0) // Se um 'E' for encontrado, deu ruim. Mas não para a busca
				exit = 1;
			if (copy[line][col] == 'C' && collec == 0) // Se um 'C' for encontrado, deu ruim. Mas não para a busca
				collec = 1;
			col++;
		}
		line++;
	}
	return (ft_check_exit(exit, collec));
}

int	ft_check_exit(int exit, int collec)
{
	if (exit != 0) // Se um 'E' foi encontrado, error
		ft_print_error(error_5"\n");
	if (collec != 0) // Se um 'C' foi encontrado, error
		ft_print_error(error_6"\n");
	if (exit != 0 || collec != 0) // Se houve algum erro, return 1
		return (1);
	return (0);
}

// Função pra validar se as bordas são paredes
int	ft_valid_walls(t_mlx *window)
{
	int	x; // Variável usada pra percorrer
	int	max; // Verifico se há mais colunas ou há mais linhas (isso é por conta de fazer tudo num único while)

	x = 0;
	max = window->col; // Se o número de colunas for maio que o de linhas o mapa é um retângulo horizontal
	if (window->col < window->line) // Senão, ele é um retângulo vertical
		max = window->line;
	while (x < max) // Percorro o mapa pelas bordas. Se algum caracter for diferente de '1' paro a brincadeira na hora
	{
		if (window->col - x > 0)
			if (window->map[0][x] != '1' || \
				window->map[window->line - 1][x] != '1') // Aqui verifico as colunas da primeira e a última linha ( nesse sentido ->)
				break ;
		if (window->line - x > 0)
			if (window->map[x][0] != '1' || \
				window->map[x][window->col - 1] != '1') // Aqui verifico as linhas da primeira e da última couna (do topo pro fim)
				break ;
		x++;
	}
	if (x != max) // Se ele não chegou no final, erro!
	{
		ft_print_error(error_4"\n");
		return (1);
	}
	return (0);
}

// Função para localizar o jogador no mapa
void	ft_locate_player(t_mlx *window)
{
	int	x; // colunas
	int	y; // linhas

	y = 0;
	while (window->map[y] != NULL) // Percorreremos todas as linhas
	{
		x = 0;
		while (window->map[y][x] != '\0') // Percorreremos todas as colunas
		{
			if (window->map[y][x] == 'P') // Se o 'P' for encontrado
			{
				window->player_line = y; // Armazena a linha na variável da struct
				window->player_col = x; // Armazena a coluna na variável da struct
				return ; // Para a busca. Afinal, achamos o jogador
			}
			x++;
		}
		y++;
	}
}

// Função pra testar se o jogador consegue acessar todos os coletáveis e saída
void	ft_test_path(char **copy, int line, int col, t_mlx *window)
{
	if (copy[line][col] == 'X' || copy[line][col] == '1' || copy[line][col] == 'S') // se algum desses caracteres for encontrado, pára a busca
		return ;
	if (copy[line][col] == 'E') // Se um 'E' for encontrado, substitui por 'S' (afinal, a gente não atravessa a saída)
	{
		copy[line][col] = 'S';
		return ; // Retorno pra não ter que mudar mais nada
	}
	else
		copy[line][col] = 'X'; // Troco o que estiver na posição onde estou atualmente por 'X'
	ft_test_path(copy, line - 1, col, window); // Chamada recursiva pra verificar a linha de cima
	ft_test_path(copy, line + 1, col, window); // Chamada recursiva pra verificar a linha de baixo
	ft_test_path(copy, line, col - 1, window); // Chamada recursiva pra verificar a coluna da esquerda
	ft_test_path(copy, line, col + 1, window); // Chamada recursiva pra verificar a coluna da direita
}

// Função para criar uma cópia do mapa
char	**ft_dup_map(t_mlx *window)
{
	char	**copy; // A matriz que vai receber a cópia
	int		y; // Linhas

	copy = malloc((window->line + 1) * sizeof(char *)); // Malloco o tanto de linhas que vou precisar
	y = 0;
	while (window->map[y] != NULL) // Percorreremos até a última linha
	{
		copy[y] = ft_strdup(window->map[y]); // Faço uma cópia da linha do mapa para a linha da variável copy
		if (copy[y] == NULL) // Verificação pro caso de algum Vinícius avaliando
		{
			ft_free_matrix(copy); // Limpa a matriz
			return (NULL); // retorna NULL pra eu me ferrar depois (se esse caso acontecer)
		}
		copy[y][window->col] = '\0'; // Ajuste bonitinho pra não ter dor de cabeça (ainda existe o \n e o \r na linha, eles precisam ser ignorados)
		y++;
	}
	copy[y] = NULL; // A última linha não foi preenchida com nada, por isso, colocamos o NULL nela
	return (copy); // Retorna a matriz
}

// Função só por causa de norma (printa erro, dá free e fecha o arquivo)
int	ft_close_and_free(int fd, char *temp)
{
	ft_print_error(error_3"\n");
	free(temp);
	close(fd); // Fecha o arquivo, pra não dar problema
	return (1);
}

// Função pra contar as linhas do mapa
int	ft_count_lines(int fd, char *temp, t_mlx *window)
{
	int		count_line_break; // Contador de \n (pra validarmos que o mapa está certinho)

	count_line_break = 0;
	window->line = 0;
	window->col = count_col (temp); // Se tudo estiver certo até agora, conto a quantidade de colunas que tem o mapa
	while (temp != NULL) // Percorreremos todas as linhas do arquivo
	{
		if (temp[0] != '\n' && count_line_break == 0) // Se tem mapa pra ser lido e não teve \n até agora, linha++ (contamos quantas linhas tem o mapa)
			window->line++;
		else if (temp[0] != '\n' && count_line_break != 0) // Se teve pelo menos um \n e algo diferente surgir no meio do caminho (um abismo) error
			return (ft_close_and_free(fd, temp));
		else if (temp[0] == '\n') // Se tiver \n só conta
			count_line_break++;
		free(temp); // Free pra evitar erro de Valgrind
		temp = get_next_line(fd); // Leio a próxima linha
		if (temp && temp[0] != '\n' && window->col != count_col(temp)) // Verifico se a próxima linha tem tamanho diferente da minha primeira linha. se sim, erro
			return (ft_close_and_free(fd, temp));
	}
	close (fd); // Fecha o arquivo
	return (0);
}

//Função que vai ler o mapa pra contar quantas linhas e colunas teremos
int	ft_read_map(t_mlx *window, char *path)
{
	int		fd;	// File descriptor do arquivo (qual o "CPF" do arquivo)
	char	*temp; // Temporária que guarda as linhas temporariamente

	fd = open(path, O_RDONLY); // Abrimos o arquivo
	if (fd < 0) // Se deu algum erro ao abrir, entra no if
	{
		ft_print_error(error_2"\n");
		return (1);
	}
	temp = get_next_line(fd); // Lemos a primeira linha
	if (temp == NULL) // Se não tem texto (mapa vazio)
		ft_print_error(error_11"\n");
	if (temp[0] != '1') // A primeira linha não começar com um '1' já tá errado
		ft_print_error(error_3"\n");
	if (temp == NULL || temp[0] != '1') // Manobra pra economizar linhas
		return (1);
	if (ft_count_lines(fd, temp, window) == 1) // Aqui ele lê o resto do arquivo e conta as linhas (chamando a função)
		return (1);
	if (window->col < 5 || window->col > 37 || \
		window->line < 3 || window->line > 28) // Verifico se ele ultrapassa o o limite que defini pra janela, ou se o mapa é muito pequeno
	{
		ft_print_error(error_3"\n");
		return (1);
	}
	return (0);
}

// Fecha tudo e da free nas coisas ("You are the Champion")
int	ft_close(t_mlx *window)
{
	mlx_destroy_image(window->mlx_ptr, window->player_left); // Dá free em todos os sprites (esse é o jogador pra esquerda)
	mlx_destroy_image(window->mlx_ptr, window->player_right); // Jogador pra direita
	mlx_destroy_image(window->mlx_ptr, window->player_down); // E assim vai
	mlx_destroy_image(window->mlx_ptr, window->player_up);
	mlx_destroy_image(window->mlx_ptr, window->collec);
	mlx_destroy_image(window->mlx_ptr, window->exit_c);
	mlx_destroy_image(window->mlx_ptr, window->exit_o);
	mlx_destroy_image(window->mlx_ptr, window->wall);
	mlx_destroy_image(window->mlx_ptr, window->floor); // Aqui é o último sprite
	ft_free_matrix(window->map); // Dou free no mapa
	mlx_destroy_window (window->mlx_ptr, window->win_ptr); // Destrói a janela
	mlx_destroy_display(window->mlx_ptr); // Destrói a parte preta da tela
	free(window->mlx_ptr); // Dá free pra não deixar nenhum resquício de memória perdida nessa bagaça
	exit (0); // Finalizo o programa
	return (0);
}

// Função pra verificar eventos do teclado
int	ft_event(int key, t_mlx *window)
{
	if (key == XK_Escape)
	{
		ft_close(window);
	}
	if (key == XK_a || key == XK_A || key == XK_Left)
	{
		window->player = window->player_left; // Atualizo a imagem do Homário conforme a tecla pressionada
		ft_event_arrow(window, -1, 0); // Chamo a função pra ele andar e envio as Coordenadas para onde ele deve ir
	}
	if (key == XK_d || key == XK_D || key == XK_Right)
	{
		window->player = window->player_right;
		ft_event_arrow(window, +1, 0);
	}
	if (key == XK_w || key == XK_W || key == XK_Up)
	{
		window->player = window->player_up;
		ft_event_arrow(window, 0, -1);
	}
	if (key == XK_s || key == XK_S || key == XK_Down)
	{
		window->player = window->player_down;
		ft_event_arrow(window, 0, +1);
	}
	return (0);
}

// Função que mostra os sprites na tela
void	ft_sprites(t_mlx *window, void **image, char *path)
{
	int	width;
	int	height;

	*image = mlx_xpm_file_to_image(window->mlx_ptr, path, &width, &height); // Carrega imagem conforme os dados enviados
	if (*image == NULL) // Verificação de erro
	{	
		ft_print_error(error_13"\n");
		ft_close(window); // Destruo tudo
	}
}

// Função para carregar os sprites ("Colocar o arquivo xpm nas variáveis")
void	ft_put_sprites(t_mlx *window)
{
	ft_sprites(window, &window->player_left, "sprites_teste/player_left.xpm"); // Manda os dados
	ft_sprites(window, &window->player_right, "sprites_teste/player_right.xpm");
	ft_sprites(window, &window->player_up, "sprites_teste/player_up.xpm");
	ft_sprites(window, &window->player_down, "sprites_teste/player_down.xpm");
	ft_sprites(window, &window->wall, "sprites_teste/wall.xpm");
	ft_sprites(window, &window->collec, "sprites_teste/Collectable.xpm");
	ft_sprites(window, &window->exit_c, "sprites_teste/exit_close.xpm");
	ft_sprites(window, &window->exit_o, "sprites_teste/exit_open.xpm");
	ft_sprites(window, &window->floor, "sprites_teste/floor.xpm");
}

// Inicializa o game (Carrega tudo da mlx e faz a janela aparecer)
int	ft_init_game(t_mlx *window)
{
	window->mlx_ptr = mlx_init(); // Inicializo mlx
	if (window->mlx_ptr == NULL) // Verificação pro caso de ocorrer erro na mlx
	{
		ft_print_error(error_12"\n");
		return (1);
	}
	window->win_ptr = mlx_new_window(window->mlx_ptr, window->col * \
		PIXEL_SIZE, window->line * PIXEL_SIZE, "So_long"); // Criando a janela
	if (window->win_ptr == NULL)
	{
		ft_print_error(error_12"\n");
		return (1);
	}
	window->movements = 0; // Inicializo o contador de movimentos
	ft_put_sprites(window); // Inicializo os sprites ("colocar" nas variáveis)
	window->player = window->player_down; // Digo que a imagem inicial é o Homário olhando pra gente
	mlx_hook(window->win_ptr, 2, (1L << 0), &ft_event, window); // Captura evento de teclado e manda pra função ft_event
	mlx_hook(window->win_ptr, 17, 0, &ft_close, window); // Captura o evento de fechar a janela no X e manda pra função ft_close
	mlx_loop_hook(window->mlx_ptr, &ft_show_map, window); // Roda em loop a função ft_show_map (pra tela atualizar direto)
	mlx_loop(window->mlx_ptr); // Já sabemos
	return (0);
}

// Função de comparação (Para checar se o argumento termina com .ber)
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i; // Contador pra verificar

	i = 0;
	if (n == 0) // proteção da função
		return (0);
	while (s1[i] != '\0' && s1[i] == s2[i] && i < n -1) // Compara caracter por caracter de s1 e s2
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]); // retorna a 0 se os dois caracteres são iguais (outros números se forem diferentes)
}

// Função para da free na matriz (valgrind)
void	ft_free_matrix(char **matrix)
{
	int		i; // Contador

	i = 0;
	while (matrix[i] != NULL)
	{
		free (matrix[i]); // Limpo a linha
		i++;
	}
	free(matrix);// Pra dar aquele toque final na eliminação de tudo
}

// Função que verifica os possíveis erros (Joguei aqui pra facilitar a vida ^^)
int	ft_check_errors(t_mlx *window, char *path)
{
	int		has_error; // Verificar se tem algum erro (pra evitar de abrir janela)
	char	**copy; // Matriz cópia para verificar o caminho

	has_error = 0; // Aqui inicializo o contador de erros
	has_error += ft_read_map(window, path); // Tento ler o mapa 
	if (has_error != 0) // Se tiver algum erro até aqui, paro tudo
		return (1);
	has_error += ft_create_map(window, path); // Tento criar a matriz que está na variável "map"
	if (has_error != 0) // Se tiver algum erro até aqui, paro tudo
		return (1);
	has_error += ft_valid_walls(window) + ft_valid_character(window); // Conseguindo ler o arquivo e alocar a matriz, valido as paredes e verifico se tem letras erradas
	if (has_error != 0) // Se tiver algum erro até aqui, paro tudo
		return (1);
	copy = ft_dup_map(window); // crio uma cópia da matriz (para fazer a validação do caminho)
	if (copy == NULL) // Verificação pra se algum avaliador do tipo Vinícius aparecer
		return (1);
	ft_locate_player(window); // Localizo o jogador
	ft_test_path(copy, window->player_line, window->player_col, window); // Testo efetivamente o caminho
	has_error += ft_check_path(copy, window); // Verifico se está tudo certo
	ft_free_matrix(copy); // Free na cópia do mapa (valgrind)
	if (has_error != 0) // Se tiver algum erro até aqui, não executo nada
		return (1);
	return (0);
}

// Função pro valgrind parar de reclamar
void	ft_struct_init(t_mlx *window)
{
	window->mlx_ptr = NULL;
	window->win_ptr = NULL;
	window->map = NULL;
	window->player = NULL;
	window->player_left = NULL;
	window->player_right = NULL;
	window->player_down = NULL;
	window->player_up = NULL;
	window->collec = NULL;
	window->exit_c = NULL;
	window->exit_o = NULL;
	window->wall = NULL;
	window->floor = NULL;
	window->stars = 0;
	window->movements = 0;
	window->col = 0;
	window->line = 0;
	window->player_line = 0;
	window->player_col = 0;
}

// Onde tudo começa
int	main(int argc, char **argv)
{
	int		size; // Guarda o tamanho do segundo argumento
	t_mlx	window; // A struct

	if (argc != 2) // Só passa se tiver exatamente 2 argumentos
	{
		ft_print_error(error_0"\n");
		return (1);
	}
	size = ft_strlen(argv[1]); // Conta aqui o tamanho do segundo argumento (o arquivo)
	if (size < 5 || ft_strncmp(".ber", argv[1] + size - 4, 5)) // Verifica se termina com .ber
	{
		ft_print_error(error_1"\n");
		return (1);
	}
	ft_struct_init(&window);
	if (ft_check_errors(&window, argv[1]) == 1) // Chamo a função pra checar os demais erros, se algo der errado, retorna 1
		return (1);
	return (ft_init_game(&window)); // Inicia o game e pega o retorno da função pra mandar pro usuário
}
