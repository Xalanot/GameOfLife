#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>
#include <thread>
#include <utility>
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

std::vector<bool> createRandomGrid(size_t gridDim, float propability=0.3, size_t seed=10)
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

size_t countNeighbors(const std::vector<bool>& grid, size_t cellPosition)
{
  size_t gridDim = static_cast<int>(sqrt(grid.size()));
  size_t row = cellPosition / gridDim;
  size_t column = cellPosition % gridDim;

  static const std::array<std::pair<int, int>, 8> neighborDirections{{
    {-1, -1},
    {0, -1},
    {1, -1},
    {-1, 0},
    {1, 0},
    {-1, 1},
    {0, 1},
    {1, 1}
  }};

  auto calculateNeighborIndices = [row, column](std::pair<int, int> direction){return std::make_pair(direction.first + row, direction.second + column);};
  auto checkBoundaryIndices = [gridDim](std::pair<int, int> index){return (index.first >= 0 && index.first < gridDim && index.second >= 0 && index.second < gridDim);};
  auto makeOneDimensionalIndex = [gridDim](std::pair<int, int> index){return index.first * gridDim + index.second;};
  auto getGridValue = [&grid](int index){return grid[index];};
  auto result = neighborDirections | std::views::transform(calculateNeighborIndices)
                                   | std::views::filter(checkBoundaryIndices)
                                   | std::views::transform(makeOneDimensionalIndex)
                                   | std::views::transform(getGridValue);
  auto neighborCount = std::ranges::count(result, true);
  return neighborCount;
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

int main() 
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