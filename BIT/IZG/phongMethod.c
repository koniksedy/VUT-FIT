/**
 * phongMethod.c
 * Autor: Michal Šedý, FIT
 * Řešení: IZG - projekt
 */

#include <student/emptyMethod.h>
#include <student/application.h>
#include <student/cpu.h>
#include <student/globals.h>
#include <student/bunny.h>

/// \addtogroup shader_side Úkoly v shaderech
/// @{

/**
 * @brief This function represents vertex shader of phong method.
 *
 * @param data vertex shader data
 */
void phong_VS(GPUVertexShaderData*const data){
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do
  /// clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve
  /// world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3)
  /// ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat
  /// osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující
  /// view a projekční matici.
  /// View matici čtěte z nulté uniformní proměnné a projekční matici
  /// čtěte z první uniformní proměnné.
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní
  /// struktuře.
	
	Vec3*position = (Vec3*)data->inVertex.attributes[0].data;
	Vec3*norm = (Vec3*)data->inVertex.attributes[1].data;
	Vec4 positionIn4Vec;


	copy_Vec3Float_To_Vec4(&positionIn4Vec, position, 1.0f);
	Mat4*viewMatrix = (Mat4*)data->uniforms->uniform[0].data;
	Mat4*projectionMatrix = (Mat4*)data->uniforms->uniform[1].data;
	Mat4 transform;
	multiply_Mat4_Mat4(&transform, projectionMatrix, viewMatrix);

	Vec4 newPos;
	multiply_Mat4_Vec4(&newPos, &transform, &positionIn4Vec);

	copy_Vec4(&data->outVertex.gl_Position, &newPos);

	copy_Vec3((Vec3*)data->outVertex.attributes[0].data, position);
	copy_Vec3((Vec3*)data->outVertex.attributes[1].data, norm);

	(void)data;

}

/**
 * @brief This function represents fragment shader of phong method.
 *
 * @param data fragment shader data
 */
void phong_FS(GPUFragmentShaderData*const data){
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací
  /// model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat
  /// interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br>
  /// Barvu zapište do proměnné gl_FragColor ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní 3 a pozici
  /// světla přečtěte z uniformní 2.
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v
  /// rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte podle normály povrchu.
  /// V případě, že normála směřuje kolmo vzhůru je difuzní barva čistě bílá.
  /// V případě, že normála směřuje vodorovně nebo dolů je difuzní barva čiště zelená.
  /// Difuzní barvu spočtěte lineární interpolací zelené a bíle barvy pomocí interpolačního parameteru t.
  /// Interpolační parameter t spočtěte z y komponenty normály pomocí t = y*y (samozřejmě s ohledem na negativní čísla).
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>

	#define CLIP(x,a,b) (((x)<(a)) ? (a) : (((x)>(b))?(b):(x)))
	
	Vec3 pos; 
	memcpy(&pos, (Vec3*)data->inFragment.attributes[0].data, sizeof(Vec3));
	Vec3 norm;
	memcpy(&norm, (Vec3*)data->inFragment.attributes[0].data, sizeof(Vec3));
	normalize_Vec3(&norm, &norm);

	float x = pos.data[0];
	float y = pos.data[1];
	float t = CLIP(norm.data[1] * norm.data[1], 0, 1);

	Vec4 white = { 1.0f, 1.0f, 1.0f, 1.0f};
	Vec4 green = { 0.0f, 1.0f, 0.0f, 1.0f};
	
	Vec4 specMat = { 1.0f, 1.0f, 1.0f, 1.0f};
	Vec4 difuzeMat;
	mix_Vec4(&difuzeMat, &white, &green, t);


	Vec3 posLight;
	memcpy(&posLight, data->uniforms->uniform[2].data, sizeof(Vec3));
	Vec3 posView;
	memcpy(&posView, data->uniforms->uniform[3].data, sizeof(Vec3));

	Vec4 light = { 1.0f, 1.0f, 1.0f, 1.0f};

	Vec3 VecView;
	sub_Vec3(&VecView, &posView, &pos);
	Vec3 VecLight;
	sub_Vec3(&VecLight, &posLight, &pos);
	Vec3 VecRef;
	reflect(&VecRef, &VecLight, &norm);

	normalize_Vec3(&VecLight, &VecLight);	
	float ScalarNormLight = fabs(norm.data[0]*VecLight.data[0]) + fabs(norm.data[1]*VecLight.data[1]) + fabs(norm.data[2]*VecLight.data[2]);
	ScalarNormLight = CLIP(ScalarNormLight, 0, 1);
	Vec4 MultiDifLightC = {difuzeMat.data[0]*light.data[0], difuzeMat.data[1]*light.data[1], difuzeMat.data[2]*light.data[2], difuzeMat.data[3]*light.data[3]};

	Vec4 colorDifuze;
	multiply_Vec4_Float(&colorDifuze, &MultiDifLightC, ScalarNormLight);

	normalize_Vec3(&VecView, &VecView);
	float ScalarViewRef = fabs(VecRef.data[0]*VecView.data[0]) + fabs(VecRef.data[1]*VecView.data[1]) + fabs(VecRef.data[2]*VecView.data[2]);
	ScalarViewRef = CLIP(ScalarViewRef, 0, 1);
	ScalarViewRef = pow(ScalarViewRef, 40);

	Vec4 colorSpec;
	multiply_Vec4_Float(&colorSpec, &light, ScalarViewRef);

	Vec4 colorDefinit;
	add_Vec4(&colorDefinit, &colorDifuze, &colorSpec);
	

	memcpy(&data->outFragment.gl_FragColor, &colorDefinit, sizeof(Vec4));
	

	#undef CLIP
}

