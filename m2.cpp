#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>
//#include <QtCore>
//ADD OUT OF RANGE AND BAD TYPE PROTECTION FOR ALL CIN>> iNPUTS
class board {
  
 public:
  
  enum class gamestatus { in_progress, lost };

  board(int s, int b)
      : size(s),
        n_mines(b),
        mines(new std::set<std::pair<int, int>>),
        field(new std::vector<std::vector<int>>),
        visible(new std::vector<std::vector<std::string>>),
        prohibited_squares(new std::set<std::pair<int, int>>),
        status(gamestatus::in_progress) {}
  ~board() {
    delete field;
    delete mines;
    delete visible;
  }
void first_click(int x, int y) {
    std::set<std::pair<int, int>> toBprohib = adjacent_squares(x, y);
    for (auto& x : toBprohib) {
      prohibited_squares->insert(x);
    }
    make_mines();
    make_field();
  }
  
  

  int mine_sweaper_counter(int x, int y) {
    int count = 0;
    for (int x_cord = x - 1; x_cord <= (x + 1); ++x_cord) {
      for (int y_cord = y - 1; y_cord <= (y + 1); ++y_cord) {
        if (x_cord >= 0 && x_cord < size && y_cord >= 0 && y_cord < size) {
          if ((*field)[y_cord][x_cord] == 1) {
            count += 1;
          }
        }
      }
    }
    return count;
  }
  
std::set<std::pair<int, int>> adjacent_squares(int x, int y) {
    std::set<std::pair<int, int>> set_of_spaces;
    for (int x_cord = x - 1; x_cord <= (x + 1); ++x_cord) {
      for (int y_cord = y - 1; y_cord <= (y + 1); ++y_cord) {
        if (x_cord >= 0 && x_cord < size && y_cord >= 0 && y_cord < size) {
          set_of_spaces.insert(std::make_pair(x_cord, y_cord));
        }
      }
    }
    return set_of_spaces;
  }
  
  std::set<std::pair<int, int>> make_set_of_zeros(std::set<std::pair<int, int>>& s) {
    int start_size = s.size();
    for (auto& x : s) {
      std::set<std::pair<int, int>> check = adjacent_squares(x.first, x.second);
      for (auto& c : check) {
        if (mine_sweaper_counter(c.first, c.second) == 0) {
          s.insert(c);
        }
      }
    }
    if (start_size == s.size()) {
      return s;
    } else {
      return make_set_of_zeros(s);
    }
  }
  
  std::set<std::pair<int, int>> make_sweap_set(int x, int y) {
    if (mine_sweaper_counter(x, y) != 0) {
      std::set<std::pair<int, int>> one_square;
      one_square.insert(std::make_pair(x, y));
      return one_square;
    } else {
      std::set<std::pair<int, int>> without_adjacent;
      without_adjacent.insert(std::make_pair(x, y));
      without_adjacent = make_set_of_zeros(without_adjacent);
      std::set<std::pair<int, int>> all_adjacent;
      for (auto& x : without_adjacent) {
        auto temp = adjacent_squares(x.first, x.second);
        for (auto& w : temp) {
          all_adjacent.insert(w);
        }
      }
      return all_adjacent;
    }
  }

  void sweap(int x, int y) {
    std::set<std::pair<int, int>> to_sweep = make_sweap_set(x, y);
    for (auto& x : to_sweep) {
      int count = mine_sweaper_counter(x.first, x.second);
      if (count == 0) {
        (*visible)[x.second][x.first] = " ";
      } else {
        (*visible)[x.second][x.first] = std::to_string(count);
      }
    }
  }

  void start() {
    make_visible();
    print_visible();
    std::cout << '\n'
              << "This is minesweaper, what is the cordinate of the first "
                 "cordinate you would like to sweap?"
              << '\n';
    int x;
    std::cout << "First x cordinate: ";
    std::cin >> x;
    int y;
    std::cout << "First y cordinate: ";
    std::cin >> y;
    first_click(x, y);
    sweap(x, y);
    print_visible();
    while (status == gamestatus::in_progress) {
      turn();
      print_visible();
    }
  }

