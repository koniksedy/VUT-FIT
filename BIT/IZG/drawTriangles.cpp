/**
 * drawTriangles.cpp
 * Autor: Michal Šedý, FIT
 * Řešení: IZG - projekt, 1.5.2019
 */


#include <student/gpu.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

/**
 * Fce načte data z bufferui, pokud je čtecí hlava povolena
 */
void copyAttribute(GPU const*const gpu, GPUAttribute*const attribut, GPUVertexPullerHead const*const head, uint64_t vertexId)
{
	if(!head->enabled)
	{
		return;
	}

	GPUBuffer const*const buffer = gpu_getBuffer(gpu, head->bufferId);
	uint8_t const*ptr = (uint8_t*)buffer->data;
	uint32_t const offset = (uint32_t)head->offset;
	uint32_t const stride = (uint32_t)head->stride;
	uint32_t const size   = (uint32_t)head->type;
	memcpy(attribut->data, ptr+offset + vertexId*stride, size);
}

/**
 * VERTEX PULLER
 */
void vertexPuller(GPUInVertex *const inVertex, GPUVertexPuller const*const vao, GPU const*const gpu, uint32_t nOfInvocation)
{
	uint32_t vertexId = nOfInvocation; // Primární nastavení
	GPUBuffer const* idbuffer;

	// Možnost použití při indexivaném kraslení
	if(vao->indices.type == UINT32 && gpu_isBuffer(gpu, vao->indices.bufferId))
	{
		idbuffer = gpu_getBuffer(gpu, vao->indices.bufferId);
		uint8_t const*ptr = (uint8_t*)idbuffer->data;
		memcpy(&vertexId, ptr + UINT32*nOfInvocation, UINT32);
	}
	else if(vao->indices.type == UINT16 && gpu_isBuffer(gpu, vao->indices.bufferId))
	{
		idbuffer = gpu_getBuffer(gpu, vao->indices.bufferId);
		uint8_t const*ptr = (uint8_t*)idbuffer->data;
		memcpy(&vertexId, ptr + UINT16*nOfInvocation, UINT16);
	}
	else if(vao->indices.type == UINT8 && gpu_isBuffer(gpu, vao->indices.bufferId))
	{
		idbuffer = gpu_getBuffer(gpu, vao->indices.bufferId);
		uint8_t const*ptr = (uint8_t*)idbuffer->data;
		memcpy(&vertexId, ptr + UINT8*nOfInvocation, UINT8);
	}

	// Uložení správnéo VertexId
	inVertex->gl_VertexID = vertexId;

	// Načtení všech Attributů
	for(size_t i = 0; i < MAX_ATTRIBUTES; i++)
	{
		copyAttribute(gpu, inVertex->attributes+i, vao->heads+i, vertexId);
	}
}

/**
 * PERSPEKTIVNÍ DĚLENÍ
 */
void perspectiveDivision(Vec4*const newpos, Vec4 const*const pos)
{
	for(int i = 0; i < 3; i++)
	{
		newpos->data[i] = pos->data[i]/pos->data[3];
	}
	newpos->data[3] = pos->data[3];
}

/**
 * ROZTAŽENÍ NA VELIKOST OKNA
 */
void stretch(Vec4* ptr, uint32_t width, uint32_t height)
{
	ptr->data[0] = (ptr->data[0]*0.5f + 0.5f)*width;
	ptr->data[1] = (ptr->data[1]*0.5f + 0.5f)*height;
	// ptr->data[2] a [3] zůstávájí
}

void perfragAndDraw(GPUOutFragment const*const outFragment, GPU*const gpu, Vec4 coord)
{
	GPUFramebuffer*const frame = &gpu->framebuffer;

	// Oříznutí obrazovkou
	if(coord.data[0] < 0 || coord.data[0] >= frame->width)
	{
		return;
	}
	if(coord.data[1] < 0 || coord.data[1] >= frame->height)
	{
		return;
	}
	if(isnan(coord.data[0]) || isnan(coord.data[1]))
	{
		return;
	}

	uint32_t const position = frame->width*(int)coord.data[1] + (int)coord.data[0];

	// Depth test
	if(frame->depth[position] <= coord.data[2])
	{
		return;
	}

	Vec4 color;
	copy_Vec4(&color, &outFragment->gl_FragColor);

	// Ořezání barvy na interval <0, 1>
	if(color.data[0] > 1.0f) color.data[0] = 1;
	if(color.data[0] < 0.0f) color.data[0] = 0;
	if(color.data[1] > 1.0f) color.data[0] = 1;
	if(color.data[1] < 0.0f) color.data[0] = 0;
	if(color.data[2] > 1.0f) color.data[0] = 1;
	if(color.data[2] < 0.0f) color.data[0] = 0;
	if(color.data[3] > 1.0f) color.data[0] = 1;
	if(color.data[3] < 0.0f) color.data[0] = 0;

	// Zápis do frame bufferu
	frame->color[position] = color;
	frame->depth[position] = coord.data[2];
}

