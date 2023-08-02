#include <iostream>
#include <fstream>
#include <vector>
#include <cstring> // For strlen
#include <climits> // for INT_MIN

constexpr int MAX_N = 100;

struct Tile {
    int t;
    int v;
};

void load_input_file(const std::string& file_name, int& n, int& H, std::vector<std::vector<Tile>>& tiles) {
    std::ifstream file(file_name);
    file >> n >> H;
    
    // Initialize the grid
    tiles.resize(n, std::vector<Tile>(n));

    for (int i = 0; i < n * n; i++) {
        if (i == 0) {
            continue; // The initial tile is zero type with zero value
        }
        Tile tile;
        int x,y;
        file >> x >> y >> tile.t >> tile.v;
        tiles[x][y] = tile;
    }
    
    file.close();
}

void print_tile_data(int n, const std::vector<std::vector<Tile>>& tiles) {
    std::cout << "Tile Types:\n";
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            std::cout << "[" << tiles[x][y].t << "] ";
        }
        std::cout << '\n';
    }

    std::cout << "\nTile Values:\n";
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            std::cout << "[" << tiles[x][y].v << "] ";
        }
        std::cout << '\n';
    }
}

// struct HP_state{
//     int HP;
//     Tile pro_tile;
//     Tile mul_tile;
//     bool pro_stored;
//     bool mul_stored;
// }

int helper(int n, int HP, int x, int y, const std::vector<std::vector<Tile>>& tiles, 
std::vector<std::vector<int>>& memo, bool pro_token, bool mul_token){
    //base case if out of bound i.e. at bottom-right corner
    if (x >=n || y >= n)
        return 0;

    // if already stored, use memo
    if (memo[x][y]!= INT_MIN)
        return memo[x][y];

    Tile curr_tile = tiles[x][y];
    //determine types of tiles and handle correspondingly
    switch (curr_tile.t){
        case 0:
            if (!pro_token)     //if without pro_token, damage normally; otherwise, do nothing
                HP-=curr_tile.v;   
            pro_token = false;  //pro_token used
            break;
        case 1:
            if (mul_token)      //if with mul_token, add 2 times healing, else add one healing
                HP+= (2 * curr_tile.v);
            else
                HP+=curr_tile.v;//mul_token used
            mul_token = false;
            break;
        case 2:
            pro_token = true;   //pro_token stored
            break;
        case 3:
            mul_token = true;   //mul_token stored
            break;
    }

    if (HP < 0)
        return HP;     //if HP < 0, lose, return lose

    int path_right = helper(n, HP, x+1, y, tiles, memo, pro_token, mul_token); //recurve down the right direction
    int path_down = helper(n, HP, x, y+1, tiles, memo, pro_token, mul_token);   // recurve down the down direction

    memo[x][y] = std::max(path_right, path_down);//store memo with max HP to the bottom right

    return memo[x][y]; //return what will be the final HP if going from this index
}

bool DP(int n, int H, const std::vector<std::vector<Tile>>& tiles) {
    // TODO
    // Placeholder function - implement your logic here
    // Your code to check whether it is possible to reach the bottom-right
    // corner without running out of HP should go here.
    // You should use dynamic programming to solve the problem.
    // Return true if possible, false otherwise.
    
    // By default, we return false
    // TODO you should change this

    //edge cases 
    if (n < 0 || H < 0)
        return false;
    if (n==0 or n == 1)
        return true;
    //2D array of size n by n, each initialized to INT_MIN
    std::vector<std::vector<int>> memo(n, std::vector<int>(n, INT_MIN));
    memo[0][0] = H;
    //initialize tokens to false
    bool pro_token = false;
    bool mul_token = false;

    //calls the recursive helper to check right and down directions
    return std::max(helper(n, H, 1, 0, tiles, memo, pro_token, mul_token), helper(n, H, 0, 1, tiles, memo, pro_token, mul_token)) >= 0;
}

void write_output_file(const std::string& output_file_name, bool result) {
    std::ofstream file(output_file_name);
    file << result;
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " a_file_name.txt\n";
        return 1;
    }

    int n, H;
    std::vector<std::vector<Tile>> tiles;
    load_input_file(argv[1], n, H, tiles);
    print_tile_data(n, tiles);
    bool result = DP(n, H, tiles);
    std::cout << "Result: " << result << '\n';
    
    // Construct the output file name
    std::string output_file_name(argv[1]);
    output_file_name.insert(output_file_name.find_last_of('.'), "_out");
    std::ofstream outfile(output_file_name);
    outfile << result;
    outfile.close();

    return 0;
}
