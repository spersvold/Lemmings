#include "Map.h"


Map::Map(){
	fileMap = "";
	idFileMap = NULL;
	fileTileset = "";
	idFileTileset = NULL;
	widthMap = 0;
	sizeTiles = 0;
	tManager = tinyManager::getInstanceTinyManager();
}

Map::~Map(){

}


void Map::init(int x, int y, const char* fileMap, const char* layerCollision, int numLayers, const char* fileTileset, bool haveSpacingTileset, int numTilesets, int srcX, int srcY, int w, int h){
	Element::init(x, y, fileMap, srcX, srcY, w, h);

	tManager->LoadTmx(fileMap, layerCollision);
	this->fileTileset = fileTileset;
	idFileTileset = sManager->getVideoManager()->getTextureID(fileTileset);
	if (fileTileset != NULL){
		for (int i = 0; i < numLayers; i++){
			vector <vector<int> > mapa = tManager->LoadMap(i);
			if (mapa.size() > 0){
				for (int i = 0; i < numTilesets; i++)
					tilesets.push_back(tManager->LoadTileset(i, haveSpacingTileset, mapa));
			}
		}
		mapCollision = tManager->LoadMapCollision();
		widthMap = tManager->GetWidthMap();
		sizeTiles = tManager->GetTileSize();
	}
	else{
		this->fileMap = fileMap;
		idFileMap = sManager->getVideoManager()->getTextureID(fileMap);
	}

	tManager->DestroyTMX();

}


void Map::render(){
	if (fileTileset != NULL){
		VideoManager* video = sManager->getVideoManager();
		for (itTilesets = tilesets.begin(); itTilesets != tilesets.end(); itTilesets++){
			vector<tinyManager::Tileset::Tile*> tiles = itTilesets->getTiles();
			for (int pos = 0; pos < itTilesets->getSizeTiles(); pos++)
				video->renderTexture(idFileTileset, tiles[pos]->getSrcPosX(), tiles[pos]->getSrcPosY(), tiles[pos]->getTileWidth(), tiles[pos]->getTileHeight(), tiles[pos]->getDstPosX() - posX, tiles[pos]->getDstPosY() - posY, 0, 0, 0);
		}
	}
	else
		sManager->getVideoManager()->renderTexture(idFileMap, 0, 0, width, height, posX, posY, 0, 0, 0);
}


int Map::GetMapa(int x, int y){
	return mapCollision[y][x];
}

int Map::GetSizeTile(){
	return sizeTiles;
}