  void turn() {
    std::cout << "Enter S to sweap place, enter F to make or remove a flag :";
    enum class path { None, Sweap, Flag };
    std::string in;
    path d = path::None;
    while (d == path::None) {
      std::cin >> in;
      if (in == "s" || in == "S") {
        d = path::Sweap;
      } else if (in == "f" || in == "F") {
        d = path::Flag;
      } else {
        path d = path::None;
        std::cout << "not a valid option, type S to sweap or F to make or "
                     "remove a flag: ";
      }
    }
    // PUT PROTECTION HERE TO MAKE SURE ONLY VALID IMPUTS ARE ACCEPTED
    if (d == path::Sweap) {
      std::cout << "x cordinate: ";
      int x;
      std::cin >> x;
      std::cout << "y cordinate: ";
      int y;
      std::cin >> y;
      if ((*field)[y][x] == 1) {
        std::cout << "GAME OVER" << '\n';
        status = gamestatus::lost;
      } else {
        sweap(x, y);
      }
    }
    if (d == path::Flag) {
      std::cout << "x cordinate of Flag: ";
      int x;
      std::cin >> x;
      std::cout << "y cordinate of Flag: ";
      int y;
      std::cin >> y;
      (*visible)[y][x] = "F";
    }
  }
  void make_mines() {
    std::mt19937_64 generator;
    std::uniform_int_distribution<int> distribution(0, size - 1);
    while (mines->size() < n_mines) {
      // mineGen tracker = mineGen::unique_pair;
      int x = distribution(generator);
      int y = distribution(generator);
      int start_size = prohibited_squares->size();
      prohibited_squares->insert(std::make_pair(x,y));
      if(start_size!=prohibited_squares->size()){mines->insert(std::make_pair(x,y));}
				 
     
    }
  }
  void make_visible() {
    for (int i = 0; i < size; ++i) {
      std::vector<std::string> start;
      for (int w = 0; w < size; ++w) {
        start.push_back("x");
      }
      visible->push_back(start);
    }
  }
  void make_field() {
    for (int i = 0; i < size; ++i) {
      std::vector<int> start;
      for (int w = 0; w < size; ++w) {
        start.push_back(0);
      }
      field->push_back(start);
    }
    
    for(auto& m:(*mines)) {
      int x = m.first;
      int y = m.second;
      (*field)[y][x] = 1;
    }
  }
  void print_field() {
    std::cout << "MINESWEAPER" << '\n'
              << "#######################" << '\n'
              << '\n';
    // std::cout<<"0 1 2 3 4 5 6 7 8 9"<<'\n';
    for (int i = (field->size() - 1); i >= 0; --i) {
      std::vector<int> temp = (*field)[i];
      for (int w = 0; w < field->size(); ++w) {
        std::cout << "|" << temp[w] << "|"
                  << " ";
      }
      std::cout << '\n';
      for (int b = 0; b < size; ++b) {
        std::cout << "____";
      }
      std::cout << '\n';
    }
  }

  void print_visible() {
    for (int i = (visible->size() - 1); i >= 0; --i) {
      std::vector<std::string> temp = (*visible)[i];
      for (int w = 0; w < visible->size(); ++w) {
        if (w == 0) {
          std::cout << i << "-";
        }
        std::cout << "|" << temp[w] << "|"
                  << " ";
      }
      std::cout << '\n';
      for (int b = 0; b < size; ++b) {
        if (b == 0) {
          std::cout << "  ";
        }
        if (i != 0) {
          std::cout << "____";
        }
      }
      if (i != 0) {
        std::cout << '\n';
      }
    }
    for (int u = 0; u < size; ++u) {
      std::cout << " "
                << "|"
                << "  ";
    }
    std::cout << '\n' << "  ";
    for (int p = 0; p < size; ++p) {
      std::cout << " " << p << "  ";
    }
    std::cout << '\n';
  }
  void print_mines() {
    for (auto& i:(*mines)) {
      //std::pair<int, int> temp = (*mines)[i];
      std::cout << i.first << ", " << i.second << '\n';
    }
  }
  void print_prohibited() {
    for(auto& i:(*prohibited_squares)) {
      std::cout << i.first << ", "
                << i.second << '\n';
    }
  };

 private:
  int size;
  int n_mines;
  std::set<std::pair<int, int>>* mines;
  std::vector<std::vector<int>>* field;
  std::vector<std::vector<std::string>>* visible;
  std::set<std::pair<int, int>>* prohibited_squares;
  gamestatus status;
};

int main() {
  board b(10, 12);
  b.start();
  return 0;
}
