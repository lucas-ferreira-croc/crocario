#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

#include <iostream>
#include <string>

class CrocPlataform :  public olc::PixelGameEngine
{

public:
	
	CrocPlataform()
	{
		sAppName = "croc_plataform";
	}

private:
	
	// Level

	std::wstring m_Level;
	bool play = false;

	int m_LevelWidth;
	int m_LevelHeight;

	// Camera
	float m_CameraPosX = 0.0f;
	float m_CameraPosY = 0.0f;

	// Player
	float m_PlayerPosX = 1.0f;
	float m_PlayerPosY = 1.0f;

	float m_PlayerVelX = 0.0f;
	float m_PlayerVelY = 0.0f;

	bool m_PlayerOnGround = false;
	bool m_PlayerMovingLeft = false;
	// Sprites
	olc::vf2d m_CrocSize = { 16.0f, 16.0f };
	olc::vi2d m_BrickSize = { 16.0f, 16.0f };

	// Crocario Sprite
	std::unique_ptr<olc::Sprite> m_CrocMovingRightTexture;
	std::unique_ptr<olc::Sprite> m_CrocMovingLeftTexture;
	
	// Sky sprites
	std::unique_ptr<olc::Sprite> m_ClearSkySprite;
	std::unique_ptr<olc::Sprite> m_DeepSkySprite;
	
	// Blocks sprites
	std::unique_ptr<olc::Sprite> m_BrickTexture;
	std::unique_ptr<olc::Sprite> m_ConstructionTexture;
	std::unique_ptr<olc::Sprite> m_FloorSprite;
	std::unique_ptr<olc::Sprite> m_QuestionSprite;
	std::unique_ptr<olc::Sprite> m_BrokenBlock;