/**
 * INTERPOLACE A FRAGMENT PROCESOR
 */
void calcAndDrawPixel(GPUVertexShaderData *const tri, float x, float y, GPUProgram const* prog, GPU*const gpu)
{
	// Připrava

	float Ax = tri[0].outVertex.gl_Position.data[0];
	float Ay = tri[0].outVertex.gl_Position.data[1];
	float Az = tri[0].outVertex.gl_Position.data[2];
	float w1 = tri[0].outVertex.gl_Position.data[3];

	float Bx = tri[1].outVertex.gl_Position.data[0];
	float By = tri[1].outVertex.gl_Position.data[1];
	float Bz = tri[1].outVertex.gl_Position.data[2];
	float w2 = tri[1].outVertex.gl_Position.data[3];

	float Cx = tri[2].outVertex.gl_Position.data[0];
	float Cy = tri[2].outVertex.gl_Position.data[1];
	float Cz = tri[2].outVertex.gl_Position.data[2];
	float w3 = tri[2].outVertex.gl_Position.data[3];

	// Délka stran
	float a = sqrt((Cx - Bx)*(Cx - Bx) + (Cy - By)*(Cy - By));
	float b = sqrt((Cx - Ax)*(Cx - Ax) + (Cy - Ay)*(Cy - Ay));
	float c = sqrt((Bx - Ax)*(Bx - Ax) + (By - Ay)*(By - Ay));
	float AP = sqrt((Ax - x)*(Ax - x) + (Ay - y)*(Ay - y));
	float BP = sqrt((Bx - x)*(Bx - x) + (By - y)*(By - y));
	float CP = sqrt((Cx - x)*(Cx - x) + (Cy - y)*(Cy - y));

	float S = (a + b + c)/2.0;
	S = sqrt(S*(S-a)*(S-b)*(S-c));

	float sTmp;

	sTmp = (a + CP + BP)/2.0;
	float l1 = sqrt(sTmp*(sTmp-a)*(sTmp-CP)*(sTmp-BP))/S;
	sTmp = (b + AP + CP)/2.0;
	float l2 = sqrt(sTmp*(sTmp-b)*(sTmp-AP)*(sTmp-CP))/S;
	float l3 = 1.0 - l1 - l2;
	
	if(isnan(l1) || isnan(l2) || isnan(l3))
	{
		l1 = 1.0f/3.0f;
		l2 = l1;
		l3 = l1;
	}

	GPUFragmentShaderData fd;

	fd.uniforms = tri[0].uniforms;

	GPUOutVertex A = tri[0].outVertex;
	GPUOutVertex B = tri[1].outVertex;
	GPUOutVertex C = tri[2].outVertex;

	float denom = l1/w1 + l2/w2 + l3/w3;

	Vec4 coord;
	init_Vec4(&coord, x, y, (Az*l1/w1 + Bz*l2/w2 + Cz*l3/w3)/denom, w1);
	memcpy(&fd.inFragment.gl_FragCoord, &coord, sizeof(Vec4));

	// Příprava proměnných pro interpolaci
	float attFloat, attAFloat, attBFloat, attCFloat;
	Vec2 attVec2, attAVec2, attBVec2, attCVec2;
	Vec3 attVec3, attAVec3, attBVec3, attCVec3;
	Vec4 attVec4, attAVec4, attBVec4, attCVec4;

	// Interpolace
	for(int i = 0; i < MAX_ATTRIBUTES; i++)
	{
		if(prog->vs2fsType[i] == ATTRIBUTE_EMPTY)
		{
			continue;
		}
		else if(prog->vs2fsType[i] == ATTRIBUTE_FLOAT)
		{
			memcpy(&attAFloat, &A.attributes[i].data[0], ATTRIBUTE_FLOAT);
			memcpy(&attBFloat, &B.attributes[i].data[0], ATTRIBUTE_FLOAT);
			memcpy(&attCFloat, &C.attributes[i].data[0], ATTRIBUTE_FLOAT);
			attFloat = (attAFloat*l1/w1 + attBFloat*l2/w2 + attCFloat*l3/w3)/denom;
			memcpy(&fd.inFragment.attributes[i].data[0], &attFloat, ATTRIBUTE_FLOAT);
		}
		else if(prog->vs2fsType[i] == ATTRIBUTE_VEC2)
		{
			memcpy(&attAVec2, &A.attributes[i].data[0], ATTRIBUTE_VEC2);
			memcpy(&attBVec2, &B.attributes[i].data[0], ATTRIBUTE_VEC2);
			memcpy(&attCVec2, &C.attributes[i].data[0], ATTRIBUTE_VEC2);
			
			for(int j = 0; j < 2; j++)
			{
				attVec2.data[j] = (attAVec2.data[j]*l1/w1 + attBVec2.data[j]*l2/w2 + attCVec2.data[j]*l3/w3)/denom;
			}

			memcpy(&fd.inFragment.attributes[i].data[0], &attVec2, ATTRIBUTE_VEC2);
		}
		else if(prog->vs2fsType[i] == ATTRIBUTE_VEC3)
		{
			memcpy(&attAVec3, &A.attributes[i].data[0], ATTRIBUTE_VEC3);
			memcpy(&attBVec3, &B.attributes[i].data[0], ATTRIBUTE_VEC3);
			memcpy(&attCVec3, &C.attributes[i].data[0], ATTRIBUTE_VEC3);
			
			for(int j = 0; j < 3; j++)
			{
				attVec3.data[j] = (attAVec3.data[j]*l1/w1 + attBVec3.data[j]*l2/w2 + attCVec3.data[j]*l3/w3)/denom;
			}

			memcpy(&fd.inFragment.attributes[i].data[0], &attVec3, ATTRIBUTE_VEC3);
		}
		else if(prog->vs2fsType[i] == ATTRIBUTE_VEC4)
		{
			memcpy(&attAVec4, &A.attributes[i].data[0], ATTRIBUTE_VEC4);
			memcpy(&attBVec4, &B.attributes[i].data[0], ATTRIBUTE_VEC4);
			memcpy(&attCVec4, &C.attributes[i].data[0], ATTRIBUTE_VEC4);
			
			for(int j = 0; j < 4; j++)
			{
				attVec4.data[j] = (attAVec4.data[j]*l1/w1 + attBVec4.data[j]*l2/w2 + attCVec4.data[j]*l3/w3)/denom;
			}

			memcpy(&fd.inFragment.attributes[i].data[0], &attVec4, ATTRIBUTE_VEC4);
		}
	}

	prog->fragmentShader(&fd);
	
	perfragAndDraw(&fd.outFragment, gpu, coord);
}

