#ifndef COUNT
#error "Please define COUNT"
#endif

#include <unordered_map>
#include <cstdint>

int main(void)
{
    std::unordered_map<int32_t, int32_t> map(COUNT);

    for (int32_t i = 0; i < COUNT; i++)
        map.insert({i, i});

    return 0;
}
