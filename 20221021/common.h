#include <chrono>
#include <string>
#include <vector>
void check(int *array, const int size);
void display(int *array, const int size);
void check_vec(std::vector<int>& array);
void display_vec(std::vector<int>& array);
//void printTime(auto start, auto end);
void printTime(std::chrono::system_clock::time_point start,
               std::chrono::system_clock::time_point end);
const int region_max = (1000*1000);

class Measure
{
private:
  std::chrono::system_clock::time_point start_, end_;

public:
  void start()
  {
    start_ = std::chrono::system_clock::now();
  }

  void end()
  {
    end_ = std::chrono::system_clock::now();
  }

  void print_elapsed_time(std::string name)
  {
    auto dur = end_ - start_;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    std::cout << name << msec << " ms \n";
  }
};

