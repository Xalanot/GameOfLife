#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

void printGrid(const std::vector<bool>& grid)
{
  auto gridDim = static_cast<size_t>(sqrt(grid.size()));
  for (auto i = 0u; i < gridDim; i++)
  {
    std::cout << "_";
  }
  for (auto i = 0u; i < grid.size(); i++)
  {
    if (i % gridDim == 0)
    {
      std::cout << "\n";
    }
    if (grid[i])
    {
      std::cout << "0";
    }
    else
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  for (auto i = 0u; i < gridDim; i++)
  {
    std::cout << "¯";
  }
  std::cout << "\n";
}

std::vector<bool> createRandomGrid(size_t gridDim, float propability=0.3, size_t seed=42)
{
  srand(seed);
  std::vector<bool> grid(gridDim * gridDim);
  for (size_t i = 0; i < gridDim * gridDim; i++)
  {
    float randomValue = static_cast<float>(rand()) / RAND_MAX;
    if (randomValue < propability)
    {
      grid[i] = true;
    }
    else
    {
      grid[i] = false;
    }
  }
  return grid;
}

bool testNeighbor(const std::vector<bool>& grid, size_t row, size_t column)
{
  auto gridDim = static_cast<size_t>(sqrt(grid.size()));
  if (row < 0 || row >= gridDim || column < 0 || column >= gridDim)
  {
    return false;
  }
  else
  {
    return grid[row * gridDim + column];
  }
}

int countNeighbors(const std::vector<bool>& grid, size_t position)
{
  static const std::array<std::pair<int, int>, 8> neighbors{{
    {-1, -1},
    {0, -1},
    {1, -1},
    {-1, 0},
    {1, 0},
    {-1, 1},
    {0, 1},
    {1, 1}
  }};
  auto gridDim = static_cast<size_t>(sqrt(grid.size()));
  size_t row = position / gridDim;
  size_t column = position % gridDim;
  size_t count = 0;
  for(auto const& direction: neighbors)
  {
    if (testNeighbor(grid, row + direction.first, column + direction.second))
    {
      count++;
    }
  }
  return count;
}

void updateGrid(const std::vector<bool>& oldGrid, std::vector<bool>& newGrid)
{
  for (size_t i = 0; i < oldGrid.size(); i++)
  {
    auto neighborCount = countNeighbors(oldGrid, i);
    if (oldGrid[i])
    {
      if (neighborCount == 2 || neighborCount == 3)
      {
        newGrid[i] = true;
      }
      else
      {
        newGrid[i] = false;
      }
    }
    else
    {
      if (neighborCount == 3)
      {
        newGrid[i] = true;
      }
      else
      {
        newGrid[i] = false;
      }
    }
  }
}

int main(int argc, char** argv)
{
  auto firstGrid = std::make_unique<std::vector<bool>>(createRandomGrid(5));
  auto secondGrid = std::make_unique<std::vector<bool>>(*firstGrid);
  while(true)
  {
    printGrid(*firstGrid);
    updateGrid(*firstGrid, *secondGrid);
    std::swap(firstGrid, secondGrid);
    std::this_thread::sleep_for(1s);
  }
  return 0;
}