/// @}

/// \addtogroup cpu_side Úkoly v cpu části
/// @{

/**
 * @brief This struct holds all variables of phong method.
 */
/// \todo Zde si vytvořte proměnné, které budete potřebovat (id bufferů, programu, ...)
struct PhongData{
	BufferID vbo;
	BufferID index;
	VertexPullerID vao;
	ProgramID prog;
	uint32_t nofVertices;
}phongData;///< this variable holds all data for phong method

/**
 * @brief This function initializes phong method.
 *
 * @param a data for initialization
 */
void phong_onInit(void*a){
	GPU*gpu = (GPU*)a;
	Vec4 clear;
	init_Vec4(&clear,.1f,.1f,.1f,1.f);
	cpu_clearColor(gpu,clear);
	cpu_clearDepth(gpu,1.f);

/// \todo Doprogramujte inicializační funkci.
/// Zde byste měli vytvořit buffery na GPU, nahrát data do bufferů, vytvořit
/// vertex puller a správně jej nakonfigurovat, vytvořit program, připojit k
/// němu shadery a nastavit atributy, které se posílají mezi vs a fs.
/// Do bufferů nahrajte vrcholy králička (pozice, normály) a indexy na vrcholy
/// ze souboru bunny.h.
/// Shader program by měl odkazovat na funkce/shadery phong_VS a phong_FS.
/// V konfiguraci vertex pulleru nastavte dvě čtecí hlavy.
/// Jednu pro pozice vrcholů a druhou pro normály vrcholů.
/// Nultý vertex/fragment atribut by měl obsahovat pozici vertexu.
/// První vertex/fragment atribut by měl obsahovat normálu vertexu.
/// Nastavte, které atributy (jaký typ) se posílají z vertex shaderu do fragment shaderu.
/// <b>Seznam funkcí, které jistě využijete:</b>
///  - cpu_createBuffer()
///  - cpu_bufferData()
///  - cpu_createVertexPuller()
///  - cpu_setVertexPuller()
///  - cpu_enableVertexPullerHead()
///  - cpu_setVertexPullerIndexing()
///  - cpu_createProgram()
///  - cpu_attachShaders()
///  - cpu_setVS2FSType()
	
	phongData.nofVertices = 2092*3;		

	phongData.vbo = cpu_createBuffer(gpu);
	cpu_bufferData(gpu, phongData.vbo, sizeof(bunnyVertices), (void*)bunnyVertices);
	phongData.index = cpu_createBuffer(gpu);
	cpu_bufferData(gpu, phongData.index, sizeof(bunnyIndices), (void*)bunnyIndices);

	phongData.vao = cpu_createVertexPuller(gpu);

	cpu_setVertexPuller(gpu, phongData.vao, 0, ATTRIBUTE_VEC3, sizeof(struct BunnyVertex), 0, phongData.vbo);
	cpu_enableVertexPullerHead(gpu, phongData.vao, 0);

	cpu_setVertexPuller(gpu, phongData.vao, 1, ATTRIBUTE_VEC3, sizeof(struct BunnyVertex), sizeof(Vec3), phongData.vbo);
	cpu_enableVertexPullerHead(gpu, phongData.vao, 1);

	cpu_setVertexPullerIndexing(gpu, phongData.vao, UINT32, phongData.index);

	phongData.prog = cpu_createProgram(gpu);
	cpu_attachShaders(gpu, phongData.prog, phong_VS, phong_FS);

	cpu_setVS2FSType(gpu, phongData.prog, 0, ATTRIBUTE_VEC3);
	cpu_setVS2FSType(gpu, phongData.prog, 1, ATTRIBUTE_VEC3);
	cpu_setVS2FSType(gpu, phongData.prog, 2, ATTRIBUTE_EMPTY);
	cpu_setVS2FSType(gpu, phongData.prog, 3, ATTRIBUTE_EMPTY);
	cpu_setVS2FSType(gpu, phongData.prog, 4, ATTRIBUTE_EMPTY);
	cpu_setVS2FSType(gpu, phongData.prog, 5, ATTRIBUTE_EMPTY);
	cpu_setVS2FSType(gpu, phongData.prog, 6, ATTRIBUTE_EMPTY);
	cpu_setVS2FSType(gpu, phongData.prog, 7, ATTRIBUTE_EMPTY);

}

