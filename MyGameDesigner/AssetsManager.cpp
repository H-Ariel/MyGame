#include "AssetsManager.h"
#include "WindowBase.h"

#include <fstream>


#define IMAGES_PATH					"./assets/images"
#define TILES_IMAGE_PATH			IMAGES_PATH "/tiles.png"
#define ENEMIES_IMAGE_PATH			IMAGES_PATH "/enemies.png"
#define ENEMIES_JSON_PATH			IMAGES_PATH "/enemies.json"


vector<D2D1_RECT_F> GetTilesPositionsVector()
{
	const int IMAGES_IN_X = 32, IMAGES_IN_Y = 31;

	vector<D2D1_RECT_F> v;

	for (int j, i = 0; i < IMAGES_IN_Y; i++)
		for (j = 0; j < IMAGES_IN_X; j++)
			v.push_back(D2D1::RectF(
				j * REAL_IMAGE_SIZE,
				i * REAL_IMAGE_SIZE,
				(j + 1) * REAL_IMAGE_SIZE,
				(i + 1) * REAL_IMAGE_SIZE
			));

	return v;
}

map<string, D2D1_RECT_F> LoadJsonToMap(string filename)
{
	map<string, D2D1_RECT_F> m;
	json::FromFile(filename).dict_iterate([&](const string& k, const json& v) {
		m[k] =
			D2D1::RectF(
				v["left"],
				v["top"],
				v["right"],
				v["bottom"]
			);
		});

	return m;
}


const vector<D2D1_RECT_F> AssetsManager::TilesPositions(GetTilesPositionsVector());
const map<string, D2D1_RECT_F> AssetsManager::EnemiesPositions(LoadJsonToMap(ENEMIES_JSON_PATH));


AssetsManager::AssetsManager(WindowBase* window)
{
	THROW_IF_NULL(TilesImageSet = window->LoadBitapImage(TILES_IMAGE_PATH));
	THROW_IF_NULL(EnemiesImageSet = window->LoadBitapImage(ENEMIES_IMAGE_PATH));
}

AssetsManager::~AssetsManager()
{
	if (TilesImageSet != nullptr) TilesImageSet->Release();
	if (EnemiesImageSet != nullptr) EnemiesImageSet->Release();
}
