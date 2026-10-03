#ifndef COA_ASCENSION_ITEM_SCALING_H
#define COA_ASCENSION_ITEM_SCALING_H

#include <array>
#include <cstdint>
#include <vector>

namespace ItemScaling
{
using ClientItemRow = std::array<std::uint32_t, 8>;

std::uint32_t BaseEntry(std::uint32_t entry);
std::vector<ClientItemRow> ClientRows();
}

#endif
