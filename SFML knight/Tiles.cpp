#include "Tiles.hpp"



TileInfo::TileInfo(SharedContext* context, const std::string& texture, unsigned int id) : m_context(context), m_id(id)
{
	TextureManager* tmgr = context->m_textureManager;
	if (texture == "")
		return;

	if (!tmgr->RequireResource(texture))
		return;

	m_texture = texture;
	m_sprite.setTexture(*tmgr->GetResource(m_texture));
	sf::IntRect tileBoundaries(m_id % (Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,
		m_id / (Sheet::Sheet_Height / Sheet::Tile_Size) * Sheet::Tile_Size,	Sheet::Tile_Size, Sheet::Tile_Size);
	m_sprite.setTextureRect(tileBoundaries);
}

TileInfo::~TileInfo()
{
	if (m_texture == "")
		return;

	m_context->m_textureManager->ReleaseResource(m_texture);
}
