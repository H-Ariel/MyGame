#include "LevelMap.h"
#include "LevelDisplay_Window.h"
#include "UIButtonElement.h"

#include <fstream>
#include <sstream>


struct LevelCell
{
	short tile_value, foreground_tile_value;
};
vector<vector<LevelCell>> levelMatrix;


// tiles that can appear on other tiles
const initializer_list<short> FOREGROUND_TILES = {
	480, 481, 482, 483, 484, 485, 486, 487, 488, 501, 502, 503, 504, 505, 506, 507,
	584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599,
	600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615,
	616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 702, 703, 704, 708, 713, 714,
	715, 716, 762, 763, 764, 765, 766, 767, 768, 769, 770, 771, 772, 773, 774, 775,
	776, 777, 778, 779, 780, 781, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831,
	832, 833, 834, 835, 836, 837, 838,
};
const initializer_list<short> MOVING_TILES_ID = {
	70, 71, 72, 73, 99, 100, 101, 102, 122, 123, 124, 125,
	176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
	213, 214, 215, 216, 217, 218, 219, 220, 321, 322, 323, 324,
	350, 351, 352, 353, 356, 413, 414, 415, 416, 816, 817, 818,
	// 819,820,821, // idustrial 2 (small tiles)
	872,873,874,875,
	
	// mushrooms are moving-tiles ?
	//929, 930, 931, 932, 933, 934, 935, 936,
	//937, 938, 939, 940, 941, 942, 943, 944,
	//945, 946, 947, 948, 949, 950, 951, 952,
	//953, 954, 955, 956, 957, 958, 959, 960
};


LevelMap::LevelMap(LevelDisplay_Window* window, D2D1::ColorF& dstBgColor)
	: UIElementBase(window), window(window), wndSize({0,0}), playerInitPos({0,0}), bgColor(dstBgColor)
{
	// fiil default map values
	Clear();
}

