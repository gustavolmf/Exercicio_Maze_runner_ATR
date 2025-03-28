#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;
std::stack<Position> valid_positions;

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    std::ifstream ifs(file_name);
    if (!ifs) {
        std::cerr << "Erro ao abrir o arquivo: " << file_name << std::endl;
        return {-1, -1};
    }

    ifs >> num_rows >> num_cols;
    ifs.ignore();

    maze.resize(num_rows, std::vector<char>(num_cols));
    Position initial = {-1, -1};

    for (int i = 0; i < num_rows; i++) {
        std::string line;
        std::getline(ifs, line);
        for (int j = 0; j < num_cols; j++) {
            maze[i][j] = line[j];
            if (maze[i][j] == 'e') {
                initial = {i, j};
            }
        }
    }

    ifs.close();
    return initial;
}

// Função para imprimir o labirinto
void print_maze() {

    for(int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++)
        {
            std::cout << maze[i][j];
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}
// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    return (row >= 0 && row < num_rows && col >= 0 && col < num_cols && (maze[row][col] == 'x' || maze[row][col] == 's'));
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    if (maze[pos.row][pos.col] == 's') {
        return true;
    }

    maze[pos.row][pos.col] = '.'; 
    print_maze();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if (is_valid_position(pos.row, pos.col + 1)) 
        valid_positions.push({pos.row, pos.col + 1});
    if (is_valid_position(pos.row, pos.col - 1)) 
        valid_positions.push({pos.row, pos.col - 1});
    if (is_valid_position(pos.row - 1, pos.col)) 
        valid_positions.push({pos.row - 1, pos.col});
    if (is_valid_position(pos.row + 1, pos.col)) 
        valid_positions.push({pos.row + 1, pos.col});

    while (!valid_positions.empty()) {
        Position next_pos = valid_positions.top();
        valid_positions.pop();
        
        if (walk(next_pos)) return true;
    }

    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_labirinto>" << std::endl;
        return 1;
    }

    Position initial_pos = load_maze(argv[1]);
    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    bool exit_found = walk(initial_pos);

    if (exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Não foi possível encontrar a saída." << std::endl;
    }

    return 0;
}
