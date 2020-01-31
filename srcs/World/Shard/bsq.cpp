// #include <iostream>
// #include <array>
// #include <cstdint>
// #include <algorithm>
// #include <vector>
// #include <string>
// #include <glm/glm.hpp>

// using uint32 = std::uint32_t;
// using uint16 = std::uint16_t;
// using uint8 = std::uint8_t;
// using uint = uint32;
// using Map2d = std::array<std::array<uint8, 16>, 16>;
// using PackedMap = std::array<uint16, 16>;

// class BsqQuad {
// public:
// 	BsqQuad() {x = 0; y = 0; size = 0;};
// 	BsqQuad(uint8 _x, uint8 _y, uint8 _size) {x = _x; y = _y; size = _size;};
// 	uint8 x;
// 	uint8 y;
// 	uint8 size;
// };

// class Quad {
// public:
// 	Quad() {_a = glm::vec3(0); _b = glm::vec3(0); _n = glm::vec3(0);};
// 	Quad(glm::vec3 a, glm::vec3 b, glm::vec3 norm) {_a = a; _b = b; _n = norm;};
// 	glm::vec3 _a;
// 	glm::vec3 _b;
// 	glm::vec3 _n;
// };

// void PrintBsqQuad(BsqQuad q) {
// 	std::cout << "Quad: [s: " << std::to_string(q.size) << ", x: " << std::to_string(q.x) << ", y: " << std::to_string(q.y) << "]" << std::endl;
// }

// void PrintQuad(Quad q) {
// 	std::cout << "Quad:" << std::endl;
// 	std::cout << "A: [x: " << q._a.x << " y: " << q._a.y << " z: " << q._a.z << "]" << std::endl;
// 	std::cout << "B: [x: " << q._b.x << " y: " << q._b.y << " z: " << q._b.z << "]" << std::endl;
// 	std::cout << "N: [x: " << q._n.x << " y: " << q._n.y << " z: " << q._n.z << "]" << std::endl;
// }

// void PrintMap(Map2d map) {
// 	for (int x = 0; x < 16; x++) {
// 		for (int y = 0; y < 16; y++) {
// 			std::string s;
// 			if (map[x][y] > 16)
// 				s = (char)map[x][y];
// 			else if (map[x][y] >= 10)
// 				s = 'A' - 10 + map[x][y];
// 			else
// 				s = std::to_string(map[x][y]);
// 			std::cout << s << " ";
// 		}
// 		std::cout << std::endl;
// 	}
// }

// void PrintMap(PackedMap pack) {
// 	for (int x = 0; x < 16; x++) {
// 		for (int y = 0; y < 16; y++) {
// 			std::cout << ((pack[x] & (1 << y)) ? "1" : "0") << " ";

// 		}
// 		std::cout << std::endl;
// 	}
// }

// Map2d NewMap2d(uint antifrequency) {
// 	Map2d map = {0};
// 	for (int x = 0; x < 16; x++)
// 		for (int y = 0; y < 16; y++) {
// 			int val = (rand() % antifrequency == 0 ? 1 : 0);
// 			map[x][y] = val;
// 		}
// 	return map;
// }

// PackedMap NewPackedMap(uint antifrequency) {
// 	PackedMap pack = {0};
// 	for (int x = 0; x < 16; x++) {
// 		for (int y = 0; y < 16; y++) {
// 			int val = (rand() % antifrequency == 0 ? 1 : 0);
// 			pack[x] = pack[x] | (val << y);
// 		}
// 	}
// 	return pack;
// }

// enum class QuadDir {
// 	PosX,
// 	PosY,
// 	PosZ,
// 	NegX,
// 	NegY,
// 	NegZ
// };

// Quad AssembleQuad(BsqQuad bq, QuadDir dir, uint8 offset) {
// 	Quad q;
// 	switch (dir) {
// 		case QuadDir::PosX:
// 			q = Quad(glm::vec3(offset, bq.x, bq.y), glm::vec3(offset, bq.x - bq.size, bq.y - bq.size), glm::vec3(1, 0, 0));
// 			break;
// 		case QuadDir::PosY:
// 			q = Quad(glm::vec3(bq.x, offset, bq.y), glm::vec3(bq.x - bq.size, offset, bq.y - bq.size), glm::vec3(0, 1, 0));
// 			break;
// 		case QuadDir::PosZ:
// 			q = Quad(glm::vec3(bq.y, bq.x, offset), glm::vec3(bq.y - bq.size, bq.x - bq.size, offset), glm::vec3(0, 0, 1));
// 			break;
// 		case QuadDir::NegX:
// 			q = Quad(glm::vec3(offset, bq.x, bq.y), glm::vec3(offset, bq.x - bq.size, bq.y - bq.size), glm::vec3(-1, 0, 0));
// 			break;
// 		case QuadDir::NegY:
// 			q = Quad(glm::vec3(bq.x, offset, bq.y), glm::vec3(bq.x - bq.size, offset, bq.y - bq.size), glm::vec3(0, -1, 0));
// 			break;
// 		case QuadDir::NegZ:
// 		default:
// 			q = Quad(glm::vec3(bq.y, bq.x, offset), glm::vec3(bq.y - bq.size, bq.x - bq.size, offset), glm::vec3(0, 0, -1));
// 			break;
// 	}
// 	return q;
// }

// void FillMap2dWithQuad(Map2d& map, const BsqQuad q) {
// 	for (int x = 0; x < q.size; x++) {
// 		for (int y = 0; y < q.size; y++) {
// 			map[q.x - x][q.y - y] = 1;
// 		}
// 	}
// }

// Quad SolveMap2dHeatmap(Map2d& map, QuadDir dir, uint8 offset) {
// 	Map2d heat = {0};
// 	BsqQuad q;

// 	for (int x = 0; x < 16; x++) {
// 		for (int y = 0; y < 16; y++) {
// 			if (map[x][y])
// 				continue;
// 			if (x == 0 or y == 0) {
// 				heat[x][y] = 1;
// 				if (q.size < 1) {
// 					q = BsqQuad(x, y, 1);
// 				}
// 				continue;
// 			}
// 			heat[x][y] = std::min(std::min(heat[x - 1][y] + 1, heat[x][y - 1] + 1), heat[x - 1][y - 1] + 1);
// 			if (q.size < heat[x][y]) {
// 				q = BsqQuad(x, y, heat[x][y]);
// 			}
// 		}
// 	}
// 	if (q.size > 0)
// 		FillMap2dWithQuad(map, q);
// 	else
// 		return Quad();
// 	return AssembleQuad(q, dir, offset);
// }

// #define ANTIFREQUENCY 20

// int main() {
// 	std::vector<Quad> vec;
// 	Map2d map = NewMap2d(ANTIFREQUENCY);
// 	PrintMap(map);
// 	for (int lim = 0; lim < 256; lim++) {
// 		Quad q = SolveMap2dHeatmap(map, QuadDir::PosX, 5);
// 		if (q._n == glm::vec3(0))
// 			break;
// 		vec.push_back(q);
// 	}
// 	std::cout << std::endl;
// 	PrintQuad(vec[0]);
// 	std::cout << std::endl;
// 	PrintQuad(vec[1]);
// 	std::cout << std::endl;
// 	PrintQuad(vec[2]);
// 	std::cout << "Quad count: " << std::to_string(vec.size()) << std::endl;
// 	return 0;
// }