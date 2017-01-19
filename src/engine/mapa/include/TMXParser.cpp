#include "TMXParser.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

namespace TMX {

  Parser::Parser( const char* filename )
  {
    load( filename );
  }

  Parser::Parser()
  {
  }

  Parser::~Parser()
  {
  }

  bool Parser::load( const char* filename )
  {
    std::string version = VERSION;
    rapidxml::xml_node<>* root_node;
    rapidxml::xml_document<> doc;
    rapidxml::file<> file( filename );
    doc.parse<0>( file.data() );
    //get root nodes
    root_node = doc.first_node( "map" );

    //load map element
    if( root_node->first_attribute( "version" )->value() != version ) {
      //std::cout << "ERROR: Map version mismatch. Required version: " << VERSION << "." << std::endl;
      return false;
    }

    mapInfo.version = root_node->first_attribute( "version" )->value();
    //std::cout << "Version: " << mapInfo.version << std::endl;
    mapInfo.orientation = root_node->first_attribute( "orientation" )->value();
    //std::cout << "Orientation: " << mapInfo.orientation << std::endl;
    mapInfo.width = std::atoi( root_node->first_attribute( "width" )->value() );
    //std::cout << "stWidth: " << mapInfo.width << std::endl;
    mapInfo.height = std::atoi( root_node->first_attribute( "height" )->value() );
    //std::cout << "Height: " << mapInfo.height << std::endl;
    mapInfo.tileWidth = std::atoi( root_node->first_attribute( "tilewidth" )->value() );
    //std::cout << "Tile stWidth: " << mapInfo.mTileWidth << std::endl;
    mapInfo.tileHeight = std::atoi( root_node->first_attribute( "tileheight" )->value() );
    //std::cout << "Tile Height: " << mapInfo.mTileHeight << std::endl;

    auto background_color = root_node->first_attribute( "backgroundcolor" );
    if( background_color && background_color->value() != 0 ) {
      mapInfo.backgroundColor = background_color->value();
      //std::cout << "Background Color: " << mapInfo.backgroundColor << std::endl;
    }

    if( root_node->first_node( "properties" ) != 0 ) {
      for( rapidxml::xml_node<>* properties_node = root_node->first_node( "properties" )->first_node( "property" ); properties_node; properties_node = properties_node->next_sibling() ) {
        mapInfo.property[properties_node->first_attribute( "name" )->value()] = properties_node->first_attribute( "value" )->value();
      }

      //std::cout << "Properties: " << std::endl;

      for( std::map<std::string, std::string>::iterator it = mapInfo.property.begin(); it != mapInfo.property.end(); ++it ) {
        //std::cout << "-> " << it->first << " : " << it->second << std::endl;
      }
    }

    //std::cout << std::endl;


    for( rapidxml::xml_node<>* tileset_node = root_node->first_node( "tileset" ); tileset_node; tileset_node = tileset_node->next_sibling( "tileset" ) ) {
        Tileset tmpTileset;
        tmpTileset.firstGID = (unsigned int) std::atoi(tileset_node->first_attribute("firstgid" )->value() );
        tmpTileset.name = tileset_node->first_attribute( "name" )->value();
        tmpTileset.tileWidth = (unsigned int) std::stoi(tileset_node->first_attribute("tilewidth" )->value());
        tmpTileset.tileHeight = (unsigned int) std::stoi(tileset_node->first_attribute("tileheight" )->value());
        tmpTileset.columns = (unsigned int) std::stoi(tileset_node->first_attribute("columns" )->value());
        tmpTileset.tileCount = (unsigned int) std::stoi(tileset_node->first_attribute("tilecount" )->value());

        tmpTileset.imgSource.source = tileset_node->first_node("image" )->first_attribute("source" )->value();
        tmpTileset.imgSource.width = (unsigned int) std::stoi(tileset_node->first_node("image" )->first_attribute("width" )->value());
        tmpTileset.imgSource.height = (unsigned int) std::stoi(tileset_node->first_node("image" )->first_attribute("height" )->value());

        for( auto tiles_node = tileset_node->first_node( "tile" ); tiles_node; tiles_node = tiles_node->next_sibling() ) {

            Tile tmpTile;
          if( tiles_node->first_node( "properties" ) != 0 ) {
            for( rapidxml::xml_node<>* properties_node = tiles_node->first_node( "properties" )->first_node( "property" ); properties_node; properties_node = properties_node->next_sibling() ) {
              tmpTile.property[properties_node->first_attribute( "name" )->value()] = properties_node->first_attribute( "value" )->value();
            }
          }
          if( tiles_node->first_node( "animation" ) != 0 ) {
            for( rapidxml::xml_node<>* frame_node = tiles_node->first_node( "animation" )->first_node( "frame" ); frame_node; frame_node = frame_node->next_sibling() ) {
              tmpTile.animation.push_back(frame_node->first_attribute( "tileid" )->value());
            }
          }
          tmpTileset.tilesMetaData[tiles_node->first_attribute( "id" )->value()] = tmpTile;

        }
        /*//std::cout << "Tileset[ First GID: " << tmpTileset.firstGID
                << " name: " << tmpTileset.name
                << " mTileWidth: " << tmpTileset.mTileWidth
                << " mTileHeight: " << tmpTileset.mTileHeight
                << " columns: " << tmpTileset.columns
                << std::endl;*/
        tilesetList.push_back( tmpTileset );
    }

    for( rapidxml::xml_node<>* layer_node = root_node->first_node( "layer" ); layer_node; layer_node = layer_node->next_sibling( "layer" ) ) {
      TileLayer layer;
        layer.name = layer_node->first_attribute( "name" )->value();
        //std::cout << std::endl << "Layer Name: " << layer.name << std::endl;
        layer.width = (unsigned int) std::stoi(layer_node->first_attribute("width" )->value());
        //std::cout << std::endl << "Layer Name: " << layer.name << std::endl;
        layer.height = (unsigned int) std::stoi(layer_node->first_attribute("height" )->value());
        //std::cout << std::endl << "Layer Name: " << layer.name << std::endl;

      if( layer_node->first_node( "properties" ) != 0 ) {
        for( rapidxml::xml_node<>* properties_node = layer_node->first_node( "properties" )->first_node( "property" ); properties_node; properties_node = properties_node->next_sibling() ) {
          layer.property[properties_node->first_attribute( "name" )->value()] = properties_node->first_attribute( "value" )->value();
        }

        //std::cout << "Properties: " << std::endl;

        for( std::map<std::string, std::string>::iterator it = mapInfo.property.begin(); it != mapInfo.property.end(); ++it ) {
          //std::cout << "-> " << it->first << " : " << it->second << std::endl;
        }
      }

      rapidxml::xml_node<>* data_node = layer_node->first_node( "data" );
      layer.data.encoding = data_node->first_attribute( "encoding" )->value();
      //std::cout << "Layer Encoding: " << layer.data.encoding << std::endl;

      if( data_node->first_attribute( "compression" ) > 0 ) {
        layer.data.compression = data_node->first_attribute( "compression" )->value();
        //std::cout << "Layer Compression: " << layer.data.compression << std::endl;
      }

      layer.data.contents = data_node->value();
      //std::cout << "Layer Data: " << layer.data.contents << std::endl;
      tileLayer[layer.name] = layer;
    }

    for( rapidxml::xml_node<>* oGroup_node = root_node->first_node( "objectgroup" ); oGroup_node; oGroup_node = oGroup_node->next_sibling( "objectgroup" ) ) {
      ObjectGroup oGroup;
      //std::cout << std::endl;
//      if(oGroup_node->first_attribute( "color" ))
  //      oGroup.color = oGroup_node->first_attribute( "color" )->value();
      //std::cout << "Object Group Color: " << oGroup.color << std::endl;
      if(oGroup_node->first_attribute( "name" ))
        oGroup.name = oGroup_node->first_attribute( "name" )->value();
      //std::cout << "Object Group Name: " << oGroup.name << std::endl;
//      if(oGroup_node->first_attribute( "opacity" ))
//        oGroup.opacity = std::atof( oGroup_node->first_attribute( "opacity" )->value() );
//      //std::cout << "Object Group Opacity: " << oGroup.opacity << std::endl;
//      if(oGroup_node->first_attribute( "visible" ))
//        oGroup.visible = std::atoi( oGroup_node->first_attribute( "visible" )->value() );
      //std::cout << "Object Group Visible: " << oGroup.visible << std::endl;

      if( oGroup_node->first_node( "properties" ) != 0 ) {
        for( rapidxml::xml_node<>* properties_node = oGroup_node->first_node( "properties" )->first_node( "property" ); properties_node; properties_node = properties_node->next_sibling() ) {
          oGroup.property[properties_node->first_attribute( "name" )->value()] = properties_node->first_attribute( "value" )->value();
        }

        //std::cout << "Properties: " << std::endl;

        for( std::map<std::string, std::string>::iterator it = mapInfo.property.begin(); it != mapInfo.property.end(); ++it ) {
          //std::cout << "-> " << it->first << " : " << it->second << std::endl;
        }
      }

      for( rapidxml::xml_node<>* objects_node = oGroup_node->first_node( "object" ); objects_node; objects_node = objects_node->next_sibling() ) {
        Object object;
        object.name = objects_node->first_attribute( "name" )->value();
        object.type = objects_node->first_attribute( "type" )->value();
        object.x = std::stoi(objects_node->first_attribute( "x" )->value());
        object.y = std::stoi(objects_node->first_attribute( "y" )->value());
        oGroup.object[object.name] = object;
      }


      objectGroup[oGroup.name] = oGroup;
    }

    for( rapidxml::xml_node<>* image_node = root_node->first_node( "imagelayer" ); image_node; image_node = image_node->next_sibling( "imagelayer" ) ) {
      ImageLayer imgLayer;
      //std::cout << std::endl;
      imgLayer.name = image_node->first_attribute( "name" )->value();
      //std::cout << "Image Layer Name: " << imgLayer.name << std::endl;

      if( image_node->first_attribute( "opacity" ) != 0 ) {
        imgLayer.opacity = std::atof( image_node->first_attribute( "opacity" )->value() );
        //std::cout << "Image Layer Opacity: " << imgLayer.opacity << std::endl;
      }

      imgLayer.visible = std::atoi( image_node->first_attribute( "visible" )->value() );
      //std::cout << "Image Layer Visibility: " << imgLayer.visible << std::endl;

      imgLayer.image.source = image_node->first_node( "image" )->first_attribute( "source" )->value();
      //std::cout << "Image Layer Source: " << imgLayer.visible << std::endl;

      if( image_node->first_node( "image" )->first_attribute( "trans" ) != 0 ) {
        imgLayer.image.transparencyColor = image_node->first_node( "image" )->first_attribute( "trans" )->value();
        //std::cout << "Image Layer Transparent Color: " << imgLayer.image.transparencyColor << std::endl;
      }

      if( image_node->first_node( "properties" ) != 0 ) {
        for( rapidxml::xml_node<>* properties_node = image_node->first_node( "properties" )->first_node( "property" ); properties_node; properties_node = properties_node->next_sibling( "property" ) ) {
          imgLayer.property[properties_node->first_attribute( "name" )->value()] = properties_node->first_attribute( "value" )->value();
        }

        //std::cout << "Properties: " << std::endl;

        for( std::map<std::string, std::string>::iterator it = imgLayer.property.begin(); it != imgLayer.property.end(); ++it ) {
          //std::cout << "-> " << it->first << " : " << it->second << std::endl;
        }
      }

      imageLayer[imgLayer.name] = imgLayer;
    }

    return true;
  }
}