void LevelMap::Logic(float elapsedTime) { wndSize = window->GetWindowSize(); }
void LevelMap::Draw()
{
	D2D1_RECT_F src = {}, dst = {};

	for (int j, i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (j = 0; j < LEVEL_WIDTH; j++)
		{
			dst = D2D1::RectF(
				j * TILE_SIZE_F,
				i * TILE_SIZE_F,
				(j + 1) * TILE_SIZE_F,
				(i + 1) * TILE_SIZE_F
			);

			if (InCameraFrame(dst))
			{
				RemoveOffset(dst);

				window->DrawTile(dst, levelMatrix[i][j].tile_value);
				window->DrawTile(dst, levelMatrix[i][j].foreground_tile_value);

				window->DrawRectangle(dst, D2D1::ColorF::White);
			}
		}
	}

	for (const EnemyData& e : enemies)
	{
		// draw range

		window->DrawLine(
			{ (e.initPos.x + 0.5f) * TILE_SIZE - position.x, (e.initPos.y + 0.5f) * TILE_SIZE - position.y },
			{ (e.finPos.x + 0.5f) * TILE_SIZE - position.x, (e.finPos.y + 0.5f) * TILE_SIZE - position.y },
			D2D1::ColorF::DarkRed
		);

		// draw enemy

		string name(e.name);
		if (name == "spider" || name == "snakeGreen" || name == "snakeRed") name += "_1";
		src = window->assetsManager->EnemiesPositions.at(name);
		dst = D2D1::RectF(
			e.initPos.x * TILE_SIZE_F,
			(e.initPos.y + 1) * TILE_SIZE_F - ORDER_SIZE(src.bottom - src.top),
			e.initPos.x * TILE_SIZE_F + ORDER_SIZE(src.right - src.left),
			(e.initPos.y + 1) * TILE_SIZE_F
		);

		if (InCameraFrame(dst))
		{
			RemoveOffset(dst);
			window->DrawBitmap(window->assetsManager->EnemiesImageSet, dst, src);
		}
	}

	for (const MovingTileData& mt : movingTiles)
	{
		// draw range

		window->DrawLine(
			{ (mt.initPos.x + 0.5f) * TILE_SIZE - position.x, (mt.initPos.y + 0.5f) * TILE_SIZE - position.y },
			{ (mt.finPos.x + 0.5f) * TILE_SIZE - position.x, (mt.finPos.y + 0.5f) * TILE_SIZE - position.y },
			D2D1::ColorF::DarkRed
		);

		// draw tile

		dst = D2D1::RectF(
			mt.initPos.x * TILE_SIZE_F,
			mt.initPos.y * TILE_SIZE_F,
			(mt.initPos.x + 1) * TILE_SIZE_F,
			(mt.initPos.y + 1) * TILE_SIZE_F
		);
		if (InCameraFrame(dst))
		{
			RemoveOffset(dst);
			window->DrawTile(dst, mt.tile_id);
		}
	}

	for (const SpringboardData& s : springs)
	{
		dst = D2D1::RectF(
			s.position.x * TILE_SIZE,
			s.position.y * TILE_SIZE,
			(s.position.x + 1) * TILE_SIZE,
			(s.position.y + 1) * TILE_SIZE
		);
		if (InCameraFrame(dst))
		{
			RemoveOffset(dst);
			window->DrawTile(dst, SpringboardDown);
		}
	}

	//  draw player
	{
		dst = D2D1::RectF((playerInitPos.x + 0.2f) * TILE_SIZE, (playerInitPos.y + 0.2f) * TILE_SIZE,
			(playerInitPos.x + 0.8f) * TILE_SIZE, (playerInitPos.y + 0.8f) * TILE_SIZE);

		if (InCameraFrame(dst))
		{
			RemoveOffset(dst);
			window->FillRectangle(dst, D2D1::ColorF::Goldenrod);
		}
	}


	if (chooseFinalPosition)
	{
		const D2D1_POINT_2U uPt = GetTilePosition(window->mousePosition);
		const D2D1_POINT_2F tilePos = { (float)uPt.x, (float)uPt.y };

		switch (GlobalVariables::SelectedType)
		{
		case Type::TILE:
		{
			const MovingTileData& mt = movingTiles[movingTiles.size() - 1];
			window->DrawLine(
				{ (mt.initPos.x + 0.5f) * TILE_SIZE - position.x, (mt.initPos.y + 0.5f) * TILE_SIZE - position.y },
				{ (tilePos.x + 0.5f) * TILE_SIZE - position.x, (tilePos.y + 0.5f) * TILE_SIZE - position.y },
				D2D1::ColorF::DarkRed
			);
		}	break;


		case Type::ENEMY:
		{
			const EnemyData& e = enemies[enemies.size() - 1];
			window->DrawLine(
				{ (e.initPos.x + 0.5f) * TILE_SIZE - position.x, (e.initPos.y + 0.5f) * TILE_SIZE - position.y },
				{ (tilePos.x + 0.5f) * TILE_SIZE - position.x, (tilePos.y + 0.5f) * TILE_SIZE - position.y },
				D2D1::ColorF::DarkRed
			);
		}	break;
		}
	}

	// Draw Selected Tile (with low opacity)
	if (!window->mouseButtonLeftPressed)
	{
		const D2D1_POINT_2U U_tilePos = GetTilePosition(window->mousePosition);
		const D2D1_POINT_2F tilePos = { (float)U_tilePos.x, (float)U_tilePos.y };

		switch (GlobalVariables::SelectedType)
		{
		case Type::TILE:
			window->DrawBitmap(
				window->assetsManager->TilesImageSet, D2D1::RectF(
					tilePos.x * TILE_SIZE - position.x,
					tilePos.y * TILE_SIZE - position.y,
					(tilePos.x + 1) * TILE_SIZE - position.x,
					(tilePos.y + 1) * TILE_SIZE - position.y),
				window->assetsManager->TilesPositions[GlobalVariables::SelectedId], false, 0.7f);
			break;

		case Type::ENEMY:
			src = window->assetsManager->EnemiesPositions.at(GlobalVariables::EnemyName);
			window->DrawBitmap(window->assetsManager->EnemiesImageSet, D2D1::RectF(
				tilePos.x * TILE_SIZE - position.x,
				(tilePos.y + 1) * TILE_SIZE - ORDER_SIZE(src.bottom - src.top) - position.y,
				tilePos.x * TILE_SIZE + ORDER_SIZE(src.right - src.left) - position.x,
				(tilePos.y + 1) * TILE_SIZE - position.y
			), src, false, 0.7f);
			break;

		case Type::NONE:
			// draw red X (sign of delete)
			window->DrawLine(
				{ tilePos.x * TILE_SIZE - position.x, tilePos.y * TILE_SIZE - position.y },
				{ (tilePos.x + 1) * TILE_SIZE - position.x, (tilePos.y + 1) * TILE_SIZE - position.y },
				D2D1::ColorF::Red, 5);
			window->DrawLine(
				{ (tilePos.x + 1) * TILE_SIZE - position.x, tilePos.y * TILE_SIZE - position.y },
				{ tilePos.x * TILE_SIZE - position.x, (tilePos.y + 1) * TILE_SIZE - position.y },
				D2D1::ColorF::Red, 5);
			break;
		}
	}
}

