
#include "CSimpleMesh.h"


namespace ion
{
	namespace Scene
	{

		CSimpleMesh::SVertex::SVertex()
		{}

		CSimpleMesh::SVertex::SVertex(
			vec3f const & position,
			vec3f const & normal,
			vec2f const & texture)
		{
			Position = position;
			Normal = normal;
			TextureCoordinates = texture;
		}

		CSimpleMesh::STriangle::STriangle()
		{
			Indices[0] = 0;
			Indices[1] = 0;
			Indices[2] = 0;
		}

		CSimpleMesh::STriangle::STriangle(uint const index0, uint const index1, uint const index2)
		{
			Indices[0] = index0;
			Indices[1] = index1;
			Indices[2] = index2;
		}


		CSimpleMesh * CSimpleMesh::FromAttributes(vector<uint> Indices, vector<f32> const & Positions, vector<f32> const & Normals, vector<f32> const & TexCoords)
		{
			CSimpleMesh * Mesh = new CSimpleMesh();

			for (uint i = 0; i < Positions.size() / 3; ++ i)
			{
				SVertex Vertex;

				Vertex.Position = vec3f(Positions[i * 3 + 0], Positions[i * 3 + 1], Positions[i * 3 + 2]);

				if (i * 3 + 2 < Normals.size())
					Vertex.Normal = vec3f(Normals[i * 3 + 0], Normals[i * 3 + 1], Normals[i * 3 + 2]);

				if (i * 2 + 1 < TexCoords.size())
					Vertex.TextureCoordinates = vec2f(TexCoords[i * 2 + 0], TexCoords[i * 2 + 1]);

				Mesh->Vertices.push_back(Vertex);
			}

			for (uint i = 0; i < Indices.size() / 3; ++ i)
			{
				Mesh->Triangles.push_back(STriangle(
					Indices[i * 3 + 0],
					Indices[i * 3 + 1],
					Indices[i * 3 + 2]));
			}

			return Mesh;
		}

		SBoundingBox3f CSimpleMesh::GetBoundingBox() const
		{
			SBoundingBox3f Box(
				vec3f(NumericLimits<float>().max()),
				vec3f(-NumericLimits<float>().max()));

			std::for_each(Vertices.begin(), Vertices.end(), [&Box](SVertex const & Vertex)
			{
				Box.AddInternalPoint(Vertex.Position);
			});

			return Box;
		}

		void CSimpleMesh::Clear()
		{
			Vertices.clear();
			Triangles.clear();
		}

		void CSimpleMesh::ResizeMesh(vec3f const & Scale)
		{
			vec3f const Extent = GetBoundingBox().GetExtent();
			vec3f const Resize = Scale / std::max(Extent.X, std::max(Extent.Y, Extent.Z));

			std::for_each(Vertices.begin(), Vertices.end(), [Resize](SVertex & Vertex)
			{
				Vertex.Position *= Resize;
			});
		}

		void CSimpleMesh::ApplyScaleFactor(vec3f const & Scale)
		{
			std::for_each(Vertices.begin(), Vertices.end(), [Scale](SVertex & Vertex)
			{
				Vertex.Position *= Scale;
			});
		}

		void CSimpleMesh::ApplyOffset(vec3f const & Offset)
		{
			std::for_each(Vertices.begin(), Vertices.end(), [Offset](SVertex & Vertex)
			{
				Vertex.Position += Offset;
			});
		}

		void CSimpleMesh::ApplyTransformation(glm::mat4 const & Transform)
		{
			std::for_each(Vertices.begin(), Vertices.end(), [Transform](SVertex & Vertex)
			{
				Vertex.Position = vec3f::FromGLMVector(Transform * glm::vec4(Vertex.Position.GetGLMVector(), 1));
			});
		}

		void CSimpleMesh::ReverseFaces()
		{
			std::for_each(Triangles.begin(), Triangles.end(), [](STriangle & Triangle)
			{
				std::swap(Triangle.Indices[1], Triangle.Indices[2]);
			});
		}

