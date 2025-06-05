#include "Render.h"

Render::Render() {}

Render::~Render() {}

void Render::Clear()const {
	// render here
	GLCall(glClearColor(0.05f, 0.05f, 0.05f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Render::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	va.Bind();
	ib.Bind();
	shader.Use();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}