class SpringWindow : public WindowBase
{
public:
	SpringWindow(SpringboardData& sData)
		: WindowBase(WS_OVERLAPPED | WS_CAPTION), springData(sData), data(this), bOK(this), command(this), input(this), bCancel(this)
	{
		backgroundColor = 0xD0D0D0;

		RECT rc = {};
		rc.top = 250; rc.bottom = 650;
		rc.left = 300; rc.right = 900;
		SetWindowRect(rc);
		SetTitle(L"Set Springboard Data");


		data.position.y = 10;
		data.size = { 600,80 };
		data.font.size = 20;
		data.text = L"position: {" + to_wstring(springData.position.x) + L", " + to_wstring(springData.position.y) + L"}\n";


		command.position.y = 100;
		command.size = { 600,80 };
		command.font.size = 20;
		command.text = L"insert the spring force value:";


		input.position.y = 190;
		input.size = { 600,80 };
		input.font.size = 20;
		input.text = L"";


		bOK.text = L"OK";
		bOK.size = { 100,50 };
		bOK.position = { 150,300 };
		bOK.OnClick = [&](MouseButtons)
		{
			if (input.text.empty())
			{
				MessageBoxA(NULL, "\"force\" can not be empty", "Error", MB_ICONERROR | MB_OK);
			}
			else
			{
				springData.force = (float)stoi(input.text);
				RunWindow = false;
				springData.used = true;
			}
		};
		bOK.bgcolor = D2D1::ColorF::Beige;


		bCancel.text = L"Cancel";
		bCancel.size = { 150,50 };
		bCancel.position = { 320,300 };
		bCancel.OnClick = [&](MouseButtons) { RunWindow = false; };
		bCancel.bgcolor = D2D1::ColorF::Beige;
		springData.used = false;


		AddUIElement(&data);
		AddUIElement(&command);
		AddUIElement(&input);
		AddUIElement(&bOK);
		AddUIElement(&bCancel);
	}
	void Logic(float elapsedTime)
	{
		WindowBase::Logic(elapsedTime);

		if (RunWindow)
			RunWindow = GameDesignerWindowBase::RunApp;
	}
	void OnKeyUp(int key) override
	{
		switch (key)
		{
		case VK_BACK:
			// delete last char
			if (input.text.length() > 0)
				input.text.pop_back();
			break;

		case VK_RETURN:
			// enter -> OK
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			input.text += key;
			break;

		case VK_NUMPAD0:
		case VK_NUMPAD1:
		case VK_NUMPAD2:
		case VK_NUMPAD3:
		case VK_NUMPAD4:
		case VK_NUMPAD5:
		case VK_NUMPAD6:
		case VK_NUMPAD7:
		case VK_NUMPAD8:
		case VK_NUMPAD9:
			input.text += key - VK_NUMPAD0 + '0';
			break;

			//default: MessageBoxA(0, to_string(key).c_str(), "", 0);
		}
	}

private:
	SpringboardData& springData;
	UITextElement data, command, input;
	UIButtonElement bOK, bCancel;
};
SpringboardData GetSpringData(D2D1_POINT_2U pos)
{
	SpringboardData ed = {};
	ed.used = true;
	ed.position = D2D1::Point2F((float)pos.x, (float)pos.y);

	SpringWindow eWnd(ed);
	eWnd.Run();

	return ed;
}


