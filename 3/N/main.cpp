#include <iostream>
#include <vector>

void add_z(size_t nx, size_t ny, size_t nz,
           size_t x, size_t y, size_t z,
           std::vector<uint32_t> &b, uint32_t v) {
    for (size_t i = x; i < nx; i = (i | (i + 1))) {
        for (size_t j = y; j < ny; j = (j | (j + 1))) {
            for (size_t k = z; k < nz; k = (k | (k + 1))) {
                b[i * ny * nz + j * nz + k] += v;
            }
        }
    }
}

void add_y(size_t nx, size_t ny, size_t nz,
           size_t z1, size_t z2,
           size_t x, size_t y,
           std::vector<std::vector<uint32_t>> &b, uint32_t v, size_t num) {
    add_z(nx, ny, nz, x, y, z1, b[num], v);
    add_z(nx, ny, nz, x, y, z2 + 1, b[num], -v);
    add_z(nx, ny, nz, x, y, z1, b[num + 1], v * static_cast<uint32_t>(z1 - 1));
    add_z(nx, ny, nz, x, y, z2 + 1, b[num + 1], -v * static_cast<uint32_t>(z2));
}

void add_x(size_t nx, size_t ny, size_t nz,
           size_t y1, size_t z1,
           size_t y2, size_t z2,
           size_t x,
           std::vector<std::vector<uint32_t>> &b, uint32_t v, size_t num) {
    add_y(nx, ny, nz, z1, z2, x, y1, b, v, num);
    add_y(nx, ny, nz, z1, z2, x, y2 + 1, b, -v, num);
    add_y(nx, ny, nz, z1, z2, x, y1, b, v * static_cast<uint32_t>(y1 - 1), num + 2);
    add_y(nx, ny, nz, z1, z2, x, y2 + 1, b, -v * static_cast<uint32_t>(y2), num + 2);
}

void add(size_t nx, size_t ny, size_t nz,
         size_t x1, size_t y1, size_t z1,
         size_t x2, size_t y2, size_t z2,
         std::vector<std::vector<uint32_t>> &b, uint32_t v) {
    add_x(nx, ny, nz, y1, z1, y2, z2, x1, b, v, 0);
    add_x(nx, ny, nz, y1, z1, y2, z2, x2 + 1, b, -v, 0);
    add_x(nx, ny, nz, y1, z1, y2, z2, x1, b, v * static_cast<uint32_t>(x1 - 1), 4);
    add_x(nx, ny, nz, y1, z1, y2, z2, x2 + 1, b, -v * static_cast<uint32_t>(x2), 4);
}

uint32_t get_z(size_t ny, size_t nz,
               size_t x, size_t y, size_t z,
               std::vector<uint32_t> &b) {
    uint32_t ans = 0;
    for (size_t i = x; i <= x; i = (i & (i + 1)) - 1) {
        for (size_t j = y; j <= y; j = (j & (j + 1)) - 1) {
            for (size_t k = z; k <= z; k = (k & (k + 1)) - 1) {
                ans += b[i * ny * nz + j * nz + k];
            }
        }
    }
    return ans;
}

uint32_t get_y(size_t ny, size_t nz,
               size_t x, size_t y,
               size_t z,
               std::vector<std::vector<uint32_t>> &b, size_t num) {
    return get_z(ny, nz, x, y, z, b[num]) * static_cast<uint32_t>(z)
           - get_z(ny, nz, x, y, z, b[num + 1]);
}

uint32_t get_x(size_t ny, size_t nz,
               size_t x,
               size_t y, size_t z,
               std::vector<std::vector<uint32_t>> &b, size_t num) {
    return get_y(ny, nz, x, y, z, b, num) * static_cast<uint32_t>(y)
           - get_y(ny, nz, x, y, z, b, num + 2);
}

uint32_t get(size_t ny, size_t nz,
             size_t x, size_t y, size_t z,
             std::vector<std::vector<uint32_t>> &b) {
    return get_x(ny, nz, x, y, z, b, 0) * static_cast<uint32_t>(x) - get_x(ny, nz, x, y, z, b, 4);
}

uint32_t get(size_t ny, size_t nz,
             size_t x1, size_t y1, size_t z1,
             size_t x2, size_t y2, size_t z2,
             std::vector<std::vector<uint32_t>> &b) {
    uint32_t ans = get(ny, nz, x2, y2, z2, b);
    if (x1 > 0) {
        ans -= get(ny, nz, x1 - 1, y2, z2, b);
    }
    if (y1 > 0) {
        ans -= get(ny, nz, x2, y1 - 1, z2, b);
    }
    if (z1 > 0) {
        ans -= get(ny, nz, x2, y2, z1 - 1, b);
    }
    if (x1 > 0 && y1 > 0) {
        ans += get(ny, nz, x1 - 1, y1 - 1, z2, b);
    }
    if (x1 > 0 && z1 > 0) {
        ans += get(ny, nz, x1 - 1, y2, z1 - 1, b);
    }
    if (y1 > 0 && z1 > 0) {
        ans += get(ny, nz, x2, y1 - 1, z1 - 1, b);
    }
    if (x1 > 0 && y1 > 0 && z1 > 0) {
        ans -= get(ny, nz, x1 - 1, y1 - 1, z1 - 1, b);
    }
    return ans;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t nx, ny, nz;
    std::cin >> nx >> ny >> nz;
    std::vector<std::vector<uint32_t>> b(8, std::vector<uint32_t>(nx * ny * nz, 0));
    size_t q;
    std::cin >> q;
    while (q-- > 0) {
        char c;
        std::cin >> c;
        if (c == 'a') {
            size_t x1, x2, y1, y2, z1, z2;
            uint32_t v;
            std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> v;
            x2--;
            y2--;
            z2--;
            add(nx, ny, nz, x1, y1, z1, x2, y2, z2, b, v);
        } else {
            size_t x1, x2, y1, y2, z1, z2;
            std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            x2--;
            y2--;
            z2--;
            std::cout << get(ny, nz, x1, y1, z1, x2, y2, z2, b) << std::endl;
        }
    }
    return 0;
}