/**
 * RASTERIZACE
 * Fce provede rasterizasi a jednotlivé body pošle na vykreslení funkci calcAndDrawPixel
 * Využit kód z třetího grafického cvičení
 */
#define MAX(a, b) (((a) > (b)) ? (a):(b))
#define MIN(a, b) (((a) > (b)) ? (b):(a))
void rasterAndDraw(GPUVertexShaderData*const tri, GPU*const gpu, GPUProgram const* prog)
{
	// -0.5f -- korekce
	int x1 = tri[0].outVertex.gl_Position.data[0] - 0.5f;
	int x2 = tri[1].outVertex.gl_Position.data[0] - 0.5f;
	int x3 = tri[2].outVertex.gl_Position.data[0] - 0.5f;
	int y1 = tri[0].outVertex.gl_Position.data[1] - 0.5f;
	int y2 = tri[1].outVertex.gl_Position.data[1] - 0.5f;
	int y3 = tri[2].outVertex.gl_Position.data[1] - 0.5f;

	// Orientace vrcholů
	int minX = MIN(MIN(x1, x2), x3);
	int minY = MIN(MIN(y1, y2), y3);
	int maxX = MAX(MAX(x1, x2), x3);
	int maxY = MAX(MAX(y1, y2), y3);

	// Oříznutí
	minX = MAX(0, minX);
	minY = MAX(0, minY);
	maxX = MIN(gpu->framebuffer.width -1, maxX);
	maxY = MIN(gpu->framebuffer.height - 1, maxY);

	// Delta X
	int deltaX1 = x2 - x1;
	int deltaX2 = x3 - x2;
	int deltaX3 = x1 - x3;

	// Delta Y
	int deltaY1 = y2 - y1;
	int deltaY2 = y3 - y2;
	int deltaY3 = y1 - y3;

	// Hrana
	int edgeF1 = (minY - y1)*deltaX1 - (minX - x1)*deltaY1;
	int edgeF2 = (minY - y2)*deltaX2 - (minX - x2)*deltaY2;
	int edgeF3 = (minY - y3)*deltaX3 - (minX - x3)*deltaY3;

	// Rasterizace
	for(int y = minY; y <= maxY; y++)
	{
		bool even = ((y - minY) %2) == 0;

		for(int x = ((even) ? minX :maxX); (even) ? (x <= maxX) : (x >= minX); x += (even) ? 1 : -1)
		{
			// Kontrola hranových funkcí
			if(edgeF1 >= 0 && edgeF2 >= 0 && edgeF3 >= 0)
			{
				calcAndDrawPixel(tri, (float)x+0.5f, (float)y+0.5f, prog, gpu);
			}
			
			// Posun v ose Y
			if(!((even && x == maxX) || (!even && x == minX)))
			{
				edgeF1 += (even) ? -deltaY1 : deltaY1;
				edgeF2 += (even) ? -deltaY2 : deltaY2;
				edgeF3 += (even) ? -deltaY3 : deltaY3;
			}
		}

		// Posun v ose X
		edgeF1 += deltaX1;
		edgeF2 += deltaX2;
		edgeF3 += deltaX3;
	}
}
#undef MAX
#undef MIN