/**
 * @brief This function draws phong method.
 *
 * @param a data
 */
void phong_onDraw(void*a){
  GPU*gpu = (GPU*)a;
  cpu_clear(gpu);

/// \todo Doprogramujte kreslící funkci.
/// Zde byste měli aktivovat shader program, aktivovat vertex puller, nahrát
/// data do uniformních proměnných a
/// vykreslit trojúhelníky pomocí funkce cpu_drawTriangles.
/// Data pro uniformní proměnné naleznete v externích globálních proměnnénych
/// viewMatrix, projectionMatrix, cameraPosition a lightPosition
/// <b>Seznam funkcí, které jistě využijete:</b>
///  - cpu_useProgram()
///  - cpu_bindVertexPuller()
///  - cpu_uniformMatrix4f()
///  - cpu_uniform3f()
///  - cpu_drawTriangles()
///  - cpu_unbindVertexPuller


	cpu_bindVertexPuller(gpu, phongData.vao);
	cpu_useProgram(gpu, phongData.prog);
	cpu_programUniformMatrix4f(gpu, phongData.prog, 0, viewMatrix);
	cpu_programUniformMatrix4f(gpu, phongData.prog, 1, projectionMatrix);
	cpu_programUniform3f(gpu, phongData.prog, 3, cameraPosition);
	cpu_programUniform3f(gpu, phongData.prog, 2, lightPosition);

	cpu_drawTriangles(gpu, phongData.nofVertices);

	cpu_unbindVertexPuller(gpu);
	
}

/**
 * @brief This functions frees all the phong data.
 *
 * @param a data
 */
void phong_onExit(void*a){
	GPU*gpu = (GPU*)a;

  ///\todo Zde uvolněte alokované zdroje

	cpu_deleteProgram(gpu, phongData.prog);
	cpu_deleteVertexPuller(gpu, phongData.vao);
	cpu_deleteBuffer(gpu, phongData.vbo);
	cpu_deleteBuffer(gpu, phongData.index);


}

/// @}
