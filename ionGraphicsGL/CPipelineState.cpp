
#include "CPipelineState.h"

#include <GL/glew.h>


namespace ion
{
	namespace Graphics
	{
		namespace GL
		{

			void CPipelineState::SetProgram(IShaderProgram * inShaderProgram)
			{
				ShaderProgram = dynamic_cast<CShaderProgram *>(inShaderProgram);
				Loaded = false;
			}

			void CPipelineState::SetVertexBuffer(IVertexBuffer * inVertexBuffer)
			{
				VertexBuffer = dynamic_cast<CVertexBuffer *>(inVertexBuffer);
				// Bound VBOs are not part of VAO state
				Loaded = false;
			}

			void CPipelineState::SetIndexBuffer(IIndexBuffer * inIndexBuffer)
			{
				IndexBuffer = dynamic_cast<CIndexBuffer *>(inIndexBuffer);
				CheckedGLCall(glBindVertexArray(VertexArrayHandle));
				CheckedGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer->Handle));
				CheckedGLCall(glBindVertexArray(0));

				Loaded = false;
			}

			void CPipelineState::Load()
			{
				if (! ShaderProgram->Linked)
				{
					ShaderProgram->Link();
				}

				CheckedGLCall(glUseProgram(ShaderProgram->Handle));
				CheckedGLCall(glBindVertexArray(VertexArrayHandle));
				CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer->Handle));

				size_t CurrentOffset = 0;
				for (auto & InputLayoutElement : ShaderProgram->InputLayout)
				{
					int AttributeLocation = -1;
					CheckedGLCall(AttributeLocation = glGetAttribLocation(ShaderProgram->Handle, InputLayoutElement.Name.c_str())); // BUGBUG Get this through reflection?
					CheckedGLCall(glEnableVertexAttribArray(AttributeLocation));
					CheckedGLCall(glVertexAttribPointer(AttributeLocation, InputLayoutElement.Components, GL_FLOAT, GL_FALSE, 0, (void *) CurrentOffset));

					CurrentOffset += sizeof(float) * InputLayoutElement.Components;
				}

				CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Remember, VBOs are not part of VAO state
				CheckedGLCall(glBindVertexArray(0));
				CheckedGLCall(glUseProgram(0));

				Loaded = true;
			}

		}
	}
}
