
#include <ionWindow.h>
#include <ionGraphics.h>
#include <ionGraphicsGL.h>


using namespace ion::Graphics;


int main()
{
	////////////////////
	// ionEngine Init //
	////////////////////

	Log::AddDefaultOutputs();

	SingletonPointer<CWindowManager> WindowManager;
	SingletonPointer<CTimeManager> TimeManager;
	WindowManager->Init();
	TimeManager->Init();

	CWindow * Window = WindowManager->CreateWindow(vec2i(640, 480), "TestGL", EWindowType::Windowed);


	//////////////////
	// Buffer Setup //
	//////////////////

	vector<f32> const Vertices
	{
		// Position    // Tex  // Color
		 0.5f,  0.5f,   1, 1,   1, 0, 0,
		 0.5f, -0.5f,   1, 0,   0, 1, 0,
		-0.5f, -0.5f,   0, 0,   0, 0, 1,
		-0.5f,  0.5f,   0, 1,   0, 1, 1,
	};

	vector<u32> const Indices
	{
		0, 1, 2,
		0, 2, 3,
	};

	IGraphicsAPI * GraphicsAPI = new COpenGLAPI();

	IVertexBuffer * VertexBuffer = GraphicsAPI->CreateVertexBuffer(Vertices.data(), Vertices.size());
	IIndexBuffer * IndexBuffer = GraphicsAPI->CreateIndexBuffer(Indices.data(), Indices.size(), EValueType::UnsignedInt32);
	

	//////////////////
	// Shader Setup //
	//////////////////

	string const VertexShaderSource = R"SHADER(
		#version 130

		in vec2 vPosition;
		in vec2 vTexCoords;
		in vec3 vColor;

		out vec2 fTexCoords;
		out vec3 fColor;

		void main()
		{
			gl_Position = vec4(vPosition, 0.0, 1.0);
			fTexCoords = vTexCoords;
			fColor = vColor;
		}
	)SHADER";

	string const FragmentShaderSource = R"SHADER(
		#version 130

		in vec2 fTexCoords;
		in vec3 fColor;
		uniform float uCurrentTime; 
		uniform sampler2D uTexture;
		out vec4 outColor;

		void main()
		{
			const float Pi = 3.1415926535897932384626433832795;

			float Alpha = (cos(uCurrentTime * 3.0) + 1.0) / 2.0;
			float Visibility = sin(uCurrentTime * 1.5 + Pi / 2.0);
			outColor = vec4(fColor * Alpha, 1.0);
			if (Visibility < 0.0)
				outColor.rgb *= texture(uTexture, fTexCoords).rgb;
		}
	)SHADER";

	IVertexShader * VertexShader = GraphicsAPI->CreateVertexShaderFromSource(VertexShaderSource);
	IPixelShader * PixelShader = GraphicsAPI->CreatePixelShaderFromSource(FragmentShaderSource);

	if (! VertexShader)
		std::cerr << "Failed to compile vertex shader!" << std::endl;

	if (! PixelShader)
		std::cerr << "Failed to compile pixel shader!" << std::endl;

	IShaderProgram * ShaderProgram = GraphicsAPI->CreateShaderProgram();
	ShaderProgram->SetVertexStage(VertexShader);
	ShaderProgram->SetPixelStage(PixelShader);

	SInputLayoutElement InputLayout[] =
	{
		{ "vPosition", 2, EValueType::Float },
		{ "vTexCoords", 2, EValueType::Float },
		{ "vColor", 3, EValueType::Float },
	};
	ShaderProgram->SetInputLayout(InputLayout, ION_ARRAYSIZE(InputLayout));
	

	///////////////
	// Draw Loop //
	///////////////

	IPipelineState * PipelineState = GraphicsAPI->CreatePipelineState();
	PipelineState->SetIndexBuffer(IndexBuffer);
	PipelineState->SetVertexBuffer(VertexBuffer);
	PipelineState->SetProgram(ShaderProgram);

	CUniformValue<float> uCurrentTime;
	PipelineState->SetUniform("uCurrentTime", &uCurrentTime);

	CImage * Image = CImage::Load("Image.jpg");
	ITexture2D * Texture = GraphicsAPI->CreateTexture2D(Image->GetSize(), true, ITexture::EFormatComponents::RGB, ITexture::EInternalFormatType::Fix8);
	Texture->Upload(Image->GetData(), Image->GetSize(), ITexture::EFormatComponents::RGB, EScalarType::UnsignedInt8);
	PipelineState->SetTexture("uTexture", Texture);

	PipelineState->Load();

	IRenderTarget * RenderTarget = GraphicsAPI->GetWindowBackBuffer(Window);

	while (WindowManager->Run())
	{
		TimeManager->Update();
		uCurrentTime = (float) TimeManager->GetRunTime();

		RenderTarget->ClearColorAndDepth();
		GraphicsAPI->Draw(PipelineState);
		Window->SwapBuffers();
	}


	/////////////
	// Cleanup //
	/////////////

	return 0;
}