	// Misc
	std::unique_ptr<olc::Sprite> m_CoinSprite;
	std::unique_ptr<olc::Sprite> m_CrocStudios;
protected:
	bool OnUserCreate() override
	{

		// map

		m_LevelWidth = 64;
		m_LevelHeight = 16;
		
		

		m_Level += L"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";
		m_Level += L"................................................................";
		m_Level += L"........ooooo...................................................";
		m_Level += L".........ooo....................................................";
		m_Level += L".......................########.................................";
		m_Level += L"......BB?BBBB?BB......###..............#.#......................";
		m_Level += L"....................###................#.#......................";
		m_Level += L"..................#####.........................................";
		m_Level += L"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG#.##############.....GGGGGGGG";
		m_Level += L"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG#.#,,,,,,,,,,,....###GGGGGGGG";
		m_Level += L"GGGGGGGGGGGGGGGGGGGGGGGG############.#,,,,,,,,,,,,###GGGGGGGGGGG";
		m_Level += L"GGGGGGGGGGGGGGGGGGGGGGGG#,,,,,,,,,,,.#,,,,,,,,,###GGGGGGGGGGGGGG";
		m_Level += L"GGGGGGGGGGGGGGGGGGGGGGGG#,############,,,,,,###GGGGGGGGGGGGGGGGG";
		m_Level += L"GGGGGGGGGGGGGGGGGGGGGGGG#,,,,,,,,,,,,,,,,###GGGGGGGGGGGGGGGGGGGG";
		m_Level += L"GGGGGGGGGGGGGGGGGGGGGGGG#################GGGGGGGGGGGGGGGGGGGGGGG";
		m_Level += L"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";

		m_CrocMovingRightTexture = std::make_unique<olc::Sprite>("./assets/crocario.png");
		m_CrocMovingLeftTexture = std::make_unique<olc::Sprite>("./assets/croc_left.png");
		
		m_ClearSkySprite = std::make_unique<olc::Sprite>("./assets/blue_sprite.png");
		m_DeepSkySprite = std::make_unique<olc::Sprite>("./assets/murky_blue.png");
		
		m_BrickTexture = std::make_unique<olc::Sprite>("./assets/brick_sprite_white.png");
		m_ConstructionTexture = std::make_unique<olc::Sprite>("./assets/constrution_sprite.png");
		m_FloorSprite = std::make_unique<olc::Sprite>("./assets/floor_sprite.png");
		m_QuestionSprite = std::make_unique<olc::Sprite>("./assets/question_sprite.png");
		m_BrokenBlock = std::make_unique<olc::Sprite>("./assets/block_sprite.png");
		
		m_CoinSprite = std::make_unique<olc::Sprite>("./assets/coin_sprite.png");
		m_CrocStudios = std::make_unique<olc::Sprite>("./assets/croc_studios.png");

		
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{	
		
		if (GetKey(olc::ENTER).bPressed)
		{
			play = true;
			m_CrocStudios = std::make_unique<olc::Sprite>("./assets/croc_studio.png");
		}

		if (play) 
		{
			// Utilities
			auto GetTile = [&](int x, int y)
			{
				if (x >= 0 && x < m_LevelWidth && y >= 0 && y < m_LevelHeight)
					return m_Level[y * m_LevelWidth + x];
				else
					return L' ';
			};

			auto SetTile = [&](int x, int y, wchar_t c)
			{
				if (x >= 0 && x < m_LevelWidth && y >= 0 && y < m_LevelHeight)
					m_Level[y * m_LevelWidth + x] = c;
			};

			//m_PlayerVelX = 0.0f;
			//m_PlayerVelY = 0.0f;

			// Input handling

			if (IsFocused())
			{
				if (GetKey(olc::UP).bHeld)
				{
					//m_PlayerVelY = -6.0f;
				}

				if (GetKey(olc::DOWN).bHeld)
				{
					m_PlayerVelY = 6.0f;
				}

				if (GetKey(olc::LEFT).bHeld)
				{
					m_PlayerVelX += (m_PlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
					m_PlayerMovingLeft = true;
				}

				if (GetKey(olc::RIGHT).bHeld)
				{
					m_PlayerVelX += (m_PlayerOnGround ? 25.0f : 15.0f) * fElapsedTime;
					m_PlayerMovingLeft = false;
				}

				if (GetKey(olc::SPACE).bPressed)
				{

					if (m_PlayerVelY == 0)
					{
						m_PlayerVelY = -12.0f;

					}
				}

			}

			// Gravity
			m_PlayerVelY += 20.0f * fElapsedTime;

			if (m_PlayerOnGround)
			{
				m_PlayerVelX += -3.0f * m_PlayerVelX * fElapsedTime;
				if (std::fabs(m_PlayerVelX) < 0.01f)
					m_PlayerVelX = 0.0f;
			}


			if (m_PlayerVelX > 10.0f)
				m_PlayerVelX = 10.0f;

			if (m_PlayerVelX < -10.0f)
				m_PlayerVelX = -10.0f;

			if (m_PlayerVelY > 100.0f)
				m_PlayerVelY = 100.0f;

			if (m_PlayerVelY < -100.0f)
				m_PlayerVelY = -100.0f;



			float newPlayerPosX = m_PlayerPosX + m_PlayerVelX * fElapsedTime;
			float newPlayerPosY = m_PlayerPosY + m_PlayerVelY * fElapsedTime;

			// Coin collision
			if (GetTile(newPlayerPosX + 0.0f, newPlayerPosY + 0.0f) == L'o')
				SetTile(newPlayerPosX + 0.0f, newPlayerPosY + 0.0f, L'.');

			if (GetTile(newPlayerPosX + 0.0f, newPlayerPosY + 1.0f) == L'o')
				SetTile(newPlayerPosX + 0.0f, newPlayerPosY + 1.0f, L'.');

			if (GetTile(newPlayerPosX + 1.0f, newPlayerPosY + 0.0f) == L'o')
				SetTile(newPlayerPosX + 1.0f, newPlayerPosY + 0.0f, L'.');

			if (GetTile(newPlayerPosX + 1.0f, newPlayerPosY + 1.0f) == L'o')
				SetTile(newPlayerPosX + 1.0f, newPlayerPosY + 1.0f, L'.');

			// Brick collision
			if (GetTile(newPlayerPosX + 0.0f, newPlayerPosY + 0.0f) == L'B')
			{
				m_PlayerVelY = 0.0f;
				m_PlayerVelY += 500.0f * fElapsedTime;
				SetTile(newPlayerPosX + 0.0f, newPlayerPosY + 0.0f, L'.');
			}

			// Question collision
			if (GetTile(newPlayerPosX + 0.0f, newPlayerPosY + 0.0f) == L'?')
			{
				m_PlayerVelY = 0.0f;
				m_PlayerVelY += 500.0f * fElapsedTime;
				SetTile(newPlayerPosX + 0.0f, newPlayerPosY + 0.0f, L'E');
			}

			// Collision

			if (m_PlayerVelX <= 0) // Left
			{
				if ((GetTile(newPlayerPosX + 0.0f, m_PlayerPosY + 0.0f) != L'.' && GetTile(newPlayerPosX + 0.0f, m_PlayerPosY + 0.0f) != L',')
					|| (GetTile(newPlayerPosX + 0.0f, m_PlayerPosY + 0.9f) != L'.' && GetTile(newPlayerPosX + 0.0f, m_PlayerPosY + 0.9f) != L','))
				{
					newPlayerPosX = (int)newPlayerPosX + 1;
					m_PlayerVelX = 0;
				}



			}
			else // Right
			{
				if ((GetTile(newPlayerPosX + 1.0f, m_PlayerPosY + 0.0f) != L'.' && GetTile(newPlayerPosX + 1.0f, m_PlayerPosY + 0.0f) != L',')
					|| (GetTile(newPlayerPosX + 1.0f, m_PlayerPosY + 0.9f) != L'.' && GetTile(newPlayerPosX + 1.0f, m_PlayerPosY + 0.9f) != L','))
				{
					newPlayerPosX = (int)newPlayerPosX;
					m_PlayerVelX = 0;

				}
			}

			m_PlayerOnGround = false;
			if (m_PlayerVelY <= 0) // Up
			{
				if ((GetTile(newPlayerPosX + 0.0f, newPlayerPosY) != L'.' && GetTile(newPlayerPosX + 0.0f, newPlayerPosY) != L',')
					|| (GetTile(newPlayerPosX + 0.9f, m_PlayerPosY) != L'.' && GetTile(newPlayerPosX + 0.9f, m_PlayerPosY) != L','))
				{
					newPlayerPosY = (int)newPlayerPosY + 1;
					m_PlayerPosY = 0;
				}



			}
			else // Down
			{
				if ((GetTile(newPlayerPosX + 0.0f, newPlayerPosY + 1.0f) != L'.' && GetTile(newPlayerPosX + 0.0f, newPlayerPosY + 1.0f) != L',')
					|| (GetTile(newPlayerPosX + 0.9f, newPlayerPosY + 1.0f) != L'.' && GetTile(newPlayerPosX + 0.9f, newPlayerPosY + 1.0f) != L','))
				{
					newPlayerPosY = (int)newPlayerPosY;
					m_PlayerVelY = 0;
					m_PlayerOnGround = true;
				}

			}

			// Repositioning the player

			m_PlayerPosX = newPlayerPosX;
			m_PlayerPosY = newPlayerPosY;

			// Linking camera position to player position
			m_CameraPosX = m_PlayerPosX;
			m_CameraPosY = m_PlayerPosY;

			// Draw Level

			int tileWidth = 16;
			int tileHeight = 16;

			int visibleTilesX = ScreenWidth() / tileWidth;
			int visibleTilesY = ScreenHeight() / tileHeight;

			// Calculate Top-Leftmost visible tile 
			float OffsetX = m_CameraPosX - (float)visibleTilesX / 2.0f;
			float OffsetY = m_CameraPosY - (float)visibleTilesY / 2.0f;

			// Clamp camera to boundaries
			if (OffsetX < 0) OffsetX = 0;
			if (OffsetY < 0) OffsetY = 0;
			if (OffsetX > m_LevelWidth - visibleTilesX) OffsetX = m_LevelWidth - visibleTilesX;
			if (OffsetY > m_LevelHeight - visibleTilesY) OffsetY = m_LevelHeight - visibleTilesY;

			// Get offsets for smooth movement
			float tileOffsetX = (OffsetX - (int)OffsetX) * tileWidth;
			float tileOffsetY = (OffsetY - (int)OffsetY) * tileHeight;


			// Drawing the map
			for (int x = -1; x < visibleTilesX + 1; x++)
			{
				for (int y = -1; y < visibleTilesY + 1; y++)
				{
					wchar_t tileID = GetTile(x + OffsetX, y + OffsetY);
					switch (tileID)
					{
					case L'.':
						/*FillRect(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY,
						(x + 1) * tileWidth - tileOffsetX, (y + 1) * tileHeight - tileOffsetY, olc::CYAN);
						*/
						DrawSprite(olc::vi2d(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY), m_ClearSkySprite.get());
						break;
					case L'#':
						/*FillRect(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY,
							(x + 1) * tileWidth - tileOffsetX, (y + 1) * tileHeight - tileOffsetY, olc::RED);
						*/
						DrawSprite(olc::vi2d(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY), m_ConstructionTexture.get());
						break;
					case L'G':
						/*FillRect(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY,
							(x + 1) * tileWidth - tileOffsetX, (y + 1) * tileHeight - tileOffsetY, olc::RED);
						*/
						DrawSprite(olc::vi2d(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY), m_FloorSprite.get());
						break;
					case L'B':
						/*FillRect(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY,
							(x + 1) * tileWidth - tileOffsetX, (y + 1) * tileHeight - tileOffsetY, olc::RED);
						*/
						DrawSprite(olc::vi2d(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY), m_BrickTexture.get());
						break;
					case L'?':
						/*FillRect(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY,
							(x + 1) * tileWidth - tileOffsetX, (y + 1) * tileHeight - tileOffsetY, olc::RED);
						*/
						DrawSprite(olc::vi2d(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY), m_QuestionSprite.get());
						break;
					case L'o':
						/*FillRect(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY,
							(x + 1) * tileWidth - tileOffsetX, (y + 1) * tileHeight - tileOffsetY, olc::RED);
						*/
						DrawSprite(olc::vi2d(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY), m_CoinSprite.get());
						break;
					case L',':
						/*FillRect(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY,
							(x + 1) * tileWidth - tileOffsetX, (y + 1) * tileHeight - tileOffsetY, olc::RED);
						*/
						DrawSprite(olc::vi2d(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY), m_DeepSkySprite.get());
						break;
					case L'E':
						/*FillRect(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY,
							(x + 1) * tileWidth - tileOffsetX, (y + 1) * tileHeight - tileOffsetY, olc::RED);
						*/
						DrawSprite(olc::vi2d(x * tileWidth - tileOffsetX, y * tileHeight - tileOffsetY), m_BrokenBlock.get());
						break;
					default:
						break;
					}
				}
			}

			//FillRect((m_PlayerPosX - OffsetX) * tileWidth, (m_PlayerPosY - OffsetY) * tileHeight, tileWidth, tileHeight, olc::GREEN);

			SetPixelMode(olc::Pixel::MASK);
			DrawSprite(olc::vi2d((m_PlayerPosX - OffsetX) * tileWidth, (m_PlayerPosY - OffsetY) * tileWidth), m_PlayerMovingLeft ? m_CrocMovingLeftTexture.get() : m_CrocMovingRightTexture.get());
			SetPixelMode(olc::Pixel::NORMAL);

		}

		DrawSprite(olc::vi2d((ScreenWidth() / 16) + 10, (ScreenHeight() / 16) - 15),  m_CrocStudios.get() , 2);

		return true;
	}
};


int main()
{
	CrocPlataform demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}