		void CSimpleMesh::SeparateTriangles()
		{
			std::vector<SVertex> newVertices;
			std::vector<STriangle> newTriangles;

			for (auto it = Triangles.begin(); it != Triangles.end(); ++ it)
			{
				for (int i = 0; i < 3; ++ i)
					newVertices.push_back(Vertices[it->Indices[i]]);
			}

			for (uint i = 0; i < newVertices.size() / 3; ++ i)
			{
				STriangle tri;
				tri.Indices[0] = i * 3;
				tri.Indices[1] = i * 3 + 1;
				tri.Indices[2] = i * 3 + 2;
				newTriangles.push_back(tri);
			}

			Vertices = newVertices;
			Triangles = newTriangles;
		}

		void CSimpleMesh::CalculateNormalsPerFace()
		{
			for (auto it = Triangles.begin(); it != Triangles.end(); ++ it)
			{
				it->Normal = (Vertices[it->Indices[1]].Position - Vertices[it->Indices[0]].Position).
					CrossProduct(Vertices[it->Indices[2]].Position - Vertices[it->Indices[0]].Position);
				Vertices[it->Indices[0]].Normal = Vertices[it->Indices[1]].Normal = Vertices[it->Indices[2]].Normal = it->Normal;
			}

			for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
			{
				it->Normal.Normalize();
			}
		}

		void CSimpleMesh::CalculateNormalsPerVertex(bool CombineNear, f32 const NearTolerance)
		{
			CalculateNormalsPerFace();

			for (auto it = Vertices.begin(); it != Vertices.end(); ++ it)
			{
				it->Normal = 0;
			}

			for (auto it = Triangles.begin(); it != Triangles.end(); ++ it)
			{
				for (int i = 0; i < 3; ++ i)
				{
					Vertices[it->Indices[i]].Normal += it->Normal;
				}
			}

			if (CombineNear)
			{
				for (uint i = 0; i < Vertices.size(); ++ i)
				{
					for (uint j = i + 1; j < Vertices.size(); ++ j)
					{
						if (Vertices[i].Position.GetDistanceSqFrom(Vertices[j].Position) < Sq(NearTolerance))
						{
							Vertices[i].Normal = Vertices[j].Normal = Vertices[i].Normal + Vertices[j].Normal;
						}
					}
				}
			}

			for (auto it = Vertices.begin(); it != Vertices.end(); ++ it)
			{
				it->Normal.Normalize();
			}
		}

		SharedPointer<Graphics::IIndexBuffer> CSimpleMesh::CreateIndexBuffer(Graphics::IGraphicsAPI * GraphicsAPI)
		{
			vector<u32> IndexData;
			IndexData.reserve(Triangles.size() * 3);

			std::for_each(Triangles.begin(), Triangles.end(), [&IndexData](STriangle const & Triangle)
			{
				for (uint i = 0; i < 3; ++ i)
				{
					IndexData.push_back(Triangle.Indices[i]);
				}
			});

			SharedPointer<Graphics::IIndexBuffer> IndexBuffer = GraphicsAPI->CreateIndexBuffer();
			IndexBuffer->UploadData(IndexData);
			return IndexBuffer;
		}

		SharedPointer<Graphics::IVertexBuffer> CSimpleMesh::CreateVertexBuffer(Graphics::IGraphicsAPI * GraphicsAPI)
		{
			vector<float> VertexData;
			VertexData.reserve(Vertices.size() * 12);

			std::for_each(Vertices.begin(), Vertices.end(), [&VertexData](SVertex const & Vertex)
			{
				for (uint i = 0; i < 3; ++ i)
				{
					VertexData.push_back(Vertex.Position[i]);
				}
				for (uint i = 0; i < 3; ++ i)
				{
					VertexData.push_back(Vertex.Normal[i]);
				}
				for (uint i = 0; i < 2; ++ i)
				{
					VertexData.push_back(Vertex.TextureCoordinates[i]);
				}
			});

			SharedPointer<Graphics::IVertexBuffer> VertexBuffer = GraphicsAPI->CreateVertexBuffer();
			VertexBuffer->UploadData(VertexData);
			Graphics::SInputLayoutElement InputLayout[] =
			{
				{ "vPosition",  3, Graphics::EAttributeType::Float },
				{ "vNormal",    3, Graphics::EAttributeType::Float },
				{ "vTexCoords", 2, Graphics::EAttributeType::Float },
			};
			VertexBuffer->SetInputLayout(InputLayout, ION_ARRAYSIZE(InputLayout));
			return VertexBuffer;
		}

	}
}
