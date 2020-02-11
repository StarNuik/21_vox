// #include "Base.h"
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine/Game.h"

#include "Types.h"
#include "Render/RenderModel.h"
#include "World/ResourceLoader.h"
#include "Render/GLRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/cl.h>

#include <glm/glm.hpp>

typedef struct		s_bounds
{
	glm::vec4		min;
	glm::vec4		max;
}					t_bounds;

typedef union		s_aabb
{
	glm::vec4		arr[2];
	t_bounds		bounds;
}					t_aabb;

t_aabb	*parse_raw_aabbs(const char *filename, int *out_aabbs_num)
{
	FILE *fp = fopen(filename, "rb");

	fread(out_aabbs_num, sizeof(int), 1, fp);

	unsigned char	*raw_data = (unsigned char*)malloc(sizeof(t_aabb) * (*out_aabbs_num));

	fread(raw_data, sizeof(t_aabb) * (*out_aabbs_num), 1, fp);

	t_aabb	*parsed_aabbs = (t_aabb*)malloc(sizeof(t_aabb) * (*out_aabbs_num));

	printf("parsed aabbs num: [%i]\n", *out_aabbs_num);

	for (int i = 0; i < *out_aabbs_num; ++i)
		parsed_aabbs[i] = ((t_aabb*)raw_data)[i];
	return parsed_aabbs;
}

void push_vertex(std::vector<float>& vec, glm::vec4 point) {
	vec.push_back(point.x);
	vec.push_back(point.y);
	vec.push_back(point.z);
}


int	main() {
	Game* game = new Game();
	game->InitSystems();
	ResourceLoader* rs = game->GetResources();
	GLRenderer* r = game->GetRenderer();
	RenderModel* cube = new RenderModel(game->GetRenderer(), rs->GetShader("Base"), rs->GetMaterial(Block::Planks), rs->GetGeometry("Pepe"));
	int num;
	t_aabb* aabb = parse_raw_aabbs("./resources/raw_aabbs_data", &num);
	std::cout << "x: " << aabb[0].bounds.min.x << " | y:" << aabb[0].bounds.min.y << " | z: " << aabb[0].bounds.min.z << std::endl;
	std::vector<float> vec;
	for (int i = 3; i < num; i++) {
		t_aabb cur = aabb[i];
		push_vertex(vec, cur.bounds.max);
		push_vertex(vec, glm::vec4(cur.bounds.min.x, cur.bounds.max.y, cur.bounds.max.z, 0));
		push_vertex(vec, cur.bounds.max);
		push_vertex(vec, glm::vec4(cur.bounds.max.x, cur.bounds.min.y, cur.bounds.max.z, 0));
		push_vertex(vec, cur.bounds.max);
		push_vertex(vec, glm::vec4(cur.bounds.max.x, cur.bounds.max.y, cur.bounds.min.z, 0));

		push_vertex(vec, cur.bounds.min);
		push_vertex(vec, glm::vec4(cur.bounds.max.x, cur.bounds.min.y, cur.bounds.min.z, 0));
		push_vertex(vec, cur.bounds.min);
		push_vertex(vec, glm::vec4(cur.bounds.min.x, cur.bounds.max.y, cur.bounds.min.z, 0));
		push_vertex(vec, cur.bounds.min);
		push_vertex(vec, glm::vec4(cur.bounds.min.x, cur.bounds.min.y, cur.bounds.max.z, 0));

		push_vertex(vec, glm::vec4(cur.bounds.max.x, cur.bounds.min.y, cur.bounds.max.z, 0));
		push_vertex(vec, glm::vec4(cur.bounds.max.x, cur.bounds.min.y, cur.bounds.min.z, 0));
		push_vertex(vec, glm::vec4(cur.bounds.max.x, cur.bounds.min.y, cur.bounds.max.z, 0));
		push_vertex(vec, glm::vec4(cur.bounds.min.x, cur.bounds.min.y, cur.bounds.max.z, 0));
	
		push_vertex(vec, glm::vec4(cur.bounds.min.x, cur.bounds.max.y, cur.bounds.max.z, 0));
		push_vertex(vec, glm::vec4(cur.bounds.min.x, cur.bounds.min.y, cur.bounds.max.z, 0));
		push_vertex(vec, glm::vec4(cur.bounds.min.x, cur.bounds.max.y, cur.bounds.max.z, 0));
		push_vertex(vec, glm::vec4(cur.bounds.min.x, cur.bounds.max.y, cur.bounds.min.z, 0));
	
		push_vertex(vec, glm::vec4(cur.bounds.max.x, cur.bounds.max.y, cur.bounds.min.z, 0));
		push_vertex(vec, glm::vec4(cur.bounds.max.x, cur.bounds.min.y, cur.bounds.min.z, 0));
		push_vertex(vec, glm::vec4(cur.bounds.max.x, cur.bounds.max.y, cur.bounds.min.z, 0));
		push_vertex(vec, glm::vec4(cur.bounds.min.x, cur.bounds.max.y, cur.bounds.min.z, 0));
	}
	r->aabb_vao = 0;
	r->ammount = 12 * num;
	uint vbo = 0;

	glGenVertexArrays(1, &r->aabb_vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(r->aabb_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vec.size(), &vec[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// game->InitWorld();
	game->GameLoop();
	return (0);
}