void LevelMap::ChangeTile(D2D1_POINT_2U mousePos)
{
	const D2D1_POINT_2U p = GetTilePosition(mousePos);

	if (
		(p.y < 0 || levelMatrix.size() <= p.y)||
		(p.x< 0 || levelMatrix[p.y].size() <= p.x)
		)
	{
		wchar_t msg[40] = {};
		wsprintf(msg, L"invalid index:\nX = %d   Y = %d", p.x, p.y);
		MessageBox(nullptr, msg, L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	switch (GlobalVariables::SelectedType)
	{
	case Type::TILE:
	{
		if (GlobalVariables::SelectedId == SpringboardDown)
		{
			SpringboardData sData = GetSpringData(p);
			if (sData.used) springs.push_back(sData);
			window->mouseButtonLeftPressed = false;
		}
		else if (FIND_IN_ARRAY(MOVING_TILES_ID, GlobalVariables::SelectedId))
		{
			if (chooseFinalPosition)
			{
				movingTiles[movingTiles.size() - 1].finPos = { (LONG)p.x,(LONG)p.y };
				movingTiles[movingTiles.size() - 1].used = true;
				chooseFinalPosition = false;
				GlobalVariables::SelectedType = Type::NONE;
				GlobalVariables::SelectedId = Empty;
				window->mouseButtonLeftPressed = false;
			}
			else
			{
				AddMovingTile(p);
			}
		}
		else
		{
			if (FIND_IN_ARRAY(FOREGROUND_TILES, GlobalVariables::SelectedId))
				levelMatrix[p.y][p.x].foreground_tile_value = GlobalVariables::SelectedId;
			else
				levelMatrix[p.y][p.x].tile_value = GlobalVariables::SelectedId;
		}
	}
	break;

	case Type::ENEMY:
	{
		if (chooseFinalPosition)
		{
			enemies[enemies.size() - 1].finPos = { (LONG)p.x,(LONG)p.y };
			enemies[enemies.size() - 1].used = true;
			chooseFinalPosition = false;
			GlobalVariables::SelectedType = Type::NONE;
			GlobalVariables::EnemyName.clear();
			window->mouseButtonLeftPressed = false;
		}
		else
		{
			AddEnemy(p);
		}
	}
	break;

	case Type::NONE:
	{
		levelMatrix[p.y][p.x].tile_value = Empty;
		levelMatrix[p.y][p.x].foreground_tile_value = Empty;

		int i;
		for (i = 0; i < enemies.size(); i++)
		{
			if (p.x == enemies[i].initPos.x && p.y == enemies[i].initPos.y)
			{
				enemies.erase(enemies.begin() + i);
				i--;
			}
		}
		for (i = 0; i < springs.size(); i++)
		{
			if (p.x == springs[i].position.x && p.y == springs[i].position.y)
			{
				springs.erase(springs.begin() + i);
				i--;
			}
		}
		for (i = 0; i < movingTiles.size(); i++)
		{
			if (p.x == movingTiles[i].initPos.x && p.y == movingTiles[i].initPos.y)
			{
				movingTiles.erase(movingTiles.begin() + i);
				i--;
			}
		}
	}
	break;
	}
}
D2D1_POINT_2U LevelMap::GetTilePosition(D2D1_POINT_2U mousePos)
{
	return D2D1::Point2U(
		(UINT32)((mousePos.x + position.x) / TILE_SIZE),
		(UINT32)((mousePos.y + position.y + (TILE_SIZE / 5.0f)) / TILE_SIZE)
	);
}


void LevelMap::AddMovingTile(D2D1_POINT_2U tilePos)
{
	MovingTileData mt = {};

	mt.initPos = mt.finPos = { (long)tilePos.x, (long)tilePos.y };
	mt.tile_id = GlobalVariables::SelectedId;
	mt.used = false;

	movingTiles.push_back(mt);

	chooseFinalPosition = true;
	window->mouseButtonLeftPressed = false;
	//MessageBoxA(NULL, "now choose the final position", "", 0);
}

void LevelMap::AddEnemy(D2D1_POINT_2U tilePos)
{
	EnemyData e = {};

	e.initPos = e.finPos = { (long)tilePos.x, (long)tilePos.y };
	e.name = GlobalVariables::EnemyName;
	e.used = false;
	e.timeDown = 3;
	e.timeUp = 1;

	enemies.push_back(e);

	if (e.name != "barnacle")
	{
		chooseFinalPosition = true;
		window->mouseButtonLeftPressed = false;
		//MessageBoxA(NULL, "now choose the final position", "", 0);
	}
	else
	{
		enemies[enemies.size() - 1].used = true;
		chooseFinalPosition = false;
		GlobalVariables::SelectedType = Type::NONE;
		GlobalVariables::EnemyName.clear();
		window->mouseButtonLeftPressed = false;
	}
}


void LevelMap::SetPlayerInitialPosition(D2D1_POINT_2U mousePos)
{
	playerInitPos = GetTilePosition(mousePos);
}

void LevelMap::Load(wstring filename)
{
	// reset to default map values
	Clear();

	// Load map from file
	ifstream myfile(filename);
	if (myfile.is_open())
	{
		myfile >> lvlJson;
		myfile.close();


		int width = lvlJson["size"]["width"];
		int height = lvlJson["size"]["height"];

		if (lvlJson.contains("tiles-map"))
		{
			int i = 0;
			lvlJson["tiles-map"].list_iterate([&](const json& j) {
				levelMatrix[i / width][i % width].tile_value = j;
				i++;
				});
		}

		if (lvlJson.contains("foreground-tiles-map"))
		{
			int i = 0;
			lvlJson["foreground-tiles-map"].list_iterate([&](const json& j) {
				levelMatrix[i / width][i % width].foreground_tile_value = j;
				i++;
				});
		}

		if (lvlJson.contains("enemies"))
		{
			lvlJson["enemies"].list_iterate([&](const json& eJson) {
				EnemyData ed = {};

				ed.initPos = { (int)eJson["initial-position"][0], (int)eJson["initial-position"][1] };
				ed.finPos = { (int)eJson["final-position"][0], (int)eJson["final-position"][1] };
				ed.name = (string)eJson["name"];
				ed.used = true;

				if (ed.name == "snakeRed" || ed.name == "snakeGreen")
				{
					ed.timeUp = eJson["time-up"];
					ed.timeDown = eJson["time-down"];
				}
				else if (ed.name == "barnacle")
				{
					ed.timeDown = eJson["time-down"];
				}

				enemies.push_back(ed);
				});
		}

		if (lvlJson.contains("springboards"))
		{
			lvlJson["springboards"].list_iterate([&](const json& sJson) {
				SpringboardData sd = {};
				sd.position.x = sJson["x"];
				sd.position.y = sJson["y"];
				sd.force = sJson["force"];
				sd.used = true;
				springs.push_back(sd);
				});
		}

		if (lvlJson.contains("moving-tiles"))
		{
			lvlJson["moving-tiles"].list_iterate([&](const json& j) {
				MovingTileData mt = {};
				mt.initPos = { (int)j["initial-position"][0], (int)j["initial-position"][1] };
				mt.finPos = { (int)j["final-position"][0], (int)j["final-position"][1] };
				mt.tile_id = j["id"];
				mt.used = true;
				movingTiles.push_back(mt);
				});
		}

		if (lvlJson.contains("player"))
		{
			playerInitPos.x = (int)lvlJson["player"]["initial-position"][0];
			playerInitPos.y = (int)lvlJson["player"]["initial-position"][1];
		}

		if (lvlJson.contains("background-color"))
		{
			DWORD rgb = (unsigned long)(long long)lvlJson["background-color"];
			bgColor = D2D1::ColorF(
				GetBValue(rgb) / 255.0f,
				GetGValue(rgb) / 255.0f,
				GetRValue(rgb) / 255.0f
			);
		}
	}
}

bool hasEnding(string fullString, string ending)
{
	if (fullString.length() >= ending.length())
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	return false;
}

void LevelMap::Save(wstring filename)
{
	ofstream myfile(filename);
	if (myfile.is_open())
	{
		// tiles & size
		{
			int x = 0, y = 0, maxX = 0, maxY = 0;

			for (y = 0; y < LEVEL_HEIGHT; y++)
			{
				for (x = 0; x < LEVEL_WIDTH; x++)
				{
					if (levelMatrix[y][x].tile_value != TilesCodes::Empty)
					{
						maxX = max(maxX, x);
						maxY = max(maxY, y);
					}
				}
			}

			maxX += 1; // +1: because indexes starting from 0
			maxY += 1; // see above

			lvlJson["size"]["width"] = maxX;
			lvlJson["size"]["height"] = maxY;


			vector<json> tMp, fgMp;
			for (y = 0; y < maxY; y++)
			{
				for (x = 0; x < maxX; x++)
				{
					tMp.push_back(levelMatrix[y][x].tile_value);
					fgMp.push_back(levelMatrix[y][x].foreground_tile_value);
				}
			}
			lvlJson["tiles-map"] = tMp;
			lvlJson["foreground-tiles-map"] = fgMp;
		}

		// enemies
		{
			vector<json> ens;
			for (const EnemyData& ed : enemies)
			{
				if (ed.used)
				{
					json eJson;
					eJson["initial-position"] = { (int)ed.initPos.x, (int)ed.initPos.y };
					eJson["final-position"] = { (int)ed.finPos.x, (int)ed.finPos.y };

					string name(ed.name);
					if (hasEnding(name, "_1"))
						name = name.substr(0, name.length() - 2);
					eJson["name"] = name;
					if (name == "snakeRed" || name == "snakeGreen")
					{
						eJson["time-up"] = ed.timeUp;
						eJson["time-down"] = ed.timeDown;
					}
					else if (name == "barnacle")
					{
						eJson["time-down"] = 5; //ed.timeDown;
					}
					ens.push_back(eJson);
				}
			}
			lvlJson["enemies"] = ens;
		}

		// springboards
		{
			vector<json> spr;
			for (const SpringboardData& sd : springs)
			{
				if (sd.used)
				{
					json sJson;
					sJson["x"] = sd.position.x;
					sJson["y"] = sd.position.y;
					sJson["force"] = sd.force;
					spr.push_back(sJson);
				}
			}
			lvlJson["springboards"] = spr;
		}

		// moving tiles
		{
			vector<json> mTls;
			for (const MovingTileData& mt : movingTiles)
			{
				if (mt.used)
				{
					json mtJson;
					mtJson["initial-position"] = { (int)mt.initPos.x, (int)mt.initPos.y };
					mtJson["final-position"] = { (int)mt.finPos.x, (int)mt.finPos.y };
					mtJson["id"] = mt.tile_id;
					mTls.push_back(mtJson);
				}
			}
			lvlJson["moving-tiles"] = mTls;
		}

		// player
		{
			lvlJson["player"]["initial-position"] = { (int)playerInitPos.x, (int)playerInitPos.y };
		}

		// background-color
		{
			lvlJson["background-color"] = (long long)RGB(bgColor.b * 255, bgColor.g * 255, bgColor.r * 255);
		}


		myfile << lvlJson;
		myfile.close();

		MessageBox(nullptr, (L"The map has been successfully saved at \"" + filename + L"\".").c_str(), L"", MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		MessageBox(nullptr, (L"The map could not be saved now at \"" + filename + L"\".").c_str(), L"", MB_ICONERROR | MB_OK);
	}
}

void LevelMap::Clear()
{
	// reset to default map values

	levelMatrix.clear();
	for (int j, i = 0; i < LEVEL_HEIGHT; i++)
	{
		levelMatrix.push_back(vector<LevelCell>());
		for (j = 0; j < LEVEL_WIDTH; j++)
		{
			levelMatrix[i].push_back({ Empty ,Empty });
		}
	}
	
	chooseFinalPosition = false;
	enemies.clear();
	springs.clear();
	movingTiles.clear();
	lvlJson.clear();
	playerInitPos = { 0,0 };
	position = { 0,0 };
}

bool LevelMap::InCameraFrame(D2D1_RECT_F rc)
{
	return position.x <= rc.right && rc.left < position.x + (float)wndSize.width
		&& position.y <= rc.bottom && rc.top < position.y + (float)wndSize.height;
}

void LevelMap::RemoveOffset(D2D1_RECT_F& rc)
{
	rc.left -= position.x;
	rc.right -= position.x;
	rc.top -= position.y;
	rc.bottom -= position.y;
}