/// \addtogroup gpu_side Implementace vykreslovacího řetězce - vykreslování trojúhelníků
/// @{

/**
 * @brief This function should draw triangles
 *
 * @param gpu gpu 
 * @param nofVertices number of vertices
 */
void gpu_drawTriangles(GPU *const gpu, uint32_t nofVertices)
{

  /// \todo Naimplementujte vykreslování trojúhelníků.
  /// nofVertices - počet vrcholů
  /// gpu - data na grafické kartě
  /// Vašim úkolem je naimplementovat chování grafické karty.
  /// Úkol je složen:
  /// 1. z implementace Vertex Pulleru
  /// 2. zavolání vertex shaderu pro každý vrchol
  /// 3. rasterizace
  /// 4. zavolání fragment shaderu pro každý fragment
  /// 5. zavolání per fragment operací nad fragmenty (depth test, zápis barvy a hloubky)
  /// Více v připojeném videu.

	GPUProgram const* prog = gpu_getActiveProgram(gpu);
	GPUVertexPuller const* vao = gpu_getActivePuller(gpu);

	GPUVertexShaderData vd;
	GPUVertexShaderData tri[3];

	vd.uniforms = &prog->uniforms;

	int TriInLine = 0;
	for(uint32_t vertex = 0; vertex < nofVertices; vertex++)
	{
		// Vertex Puller načítá data
		vertexPuller(&vd.inVertex, vao, gpu, vertex);

		// Volání Vertex Shaderu pro každý vrchol
		prog->vertexShader(&vd);
		
		// Perspektivní dělení
		Vec4 pos;
		copy_Vec4(&pos, &vd.outVertex.gl_Position);
		Vec4 newpos;
		perspectiveDivision(&newpos, &pos);

		// Roztažení na velikost okna
		stretch(&newpos, gpu->framebuffer.width, gpu->framebuffer.height);
		
		copy_Vec4(&vd.outVertex.gl_Position, &newpos);

		// Přidání vrchlu do trojúhelníku
		memcpy(&tri[TriInLine], &vd, sizeof(GPUVertexShaderData));
		TriInLine++;

		// Je utvořen trojuhelník
		if(TriInLine == 3)
		{
			rasterAndDraw(tri, gpu, prog);
			TriInLine = 0;
		}

	}

	(void)gpu;
	(void)nofVertices;

}

/// @}
